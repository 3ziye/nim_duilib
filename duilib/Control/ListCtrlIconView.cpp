#include "ListCtrlIconView.h" 
#include "duilib/Control/ListCtrl.h"
#include "duilib/Control/ListCtrlData.h"

namespace ui
{
ListCtrlIconView::ListCtrlIconView(bool bListMode):
    VirtualListBox(new VirtualVTileLayout),
    m_pListCtrl(nullptr),
    m_pData(nullptr),
    m_bListMode(bListMode),
    m_bMouseDown(false),
    m_bRMouseDown(false),
    m_bInMouseMove(false),
    m_pMouseSender(nullptr),
    m_bEnableFrameSelection(true),
    m_nLastNoShiftIndex(0),
    m_bMouseDownInView(false)
{
    if (bListMode) {
        Layout* pLayout = GetLayout();
        VirtualHTileLayout* pNewLayout = new VirtualHTileLayout;
        ReSetLayout(pNewLayout);
        SetVirtualLayout(pNewLayout);
        if (pLayout) {
            delete pLayout;
            pLayout = nullptr;
        }        
    }
    else {
        VirtualVTileLayout* pDataLayout = dynamic_cast<VirtualVTileLayout*>(GetLayout());
        ASSERT(pDataLayout != nullptr);
        VirtualLayout* pVirtualLayout = pDataLayout;
        SetVirtualLayout(pVirtualLayout);
    }    
}

ListCtrlIconView::~ListCtrlIconView()
{
}

void ListCtrlIconView::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"horizontal_layout") {
        SetHorizontalLayout(strValue == L"true");
    }
    else if (strName == L"enable_frame_selection") {
        m_bEnableFrameSelection = (strValue == L"true");
    }
    else if (strName == L"frame_selection_color") {
        m_frameSelectionColor = strValue;
    }
    else if (strName == L"frame_selection_alpha") {
        m_frameSelectionAlpha = (uint8_t)_wtoi(strValue.c_str());
    }
    else if (strName == L"frame_selection_border") {
        m_frameSelectionBorderSize = (uint8_t)_wtoi(strValue.c_str());
    }
    else if (strName == L"frame_selection_border_color") {
        m_frameSelectionBorderColor = strValue;
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrlIconView::SetHorizontalLayout(bool bHorizontal)
{
    if (bHorizontal) {
        //���򲼾�        
        if (dynamic_cast<VirtualHTileLayout*>(GetLayout()) == nullptr) {
            Layout* pLayout = GetLayout();
            VirtualVTileLayout* pOldLayout = dynamic_cast<VirtualVTileLayout*>(pLayout);
            VirtualHTileLayout* pNewLayout = new VirtualHTileLayout;
            ReSetLayout(pNewLayout);
            SetVirtualLayout(pNewLayout);
            if (pOldLayout != nullptr) {
                //ͬ������
                pNewLayout->SetItemSize(pOldLayout->GetItemSize());
                pNewLayout->SetRows(pOldLayout->GetColumns());
                pNewLayout->SetAutoCalcRows(pOldLayout->IsAutoCalcColumns());
                pNewLayout->SetChildMarginX(pOldLayout->GetChildMarginX());
                pNewLayout->SetChildMarginY(pOldLayout->GetChildMarginY());
                pNewLayout->SetScaleDown(pOldLayout->IsScaleDown());
            }
            if (pLayout) {
                delete pLayout;
                pLayout = nullptr;
            }
        }
    }
    else {
        //���򲼾�
        if (dynamic_cast<VirtualVTileLayout*>(GetLayout()) == nullptr) {
            Layout* pLayout = GetLayout();
            VirtualHTileLayout* pOldLayout = dynamic_cast<VirtualHTileLayout*>(pLayout);
            VirtualVTileLayout* pNewLayout = new VirtualVTileLayout;
            ReSetLayout(pNewLayout);
            SetVirtualLayout(pNewLayout);
            if (pOldLayout != nullptr) {
                //ͬ������
                pNewLayout->SetItemSize(pOldLayout->GetItemSize());
                pNewLayout->SetColumns(pOldLayout->GetRows());
                pNewLayout->SetAutoCalcColumns(pOldLayout->IsAutoCalcRows());
                pNewLayout->SetChildMarginX(pOldLayout->GetChildMarginX());
                pNewLayout->SetChildMarginY(pOldLayout->GetChildMarginY());
                pNewLayout->SetScaleDown(pOldLayout->IsScaleDown());
            }
            if (pLayout) {
                delete pLayout;
                pLayout = nullptr;
            }
        }
    }
}

bool ListCtrlIconView::IsHorizontalLayout() const
{
    VirtualHTileLayout* pDataLayout = dynamic_cast<VirtualHTileLayout*>(GetLayout());
    return pDataLayout != nullptr;
}

void ListCtrlIconView::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

void ListCtrlIconView::SetDataProvider(VirtualListBoxElement* pProvider)
{
    __super::SetDataProvider(pProvider);
    m_pData = dynamic_cast<ListCtrlData*>(GetDataProvider());
}

void ListCtrlIconView::SendEvent(EventType eventType, WPARAM wParam, LPARAM lParam, TCHAR tChar, const UiPoint& mousePos)
{
    __super::SendEvent(eventType, wParam, lParam, tChar, mousePos);
}

void ListCtrlIconView::SendEvent(const EventArgs& event)
{
    __super::SendEvent(event);
    if ((event.Type == kEventSelect) || (event.Type == kEventUnSelect)) {
        SendEvent(kEventSelChange);
    }
}

void ListCtrlIconView::DoInit()
{
    //��ֹ�������ֵĹ����ı�ѡ����
    SetScrollSelect(false);
}

Control* ListCtrlIconView::CreateDataItem()
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nullptr;
    }
    Control* pControl = nullptr;
    if (m_bListMode) {
        //�б���ͼ: ˮƽ����
        ListCtrlListViewItem* pItem = new ListCtrlListViewItem;
        pControl = pItem;
        pControl->SetClass(m_pListCtrl->GetListViewItemClass());
        Control* pItemImage = new Control;
        Label* pItemLabel = new Label;
        pItemImage->SetMouseEnabled(false);
        pItemLabel->SetMouseEnabled(false);
        pItem->AddItem(pItemImage);
        pItem->AddItem(pItemLabel);
    }
    else {
        //ͼ����ͼ����ֱ����
        ListCtrlIconViewItem* pItem = new ListCtrlIconViewItem;
        pControl = pItem;
        pControl->SetClass(m_pListCtrl->GetIconViewItemClass());
        Control* pItemImage = new Control;
        Label* pItemLabel = new Label;
        pItemImage->SetMouseEnabled(false);
        pItemLabel->SetMouseEnabled(false);
        pItem->AddItem(pItemImage);
        pItem->AddItem(pItemLabel);
    }
    if (pControl != nullptr) {
        //��������¼�
        pControl->AttachButtonDown([this](const EventArgs& args) {
            OnButtonDown(args.ptMouse, args.pSender);
            return true;
            });
        pControl->AttachButtonUp([this](const EventArgs& args) {
            OnButtonUp(args.ptMouse, args.pSender);
            return true;
            });
        pControl->AttachRButtonDown([this](const EventArgs& args) {
            OnRButtonDown(args.ptMouse, args.pSender);
            return true;
            });
        pControl->AttachRButtonUp([this](const EventArgs& args) {
            OnButtonUp(args.ptMouse, args.pSender);
            return true;
            });
        pControl->AttachMouseMove([this](const EventArgs& args) {
            OnMouseMove(args.ptMouse, args.pSender);
            return true;
            });
        pControl->AttachWindowKillFocus([this](const EventArgs&) {
            OnWindowKillFocus();
            return true;
            });
    }
    return pControl;
}

