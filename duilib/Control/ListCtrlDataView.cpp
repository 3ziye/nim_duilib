#include "ListCtrlDataView.h"
#include "ListCtrl.h"
#include "duilib/Render/AutoClip.h"

//�����ࣺListCtrlDataView / ListCtrlDataLayout

namespace ui
{
ListCtrlDataView::ListCtrlDataView() :
    VirtualListBox(new ListCtrlDataLayout),
    m_pListCtrl(nullptr),
    m_nTopElementIndex(0)
{
    VirtualLayout* pVirtualLayout = dynamic_cast<VirtualLayout*>(GetLayout());
    SetVirtualLayout(pVirtualLayout);
}

ListCtrlDataView::~ListCtrlDataView() 
{
}

void ListCtrlDataView::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

int32_t ListCtrlDataView::GetListCtrlWidth() const
{
    int32_t nToltalWidth = 0;
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nToltalWidth;
    }
    ListCtrlHeader* pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    if (pHeaderCtrl == nullptr) {
        return nToltalWidth;
    }
    size_t nColumnCount = pHeaderCtrl->GetColumnCount();
    for (size_t index = 0; index < nColumnCount; ++index) {
        ListCtrlHeaderItem* pHeaderItem = pHeaderCtrl->GetColumn(index);
        if ((pHeaderItem != nullptr) && pHeaderItem->IsColumnVisible()) {
            nToltalWidth += pHeaderItem->GetColumnWidth();
        }
    }
    return nToltalWidth;
}

void ListCtrlDataView::SetTopElementIndex(size_t nTopElementIndex)
{
    m_nTopElementIndex = nTopElementIndex;
}

size_t ListCtrlDataView::GetTopElementIndex() const
{
    return m_nTopElementIndex;
}

void ListCtrlDataView::SetDisplayDataItems(const std::vector<size_t>& itemIndexList)
{
    m_diplayItemIndexList = itemIndexList;
}

void ListCtrlDataView::GetDisplayDataItems(std::vector<size_t>& itemIndexList) const
{
    itemIndexList = m_diplayItemIndexList;
}

bool ListCtrlDataView::IsDataItemDisplay(size_t itemIndex) const
{
    auto iter = std::find(m_diplayItemIndexList.begin(), m_diplayItemIndexList.end(), itemIndex);
    return iter != m_diplayItemIndexList.end();
}

bool ListCtrlDataView::EnsureDataItemVisible(size_t itemIndex, bool bToTop)
{
    if (!Box::IsValidItemIndex(itemIndex) || (itemIndex >= GetElementCount())) {
        return false;
    }
    VirtualLayout* pVirtualLayout = dynamic_cast<VirtualLayout*>(GetLayout());
    if (pVirtualLayout != nullptr) {
        pVirtualLayout->EnsureVisible(GetRect(), itemIndex, bToTop);
        return true;
    }
    return false;
}

void ListCtrlDataView::PaintChild(IRender* pRender, const UiRect& rcPaint)
{
    //��дVirtualListBox::PaintChild / ScrollBox::PaintChild������ȷ��Header��������
    ASSERT(pRender != nullptr);
    if (pRender == nullptr) {
        return;
    }
    UiRect rcTemp;
    if (!UiRect::Intersect(rcTemp, rcPaint, GetRect())) {
        return;
    }

    const size_t nItemCount = GetItemCount();
    if (nItemCount <= 1) {
        __super::PaintChild(pRender, rcPaint);
        return;
    }
    ListCtrlHeader* pHeaderCtrl = dynamic_cast<ListCtrlHeader*>(GetItemAt(0));
    if ((pHeaderCtrl == nullptr) || !pHeaderCtrl->IsVisible()) {
        __super::PaintChild(pRender, rcPaint);
        return;
    }

    //��Ҫ�������Ʋ���
    ReArrangeChild(false);
    std::vector<Control*> items = m_items;
    if (!items.empty()) {
        //������Header�ؼ������ⱻ�����ĸ���
        Control* pHeader = items.front();
        items.erase(items.begin());
        items.push_back(pHeader);
    }
    for (Control* pControl : items) {
        if (pControl == nullptr) {
            continue;
        }
        if (!pControl->IsVisible()) {
            continue;
        }

        UiSize scrollPos = GetScrollOffset();
        UiRect rcNewPaint = GetPosWithoutPadding();
        AutoClip alphaClip(pRender, rcNewPaint, IsClip());
        rcNewPaint.Offset(scrollPos.cx, scrollPos.cy);
        rcNewPaint.Offset(GetRenderOffset().x, GetRenderOffset().y);

        UiPoint ptOffset(scrollPos.cx, scrollPos.cy);
        UiPoint ptOldOrg = pRender->OffsetWindowOrg(ptOffset);
        pControl->AlphaPaint(pRender, rcNewPaint);
        pRender->SetWindowOrg(ptOldOrg);
    }
    ScrollBar* pVScrollBar = GetVScrollBar();
    ScrollBar* pHScrollBar = GetHScrollBar();
    if ((pHScrollBar != nullptr) && pHScrollBar->IsVisible()) {
        pHScrollBar->AlphaPaint(pRender, rcPaint);
    }

    if ((pVScrollBar != nullptr) && pVScrollBar->IsVisible()) {
        pVScrollBar->AlphaPaint(pRender, rcPaint);
    }
}

