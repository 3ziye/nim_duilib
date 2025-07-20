#include "BrowserForm.h"
#include "TestApplication.h"
#include "BrowserBox.h"
#include "BrowserManager.h"
#include <chrono>

using namespace ui;

namespace
{
    // 窗口收到WM_CLOSE消息的原因
    enum CloseReason
    {
        kDefaultClose    = 10,  // 在任务栏右击关闭窗口，按Alt+F4等常规原因
        kBrowserBoxClose = 11   // 关闭了最后一个浏览器盒子导致窗口关闭
    };
}

BrowserForm::BrowserForm()
{
    m_pEditUrl = nullptr;
    m_pTabCtrl = nullptr;
    m_pBorwserBoxTab = nullptr;
    m_pActiveBrowserBox = nullptr;
}

BrowserForm::~BrowserForm()
{
    m_pEditUrl = nullptr;
    m_pTabCtrl = nullptr;
    m_pBorwserBoxTab = nullptr;
    m_pActiveBrowserBox = nullptr;
}

DString BrowserForm::GetSkinFolder()
{
    return _T("webview2_browser");
}

DString BrowserForm::GetSkinFile()
{
    return _T("webview2_browser.xml");
}

/** 标题栏区域的布局管理
*/
class TitleBarHLayout : public HLayout
{
public:
    TitleBarHLayout() = default;
    virtual ~TitleBarHLayout() override = default;

public:
    /** 调整内部所有控件的位置信息
     * @param[in] items 控件列表
     * @param[in] rc 当前容器位置信息, 包含内边距，但不包含外边距
     * @return 返回排列后最终盒子的宽度和高度信息，包含Owner Box的内边距，不包含外边距
     */
    virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override
    {
        UiSize64 szSize = HLayout::ArrangeChild(items, rc);
        ASSERT(items.empty() || items.size() == 5);
        if (items.size() != 5) {
            return szSize;
        }
        //固定结构, 校验后修改标签栏的宽度
        int64_t nTotalWidth = 0;
        for (Control* pControl : items) {
            if (pControl == nullptr) {
                continue;
            }
            ui::UiMargin margin = pControl->GetMargin();
            nTotalWidth += (pControl->GetRect().Width() + margin.left + margin.right);
        }
        ASSERT(szSize.cx == nTotalWidth);
        if (szSize.cx != nTotalWidth) {
            return szSize;
        }
        ASSERT(rc.Width() == nTotalWidth);
        if (rc.Width() != nTotalWidth) {
            return szSize;
        }

        //校验标签栏的TabCtrl控件
        ui::TabCtrl* pTabCtrl = nullptr;
        ui::Control* pItem = items[2];
        if (pItem != nullptr) {
            pTabCtrl = dynamic_cast<ui::TabCtrl*>(pItem);            
        }
        ASSERT(pTabCtrl != nullptr);
        if (pTabCtrl == nullptr) {
            return szSize;
        }
        ASSERT(pTabCtrl->GetFixedWidth().IsStretch());
        if (!pTabCtrl->GetFixedWidth().IsStretch()) {
            return szSize;
        }

        //等于总宽度：计算标签控件的实际总宽度
        int32_t nTabItemTotalWidth = 0;
        UiRect rcTabCtrl = pItem->GetPos();
        const size_t nTabItemCount = pTabCtrl->GetItemCount();
        for (size_t nTabItem = 0; nTabItem < nTabItemCount; ++nTabItem) {
            Control* pTabItem = pTabCtrl->GetItemAt(nTabItem);
            if ((pTabItem == nullptr) || !pTabItem->IsVisible()) {
                continue;
            }
            ui::UiMargin rcTabItemMargin = pTabItem->GetMargin();
            nTabItemTotalWidth += (pTabItem->GetPos().Width() + rcTabItemMargin.left + rcTabItemMargin.right);
        }
        if (nTabItemTotalWidth < rcTabCtrl.Width()) {
            int32_t nItemDiff = rcTabCtrl.Width() - nTabItemTotalWidth;
            //标签控件：长度缩短
            rcTabCtrl.right -= nItemDiff;
            pTabCtrl->SetPos(rcTabCtrl);

            //新建按钮控件：向左移动
            Control* pItem = items[items.size() - 2];
            if (pItem != nullptr) {
                UiRect rcItem = pItem->GetPos();
                rcItem.Offset(-nItemDiff, 0);
                pItem->SetPos(rcItem);
            }
            //最后的控件：宽度增加
            pItem = items[items.size() - 1];
            if (pItem != nullptr) {
                UiRect rcItem = pItem->GetPos();
                rcItem.left -= nItemDiff;
                pItem->SetPos(rcItem);
            }
        }

        //最后再校验
        nTotalWidth = 0;
        for (Control* pControl : items) {
            if (pControl == nullptr) {
                continue;
            }
            ui::UiMargin margin = pControl->GetMargin();
            nTotalWidth += (pControl->GetRect().Width() + margin.left + margin.right);
        }
        ASSERT(rc.Width() == nTotalWidth);
        return szSize;
    }
};

