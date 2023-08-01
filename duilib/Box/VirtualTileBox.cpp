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

int32_t VirtualTileLayout::CalcTileColumns(int32_t rcWidth) const
{
    int32_t nColumns = GetColumns();
    if (nColumns < 0) {
        nColumns = 0;
    }
    if (IsAutoCalcColumns()) {
        nColumns = 0;
    }
    if (nColumns > 0) {
        return nColumns;
    }
    int32_t totalWidth = rcWidth;
    int32_t tileWidth = GetItemSize().cx;
    int32_t childMarginX = GetChildMarginX();
    if (childMarginX < 0) {
        childMarginX = 0;
    }
    if (tileWidth > 0) {
        while (totalWidth > 0) {
            totalWidth -= tileWidth;
            if (nColumns != 0) {
                totalWidth -= childMarginX;
            }
            if (totalWidth >= 0) {
                ++nColumns;
            }
        }
    }
    if (nColumns < 1) {
        nColumns = 1;
    }
    return nColumns;
}

UiSize64 VirtualTileLayout::ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc)
{
    VirtualTileBox* pList = dynamic_cast<VirtualTileBox*>(m_pOwner);
    if ((pList != nullptr) && (!pList->HasDataProvider())) {
        //���δ�������ݽӿڣ�����ݻ���Ĺ���
        return __super::ArrangeChild(items, rc);
    }
    DeflatePadding(rc);
    int64_t nTotalHeight = GetElementsHeight(rc, Box::InvalidIndex);
    UiSize64 sz(rc.Width(), rc.Height());
    sz.cy = std::max(nTotalHeight, sz.cy);
    LazyArrangeChild(rc);
    return sz;
}

UiSize VirtualTileLayout::EstimateSizeByChild(const std::vector<Control*>& items, ui::UiSize szAvailable)
{
    VirtualTileBox* pList = dynamic_cast<VirtualTileBox*>(m_pOwner);
    if ((pList != nullptr) && (!pList->HasDataProvider())) {
        //���δ�������ݽӿڣ�����ݻ���Ĺ���
        return __super::EstimateSizeByChild(items, szAvailable);
    }
    UiSize szItem = GetItemSize();
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return UiSize();
    }
    szAvailable.Validate();
    int32_t nColumns = CalcTileColumns(szAvailable.cx);
    UiEstSize estSize;
    if (m_pOwner != nullptr) {
        estSize = m_pOwner->Control::EstimateSize(szAvailable);
    }
    UiSize size(estSize.cx.GetInt32(), estSize.cy.GetInt32());
    if (estSize.cx.IsStretch()) {
        size.cx = szAvailable.cx;
    }
    if (estSize.cy.IsStretch()) {
        size.cy = szAvailable.cy;
    }    
    if (size.cx == 0) {
        size.cx = szItem.cx * nColumns + GetChildMarginX() * (nColumns - 1);
    }
    size.Validate();
    return size;
}

int64_t VirtualTileLayout::GetElementsHeight(UiRect rc, size_t nCount)
{
    UiSize szItem = GetItemSize();
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return 0;
    }
    int32_t nColumns = CalcTileColumns(rc.Width());
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

void VirtualTileLayout::LazyArrangeChild(UiRect rc)
{
    UiSize szItem = GetItemSize();
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return;
    }
    VirtualTileBox* pList = dynamic_cast<VirtualTileBox*>(m_pOwner);
    ASSERT(pList != nullptr);
    if (pList == nullptr) {
        return;
    }

    //����
    int32_t nColumns = CalcTileColumns(rc.Width());

    //����������ʼλ�� 
    int32_t iPosLeft = rc.left;

    //����Ķ�����ʼλ��
    int32_t iPosTop = rc.top;

    //��������ƫ�ƣ������������ϴ�ʱ��rc���32λ�ľ��εĸ߶Ȼ�Խ�磬��Ҫ64λ���Ͳ�������
    pList->SetScrollVirtualOffsetY(pList->GetScrollPos().cy);

    //�ؼ������Ͻ�����ֵ
    ui::UiPoint ptTile(iPosLeft, iPosTop);

    // ����index
    size_t nTopIndex = pList->GetTopElementIndex();
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

size_t VirtualTileLayout::AjustMaxItem(UiRect rc)
{
    UiSize szItem = GetItemSize();
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return 0;
    }
    if (rc.IsEmpty()) {
        return 0;
    }
    int32_t nColumns = CalcTileColumns(rc.Width());
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

/////////////////////////////////////////////////////////////////////////////
//
VirtualTileBox::VirtualTileBox(Layout* pLayout /*= new VirtualTileLayout*/)
    : ListBox(pLayout)
    , m_pDataProvider(nullptr)
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

bool VirtualTileBox::HasDataProvider() const
{
    return m_pDataProvider != nullptr;
}

Control* VirtualTileBox::CreateElement()
{
    ASSERT(m_pDataProvider != nullptr);
    if (m_pDataProvider != nullptr) {
        return m_pDataProvider->CreateElement();
    }
    return nullptr;
}

