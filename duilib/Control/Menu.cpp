#include "Menu.h"
#include "duilib/Utils/Macros.h"

namespace ui {

ContextMenuObserver& CMenuWnd::GetMenuObserver()
{
	static ContextMenuObserver s_context_menu_observer;
	return s_context_menu_observer;
}

//�������߶༶�Ӳ˵����й���
class CSubMenuUI: public ui::ListContainerElement
{
};

ui::Control* CMenuWnd::CreateControl(const std::wstring& pstrClass)
{
	if (pstrClass == DUI_CTR_MENUELEMENT){
		return new CMenuElementUI();
	}
	else if (pstrClass == DUI_CTR_SUB_MENU) {
		return new CSubMenuUI();
	}
	return NULL;
}

BOOL CMenuWnd::Receive(ContextMenuParam param)
{
	switch (param.wParam)
	{
	case MenuCloseType::eMenuCloseAll:
		CloseMenu();
		break;
		case MenuCloseType::eMenuCloseThis:
		{
			HWND hParent = GetParent(GetHWND());
			while (hParent != NULL) {
				if (hParent == param.hWnd) {
					CloseMenu();
					break;
				}
				hParent = GetParent(hParent);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

CMenuWnd::CMenuWnd(HWND hParent) :
	m_hParent(hParent),
	m_menuPoint({ 0 }),
	m_popupPosType(MenuPopupPosType::RIGHT_TOP),
	m_skinFolder(L"menu"),
	m_submenuXml(L"submenu.xml"),
	m_submenuNodeName(L"submenu"),
	m_xml(L""),
	m_noFocus(false),
	m_pOwner(nullptr),
	m_pLayout(nullptr)
{
}

void CMenuWnd::SetSkinFolder(const std::wstring& skinFolder)
{
	m_skinFolder = skinFolder;
}

void CMenuWnd::SetSubMenuXml(const std::wstring& submenuXml, const std::wstring& submenuNodeName)
{
	m_submenuXml = submenuXml;
	m_submenuNodeName = submenuNodeName;
}

void CMenuWnd::ShowMenu(const std::wstring& xml, const CPoint& point, MenuPopupPosType popupPosType, bool noFocus, CMenuElementUI* pOwner)
{
	m_menuPoint = point;
	m_popupPosType = popupPosType;

	m_xml = xml;
	m_noFocus = noFocus;
	m_pOwner = pOwner;

	CMenuWnd::GetMenuObserver().AddReceiver(this);

	CreateWnd(m_hParent, L"DUILIB_MENU_WINDOW", WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, true, ui::UiRect());
	// HACK: Don't deselect the parent's caption
	HWND hWndParent = GetHWND();
	while (::GetParent(hWndParent) != NULL) {
		hWndParent = ::GetParent(hWndParent);
	}
	::ShowWindow(GetHWND(), noFocus ? SW_SHOWNOACTIVATE : SW_SHOW);
	if (m_pOwner) {
		ResizeSubMenu();
	}
	else {
		ResizeMenu();
	}
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CMenuWnd::CloseMenu()
{
	//�����رգ�������������ʱ�໥����
	PostMessage(WM_CLOSE, 0L, 0L);
}

void CMenuWnd::DetachOwner()
{
	if (m_pOwner != nullptr) {
		if (m_pLayout != nullptr) {
			m_pLayout->SelectItem(-1);
		}

		//����OnInitWindow�У���ӵ�Layout�ϵĽڵ㣬�Ӵ�������ϵ
		std::vector<Control*> submenuControls;
		CMenuElementUI::GetAllSubMenuControls(m_pOwner, submenuControls);
		for (auto pItem : submenuControls) {
			pItem->SetWindow(nullptr, nullptr, false);
		}

		m_pLayout->RemoveAll();
		m_pOwner->m_pSubWindow = nullptr;
		m_pOwner->Invalidate();
		m_pOwner = nullptr;
	}
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	DetachOwner();
	Window::OnFinalMessage(hWnd);
	delete this;
}

std::wstring CMenuWnd::GetSkinFolder()
{
	return m_skinFolder;
}

std::wstring CMenuWnd::GetSkinFile() 
{
	return m_xml;
}

std::wstring CMenuWnd::GetWindowClassName() const
{
	return L"MenuWnd";
}

LRESULT CMenuWnd::OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = true;
	switch (uMsg)
	{
 		case WM_KILLFOCUS:
 		{
			HWND hFocusWnd = (HWND)wParam;

			BOOL bInMenuWindowList = FALSE;
			ContextMenuParam param;
			param.hWnd = GetHWND();

			ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(GetMenuObserver());
			ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
			while (pReceiver != nullptr) {
				CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
				if ((pContextMenu != nullptr) && (pContextMenu->GetHWND() == hFocusWnd)) {
					bInMenuWindowList = TRUE;
					break;
				}
				pReceiver = iterator.next();
			}

			if (!bInMenuWindowList) {
				param.wParam = MenuCloseType::eMenuCloseAll;
				GetMenuObserver().RBroadcast(param);

				return 0;
			}
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE || wParam == VK_LEFT) {
			CloseMenu();
		}
		else if (wParam == VK_RIGHT) {
			if (m_pLayout) {
				int index = m_pLayout->GetCurSel();
				CMenuElementUI* pItem = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(index));
				if (pItem != nullptr) {
					pItem->CheckSubMenuItem();
				}
			}
		}
		else if (wParam == VK_RETURN || wParam == VK_SPACE)
		{
			if (m_pLayout) {
				int index = m_pLayout->GetCurSel();
				CMenuElementUI* pItem = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(index));
				if (pItem) {
					if (!pItem->CheckSubMenuItem()) {
						ContextMenuParam param;
						param.hWnd = GetHWND();
						param.wParam = MenuCloseType::eMenuCloseAll;
						CMenuWnd::GetMenuObserver().RBroadcast(param);
					}
				}
			}
		}
		break;
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return 0L;

	default:
		break;
	}
	return __super::OnWindowMessage(uMsg, wParam, lParam, bHandled);
}

void CMenuWnd::ResizeMenu()
{
	ui::Control* pRoot = GetRoot();
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	//�����������������ĻΪ��
	::GetMonitorInfo(::MonitorFromPoint(m_menuPoint, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	ui::UiRect rcWork(oMonitor.rcWork);

	ui::CSize szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = pRoot->EstimateSize(szAvailable);   //�����������Ӱ����
	SetInitSize(szAvailable.cx, szAvailable.cy);
	ui::UiRect rcCorner = GetShadowCorner();
	ui::CSize szInit=szAvailable;
	szInit.cx -= rcCorner.left + rcCorner.right;
	szInit.cy -= rcCorner.top + rcCorner.bottom; //����ȥ����Ӱ���ڣ����û����Ӿ���Ч��� szInit<=szAvailable
	
	ui::CPoint point(m_menuPoint);  //�����и�bug�����������������ڴ����ڣ���ֱ�ӳ���mouseenter���³����Ӳ˵���ƫ��1������
	if (static_cast<int>(m_popupPosType) & static_cast<int>(eMenuAlignment_Right)) {
		point.x += -szAvailable.cx + rcCorner.right + rcCorner.left;
		point.x -= 1;
	}
	else if (static_cast<int>(m_popupPosType) & static_cast<int>(eMenuAlignment_Left)) {
		point.x += 1;
	}
	if (static_cast<int>(m_popupPosType) & static_cast<int>(eMenuAlignment_Bottom))	{
		point.y += -szAvailable.cy + rcCorner.bottom + rcCorner.top;
		point.y += 1;
	}
	else if (static_cast<int>(m_popupPosType) & static_cast<int>(eMenuAlignment_Top)) {
		point.y += 1;
	}
	if (static_cast<int>(m_popupPosType) & static_cast<int>(eMenuAlignment_Intelligent)) {
		if (point.x < rcWork.left) {
			point.x = rcWork.left;
		}
		else if (point.x + szInit.cx> rcWork.right) {
			point.x = rcWork.right - szInit.cx;
		}
		if (point.y < rcWork.top) {
			point.y = rcWork.top ;
		}
		else if (point.y + szInit.cy > rcWork.bottom) {
			point.y = rcWork.bottom - szInit.cy;
		}
	}
	if (!m_noFocus) {
		SetForegroundWindow(GetHWND());
		SetFocus(m_pLayout);
	}
	SetWindowPos(GetHWND(), HWND_TOPMOST,
		         point.x - rcCorner.left, point.y - rcCorner.top,
		         szAvailable.cx, szAvailable.cy,
		         SWP_SHOWWINDOW | (m_noFocus ? SWP_NOACTIVATE : 0));
}

void CMenuWnd::ResizeSubMenu()
{
	ASSERT(m_pOwner != nullptr);
	if (m_pOwner == nullptr) {
		return;
	}
	ASSERT(m_pOwner->GetWindow() != nullptr);

	// Position the popup window in absolute space
	RECT rcOwner = m_pOwner->GetPos();
	RECT rc = rcOwner;

	int cxFixed = 0;
	int cyFixed = 0;

	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromPoint(m_menuPoint, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	ui::UiRect rcWork (oMonitor.rcWork);
	ui::CSize szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

	for (int it = 0; it < m_pLayout->GetCount(); ++it) {
		//ȡ�Ӳ˵����е����ֵ��Ϊ�˵���
		CMenuElementUI* pItem = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(it));
		if (pItem != nullptr) {
			SIZE sz = pItem->EstimateSize(szAvailable);
			cyFixed += sz.cy;

			if (cxFixed < sz.cx) {
				cxFixed = sz.cx;
			}				
		}
	}
	ui::UiRect rcCorner = GetShadowCorner();
	RECT rcWindow;
	GetWindowRect(m_pOwner->GetWindow()->GetHWND(), &rcWindow);
	//ȥ��Ӱ
	{
		rcWindow.left += rcCorner.left;
		rcWindow.right -= rcCorner.right;
		rcWindow.top += rcCorner.top;
		rcWindow.bottom -=  rcCorner.bottom;
	}

	::MapWindowRect(m_pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
	
	rc.left = rcWindow.right;
	rc.right = rc.left + cxFixed;
	rc.bottom = rc.top + cyFixed;

	bool bReachBottom = false;
	bool bReachRight = false;

	RECT rcPreWindow = { 0 };
	ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(GetMenuObserver());
	ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
	while (pReceiver != nullptr) {
		CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
		if (pContextMenu != nullptr) {
			GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);  //��Ҫ������Ӱ

			bReachRight = (rcPreWindow.left + rcCorner.left) >= rcWindow.right;
			bReachBottom = (rcPreWindow.top + rcCorner.top) >= rcWindow.bottom;
			if (pContextMenu->GetHWND() == m_pOwner->GetWindow()->GetHWND()
				|| bReachBottom || bReachRight) {
				break;
			}
		}
		pReceiver = iterator.next();
	}
	if (bReachBottom) {
		rc.bottom = rcWindow.top;
		rc.top = rc.bottom - cyFixed;
	}

	if (bReachRight) {
		rc.right = rcWindow.left;
		rc.left = rc.right - cxFixed;
	}

	if (rc.bottom > rcWork.bottom) {
		rc.bottom = rc.top;
		rc.top = rc.bottom - cyFixed;
	}

	if (rc.right > rcWork.right) {
		rc.right = rcWindow.left;
		rc.left = rc.right - cxFixed;
	}

	if (rc.top < rcWork.top) {
		rc.top = rcOwner.top;
		rc.bottom = rc.top + cyFixed;
	}

	if (rc.left < rcWork.left) {
		rc.left = rcWindow.right;
		rc.right = rc.left + cxFixed;
	}

	if (!m_noFocus) {
		SetForegroundWindow(GetHWND());
		SetFocus(m_pLayout);
	}
	SetWindowPos(GetHWND(), HWND_TOPMOST,
		         rc.left - rcCorner.left, rc.top - rcCorner.top,
				 rc.right - rc.left, rc.bottom - rc.top,
				 SWP_SHOWWINDOW | (m_noFocus ? SWP_NOACTIVATE : 0));
}

void CMenuWnd::OnInitWindow()
{
	if (m_pOwner != nullptr) {
		m_pLayout = dynamic_cast<ui::ListBox*>(FindControl(m_submenuNodeName.c_str()));
		ASSERT(m_pLayout);
		if (m_pLayout == nullptr) {
			return;
		}
		//���ò��Զ�����Child������Ϊ�Ǵ�owner���ƹ����ģ���Դ���ã���Owner����������������ڣ�
		m_pLayout->SetAutoDestroyChild(false);

		//��ȡ�Ӳ˵�����Ҫ���ƵĿؼ�������ӵ�Layout
		std::vector<Control*> submenuControls;
		CMenuElementUI::GetAllSubMenuControls(m_pOwner, submenuControls);
		for (auto pControl : submenuControls) {
			if (pControl != nullptr) {
				m_pLayout->Add(pControl); //�ڲ������subMenuItem->SetOwner(m_pLayout); �����SetWindows���ı��˹������ڡ����ؼ���
				continue;
			}
		}
	}
	else {
		m_pLayout = dynamic_cast<ui::ListBox*>(GetRoot());
		if (m_pLayout == nullptr) {
			//���������ײ���Ӱ
			if ((GetRoot() != nullptr) && (GetRoot()->GetCount() > 0)) {
				m_pLayout = dynamic_cast<ui::ListBox*>(GetRoot()->GetItemAt(0));
			}
		}
		ASSERT(m_pLayout != nullptr);
	}
}

bool CMenuWnd::AddMenuItem(CMenuElementUI* pMenuItem)
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	ASSERT(m_pLayout != nullptr);
	if (m_pLayout != nullptr) {
		return m_pLayout->Add(pMenuItem);
	}
	return false;
}

bool CMenuWnd::AddMenuItemAt(CMenuElementUI* pMenuItem, size_t iIndex)
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	ASSERT(m_pLayout != nullptr);
	
	size_t itemIndex = 0;
	CMenuElementUI* pElementUI = nullptr;
	const int count = m_pLayout->GetCount();
	for (size_t i = 0; i < count; ++i) {
		Control* pControl = m_pLayout->GetItemAt(i);
		pElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (pElementUI != nullptr) {
			if (itemIndex == iIndex) {
				return m_pLayout->AddAt(pMenuItem, i);
			}
			++itemIndex;
		}
		pElementUI = nullptr;
	}
	return false;
}

bool CMenuWnd::RemoveMenuItem(CMenuElementUI* pMenuItem)
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	ASSERT(m_pLayout != nullptr);
	CMenuElementUI* pElementUI = nullptr;
	if (m_pLayout != nullptr) {
		const int count = m_pLayout->GetCount();
		for (int i = 0; i < count; ++i) {
			pElementUI = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(i));
			if (pMenuItem == pElementUI) {
				m_pLayout->RemoveAt(i);
			}
			pElementUI = nullptr;
		}
	}
	return false;
}

bool CMenuWnd::RemoveMenuItemAt(size_t iIndex)
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	CMenuElementUI* pMenuElementUI = GetMenuItemAt(iIndex);
	if (pMenuElementUI != nullptr) {
		return RemoveMenuItem(pMenuElementUI);
	}
	return false;
}