Control* ListCtrlDataView::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos)
{
    //���£�Box::FindControl ��������Header���ȱ����ҵ���ֻ������UIFIND_TOP_FIRST��־�����
    Control* pFoundControl = __super::FindControl(Proc, pData, uFlags, scrollPos);
    if ((uFlags & UIFIND_TOP_FIRST) == 0) {
        return pFoundControl;
    }
    else {
        if (GetItemIndex(pFoundControl) == Box::InvalidIndex) {
            return pFoundControl;
        }
    }
   
    // Check if this guy is valid
    if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) {
        return nullptr;
    }
    if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) {
        return nullptr;
    }
    if ((uFlags & UIFIND_HITTEST) != 0) {
        ASSERT(pData != nullptr);
        UiPoint pt(*(static_cast<UiPoint*>(pData)));
        if ((pData != nullptr) && !GetRect().ContainsPt(pt)) {
            return nullptr;
        }
        if (!IsMouseChildEnabled()) {
            Control* pResult = Control::FindControl(Proc, pData, uFlags);
            return pResult;
        }
    }

    if ((uFlags & UIFIND_ME_FIRST) != 0) {
        Control* pControl = Control::FindControl(Proc, pData, uFlags);
        if (pControl != nullptr) {
            return pControl;
        }
    }
    const std::vector<Control*>& items = m_items;
    UiRect rc = GetRectWithoutPadding();
    if ((uFlags & UIFIND_TOP_FIRST) != 0) {
        std::vector<Control*> newItems = m_items;
        ListCtrlHeader* pHeaderCtrl = dynamic_cast<ListCtrlHeader*>(GetItemAt(0));
        if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
            if (!newItems.empty()) {
                //������Header�ؼ������ⱻ�����ĸ���
                Control* pHeader = newItems.front();
                newItems.erase(newItems.begin());
                newItems.push_back(pHeader);
            }
        }
        for (int it = (int)newItems.size() - 1; it >= 0; --it) {
            if (newItems[it] == nullptr) {
                continue;
            }
            Control* pControl = nullptr;
            if ((uFlags & UIFIND_HITTEST) != 0) {
                ASSERT(pData != nullptr);
                if (pData != nullptr) {
                    UiPoint newPoint(*(static_cast<UiPoint*>(pData)));
                    newPoint.Offset(scrollPos);
                    pControl = newItems[it]->FindControl(Proc, &newPoint, uFlags);
                }
            }
            else {
                pControl = newItems[it]->FindControl(Proc, pData, uFlags);
            }
            if (pControl != nullptr) {
                if ((uFlags & UIFIND_HITTEST) != 0 &&
                    !pControl->IsFloat() &&
                    (pData != nullptr) &&
                    !rc.ContainsPt(*(static_cast<UiPoint*>(pData)))) {
                    continue;
                }
                else {
                    return pControl;
                }
            }
        }
    }
    else {
        for (Control* pItemControl : items) {
            if (pItemControl == nullptr) {
                continue;
            }
            Control* pControl = nullptr;
            if ((uFlags & UIFIND_HITTEST) != 0) {
                ASSERT(pData != nullptr);
                if (pData != nullptr) {
                    UiPoint newPoint(*(static_cast<UiPoint*>(pData)));
                    newPoint.Offset(scrollPos);
                    pControl = pItemControl->FindControl(Proc, &newPoint, uFlags);
                }
            }
            else {
                pControl = pItemControl->FindControl(Proc, pData, uFlags);
            }
            if (pControl != nullptr) {
                if ((uFlags & UIFIND_HITTEST) != 0 &&
                    !pControl->IsFloat() &&
                    (pData != nullptr) &&
                    !rc.ContainsPt(*(static_cast<UiPoint*>(pData)))) {
                    continue;
                }
                else {
                    return pControl;
                }
            }
        }
    }

    Control* pResult = nullptr;
    if ((uFlags & UIFIND_ME_FIRST) == 0) {
        pResult = Control::FindControl(Proc, pData, uFlags);
    }
    return pResult;
}

