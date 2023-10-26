#include "ListCtrl.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Render/IRender.h"
#include <unordered_map>

namespace ui
{
////////////////////////////////////////////////////////
//ListCtrlHeaderItem

ListCtrlHeaderItem::ListCtrlHeaderItem() :
    m_pSortedDownImage(nullptr),
    m_pSortedUpImage(nullptr),
    m_sortMode(SortMode::kDown),
    m_pSplitBox(nullptr),
    m_bColumnResizeable(true),
    m_nColumnWidth(0),
    m_bMouseDown(false),
    m_bInDragging(false),
    m_nOldAlpha(255)
{
    m_nIconSpacing = GlobalManager::Instance().Dpi().GetScaleInt(6);
}

ListCtrlHeaderItem::~ListCtrlHeaderItem()
{
    if (m_pSortedDownImage != nullptr) {
        delete m_pSortedDownImage;
        m_pSortedDownImage = nullptr;
    }
    if (m_pSortedUpImage != nullptr) {
        delete m_pSortedUpImage;
        m_pSortedUpImage = nullptr;
    }
}
 
std::wstring ListCtrlHeaderItem::GetType() const { return L"ListCtrlHeaderItem"; }

void ListCtrlHeaderItem::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"sorted_up_image") {
        SetSortedUpImage(strValue);
    }
    else if (strName == L"sorted_down_image") {
        SetSortedDownImage(strValue);
    }
    else if (strName == L"icon_spacing") {
        SetIconSpacing(_wtoi(strValue.c_str()), true);
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrlHeaderItem::PaintText(IRender* pRender)
{
    __super::PaintText(pRender);
    if (pRender == nullptr) {
        return;
    }

    Image* pImage = nullptr;
    if (m_sortMode == SortMode::kUp) {
        //����
        pImage = m_pSortedUpImage;
    }
    else if (m_sortMode == SortMode::kDown) {
        //����
        pImage = m_pSortedDownImage;
    }
    if (pImage == nullptr) {
        return;
    }
    
    int32_t nIconTextSpacing = m_nIconSpacing;
    UiRect rc = GetRect();
    UiPadding rcPadding = GetControlPadding();
    rc.Deflate(rcPadding);
    rc.Deflate(GetTextPadding());
    uint32_t textStyle = GetTextStyle();
    std::wstring textValue = GetText();
    UiRect textRect = pRender->MeasureString(textValue, GetFontId(), textStyle);
    if (textStyle & TEXT_CENTER) {
        rc.left = rc.CenterX() + textRect.Width() / 2;
        rc.left += nIconTextSpacing;
    }
    else if (textStyle & TEXT_RIGHT) {
        rc.left = rc.right - textRect.Width() - nIconTextSpacing;
        if (pImage != nullptr) {
            if (pImage->GetImageCache() == nullptr) {
                LoadImageData(*pImage);
            }
            if (pImage->GetImageCache() != nullptr) {
                rc.left -= pImage->GetImageCache()->GetWidth();
            }
        }
    }
    else {
        rc.left += textRect.Width();
        rc.left += nIconTextSpacing;
    }
    rc.Validate();

    //��������ͼ��
    PaintImage(pRender, pImage, L"", -1, nullptr, &rc, nullptr);
}

void ListCtrlHeaderItem::Activate()
{
    if (!this->IsActivatable()) {
        return;
    }
    if (m_sortMode == SortMode::kUp) {
        m_sortMode = SortMode::kDown;
        Invalidate();
    }
    else if (m_sortMode == SortMode::kDown) {
        m_sortMode = SortMode::kUp;
        Invalidate();
    }
    __super::Activate();
}

void ListCtrlHeaderItem::SetSortMode(SortMode sortMode)
{
    if (m_sortMode != sortMode) {
        m_sortMode = sortMode;
        Invalidate();
    }
}

ListCtrlHeaderItem::SortMode ListCtrlHeaderItem::GetSortMode() const
{
    return m_sortMode;
}

void ListCtrlHeaderItem::SetSortedDownImage(const std::wstring& sImageString)
{
    if (m_pSortedDownImage == nullptr) {
        m_pSortedDownImage = new Image;
    }
    m_pSortedDownImage->SetImageString(sImageString);
    Invalidate();
}

void ListCtrlHeaderItem::SetSortedUpImage(const std::wstring& sImageString)
{
    if (m_pSortedUpImage == nullptr) {
        m_pSortedUpImage = new Image;
    }
    m_pSortedUpImage->SetImageString(sImageString);
    Invalidate();
}

size_t ListCtrlHeaderItem::GetColomnId() const
{
    return (size_t)this;
}

void ListCtrlHeaderItem::SetSplitBox(SplitBox* pSplitBox)
{
    m_pSplitBox = pSplitBox;
    if (pSplitBox != nullptr) {
        ASSERT(pSplitBox->GetFixedWidth().IsInt32());
        pSplitBox->SetEnabled(IsColumnResizeable() ? true : false);
    }
    if (GetColumnWidth() > 0) {
        CheckColumnWidth();
    }
}

SplitBox* ListCtrlHeaderItem::GetSplitBox() const
{
    return m_pSplitBox;
}

void ListCtrlHeaderItem::SetColumnResizeable(bool bResizeable)
{
    m_bColumnResizeable = bResizeable;
    if (m_pSplitBox != nullptr) {
        m_pSplitBox->SetEnabled(IsColumnResizeable() ? true : false);
    }
}

bool ListCtrlHeaderItem::IsColumnResizeable() const
{
    return m_bColumnResizeable;
}

void ListCtrlHeaderItem::SetColumnWidth(int32_t nWidth, bool bNeedDpiScale)
{
    if (nWidth < 0) {
        nWidth = 0;
    }
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nWidth);
    }
    m_nColumnWidth = nWidth;
    CheckColumnWidth();
}

int32_t ListCtrlHeaderItem::GetColumnWidth() const
{
    return m_nColumnWidth;
}

void ListCtrlHeaderItem::CheckColumnWidth()
{
    int32_t nSplitWidth = 0;
    if (m_pSplitBox != nullptr) {
        ASSERT(m_pSplitBox->GetFixedWidth().IsInt32());
        nSplitWidth = m_pSplitBox->GetFixedWidth().GetInt32();
    }
    int32_t nWidth = GetFixedWidth().GetInt32();
    if ((nWidth + nSplitWidth) != GetColumnWidth()) {
        nWidth = GetColumnWidth() - nSplitWidth;
        if (nWidth < 0) {
            nWidth = 0;
        }
        SetFixedWidth(UiFixedInt(nWidth), true, false);
    }
}

void ListCtrlHeaderItem::SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nIconSpacing);
    }
    m_nIconSpacing = nIconSpacing;
    if (m_nIconSpacing < 0) {
        m_nIconSpacing = 0;
    }
}

int32_t ListCtrlHeaderItem::GetIconSpacing() const
{
    return m_nIconSpacing;
}

bool ListCtrlHeaderItem::SetCheckBoxVisible(bool bVisible)
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            pCheckBox->SetVisible(bVisible);
            return true;
        }
    }
    return false;
}

bool ListCtrlHeaderItem::IsCheckBoxVisible() const
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            return pCheckBox->IsVisible();
        }
    }
    return false;
}

bool ListCtrlHeaderItem::HasCheckBox() const
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            return true;
        }
    }
    return false;
}

bool ListCtrlHeaderItem::SetCheckBoxSelect(bool bSelected, bool bPartSelect)
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            pCheckBox->SetSelected(bSelected);
            if (bSelected) {
                pCheckBox->SetPartSelected(bPartSelect);
            }
            pCheckBox->Invalidate();
            return true;
        }
    }
    return false;
}

bool ListCtrlHeaderItem::ButtonDown(const EventArgs& msg)
{
    bool bRet = __super::ButtonDown(msg);
    ListCtrlHeader* pParentBox = dynamic_cast<ListCtrlHeader*>(GetParent());
    ASSERT(pParentBox != nullptr);
    if (pParentBox == nullptr) {
        return bRet;
    }
    if (!pParentBox->IsEnableHeaderDragOrder()) {
        //��֧���϶�����˳��
        return bRet;
    }
    m_bMouseDown = true;
    m_ptMouseDown = msg.ptMouse;
    m_rcMouseDown = GetRect();
    
    size_t nItemCount = pParentBox->GetItemCount();
    for (size_t index = 0; index < nItemCount; ++index) {
        ItemStatus itemStatus;
        itemStatus.m_index = index;
        itemStatus.m_pItem = pParentBox->GetItemAt(index);
        if (itemStatus.m_pItem != nullptr) {
            itemStatus.m_rcPos = itemStatus.m_pItem->GetRect();
            m_rcItemList.push_back(itemStatus);
        }
        if (itemStatus.m_pItem == this) {
            ASSERT(itemStatus.m_rcPos.ContainsPt(m_ptMouseDown));
        }
    }
    return bRet;
}