void BrowserForm::OnInitWindow()
{
    TestApplication::Instance().AddMainWindow(this);
    AttachWindowSetFocus([this](const ui::EventArgs&) {
        TestApplication::Instance().SetActiveMainWindow(this);
        return true;
        });

    GetRoot()->AttachBubbledEvent(ui::kEventClick, UiBind(&BrowserForm::OnClicked, this, std::placeholders::_1));

    m_pEditUrl = static_cast<RichEdit*>(FindControl(_T("edit_url")));
    m_pEditUrl->AttachReturn(UiBind(&BrowserForm::OnReturn, this, std::placeholders::_1));
    if (m_pEditUrl != nullptr) {
        //鼠标点击切入地址栏时，全选文本
        m_pEditUrl->SetSelAllOnFocus(true);
    }

    //替换标题栏的布局
    HBox* pTitleBar = static_cast<HBox*>(FindControl(_T("title_bar")));
    if (pTitleBar != nullptr) {
        TitleBarHLayout* pNewLayout = new TitleBarHLayout;
        Layout* pOldLayout = pTitleBar->ResetLayout(pNewLayout);
        if (pOldLayout != nullptr) {
            //复制布局属性
            pNewLayout->SetChildMarginX(pOldLayout->GetChildMarginX());
            pNewLayout->SetChildMarginY(pOldLayout->GetChildMarginY());
            pTitleBar->FreeLayout(pOldLayout);
        }
    }

    m_pTabCtrl = static_cast<TabCtrl*>(FindControl(_T("tab_ctrl")));
    m_pBorwserBoxTab = static_cast<TabBox*>(FindControl(_T("browser_box_tab")));

    if (m_pTabCtrl != nullptr) {
        m_pTabCtrl->AttachSelect(UiBind(&BrowserForm::OnTabItemSelected, this, std::placeholders::_1));
    }

    //设置按钮的状态
    Control* pButton = FindControl(_T("btn_back"));
    if (pButton != nullptr) {
        pButton->SetEnabled(false);
    }
    pButton = FindControl(_T("btn_forward"));
    if (pButton != nullptr) {
        pButton->SetEnabled(false);
    }
    pButton = FindControl(_T("btn_refresh"));
    if (pButton != nullptr) {
        pButton->SetVisible(true);
    }
    pButton = FindControl(_T("btn_stop"));
    if (pButton != nullptr) {
        pButton->SetVisible(false);
    }
}