size_t CMenuWnd::GetMenuItemCount() const
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	if (m_pLayout == nullptr) {
		return 0;
	}
	size_t itemCount = 0;
	const int count = m_pLayout->GetCount();
	for (size_t i = 0; i < count; ++i) {
		if (dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(i)) != nullptr) {
			++itemCount;
		}
	}
	return itemCount;
}

CMenuElementUI* CMenuWnd::GetMenuItemAt(size_t iIndex) const
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	ASSERT(m_pLayout != nullptr);	
	if (m_pLayout == nullptr) {
		return nullptr;
	}
	size_t itemIndex = 0;
	CMenuElementUI* pElementUI = nullptr;
	const int count = m_pLayout->GetCount();
	for (size_t i = 0; i < count; ++i) {
		Control* pControl = m_pLayout->GetItemAt(i);
		pElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (pElementUI != nullptr) {
			if (itemIndex == iIndex) {
				break;
			}
			++itemIndex;
		}
		pElementUI = nullptr;
	}
	return pElementUI;
}

CMenuElementUI* CMenuWnd::GetMenuItemByName(const std::wstring& name) const
{
	//Ŀǰֻ��һ���˵����Է�������ӿ�
	ASSERT(m_pOwner == nullptr);
	ASSERT(m_pLayout != nullptr);
	CMenuElementUI* pElementUI = nullptr;
	if (m_pLayout != nullptr) {
		const int count = m_pLayout->GetCount();
		for (int i = 0; i < count; ++i) {
			pElementUI = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(i));
			if ((pElementUI != nullptr) && (pElementUI->GetName() == name)) {
				break;
			}
			pElementUI = nullptr;
		}
	}
	return pElementUI;
}