bool ListCtrlHeaderItem::ButtonUp(const EventArgs& msg)
{
    bool bRet = __super::ButtonUp(msg);
    Box* pParentBox = dynamic_cast<Box*>(GetParent());
    if (pParentBox == nullptr) {
        ClearDragStatus();
        return bRet;
    }

    bool bOrderChanged = false;
    const size_t itemCount = pParentBox->GetItemCount();
    size_t nMouseItemIndex = Box::InvalidIndex;
    size_t nCurrentItemIndex = Box::InvalidIndex;
    for (const ItemStatus& itemStatus : m_rcItemList) {
        if (itemStatus.m_rcPos.ContainsPt(msg.ptMouse)) {
            nMouseItemIndex = itemStatus.m_index;
        }
        if ((itemStatus.m_pItem == this) && !itemStatus.m_rcPos.ContainsPt(msg.ptMouse)){
            nCurrentItemIndex = itemStatus.m_index;
        }
    }
    if ((nMouseItemIndex != Box::InvalidIndex) && 
        (nCurrentItemIndex != Box::InvalidIndex) &&
        (nMouseItemIndex < itemCount) &&
        (nCurrentItemIndex < itemCount)) {
        //�����ؼ���λ��
        if (nMouseItemIndex < nCurrentItemIndex) {
            //����ཻ��
            pParentBox->SetItemIndex(this, nMouseItemIndex);
            if (m_pSplitBox != nullptr) {
                size_t nNewIndex = pParentBox->GetItemIndex(this);
                ASSERT(nNewIndex < itemCount);
                ASSERT((nNewIndex + 1) < itemCount);
                if ((nNewIndex + 1) < itemCount) {
                    pParentBox->SetItemIndex(m_pSplitBox, nNewIndex + 1);
                }
            }
        }
        else {
            //���Ҳཻ��
            nMouseItemIndex += 1;
            ASSERT(nMouseItemIndex < itemCount);
            if (nMouseItemIndex < itemCount) {
                pParentBox->SetItemIndex(this, nMouseItemIndex);
                if (m_pSplitBox != nullptr) {
                    size_t nNewIndex = pParentBox->GetItemIndex(this);
                    ASSERT(nNewIndex < itemCount);
                    pParentBox->SetItemIndex(m_pSplitBox, nNewIndex);
                }
            }
        }
        bOrderChanged = true;
        ASSERT(pParentBox->GetItemIndex(this) == (pParentBox->GetItemIndex(m_pSplitBox) - 1));

        //�����󣬶����е������У��
        for (size_t index = 0; index < itemCount; index += 2) {
            ASSERT(dynamic_cast<ListCtrlHeaderItem*>(pParentBox->GetItemAt(index)) != nullptr);
            ASSERT((index + 1) < itemCount);
            if ((index + 1) >= itemCount) {
                break;
            }
            ASSERT(dynamic_cast<SplitBox*>(pParentBox->GetItemAt(index + 1)) != nullptr);
            ASSERT(dynamic_cast<SplitBox*>(pParentBox->GetItemAt(index + 1)) == 
                   dynamic_cast<ListCtrlHeaderItem*>(pParentBox->GetItemAt(index))->m_pSplitBox);
        }
    }
    ClearDragStatus();

    if (bOrderChanged) {
        //�����н����¼�
        ListCtrlHeader* pHeader = dynamic_cast<ListCtrlHeader*>(GetParent());
        ASSERT(pHeader != nullptr);
        if (pHeader != nullptr) {
            pHeader->OnHeaderColumnOrderChanged();
        }
    }
    return bRet;
}

bool ListCtrlHeaderItem::MouseMove(const EventArgs& msg)
{
    bool bRet = __super::MouseMove(msg);
    if (!m_bMouseDown) {
        return bRet;
    }
    Control* pParent = GetParent();
    if (pParent == nullptr) {
        return bRet;
    }
    UiPoint pt = msg.ptMouse;
    int32_t xOffset = pt.x - m_ptMouseDown.x;
    if (std::abs(xOffset) < GlobalManager::Instance().Dpi().GetScaleInt(2)) {
        return bRet;
    }

    UiRect boxRect = pParent->GetRect();
    if ((pt.x >= boxRect.left) && (pt.x < boxRect.right)) {
        UiRect rect = m_rcMouseDown;
        rect.left += xOffset;
        rect.right += xOffset;
        SetPos(rect);

        if (!m_bInDragging) {
            m_bInDragging = true;
            m_nOldAlpha = (uint8_t)GetAlpha();
            //����Ϊ��͸����Ч��
            SetAlpha(216);
        }

        AdjustHeaderItemPos(pt);
    }
    return bRet;
}

bool ListCtrlHeaderItem::OnWindowKillFocus(const EventArgs& msg)
{
    bool bRet = __super::OnWindowKillFocus(msg);
    ClearDragStatus();
    return bRet;
}

void ListCtrlHeaderItem::AdjustHeaderItemPos(const UiPoint& mousePt)
{
    Control* pMouseItem = nullptr;
    size_t nMouseItemIndex = Box::InvalidIndex;
    size_t nMouseDownItemIndex = Box::InvalidIndex;
    for (const ItemStatus& itemStatus : m_rcItemList) {
        if (itemStatus.m_rcPos.ContainsPt(mousePt)) {
            pMouseItem = itemStatus.m_pItem;
            nMouseItemIndex = itemStatus.m_index;
        }
        if (itemStatus.m_pItem == this) {
            nMouseDownItemIndex = itemStatus.m_index;
        }
    }
    if ((pMouseItem == nullptr) ||
        (nMouseItemIndex == Box::InvalidIndex) || 
        (nMouseDownItemIndex == Box::InvalidIndex)) {
        return;
    }
    if (dynamic_cast<ListCtrlHeaderItem*>(pMouseItem) == nullptr) {
        //��겻�ڱ�ͷ�ؼ���
        return;
    }

    const size_t itemCount = m_rcItemList.size();
    int32_t xOffset = mousePt.x - m_ptMouseDown.x;
    if (pMouseItem == this) {
        //��ǰ���λ�ã��������λ�ã��ָ������ؼ���ʵ��λ��
        for (ItemStatus& item : m_rcItemList) {
            if (item.m_pItem == this) {
                continue;
            }
            if (item.m_pItem->GetRect() != item.m_rcPos) {
                item.m_pItem->SetPos(item.m_rcPos);
            }
        }
    }
    else if (xOffset < 0) {
        //��ǰ���λ�ã��ڰ��µ����࣬���Ҳ��ƶ��ؼ�
        for (size_t index = 0; index < itemCount; ++index) {
            ItemStatus& item = m_rcItemList[index];
            if (item.m_pItem == this) {
                //�ָ�������Split�ؼ�λ��
                if ((index + 1) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index + 1];
                    if ((nextItem.m_pItem->GetRect() != nextItem.m_rcPos)) {
                        nextItem.m_pItem->SetPos(nextItem.m_rcPos);
                    }
                }
                continue;
            }
            else if ((item.m_index >= nMouseItemIndex) && (item.m_index < nMouseDownItemIndex)) {
                //���Ҳ��ƶ�
                if ((index + 2) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index + 2];
                    item.m_pItem->SetPos(nextItem.m_rcPos);
                }
                else {
                    if (item.m_pItem->GetRect() != item.m_rcPos) {
                        item.m_pItem->SetPos(item.m_rcPos);
                    }
                }
            }
            else {
                //�ָ�ԭλ��
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
    else {
        //��ǰ���λ�ã��ڰ��µ���Ҳ࣬������ƶ��ؼ�
        for (size_t index = 0; index < itemCount; ++index) {
            ItemStatus& item = m_rcItemList[index];
            if (item.m_pItem == this) {
                //�ָ�������Split�ؼ�λ��
                if ((index + 1) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index + 1];
                    if ((nextItem.m_pItem->GetRect() != nextItem.m_rcPos)) {
                        nextItem.m_pItem->SetPos(nextItem.m_rcPos);
                    }
                }
                continue;
            }
            else if ((item.m_index > nMouseDownItemIndex) && (item.m_index <= nMouseItemIndex)) {
                //������ƶ�
                if ((index - 2) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index - 2];
                    item.m_pItem->SetPos(nextItem.m_rcPos);
                }
                else {
                    if (item.m_pItem->GetRect() != item.m_rcPos) {
                        item.m_pItem->SetPos(item.m_rcPos);
                    }
                }
            }
            else {
                //�ָ�ԭλ��
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
}

void ListCtrlHeaderItem::ClearDragStatus()
{
    if (m_bInDragging) {
        SetAlpha(m_nOldAlpha);
        m_nOldAlpha = 255;
        m_bInDragging = false;
    }
    m_bMouseDown = false;
    m_rcItemList.clear();
    if (GetParent() != nullptr) {
        GetParent()->Invalidate();
        GetParent()->SetPos(GetParent()->GetPos());
    }
}

////////////////////////////////////////////////////////////////
/** ListCtrl�ı�ͷ�ؼ�
*/
ListCtrlHeader::ListCtrlHeader() :
    m_pListCtrl(nullptr)
{
}

std::wstring ListCtrlHeader::GetType() const { return L"ListCtrlHeader"; }

ListCtrlHeaderItem* ListCtrlHeader::InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo)
{
    int32_t nColumnWidth = columnInfo.nColumnWidth;
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nullptr;
    }
    if (columnInfo.bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nColumnWidth);
    }
    if (nColumnWidth < 0) {
        nColumnWidth = 0;
    }

    ListCtrlHeaderItem* pHeaderItem = new ListCtrlHeaderItem;
    SplitBox* pHeaderSplit = new SplitBox;
    size_t nColumnCount = GetColumnCount();
    if ((size_t)columnIndex >= nColumnCount) {
        //�������
        AddItem(pHeaderItem);
        AddItem(pHeaderSplit);
    }
    else {
        //�������м�λ��        
        AddItemAt(pHeaderSplit, columnIndex);
        AddItemAt(pHeaderItem, columnIndex);
    }

    //��������
    if (!m_pListCtrl->GetHeaderSplitBoxClass().empty()) {
        pHeaderSplit->SetClass(m_pListCtrl->GetHeaderSplitBoxClass());
    }

    Control* pSplitCtrl = new Control;
    pSplitCtrl->SetMouseEnabled(false);
    pSplitCtrl->SetMouseFocused(false);
    pSplitCtrl->SetNoFocus();
    if (!m_pListCtrl->GetHeaderSplitControlClass().empty()) {
        pSplitCtrl->SetClass(m_pListCtrl->GetHeaderSplitControlClass());
    }
    pHeaderSplit->AddItem(pSplitCtrl);

    if (!m_pListCtrl->GetHeaderItemClass().empty()) {
        pHeaderItem->SetClass(m_pListCtrl->GetHeaderItemClass());
    }
    pHeaderItem->SetText(columnInfo.text);

    //���������Split�ؼ��ӿ�
    pHeaderItem->SetSplitBox(pHeaderSplit);
    pHeaderItem->SetColumnWidth(nColumnWidth, false);
    if (columnInfo.nColumnWidthMin > 0) {
        //�п���Сֵ
        pHeaderItem->SetMinWidth(columnInfo.nColumnWidthMin, columnInfo.bNeedDpiScale);
    }
    if (columnInfo.nColumnWidthMax > 0) {
        //�п����ֵ
        pHeaderItem->SetMaxWidth(columnInfo.nColumnWidthMax, columnInfo.bNeedDpiScale);
    }

    if (columnInfo.bSortable) {
        pHeaderItem->SetSortMode(ListCtrlHeaderItem::SortMode::kUp);
    }
    else {
        pHeaderItem->SetSortMode(ListCtrlHeaderItem::SortMode::kNone);
    }
    pHeaderItem->SetColumnResizeable(columnInfo.bResizeable);

    if (columnInfo.nTextFormat >= 0) {
        uint32_t textStyle = columnInfo.nTextFormat;
        //ˮƽ����
        if (columnInfo.nTextFormat & TEXT_CENTER) {
            //�ı������ж���
            textStyle &= ~(TEXT_LEFT | TEXT_RIGHT);
            textStyle |= TEXT_CENTER;            
        }
        else if (columnInfo.nTextFormat & TEXT_RIGHT) {
            //�ı����Ҷ���
            textStyle &= ~(TEXT_LEFT | TEXT_CENTER);
            textStyle |= TEXT_RIGHT;
        }
        else {
            //�ı��������
            textStyle &= ~(TEXT_CENTER | TEXT_RIGHT);
            textStyle |= TEXT_LEFT;
        }

        //��ֱ����
        if (columnInfo.nTextFormat & TEXT_VCENTER) {
            //�ı������ж���
            textStyle &= ~(TEXT_TOP | TEXT_BOTTOM);
            textStyle |= TEXT_VCENTER;
        }
        else if (columnInfo.nTextFormat & TEXT_BOTTOM) {
            //�ı����ײ�����
            textStyle &= ~(TEXT_TOP | TEXT_VCENTER);
            textStyle |= TEXT_BOTTOM;
        }
        else {
            //�ı�����������
            textStyle &= ~(TEXT_BOTTOM | TEXT_VCENTER);
            textStyle |= TEXT_TOP;
        }
        pHeaderItem->SetTextStyle(textStyle, true);
    }

    //CheckBox����
    if (columnInfo.bShowCheckBox) {
        std::wstring checkBoxClass = m_pListCtrl->GetCheckBoxClass();
        ASSERT(!checkBoxClass.empty());
        CheckBox* pCheckBox = new CheckBox;
        pHeaderItem->AddItem(pCheckBox);
        if (!checkBoxClass.empty()) {
            pCheckBox->SetClass(checkBoxClass);
        }
        UiPadding textPadding = pHeaderItem->GetTextPadding();
        int32_t nCheckBoxWidth = columnInfo.nCheckBoxWidth;
        if (columnInfo.bNeedDpiScale) {
            GlobalManager::Instance().Dpi().ScaleInt(nCheckBoxWidth);
        }
        if (textPadding.left < nCheckBoxWidth) {
            textPadding.left = nCheckBoxWidth;
            pHeaderItem->SetTextPadding(textPadding, false);
        }

        //����CheckBox���¼�����
        pCheckBox->AttachSelect([this, pHeaderItem](const EventArgs& /*args*/) {
            OnHeaderColumnCheckStateChanged(pHeaderItem, true);
            return true;
            });
        pCheckBox->AttachUnSelect([this, pHeaderItem](const EventArgs& /*args*/) {
            OnHeaderColumnCheckStateChanged(pHeaderItem, false);
            return true;
            });
    }

    //���������¼�
    pHeaderItem->AttachClick([this, pHeaderItem](const EventArgs& /*args*/) {
        OnHeaderColumnSorted(pHeaderItem);
        return true;
        });

        //�����϶���Ӧ�¼�
    pHeaderSplit->AttachSplitDraged([this](const EventArgs& args) {
        OnHeaderColumnResized((Control*)args.wParam, (Control*)args.lParam);
        return true;
        });

    m_pListCtrl->OnHeaderColumnAdded(pHeaderItem->GetColomnId());
    return pHeaderItem;
}

