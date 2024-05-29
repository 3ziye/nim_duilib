#include "app_item.h"
#include "app_db.h"

AppItemUi* AppItemUi::Create(const AppItem& item, Box* p)
{
    ASSERT(p != nullptr);
    if (p == nullptr) {
        return nullptr;
    }
    ASSERT(p->GetWindow() != nullptr);
    AppItemUi* uiItem = new AppItemUi(p->GetWindow());
    uiItem->SetAppdata(item, false);
    ui::GlobalManager::Instance().FillBoxWithCache(uiItem, _T("move_control/app_item.xml"));
    return uiItem;
}

AppItemUi::AppItemUi(ui::Window* pWindow) :
    ui::ControlDragableT<ui::VBox>(pWindow)
{
}

void AppItemUi::OnInit()
{
    if (IsInited()) {
        return;
    }
    __super::OnInit();
    m_app_icon = static_cast<ui::Control*>(FindSubControl(_T("app_icon")));
    if (m_app_icon) {
        m_app_icon->SetBkImage(m_app_data.m_icon);
    }
    m_app_name = static_cast<ui::Label*>(FindSubControl(_T("app_name")));
    if (m_app_name) {
        m_app_name->SetText(m_app_data.m_name);
    }

    //绑定事件
}

void AppItemUi::SetAppdata(const AppItem& item, bool refresh)
{
    m_app_data = item;
    if (refresh) {
        if (m_app_icon) {
            m_app_icon->SetBkImage(m_app_data.m_icon);
        }
        if (m_app_name) {
            m_app_name->SetText(m_app_data.m_name);
        }
    }
}

ui::Control* AppItemUi::CreateDestControl(Box* pTargetBox)
{
    Control* pControl = __super::CreateDestControl(pTargetBox);
    if (pControl != nullptr) {
        pControl->SetBkImage(_T("file='move_control/1.png'"));
    }
    return pControl;
}

void AppItemUi::OnItemBoxChanged(Box* /*pOldBox*/, size_t /*nOldItemIndex*/,
                                 Box* pNewBox, size_t /*nNewItemIndex*/)
{
    if (pNewBox == m_pFrequentBox) {
        //常用应用
        m_app_data.m_isFrequent = true;
    }
    else {
        //我的应用
        m_app_data.m_isFrequent = false;
    }
    CAppDb::GetInstance().SaveToDb(m_app_data);
}

void AppItemUi::SetFrequentBox(ui::Box* pFrequentBox)
{
    m_pFrequentBox = pFrequentBox;
}