void BrowserForm::OnCloseWindow()
{
    TestApplication::Instance().RemoveMainWindow(this);
    // 使用m_pTabCtrl来判断浏览器盒子总数，browser_box_tab_获取的总数不准确
    int browser_box_count = GetBoxCount();
    for (int i = 0; i < browser_box_count; i++) {
        Control* pBoxItem = m_pBorwserBoxTab->GetItemAt(i);
        ASSERT(nullptr != pBoxItem);
        if (nullptr == pBoxItem) {
            continue;
        }

        BrowserBox* pBrowserBox = dynamic_cast<BrowserBox*>(pBoxItem);
        if (nullptr != pBrowserBox) {
            pBrowserBox->UninitBrowserBox();
        }
    }
}

void BrowserForm::OnLoadingStateChange(BrowserBox* pBrowserBox)
{
    if (m_pActiveBrowserBox != pBrowserBox) {
        return;
    }
    ui::WebView2Control* pWebView2Ccontrol = m_pActiveBrowserBox->GetWebView2Control();
    if (pWebView2Ccontrol == nullptr) {
        return;
    }
    bool isLoading = pWebView2Ccontrol->IsNavigating();
    bool canGoBack = pWebView2Ccontrol->CanGoBack();
    bool canGoForward = pWebView2Ccontrol->CanGoForward();
    Control* pButton = FindControl(_T("btn_back"));
    if (pButton != nullptr) {
        pButton->SetEnabled(canGoBack);
    }
    pButton = FindControl(_T("btn_forward"));
    if (pButton != nullptr) {
        pButton->SetEnabled(canGoForward);
    }
    pButton = FindControl(_T("btn_refresh"));
    if (pButton != nullptr) {
        pButton->SetVisible(!isLoading);
    }
    pButton = FindControl(_T("btn_stop"));
    if (pButton != nullptr) {
        pButton->SetVisible(isLoading);
    }
}

LRESULT BrowserForm::OnKeyDownMsg(VirtualKeyCode vkCode, uint32_t modifierKey, const NativeMsg& nativeMsg, bool& bHandled)
{
    if ((vkCode == VirtualKeyCode::kVK_TAB) && ui::Keyboard::IsKeyDown(VirtualKeyCode::kVK_CONTROL)) {
        // 处理Ctrl+Tab快捷键: 切换标签
        bHandled = true;
        size_t nNextItem = m_pTabCtrl->GetCurSel();
        nNextItem = (nNextItem + 1) % m_pTabCtrl->GetItemCount();
        m_pTabCtrl->SelectItem(nNextItem, true, true);
        return 0;
    }
    return BaseClass::OnKeyDownMsg(vkCode, modifierKey, nativeMsg, bHandled);
}

LRESULT BrowserForm::OnWindowCloseMsg(uint32_t wParam, const ui::NativeMsg& nativeMsg, bool& bHandled)
{
    int browser_count = GetBoxCount();
    if (browser_count > 0 && nullptr != m_pActiveBrowserBox) {
        // 如果只有一个浏览器盒子，就直接关闭
        if (1 == browser_count) {
            CloseBox(m_pActiveBrowserBox->GetBrowserId());
        }        
        else {
            // 如果包含多个浏览器盒子
            while (GetBoxCount() > 0) {
                Control* pTabItem = m_pTabCtrl->GetItemAt(0);
                ASSERT(nullptr != pTabItem);
                if (nullptr == pTabItem) {
                    break;
                }
                CloseBox(pTabItem->GetUTF8Name());
            }
        }
    }
    return BaseClass::OnWindowCloseMsg(wParam, nativeMsg, bHandled);
}

