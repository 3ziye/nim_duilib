#include "DpiAwareness.h"
#include "duilib/Utils/ApiWrapper.h"
#include <VersionHelpers.h>

namespace ui
{
DpiInitParam::DpiInitParam():
	m_dpiAwarenessFlag(DpiInitParam::DpiAwarenessFlag::kFromUserDefine),
	m_dpiAwarenessMode(DpiAwarenessMode::kPerMonitorDpiAware_V2),
	m_dpiFlag(DpiInitParam::DpiFlag::kFromSystem),
	m_uDPI(96)
{
}

DpiAwareness::DpiAwareness()
{
}

DpiAwareness::~DpiAwareness()
{
}

bool DpiAwareness::InitDpiAwareness(const DpiInitParam& initParam)
{
	bool bRet = true;
	if (initParam.m_dpiAwarenessFlag == DpiInitParam::DpiAwarenessFlag::kFromUserDefine) {
		//����һ�� Dpi Awareness
		SetDpiAwareness(initParam.m_dpiAwarenessMode);
		DpiAwarenessMode dpiAwarenessMode = GetDpiAwareness();
		if (initParam.m_dpiAwarenessMode == DpiAwarenessMode::kDpiUnaware) {
			bRet = (dpiAwarenessMode == DpiAwarenessMode::kDpiUnaware) ? true : false;
		}
		else {
			bRet = (dpiAwarenessMode != DpiAwarenessMode::kDpiUnaware) ? true : false;
		}
	}
	return bRet;
}

DpiAwarenessMode DpiAwareness::SetDpiAwareness(DpiAwarenessMode dpiAwarenessMode) const
{
	if (!::IsWindowsVistaOrGreater()) {
		//Vista���°汾ϵͳ����֧��DPI��֪
		return DpiAwarenessMode::kDpiUnaware;
	}

	//˵�������Ӧ�ó��� (.exe) �嵥���� DPI ��֪������ص�����API�����ʧ��
	//     �����ǰ���û�һ��ִ�к�������ڶ��ε��õ�ʱ���ʧ�ܣ�Ӧ�������ƣ�ֻ��������һ�Σ�
	if (dpiAwarenessMode != DpiAwarenessMode::kDpiUnaware) {
		bool bSetOk = false;
		if (!bSetOk && ::IsWindows10OrGreater()) {
			//Windows10 ������
			PROCESS_DPI_AWARENESS_CONTEXT newValueWin10 = PROCESS_DPI_AWARENESS_CONTEXT_UNAWARE;
			if (dpiAwarenessMode == DpiAwarenessMode::kPerMonitorDpiAware_V2) {
				newValueWin10 = PROCESS_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2;
			}
			else if (dpiAwarenessMode == DpiAwarenessMode::kPerMonitorDpiAware) {
				newValueWin10 = PROCESS_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE;
			}
			else {
				newValueWin10 = PROCESS_DPI_AWARENESS_CONTEXT_SYSTEM_AWARE;
			}
			PROCESS_DPI_AWARENESS_CONTEXT oldValueWin10 = PROCESS_DPI_AWARENESS_CONTEXT_UNAWARE;
			if (GetProcessDpiAwarenessContextWrapper(oldValueWin10)) {
				if (AreDpiAwarenessContextsEqualWrapper(oldValueWin10, newValueWin10)) {
					bSetOk = true;
				}
				if (!bSetOk && SetProcessDpiAwarenessContextWrapper(newValueWin10)) {
					bSetOk = true;
				}
				if (!bSetOk && (dpiAwarenessMode == DpiAwarenessMode::kPerMonitorDpiAware_V2)) {
					newValueWin10 = PROCESS_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE;
					if (SetProcessDpiAwarenessContextWrapper(newValueWin10)) {
						bSetOk = true;
					}
				}
			}
		}

		if (!bSetOk && ::IsWindows8Point1OrGreater()) {
			//Win8.1 ������
			PROCESS_DPI_AWARENESS newValueWin8 = PROCESS_DPI_UNAWARE;
			if ((dpiAwarenessMode == DpiAwarenessMode::kPerMonitorDpiAware) || 
				(dpiAwarenessMode == DpiAwarenessMode::kPerMonitorDpiAware_V2) ) {
				newValueWin8 = PROCESS_PER_MONITOR_DPI_AWARE;
			}
			else {
				newValueWin8 = PROCESS_SYSTEM_DPI_AWARE;
			}
			PROCESS_DPI_AWARENESS oldValueWin8 = PROCESS_DPI_UNAWARE;
			if (GetProcessDPIAwarenessWrapper(oldValueWin8)) {
				if (oldValueWin8 == newValueWin8) {
					bSetOk = true;
				}
				if (!bSetOk && SetProcessDPIAwarenessWrapper(newValueWin8)) {
					bSetOk = true;
				}
			}
		}

		if (!bSetOk) {
			bool bAware = false;
			if (IsProcessDPIAwareWrapper(bAware)) {
				if (bAware) {
					bSetOk = true;
				}
			}
			if (!bSetOk && SetProcessDPIAwareWrapper()) {
				bSetOk = true;
			}
		}
	}
	return GetDpiAwareness();
}

DpiAwarenessMode DpiAwareness::GetDpiAwareness() const
{
	DpiAwarenessMode dpiAwarenessMode = DpiAwarenessMode::kDpiUnaware;
	if (!::IsWindowsVistaOrGreater()) {
		//Vista���°汾ϵͳ����֧��DPI��֪
		return dpiAwarenessMode;
	}
	bool bDpiInited = false;
	if (!bDpiInited && ::IsWindows10OrGreater()) {
		//Windows10 ������
		PROCESS_DPI_AWARENESS_CONTEXT value = PROCESS_DPI_AWARENESS_CONTEXT_UNAWARE;
		if (GetProcessDpiAwarenessContextWrapper(value)) {
			bDpiInited = true;
			if (AreDpiAwarenessContextsEqualWrapper(value, PROCESS_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
				dpiAwarenessMode = DpiAwarenessMode::kPerMonitorDpiAware_V2;
			}
			else if (AreDpiAwarenessContextsEqualWrapper(value, PROCESS_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) {
				dpiAwarenessMode = DpiAwarenessMode::kPerMonitorDpiAware;
			}
			else if (AreDpiAwarenessContextsEqualWrapper(value, PROCESS_DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {
				dpiAwarenessMode = DpiAwarenessMode::kSystemDpiAware;
			}
			else if (AreDpiAwarenessContextsEqualWrapper(value, PROCESS_DPI_AWARENESS_CONTEXT_UNAWARE)) {
				dpiAwarenessMode = DpiAwarenessMode::kDpiUnaware;
			}
			else {
				dpiAwarenessMode = DpiAwarenessMode::kDpiUnaware;
			}
		}
	}
	if (!bDpiInited && ::IsWindows8Point1OrGreater()) {
		//Win8.1 ������
		PROCESS_DPI_AWARENESS value = PROCESS_DPI_UNAWARE;
		if (GetProcessDPIAwarenessWrapper(value)) {
			bDpiInited = true;
			if (value == PROCESS_PER_MONITOR_DPI_AWARE) {
				dpiAwarenessMode = DpiAwarenessMode::kPerMonitorDpiAware;
			}
			else if (value == PROCESS_SYSTEM_DPI_AWARE) {
				dpiAwarenessMode = DpiAwarenessMode::kSystemDpiAware;
			}
			else {
				dpiAwarenessMode = DpiAwarenessMode::kDpiUnaware;
			}
		}
	}
	if (!bDpiInited) {
		bool bAware = false;
		if (IsProcessDPIAwareWrapper(bAware)) {
			bDpiInited = true;
			if (bAware) {
				dpiAwarenessMode = DpiAwarenessMode::kSystemDpiAware;
			}
			else {
				dpiAwarenessMode = DpiAwarenessMode::kDpiUnaware;
			}
		}
	}
	return dpiAwarenessMode;
}

}