size_t ListCtrlHeader::GetColumnCount() const
{
    size_t nItemCount = GetItemCount();
    if (nItemCount == 0) {
        return 0;
    }
    ASSERT((nItemCount % 2) == 0);
    if ((nItemCount % 2) != 0) {
        return 0;
    }
    const size_t nColumnCount = nItemCount / 2;
#ifdef _DEBUG
    //У��ṹ�Ƿ����Ԥ��    
    for (size_t index = 0; index < nColumnCount; ++index) {
        ASSERT(dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2)) != nullptr);
        ASSERT(dynamic_cast<SplitBox*>(GetItemAt(index * 2 + 1)) != nullptr);
    }
#endif // _DEBUG  
    
    return nColumnCount;
}

int32_t ListCtrlHeader::GetColumnWidth(size_t columnIndex) const
{
    int32_t nColumnWidth = 0;
    size_t nColumnCount = GetColumnCount();
    ASSERT(columnIndex < nColumnCount);
    if (columnIndex >= nColumnCount) {
        return nColumnWidth;
    }
    ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(columnIndex * 2));
    ASSERT(pHeaderItem != nullptr);
    if (pHeaderItem != nullptr) {
        nColumnWidth = pHeaderItem->GetColumnWidth();
    }
    return nColumnWidth;
}

ListCtrlHeaderItem* ListCtrlHeader::GetColumn(size_t columnIndex) const
{
    size_t nColumnCount = GetColumnCount();
    ASSERT(columnIndex < nColumnCount);
    if (columnIndex >= nColumnCount) {
        return nullptr;
    }
    ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(columnIndex * 2));
    ASSERT(pHeaderItem != nullptr);
    return pHeaderItem;
}

ListCtrlHeaderItem* ListCtrlHeader::GetColumnById(size_t columnId) const
{
    ListCtrlHeaderItem* pFoundHeaderItem = nullptr;
    size_t nColumnCount = GetColumnCount();
    for (size_t index = 0; index < nColumnCount; ++index) {
        ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2));
        ASSERT(pHeaderItem != nullptr);
        if (pHeaderItem != nullptr) {
            if (pHeaderItem->GetColomnId() == columnId) {
                pFoundHeaderItem = pHeaderItem;
                break;
            }
        }
    }
    return pFoundHeaderItem;
}

bool ListCtrlHeader::GetColumnInfo(size_t columnId, size_t& columnIndex, int32_t& nColumnWidth) const
{
    bool bRet = false;
    columnIndex = Box::InvalidIndex;
    nColumnWidth = -1;
    size_t nColumnCount = GetColumnCount();
    for (size_t index = 0; index < nColumnCount; ++index) {
        ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2));
        ASSERT(pHeaderItem != nullptr);
        if (pHeaderItem != nullptr) {
            if (pHeaderItem->GetColomnId() == columnId) {
                nColumnWidth = pHeaderItem->GetColumnWidth();
                columnIndex = index;
                bRet = true;
                break;
            }
        }
    }
    return bRet;
}

size_t ListCtrlHeader::GetColumnIndex(size_t columnId) const
{
    size_t columnIndex = Box::InvalidIndex;
    int32_t nColumnWidth = -1;
    GetColumnInfo(columnId, columnIndex, nColumnWidth);
    return columnIndex;
}

size_t ListCtrlHeader::GetColumnId(size_t columnIndex) const
{
    size_t columnId = Box::InvalidIndex;
    ListCtrlHeaderItem* pHeaderItem = GetColumn(columnIndex);
    if (pHeaderItem != nullptr) {
        columnId = pHeaderItem->GetColomnId();
    }
    return columnId;
}

bool ListCtrlHeader::DeleteColumn(size_t columnIndex)
{
    bool bRet = false;
    size_t columnId = Box::InvalidIndex;
    size_t nColumnCount = GetColumnCount();
    if (columnIndex < nColumnCount) {
        ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(columnIndex * 2));
        ASSERT(pHeaderItem != nullptr);
        if (pHeaderItem != nullptr) {
            columnId = pHeaderItem->GetColomnId();
            if (pHeaderItem->GetSplitBox() != nullptr) {
                ASSERT(dynamic_cast<SplitBox*>(GetItemAt(columnIndex * 2 + 1)) == pHeaderItem->GetSplitBox());
                RemoveItem(pHeaderItem->GetSplitBox());
            }
            RemoveItem(pHeaderItem);
            bRet = true;
        }
    }
    if (bRet && (m_pListCtrl != nullptr)) {
        m_pListCtrl->OnHeaderColumnRemoved(columnId);
    }
    return bRet;
}

void ListCtrlHeader::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

bool ListCtrlHeader::IsEnableHeaderDragOrder() const
{
    if (m_pListCtrl != nullptr) {
        return m_pListCtrl->IsEnableHeaderDragOrder();
    }
    return false;
}

void ListCtrlHeader::OnHeaderColumnResized(Control* pLeftHeaderItem, Control* pRightHeaderItem)
{
    size_t nColumnId1 = Box::InvalidIndex;
    size_t nColumnId2 = Box::InvalidIndex;
    ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(pLeftHeaderItem);
    if (pHeaderItem != nullptr) {
        int32_t nSplitWidth = 0;
        if (pHeaderItem->GetSplitBox() != nullptr) {
            nSplitWidth = pHeaderItem->GetSplitBox()->GetFixedWidth().GetInt32();
        }
        int32_t nItemWidth = pHeaderItem->GetFixedWidth().GetInt32();
        int32_t nColumnWidth = nItemWidth + nSplitWidth;
        pHeaderItem->SetColumnWidth(nColumnWidth, false);
        nColumnId1 = pHeaderItem->GetColomnId();
    }
    pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(pRightHeaderItem);
    if (pHeaderItem != nullptr) {
        int32_t nSplitWidth = 0;
        if (pHeaderItem->GetSplitBox() != nullptr) {
            nSplitWidth = pHeaderItem->GetSplitBox()->GetFixedWidth().GetInt32();
        }
        int32_t nItemWidth = pHeaderItem->GetFixedWidth().GetInt32();
        int32_t nColumnWidth = nItemWidth + nSplitWidth;
        pHeaderItem->SetColumnWidth(nColumnWidth, false);
        nColumnId2 = pHeaderItem->GetColomnId();
    }

    if ((nColumnId1 != Box::InvalidIndex) || (nColumnId2 != Box::InvalidIndex)) {
        if (m_pListCtrl != nullptr) {
            m_pListCtrl->OnColumnWidthChanged(nColumnId1, nColumnId2);
        }
    }
}

void ListCtrlHeader::OnHeaderColumnSorted(ListCtrlHeaderItem* pHeaderItem)
{
    if (pHeaderItem == nullptr) {
        return;
    }
    size_t nColumnId = pHeaderItem->GetColomnId();
    ListCtrlHeaderItem::SortMode sortMode = pHeaderItem->GetSortMode();
    ASSERT(sortMode != ListCtrlHeaderItem::SortMode::kNone);
    if (sortMode == ListCtrlHeaderItem::SortMode::kNone) {
        return;
    }
    if (m_pListCtrl != nullptr) {
        bool bSortedUp = (sortMode == ListCtrlHeaderItem::SortMode::kUp) ? true : false;
        m_pListCtrl->OnColumnSorted(nColumnId, bSortedUp);
    }
}