bool BrowserForm::OnClicked(const ui::EventArgs& arg )
{
    DString name = arg.GetSender()->GetName();
    if (name == _T("btn_close")) {
        if (m_pActiveBrowserBox != nullptr) {
            CloseBox(m_pActiveBrowserBox->GetBrowserId());
        }
    }
    else if (name == _T("btn_add")) {
        uint64_t nTimeMS = std::chrono::steady_clock::now().time_since_epoch().count() / 1000;
        std::string timeStamp = ui::StringUtil::UInt64ToStringA(nTimeMS);
        BrowserManager::GetInstance()->CreateBorwserBox(this, timeStamp, _T(""));
    }
    else if (m_pActiveBrowserBox) {
        WebView2Control* pWebView2Control = m_pActiveBrowserBox->GetWebView2Control();
        if (!pWebView2Control) {
            return true;
        }

        if (name == _T("btn_back")) {
            pWebView2Control->NavigateBack();
        }
        else if (name == _T("btn_forward")) {
            pWebView2Control->NavigateForward();
        }
        else if (name == _T("btn_refresh")) {
            pWebView2Control->Refresh();
        }
        else if (name == _T("btn_stop")) {
            pWebView2Control->Stop();
        }
    }

    return true;
}

bool BrowserForm::OnReturn(const ui::EventArgs& arg)
{
    if (m_pEditUrl != nullptr) {
        DString url = m_pEditUrl->GetText();
        if (!url.empty()) {
            if ((m_pActiveBrowserBox != nullptr) && (m_pActiveBrowserBox->GetWebView2Control())) {
                m_pActiveBrowserBox->GetWebView2Control()->Navigate(url);
            }
        }        
    }
    return true;
}

void BrowserForm::OpenLinkUrl(const DString& url, bool bInNewWindow)
{
    if (ui::GlobalManager::Instance().IsInUIThread()) {
        std::string id = BrowserManager::GetInstance()->CreateBrowserID();
        if (bInNewWindow) {
            BrowserManager::GetInstance()->CreateBorwserBox(nullptr, id, url);
        }
        else {
            BrowserManager::GetInstance()->CreateBorwserBox(this, id, url);
        }
    }
    else {
        //转发到UI线程处理
        ui::GlobalManager::Instance().Thread().PostTask(ui::kThreadUI, UiBind(&BrowserForm::OpenLinkUrl, this, url, bInNewWindow));
    }
}

BrowserBox* BrowserForm::CreateBrowserBox(ui::Window* pWindow, std::string browserId)
{
    return new BrowserBox(pWindow, browserId);
}

BrowserBox* BrowserForm::CreateBox(const std::string& browserId, DString url)
{
    DString id = ui::StringConvert::UTF8ToT(browserId);
    if (nullptr != FindTabItem(id)) {
        ASSERT(0);
        return nullptr;
    }
    if (nullptr != FindBox(id)) {
        ASSERT(0);
        return nullptr;
    }

    TabCtrlItem* pTabItem = new TabCtrlItem(m_pTabCtrl->GetWindow());
    GlobalManager::Instance().FillBoxWithCache(pTabItem, ui::FilePath(_T("webview2_browser/tab_item.xml")));
    m_pTabCtrl->AddItemAt(pTabItem, GetBoxCount());
    pTabItem->SetUTF8Name(browserId);
    ui::Button* btn_item_close = pTabItem->GetCloseButton();
    ASSERT(btn_item_close != nullptr);
    if (btn_item_close != nullptr) {
        btn_item_close->AttachClick(UiBind(&BrowserForm::OnTabItemClose, this, std::placeholders::_1, browserId));
    }

    BrowserBox* pBrowserBox = CreateBrowserBox(m_pBorwserBoxTab->GetWindow(), browserId);
    m_pBorwserBoxTab->AddItem(pBrowserBox);
    GlobalManager::Instance().FillBoxWithCache(pBrowserBox, ui::FilePath(_T("webview2_browser/browser_box.xml")), nullptr);
    pBrowserBox->SetName(id);
    pBrowserBox->InitBrowserBox(url);

    if (GetBoxCount() <= 1) {
        m_pActiveBrowserBox = pBrowserBox;
    }

    // 切换到新的浏览器盒子
    // 如果pTabItem处于隐藏状态，则无法顺利触发选择事件，所以这里直接切换到目标浏览器盒子
    pTabItem->Selected(true, false, 0);
    ChangeToBox(id);

    OnCreateNewTabPage(pTabItem, pBrowserBox);
    return pBrowserBox;
}

