#include "ListCtrlItem.h"
#include "duilib/Control/ListCtrlSubItem.h"
#include "duilib/Control/ListCtrl.h"

namespace ui
{
ListCtrlItem::ListCtrlItem():
    m_bSelectable(true),
    m_pListCtrl(nullptr),
    m_nCheckBoxPadding(0)
{
}

ListCtrlItem::~ListCtrlItem()
{
}

std::wstring ListCtrlItem::GetType() const { return L"ListCtrlItem"; }

bool ListCtrlItem::IsSelectableType() const
{
    return m_bSelectable;
}

void ListCtrlItem::SetSelectableType(bool bSelectable)
{
    m_bSelectable = bSelectable;
}

void ListCtrlItem::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

ListCtrl* ListCtrlItem::GetListCtrl() const
{
    ASSERT(m_pListCtrl != nullptr);
    return m_pListCtrl;
}

size_t ListCtrlItem::GetSubItemCount() const
{
    return GetItemCount();
}

ListCtrlSubItem* ListCtrlItem::GetSubItem(size_t columnIndex) const
{
    ASSERT(columnIndex < GetSubItemCount());
    ListCtrlSubItem* pSubItem = dynamic_cast<ListCtrlSubItem*>(GetItemAt(columnIndex));
    ASSERT(pSubItem != nullptr);
    return pSubItem;
}

void ListCtrlItem::Selected(bool bSelect, bool bTriggerEvent)
{
    if (__super::IsSelected() != bSelect) {
        __super::Selected(bSelect, bTriggerEvent);
    }
}

void ListCtrlItem::Activate()
{
    //��д�����ʵ���߼�������ֻ����һ��Click�¼�
    if (IsActivatable()) {
        SendEvent(kEventClick);
    }
}

bool ListCtrlItem::ButtonDown(const EventArgs& msg)
{
    if (IsEnabled() && IsActivatable() && IsPointInWithScrollOffset(msg.ptMouse)) {
        uint64_t vkFlag = kVkLButton;
#ifdef UILIB_IMPL_WINSDK
        if (msg.wParam & MK_CONTROL) {
            vkFlag |= kVkControl;
        }
        if (msg.wParam & MK_SHIFT) {
            vkFlag |= kVkShift;
        }
#endif
        //������µ�ʱ��ѡ��
        SelectItem(vkFlag);
    }
    return __super::ButtonDown(msg);
}

bool ListCtrlItem::ButtonUp(const EventArgs& msg)
{
    if ((m_pListCtrl != nullptr) && m_pListCtrl->IsAutoCheckSelect()) {
        //����CheckBox�Ĵ����������⹦�ܳ�ͻ
        return HBox::ButtonUp(msg);
    }
    else {
        return __super::ButtonUp(msg);
    }
}

bool ListCtrlItem::ButtonDoubleClick(const EventArgs& msg)
{
    return __super::ButtonDoubleClick(msg);
}

bool ListCtrlItem::RButtonDown(const EventArgs& msg)
{
    uint64_t vkFlag = kVkRButton;
#ifdef UILIB_IMPL_WINSDK
    if (msg.wParam & MK_CONTROL) {
        vkFlag |= kVkControl;
    }
    if (msg.wParam & MK_SHIFT) {
        vkFlag |= kVkShift;
    }
#endif
    //������µ�ʱ��ѡ��
    SelectItem(vkFlag);
    return __super::RButtonDown(msg);
}

bool ListCtrlItem::RButtonUp(const EventArgs& msg)
{
    return __super::RButtonUp(msg);
}

bool ListCtrlItem::RButtonDoubleClick(const EventArgs& msg)
{
    return __super::RButtonDoubleClick(msg);
}

void ListCtrlItem::SelectItem(uint64_t vkFlag)
{
    IListBoxOwner* pOwner = GetOwner();
    ASSERT(pOwner != nullptr);
    if (pOwner != nullptr) {
        size_t nListBoxIndex = GetListBoxIndex();
        pOwner->SelectItem(nListBoxIndex, true, true, vkFlag);
    }
}

bool ListCtrlItem::SupportCheckedMode() const
{
    return true;
}

bool ListCtrlItem::SetShowCheckBox(bool bShow)
{
    bool bRet = false;
    if (bShow) {
        if (IsShowCheckBox()) {
            return true;
        }
        ListCtrl* pListCtrl = GetListCtrl();
        if (pListCtrl != nullptr) {
            std::wstring checkBoxClass = pListCtrl->GetCheckBoxClass();
            if (!checkBoxClass.empty()) {
                SetClass(checkBoxClass);
                bRet = IsShowCheckBox();
                if (bRet) {
                    //��������Paddingֵ������CheckBox�������ص�
                    int32_t nWidth = pListCtrl->GetCheckBoxPadding();
                    if (nWidth > 0) {
                        UiPadding rcPadding = GetPadding();
                        if (rcPadding.left < nWidth) {
                            rcPadding.left = nWidth;
                            SetPadding(rcPadding, false);
                            m_nCheckBoxPadding = nWidth;
                        }
                    }
                }
            }
        }
    }
    else {
        //���CheckBoxͼƬ��Դ���Ͳ���ʾ��
        ClearStateImages();
        ASSERT(!IsShowCheckBox());
        if (m_nCheckBoxPadding > 0) {
            UiPadding rcPadding = GetPadding();
            if (rcPadding.left >= m_nCheckBoxPadding) {
                rcPadding.left -= m_nCheckBoxPadding;
                SetPadding(rcPadding, false);
            }
            m_nCheckBoxPadding = 0;
        }
        bRet = true;
    }
    return bRet;
}

bool ListCtrlItem::IsShowCheckBox() const
{
    //�����CheckBoxͼƬ��Դ������Ϊ��ʾ��CheckBox
    return !GetStateImage(kControlStateNormal).empty() && !GetSelectedStateImage(kControlStateNormal).empty();
}

}//namespace ui

