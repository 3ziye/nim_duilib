#ifndef UI_UTILS_PROCESS_SINGLETON_WINDOWS_H_
#define UI_UTILS_PROCESS_SINGLETON_WINDOWS_H_

#include "ProcessSingletonData.h"

#ifdef DUILIB_BUILD_FOR_WIN

#include <wincrypt.h>
#include <sddl.h>
#include <userenv.h>

namespace ui
{
/** 跨进程单例的实现（通信部分, Windows实现）
*/
class UILIB_API ProcessSingletonImpl : public ProcessSingleton
{
public:
    explicit ProcessSingletonImpl(const std::string& strAppName) :
        ProcessSingleton(strAppName)
    {
        InitializePlatformComponents();
    }

    virtual ~ProcessSingletonImpl() override
    {
        m_bRunning = false;
        CleanupPlatformComponents();
    }

protected:
    ProcessSingletonImpl(const ProcessSingleton&) = delete;
    ProcessSingletonImpl& operator=(const ProcessSingletonImpl&) = delete;

public:
    virtual void InitializePlatformComponents() override final
    {
        try {
            SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
            sa.bInheritHandle = FALSE;

            // 设置低完整性级别
            ::ConvertStringSecurityDescriptorToSecurityDescriptorW(L"S:(ML;;NW;;;LW)", SDDL_REVISION_1, &sa.lpSecurityDescriptor, nullptr);
            std::wstring wstrMutexName = GetUserSpecificName();
            m_hMutex = ::CreateMutexW(&sa, FALSE, wstrMutexName.c_str());
            if (!m_hMutex) {
                ::LocalFree(sa.lpSecurityDescriptor);
                throw std::system_error(GetLastError(), std::system_category(), "CreateMutex failed");
            }
            ::LocalFree(sa.lpSecurityDescriptor);
        }
        catch (const std::exception& /*ex*/) {
            CleanupPlatformComponents();
            throw;
        }
    }

    virtual bool PlatformCheckInstance() override final
    {
        return ::GetLastError() == ERROR_ALREADY_EXISTS;
    }

    virtual bool PlatformSendData(const std::string& strData) override final
    {
        try {
            std::wstring wstrPipeName = L"\\\\.\\pipe\\" + GetUserSpecificName();
            HANDLE hPipe = ::CreateFileW(wstrPipeName.c_str(),
                                        GENERIC_WRITE,
                                        0,
                                        nullptr,
                                        OPEN_EXISTING,
                                        FILE_FLAG_OVERLAPPED,
                                        nullptr);
            if (hPipe == INVALID_HANDLE_VALUE) {
                return false;
            }

            OVERLAPPED overlapped = { 0 };
            overlapped.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

            DWORD dwWritten = 0;
            if (!::WriteFile(hPipe, strData.data(), static_cast<DWORD>(strData.size()), &dwWritten, &overlapped)) {
                if (::GetLastError() == ERROR_IO_PENDING) {
                    ::WaitForSingleObject(overlapped.hEvent, INFINITE);
                }
                else {
                    ::CloseHandle(hPipe);
                    ::CloseHandle(overlapped.hEvent);
                    return false;
                }
            }

            ::CloseHandle(hPipe);
            ::CloseHandle(overlapped.hEvent);
            return true;
        }
        catch (const std::exception& ex) {
            LogError("Windows send error: " + std::string(ex.what()));
            return false;
        }
    }