CMenuElementUI::CMenuElementUI():
	m_pSubWindow(nullptr)
{
	m_bMouseChildEnabled = false;
}

void CMenuElementUI::GetAllSubMenuItem(const CMenuElementUI* pParentElementUI,
								       std::vector<CMenuElementUI*>& submenuItems)
{
	submenuItems.clear();
	ASSERT(pParentElementUI != nullptr);
	if (pParentElementUI == nullptr) {
		return;
	}
	const int itemCount = pParentElementUI->GetCount();
	for (int i = 0; i < itemCount; ++i) {
		Control* pControl = pParentElementUI->GetItemAt(i);
		CMenuElementUI* menuElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (menuElementUI != nullptr) {
			submenuItems.push_back(menuElementUI);
			continue;
		}

		menuElementUI = nullptr;
		CSubMenuUI* subMenu = dynamic_cast<CSubMenuUI*>(pControl);
		if (subMenu != nullptr) {
			const int count = subMenu->GetCount();
			for (int j = 0; j < count; ++j) {
				menuElementUI = dynamic_cast<CMenuElementUI*>(subMenu->GetItemAt(j));
				if (menuElementUI != nullptr) {
					submenuItems.push_back(menuElementUI);
					continue;
				}
			}
		}
	}
}

void CMenuElementUI::GetAllSubMenuControls(const CMenuElementUI* pParentElementUI,
										   std::vector<Control*>& submenuControls)
{
	submenuControls.clear();
	ASSERT(pParentElementUI != nullptr);
	if (pParentElementUI == nullptr) {
		return;
	}
	const int itemCount = pParentElementUI->GetCount();
	for (int i = 0; i < itemCount; ++i) {
		Control* pControl = pParentElementUI->GetItemAt(i);
		CMenuElementUI* menuElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (menuElementUI != nullptr) {
			submenuControls.push_back(menuElementUI);
			continue;
		}

		CSubMenuUI* subMenu = dynamic_cast<CSubMenuUI*>(pControl);
		if (subMenu != nullptr) {
			const int count = subMenu->GetCount();
			for (int j = 0; j < count; ++j) {
				Control* pSubControl = subMenu->GetItemAt(j);
				if (pSubControl != nullptr) {
					submenuControls.push_back(pSubControl);
				}
			}
		}
	}
}