void ListCtrlHeader::OnHeaderColumnOrderChanged()
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->OnHeaderColumnOrderChanged();
    }
}

void ListCtrlHeader::OnHeaderColumnCheckStateChanged(ListCtrlHeaderItem* pHeaderItem, bool bChecked)
{
    if (pHeaderItem == nullptr) {
        return;
    }
    size_t nColumnId = pHeaderItem->GetColomnId();
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->OnHeaderColumnCheckStateChanged(nColumnId, bChecked);
    }
}

/////////////////////////////////////////////////////////////////
/** �б�������ݹ�����
*/
class ListCtrlDataProvider : public ui::VirtualListBoxElement
{
public:
    /** �洢�����ݽṹ
    */
    struct Storage
    {
        UiString text;                  //�ı�����
        uint16_t nTextFormat = 0;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
        int32_t nImageIndex = -1;       //ͼ����Դ�����ţ���ͼƬ�б�������±�ֵ�����Ϊ-1��ʾ����ʾͼ��
        UiColor textColor;              //�ı���ɫ
        UiColor bkColor;                //������ɫ
        bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox
        uint8_t nCheckBoxWidth = 0;     //CheckBox�ؼ���ռ�Ŀ�ȣ�����bShowCheckBoxΪtrueʱ��Ч
        bool bSelected = false;         //�Ƿ���ѡ��״̬��ListBoxItem������ѡ�У�
        bool bChecked = false;          //�Ƿ��ڹ�ѡ״̬��CheckBox��ѡ״̬��
        size_t nItemData = 0;           //�û��Զ�������
    };
public:
    ListCtrlDataProvider();

    /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual Control* CreateElement() override;

    /** ���ָ��������
    * @param [in] pControl ������ؼ�ָ��
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    */
    virtual bool FillElement(ui::Control* pControl, size_t nElementIndex) override;

    /** ��ȡ����������
    * @return ��������������
    */
    virtual size_t GetElementCount() override;

    /** ����ѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] bSelected true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual void SetElementSelected(size_t nElementIndex, bool bSelected) override;

    /** ��ȡѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @return true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual bool IsElementSelected(size_t nElementIndex) override;

public:
    /** ���ñ�ͷ�ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

    /** ����һ��
    * @param [in] columnId �е�ID
    */
    bool AddColumn(size_t columnId);

    /** ɾ��һ��
    * @param [in] columnId �е�ID
    */
    bool RemoveColumn(size_t columnId);

    /** ����һ�еĹ�ѡ״̬��Checked����UnChecked��
    * @param [in] columnId �е�ID
    * @param [in] bChecked true��ʾѡ��false��ʾȡ��ѡ��
    */
    bool SetColumnCheck(size_t columnId, bool bChecked);

    /** ��ȡ�������ܸ���
    */
    size_t GetDataItemCount() const;

    /** �����������ܸ���
    * @param [in] itemCount �����������������ÿ������������ݣ�ͨ���ص��ķ�ʽ������䣨�ڲ�Ϊ���ʵ�֣�
    */
    bool SetDataItemCount(size_t itemCount);

    /** ��������һ��������
    * @param [in] dataItem �����������
    * @return �ɹ��������������������(rowIndex)��ʧ���򷵻�Box::InvalidIndex
    */
    size_t AddDataItem(const ListCtrlDataItem& dataItem);

    /** ��ָ����λ�����һ��������
    * @param [in] itemIndex �������������
    * @param [in] dataItem �����������
    */
    bool InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ����ָ���е�������
    * @param [in] itemIndex �������������
    * @param [in] dataItem �����������
    */
    bool SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ɾ��ָ���е�������
    * @param [in] itemIndex �������������
    */
    bool DeleteDataItem(size_t itemIndex);

    /** ������������Զ�������
    * @param [in] itemIndex �������������
    * @param [in] itemData ������������Զ�������
    */
    bool SetDataItemData(size_t itemIndex, size_t itemData);

    /** ��ȡ��������Զ�������
    * @param [in] itemIndex �������������
    * @return ����������������Զ�������
    */
    size_t GetDataItemData(size_t itemIndex) const;

    /** ����ָ����������ı�
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] text ��Ҫ���õ��ı�����
    */
    bool SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text);

    /** ��ȡָ����������ı�
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ������������ı�����
    */
    std::wstring GetDataItemText(size_t itemIndex, size_t columnIndex) const;

    /** ����ָ����������ı���ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] textColor ��Ҫ���õ��ı���ɫ
    */
    bool SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor);

    /** ��ȡָ����������ı���ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] textColor ������������ı���ɫ
    */
    bool GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const;

    /** ����ָ��������ı�����ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] bkColor ��Ҫ���õı�����ɫ
    */
    bool SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor);

    /** ��ȡָ��������ı�����ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] bkColor ����������ı�����ɫ
    */
    bool GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const;

    /** ����������
    * @param [in] columnId �е�ID
    * @param [in] bSortedUp true��ʾ����false��ʾ����
    */
    bool SortColumnData(size_t nColumnId, bool bSortedUp);

private:
    /** UIԪ�������ź����������ŵ��໥ת��
    */
    size_t DataItemIndexToElementIndex(size_t itemIndex) const;
    size_t ElementIndexToDataItemIndex(size_t elementIndex) const;

    /** ����ת��Ϊ�洢���ݽṹ
    */
    void DataItemToStorage(Storage& storage, const ListCtrlDataItem& item) const;

    /** ��������Ų�����ID
    * @return ������ID�����ƥ�䲻�����򷵻�Box::InvalidIndex
    */
    size_t GetColumnId(size_t nColumnIndex) const;

    /** ������ID���������
    * @param [in] nColumnId ��ID
    * @return ��������ţ����ƥ�䲻�����򷵻�Box::InvalidIndex
    */
    size_t GetColumnIndex(size_t nColumnId) const;

    /** �ж�һ�������������Ƿ���Ч
    */
    bool IsValidDataItemIndex(size_t itemIndex) const;

    /** �ж�һ����ID�����ݴ洢���Ƿ���Ч
    */
    bool IsValidDataColumnId(size_t nColumnId) const;

    /** ��ȡָ�������������, ��ȡ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ���ʧ���򷵻�nullptr
    */
    std::shared_ptr<Storage> GetDataItemStorage(size_t itemIndex, size_t columnIndex) const;

    /** ��ȡָ�������������, д��
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ���ʧ���򷵻�nullptr
    */
    std::shared_ptr<Storage> GetDataItemStorageForWrite(size_t itemIndex, size_t columnIndex);

    /** ��ȡ�����е����ݣ�����UIչʾ
    * @param [in] nDataItemIndex ����Item���±꣬������
    * @param [in] columnIdList ��ID�б�
    * @param [out] storageList ���������б�
    */
    bool GetDataItemStorageList(size_t nDataItemIndex,
                                std::vector<size_t>& columnIdList,
                                std::vector<std::shared_ptr<Storage>>& storageList) const;

    /** ĳ���������Check��ѡ״̬�仯    
    * @param [in] itemIndex ����Item���±꣬������
    * @param [in] nColumnId ��ID
    * @param [in] bChecked �Ƿ�ѡ
    */
    void OnDataItemChecked(size_t itemIndex, size_t nColumnId, bool bChecked);

    /** ͬ��UI��Check״̬
    */
    void UpdateControlCheckStatus(size_t nColumnId);

private:
    /** ��������
    */
    struct StorageData
    {
        size_t index; //ԭ��������������
        std::shared_ptr<Storage> pStorage;
    };

    /** ����������
    * @param [in] dataList �����������
    * @param [in] bSortedUp true��ʾ����false��ʾ����
    */
    bool SortStorageData(std::vector<StorageData>& dataList, bool bSortedUp);

    /** Ĭ�ϵ������������������
    * @return ���a < b ����true�����򷵻�false
    */
    static bool SortStorageFunction(const StorageData& a, const StorageData& b);

private:
    /** ��ͷ�ؼ�
    */
    ListCtrl* m_pListCtrl;

    /** ���ݣ����б��棬ÿ����һ������
    */
    typedef std::shared_ptr<Storage> StoragePtr;
    typedef std::vector<StoragePtr> StoragePtrList;
    typedef std::unordered_map<size_t, std::vector<std::shared_ptr<Storage>>> StorageMap;
    StorageMap m_dataMap;
};

ListCtrlDataProvider::ListCtrlDataProvider() :
    m_pListCtrl(nullptr)
{
}

Control* ListCtrlDataProvider::CreateElement()
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nullptr;
    }
    std::wstring dataItemClass = m_pListCtrl->GetDataItemClass();
    ASSERT(!dataItemClass.empty());
    if (dataItemClass.empty()) {
        return nullptr;
    }
    ListCtrlItem* pItem = new ListCtrlItem;
    pItem->SetClass(dataItemClass);
    return pItem;
}