    virtual void PlatformListen() override final
    {
        while (m_bRunning) {
            try {
                std::wstring wstrPipeName = L"\\\\.\\pipe\\" + GetUserSpecificName();
                m_hPipe = ::CreateNamedPipeW(wstrPipeName.c_str(),
                                             PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
                                             PIPE_TYPE_BYTE | PIPE_WAIT,
                                             PIPE_UNLIMITED_INSTANCES,
                                             ProcessSingletonData::MAX_DATA_SIZE + sizeof(ProcessSingletonData::ProtocolHeader),
                                             ProcessSingletonData::MAX_DATA_SIZE + sizeof(ProcessSingletonData::ProtocolHeader),
                                             0,
                                             nullptr);

                if (m_hPipe == INVALID_HANDLE_VALUE) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }

                OVERLAPPED overlapped = { 0 };
                overlapped.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

                if (!::ConnectNamedPipe(m_hPipe, &overlapped)) {
                    if (::GetLastError() == ERROR_IO_PENDING) {
                        ::WaitForSingleObject(overlapped.hEvent, INFINITE);
                    }
                    else {
                        ::CloseHandle(m_hPipe);
                        m_hPipe = INVALID_HANDLE_VALUE;
                        continue;
                    }
                }

                char pBuffer[ProcessSingletonData::MAX_DATA_SIZE + sizeof(ProcessSingletonData::ProtocolHeader)] = { 0 };
                DWORD dwRead = 0;
                if (::ReadFile(m_hPipe, pBuffer, sizeof(pBuffer), &dwRead, &overlapped)) {
                    try {
                        auto vecArgs = ProcessSingletonData::DeserializeData(std::string(pBuffer, dwRead));
                        OnAlreadyRunningAppRelaunch(vecArgs);
                    }
                    catch (const std::exception& ex) {
                        LogError("Invalid data received: " + std::string(ex.what()));
                    }
                }

                ::DisconnectNamedPipe(m_hPipe);
                ::CloseHandle(m_hPipe);
                ::CloseHandle(overlapped.hEvent);
                m_hPipe = INVALID_HANDLE_VALUE;
            }
            catch (const std::exception& ex) {
                LogError("Windows listener error: " + std::string(ex.what()));
            }
        }
    }
    
    virtual void CleanupPlatformComponents() override final
    {
        if (m_hMutex) {
            ::CloseHandle(m_hMutex);
            m_hMutex = nullptr;
        }
        if (m_hPipe != INVALID_HANDLE_VALUE) {
            ::CloseHandle(m_hPipe);
            m_hPipe = INVALID_HANDLE_VALUE;
        }
        
        if (m_thListener.joinable()) {
            m_thListener.join();
        }
    }
    
private:
    std::wstring GetUserSpecificName() 
    {
        HANDLE hToken = nullptr;
        if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken)) {
            throw std::system_error(GetLastError(), std::system_category(), "OpenProcessToken failed");
        }

        DWORD dwSize = 0;
        SECURITY_INFORMATION siRequested = OWNER_SECURITY_INFORMATION;
        ::GetUserObjectSecurity(hToken, &siRequested, nullptr, 0, &dwSize);
        if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            ::CloseHandle(hToken);
            throw std::system_error(GetLastError(), std::system_category(), "GetUserObjectSecurity failed");
        }

        std::vector<BYTE> vecBuffer(dwSize);
        PSECURITY_DESCRIPTOR pSD = reinterpret_cast<PSECURITY_DESCRIPTOR>(vecBuffer.data());
        siRequested = OWNER_SECURITY_INFORMATION;
        if (!::GetUserObjectSecurity(hToken, &siRequested, pSD, dwSize, &dwSize)) {
            ::CloseHandle(hToken);
            throw std::system_error(GetLastError(), std::system_category(), "GetUserObjectSecurity failed");
        }

        PSID pSid = nullptr;
        BOOL bOwnerDefaulted = FALSE;
        if (!::GetSecurityDescriptorOwner(pSD, &pSid, &bOwnerDefaulted) || pSid == nullptr) {
            ::CloseHandle(hToken);
            throw std::runtime_error("GetSecurityDescriptorOwner failed");
        }

        LPWSTR pszSid = nullptr;
        if (!::ConvertSidToStringSidW(pSid, &pszSid)) {
            ::CloseHandle(hToken);
            throw std::system_error(GetLastError(), std::system_category(), "ConvertSidToStringSid failed");
        }

        std::wstring wstrSid(pszSid);
        ::LocalFree(pszSid);
        ::CloseHandle(hToken);

        return L"Local\\" + std::wstring(m_strAppName.begin(), m_strAppName.end()) + L"_" + wstrSid;
    }

private:
    // Windows实现
    HANDLE m_hMutex = nullptr;
    HANDLE m_hPipe = INVALID_HANDLE_VALUE;
};

}

#endif //DUILIB_BUILD_FOR_WIN

#endif // UI_UTILS_PROCESS_SINGLETON_WINDOWS_H_
