#include "DpiManager.h"
#include "duilib/Utils/VersionHelpers.h"
#include "duilib/Utils/ApiWrapper.h"

namespace ui
{

DpiManager* DpiManager::GetInstance()
{
	static DpiManager dpiManager;
	return &dpiManager;
}

DpiManager::DpiManager()
{
	m_nScaleFactor = 100;
	m_bAdaptDPI = false;
}

uint32_t DpiManager::GetMonitorDPI(HMONITOR hMonitor)
{
	uint32_t dpix = 96;
	uint32_t dpiy = 96;
	if (IsWindows8OrGreater()) {
		if (!GetDpiForMonitorWrapper(hMonitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy)) {
			dpix = 96;
		}
	}
	else {
		HDC desktopDc = GetDC(NULL);
		dpix = (uint32_t)GetDeviceCaps(desktopDc, LOGPIXELSX);
		ReleaseDC(0, desktopDc);
	}
	return dpix;
}

uint32_t DpiManager::GetMainMonitorDPI()
{
	POINT pt = { 1, 1 };
	HMONITOR hMonitor;
	hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	return GetMonitorDPI(hMonitor);
}

bool DpiManager::IsAdaptDPI() const
{
	return m_bAdaptDPI;
}

bool DpiManager::SetAdaptDPI(bool bAdaptDPI)
{
	m_bAdaptDPI = bAdaptDPI;
	if (!IsWindowsVistaOrGreater()) {		
		return true;
	}

	//˵�������Ӧ�ó��� (.exe) �嵥���� DPI ��֪����API�����ʧ��
	//     �����ǰ���û�һ��ִ�к�������ڶ��ε��õ�ʱ���ʧ�ܣ�Ӧ�������ƣ�ֻ��������һ�Σ�
	bool isOk = false;
	PROCESS_DPI_AWARENESS_CONTEXT new_value_win10 = bAdaptDPI ? PROCESS_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 : PROCESS_DPI_AWARENESS_CONTEXT_UNAWARE;
	PROCESS_DPI_AWARENESS_CONTEXT old_value_win10 = PROCESS_DPI_AWARENESS_CONTEXT_UNAWARE;		
	if (GetProcessDpiAwarenessContextWrapper(old_value_win10)) {
		if (AreDpiAwarenessContextsEqualWrapper(old_value_win10, new_value_win10)) {
			isOk = true;
		}
		if (!isOk && SetProcessDpiAwarenessContextWrapper(new_value_win10)) {
			isOk = true;
		}
	}

	if (!isOk) {
		PROCESS_DPI_AWARENESS new_value_win8 = bAdaptDPI ? PROCESS_PER_MONITOR_DPI_AWARE : PROCESS_DPI_UNAWARE;
		PROCESS_DPI_AWARENESS old_value_win8 = PROCESS_DPI_UNAWARE;
		if (GetProcessDPIAwarenessWrapper(old_value_win8)) {
			if (old_value_win8 == new_value_win8) {
				isOk = true;
			}
			if (!isOk && SetProcessDPIAwarenessWrapper(new_value_win8)) {
				isOk = true;
			}
		}
	}
	
	if (!isOk) {
		bool bAware = false;
		if (IsProcessDPIAwareWrapper(bAware)) {
			if (bAware == bAdaptDPI) {
				isOk = true;
			}
		}
		if (!isOk && SetProcessDPIAwareWrapper()) {
			isOk = true;
		}
	}
	return isOk;
}

uint32_t DpiManager::GetScale() const
{
	if (!m_bAdaptDPI) {
		return 100;
	}
	return m_nScaleFactor;
}

void DpiManager::SetScale(uint32_t uDPI)
{
	if (m_bAdaptDPI) {
		m_nScaleFactor = MulDiv(uDPI, 100, 96);
	}
	ASSERT(m_nScaleFactor >= 100);
}

int DpiManager::ScaleInt(int &iValue)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return iValue;
	}
	iValue = MulDiv(iValue, m_nScaleFactor, 100);
	return iValue;
}

void DpiManager::ScaleSize(SIZE &size)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return;
	}
	size.cx = MulDiv(size.cx, m_nScaleFactor, 100);
	size.cy = MulDiv(size.cy, m_nScaleFactor, 100);
}

void DpiManager::ScaleSize(UiSize &size)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return;
	}
	size.cx = MulDiv(size.cx, m_nScaleFactor, 100);
	size.cy = MulDiv(size.cy, m_nScaleFactor, 100);
}

void DpiManager::ScalePoint(POINT &point)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return;
	}
	point.x = MulDiv(point.x, m_nScaleFactor, 100);
	point.y = MulDiv(point.y, m_nScaleFactor, 100);
}

void DpiManager::ScalePoint(UiPoint &point)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return;
	}

	point.x = MulDiv(point.x, m_nScaleFactor, 100);
	point.y = MulDiv(point.y, m_nScaleFactor, 100);
}

void DpiManager::ScaleRect(RECT &rect)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return;
	}
	int width = MulDiv(rect.right - rect.left, m_nScaleFactor, 100);
	int height = MulDiv(rect.bottom - rect.top, m_nScaleFactor, 100);
	rect.left = MulDiv(rect.left, m_nScaleFactor, 100);
	rect.top = MulDiv(rect.top, m_nScaleFactor, 100);
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

void DpiManager::ScaleRect(UiRect &rect)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100) {
		return;
	}
	int width = MulDiv(rect.right - rect.left, m_nScaleFactor, 100);
	int height = MulDiv(rect.bottom - rect.top, m_nScaleFactor, 100);
	rect.left = MulDiv(rect.left, m_nScaleFactor, 100);
	rect.top = MulDiv(rect.top, m_nScaleFactor, 100);
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

}