bool CMenuElementUI::AddSubMenuItem(CMenuElementUI* pMenuItem)
{
	return Add(pMenuItem);
}

bool CMenuElementUI::AddSubMenuItemAt(CMenuElementUI* pMenuItem, size_t iIndex)
{
	const size_t subMenuCount = GetSubMenuItemCount();
	ASSERT(iIndex <= subMenuCount);
	if (iIndex > subMenuCount) {
		return false;
	}
	
	size_t itemIndex = 0;
	const int itemCount = GetCount();
	for (int i = 0; i < itemCount; ++i) {
		Control* pControl = GetItemAt(i);
		CMenuElementUI* menuElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (menuElementUI != nullptr) {
			if (itemIndex == iIndex) {
				//�ڵ�ǰ�ڵ���ƥ�䵽
				return AddAt(pMenuItem, i);
			}
			++itemIndex;
			continue;
		}

		menuElementUI = nullptr;
		CSubMenuUI* subMenu = dynamic_cast<CSubMenuUI*>(pControl);
		if (subMenu != nullptr) {
			const int count = subMenu->GetCount();
			for (int j = 0; j < count; ++j) {
				menuElementUI = dynamic_cast<CMenuElementUI*>(subMenu->GetItemAt(j));
				if (menuElementUI != nullptr) {
					if (itemIndex == iIndex) {
						//�ڵ�ǰ�ڵ��µ�SubMenu��ƥ�䵽
						return subMenu->AddAt(pMenuItem, j);
					}
					++itemIndex;
					continue;
				}
			}
		}
	}
	//���ƥ�䲻���������ӵ������
	return Add(pMenuItem);
}