void ListCtrlDataView::OnRefresh()
{
}

void ListCtrlDataView::OnArrangeChild()
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->UpdateControlCheckStatus(Box::InvalidIndex);
    }
}

UiSize64 ListCtrlDataLayout::ArrangeChild(const std::vector<ui::Control*>& /*items*/, ui::UiRect rc)
{
    ListCtrlDataView* pOwnerListBox = dynamic_cast<ListCtrlDataView*>(GetOwner());
    if ((pOwnerListBox == nullptr) || !pOwnerListBox->HasDataProvider()) {
        ASSERT(FALSE);
        return UiSize64();
    }
    DeflatePadding(rc);
    const int32_t nHeaderHeight = GetHeaderHeight();
    int64_t nTotalHeight = GetElementsHeight(Box::InvalidIndex) + nHeaderHeight;
    UiSize64 sz(rc.Width(), rc.Height());
    sz.cy = std::max(nTotalHeight, sz.cy);
    sz.cx = std::max(GetItemWidth(), rc.Width()); //������ֺ��������
    LazyArrangeChild(rc);

#ifdef _DEBUG
    //TEST ����Ϊ���Դ���
    {
        size_t s0 = pOwnerListBox->GetTopElementIndex();
        size_t s1 = GetTopElementIndex(pOwnerListBox->GetRect());
        ASSERT(s0 == s1);

        std::vector<size_t> itemIndexList;
        pOwnerListBox->GetDisplayDataItems(itemIndexList);

        std::vector<size_t> collection;
        GetDisplayElements(pOwnerListBox->GetRect(), collection);

        std::vector<size_t> displayItemIndexList;
        for (size_t i = 0; i < 200; ++i) {
            if (IsElementDisplay(pOwnerListBox->GetRect(), i)) {
                displayItemIndexList.push_back(i);
            }
        }
        //if (!itemIndexList.empty()) {
        //    ASSERT(itemIndexList == collection);
        //    ASSERT(itemIndexList == displayItemIndexList);
        //}
        //EnsureVisible(pOwnerListBox->GetRect(), 50, false);
    }
    //TEST
#endif

    return sz;
}

UiSize ListCtrlDataLayout::EstimateSizeByChild(const std::vector<Control*>& /*items*/, ui::UiSize szAvailable)
{
    ListCtrlDataView* pList = dynamic_cast<ListCtrlDataView*>(GetOwner());
    if ((pList == nullptr) || !pList->HasDataProvider()) {
        ASSERT(FALSE);
        return UiSize();
    }
    szAvailable.Validate();
    UiEstSize estSize;
    if (GetOwner() != nullptr) {
        estSize = GetOwner()->Control::EstimateSize(szAvailable);
    }
    UiSize size(estSize.cx.GetInt32(), estSize.cy.GetInt32());
    if (estSize.cx.IsStretch()) {
        size.cx = CalcStretchValue(estSize.cx, szAvailable.cx);
    }
    if (estSize.cy.IsStretch()) {
        size.cy = CalcStretchValue(estSize.cy, szAvailable.cy);
    }
    if (size.cx == 0) {
        size.cx = GetItemWidth();
    }
    if (size.cy == 0) {
        size.cy = szAvailable.cy;
    }
    size.Validate();
    return size;
}