bool ListCtrlDataProvider::FillElement(ui::Control* pControl, size_t nElementIndex)
{
    ListCtrlHeader* pHeaderCtrl = nullptr;
    if (m_pListCtrl != nullptr) {
        pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    }
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return false;
    }
    if (nElementIndex == 0) {
        //��һ��Ԫ����List Header������Ҫ���
        return true;
    }
    ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(pControl);
    ASSERT(pItem != nullptr);
    if (pItem == nullptr) {
        return false;
    }
    // �����ṹ: <ListCtrlItem> <LabelBox/> ... <LabelBox/>  </ListCtrlItem>
    // ����˵��: 1. ListCtrlItem �� HBox������;   
    //          2. ÿһ�У�����һ��LabelBox�ؼ�

    //��ȡ��Ҫ��ʾ�ĸ����е�����
    struct ElementData
    {
        size_t nColumnId = Box::InvalidIndex;
        int32_t nColumnWidth = 0;
        StoragePtr pStorage;
    };
    std::vector<ElementData> elementDataList;
    const size_t nColumnCount = pHeaderCtrl->GetColumnCount();
    for (size_t nColumnIndex = 0; nColumnIndex < nColumnCount; ++nColumnIndex) {
        ListCtrlHeaderItem* pHeaderItem = pHeaderCtrl->GetColumn(nColumnIndex);
        if ((pHeaderItem == nullptr) || !pHeaderItem->IsVisible()){
            continue;
        }
        int32_t nColumnWidth = pHeaderCtrl->GetColumnWidth(nColumnIndex);
        if (nColumnWidth < 0) {
            nColumnWidth = 0;
        }
        ElementData data;
        data.nColumnId = pHeaderCtrl->GetColumnId(nColumnIndex);
        data.nColumnWidth = nColumnWidth;
        elementDataList.push_back(data);
    }
    std::vector<size_t> columnIdList;
    for (auto data : elementDataList) {
        columnIdList.push_back(data.nColumnId);
    }
    size_t nDataItemIndex = ElementIndexToDataItemIndex(nElementIndex);
    std::vector<std::shared_ptr<Storage>> storageList;
    if (!GetDataItemStorageList(nDataItemIndex, columnIdList, storageList)) {
        return false;
    }
    ASSERT(storageList.size() == elementDataList.size());
    for (size_t index = 0; index < storageList.size(); ++index) {
        elementDataList[index].pStorage = storageList[index];
    }
    storageList.clear();
    ASSERT(!elementDataList.empty());
    if (elementDataList.empty()) {
        return false;
    }

    const size_t showColumnCount = elementDataList.size(); //��ʾ������
    while (pItem->GetItemCount() > showColumnCount) {
        //�Ƴ��������
        if (pItem->RemoveItemAt(pItem->GetItemCount() - 1)) {
            ASSERT(!"RemoveItemAt failed!");
            return false;
        }
    }

    //Ĭ������
    std::wstring defaultLabelBoxClass;
    if (m_pListCtrl != nullptr) {
        defaultLabelBoxClass = m_pListCtrl->GetDataItemLabelClass();
    }
    LabelBox defaultLabelBox;
    if (!defaultLabelBoxClass.empty()) {
        defaultLabelBox.SetClass(defaultLabelBoxClass);
    }

    bool bFirstLine = (nElementIndex == 1);//��һ��������
    for (size_t nColumn = 0; nColumn < showColumnCount; ++nColumn) {
        const ElementData& elementData = elementDataList[nColumn];
        LabelBox* pLabelBox = nullptr;
        if (nColumn < pItem->GetItemCount()) {
            pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(nColumn));
            ASSERT(pLabelBox != nullptr);
            if (pLabelBox == nullptr) {
                return false;
            }
        }
        else {
            pLabelBox = new LabelBox;
            pItem->AddItem(pLabelBox);            
            if (!defaultLabelBoxClass.empty()) {
                pLabelBox->SetClass(defaultLabelBoxClass);
            }
            pLabelBox->SetMouseEnabled(false);
        }

        //������ݣ���������        
        pLabelBox->SetFixedWidth(UiFixedInt(elementData.nColumnWidth), true, false);
        const StoragePtr& pStorage = elementData.pStorage;
        if (pStorage != nullptr) {
            pLabelBox->SetText(pStorage->text.c_str());
            if (pStorage->nTextFormat != 0) {
                pLabelBox->SetTextStyle(pStorage->nTextFormat, false);
            }
            else {
                pLabelBox->SetTextStyle(defaultLabelBox.GetTextStyle(), false);
            }
            pLabelBox->SetTextPadding(defaultLabelBox.GetTextPadding(), false);
            if (!pStorage->textColor.IsEmpty()) {
                pLabelBox->SetStateTextColor(kControlStateNormal, pLabelBox->GetColorString(pStorage->textColor));
            }
            else {
                pLabelBox->SetStateTextColor(kControlStateNormal, defaultLabelBox.GetStateTextColor(kControlStateNormal));
            }
            if (!pStorage->bkColor.IsEmpty()) {
                pLabelBox->SetBkColor(pStorage->bkColor);
            }
            else {
                pLabelBox->SetBkColor(defaultLabelBox.GetBkColor());
            }
            if (pStorage->bShowCheckBox) {
                //���CheckBox
                CheckBox* pCheckBox = nullptr;
                if (pLabelBox->GetItemCount() > 0) {
                    pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                }
                if (pCheckBox == nullptr) {
                    pCheckBox = new CheckBox;
                    std::wstring checkBoxClass = m_pListCtrl->GetCheckBoxClass();
                    ASSERT(!checkBoxClass.empty());
                    pLabelBox->AddItem(pCheckBox);
                    if (!checkBoxClass.empty()) {
                        pCheckBox->SetClass(checkBoxClass);
                    }
                }
                UiPadding textPadding = pLabelBox->GetTextPadding();
                int32_t nCheckBoxWidth = pStorage->nCheckBoxWidth;
                if (textPadding.left < nCheckBoxWidth) {
                    textPadding.left = nCheckBoxWidth;
                    pLabelBox->SetTextPadding(textPadding, false);
                }

                //����CheckBox���¼�����
                pCheckBox->DetachEvent(kEventSelect);
                pCheckBox->DetachEvent(kEventUnSelect);
                pCheckBox->SetSelected(pStorage->bChecked);
                size_t nColumnId = elementData.nColumnId;
                pCheckBox->AttachSelect([this, nColumnId, nElementIndex](const EventArgs& /*args*/) {
                    OnDataItemChecked(ElementIndexToDataItemIndex(nElementIndex), nColumnId, true);
                    return true;
                    });
                pCheckBox->AttachUnSelect([this, nColumnId, nElementIndex](const EventArgs& /*args*/) {
                    OnDataItemChecked(ElementIndexToDataItemIndex(nElementIndex), nColumnId, false);
                    return true;
                    });
            }
            else {
                if (pLabelBox->GetItemCount() > 0) {
                    CheckBox* pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                    if (pCheckBox != nullptr) {
                        pLabelBox->RemoveItemAt(0);
                    }
                }
            }
        }
        else {
            pLabelBox->SetTextStyle(defaultLabelBox.GetTextStyle(), false);
            pLabelBox->SetText(defaultLabelBox.GetText());
            pLabelBox->SetTextPadding(defaultLabelBox.GetTextPadding(), false);
            pLabelBox->SetStateTextColor(kControlStateNormal, defaultLabelBox.GetStateTextColor(kControlStateNormal));
            pLabelBox->SetBkColor(defaultLabelBox.GetBkColor());
            if (pLabelBox->GetItemCount() > 0) {
                CheckBox* pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                if (pCheckBox != nullptr) {
                    pLabelBox->RemoveItemAt(0);
                }
            }
        }

        //���Ʊ���
        int32_t mColumnLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);//������߿��        
        int32_t mRowLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);   //������߿��
        if (m_pListCtrl != nullptr) {
            mColumnLineWidth = m_pListCtrl->GetColumnGridLineWidth();
            mRowLineWidth = m_pListCtrl->GetRowGridLineWidth();
        }
        if (bFirstLine) {
            //��һ��
            if (nColumn == 0) {
                //��һ��
                UiRect rc(mColumnLineWidth, mRowLineWidth, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
            else {
                //�ǵ�һ��
                UiRect rc(0, mRowLineWidth, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
        }
        else {
            //�ǵ�һ��
            if (nColumn == 0) {
                //��һ��
                UiRect rc(mColumnLineWidth, 0, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
            else {
                //�ǵ�һ��
                UiRect rc(0, 0, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
        }
    }
    return true;
}

size_t ListCtrlDataProvider::GetElementCount()
{
    return DataItemIndexToElementIndex(GetDataItemCount());
}

void ListCtrlDataProvider::SetElementSelected(size_t nElementIndex, bool bSelected)
{
    size_t nItemIndex = ElementIndexToDataItemIndex(nElementIndex);
    if (nItemIndex == Box::InvalidIndex) {
        return;
    }
    StoragePtrList& storageList = m_dataMap[0];
    ASSERT(nItemIndex < storageList.size());
    if (nItemIndex < storageList.size()) {
        StoragePtr pStorage = storageList.at(nItemIndex);
        if (pStorage != nullptr) {
            pStorage->bSelected = bSelected;
        }
        else {
            pStorage = std::make_shared<Storage>();
            pStorage->bSelected = bSelected;
            storageList[nItemIndex] = pStorage;
        }
    }
}

bool ListCtrlDataProvider::IsElementSelected(size_t nElementIndex)
{
    size_t nItemIndex = ElementIndexToDataItemIndex(nElementIndex);
    if (nItemIndex == Box::InvalidIndex) {
        return false;
    }
    bool bSelected = false;
    const StoragePtrList& storageList = m_dataMap[0];
    ASSERT(nItemIndex < storageList.size());
    if (nItemIndex < storageList.size()) {
        const StoragePtr pStorage = storageList.at(nItemIndex);
        if (pStorage != nullptr) {
            bSelected = pStorage->bSelected;
        }        
    }
    return bSelected;
}

void ListCtrlDataProvider::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

size_t ListCtrlDataProvider::DataItemIndexToElementIndex(size_t itemIndex) const
{
    //��һ��Ԫ����Header������
    if (itemIndex != Box::InvalidIndex) {
        return itemIndex + 1;
    }
    return itemIndex;
}

size_t ListCtrlDataProvider::ElementIndexToDataItemIndex(size_t elementIndex) const
{
    if (elementIndex == 0) {
        return Box::InvalidIndex;
    }
    else if (elementIndex != Box::InvalidIndex) {
        return elementIndex - 1;
    }
    return Box::InvalidIndex;
}

void ListCtrlDataProvider::DataItemToStorage(Storage& storage, const ListCtrlDataItem& item) const
{
    storage.text = item.text;
    if (item.nTextFormat >= 0) {
        storage.nTextFormat = TruncateToUInt16(item.nTextFormat);
    }
    else {
        storage.nTextFormat = 0;
    }
    int32_t nCheckBoxWidth = item.nCheckBoxWidth;
    if (item.bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nCheckBoxWidth);
    }
    storage.nImageIndex = item.nImageIndex;
    storage.textColor = item.textColor;
    storage.bkColor = item.bkColor;
    storage.bShowCheckBox = item.bShowCheckBox;
    storage.nCheckBoxWidth = TruncateToUInt8(nCheckBoxWidth);

    storage.bSelected = false;
    storage.bChecked = false;
    storage.nItemData = 0;
}

size_t ListCtrlDataProvider::GetColumnId(size_t nColumnIndex) const
{
    ListCtrlHeader* pHeaderCtrl = nullptr;
    if (m_pListCtrl != nullptr) {
        pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    }
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    size_t columnId = pHeaderCtrl->GetColumnId(nColumnIndex);
    return columnId;
}

size_t ListCtrlDataProvider::GetColumnIndex(size_t nColumnId) const
{
    ListCtrlHeader* pHeaderCtrl = nullptr;
    if (m_pListCtrl != nullptr) {
        pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    }
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    size_t columnIndex = pHeaderCtrl->GetColumnIndex(nColumnId);
    return columnIndex;
}

bool ListCtrlDataProvider::IsValidDataItemIndex(size_t itemIndex) const
{
    if (itemIndex == Box::InvalidIndex) {
        return false;
    }
    if (m_dataMap.empty()) {
        return false;
    }
    bool bValidItemIndex = true;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        const StoragePtrList& storageList = iter->second;
        if (itemIndex >= storageList.size()) {
            bValidItemIndex = false;
            break;
        }
    }
    return bValidItemIndex;
}

bool ListCtrlDataProvider::IsValidDataColumnId(size_t nColumnId) const
{
    bool bValidColumnId = false;
    if (nColumnId == Box::InvalidIndex) {
        return false;
    }
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        if (id == nColumnId) {
            bValidColumnId = true;
            break;
        }
    }
    return bValidColumnId;
}

bool ListCtrlDataProvider::AddColumn(size_t columnId)
{
    ASSERT((columnId != Box::InvalidIndex) && (columnId != 0));
    if ((columnId == Box::InvalidIndex) || (columnId == 0)) {
        return false;
    }
    std::vector<std::shared_ptr<Storage>>& storageList = m_dataMap[columnId];
    auto iter = m_dataMap.find(0);
    if (iter != m_dataMap.end()) {
        //��������������ͬ
        storageList.resize(iter->second.size());
    }
    else {
        m_dataMap[0].resize(storageList.size());
    }
    EmitCountChanged();
    return true;
}

bool ListCtrlDataProvider::RemoveColumn(size_t columnId)
{
    ASSERT((columnId != Box::InvalidIndex) && (columnId != 0));
    if ((columnId == Box::InvalidIndex) || (columnId == 0)) {
        return false;
    }
    auto iter = m_dataMap.find(columnId);
    if (iter != m_dataMap.end()) {
        m_dataMap.erase(iter);
        if (m_dataMap.size() == 1) {
            iter = m_dataMap.find(0);
            if (iter != m_dataMap.end()) {
                m_dataMap.erase(iter);
            }
        }
        EmitCountChanged();
        return true;
    }
    return false;
}

bool ListCtrlDataProvider::SetColumnCheck(size_t columnId, bool bChecked)
{
    ASSERT((columnId != Box::InvalidIndex) && (columnId != 0));
    if ((columnId == Box::InvalidIndex) || (columnId == 0)) {
        return false;
    }
    auto iter = m_dataMap.find(columnId);
    if (iter != m_dataMap.end()) {
        StoragePtrList& storageList = iter->second;
        for (StoragePtr pStorage : storageList) {
            if (pStorage != nullptr) {
                pStorage->bChecked = bChecked;
            }
        }
    }
    return true;
}

std::shared_ptr<ListCtrlDataProvider::Storage> ListCtrlDataProvider::GetDataItemStorage(
    size_t itemIndex, size_t columnIndex) const
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //��������Ч
        return nullptr;
    }
    size_t columnId = GetColumnId(columnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return nullptr;
    }
    StoragePtr pStorage;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        if (id == columnId) {
            const StoragePtrList& storageList = iter->second;
            if (itemIndex < storageList.size()) {
                //�����У���ȡ����
                pStorage = storageList.at(itemIndex);
            }
            break;
        }
    }
    return pStorage;
}