bool CMenuElementUI::RemoveSubMenuItem(CMenuElementUI* pMenuItem)
{
	const int itemCount = GetCount();
	for (int i = 0; i < itemCount; ++i) {
		Control* pControl = GetItemAt(i);
		CMenuElementUI* menuElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (menuElementUI != nullptr) {
			if (pMenuItem == menuElementUI) {
				//�ڵ�ǰ�ڵ���ƥ�䵽
				return RemoveAt(i);
			}
			continue;
		}

		menuElementUI = nullptr;
		CSubMenuUI* subMenu = dynamic_cast<CSubMenuUI*>(pControl);
		if (subMenu != nullptr) {
			const int count = subMenu->GetCount();
			for (int j = 0; j < count; ++j) {
				menuElementUI = dynamic_cast<CMenuElementUI*>(subMenu->GetItemAt(j));
				if (menuElementUI != nullptr) {
					if (menuElementUI == pMenuItem) {
						//�ڵ�ǰ�ڵ��µ�SubMenu��ƥ�䵽
						return subMenu->RemoveAt(j);
					}
					continue;
				}
			}
		}
	}
	return false;
}
bool CMenuElementUI::RemoveSubMenuItemAt(size_t iIndex)
{
	size_t itemIndex = 0;
	const int itemCount = GetCount();
	for (int i = 0; i < itemCount; ++i) {
		Control* pControl = GetItemAt(i);
		CMenuElementUI* menuElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (menuElementUI != nullptr) {
			if (itemIndex == iIndex) {
				//�ڵ�ǰ�ڵ���ƥ�䵽
				return RemoveAt(i);				 
			}
			++itemIndex;
			continue;
		}

		menuElementUI = nullptr;
		CSubMenuUI* subMenu = dynamic_cast<CSubMenuUI*>(pControl);
		if (subMenu != nullptr) {
			const int count = subMenu->GetCount();
			for (int j = 0; j < count; ++j) {
				menuElementUI = dynamic_cast<CMenuElementUI*>(subMenu->GetItemAt(j));
				if (menuElementUI != nullptr) {
					if (itemIndex == iIndex) {
						//�ڵ�ǰ�ڵ��µ�SubMenu��ƥ�䵽
						return subMenu->RemoveAt(j);
					}
					++itemIndex;
					continue;
				}
			}
		}
	}
	return false;
}