void ListCtrlDataLayout::LazyArrangeChild(UiRect rc) const
{
    UiRect orgRect = rc;
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return;
    }
    if (!pOwnerBox->HasDataProvider()) {
        return;
    }

    const size_t nItemCount = pOwnerBox->GetItemCount();
    if (nItemCount > 0) {
        //��һ��Ԫ���Ǳ�ͷ�ؼ���������λ�ô�С
        Control* pHeaderCtrl = pOwnerBox->GetItemAt(0);
        if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
            int32_t nHeaderHeight = pHeaderCtrl->GetFixedHeight().GetInt32();
            if (nHeaderHeight > 0) {
                int32_t nHeaderWidth = GetElementSize(rc.Width(), 0).cx;
                if (nHeaderWidth <= 0) {
                    nHeaderWidth = rc.Width();
                }
                ui::UiRect rcTile(rc.left, rc.top, rc.left + nHeaderWidth, rc.top + nHeaderHeight);
                pHeaderCtrl->SetPos(rcTile);
                rc.top += nHeaderHeight;
            }
        }
    }
    //��¼�ɼ���Ԫ���������б�
    std::vector<size_t> diplayItemIndexList;

    // ����Ԫ�ص�������
    const size_t nTopElementIndex = GetTopElementIndex(orgRect);
    pOwnerBox->SetTopElementIndex(nTopElementIndex);

    //����
    int32_t nColumns = CalcTileColumns();

    //��������Y����λ��
    int64_t nScrollPosY = pOwnerBox->GetScrollPos().cy;

    //����������ʼλ�� 
    int32_t iPosLeft = rc.left;

    //Y�������ƫ�ƣ���Ҫ���֣��������λ�ñ䶯������ˢ�½������ƫ��
    int32_t yOffset = 0;
    UiSize szTopItem = GetElementSize(rc.Width(), nTopElementIndex);
    if (szTopItem.cy > 0) {
        yOffset = TruncateToInt32(nScrollPosY % szTopItem.cy);
    }

    //����Ķ�����ʼλ��
    int32_t iPosTop = rc.top - yOffset;

    //��������ƫ�ƣ������������ϴ�ʱ��rc���32λ�ľ��εĸ߶Ȼ�Խ�磬��Ҫ64λ���Ͳ�������
    pOwnerBox->SetScrollVirtualOffsetY(nScrollPosY);

    //�ؼ������Ͻ�����ֵ
    ui::UiPoint ptTile(iPosLeft, iPosTop);

    UiSize szItem;
    size_t iCount = 0;
    //��һ��Ԫ���Ǳ�ͷ�ؼ�������������ݣ����Դ�1��ʼ
    for (size_t index = 1; index < nItemCount; ++index) {        
        Control* pControl = pOwnerBox->GetItemAt(index);
        if (pControl == nullptr) {
            continue;
        }
        //��ǰ����Ԫ�ص�������
        const size_t nElementIndex = nTopElementIndex + iCount;
        szItem = GetElementSize(rc.Width(), nElementIndex);

        //���õ�ǰ�ؼ��Ĵ�С��λ��
        ui::UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + szItem.cx, ptTile.y + szItem.cy);
        pControl->SetPos(rcTile);

        // �������        
        if (nElementIndex < pOwnerBox->GetElementCount()) {
            if (!pControl->IsVisible()) {
                pControl->SetVisible(true);
            }
            pOwnerBox->FillElement(pControl, nElementIndex);
            diplayItemIndexList.push_back(nElementIndex);
        }
        else {
            if (pControl->IsVisible()) {
                pControl->SetVisible(false);
            }
        }

        if ((++iCount % nColumns) == 0) {
            ptTile.x = iPosLeft;
            ptTile.y += szItem.cy + GetChildMarginY();
        }
        else {
            ptTile.x += rcTile.Width() + GetChildMarginX();
        }
    }
    pOwnerBox->SetDisplayDataItems(diplayItemIndexList);
    pOwnerBox->OnArrangeChild();
}