std::shared_ptr<ListCtrlDataProvider::Storage> ListCtrlDataProvider::GetDataItemStorageForWrite(
    size_t itemIndex, size_t columnIndex)
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //��������Ч
        return nullptr;
    }
    size_t columnId = GetColumnId(columnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return nullptr;
    }

    StoragePtr pStorage;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        if (id == columnId) {
            StoragePtrList& storageList = iter->second;
            if (itemIndex < storageList.size()) {
                //�����У���ȡ�ɸ�������
                pStorage = storageList.at(itemIndex);
                if (pStorage == nullptr) {
                    pStorage = std::make_shared<Storage>();
                    storageList[itemIndex] = pStorage;
                }
            }
            break;
        }
    }
    return pStorage;
}

bool ListCtrlDataProvider::GetDataItemStorageList(size_t nDataItemIndex, std::vector<size_t>& columnIdList,
                                          std::vector<std::shared_ptr<Storage>>& storageList) const
{
    storageList.clear();
    ASSERT(nDataItemIndex != Box::InvalidIndex);
    if (nDataItemIndex == Box::InvalidIndex) {
        return false;
    }
    for (size_t nColumnId : columnIdList) {
        auto iter = m_dataMap.find(nColumnId);
        StoragePtr pStorage;
        if (iter != m_dataMap.end()) {
            const std::vector<std::shared_ptr<Storage>>& dataList = iter->second;
            if (nDataItemIndex < dataList.size()) {
                pStorage = dataList.at(nDataItemIndex);
            }
        }
        storageList.push_back(pStorage);
    }
    return storageList.size() == columnIdList.size();
}

void ListCtrlDataProvider::OnDataItemChecked(size_t itemIndex, size_t nColumnId, bool bChecked)
{
    auto iter = m_dataMap.find(nColumnId);
    StoragePtr pStorage;
    if (iter != m_dataMap.end()) {
        const std::vector<std::shared_ptr<Storage>>& dataList = iter->second;
        if (itemIndex < dataList.size()) {
            pStorage = dataList.at(itemIndex);
        }
    }
    if (pStorage != nullptr) {
        //���´洢״̬
        pStorage->bChecked = bChecked;
    }

    //����header�Ĺ�ѡ״̬����̬��������ͬ��UI��״̬
    UpdateControlCheckStatus(nColumnId);
}

void ListCtrlDataProvider::UpdateControlCheckStatus(size_t nColumnId)
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->UpdateControlCheckStatus(nColumnId);
    }
}

size_t ListCtrlDataProvider::GetDataItemCount() const
{
    size_t nDataCount = 0;
    if (!m_dataMap.empty()) {        
        auto iter = m_dataMap.begin();
        for (; iter != m_dataMap.end(); ++iter) {
            if (!iter->second.empty()) {
                nDataCount = std::max(nDataCount, iter->second.size());
            }
        }
    }
    return nDataCount;
}

bool ListCtrlDataProvider::SetDataItemCount(size_t itemCount)
{
    ASSERT(itemCount != Box::InvalidIndex);
    if (itemCount == Box::InvalidIndex) {
        return false;
    }
    if (m_dataMap.empty()) {
        m_dataMap[0].resize(itemCount);
    }
    else {
        m_dataMap[0].resize(itemCount);
        auto iter = m_dataMap.begin();
        for (; iter != m_dataMap.end(); ++iter) {
            iter->second.resize(itemCount);
        }
    }
    EmitCountChanged();
    return true;
}

size_t ListCtrlDataProvider::AddDataItem(const ListCtrlDataItem& dataItem)
{
    size_t columnId = GetColumnId(dataItem.nColumnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return Box::InvalidIndex;
    }
    ASSERT(!m_dataMap.empty());
    ASSERT(m_dataMap.find(0) != m_dataMap.end());
    ASSERT(m_dataMap.find(columnId) != m_dataMap.end());

    Storage storage;
    DataItemToStorage(storage, dataItem);

    size_t nDataItemIndex = Box::InvalidIndex;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        StoragePtrList& storageList = iter->second;
        if ((id == 0) || (id == columnId)) {
            //�����У���������
            storageList.push_back(std::make_shared<Storage>(storage));
            if (id == columnId) {
                nDataItemIndex = storageList.size();
            }
        }
        else {
            //�����У����������
            storageList.push_back(nullptr);
        }
    }

    EmitCountChanged();
    return nDataItemIndex;
}

bool ListCtrlDataProvider::InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    size_t columnId = GetColumnId(dataItem.nColumnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return false;
    }

    if (!IsValidDataItemIndex(itemIndex)) {
        //�����������Ч����׷�ӵķ�ʽ�������
        return (AddDataItem(dataItem) != Box::InvalidIndex);
    }

    Storage storage;
    DataItemToStorage(storage, dataItem);

    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        StoragePtrList& storageList = iter->second;
        if ((id == 0) || (id == columnId)) {
            //�����У���������
            storageList.insert(storageList.begin() + itemIndex, std::make_shared<Storage>(storage));
        }
        else {
            //�����У����������
            storageList.insert(storageList.begin() + itemIndex, nullptr);
        }
    }

    EmitCountChanged();
    return true;
}

bool ListCtrlDataProvider::SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    size_t elementIndex = DataItemIndexToElementIndex(itemIndex);
    ASSERT(elementIndex != Box::InvalidIndex);
    if (elementIndex == Box::InvalidIndex) {
        return false;
    }
    size_t columnId = GetColumnId(dataItem.nColumnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return false;
    }

    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //��������Ч
        return false;
    }

    Storage storage;
    DataItemToStorage(storage, dataItem);

    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;        
        if (id == columnId) {
            StoragePtrList& storageList = iter->second;
            //�����У���������
            StoragePtr pStorage = storageList[itemIndex];
            if (pStorage == nullptr) {
                storageList[itemIndex] = std::make_shared<Storage>(storage);
            }
            else {
                *pStorage = storage;
            }
            break;
        }
    }

    EmitDataChanged(elementIndex, elementIndex);
    return true;
}

bool ListCtrlDataProvider::DeleteDataItem(size_t itemIndex)
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //��������Ч
        return false;
    }

    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        StoragePtrList& storageList = iter->second;
        if (itemIndex < storageList.size()) {
            storageList.erase(storageList.begin() + itemIndex);
        }
    }

    EmitCountChanged();
    return true;
}

bool ListCtrlDataProvider::SetDataItemData(size_t itemIndex, size_t itemData)
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //��������Ч
        return false;
    }

    StoragePtrList& storageList = m_dataMap[0];
    if (itemIndex < storageList.size()) {
        StoragePtr pStorage = storageList[itemIndex];
        if (pStorage == nullptr) {
            pStorage = std::make_shared<Storage>();
            storageList[itemIndex] = pStorage;
        }
        pStorage->nItemData = itemData;
        return true;
    }
    return false;
}