void CMenuElementUI::RemoveAllSubMenuItem()
{
	RemoveAll();
}

size_t CMenuElementUI::GetSubMenuItemCount() const
{
	std::vector<CMenuElementUI*> submenuItems;
	GetAllSubMenuItem(this, submenuItems);
	return submenuItems.size();
};

CMenuElementUI* CMenuElementUI::GetSubMenuItemAt(size_t iIndex) const
{
	CMenuElementUI* foundItem = nullptr;
	std::vector<CMenuElementUI*> submenuItems;
	GetAllSubMenuItem(this, submenuItems);
	if (iIndex <= submenuItems.size()) {
		foundItem = submenuItems.at(iIndex);
	}
	return foundItem;
}

CMenuElementUI* CMenuElementUI::GetSubMenuItemByName(const std::wstring& name) const
{
	std::vector<CMenuElementUI*> submenuItems;
	GetAllSubMenuItem(this, submenuItems);
	CMenuElementUI* subMenuItem = nullptr;
	for (auto item : submenuItems) {
		if ((item != nullptr) && (item->GetName() == name)) {
			subMenuItem = item;
			break;
		}
	}
	return subMenuItem;
}

bool CMenuElementUI::ButtonUp(ui::EventArgs& msg)
{
	Window* pWindow = GetWindow();
	ASSERT(pWindow != nullptr);
	if (pWindow == nullptr) {
		return false;
	}
	std::weak_ptr<nbase::WeakFlag> weakFlag = pWindow->GetWeakFlag();
	bool ret = __super::ButtonUp(msg);
	if (ret && !weakFlag.expired()) {
		//���ﴦ����������Ӳ˵�����ʾ�Ӳ˵�
		if (!CheckSubMenuItem()){
			ContextMenuParam param;
			param.hWnd = pWindow->GetHWND();
			param.wParam = MenuCloseType::eMenuCloseAll;
			CMenuWnd::GetMenuObserver().RBroadcast(param);
		}
	}
	return ret;
}