size_t ListCtrlDataLayout::AjustMaxItem(UiRect rc) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return 0;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 0;
    }
    if (rc.IsEmpty()) {
        return 0;
    }
    int32_t nRows = 0;
    int32_t nHeaderHeight = GetHeaderHeight();
    if (nHeaderHeight > 0) {
        nRows += 1;
        rc.top += nHeaderHeight;
        rc.Validate();
    }

    int32_t nColumns = CalcTileColumns();
    nRows += rc.Height() / (nItemHeight + GetChildMarginY() / 2);
    //��֤������
    if (nRows > 1) {
        int32_t calcHeight = nRows * nItemHeight + (nRows - 1) * GetChildMarginY();
        if (calcHeight < rc.Height()) {
            nRows += 1;
        }
    }
    //��������1�У�ȷ����ʵ�ؼ��������������ʾ����
    nRows += 1;
    return nRows * nColumns;
}

size_t ListCtrlDataLayout::GetTopElementIndex(UiRect /*rc*/) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return 0;
    }
    int64_t nScrollPosY = pOwnerBox->GetScrollPos().cy;
    
    int64_t nColumns = (int64_t)CalcTileColumns();
    if (nColumns < 0) {
        nColumns = 0;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 0;
    }
    int64_t iIndex = (nScrollPosY / nItemHeight) * nColumns;
    return static_cast<size_t>(iIndex);
}

bool ListCtrlDataLayout::IsElementDisplay(UiRect rc, size_t iIndex) const
{
    if (!Box::IsValidItemIndex(iIndex)) {
        return false;
    }
    std::vector<size_t> itemIndexList;
    GetDisplayElements(rc, itemIndexList);
    return std::find(itemIndexList.begin(), itemIndexList.end(), iIndex) != itemIndexList.end();
}

bool ListCtrlDataLayout::NeedReArrange() const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return false;
    }
    if (!pOwnerBox->HasDataProvider()) {
        return false;
    }
    size_t nCount = pOwnerBox->GetItemCount();
    if (nCount == 0) {
        return false;
    }

    if (pOwnerBox->GetElementCount() <= nCount) {
        return false;
    }

    UiRect rcThis = pOwnerBox->GetPos();
    if (rcThis.IsEmpty()) {
        return false;
    }

    int64_t nScrollPosY = pOwnerBox->GetScrollPos().cy;
    int64_t nVirtualOffsetY = pOwnerBox->GetScrollVirtualOffset().cy;
    //ֻҪ����λ�÷����仯������Ҫ���²���
    return (nScrollPosY != nVirtualOffsetY);
}

void ListCtrlDataLayout::GetDisplayElements(UiRect rc, std::vector<size_t>& collection) const
{
    collection.clear();
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return;
    }

    if (pOwnerBox->GetItemCount() == 0) {
        return;
    }

    size_t nEleHeight = GetElementsHeight(1);
    if (nEleHeight == 0) {
        return;
    }

    int32_t nHeaderHeight = GetHeaderHeight();
    int64_t nScrollPosY = pOwnerBox->GetScrollPos().cy;
    rc.top += nHeaderHeight;
    rc.Validate();

    int32_t nColumns = CalcTileColumns();
    size_t min = ((size_t)nScrollPosY / nEleHeight) * nColumns;
    size_t max = min + ((size_t)rc.Height() / nEleHeight) * nColumns;

    size_t nCount = pOwnerBox->GetElementCount();
    if (nCount > 0) {
        if (max >= nCount) {
            max = nCount - 1;
        }
    }
    else {
        return;
    }

    for (size_t i = min; i <= max; ++i) {
        collection.push_back(i);
    }
}