bool ListCtrlIconView::FillDataItem(Control* pControl,
                                    size_t /*nElementIndex*/,
                                    const ListCtrlItemData& itemData,
                                    const std::vector<ListCtrlSubItemData2Pair>& subItemList)
{
    ASSERT((pControl != nullptr) && (m_pListCtrl != nullptr));
    if ((pControl == nullptr) || (m_pListCtrl == nullptr)) {
        return false;
    }
    ListCtrlSubItemData2Ptr pSubItemData;
    int32_t nImageId = -1;
    size_t nColumnId = m_pListCtrl->GetColumnId(0); //ȡ��һ�е�ID
    for (const ListCtrlSubItemData2Pair& pair : subItemList) {
        if (pair.nColumnId == nColumnId) {
            if (pair.pSubItemData != nullptr) {
                nImageId = pair.pSubItemData->nImageId;
                pSubItemData = pair.pSubItemData;
            }
            break;
        }
    }
    if (nImageId < 0) {
        //�����û������ͼ�꣬��ȡ�е�
        nImageId = itemData.nImageId;
    }
    Box* pItemBox = dynamic_cast<Box*>(pControl);
    ASSERT(pItemBox != nullptr);
    if (pItemBox == nullptr) {
        return false;
    }

    Control* pItemImage = pItemBox->GetItemAt(0);
    Label* pItemLabel = dynamic_cast<Label*>(pItemBox->GetItemAt(1));
    ASSERT((pItemImage != nullptr) && (pItemLabel != nullptr));
    if ((pItemImage == nullptr) || (pItemLabel == nullptr)) {
        return false;
    }

    if (m_bListMode) {//�б���ͼ
        //ͼ�������
        pItemImage->SetClass(m_pListCtrl->GetListViewItemImageClass());
        //�ı�������
        pItemLabel->SetClass(m_pListCtrl->GetListViewItemLabelClass());
    }
    else {//ͼ����ͼ
        //ͼ�������
        pItemImage->SetClass(m_pListCtrl->GetIconViewItemImageClass());
        //�ı�������
        pItemLabel->SetClass(m_pListCtrl->GetIconViewItemLabelClass());
    }

    std::wstring imageString;
    UiSize imageSize;
    if (nImageId >= 0) {
        ImageList* pImageList = nullptr;
        if (m_bListMode) {
            pImageList = m_pListCtrl->GetImageList(ListCtrlType::List);
        }
        else {
            pImageList = m_pListCtrl->GetImageList(ListCtrlType::Icon);
        }
        if (pImageList == nullptr) {
            pImageList = m_pListCtrl->GetImageList(ListCtrlType::Report);
        }
        if (pImageList != nullptr) {
            imageString = pImageList->GetImageString(nImageId);
            imageSize = pImageList->GetImageSize();
        }
    }
    pItemImage->SetBkImage(imageString);
    if (imageSize.cx > 0) {
        pItemImage->SetFixedWidth(UiFixedInt(imageSize.cx), false, false);
    }
    if (imageSize.cy > 0) {
        pItemImage->SetFixedHeight(UiFixedInt(imageSize.cy), false, false);
    }
    if (pSubItemData != nullptr) {
        pItemLabel->SetText(pSubItemData->text.c_str());
    }
    else {
        pItemLabel->SetText(L"");
    }
    return true;
}