size_t ListCtrlDataProvider::GetDataItemData(size_t itemIndex) const
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //��������Ч
        return 0;
    }
    size_t nItemData = 0;
    auto iter = m_dataMap.find(0);
    if (iter != m_dataMap.end()) {
        const StoragePtrList& storageList = iter->second;
        if (itemIndex < storageList.size()) {
            StoragePtr pStorage = storageList[itemIndex];
            if (pStorage != nullptr) {
                nItemData = pStorage->nItemData;
            }
        }
    }
    return nItemData;
}

bool ListCtrlDataProvider::SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text)
{
    StoragePtr pStorage = GetDataItemStorageForWrite(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //��������Ч
        return false;
    }
    size_t elementIndex = DataItemIndexToElementIndex(itemIndex);
    if (elementIndex == Box::InvalidIndex) {
        return false;
    }
    pStorage->text = text;
    EmitDataChanged(elementIndex, elementIndex);
    return true;
}

std::wstring ListCtrlDataProvider::GetDataItemText(size_t itemIndex, size_t columnIndex) const
{
    StoragePtr pStorage = GetDataItemStorage(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //��������Ч
        return std::wstring();
    }
    return pStorage->text.c_str();
}

bool ListCtrlDataProvider::SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor)
{
    StoragePtr pStorage = GetDataItemStorageForWrite(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //��������Ч
        return false;
    }
    size_t elementIndex = DataItemIndexToElementIndex(itemIndex);
    if (elementIndex == Box::InvalidIndex) {
        return false;
    }
    pStorage->textColor = textColor;
    EmitDataChanged(elementIndex, elementIndex);
    return true;
}

bool ListCtrlDataProvider::GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const
{
    StoragePtr pStorage = GetDataItemStorage(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //��������Ч
        return false;
    }
    textColor = pStorage->textColor;
    return true;
}

bool ListCtrlDataProvider::SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor)
{
    StoragePtr pStorage = GetDataItemStorageForWrite(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //��������Ч
        return false;
    }
    size_t elementIndex = DataItemIndexToElementIndex(itemIndex);
    if (elementIndex == Box::InvalidIndex) {
        return false;
    }
    pStorage->bkColor = bkColor;
    EmitDataChanged(elementIndex, elementIndex);
    return true;
}

bool ListCtrlDataProvider::GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const
{
    StoragePtr pStorage = GetDataItemStorage(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //��������Ч
        return false;
    }
    bkColor = pStorage->bkColor;
    return true;
}

bool ListCtrlDataProvider::SortColumnData(size_t nColumnId, bool bSortedUp)
{
    StorageMap::iterator iter = m_dataMap.find(nColumnId);
    if (iter == m_dataMap.end()) {
        return false;
    }
    StoragePtrList sortStorageList = iter->second;
    if (sortStorageList.empty()) {
        return false;
    }
    std::vector<StorageData> sortedDataList;
    const size_t dataCount = sortStorageList.size();
    for (size_t index = 0; index < dataCount; ++index) {
        sortedDataList.push_back({index, sortStorageList[index] });
    }    
    SortStorageData(sortedDataList, bSortedUp);

    //��ԭ���ݽ���˳�����
    const size_t sortedDataCount = sortedDataList.size();
    StoragePtrList orgStorageList; //��������
    for (iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        orgStorageList = iter->second; 
        StoragePtrList& storageList = iter->second;   //�޸�Ŀ��
        ASSERT(storageList.size() == sortedDataList.size());
        if (storageList.size() != sortedDataList.size()) {
            return false;
        }
        for (size_t index = 0; index < sortedDataCount; ++index) {
            const StorageData& sortedData = sortedDataList[index];
            storageList[index] = orgStorageList[sortedData.index]; //��ֵԭ����
        }
    }
    return true;
}

bool ListCtrlDataProvider::SortStorageData(std::vector<StorageData>& dataList, bool bSortedUp)
{
    if (dataList.empty()) {
        return false;
    }
    //��������
    std::sort(dataList.begin(), dataList.end(), SortStorageFunction);
    if (!bSortedUp) {
        //����
        std::reverse(dataList.begin(), dataList.end());
    }
    return true;
}

bool ListCtrlDataProvider::SortStorageFunction(const StorageData& a, const StorageData& b)
{
    //ʵ��(a < b)�ıȽ��߼�
    if (b.pStorage == nullptr) {
        return false;
    }
    if (a.pStorage == nullptr) {
        return true;
    }
    const Storage& storageA = *a.pStorage;
    const Storage& storageB = *b.pStorage;
    return std::wstring(storageA.text.c_str()) < std::wstring(storageB.text.c_str());
}

/** �б�������ʾ�ؼ�
*/
class ListCtrlDataView : public VirtualListBox
{
    friend class ListCtrlDataLayout;
public:
    ListCtrlDataView();
    virtual ~ListCtrlDataView();

    /** ����ListCtrl�ؼ��ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

protected:
    /** ִ����ˢ�²���, �����UI�ؼ��������ܻᷢ���仯
    */
    virtual void OnRefresh() override;

    /** ִ�������Ų����������UI�ؼ�����������������䣨ͨ��FillElement������
    */
    virtual void OnArrangeChild() override;

private:
    /** ListCtrl �ؼ��ӿ�
    */
    ListCtrl* m_pListCtrl;
};

/** �б�������ʾ�ؼ��Ĳ��ֹ���ӿ�
*/
class ListCtrlDataLayout: public VirtualVTileLayout
{
public:
};

/** �б�������ʾ�ؼ�
*/
ListCtrlDataView::ListCtrlDataView() :
    VirtualListBox(new ListCtrlDataLayout),
    m_pListCtrl(nullptr)
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

void ListCtrlDataView::OnRefresh()
{
}

void ListCtrlDataView::OnArrangeChild()
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->UpdateControlCheckStatus(Box::InvalidIndex);
    }
}

ListCtrl::ListCtrl():
    m_bInited(false),
    m_pHeaderCtrl(nullptr),
    m_pDataView(nullptr),
    m_bEnableHeaderDragOrder(true),
    m_bCanUpdateHeaderCheckStatus(true)
{
    m_pDataProvider = new ListCtrlDataProvider;
    m_nRowGridLineWidth = ui::GlobalManager::Instance().Dpi().GetScaleInt(1);
    m_nColumnGridLineWidth = ui::GlobalManager::Instance().Dpi().GetScaleInt(1);
}

ListCtrl::~ListCtrl()
{
    if (m_pDataProvider != nullptr) {
        delete m_pDataProvider;
        m_pDataProvider = nullptr;
    }
}

std::wstring ListCtrl::GetType() const { return DUI_CTR_LISTCTRL; }

void ListCtrl::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"header_class") {
        SetHeaderClass(strValue);
    }
    else if (strName == L"header_item_class") {
        SetHeaderItemClass(strValue);
    }
    else if (strName == L"header_split_box_class") {
        SetHeaderSplitBoxClass(strValue);
    }
    else if (strName == L"header_split_control_class") {
        SetHeaderSplitControlClass(strValue);
    }
    else if (strName == L"enable_header_drag_order") {
        SetEnableHeaderDragOrder(strValue == L"true");
    }
    else if (strName == L"check_box_class") {
        SetCheckBoxClass(strValue);
    }
    else if (strName == L"data_item_class") {
        SetDataItemClass(strValue);
    }
    else if (strName == L"data_item_label_class") {
        SetDataItemLabelClass(strValue);
    }
    else if (strName == L"row_grid_line_width") {
        SetRowGridLineWidth(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"column_grid_line_width") {
        SetColumnGridLineWidth(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"data_view_class") {
        SetDataViewClass(strValue);
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrl::SetHeaderClass(const std::wstring& className)
{
    m_headerClass = className;
    if (m_pHeaderCtrl != nullptr) {
        m_pHeaderCtrl->SetClass(className);
    }
}

void ListCtrl::SetHeaderItemClass(const std::wstring& className)
{
    m_headerItemClass = className;
}

std::wstring ListCtrl::GetHeaderItemClass() const
{
    return m_headerItemClass.c_str();
}

void ListCtrl::SetHeaderSplitBoxClass(const std::wstring& className)
{
    m_headerSplitBoxClass = className;
}

std::wstring ListCtrl::GetHeaderSplitBoxClass() const
{
    return m_headerSplitBoxClass.c_str();
}

void ListCtrl::SetHeaderSplitControlClass(const std::wstring& className)
{
    m_headerSplitControlClass = className;
}

std::wstring ListCtrl::GetHeaderSplitControlClass() const
{
    return m_headerSplitControlClass.c_str();
}

void ListCtrl::SetCheckBoxClass(const std::wstring& className)
{
    m_checkBoxClass = className;
}

std::wstring ListCtrl::GetCheckBoxClass() const
{
    return m_checkBoxClass.c_str();
}

void ListCtrl::SetDataViewClass(const std::wstring& className)
{
    m_dataViewClass = className;
}

std::wstring ListCtrl::GetDataViewClass() const
{
    return m_dataViewClass.c_str();
}

void ListCtrl::SetDataItemClass(const std::wstring& className)
{
    m_dataItemClass = className;
}

std::wstring ListCtrl::GetDataItemClass() const
{
    return m_dataItemClass.c_str();
}

void ListCtrl::SetDataItemLabelClass(const std::wstring& className)
{
    m_dataItemLabelClass = className;
}

std::wstring ListCtrl::GetDataItemLabelClass() const
{
    return m_dataItemLabelClass.c_str();
}

void ListCtrl::SetRowGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nLineWidth);
    }
    if (nLineWidth < 0) {
        nLineWidth = 0;
    }
    m_nRowGridLineWidth = nLineWidth;
}

int32_t ListCtrl::GetRowGridLineWidth() const
{
    return m_nRowGridLineWidth;
}

void ListCtrl::SetColumnGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nLineWidth);
    }
    if (nLineWidth < 0) {
        nLineWidth = 0;
    }
    m_nColumnGridLineWidth = nLineWidth;
}

int32_t ListCtrl::GetColumnGridLineWidth() const
{
    return m_nColumnGridLineWidth;
}