void VirtualTileBox::FillElement(Control* pControl, size_t iIndex)
{
    ASSERT(m_pDataProvider != nullptr);
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

void VirtualTileBox::Refresh()
{
    if (!HasDataProvider()) {
        return;
    }
    //���������
    size_t nMaxItemCount = GetTileLayout()->AjustMaxItem(GetPosWithoutPadding());
    if (nMaxItemCount == 0) {
        return;
    }

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

void VirtualTileBox::GetDisplayCollection(std::vector<size_t>& collection)
{
    collection.clear();
    if (GetItemCount() == 0) {
        return;
    }

    // ��ȡBox��Rect
    ui::UiRect rcThis = GetPosWithoutPadding();

    size_t nEleHeight = GetRealElementHeight();
    if (nEleHeight == 0) {
        return;
    }

    size_t min = ((size_t)GetScrollPos().cy / nEleHeight) * GetColumns();
    size_t max = min + ((size_t)rcThis.Height() / nEleHeight) * GetColumns();

    size_t nCount = GetElementCount();
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

void VirtualTileBox::EnsureVisible(size_t iIndex, bool bToTop)
{
    if (!Box::IsValidItemIndex(iIndex) || iIndex >= GetElementCount()) {
        return;
    }
    if (m_pVScrollBar == nullptr) {
        return;
    }
    int64_t nPos = GetScrollPos().cy;
    int64_t elementHeight = GetRealElementHeight();
    if (elementHeight == 0) {
        return;
    }
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
    bool isChanged = GetScrollPos().cy != szPos.cy;
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
            __super::HandleEvent(event);
        }
        return;
    }
    if (!HasDataProvider()) {
        return __super::HandleEvent(event);
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

bool VirtualTileBox::RemoveItem(Control* pControl)
{
    return __super::RemoveItem(pControl);
}

bool VirtualTileBox::RemoveItemAt(size_t iIndex)
{
    return __super::RemoveItemAt(iIndex);
}

void VirtualTileBox::RemoveAllItems()
{
    return __super::RemoveAllItems();
}

bool VirtualTileBox::NeedReArrange()
{
    if (!HasDataProvider()) {
        return false;
    }
    size_t nCount = GetItemCount();
    if (nCount == 0) {
        return false;
    }

    if (GetElementCount() <= nCount) {
        return false;
    }

    ui::UiRect rcThis = this->GetPos();
    if (rcThis.IsEmpty()) {
        return false;
    }

    int64_t nPos = GetScrollPos().cy;
    int64_t nVirtualOffsetY = GetScrollVirtualOffset().cy;
    if (nPos >= nVirtualOffsetY) {
        //���¹���
        ui::UiRect rcItem = m_items[nCount - 1]->GetPos();
        if ((rcItem.bottom + nVirtualOffsetY) < (nPos + rcThis.bottom)) {
            return true;
        }
    }
    else {
        //���Ϲ���
        ui::UiRect rcItem = m_items[0]->GetPos();
        if ((rcItem.top + nVirtualOffsetY) > (nPos + rcThis.top)) {
            return true;
        }
    }
    return false;
}

void VirtualTileBox::ReArrangeChild(bool bForce)
{
    if (!HasDataProvider()) {
        return;
    }
    if (!bForce) {
        if (!NeedReArrange()) {
            return;
        }
    }
    LazyArrangeChild();
}

void VirtualTileBox::LazyArrangeChild()
{
    if (!HasDataProvider()) {
        return;
    }
    GetTileLayout()->LazyArrangeChild(GetPosWithoutPadding());
}

int64_t VirtualTileBox::CalcElementsHeight(size_t nCount)
{
    return GetTileLayout()->GetElementsHeight(GetPosWithoutPadding(), nCount);
}

VirtualTileLayout* VirtualTileBox::GetTileLayout()
{
    auto* pLayout = dynamic_cast<VirtualTileLayout*>(GetLayout());
    ASSERT(pLayout != nullptr);
    return pLayout;
}

int64_t VirtualTileBox::GetRealElementHeight()
{
    return GetTileLayout()->GetElementsHeight(GetPosWithoutPadding(), 1);
}

size_t VirtualTileBox::GetColumns()
{
    return GetTileLayout()->CalcTileColumns(GetPosWithoutPadding().Width());
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

size_t VirtualTileBox::GetTopElementIndex()
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
        return 0;
    }
    int64_t iIndex = (nPos / nHeight) * nColumns;
    return static_cast<size_t>(iIndex);
}

size_t VirtualTileBox::ElementIndexToItemIndex(size_t nElementIndex)
{
    if (IsElementDisplay(nElementIndex)) {
        size_t nTopItemIndex = GetTopElementIndex();
        ASSERT(nElementIndex >= nTopItemIndex);
        if (nElementIndex >= nTopItemIndex) {
            return nElementIndex - nTopItemIndex;
        }
    }
    return Box::InvalidIndex;
}

size_t VirtualTileBox::ItemIndexToElementIndex(size_t nItemIndex)
{
    return GetTopElementIndex() + nItemIndex;
}

}