bool BrowserForm::CloseBox(const std::string& browserId)
{
    if (browserId.empty()) {
        return false;
    }

    DString id = ui::StringConvert::UTF8ToT(browserId);

    bool bRet = false;
    // 从左侧会话列表项移除对应item
    TabCtrlItem* pTabItem = FindTabItem(id);
    if (nullptr != pTabItem) {
        m_pTabCtrl->RemoveItem(pTabItem);
        m_pTabCtrl->ArrangeAncestor();
        bRet = true;
    }

    // 在浏览器列表中找到浏览器盒子并且移除盒子
    BrowserBox* pBrowserBox = FindBox(id);
    ASSERT(pBrowserBox != nullptr);
    if (pBrowserBox != nullptr) {
        OnCloseTabPage(pBrowserBox);
        pBrowserBox->UninitBrowserBox();
        // 如果浏览器盒子的数量大于1就立马移除盒子，否则不移除
        // 如果最后一个浏览器盒子在这里立马移除，在窗口关闭时界面会因为没有控件而变成黑色
        // 窗口关闭时，会自动的移除这个浏览器盒子
        if (m_pBorwserBoxTab->GetItemCount() > 1) {
            m_pBorwserBoxTab->RemoveItem(pBrowserBox);
            if (m_pActiveBrowserBox == pBrowserBox) {
                m_pActiveBrowserBox = nullptr;
                size_t nSelItem = m_pBorwserBoxTab->GetCurSel();
                if (nSelItem != Box::InvalidIndex) {
                    m_pActiveBrowserBox = dynamic_cast<BrowserBox*>(m_pBorwserBoxTab->GetItemAt(nSelItem));
                }                
            }
        }
        else {
            m_pActiveBrowserBox = nullptr;
        }
    }

    if (GetBoxCount() == 0) {
        // 当浏览器盒子清空时，关闭浏览器窗口
        this->CloseWnd(kBrowserBoxClose);
    }
    else {
        if (m_pActiveBrowserBox != nullptr) {
            //选中新标签
            std::string newId = m_pActiveBrowserBox->GetBrowserId();
            SetActiveBox(newId);
        }
    }
    return bRet;
}

bool BrowserForm::AttachBox(BrowserBox* pBrowserBox)
{
    if (nullptr == pBrowserBox) {
        return false;
    }

    DString id = ui::StringConvert::UTF8ToT(pBrowserBox->GetBrowserId());
    if (nullptr != FindTabItem(id)) {
        ASSERT(0);
        return false;
    }
    if (nullptr != FindBox(id)) {
        ASSERT(0);
        return false;
    }

    TabCtrlItem* pTabItem = new TabCtrlItem(m_pTabCtrl->GetWindow());
    GlobalManager::Instance().FillBoxWithCache(pTabItem, ui::FilePath(_T("webview2_browser/tab_item.xml")));
    m_pTabCtrl->AddItemAt(pTabItem, GetBoxCount());
    pTabItem->SetUTF8Name(pBrowserBox->GetBrowserId());
    pTabItem->SetTitle(pBrowserBox->GetTitle());
    ui::Button* btn_item_close = pTabItem->GetCloseButton();
    ASSERT(btn_item_close != nullptr);
    if (btn_item_close != nullptr) {
        btn_item_close->AttachClick(UiBind(&BrowserForm::OnTabItemClose, this, std::placeholders::_1, pBrowserBox->GetBrowserId()));
    }

    // 当另一个窗体创建的pBrowserBox浏览器盒子控件添加到另一个窗体内的容器控件时
    // AddItem函数会重新的修改pBrowserBox内所有子控件的m_pWindow为新的窗体指针
    m_pBorwserBoxTab->AddItem(pBrowserBox);

    if (GetBoxCount() <= 1) {
        m_pActiveBrowserBox = pBrowserBox;
    }

    // 切换到新的浏览器盒子
    // 如果pTabItem处于隐藏状态，则无法顺利触发选择事件，所以这里直接切换到目标浏览器盒子
    pTabItem->Selected(true, false, 0);
    ChangeToBox(id);

    //重新下载网站图标
    ui::WebView2Control* pWebView2Control = pBrowserBox->GetWebView2Control();
    if (pWebView2Control != nullptr) {
        //TODO: 待实现
        //pWebView2Control->ReDownloadFavIcon();
    }

    OnCreateNewTabPage(pTabItem, pBrowserBox);
    return true;
}

