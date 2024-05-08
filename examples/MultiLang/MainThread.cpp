//MainThread.cpp
#include "MainThread.h"
#include "MainForm.h"
#include "resource.h"

WorkerThread::WorkerThread(ThreadId threadID, const char* name)
    : FrameworkThread(name)
    , m_threadID(threadID)
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::Init()
{
    ::OleInitialize(nullptr);
    nbase::ThreadManager::RegisterThread(m_threadID);
}

void WorkerThread::Cleanup()
{
    nbase::ThreadManager::UnregisterThread();
    ::OleUninitialize();
}

MainThread::MainThread() :
    nbase::FrameworkThread("MainThread")
{
}

MainThread::~MainThread()
{
}

void MainThread::Init()
{
    ::OleInitialize(nullptr);
    nbase::ThreadManager::RegisterThread(kThreadUI);

    //���������߳�
    m_workerThread.reset(new WorkerThread(kThreadWorker, "WorkerThread"));
    m_workerThread->Start();

	//��ʼ��ȫ����Դ
	constexpr ui::ResourceType resType = ui::ResourceType::kLocalFiles;
	if (resType == ui::ResourceType::kLocalFiles) {
		//ʹ�ñ����ļ�����Ϊ��Դ
		std::wstring resourcePath = nbase::win32::GetCurrentModuleDirectory();
		resourcePath += L"resources\\";
		ui::GlobalManager::Instance().Startup(ui::LocalFilesResParam(resourcePath));
	}
	else if (resType == ui::ResourceType::kZipFile) {
		//ʹ�ñ���zipѹ������Ϊ��Դ��ѹ����λ��exe��ͬĿ¼��	
		ui::ZipFileResParam resParam;
		resParam.resourcePath = L"resources\\";
		resParam.zipFilePath = nbase::win32::GetCurrentModuleDirectory();
		resParam.zipFilePath += L"resources.zip";
		resParam.zipPassword = "";
		ui::GlobalManager::Instance().Startup(resParam);
	}
	else if (resType == ui::ResourceType::kResZipFile) {
		//ʹ��exe��Դ�ļ��е�zipѹ����
		ui::ResZipFileResParam resParam;
		resParam.resourcePath = L"resources\\";
		resParam.hResModule = nullptr;
		resParam.resourceName = MAKEINTRESOURCE(IDR_THEME);
		resParam.resourceType = L"THEME";
		resParam.zipPassword = "";
		ui::GlobalManager::Instance().Startup(resParam);
	}
	else {
		return;
	}

    //����������������ڴ���
    //����һ��Ĭ�ϴ�����Ӱ�ľ��д���
    MainForm* window = new MainForm();
    window->CreateWnd(nullptr, MainForm::kClassName.c_str(), UI_WNDSTYLE_FRAME, WS_EX_LAYERED);
    window->CenterWindow();
    window->ShowWindow();
}

void MainThread::Cleanup()
{
    ui::GlobalManager::Instance().Shutdown();
    if (m_workerThread != nullptr) {
        m_workerThread->Stop();
        m_workerThread.reset(nullptr);
    }
    SetThreadWasQuitProperly(true);
    nbase::ThreadManager::UnregisterThread();
    ::OleUninitialize();
}