bool CMenuElementUI::MouseEnter(ui::EventArgs& msg)
{
	Window* pWindow = GetWindow();
	ASSERT(pWindow != nullptr);
	if (pWindow == nullptr) {
		return false;
	}
	std::weak_ptr<nbase::WeakFlag> weakFlag = pWindow->GetWeakFlag();
	bool ret = __super::MouseEnter(msg);
	if (ret && !weakFlag.expired()) {
		//���ﴦ����������Ӳ˵�����ʾ�Ӳ˵�
		if (!CheckSubMenuItem()) {
			ContextMenuParam param;
			param.hWnd = pWindow->GetHWND();
			param.wParam = MenuCloseType::eMenuCloseThis;
			CMenuWnd::GetMenuObserver().RBroadcast(param);
			//����ð�֮ǰѡ�е���Ϊδѡ��
			if (m_pOwner != nullptr) {
				m_pOwner->SelectItem(-1, false);
			}
		}
	}
	return ret;
}

void CMenuElementUI::PaintChild(ui::IRenderContext* pRender, const ui::UiRect& rcPaint)
{
	ui::UiRect rcTemp;
	if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) {
		return;
	}

	for (auto item : m_items) {
		Control* pControl = item;

		//���ڶ༶�˵�������ݣ�������
		CMenuElementUI* menuElementUI = dynamic_cast<CMenuElementUI*>(pControl);
		if (menuElementUI != nullptr){
			continue;
		}
		CSubMenuUI* subMenu = dynamic_cast<CSubMenuUI*>(pControl);
		if (subMenu != nullptr) {
			continue;
		}
		
		if (!pControl->IsVisible()) {
			continue;
		}
		pControl->AlphaPaint(pRender, rcPaint);
	}
}

bool CMenuElementUI::CheckSubMenuItem()
{
	bool hasSubMenu = false;
	for (auto item : m_items) {
		CMenuElementUI* subMenuItem = dynamic_cast<CMenuElementUI*>(item);
		if (subMenuItem != nullptr) {
			hasSubMenu = true;
			break;
		}
	}
	if (hasSubMenu) {
		if (m_pOwner != nullptr) {
			m_pOwner->SelectItem(GetIndex(), true);
		}
		if (m_pSubWindow == nullptr) {
			CreateMenuWnd();
		}
		else {
			//�ϴ�չʾ���Ӳ˵����ڣ���δ��ʧ������չʾ
			hasSubMenu = false;
		}
	}
	return hasSubMenu;
}

void CMenuElementUI::CreateMenuWnd()
{
	ASSERT(m_pSubWindow == nullptr);
	if (m_pSubWindow != nullptr) {
		return;
	}

	HWND hWnd = nullptr;
	Window* pWindow = GetWindow();
	if (pWindow != nullptr) {
		hWnd = pWindow->GetHWND();
	}
	m_pSubWindow = new CMenuWnd(hWnd);
	ContextMenuParam param;
	param.hWnd = hWnd;
	param.wParam = MenuCloseType::eMenuCloseThis;
	CMenuWnd::GetMenuObserver().RBroadcast(param);

	//�ϼ����˵����ڽӿڣ�����ͬ��������Ϣ
	CMenuWnd* pParentWindow = dynamic_cast<CMenuWnd*>(pWindow);
	ASSERT(pParentWindow != nullptr);
	if (pParentWindow != nullptr) {
		m_pSubWindow->SetSkinFolder(pParentWindow->m_skinFolder);
		m_pSubWindow->SetSubMenuXml(pParentWindow->m_submenuXml, pParentWindow->m_submenuNodeName);
		m_pSubWindow->ShowMenu(pParentWindow->m_submenuXml, CPoint(), MenuPopupPosType::RIGHT_BOTTOM, false, this);
	}
}

} // namespace ui