int32_t ListCtrlIconView::GetMaxDataItemWidth(const std::vector<ListCtrlSubItemData2Ptr>& /*subItemList*/)
{
    //����Ҫʵ��
    return -1;
}

size_t ListCtrlIconView::GetTopElementIndex() const
{
    std::vector<size_t> itemIndexList;
    GetDisplayElements(itemIndexList);
    if (itemIndexList.empty()) {
        return Box::InvalidIndex;
    }
    else {
        return itemIndexList.front();
    }
}

bool ListCtrlIconView::IsDataItemDisplay(size_t itemIndex) const
{
    std::vector<size_t> itemIndexList;
    GetDisplayElements(itemIndexList);
    return std::find(itemIndexList.begin(), itemIndexList.end(), itemIndex) != itemIndexList.end();
}

void ListCtrlIconView::GetDisplayDataItems(std::vector<size_t>& itemIndexList) const
{
    itemIndexList.clear();
    GetDisplayElements(itemIndexList);
}

bool ListCtrlIconView::EnsureDataItemVisible(size_t itemIndex, bool bToTop)
{
    EnsureVisible(itemIndex, bToTop);
    return true;
}

bool ListCtrlIconView::ButtonDown(const EventArgs& msg)
{
    bool bRet = __super::ButtonDown(msg);
    OnButtonDown(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlIconView::ButtonUp(const EventArgs& msg)
{
    bool bRet = __super::ButtonUp(msg);
    OnButtonUp(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlIconView::RButtonDown(const EventArgs& msg)
{
    bool bRet = __super::RButtonDown(msg);
    OnRButtonDown(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlIconView::RButtonUp(const EventArgs& msg)
{
    bool bRet = __super::RButtonUp(msg);
    OnRButtonUp(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlIconView::MouseMove(const EventArgs& msg)
{
    bool bRet = __super::MouseMove(msg);
    OnMouseMove(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlIconView::OnWindowKillFocus(const EventArgs& msg)
{
    bool bRet = __super::OnWindowKillFocus(msg);
    OnWindowKillFocus();
    return bRet;
}

void ListCtrlIconView::OnButtonDown(const UiPoint& ptMouse, Control* pSender)
{
    if (m_bInMouseMove) {
        m_bInMouseMove = false;
        Invalidate();
    }
    m_bMouseDownInView = (pSender == this) ? true : false;
    m_bMouseDown = true;
    m_pMouseSender = pSender;
    UiSize64 scrollPos = GetScrollPos();
    m_ptMouseDown.cx = ptMouse.x + scrollPos.cx;
    m_ptMouseDown.cy = ptMouse.y + scrollPos.cy;
}

void ListCtrlIconView::OnButtonUp(const UiPoint& /*ptMouse*/, Control* pSender)
{
    bool bClickedBlank = false;
    if (m_bMouseDownInView && !m_bInMouseMove && (pSender == this)) {
        bClickedBlank = true;
    }
    if (m_bInMouseMove) {
        m_bInMouseMove = false;
        Invalidate();
    }
    m_bMouseDownInView = false;
    m_bMouseDown = false;
    m_pMouseSender = nullptr;
    if (bClickedBlank) {
        if (OnListCtrlClickedBlank()) {
            SendEvent(kEventSelChange);
        }
    }
}

void ListCtrlIconView::OnRButtonDown(const UiPoint& ptMouse, Control* pSender)
{
    if (m_bInMouseMove) {
        m_bInMouseMove = false;
        Invalidate();
    }
    m_bMouseDownInView = (pSender == this) ? true : false;
    m_bRMouseDown = true;
    m_pMouseSender = pSender;
    UiSize64 scrollPos = GetScrollPos();
    m_ptMouseDown.cx = ptMouse.x + scrollPos.cx;
    m_ptMouseDown.cy = ptMouse.y + scrollPos.cy;
}

void ListCtrlIconView::OnRButtonUp(const UiPoint& /*ptMouse*/, Control* pSender)
{
    bool bClickedBlank = false;
    if (m_bMouseDownInView && !m_bInMouseMove && (pSender == this)) {
        bClickedBlank = true;
    }
    if (m_bInMouseMove) {
        m_bInMouseMove = false;
        Invalidate();
    }
    m_bMouseDownInView = false;
    m_bRMouseDown = false;
    m_pMouseSender = nullptr;
    if (bClickedBlank) {
        if (OnListCtrlClickedBlank()) {
            SendEvent(kEventSelChange);
        }
    }
}

void ListCtrlIconView::OnMouseMove(const UiPoint& ptMouse, Control* pSender)
{
    if (!m_bEnableFrameSelection || !IsMultiSelect()) {
        //���ܹر� ���� ��ѡģʽ
        return;
    }
    if ((m_bMouseDown || m_bRMouseDown) &&
        (pSender != nullptr) &&
        (m_pMouseSender == pSender) && pSender->IsMouseFocused()) {
        UiSize64 scrollPos = GetScrollPos();
        m_ptMouseMove.cx = ptMouse.x + scrollPos.cx;
        m_ptMouseMove.cy = ptMouse.y + scrollPos.cy;
        //���������ͼ������������ڹ������λ��
        m_bInMouseMove = true;
        OnCheckScrollView();
    }
    else if (m_bInMouseMove) {
        m_bInMouseMove = false;
        Invalidate();
    }
}

void ListCtrlIconView::OnWindowKillFocus()
{
    if (m_bInMouseMove) {
        Invalidate();
    }
    m_bMouseDownInView = false;
    m_bMouseDown = false;
    m_bRMouseDown = false;
    m_bInMouseMove = false;
    m_pMouseSender = nullptr;
}

bool ListCtrlIconView::OnListCtrlClickedBlank()
{
    //�ڿհ״���������������Ҽ���ȡ��ȫ��ѡ��
    return SetSelectNone();
}

void ListCtrlIconView::OnCheckScrollView()
{
    if (!m_bInMouseMove) {
        //ȡ����ʱ��
        m_scrollViewFlag.Cancel();
        return;
    }
    bool bScrollView = false;
    const UiSize64 scrollPos = GetScrollPos();
    UiSize64 pt = m_ptMouseMove;
    pt.cx -= scrollPos.cx;
    pt.cy -= scrollPos.cy;
    const UiSize64 ptMouseMove = pt; //��¼ԭֵ

    if (m_bInMouseMove) {
        int32_t nHScrollValue = DUI_NOSET_VALUE;
        int32_t nVScrollValue = DUI_NOSET_VALUE;
        GetScrollDeltaValue(nHScrollValue, nVScrollValue);
        UiRect viewRect = GetRect();
        if (pt.cx <= viewRect.left) {
            //���������ͼ
            LineLeft(nHScrollValue);
            bScrollView = true;
        }
        else if (pt.cx >= viewRect.right) {
            //���ҹ�����ͼ
            LineRight(nHScrollValue);
            bScrollView = true;
        }
        if (pt.cy <= viewRect.top) {
            //���Ϲ�����ͼ
            LineUp(nVScrollValue, false);
            bScrollView = true;
        }
        else if (pt.cy >= viewRect.bottom) {
            //���¹�����ͼ
            LineDown(nVScrollValue, false);
            bScrollView = true;
        }
    }

    if (bScrollView) {
        UiSize64 scrollPosNew = GetScrollPos();
        if (scrollPos != scrollPosNew) {
            //�������λ��
            m_ptMouseMove.cx = ptMouseMove.cx + scrollPosNew.cx;
            m_ptMouseMove.cy = ptMouseMove.cy + scrollPosNew.cy;
        }

        //������ʱ��
        m_scrollViewFlag.Cancel();
        GlobalManager::Instance().Timer().AddCancelableTimer(m_scrollViewFlag.GetWeakFlag(),
                                    nbase::Bind(&ListCtrlIconView::OnCheckScrollView, this),
                                    50, 1); //ִֻ��һ��
    }
    else {
        //ȡ����ʱ��
        m_scrollViewFlag.Cancel();
    }

    int64_t top = std::min(m_ptMouseDown.cy, m_ptMouseMove.cy);
    int64_t bottom = std::max(m_ptMouseDown.cy, m_ptMouseMove.cy);
    int64_t left = std::min(m_ptMouseDown.cx, m_ptMouseMove.cx);
    int64_t right = std::max(m_ptMouseDown.cx, m_ptMouseMove.cx);
    int32_t offsetTop = GetRect().top;//��ǰ�ؼ����Ͻǵ�top����
    top -= offsetTop;
    bottom -= offsetTop;

    int32_t offsetLeft = GetRect().left;//��ǰ�ؼ����Ͻǵ�left����
    left -= offsetLeft;
    right -= offsetLeft;
    bool bRet = OnFrameSelection(left, right, top, bottom);
    Invalidate();
    if (bRet) {
        SendEvent(kEventSelChange);
    }
}

void ListCtrlIconView::GetScrollDeltaValue(int32_t& nHScrollValue, int32_t& nVScrollValue) const
{
    nHScrollValue = DUI_NOSET_VALUE;
    nVScrollValue = DUI_NOSET_VALUE;
    if (IsHorizontalLayout()) {
        //���򲼾�
        VirtualHTileLayout* pLayout = dynamic_cast<VirtualHTileLayout*>(GetLayout());
        if (pLayout != nullptr) {
            UiSize szItem = pLayout->GetItemSize();
            int32_t deltaValue = szItem.cx * 2; 
            if (deltaValue > 0) {
                deltaValue = std::max(GetRect().Width() / 3, deltaValue);
                nHScrollValue = deltaValue;
            }
        }
    }
    else {
        //���򲼾�
        VirtualVTileLayout* pLayout = dynamic_cast<VirtualVTileLayout*>(GetLayout());
        if (pLayout != nullptr) {
            UiSize szItem = pLayout->GetItemSize();
            int32_t deltaValue = szItem.cy * 2;
            if (deltaValue > 0) {
                deltaValue = std::max(GetRect().Height() / 3, deltaValue);
                nHScrollValue = deltaValue;
            }
        }
    }
}

bool ListCtrlIconView::OnFrameSelection(int64_t left, int64_t right, int64_t top, int64_t bottom)
{
    ASSERT((top <= bottom) && (left <= right));
    if ((top > bottom) && (left > right)) {
        return false;
    }
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return false;
    }
    ListCtrlData* pDataProvider = m_pData;
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return false;
    }
    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    const size_t dataItemCount = itemDataList.size();
    if (dataItemCount == 0) {
        return false;
    }
    Layout* pLayout = GetLayout();
    VirtualHTileLayout* pHLayout = dynamic_cast<VirtualHTileLayout*>(pLayout);
    VirtualVTileLayout* pVLayout = dynamic_cast<VirtualVTileLayout*>(pLayout);
    if ((pHLayout == nullptr) && (pVLayout == nullptr)) {
        return false;
    }
    else if ((pHLayout != nullptr) && (pVLayout != nullptr)) {
        return false;
    }

    UiSize szItem;
    if (pHLayout != nullptr) {
        szItem = pHLayout->GetItemSize();
    }
    else {
        szItem = pVLayout->GetItemSize();
    }
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return false;
    }

    int64_t iLeft = 0;
    int64_t iTop = 0;
    int64_t iRight = 0;
    int64_t iBottom = 0;

    int64_t cLeft = 0;
    int64_t cTop = 0;
    int64_t cRight = 0;
    int64_t cBottom = 0;

    bool bRet = false;
    if (pHLayout != nullptr) {
        //���򲼾�
        int32_t childMarginY = pHLayout->GetChildMarginY();
        if (childMarginY < 0) {
            childMarginY = 0;
        }
        int32_t nRows = CalcRows();

        //��֧������
        std::vector<size_t> itemIndexList;
        int32_t childMarginX = pHLayout->GetChildMarginX();
        if (childMarginX < 0) {
            childMarginX = 0;
        }
        int64_t nStartIndex = (left / (szItem.cx + childMarginX)) * nRows;
        const size_t nCount = GetElementCount();
        for (size_t nElemenetIndex = nStartIndex; nElemenetIndex < nCount; ++nElemenetIndex) {
            CalcElementRectH(nElemenetIndex, szItem, nRows, childMarginX, childMarginY,
                             iLeft, iTop, iRight, iBottom);
            cLeft = std::max(left, iLeft);
            cTop = std::max(top, iTop);
            cRight = std::min(right, iRight);
            cBottom = std::min(bottom, iBottom);
            if ((cRight > cLeft) && (cBottom > cTop)) {
                itemIndexList.push_back(nElemenetIndex);
            }
            if (iLeft > right) {
                break;
            }
        }
        bRet = SetSelectedElements(itemIndexList, true);
    }
    else {
        //���򲼾�
        int32_t childMarginX = pVLayout->GetChildMarginX();
        if (childMarginX < 0) {
            childMarginX = 0;
        }
        int32_t nColumns = CalcColumns();

        //��֧������
        std::vector<size_t> itemIndexList;
        int32_t childMarginY = pVLayout->GetChildMarginY();
        if (childMarginY < 0) {
            childMarginY = 0;
        }
        int64_t nStartIndex = (top / (szItem.cy + childMarginY)) * nColumns;
        const size_t nCount = GetElementCount();
        for (size_t nElemenetIndex = nStartIndex; nElemenetIndex < nCount; ++nElemenetIndex) {
            CalcElementRectV(nElemenetIndex, szItem, nColumns, childMarginX, childMarginY,
                            iLeft, iTop, iRight, iBottom);
            cLeft = std::max(left, iLeft);
            cTop = std::max(top, iTop);
            cRight = std::min(right, iRight);
            cBottom = std::min(bottom, iBottom);
            if ((cRight > cLeft) && (cBottom > cTop)) {
                itemIndexList.push_back(nElemenetIndex);
            }
            if (iTop > bottom) {
                break;
            }
        }
        bRet = SetSelectedElements(itemIndexList, true);
    }
    return bRet;
}

int32_t ListCtrlIconView::CalcRows() const
{
    VirtualHTileLayout* pHLayout = dynamic_cast<VirtualHTileLayout*>(GetLayout());
    return CalcHTileRows(pHLayout);
}

int32_t ListCtrlIconView::CalcColumns() const
{
    VirtualVTileLayout* pVLayout = dynamic_cast<VirtualVTileLayout*>(GetLayout());
    return CalcVTileColumns(pVLayout);
}

void ListCtrlIconView::CalcElementRectV(size_t nElemenetIndex, const UiSize& szItem,
                                        int32_t nColumns, int32_t childMarginX, int32_t childMarginY,
                                        int64_t& iLeft, int64_t& iTop, 
                                        int64_t& iRight, int64_t& iBottom) const
{
    iLeft = 0;
    iRight = 0;
    iTop = 0;
    iBottom = 0;
    ASSERT(nColumns > 0);
    if (nColumns <= 0) {
        return;
    }
    if (childMarginX < 0) {
        childMarginX = 0;
    }
    if (childMarginY < 0) {
        childMarginY = 0;
    }
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return;
    }

    size_t nRowIndex = nElemenetIndex / nColumns;
    size_t nColumnIndex = nElemenetIndex % nColumns;

    iBottom = (nRowIndex + 1) * szItem.cy + nRowIndex * childMarginY;
    iTop = iBottom - szItem.cy;

    iRight = (nColumnIndex + 1) * szItem.cx + nColumnIndex * childMarginX;
    iLeft = iRight - szItem.cx;
}

void ListCtrlIconView::CalcElementRectH(size_t nElemenetIndex, const UiSize& szItem,
                                        int32_t nRows, int32_t childMarginX, int32_t childMarginY,
                                        int64_t& iLeft, int64_t& iTop,
                                        int64_t& iRight, int64_t& iBottom) const
{
    iLeft = 0;
    iRight = 0;
    iTop = 0;
    iBottom = 0;
    ASSERT(nRows > 0);
    if (nRows <= 0) {
        return;
    }
    if (childMarginX < 0) {
        childMarginX = 0;
    }
    if (childMarginY < 0) {
        childMarginY = 0;
    }
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return;
    }

    size_t nColumnIndex = nElemenetIndex / nRows;
    size_t nRowIndex = nElemenetIndex % nRows;    

    iRight = (nColumnIndex + 1) * szItem.cx + nColumnIndex * childMarginX;
    iLeft = iRight - szItem.cx;

    iBottom = (nRowIndex + 1) * szItem.cy + nRowIndex * childMarginY;
    iTop = iBottom - szItem.cy;
}

void ListCtrlIconView::PaintChild(IRender* pRender, const UiRect& rcPaint)
{
    __super::PaintChild(pRender, rcPaint);
    PaintFrameSelection(pRender);
}

void ListCtrlIconView::PaintFrameSelection(IRender* pRender)
{
    if (!m_bInMouseMove || (pRender == nullptr)) {
        return;
    }
    UiSize64 scrollPos = GetScrollPos();
    int64_t left = std::min(m_ptMouseDown.cx, m_ptMouseMove.cx) - scrollPos.cx;
    int64_t right = std::max(m_ptMouseDown.cx, m_ptMouseMove.cx) - scrollPos.cx;
    int64_t top = std::min(m_ptMouseDown.cy, m_ptMouseMove.cy) - scrollPos.cy;
    int64_t bottom = std::max(m_ptMouseDown.cy, m_ptMouseMove.cy) - scrollPos.cy;

    UiRect rect(TruncateToInt32(left), TruncateToInt32(top),
                TruncateToInt32(right), TruncateToInt32(bottom));

    if ((m_frameSelectionBorderSize > 0) && !m_frameSelectionBorderColor.empty()) {
        pRender->DrawRect(rect, GetUiColor(m_frameSelectionBorderColor.c_str()), m_frameSelectionBorderSize);
    }
    if (!m_frameSelectionColor.empty()) {
        pRender->FillRect(rect, GetUiColor(m_frameSelectionColor.c_str()), m_frameSelectionAlpha);
    }
}

void ListCtrlIconView::HandleEvent(const EventArgs& msg)
{
    if (IsDisabledEvents(msg)) {
        //�������������Ϣ�����ҿؼ���Disabled�ģ�ת�����ϲ�ؼ�
        Box* pParent = GetParent();
        if (pParent != nullptr) {
            pParent->SendEvent(msg);
        }
        else {
            __super::HandleEvent(msg);
        }
        return;
    }
    bool bHandled = false;
    if (msg.Type == kEventKeyDown) {
        bHandled = OnListCtrlKeyDown(msg);
    }
    if (!bHandled) {
        __super::HandleEvent(msg);
    }
}

bool ListCtrlIconView::OnListCtrlKeyDown(const EventArgs& msg)
{
    ASSERT(msg.Type == kEventKeyDown);
    bool bHandled = false;
    bool bCtrlADown = (msg.Type == kEventKeyDown) && ((msg.chKey == L'A') || (msg.chKey == L'a'));
    if (bCtrlADown) {
        //Ctrl + A ȫѡ����
        bHandled = true;
        bool bRet = SetSelectAll();
        if (bRet) {
            SendEvent(kEventSelChange);
        }
        return bHandled;
    }

    //���������
    bool bArrowKeyDown = (msg.Type == kEventKeyDown) &&
                         ((msg.chKey == VK_UP) || (msg.chKey == VK_DOWN) ||
                          (msg.chKey == VK_LEFT) || (msg.chKey == VK_RIGHT) ||
                          (msg.chKey == VK_PRIOR) || (msg.chKey == VK_NEXT) ||
                          (msg.chKey == VK_HOME) || (msg.chKey == VK_END));
    const size_t nElementCount = GetElementCount();
    if (!bArrowKeyDown || !IsMultiSelect() || (nElementCount == 0)) {
        //�ڷ����������Ϣ�������ݡ���֧�ֶ�ѡ������£���Ĭ�ϴ�������
        return bHandled;
    }

#ifdef UILIB_IMPL_WINSDK
    bool bShiftDown = ::GetAsyncKeyState(VK_SHIFT) < 0;
    bool bControlDown = ::GetAsyncKeyState(VK_CONTROL) < 0;
    bool bAltDown = ::GetAsyncKeyState(VK_MENU) < 0;
#else
    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;
#endif

    if (bAltDown || bControlDown) {
        //�����סCtrl�� ���� Alt����Ĭ������
        return bHandled;
    }

    // �������̴����������
    // �����ѡ����µķ���������������뵥ѡ�������ƣ���ѡ�������GetCurSel()ֵ��һ����ȷ����ҪУ׼
    size_t nCurSel = GetCurSel();
    if (nCurSel < GetItemCount()) {
        //�ж����Ƿ����ѡ���ö����ǲ���ѡ��ģ�Ӧ���Թ���
        Control* pControl = GetItemAt(nCurSel);
        if ((pControl == nullptr) || !pControl->IsVisible() || !pControl->IsSelectableType()) {
            nCurSel = Box::InvalidIndex;
        }
    }
    if (nCurSel >= GetItemCount()) {
        //���ҵ�ǰ��ͼ�ڵ�һ����ѡ�����Ŀ����Ϊ��ʼ��
        size_t nCount = GetItemCount();
        for (size_t index = 0; index < nCount; ++index) {
            Control* pControl = GetItemAt(index);
            if ((pControl == nullptr) || !pControl->IsVisible() || !pControl->IsSelectableType()) {
                continue;
            }
            IListBoxItem* pItem = dynamic_cast<IListBoxItem*>(pControl);
            if ((pItem != nullptr) && pItem->IsSelected()) {
                nCurSel = index;
                break;
            }
        }
    }
    if (nCurSel >= GetItemCount()) {
        //���Բ���δ�ڵ�ǰ��ͼ��ʾ����ѡ��Ԫ��
        size_t nDestItemIndex = Box::InvalidIndex;
        if (OnFindSelectable(GetCurSel(), SelectableMode::kSelect, 1, nDestItemIndex)) {
            nCurSel = GetCurSel();
            ASSERT(nCurSel == nDestItemIndex);
        }
    }
    if (nCurSel >= GetItemCount()) {
        //ȫ������δ�ɹ����Ե�ǰ��ͼ�ڵĵ�һ��Ԫ��Ϊ��ʼ��
        size_t nCount = GetItemCount();
        for (size_t index = 0; index < nCount; ++index) {
            Control* pControl = GetItemAt(index);
            if ((pControl == nullptr) || !pControl->IsVisible()) {
                continue;
            }
            IListBoxItem* pItem = dynamic_cast<IListBoxItem*>(pControl);
            if ((pItem != nullptr) &&
                (pItem->GetElementIndex() < nElementCount) &&
                pControl->IsSelectableType()) {
                nCurSel = index;
                SetCurSel(nCurSel);
                break;
            }
        }
    }

    size_t nIndexCurSel = Box::InvalidIndex;
    if (nCurSel < GetItemCount()) {
        nIndexCurSel = GetDisplayItemElementIndex(nCurSel);
    }
    const bool bForward = (msg.chKey == VK_DOWN) || (msg.chKey == VK_RIGHT) || 
                          (msg.chKey == VK_NEXT) || (msg.chKey == VK_HOME);
    if (nIndexCurSel < nElementCount) {
        //ƥ���ѡ����
        nIndexCurSel = FindSelectableElement(nIndexCurSel, bForward);
    }
    if (nIndexCurSel >= nElementCount) {
        //û����Ч������ѡ����
        return bHandled;
    }

    const int32_t nRows = CalcRows();
    const int32_t nColumns = CalcColumns();

    size_t nIndexEnsureVisible = Box::InvalidIndex; //��Ҫ��֤�ɼ���Ԫ��
    size_t nIndexEnd = Box::InvalidIndex;
    //ʵ��Shift�� + �������ѡ���߼�
    switch (msg.chKey) {
    case VK_UP:
        if (IsHorizontalLayout()) {
            //���򲼾�
            if (nIndexCurSel >= 1) {
                nIndexEnd = nIndexCurSel - 1;
            }
            else {
                nIndexEnsureVisible = 0;
            }
        }
        else {
            //���򲼾�
            if (nIndexCurSel >= nColumns) {
                nIndexEnd = nIndexCurSel - nColumns;
            }
            else {
                nIndexEnsureVisible = 0;
            }
        }
        break;
    case VK_DOWN:
        if (IsHorizontalLayout()) {
            //���򲼾�
            if ((nIndexCurSel + 1) < nElementCount) {
                nIndexEnd = nIndexCurSel + 1;
            }
            else {
                nIndexEnsureVisible = nElementCount - 1;
            }
        }
        else {
            //���򲼾�
            if ((nIndexCurSel + nColumns) < nElementCount) {
                nIndexEnd = nIndexCurSel + nColumns;
            }
            else {
                nIndexEnsureVisible = nElementCount - 1;
            }
        }
        break;
    case VK_LEFT:
        if (IsHorizontalLayout()) {
            //���򲼾�
            if (nIndexCurSel >= nRows) {
                nIndexEnd = nIndexCurSel - nRows;
            }
            else {
                nIndexEnsureVisible = 0;
            }
        }
        else {
            //���򲼾�
            if (nIndexCurSel >= 1) {
                nIndexEnd = nIndexCurSel - 1;
            }
            else {
                nIndexEnsureVisible = 0;
            }
        }
        break;
    case VK_RIGHT:
        if (IsHorizontalLayout()) {
            //���򲼾�
            if ((nIndexCurSel + nRows) < nElementCount) {
                nIndexEnd = nIndexCurSel + nRows;
            }
            else {
                nIndexEnsureVisible = nElementCount - 1;
            }
        }
        else {
            //���򲼾�
            if ((nIndexCurSel + 1) < nElementCount){
                nIndexEnd = nIndexCurSel + 1;
            }
            else {
                nIndexEnsureVisible = nElementCount - 1;
            }
        }
        break;
    case VK_PRIOR:
    {
        size_t nShowColumns = 0;
        size_t nShowRows = 0;
        if (IsHorizontalLayout()) {
            //���򲼾�
            GetDisplayItemCount(true, nShowColumns, nShowRows);
        }
        else {
            //���򲼾�            
            GetDisplayItemCount(false, nShowColumns, nShowRows);
        }
        size_t nScrollCount = nShowColumns * nShowRows;
        if (nIndexCurSel >= nScrollCount) {
            nIndexEnd = nIndexCurSel - nScrollCount;
        }
        else {
            if (IsHorizontalLayout()) {
                for (int32_t nColumn = (int32_t)nShowColumns - 1; nColumn >= 0; --nColumn) {
                    nScrollCount = (size_t)nColumn * nShowRows;
                    if (nIndexCurSel >= nScrollCount) {
                        //��ת����һ�У�ͬ�е�λ��
                        nIndexEnd = nIndexCurSel - nScrollCount;
                        break;
                    }
                }
            }
            else {
                for (int32_t nRow = (int32_t)nShowRows - 1; nRow >= 0; --nRow) {
                    nScrollCount = nShowColumns * (size_t)nRow;
                    if (nIndexCurSel >= nScrollCount) {
                        //��ת����һ�У�ͬ�е�λ��
                        nIndexEnd = nIndexCurSel - nScrollCount;
                        break;
                    }
                }
            }
        }
    }
    break;
    case VK_NEXT:
    {
        size_t nShowColumns = 0;
        size_t nShowRows = 0;
        if (IsHorizontalLayout()) {
            //���򲼾�
            GetDisplayItemCount(true, nShowColumns, nShowRows);            
        }
        else {
            //���򲼾�            
            GetDisplayItemCount(false, nShowColumns, nShowRows);            
        }
        size_t nScrollCount = nShowColumns * nShowRows;
        if ((nIndexCurSel + nScrollCount) < nElementCount) {
            nIndexEnd = nIndexCurSel + nScrollCount;
        }
        else {
            if (IsHorizontalLayout()) {
                for (int32_t nColumn = (int32_t)nShowColumns - 1; nColumn >= 0; --nColumn) {
                    nScrollCount = (size_t)nColumn * nShowRows;
                    if ((nIndexCurSel + nScrollCount) < nElementCount) {
                        //��ת�����һ�У�ͬ�е�λ��
                        nIndexEnd = nIndexCurSel + nScrollCount;
                        nIndexEnsureVisible = nElementCount - 1;
                        break;
                    }
                }
            }
            else {
                for (int32_t nRow = (int32_t)nShowRows - 1; nRow >= 0; --nRow) {
                    nScrollCount = nShowColumns * (size_t)nRow;
                    if ((nIndexCurSel + nScrollCount) < nElementCount) {
                        //��ת�����һ�У�ͬ�е�λ��
                        nIndexEnd = nIndexCurSel + nScrollCount;
                        nIndexEnsureVisible = nElementCount - 1;
                        break;
                    }
                }
            }
        }
    }
    break;
    case VK_HOME:
        nIndexEnd = 0;
        break;
    case VK_END:
        nIndexEnd = nElementCount - 1;
        break;
    default:
        break;
    }

    if (nIndexEnd >= nElementCount) {
        if (nIndexEnsureVisible != Box::InvalidIndex) {
            EnsureVisible(nIndexEnsureVisible, false);
        }
        return bHandled;
    }

    //ƥ���ѡ����
    nIndexEnd = FindSelectableElement(nIndexEnd, bForward);
    if (nIndexEnd >= nElementCount) {
        return bHandled;
    }

    bHandled = true;
    std::vector<size_t> selectedIndexs; //��Ҫѡ����б�
    if (bShiftDown) {
        //��סShift����ѡ��Χ�ڵ���������
        size_t nLastNoShiftIndex = m_nLastNoShiftIndex;//��ʼ��Ԫ��������
        if (nLastNoShiftIndex >= nElementCount) {
            nLastNoShiftIndex = 0;
        }
        size_t nStartElementIndex = std::min(nLastNoShiftIndex, nIndexEnd);
        size_t nEndElementIndex = std::max(nLastNoShiftIndex, nIndexEnd);
        for (size_t i = nStartElementIndex; i <= nEndElementIndex; ++i) {
            if (IsSelectableElement(i)) {
                selectedIndexs.push_back(i);
            }
        }
    }
    else {
        //û�а�סShift����ֻѡ�����һ������
        selectedIndexs.push_back(nIndexEnd);
    }

    //ѡ�������Χ�ڵ�����Ԫ��
    std::vector<size_t> refreshIndexs;
    SetSelectedElements(selectedIndexs, true, refreshIndexs);
    RefreshElements(refreshIndexs);
    if (nIndexEnsureVisible != Box::InvalidIndex) {
        EnsureVisible(nIndexEnsureVisible, false);
    }
    else {
        EnsureVisible(nIndexEnd, false);
    }    
    nCurSel = GetDisplayItemIndex(nIndexEnd);
    ASSERT(nCurSel < GetItemCount());
    bool bTriggerEvent = false;
    if (nCurSel < GetItemCount()) {
        SetCurSel(nCurSel);
        SelectItemSingle(nCurSel, true, false);
        bTriggerEvent = true;
        ASSERT(GetItemAt(nCurSel)->IsFocused());
        ASSERT(IsElementSelected(nIndexEnd));
#ifdef _DEBUG
        std::vector<size_t> selected;
        GetSelectedItems(selected);
        ASSERT(std::find(selected.begin(), selected.end(), nCurSel) != selected.end());
#endif
    }
    if (bTriggerEvent) {
        SendEvent(kEventSelect, nCurSel, Box::InvalidIndex);
    }
    return bHandled;
}

}//namespace ui