void ListCtrlDataLayout::EnsureVisible(UiRect rc, size_t iIndex, bool bToTop) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return;
    }
    if (!Box::IsValidItemIndex(iIndex) || iIndex >= pOwnerBox->GetElementCount()) {
        return;
    }
    ScrollBar* pVScrollBar = pOwnerBox->GetVScrollBar();
    if (pVScrollBar == nullptr) {
        return;
    }
    const int64_t nElementHeight = GetElementsHeight(1);
    if (nElementHeight == 0) {
        return;
    }
    int64_t nNewPos = 0;
    if (bToTop) {
        if (iIndex > 0) {
            nNewPos = GetElementsHeight(iIndex);
        }
    }
    else {
        std::vector<size_t> itemIndexList;
        GetDisplayElements(rc, itemIndexList);
        bool bDisplay = std::find(itemIndexList.begin(), itemIndexList.end(), iIndex) != itemIndexList.end();
        if (bDisplay) {
            return;
        }
        if (iIndex > 0) {
            nNewPos = GetElementsHeight(iIndex);
        }
        if (itemIndexList.size() >= 2) {
            nNewPos -= (itemIndexList.size() - 2) * nElementHeight;
        }
    }
    if (nNewPos < 0) {
        nNewPos = 0;
    }    
    if (nNewPos > pVScrollBar->GetScrollRange()) {
        nNewPos = pVScrollBar->GetScrollRange();
    }
    ui::UiSize64 sz = pOwnerBox->GetScrollPos();
    sz.cy = nNewPos;
    pOwnerBox->SetScrollPos(sz);
}

ListCtrlDataView* ListCtrlDataLayout::GetOwnerBox() const
{
    ListCtrlDataView* pList = dynamic_cast<ListCtrlDataView*>(GetOwner());
    ASSERT(pList != nullptr);
    return pList;
}

int64_t ListCtrlDataLayout::GetElementsHeight(size_t nCount) const
{
    if (nCount == 0) {
        return 0;
    }
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return 0;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 0;
    }
    int32_t nColumns = CalcTileColumns();
    if (nCount <= nColumns && nCount != Box::InvalidIndex) {
        //����1�У����߸պ�1��
        return nItemHeight + GetChildMarginY();
    }
    if (!Box::IsValidItemIndex(nCount)) {
        nCount = pOwnerBox->GetElementCount();
    }
    if (!Box::IsValidItemIndex(nCount)) {
        ASSERT(FALSE);
        return nItemHeight + GetChildMarginY();
    }

    int64_t rows = nCount / nColumns;
    if (nCount % nColumns != 0) {
        rows += 1;
    }
    int64_t iChildMargin = 0;
    if (GetChildMarginY() > 0) {
        iChildMargin = GetChildMarginY();
    }
    if (nCount > 0) {
        int64_t childMarginTotal = 0;
        if (nCount % nColumns == 0) {
            childMarginTotal = ((int64_t)nCount / nColumns - 1) * iChildMargin;
        }
        else {
            childMarginTotal = ((int64_t)nCount / nColumns) * iChildMargin;
        }
        return nItemHeight * rows + childMarginTotal;
    }
    return 0;
}

UiSize ListCtrlDataLayout::GetElementSize(int32_t rcWidth, size_t /*nElementIndex*/) const
{
    UiSize szElementSize;
    szElementSize.cx = std::max(GetItemWidth(), rcWidth);
    szElementSize.cy = GetItemHeight();
    return szElementSize;
}

int32_t ListCtrlDataLayout::CalcTileColumns() const
{
    //�̶���ֻ��һ��
    return 1;
}

int32_t ListCtrlDataLayout::GetItemWidth() const
{
    //������ͷ�Ŀ����ͬ
    int32_t nItemWidth = 0;
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox != nullptr) {
        nItemWidth = pOwnerBox->GetListCtrlWidth();
    }
    return nItemWidth;
}

int32_t ListCtrlDataLayout::GetItemHeight() const
{
    //�����еĸ߶���ͬ�����Ҵ����ö�ȡ
    int32_t nItemHeight = 0;
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if ((pOwnerBox != nullptr) && (pOwnerBox->m_pListCtrl != nullptr)) {
        nItemHeight = pOwnerBox->m_pListCtrl->GetDataItemHeight();
    }
    return nItemHeight;
}

int32_t ListCtrlDataLayout::GetHeaderHeight() const
{
    int32_t nHeaderHeight = 0;
    size_t nItemCount = 0;
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox != nullptr) {
        nItemCount = pOwnerBox->GetItemCount();
    }
    if (nItemCount > 0) {
        //��һ��Ԫ���Ǳ�ͷ�ؼ���������λ�ô�С
        Control* pHeaderCtrl = pOwnerBox->GetItemAt(0);
        if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
            nHeaderHeight = pHeaderCtrl->GetFixedHeight().GetInt32();
        }
    }
    return nHeaderHeight;
}

}//namespace ui