bool BrowserForm::DetachBox(BrowserBox* pBrowserBox)
{
    if (pBrowserBox == nullptr) {
        return false;
    }

    DString id = ui::StringConvert::UTF8ToT(pBrowserBox->GetBrowserId());

    // 从顶部标签页移除对应item
    TabCtrlItem* pTabItem = FindTabItem(id);
    if (pTabItem == nullptr) {
        return false;
    }

    m_pTabCtrl->RemoveItem(pTabItem);
    m_pTabCtrl->ArrangeAncestor();
    OnCloseTabPage(pBrowserBox);

    // 在右侧Tab浏览器盒子列表中找到浏览器盒子并且移除盒子
    // 在这里不能delete pBrowserBox
    bool auto_destroy = m_pBorwserBoxTab->IsAutoDestroyChild();
    m_pBorwserBoxTab->SetAutoDestroyChild(false);
    if (!m_pBorwserBoxTab->RemoveItem(pBrowserBox)) {
        m_pBorwserBoxTab->SetAutoDestroyChild(auto_destroy);
        return false;
    }
    m_pBorwserBoxTab->SetAutoDestroyChild(auto_destroy);
    if (m_pActiveBrowserBox == pBrowserBox) {
        m_pActiveBrowserBox = nullptr;
    }

    // 当浏览器盒子清空时，关闭浏览器窗口
    if (GetBoxCount() == 0) {
        this->CloseWnd(kBrowserBoxClose);
    }
    return true;
}

BrowserBox* BrowserForm::GetSelectedBox()
{
    return m_pActiveBrowserBox;
}

void BrowserForm::SetActiveBox(const std::string& browserId)
{
    if (browserId.empty()) {
        return;
    }

    if (IsWindowMinimized()) {
        ShowWindow(kSW_RESTORE);
    }
    else {
        ShowWindow(kSW_SHOW);
    }

    // 从窗口左侧会话列表找到要激活的浏览器盒子项
    DString id = ui::StringConvert::UTF8ToT(browserId);
    TabCtrlItem* pTabItem = FindTabItem(id);
    if (nullptr == pTabItem) {
        return;
    }

    // 如果pTabItem处于隐藏状态，则无法顺利触发选择事件，所以这里直接切换到目标浏览器盒子
    pTabItem->Selected(true, false, 0);
    ChangeToBox(id);
}

bool BrowserForm::IsActiveBox(const BrowserBox* pBrowserBox)
{
    ASSERT(nullptr != pBrowserBox);
    return (pBrowserBox == m_pActiveBrowserBox && IsWindowForeground() && !IsWindowMinimized() && IsWindowVisible());
}

bool BrowserForm::IsActiveBox(const DString& browserId)
{
    ASSERT(!browserId.empty());
    return (IsWindowForeground() && !IsWindowMinimized() && IsWindowVisible() && FindBox(browserId) == m_pActiveBrowserBox);
}

int32_t BrowserForm::GetBoxCount() const
{
    int32_t nBoxCount = 0;
    if (m_pTabCtrl != nullptr) {
        nBoxCount = (int32_t)m_pTabCtrl->GetItemCount();
    }
    return nBoxCount;
}

