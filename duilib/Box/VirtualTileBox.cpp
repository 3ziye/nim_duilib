#include "VirtualTileBox.h"
#include "duilib/Core/ScrollBar.h"
#include <algorithm>

namespace ui {

VirtualTileBoxElement::VirtualTileBoxElement():
    m_pfnCountChangedNotify(),
    m_pfnDataChangedNotify()
{
}

void VirtualTileBoxElement::RegNotifys(const DataChangedNotify& dcNotify, const CountChangedNotify& ccNotify)
{
    m_pfnDataChangedNotify = dcNotify;
    m_pfnCountChangedNotify = ccNotify;
}

void VirtualTileBoxElement::EmitDataChanged(size_t nStartIndex, size_t nEndIndex)
{
    if (m_pfnDataChangedNotify) {
        m_pfnDataChangedNotify(nStartIndex, nEndIndex);
    }
}

void VirtualTileBoxElement::EmitCountChanged()
{
    if (m_pfnCountChangedNotify) {
        m_pfnCountChangedNotify();
    }
}

VirtualTileLayout::VirtualTileLayout()
{
    SetColumns(0);
    SetAutoCalcColumns(true);
}

UiSize64 VirtualTileLayout::ArrangeChild(const std::vector<ui::Control*>& /*items*/, ui::UiRect rc)
{
    DeflatePadding(rc);
    UiSize64 sz(rc.Width(), rc.Height());
    int64_t nTotalHeight = GetElementsHeight(Box::InvalidIndex);
    sz.cy = std::max(nTotalHeight, sz.cy);
    LazyArrangeChild();
    return sz;
}

UiSize VirtualTileLayout::EstimateSizeByChild(const std::vector<Control*>& /*items*/, ui::UiSize szAvailable)
{
    int32_t nColumns = GetColumns();
    UiSize szItem = GetItemSize();
    ASSERT(nColumns > 0);
    UiEstSize estSize = m_pOwner->Control::EstimateSize(szAvailable);
    UiSize size(estSize.cx.GetInt32(), estSize.cy.GetInt32());
    if (estSize.cx.IsStretch()) {
        size.cx = 0;
    }
    if (estSize.cy.IsStretch()) {
        size.cy = 0;
    }    
    if (estSize.cx.GetInt32() == 0) {
        size.cx = szItem.cx * nColumns + GetChildMarginX() * (nColumns - 1);
    }
    return size;
}

int64_t VirtualTileLayout::GetElementsHeight(size_t nCount)
{
    int32_t nColumns = GetColumns();
    UiSize szItem = GetItemSize();
    ASSERT(nColumns > 0);
    if (nColumns < 1) {
        return szItem.cy + GetChildMarginY();
    }
    if (nCount <= nColumns && nCount != Box::InvalidIndex) {
        //����1�У����߸պ�1��
        return szItem.cy + GetChildMarginY();
    }
    if (!Box::IsValidItemIndex(nCount)) {
        VirtualTileBox* pList = dynamic_cast<VirtualTileBox*>(m_pOwner);
        ASSERT(pList != nullptr);
        if (pList != nullptr) {
            nCount = pList->GetElementCount();
        }        
    }
    if (!Box::IsValidItemIndex(nCount)) {
        ASSERT(FALSE);
        return szItem.cy + GetChildMarginY();
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
        return szItem.cy * (rows + 1) + childMarginTotal;
    }
    return 0;
}

void VirtualTileLayout::LazyArrangeChild()
{
    VirtualTileBox* pList = dynamic_cast<VirtualTileBox*>(m_pOwner);
    ASSERT(pList != nullptr);


    // ����SetPosʱ�Ѿ����ú�
    int32_t nColumns = GetColumns();
    UiSize szItem = GetItemSize();
    ASSERT(nColumns > 0);
    if (nColumns < 1) {
        return;
    }

    // ��ȡVirtualTileBox��Rect
    ui::UiRect rc = pList->GetPaddingPos();

    // ����������ʼλ�� 
    int iPosLeft = rc.left;

    // ����Ķ�����ʼλ��
    int32_t iPosTop = rc.top;
    pList->SetScrollVirtualOffsetY(pList->GetScrollPos().cy);

    ui::UiPoint ptTile(iPosLeft, iPosTop);

    // ����index
    size_t nTopIndex = pList->GetTopElementIndex(nullptr);
    size_t iCount = 0;
    for (auto pControl : pList->m_items) {
        if (pControl == nullptr) {
            continue;
        }
        // Determine size
        ui::UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + szItem.cx, ptTile.y + szItem.cy);
        pControl->SetPos(rcTile);

        // �������
        size_t nElementIndex = nTopIndex + iCount;
        if (nElementIndex < pList->GetElementCount()) {
            if (!pControl->IsVisible()) {
                pControl->SetVisible(true);
            }
            pList->FillElement(pControl, nElementIndex);
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
}

size_t VirtualTileLayout::AjustMaxItem()
{
    ASSERT(m_pOwner != nullptr);
    if (m_pOwner == nullptr) {
        return 0;
    }
    ui::UiRect rc = m_pOwner->GetPaddingPos();
    if (rc.IsEmpty()) {
        return 0;
    }
    int32_t nColumns = GetColumns();
    UiSize szItem = GetItemSize();
    if (IsAutoCalcColumns() || (nColumns <= 0)) {
        //������Ҫ����
        if (szItem.cx > 0) {
            nColumns = rc.Width() / (szItem.cx + GetChildMarginX() / 2);
            //��֤������
            if (nColumns > 1) {
                int32_t calcWidth = nColumns * szItem.cx + (nColumns - 1) * GetChildMarginX();
                if (calcWidth > rc.Width()) {
                    nColumns -= 1;
                }
            }
        }        
    }
    if (nColumns <= 0) {
        nColumns = 1;
    }
    int32_t nRows = rc.Height() / (szItem.cy + GetChildMarginY() / 2);
    //��֤������
    if (nRows > 1) {
        int32_t calcHeight = nRows * szItem.cy + (nRows - 1) * GetChildMarginY();
        if (calcHeight < rc.Height()) {
            nRows += 1;
        }
    }
    //��������1�У�ȷ����ʵ�ؼ��������������ʾ����
    nRows += 1;
    return nRows * nColumns;
}

VirtualTileBox::VirtualTileBox(Layout* pLayout /*= new VirtualTileLayout*/)
    : ListBox(pLayout)
    , m_pDataProvider(nullptr)
    , m_nMaxItemCount(0)
    , m_nOldYScrollPos(0)
    , m_bArrangedOnce(false)
    , m_bForceArrange(false)
{
}

void VirtualTileBox::SetDataProvider(VirtualTileBoxElement* pProvider)
{
    m_pDataProvider = pProvider;
    if (pProvider != nullptr) {
        // ע��ģ�����ݱ䶯֪ͨ�ص�
        pProvider->RegNotifys(
            nbase::Bind(&VirtualTileBox::OnModelDataChanged, this, std::placeholders::_1, std::placeholders::_2),
            nbase::Bind(&VirtualTileBox::OnModelCountChanged, this));
    }
}

VirtualTileBoxElement* VirtualTileBox::GetDataProvider()
{
    return m_pDataProvider;
}

void VirtualTileBox::Refresh()
{
    //���������
    size_t nMaxItemCount = GetTileLayout()->AjustMaxItem();
    if (nMaxItemCount == 0) {
        return;
    }
    m_nMaxItemCount = nMaxItemCount;

    //��ǰ��������
    size_t nElementCount = GetElementCount();

    //��ǰ������
    size_t nItemCount = GetItemCount();

    //ˢ�º��������
    size_t nNewItemCount = nElementCount;
    if (nNewItemCount > nMaxItemCount) {
        nNewItemCount = nMaxItemCount;
    }
    
    if (nItemCount > nNewItemCount) {
        //��������������������¼�������������Ƴ��������������������
        size_t n = nItemCount - nNewItemCount;
        for (size_t i = 0; i < n; ++i) {
            this->RemoveItemAt(0);
        }
    }
    else if (nItemCount < nNewItemCount) {
        //���������������С���¼���������������������������ٵ�����
        size_t n = nNewItemCount - nItemCount;
        for (size_t i = 0; i < n; ++i) {
            Control* pControl = CreateElement();
            this->AddItem(pControl);
        }
    }
    if (nElementCount > 0) {
        ReArrangeChild(true);
        Arrange();
    }    
}

void VirtualTileBox::RemoveAllItems()
{
    __super::RemoveAllItems();

    if (m_pVScrollBar) {
        m_pVScrollBar->SetScrollPos(0);
    }
    SetScrollVirtualOffset({ 0, 0 });
    m_nOldYScrollPos = 0;
    m_bArrangedOnce = false;
    m_bForceArrange = false;
}

void VirtualTileBox::SetForceArrange(bool bForce)
{
    m_bForceArrange = bForce;
}

void VirtualTileBox::GetDisplayCollection(std::vector<size_t>& collection)
{
    collection.clear();
    if (GetItemCount() == 0) {
        return;
    }

    // ��ȡBox��Rect
    ui::UiRect rcThis = this->GetPaddingPos();

    size_t nEleHeight = GetRealElementHeight();

    size_t min = ((size_t)GetScrollPos().cy / nEleHeight) * GetColumns();
    size_t max = min + ((size_t)rcThis.Height() / nEleHeight) * GetColumns();

    size_t nCount = GetElementCount();
    if (nCount > 0) {
        if (max >= nCount) {
            max = nCount - 1;
        }
    }
    else {
        ASSERT(FALSE);
        max = 0;
    }

    for (size_t i = min; i <= max; ++i) {
        collection.push_back(i);
    }
}

void VirtualTileBox::EnsureVisible(size_t iIndex, bool bToTop /*= false*/)
{
    if (!Box::IsValidItemIndex(iIndex) || iIndex >= GetElementCount()) {
        return;
    }
    if (m_pVScrollBar == nullptr) {
        return;
    }
    int64_t nPos = GetScrollPos().cy;
    int64_t elementHeight = GetRealElementHeight();
    int64_t nTopIndex = 0;
    if (elementHeight > 0) {
        nTopIndex = (nPos / elementHeight) * GetColumns();
    }
    int64_t nNewPos = 0;

    if (bToTop)
    {
        nNewPos = CalcElementsHeight(iIndex);
        if (nNewPos > elementHeight) {
            nNewPos -= elementHeight;
        }
    }
    else {
        if (IsElementDisplay(iIndex)) {
            return;
        }

        if ((int64_t)iIndex > nTopIndex) {
            // ����
            int64_t height = CalcElementsHeight(iIndex + 1);
            nNewPos = height - GetRect().Height();
        }
        else {
            // ����
            nNewPos = CalcElementsHeight(iIndex);
            if (nNewPos > elementHeight) {
                nNewPos -= elementHeight;
            }            
        }
    }
    if (nNewPos < 0) {
        nNewPos = 0;
    }
    if (nNewPos > m_pVScrollBar->GetScrollRange()) {
        nNewPos = m_pVScrollBar->GetScrollRange();
    }
    ui::UiSize64 sz(0, nNewPos);
    SetScrollPos(sz);
}

void VirtualTileBox::SetScrollPos(UiSize64 szPos)
{
    ASSERT(GetScrollPos().cy >= 0);
    bool isChanged = m_nOldYScrollPos != GetScrollPos().cy;
    m_nOldYScrollPos = GetScrollPos().cy;
    ListBox::SetScrollPos(szPos);
    if (isChanged) {
        ReArrangeChild(false);
    }
}

void VirtualTileBox::HandleEvent(const EventArgs& event)
{
    if (!IsMouseEnabled() && (event.Type > ui::kEventMouseBegin) && (event.Type < ui::kEventMouseEnd)) {
        if (GetParent() != nullptr) {
            GetParent()->SendEvent(event);
        }
        else {
            ui::ScrollBox::HandleEvent(event);
        }
        return;
    }

    switch (event.Type) {
    case ui::kEventKeyDown: {
        switch (event.chKey) {
        case VK_UP: {
            OnKeyDown(VK_UP);
            return;
        }
        case VK_DOWN: {
            OnKeyDown(VK_DOWN);
            return;
        }
        case VK_HOME:
            SetScrollPosY(0);
            return;
        case VK_END: {
            int64_t range = GetScrollRange().cy;
            SetScrollPosY(range);
            return;
        }
        default:
            break;
        }
    }
    case ui::kEventKeyUp: {
        switch (event.chKey) {
        case VK_UP: {
            OnKeyUp(VK_UP);
            return;
        }
        case VK_DOWN: {
            OnKeyUp(VK_DOWN);
            return;
        }
        default:
            break;
        }
    default:
        break;
    }
    }

    __super::HandleEvent(event);
}

void VirtualTileBox::OnKeyDown(TCHAR ch)
{ 
    if (ch == VK_UP) {
        LineUp(-1, false);
    }
    else if (ch == VK_DOWN) {
        LineDown(-1, false);
    }
}

void VirtualTileBox::OnKeyUp(TCHAR /*ch*/)
{ 
}

void VirtualTileBox::SetPos(ui::UiRect rc)
{
    bool bChange = false;
    if (!GetRect().Equals(rc)) {
        bChange = true;
    }
    ListBox::SetPos(rc);
    if (bChange) {
        Refresh();
    }
}

void VirtualTileBox::PaintChild(IRender* pRender, const UiRect& rcPaint)
{
    ReArrangeChild(false);
    __super::PaintChild(pRender, rcPaint);
}

void VirtualTileBox::ReArrangeChild(bool bForce)
{
    ScrollDirection direction = ScrollDirection::kScrollUp;
    if (!bForce && !m_bForceArrange) {
        if (!NeedReArrange(direction)) {
            return;
        }
    }
    LazyArrangeChild();
}

ui::Control* VirtualTileBox::CreateElement()
{
    if (m_pDataProvider != nullptr) {
        return m_pDataProvider->CreateElement();
    }
    return nullptr;
}

void VirtualTileBox::FillElement(Control* pControl, size_t iIndex)
{
    if (m_pDataProvider != nullptr) {
        m_pDataProvider->FillElement(pControl, iIndex);
    }
}

size_t VirtualTileBox::GetElementCount()
{
    size_t elementCount = 0;
    if (m_pDataProvider != nullptr) {
        elementCount = m_pDataProvider->GetElementCount();
    }
    return elementCount;
}

int64_t VirtualTileBox::CalcElementsHeight(size_t nCount)
{
    int64_t height = GetTileLayout()->GetElementsHeight(nCount);
    ASSERT(height >= 0);
    if (height < 0) {
        height = 0;
    }
    return height;
}

size_t VirtualTileBox::GetTopElementIndex(int64_t* bottom)
{
    int64_t nPos = GetScrollPos().cy;
    if (nPos < 0) {
        nPos = 0;
    }
    int64_t nColumns = (int64_t)GetColumns();
    if (nColumns < 0) {
        nColumns = 0;
    }
    int64_t nHeight = GetRealElementHeight();
    ASSERT(nHeight >= 0);
    if (nHeight <= 0) {
        if (bottom != nullptr) {
            *bottom = 0;
        }
        return 0;
    }
    int64_t iIndex = (nPos / nHeight) * nColumns;
    if (bottom != nullptr) {
        *bottom = iIndex * nHeight;
    }
    return static_cast<size_t>(iIndex);
}

bool VirtualTileBox::IsElementDisplay(size_t iIndex)
{
    if (!Box::IsValidItemIndex(iIndex)) {
        return false;
    }

    int64_t nPos = GetScrollPos().cy;
    int64_t nElementPos = CalcElementsHeight(iIndex);
    if (nElementPos >= nPos) {
        int64_t nHeight = this->GetHeight();
        if (nElementPos <= nPos + nHeight) {
            return true;
        }
    }
    return false;
}

bool VirtualTileBox::NeedReArrange(ScrollDirection& direction)
{
    direction = ScrollDirection::kScrollUp;
    if (!m_bArrangedOnce) {
        //���ݱ仯�������û�����һ�β��֣���ô��ǿ����һ��Arrange
        m_bArrangedOnce = true;
        return true;
    }

    size_t nCount = GetItemCount();
    if (nCount == 0) {
        return false;
    }

    if (GetElementCount() <= nCount) {
        return false;
    }

    ui::UiRect rcThis = this->GetPos();
    if (rcThis.Width() <= 0) {
        return false;
    }

    int64_t nPos = GetScrollPos().cy;
    int64_t nVirtualOffsetY = GetScrollVirtualOffset().cy;
    ui::UiRect rcItem;

    rcItem = m_items[0]->GetPos();

    if (nPos >= m_nOldYScrollPos) {
        // ��
        rcItem = m_items[nCount - 1]->GetPos();
        int64_t nSub = (rcItem.bottom + nVirtualOffsetY - rcThis.top) - (nPos + rcThis.Height());
        if (nSub < 0) {
            direction = ScrollDirection::kScrollDown;
            return true;
        }
    }
    else {
        // ��
        rcItem = m_items[0]->GetPos();
        if (nPos < (rcItem.top + nVirtualOffsetY - rcThis.top)) {
            direction = ScrollDirection::kScrollUp;
            return true;
        }
    }
    return false;
}

VirtualTileLayout* VirtualTileBox::GetTileLayout()
{
    auto* pLayout = dynamic_cast<VirtualTileLayout*>(GetLayout());
    ASSERT(pLayout != nullptr);
    return pLayout;
}

int64_t VirtualTileBox::GetRealElementHeight()
{
    int64_t height = GetTileLayout()->GetElementsHeight(1);
    ASSERT(height >= 0);
    if (height < 0) {
        height = 0;
    }
    return height;
}

size_t VirtualTileBox::GetColumns()
{
    int columns = GetTileLayout()->GetColumns();
    ASSERT(columns >= 0);
    if (columns < 0) {
        columns = 0;
    }
    return static_cast<size_t>(columns);
}

void VirtualTileBox::LazyArrangeChild()
{
    GetTileLayout()->LazyArrangeChild();
}

void VirtualTileBox::OnModelDataChanged(size_t nStartIndex, size_t nEndIndex)
{
    for (size_t i = nStartIndex; i <= nEndIndex; ++i) {
        size_t nItemIndex = ElementIndexToItemIndex(nStartIndex);
        if (Box::IsValidItemIndex(nItemIndex) && nItemIndex < m_items.size()) {
            FillElement(m_items[nItemIndex], i);
        }
    }
}

void VirtualTileBox::OnModelCountChanged()
{
    Refresh();
}

size_t VirtualTileBox::ElementIndexToItemIndex(size_t nElementIndex)
{
    if (IsElementDisplay(nElementIndex)) {
        size_t nTopItemIndex = GetTopElementIndex(nullptr);
        ASSERT(nElementIndex >= nTopItemIndex);
        if (nElementIndex >= nTopItemIndex)
        {
            return nElementIndex - nTopItemIndex;
        }
    }
    return Box::InvalidIndex;
}

size_t VirtualTileBox::ItemIndexToElementIndex(size_t nItemIndex)
{
    return GetTopElementIndex(nullptr) + nItemIndex;
}

}