void ListCtrl::DoInit()
{
    if (m_bInited) {
        return;
    }
    m_bInited = true;

    //��ʼ��Header
    ASSERT(m_pHeaderCtrl == nullptr);
    if (m_pHeaderCtrl == nullptr) {
        m_pHeaderCtrl = new ListCtrlHeader;
    }
    m_pHeaderCtrl->SetListCtrl(this);

    if (!m_headerClass.empty()) {
        m_pHeaderCtrl->SetClass(m_headerClass.c_str());
    }
    m_pDataProvider->SetListCtrl(this);

    //��ʼ��Body
    ASSERT(m_pDataView == nullptr);
    m_pDataView = new ListCtrlDataView;
    m_pDataView->SetListCtrl(this);
    m_pDataView->SetDataProvider(m_pDataProvider);
    if (!m_dataViewClass.empty()) {
        m_pDataView->SetClass(m_dataViewClass.c_str());
    }
    m_pDataView->AddItem(m_pHeaderCtrl);
    AddItem(m_pDataView);

    //TEST
    const size_t columnCount = 5;
    const size_t rowCount = 20;
    //�����
    for (size_t i = 0; i < columnCount; ++i) {
        ListCtrlColumn columnInfo;
        columnInfo.nColumnWidth = 200;
        //columnInfo.nTextFormat = TEXT_LEFT | TEXT_VCENTER;
        columnInfo.text = StringHelper::Printf(L"�� %d ��", i);
        m_pHeaderCtrl->InsertColumn(-1, columnInfo);
    }
    //�������
    SetDataItemCount(rowCount);
    for (size_t itemIndex = 0; itemIndex < rowCount; ++itemIndex) {
        for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
            SetDataItem(itemIndex, { columnIndex, StringHelper::Printf(L"�� %02d ��/�� %02d ��", itemIndex, columnIndex), });
        }
    }
    //TESTs
}

ListCtrlHeaderItem* ListCtrl::InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->InsertColumn(columnIndex, columnInfo);
    }
}

size_t ListCtrl::GetColumnCount() const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return 0;
    }
    else {
        return m_pHeaderCtrl->GetColumnCount();
    }
}

int32_t ListCtrl::GetColumnWidth(size_t columnIndex) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return 0;
    }
    else {
        return m_pHeaderCtrl->GetColumnWidth(columnIndex);
    }
}

ListCtrlHeaderItem* ListCtrl::GetColumn(size_t columnIndex) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->GetColumn(columnIndex);
    }
}

ListCtrlHeaderItem* ListCtrl::GetColumnById(size_t columnId) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->GetColumnById(columnId);
    }
}

size_t ListCtrl::GetColumnIndex(size_t columnId) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    else {
        return m_pHeaderCtrl->GetColumnIndex(columnId);
    }
}

bool ListCtrl::DeleteColumn(size_t columnIndex)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return false;
    }
    else {
        return m_pHeaderCtrl->DeleteColumn(columnIndex);
    }
}

ListCtrlHeader* ListCtrl::GetListCtrlHeader() const
{
    return m_pHeaderCtrl;
}

void ListCtrl::SetEnableHeaderDragOrder(bool bEnable)
{
    m_bEnableHeaderDragOrder = bEnable;
}

bool ListCtrl::IsEnableHeaderDragOrder() const
{
    return m_bEnableHeaderDragOrder;
}

void ListCtrl::OnColumnWidthChanged(size_t nColumnId1, size_t nColumnId2)
{
    if ((m_pDataView == nullptr) || (m_pHeaderCtrl == nullptr)){
        return;
    }

    size_t nColumn1 = Box::InvalidIndex;
    size_t nColumn2 = Box::InvalidIndex;
    int32_t nColumnWidth1 = -1;
    int32_t nColumnWidth2 = -1;
    if (!m_pHeaderCtrl->GetColumnInfo(nColumnId1, nColumn1, nColumnWidth1)) {
        nColumnWidth1 = -1;
    }
    if (!m_pHeaderCtrl->GetColumnInfo(nColumnId2, nColumn2, nColumnWidth2)) {
        nColumnWidth2 = -1;
    }

    if ((nColumnWidth1 < 0) && (nColumnWidth2 < 0)) {
        return;
    }

    size_t itemCount = m_pDataView->GetItemCount();
    for (size_t index = 1; index < itemCount; ++index) {
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(m_pDataView->GetItemAt(index));
        if (pItem == nullptr) {
            continue;
        }
        size_t columnCount = pItem->GetItemCount();
        for (size_t nColumn = 0; nColumn < columnCount; ++nColumn) {
            if ((nColumn == nColumn1) && (nColumnWidth1 >= 0)) {
                LabelBox* pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(nColumn));
                if (pLabelBox != nullptr) {
                    pLabelBox->SetFixedWidth(UiFixedInt(nColumnWidth1), true, false);
                }
            }
            if ((nColumn == nColumn2) && (nColumnWidth2 >= 0)) {
                LabelBox* pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(nColumn));
                if (pLabelBox != nullptr) {
                    pLabelBox->SetFixedWidth(UiFixedInt(nColumnWidth2), true, false);
                }
            }
        }
    }
}

void ListCtrl::OnHeaderColumnAdded(size_t nColumnId)
{
    m_pDataProvider->AddColumn(nColumnId);
}

void ListCtrl::OnHeaderColumnRemoved(size_t nColumnId)
{
    m_pDataProvider->RemoveColumn(nColumnId);
}

void ListCtrl::OnColumnSorted(size_t nColumnId, bool bSortedUp)
{
    //����������Ȼ��ˢ�½�����ʾ
    m_pDataProvider->SortColumnData(nColumnId, bSortedUp);

    ASSERT(m_pDataView != nullptr);
    if (m_pDataView != nullptr) {
        m_pDataView->Refresh();
    }
}

void ListCtrl::OnHeaderColumnOrderChanged()
{
    ASSERT(m_pDataView != nullptr);
    if (m_pDataView != nullptr) {
        m_pDataView->Refresh();
    }
}

void ListCtrl::OnHeaderColumnCheckStateChanged(size_t nColumnId, bool bChecked)
{
    m_bCanUpdateHeaderCheckStatus = false;
    m_pDataProvider->SetColumnCheck(nColumnId, bChecked);
    ASSERT(m_pDataView != nullptr);
    if (m_pDataView != nullptr) {
        m_pDataView->Refresh();
    }
    m_bCanUpdateHeaderCheckStatus = true;
}

void ListCtrl::UpdateControlCheckStatus(size_t nColumnId)
{
    if (!m_bCanUpdateHeaderCheckStatus) {
        //���ⲻ��Ҫ�ĸ���
        return;
    }
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return;
    }
    ASSERT(m_pDataView != nullptr);
    if (m_pDataView == nullptr) {
        return;
    }
    std::vector<size_t> columnIndexList;
    if (nColumnId == Box::InvalidIndex) {
        const size_t columnCount = GetColumnCount();
        for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())) {
                columnIndexList.push_back(columnIndex);
            }
        }
    }
    else {
        size_t columnIndex = GetColumnIndex(nColumnId);
        if (columnIndex != Box::InvalidIndex) {
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())){
                columnIndexList.push_back(columnIndex);
            }
        }
    }
    for (size_t columnIndex : columnIndexList) {        
        std::vector<bool> checkList;
        const size_t itemCount = m_pDataView->GetItemCount();
        for (size_t itemIndex = 1; itemIndex < itemCount; ++itemIndex) {
            ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(m_pDataView->GetItemAt(itemIndex));
            if (pItem != nullptr) {
                const size_t columnCount = pItem->GetItemCount();
                LabelBox* pLabelBox = nullptr;
                if (columnIndex < columnCount) {
                    pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(columnIndex));
                }
                CheckBox* pCheckBox = nullptr;
                if (pLabelBox != nullptr) {
                    pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                }
                if (pCheckBox != nullptr) {
                    checkList.push_back(pCheckBox->IsSelected());
                }
            }
        }

        if (!checkList.empty()) {
            bool bHasChecked = false;
            bool bHasUnChecked = false;
            for (bool bChecked : checkList) {
                if (bChecked) {
                    bHasChecked = true;
                }
                else {
                    bHasUnChecked = true;
                }
            }

            bool bSelected = bHasChecked;
            bool bPartSelect = bSelected && bHasUnChecked;
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())) {
                pHeaderItem->SetCheckBoxSelect(bSelected, bPartSelect);
            }
        }
    }
}

size_t ListCtrl::GetDataItemCount() const
{
    return m_pDataProvider->GetDataItemCount();
}

bool ListCtrl::SetDataItemCount(size_t itemCount)
{
    return m_pDataProvider->SetDataItemCount(itemCount);
}

size_t ListCtrl::AddDataItem(const ListCtrlDataItem& dataItem)
{
    return m_pDataProvider->AddDataItem(dataItem);
}

bool ListCtrl::InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    return m_pDataProvider->InsertDataItem(itemIndex, dataItem);
}

bool ListCtrl::SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    return m_pDataProvider->SetDataItem(itemIndex, dataItem);
}

bool ListCtrl::DeleteDataItem(size_t itemIndex)
{
    return m_pDataProvider->DeleteDataItem(itemIndex);
}

bool ListCtrl::SetDataItemData(size_t itemIndex, size_t itemData)
{
    return m_pDataProvider->SetDataItemData(itemIndex, itemData);
}

size_t ListCtrl::GetDataItemData(size_t itemIndex) const
{
    return m_pDataProvider->GetDataItemData(itemIndex);
}

bool ListCtrl::SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text)
{
    return m_pDataProvider->SetDataItemText(itemIndex, columnIndex, text);
}

std::wstring ListCtrl::GetDataItemText(size_t itemIndex, size_t columnIndex) const
{
    return m_pDataProvider->GetDataItemText(itemIndex, columnIndex);
}

bool ListCtrl::SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor)
{
    return m_pDataProvider->SetDataItemTextColor(itemIndex, columnIndex, textColor);
}

bool ListCtrl::GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const
{
    return m_pDataProvider->GetDataItemTextColor(itemIndex, columnIndex, textColor);
}

bool ListCtrl::SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor)
{
    return m_pDataProvider->SetDataItemBkColor(itemIndex, columnIndex, bkColor);
}

bool ListCtrl::GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const
{
    return m_pDataProvider->GetDataItemBkColor(itemIndex, columnIndex, bkColor);
}

}//namespace ui