bool BrowserForm::OnTabItemSelected(const ui::EventArgs& param)
{
    if (kEventSelect == param.eventType) {
        ASSERT(param.GetSender() == m_pTabCtrl);
        if (m_pTabCtrl != nullptr) {
            // 如果单击了顶部的标签，则找到下方Tab里对应的浏览器盒子并选中
            Control* pSelectedItem = m_pTabCtrl->GetItemAt(m_pTabCtrl->GetCurSel());
            ASSERT(pSelectedItem != nullptr);
            if (pSelectedItem != nullptr) {
                DString session_id = pSelectedItem->GetName();
                ChangeToBox(session_id);
            }
        }
    }
    return true;
}

bool BrowserForm::OnTabItemClose(const ui::EventArgs& param, const std::string& browserId)
{
    CloseBox(browserId);
    return true;
}

BrowserBox* BrowserForm::FindBox(const DString& browserId)
{
    for (int i = 0; i < (int)m_pBorwserBoxTab->GetItemCount(); i++) {
        Control *pBoxItem = m_pBorwserBoxTab->GetItemAt(i);
        ASSERT(nullptr != pBoxItem);
        if (nullptr == pBoxItem) {
            return nullptr;
        }

        if (pBoxItem->GetName() == browserId) {
            return dynamic_cast<BrowserBox*>(pBoxItem);
        }
    }
    return nullptr;
}

TabCtrlItem* BrowserForm::FindTabItem(const DString& browserId)
{
    for (int i = 0; i < GetBoxCount(); i++) {
        Control *pTabItem = m_pTabCtrl->GetItemAt(i);
        ASSERT(nullptr != pTabItem);
        if (nullptr == pTabItem) {
            return nullptr;
        }

        if (pTabItem->GetName() == browserId) {
            return dynamic_cast<TabCtrlItem*>(pTabItem);
        }
    }
    return nullptr;
}

void BrowserForm::SetTabItemName(const DString& browserId, const DString& name)
{
    TabCtrlItem* pTabItem = FindTabItem(browserId);
    if (nullptr != pTabItem) {
        pTabItem->SetTitle(name);
    }
}

void BrowserForm::SetURL(const std::string& browserId, const DString& url)
{
    if ((m_pEditUrl != nullptr) && (m_pActiveBrowserBox != nullptr) && (m_pActiveBrowserBox->GetBrowserId() == browserId)) {
        m_pEditUrl->SetText(url);
    }
}

bool BrowserForm::ChangeToBox(const DString& browserId)
{
    if (browserId.empty()) {
        return false;
    }

    BrowserBox* pBoxItem = FindBox(browserId);
    if (nullptr == pBoxItem) {
        return false;
    }
    pBoxItem->SetPos(pBoxItem->GetPos());
    m_pBorwserBoxTab->SelectItem(pBoxItem);
    pBoxItem->SetFocus();
    m_pActiveBrowserBox = pBoxItem;

    // 根据当前激活的浏览器盒子，更新任务栏的图标和标题
    DString url = m_pActiveBrowserBox->GetWebView2Control()->GetUrl();
    m_pEditUrl->SetText(url);
    OnLoadingStateChange(m_pActiveBrowserBox);
    return true;
}

void BrowserForm::NotifyFavicon(const BrowserBox* pBrowserBox, int32_t nWidth, int32_t nHeight, const std::vector<uint8_t>& imageData)
{
    if ((pBrowserBox == nullptr) || imageData.empty()) {
        return;
    }
    if ((nWidth < 1) || (nHeight < 1)) {
        return;
    }
    if (imageData.size() != nHeight * nWidth * 4) {
        return;
    }

    DString id = ui::StringConvert::UTF8ToT(pBrowserBox->GetBrowserId());
    TabCtrlItem* pTabItem = FindTabItem(id);
    if (pTabItem == nullptr) {
        return;
    }    
    pTabItem->SetIconData(nWidth, nHeight, imageData.data(), (int32_t)imageData.size());
}
