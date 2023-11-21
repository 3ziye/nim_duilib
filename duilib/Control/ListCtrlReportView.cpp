#include "ListCtrlReportView.h" 
#include "ListCtrl.h"
#include "duilib/Render/AutoClip.h"

//�����ࣺListCtrlReportView / ListCtrlReportLayout

namespace ui
{
ListCtrlReportView::ListCtrlReportView() :
    VirtualListBox(new ListCtrlReportLayout),
    m_pListCtrl(nullptr),
    m_nTopElementIndex(0),
    m_bMouseDown(false),
    m_bRMouseDown(false),
    m_bInMouseMove(false),
    m_pMouseSender(nullptr),
    m_nNormalItemTop(-1),
    m_bEnableFrameSelection(true),
    m_nLastNoShiftIndex(0),
    m_bMouseDownInView(false)
{
    ListCtrlReportLayout* pDataLayout = dynamic_cast<ListCtrlReportLayout*>(GetLayout());
    ASSERT(pDataLayout != nullptr);

    VirtualLayout* pVirtualLayout = pDataLayout;
    ASSERT(pVirtualLayout != nullptr);
    SetVirtualLayout(pVirtualLayout);
    
    if (pDataLayout != nullptr) {
        pDataLayout->SetDataView(this);
    }
    m_frameSelectionBorderSize = (uint8_t)GlobalManager::Instance().Dpi().GetScaleInt(1);
    m_frameSelectionAlpha = 128;

    m_nRowGridLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);
    m_nColumnGridLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);
}

ListCtrlReportView::~ListCtrlReportView() 
{
}

void ListCtrlReportView::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"enable_frame_selection") {
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

void ListCtrlReportView::DoInit()
{
    //��ֹ�������ֵĹ����ı�ѡ����
    SetScrollSelect(false);
}

void ListCtrlReportView::HandleEvent(const EventArgs& msg)
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

bool ListCtrlReportView::OnListCtrlKeyDown(const EventArgs& msg)
{
    ASSERT(msg.Type == kEventKeyDown);
    bool bHandled = false;
    bool bCtrlADown = (msg.Type == kEventKeyDown) && ((msg.chKey == L'A') || (msg.chKey == L'a'));
    if (bCtrlADown) {
        //Ctrl + A ȫѡ����
        bHandled = true;
        bool bRet = SetSelectAll();
        if (bRet) {
            OnSelectStatusChanged();
            SendEvent(kEventSelChange);
        }        
        return bHandled;
    }

    //���������
    bool bArrowKeyDown = (msg.Type == kEventKeyDown) &&
                          ((msg.chKey == VK_UP) || (msg.chKey == VK_DOWN) ||
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
    const bool bForward = (msg.chKey == VK_DOWN) || (msg.chKey == VK_NEXT) || (msg.chKey == VK_HOME);
    if (nIndexCurSel < nElementCount) {
        //ƥ���ѡ����
        nIndexCurSel = FindSelectableElement(nIndexCurSel, bForward);
    }
    if (nIndexCurSel >= nElementCount) {
        //û����Ч������ѡ����
        return bHandled;
    }

    size_t nIndexEnd = Box::InvalidIndex;
    //ʵ��Shift�� + �������ѡ���߼�
    switch (msg.chKey) {
    case VK_UP:
        //���������������1��
        if (nIndexCurSel > 0) {
            nIndexEnd = nIndexCurSel - 1;
        }
        else {
            nIndexEnd = 0;
        }
        break;
    case VK_DOWN:
        //�����������������1��
        nIndexEnd = nIndexCurSel + 1;
        if (nIndexEnd >= nElementCount) {
            nIndexEnd = nElementCount - 1;
        }
        break;
    case VK_PRIOR:
    {
        size_t nColumns = 0;
        size_t nRows = 0;
        GetDisplayItemCount(false, nColumns, nRows);
        if (nRows > 2) {
            nRows -= 1;
        }
        if (nIndexCurSel > nRows) {
            nIndexEnd = nIndexCurSel - nRows;
        }
        else {
            nIndexEnd = 0;
        }
    }
    break;
    case VK_NEXT:
    {
        size_t nColumns = 0;
        size_t nRows = 0;
        GetDisplayItemCount(false, nColumns, nRows);
        if (nRows > 2) {
            nRows -= 1;
        }
        nIndexEnd = nIndexCurSel + nRows;
        if (nIndexEnd >= nElementCount) {
            nIndexEnd = nElementCount - 1;
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
    EnsureVisible(nIndexEnd, false);
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
    OnSelectStatusChanged();
    if (bTriggerEvent) {
        SendEvent(kEventSelect, nCurSel, Box::InvalidIndex);
    }
    return bHandled;
}

bool ListCtrlReportView::OnListCtrlClickedBlank()
{
    //�ڿհ״���������������Ҽ���ȡ��ȫ��ѡ��
    bool bRet = SetSelectNone();
    OnSelectStatusChanged();
    return bRet;
}

bool ListCtrlReportView::SelectItem(size_t iIndex, bool bTakeFocus, bool bTriggerEvent, uint64_t vkFlag)
{
    //�¼���������Ҫ���ں�������֮ǰ�����ܷ��ڴ����м�
    bool bSelectStatusChanged = false;
    bool bRet = false;
    if (IsMultiSelect()) {
        //��ѡģʽ
        const size_t nCurElementIndex = GetDisplayItemElementIndex(iIndex);
        if ((nCurElementIndex >= GetElementCount()) || !IsSelectableElement(nCurElementIndex)){
            //����Чѡ�񣬻���ѡ�����ö������������ѡ����
            bRet = SelectItemMulti(iIndex, bTakeFocus, bTriggerEvent);
            return bRet;
        }

        bool bRbuttonDown = vkFlag & kVkRButton;
        bool bShift = vkFlag & kVkShift;
        bool bControl = vkFlag & kVkControl;
        if (bShift && bControl) {
            //ͬʱ����Shift��Ctrl��������
            bShift = false;
            bControl = false;
        }
        if (bRbuttonDown || (!bShift && !bControl)) {
            //���Ҽ���ʱ�������ǰ��ûѡ�񣬰���ѡ�߼�ʵ�֣�ֻ����һ��ѡ�
            //            ����Ѿ�ѡ���򱣳�ԭѡ��������ѡ��״̬���䣨���ṩ�Ҽ��˵�������ѡ������Ļ��ᣩ
            //��û�а���Control��Ҳû�а�Shift��������ѡ�߼�ʵ�֣�ֻ����һ��ѡ��            
            size_t nElementIndex = GetDisplayItemElementIndex(iIndex);
            if (bRbuttonDown && IsElementSelected(nElementIndex)) {
                bRet = true;
            }
            else {
                std::vector<size_t> refreshDataIndexs;
                m_nLastNoShiftIndex = nElementIndex;
                if (nElementIndex == Box::InvalidIndex) {
                    SetSelectNone(refreshDataIndexs);
                }
                else {
                    std::vector<size_t> excludeIndexs;
                    excludeIndexs.push_back(nElementIndex);
                    SetSelectNoneExclude(excludeIndexs, refreshDataIndexs);
                }
                SetCurSel(iIndex);
                bRet = SelectItemSingle(iIndex, bTakeFocus, false);
                RefreshElements(refreshDataIndexs);
                ASSERT(IsElementSelected(nElementIndex));
                ASSERT(nElementIndex == GetDisplayItemElementIndex(iIndex));
                bSelectStatusChanged = true;
                bRet = true;
            }
        }
        else {            
            if (bShift) {
                //�����: ͬʱ������Shift��
                size_t nIndexStart = m_nLastNoShiftIndex;
                if (nIndexStart >= GetElementCount()) {
                    nIndexStart = 0;
                }
                size_t nElementIndex = GetDisplayItemElementIndex(iIndex);
                if (nElementIndex < GetElementCount()) {
                    std::vector<size_t> selectedIndexs;
                    size_t iStart = std::min(nIndexStart, nElementIndex);
                    size_t iEnd = std::max(nIndexStart, nElementIndex);
                    for (size_t i = iStart; i <= iEnd; ++i) {
                        if (IsSelectableElement(i)) {
                            selectedIndexs.push_back(i);
                        }                        
                    }
                    std::vector<size_t> refreshDataIndexs;
                    SetSelectedElements(selectedIndexs, true, refreshDataIndexs);
                    SetCurSel(iIndex);
                    bRet = SelectItemSingle(iIndex, bTakeFocus, false);
                    RefreshElements(refreshDataIndexs);
                    ASSERT(IsElementSelected(nElementIndex));
                    ASSERT(nElementIndex == GetDisplayItemElementIndex(iIndex));
                    bSelectStatusChanged = true;
                    bRet = true;
                }
                else {
                    //δ֪����������޷��ߵ�����
                    bRet = SelectItemMulti(iIndex, bTakeFocus, false);
                }
            }
            else {
                //�����: ͬʱ������Control�������ֶ�ѡ
                bRet = SelectItemMulti(iIndex, bTakeFocus, false);
                if (bRet) {
                    m_nLastNoShiftIndex = GetDisplayItemElementIndex(iIndex);
                }                
            }
        }
    }
    else {
        //��ѡ
        bRet = SelectItemSingle(iIndex, bTakeFocus, false);
    }
    if (bSelectStatusChanged) {
        OnSelectStatusChanged();
    }
    if (bTriggerEvent && bRet) {
        SendEvent(kEventSelect, iIndex, Box::InvalidIndex);
    }
    return bRet;
}

void ListCtrlReportView::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

void ListCtrlReportView::Refresh()
{
    if ((m_pListCtrl != nullptr) && !m_pListCtrl->IsEnableRefresh()) {
        //ˢ�¹����Ѿ���ֹ
        return;
    }
    if ((GetWindow() == nullptr) || !HasDataProvider()) {
        return;
    }
    //����UI�ؼ��ĸ�������������ʾʹ��
    AjustItemCount();

    //ˢ��UI�ؼ�����ʾ����
    if (GetElementCount() > 0) {
        ReArrangeChild(true);
        Arrange();
    }
}

void ListCtrlReportView::AjustItemCount()
{
    VirtualLayout* pVirtualLayout = GetVirtualLayout();
    if (pVirtualLayout == nullptr) {
        return;
    }

    //���������
    size_t nMaxItemCount = pVirtualLayout->AjustMaxItem(GetPosWithoutPadding());
    if (nMaxItemCount == 0) {
        return;
    }

    //��ǰ��������: �������� + Header�ؼ�
    size_t nElementCount = GetElementCount() + 1;

    //��ǰ������
    size_t nItemCount = GetItemCount();

    //ˢ�º��������
    size_t nNewItemCount = nElementCount;
    if (nNewItemCount > nMaxItemCount) {
        nNewItemCount = nMaxItemCount;
    }

    if (nItemCount > nNewItemCount) {
        //��������������������¼�������������Ƴ��������������������Ӻ���ɾ����
        size_t n = nItemCount - nNewItemCount;
        for (size_t i = 0; i < n; ++i) {
            size_t itemCount = GetItemCount();
            if (itemCount > 1) {
                //���ٱ���һ��Header�ؼ�
                RemoveItemAt(itemCount - 1);
            }
        }
    }
    else if (nItemCount < nNewItemCount) {
        //���������������С���¼���������������������������ٵ�����
        size_t n = nNewItemCount - nItemCount;
        for (size_t i = 0; i < n; ++i) {
            Control* pControl = CreateElement();
            ASSERT(pControl != nullptr);
            if (pControl != nullptr) {
                AddItem(pControl);

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
        }
    }
}

int32_t ListCtrlReportView::GetListCtrlWidth() const
{
    int32_t nToltalWidth = 0;
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nToltalWidth;
    }
    ListCtrlHeader* pHeaderCtrl = m_pListCtrl->GetHeaderCtrl();
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
    nToltalWidth += pHeaderCtrl->GetPadding().left;
    return nToltalWidth;
}

void ListCtrlReportView::SetTopElementIndex(size_t nTopElementIndex)
{
    m_nTopElementIndex = nTopElementIndex;
}

size_t ListCtrlReportView::GetTopElementIndex() const
{
    return m_nTopElementIndex;
}

void ListCtrlReportView::SetDisplayDataItems(const std::vector<size_t>& itemIndexList)
{
    m_diplayItemIndexList = itemIndexList;
}

void ListCtrlReportView::GetDisplayDataItems(std::vector<size_t>& itemIndexList) const
{
    itemIndexList = m_diplayItemIndexList;
}

bool ListCtrlReportView::IsDataItemDisplay(size_t itemIndex) const
{
    auto iter = std::find(m_diplayItemIndexList.begin(), m_diplayItemIndexList.end(), itemIndex);
    return iter != m_diplayItemIndexList.end();
}

bool ListCtrlReportView::EnsureDataItemVisible(size_t itemIndex, bool bToTop)
{
    if (!Box::IsValidItemIndex(itemIndex) || (itemIndex >= GetElementCount())) {
        return false;
    }
    VirtualLayout* pVirtualLayout = GetVirtualLayout();
    if (pVirtualLayout != nullptr) {
        pVirtualLayout->EnsureVisible(GetRect(), itemIndex, bToTop);
        return true;
    }
    return false;
}

size_t ListCtrlReportView::GetTopDataItemIndex(int64_t nScrollPosY) const
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return 0;
    }
    size_t itemIndex = 0;
    if (nScrollPosY <= 0) {
        return itemIndex;
    }
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return itemIndex;
    }
    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    int64_t totalItemHeight = 0;
    int32_t nItemHeight = 0;
    const size_t dataItemCount = itemDataList.size();
    for (size_t index = 0; index < dataItemCount; ++index) {
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = rowData.nItemHeight;
        if (nItemHeight < 0) {
            //ȡĬ���и�
            nItemHeight = m_pListCtrl->GetDataItemHeight();
        }
        if (!rowData.bVisible || (nItemHeight  == 0)){
            continue;
        }
        totalItemHeight += nItemHeight;
        //���ÿ�и߶ȶ���ͬ���൱�� nScrollPosY / ItemHeight
        if (totalItemHeight > nScrollPosY) {
            itemIndex = index;
            break;
        }
    }
    return itemIndex;
}

int32_t ListCtrlReportView::GetDataItemHeight(size_t itemIndex) const
{
    int32_t nItemHeight = 0;
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return 0;
    }
    nItemHeight = m_pListCtrl->GetDataItemHeight();
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return 0;
    }
    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    ASSERT(itemIndex < itemDataList.size());
    if (itemIndex < itemDataList.size()) {
        if (itemDataList[itemIndex].nItemHeight >= 0) {
            nItemHeight = itemDataList[itemIndex].nItemHeight;
        }        
    }
    return nItemHeight;
}

void ListCtrlReportView::GetDataItemsToShow(int64_t nScrollPosY, size_t maxCount, 
                                          std::vector<ShowItemInfo>& itemIndexList,
                                          std::vector<ShowItemInfo>& atTopItemIndexList,
                                          int64_t& nPrevItemHeights) const
{
    nPrevItemHeights = 0;
    itemIndexList.clear();
    atTopItemIndexList.clear();
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return;
    }
    ASSERT(nScrollPosY >= 0);
    if (nScrollPosY < 0) {
        return;
    }
    ASSERT(maxCount != 0);
    if (maxCount == 0) {
        return;
    }
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return;
    }
    size_t nTopDataItemIndex = Box::InvalidIndex; //�����ɼ��ĵ�һ��Ԫ�����
    const int32_t nDefaultItemHeight = m_pListCtrl->GetDataItemHeight(); //Ĭ���и�
    //�ö���Ԫ�����
    struct AlwaysAtTopData
    {
        int8_t nAlwaysAtTop;    //�ö����ȼ�
        size_t nItemIndex;      //Ԫ������
        int32_t nItemHeight;    //Ԫ�صĸ߶�
    };
    std::vector<AlwaysAtTopData> alwaysAtTopItemList;
    
    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    int64_t totalItemHeight = 0;
    int32_t nItemHeight = 0;
    const size_t dataItemCount = itemDataList.size();
    for (size_t index = 0; index < dataItemCount; ++index) {
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = (rowData.nItemHeight < 0) ? nDefaultItemHeight : rowData.nItemHeight;
        if (!rowData.bVisible || (nItemHeight == 0)) {
            //���ɼ��ģ�����
            continue;
        }

        if (rowData.nAlwaysAtTop >= 0) {
            //�ö���Ԫ��
            if (alwaysAtTopItemList.size() < maxCount) {
                alwaysAtTopItemList.push_back({ rowData.nAlwaysAtTop, index, nItemHeight });
            }
            continue;
        }

        if (nTopDataItemIndex == Box::InvalidIndex) {
            totalItemHeight += nItemHeight;
            //���ÿ�и߶ȶ���ͬ���൱�� nScrollPosY / ItemHeight
            if (totalItemHeight > nScrollPosY) {
                nTopDataItemIndex = index;
                nPrevItemHeights = totalItemHeight - nItemHeight;
            }
        }

        if ((nTopDataItemIndex != Box::InvalidIndex) && (itemIndexList.size() < maxCount)){
            itemIndexList.push_back({ index, nItemHeight });
        }
    }

    //���ö�������
    if (!alwaysAtTopItemList.empty()) {
        std::stable_sort(alwaysAtTopItemList.begin(), alwaysAtTopItemList.end(),
            [](const AlwaysAtTopData& a, const AlwaysAtTopData& b) {
                //nAlwaysAtTopֵ��ģ�����ǰ��
                return a.nAlwaysAtTop > b.nAlwaysAtTop;
            });
    }
    for (const AlwaysAtTopData& item : alwaysAtTopItemList) {
        atTopItemIndexList.push_back({ item.nItemIndex, item.nItemHeight });
    }
    if (atTopItemIndexList.size() >= maxCount) {
        atTopItemIndexList.resize(maxCount);
        itemIndexList.clear();
    }
    else {
        size_t nLeftCount = maxCount - atTopItemIndexList.size();
        if (itemIndexList.size() > nLeftCount) {
            itemIndexList.resize(nLeftCount);
        }
    }
    ASSERT((itemIndexList.size() + atTopItemIndexList.size()) <= maxCount);
}

int32_t ListCtrlReportView::GetMaxDataItemsToShow(int64_t nScrollPosY, int32_t nRectHeight, 
                                                std::vector<size_t>* pItemIndexList,
                                                std::vector<size_t>* pAtTopItemIndexList) const
{
    if (pItemIndexList) {
        pItemIndexList->clear();
    }
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return 0;
    }
    ASSERT(nScrollPosY >= 0);
    if (nScrollPosY < 0) {
        return 0;
    }
    ASSERT(nRectHeight > 0);
    if (nRectHeight <= 0) {
        return 0;
    }
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return 0;
    }
    size_t nTopDataItemIndex = Box::InvalidIndex; //�����ɼ��ĵ�һ��Ԫ�����
    const int32_t nDefaultItemHeight = m_pListCtrl->GetDataItemHeight(); //Ĭ���и�
    //�ö���Ԫ�����
    struct AlwaysAtTopData
    {
        int8_t nAlwaysAtTop;
        size_t index;
    };
    std::vector<AlwaysAtTopData> alwaysAtTopItemList;
    std::vector<size_t> itemIndexList;

    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    int64_t totalItemHeight = 0;
    int32_t nItemHeight = 0;
    const size_t dataItemCount = itemDataList.size();
    for (size_t index = 0; index < dataItemCount; ++index) {
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = (rowData.nItemHeight < 0) ? nDefaultItemHeight : rowData.nItemHeight;
        if (!rowData.bVisible || (nItemHeight == 0)) {
            //���ɼ��ģ�����
            continue;
        }

        if (rowData.nAlwaysAtTop >= 0) {
            //�ö���Ԫ��
            alwaysAtTopItemList.push_back({ rowData.nAlwaysAtTop, index });
            continue;
        }

        if (nTopDataItemIndex == Box::InvalidIndex) {
            totalItemHeight += nItemHeight;
            //���ÿ�и߶ȶ���ͬ���൱�� nScrollPosY / ItemHeight
            if (totalItemHeight > nScrollPosY) {
                nTopDataItemIndex = index;
            }
        }

        if (nTopDataItemIndex != Box::InvalidIndex) {
            itemIndexList.push_back(index);
        }
    }

    //���ö�������
    if (!alwaysAtTopItemList.empty()) {
        std::stable_sort(alwaysAtTopItemList.begin(), alwaysAtTopItemList.end(),
            [](const AlwaysAtTopData& a, const AlwaysAtTopData& b) {
                //nAlwaysAtTopֵ��ģ�����ǰ��
                return a.nAlwaysAtTop > b.nAlwaysAtTop;
            });
    }
    std::vector<size_t> atTopIndexList;
    std::vector<size_t> tempItemIndexList;
    tempItemIndexList.swap(itemIndexList);
    for (const AlwaysAtTopData& item : alwaysAtTopItemList) {
        itemIndexList.push_back(item.index);
        atTopIndexList.push_back(item.index);
    }
    for (size_t index : tempItemIndexList) {
        itemIndexList.push_back(index);
    }
    int32_t nShowItemCount = 0;
    int64_t nTotalHeight = 0;
    for (size_t index : itemIndexList) {
        if (index >= dataItemCount) {
            continue;
        }
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = rowData.nItemHeight;
        if (nItemHeight < 0) {
            //ȡĬ���и�
            nItemHeight = nDefaultItemHeight;
        }
        nTotalHeight += nItemHeight;
        if (nTotalHeight < nRectHeight) {
            if (pItemIndexList) {
                pItemIndexList->push_back(index);
            }
            if (pAtTopItemIndexList != nullptr) {
                if (std::find(atTopIndexList.begin(),
                              atTopIndexList.end(), index) != atTopIndexList.end()) {
                    pAtTopItemIndexList->push_back(index);
                }
            }
            ++nShowItemCount;
        }
        else {
            nShowItemCount += 2;
            break;
        }
    }
    return nShowItemCount;
}

int64_t ListCtrlReportView::GetDataItemTotalHeights(size_t itemIndex, bool bIncludeAtTops) const
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return 0;
    }
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return 0;
    }
    const int32_t nDefaultItemHeight = m_pListCtrl->GetDataItemHeight(); //Ĭ���и�
    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    int64_t totalItemHeight = 0;
    int32_t nItemHeight = 0;
    const size_t dataItemCount = itemDataList.size();
    for (size_t index = 0; index < dataItemCount; ++index) {
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = (rowData.nItemHeight < 0) ? nDefaultItemHeight : rowData.nItemHeight;
        if (!rowData.bVisible || (nItemHeight == 0)) {
            //���ɼ��ģ�����
            continue;
        }

        if (rowData.nAlwaysAtTop >= 0) {
            //�ö���Ԫ�أ���Ҫͳ������
            if (bIncludeAtTops) {
                totalItemHeight += nItemHeight;
            }            
        }
        else if (index < itemIndex) {
            //����Ҫ���Ԫ��
            totalItemHeight += nItemHeight;
        }
        else if (!bIncludeAtTops) {
            //�Ѿ����
            break;
        }
    }
    return totalItemHeight;
}

bool ListCtrlReportView::IsNormalMode() const
{
    bool bNormalMode = true;
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    if (pDataProvider != nullptr) {
        bNormalMode = pDataProvider->IsNormalMode();
    }
    return bNormalMode;
}

void ListCtrlReportView::SetAtTopControlIndex(const std::vector<size_t>& atTopControlList)
{
    m_atTopControlList = atTopControlList;
}

void ListCtrlReportView::MoveTopItemsToLast(std::vector<Control*>& items, std::vector<Control*>& atTopItems) const
{
    atTopItems.clear();
    if (items.empty()) {
        return;
    }
    for (size_t index : m_atTopControlList) {
        if (index < items.size()) {
            atTopItems.push_back(items[index]);
        }
    }
    for (int32_t i = (int32_t)m_atTopControlList.size() - 1; i >= 0; --i) {
        size_t index = m_atTopControlList[i];
        if (index < items.size()) {
            items.erase(items.begin() + index);
        }
    }
    for (Control* pControl : atTopItems) {
        items.push_back(pControl);
    }
    ListCtrlHeader* pHeaderCtrl = dynamic_cast<ListCtrlHeader*>(GetItemAt(0));
    if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
        //������Header�ؼ������ⱻ�����ĸ���
        Control* pHeader = items.front();
        items.erase(items.begin());
        items.push_back(pHeader);

        //��Header�ؼ���ӵ��б�
        atTopItems.push_back(pHeader);
    }    
    ASSERT(items.size() == m_items.size());
}

void ListCtrlReportView::PaintChild(IRender* pRender, const UiRect& rcPaint)
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
        //�����Ǳ�ͷ��ֱ�ӻ���
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
    //Header���ö���Ԫ�أ��ƶ������
    std::vector<Control*> atTopItems;
    MoveTopItemsToLast(items, atTopItems);

    UiRect rcTopControls; //�����ö��ؼ��ľ�������
    for (size_t i = 0; i < atTopItems.size(); ++i) {
        const Control* pTopControl = atTopItems[i];
        if (i == 0) {
            rcTopControls = pTopControl->GetRect();
        }
        else {
            rcTopControls.Union(pTopControl->GetRect());
        }
    }

    //�����б����ӿؼ�
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

        bool bHasClip = false;
        if (!atTopItems.empty() &&
            (std::find(atTopItems.begin(), atTopItems.end(), pControl) == atTopItems.end())) {            
            UiRect rcControlRect = pControl->GetRect();
            UiRect rUnion;
            if (UiRect::Intersect(rUnion, rcTopControls, rcControlRect)) {
                //�н�������Ҫ���òü�����������ö�Ԫ��������Ԫ���ص�������
                pRender->SetClip(rUnion, false);
                bHasClip = true;
            }
        }

        UiPoint ptOffset(scrollPos.cx, scrollPos.cy);
        UiPoint ptOldOrg = pRender->OffsetWindowOrg(ptOffset);
        pControl->AlphaPaint(pRender, rcNewPaint);
        pRender->SetWindowOrg(ptOldOrg);
        if (bHasClip) {
            pRender->ClearClip();
        }
    }

    //�����ߵĻ���
    PaintGridLines(pRender);

    //���ƹ�����
    ScrollBar* pVScrollBar = GetVScrollBar();
    ScrollBar* pHScrollBar = GetHScrollBar();
    if ((pHScrollBar != nullptr) && pHScrollBar->IsVisible()) {
        pHScrollBar->AlphaPaint(pRender, rcPaint);
    }

    if ((pVScrollBar != nullptr) && pVScrollBar->IsVisible()) {
        pVScrollBar->AlphaPaint(pRender, rcPaint);
    }

    //����ѡ���ܵĿ�ѡ�����
    PaintFrameSelection(pRender);
}

void ListCtrlReportView::PaintGridLines(IRender* pRender)
{
    int32_t nColumnLineWidth = GetColumnGridLineWidth();//������߿��        
    int32_t nRowLineWidth = GetRowGridLineWidth();   //������߿��
    UiColor columnLineColor;
    UiColor rowLineColor;
    std::wstring color = GetColumnGridLineColor();
    if (!color.empty()) {
        columnLineColor = GetUiColor(color);
    }
    color = GetRowGridLineColor();
    if (!color.empty()) {
        rowLineColor = GetUiColor(color);
    }

    if ((nColumnLineWidth > 0) && !columnLineColor.IsEmpty()) {
        //��������������        
        UiRect viewRect = GetRect();
        int32_t yTop = viewRect.top;
        std::vector<int32_t> xPosList;
        const size_t itemCount = GetItemCount();
        for (size_t index = 0; index < itemCount; ++index) {
            if (index == 0) {
                ListCtrlHeader* pHeader = dynamic_cast<ListCtrlHeader*>(GetItemAt(index));
                if (pHeader != nullptr) {
                    if (pHeader->IsVisible() && (pHeader->GetHeight() > 0)) {
                        yTop = pHeader->GetRect().bottom;//��Header�ĵͶ˿�ʼ����
                    }
                    continue;
                }
            }
            ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(GetItemAt(index));
            if ((pItem == nullptr) || !pItem->IsVisible() || (pItem->GetWidth() <= 0)) {
                continue;
            }
            size_t nSubItemCount = pItem->GetItemCount();
            for (size_t nSubItem = 0; nSubItem < nSubItemCount; ++nSubItem) {
                ListCtrlSubItem* pSubItem = dynamic_cast<ListCtrlSubItem*>(pItem->GetItemAt(nSubItem));
                if ((pSubItem == nullptr) || !pSubItem->IsVisible() || (pSubItem->GetWidth() <= 0)) {
                    continue;
                }
                UiPoint scrollBoxOffset = pSubItem->GetScrollOffsetInScrollBox();
                UiRect subItemRect = pSubItem->GetRect();
                subItemRect.Offset(-scrollBoxOffset.x, -scrollBoxOffset.y);
                xPosList.push_back(subItemRect.right);
            }
            break;
        }

        for (int32_t xPos : xPosList) {
            //������λ�÷���ÿ������ؼ����Ҳಿ            
            UiPoint pt1(xPos, yTop);
            UiPoint pt2(xPos, viewRect.bottom);
            pRender->DrawLine(pt1, pt2, columnLineColor, nColumnLineWidth);
        }
    }
    if ((nRowLineWidth > 0) && !rowLineColor.IsEmpty()) {
        //���ƺ���������
        UiRect viewRect = GetRect();
        const size_t itemCount = GetItemCount();
        for (size_t index = 0; index < itemCount; ++index) {
            ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(GetItemAt(index));
            if ((pItem == nullptr) || !pItem->IsVisible() || (pItem->GetHeight() <= 0)) {
                continue;
            }
            //������λ�÷���ÿ������ؼ��ĵײ���Header�ؼ��ĵײ������ߣ�
            int32_t yPos = pItem->GetRect().bottom;
            int32_t nChildMarginY = 0;
            Layout* pLayout = GetLayout();
            if (pLayout != nullptr) {
                nChildMarginY = pLayout->GetChildMarginY();
            }
            yPos += nChildMarginY / 2;

            UiPoint pt1(viewRect.left, yPos);
            UiPoint pt2(viewRect.right, yPos);
            pRender->DrawLine(pt1, pt2, rowLineColor, nRowLineWidth);
        }
    }
}

void ListCtrlReportView::PaintFrameSelection(IRender* pRender)
{
    if (!m_bInMouseMove || (pRender == nullptr)) {
        return;
    }
    UiSize64 scrollPos = GetScrollPos();
    int64_t left = std::min(m_ptMouseDown.cx, m_ptMouseMove.cx) - scrollPos.cx;
    int64_t right = std::max(m_ptMouseDown.cx, m_ptMouseMove.cx) - scrollPos.cx;
    int64_t top = std::min(m_ptMouseDown.cy, m_ptMouseMove.cy) - scrollPos.cy;
    int64_t bottom = std::max(m_ptMouseDown.cy, m_ptMouseMove.cy) - scrollPos.cy;
    if (m_nNormalItemTop > 0) {
        if (top < m_nNormalItemTop) {
            top = m_nNormalItemTop - GlobalManager::Instance().Dpi().GetScaleInt(4);
        }
        if (bottom < m_nNormalItemTop) {
            bottom = m_nNormalItemTop;
        }
    }

    UiRect rect(TruncateToInt32(left), TruncateToInt32(top),
                TruncateToInt32(right), TruncateToInt32(bottom));

    if ((m_frameSelectionBorderSize > 0) && !m_frameSelectionBorderColor.empty()) {
        pRender->DrawRect(rect, GetUiColor(m_frameSelectionBorderColor.c_str()), m_frameSelectionBorderSize);
    }
    if (!m_frameSelectionColor.empty()) {
        pRender->FillRect(rect, GetUiColor(m_frameSelectionColor.c_str()), m_frameSelectionAlpha);
    }
}

Control* ListCtrlReportView::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos)
{
    //��д��ScrollBox::FindControl ��������Header���ȱ����ҵ���ֻ������UIFIND_TOP_FIRST��־�����
    if ((uFlags & UIFIND_TOP_FIRST) == 0) {
        return __super::FindControl(Proc, pData, uFlags, scrollPos);
    }

    std::vector<Control*> newItems = m_items;
    //Header���ö���Ԫ�أ��ƶ������
    std::vector<Control*> atTopItems;
    MoveTopItemsToLast(newItems, atTopItems);

    // Check if this guy is valid
    if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) {
        return nullptr;
    }
    if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) {
        return nullptr;
    }
    ScrollBar* pVScrollBar = GetVScrollBar();
    ScrollBar* pHScrollBar = GetHScrollBar();
    if ((uFlags & UIFIND_HITTEST) != 0) {
        ASSERT(pData != nullptr);
        if (pData == nullptr) {
            return nullptr;
        }
        UiPoint pt(*(static_cast<UiPoint*>(pData)));
        if (!GetRect().ContainsPt(pt)) {
            return nullptr;
        }
        if (!IsMouseChildEnabled()) {
            Control* pResult = nullptr;            
            if (pVScrollBar != nullptr) {
                pResult = pVScrollBar->FindControl(Proc, pData, uFlags);
            }            
            if ((pResult == nullptr) && (pHScrollBar != nullptr)) {
                pResult = pHScrollBar->FindControl(Proc, pData, uFlags);
            }
            if (pResult == nullptr) {
                pResult = Control::FindControl(Proc, pData, uFlags);
            }
            return pResult;
        }
    }

    Control* pResult = nullptr;
    if (pVScrollBar != nullptr) {
        pResult = pVScrollBar->FindControl(Proc, pData, uFlags);
    }
    if ((pResult == nullptr) && (pHScrollBar != nullptr)) {
        pResult = pHScrollBar->FindControl(Proc, pData, uFlags);
    }
    if (pResult != nullptr) {
        return pResult;
    }

    UiSize ptScrollPos = GetScrollOffset();
    UiPoint ptNewScrollPos(ptScrollPos.cx, ptScrollPos.cy);
    return FindControlInItems(newItems, Proc, pData, uFlags, ptNewScrollPos);
}

Control* ListCtrlReportView::CreateDataItem()
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nullptr;
    }
    ListCtrlItem* pItem = new ListCtrlItem;
    pItem->SetListCtrl(m_pListCtrl);
    pItem->SetClass(m_pListCtrl->GetDataItemClass());
    pItem->SetAutoCheckSelect(m_pListCtrl->IsAutoCheckSelect());
    return pItem;
}

bool ListCtrlReportView::FillDataItem(Control* pControl,
                                    size_t nElementIndex,
                                    const ListCtrlItemData& itemData,
                                    const std::vector<ListCtrlSubItemData2Pair>& subItemList)
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return false;
    }
    ListCtrlHeader* pHeaderCtrl = m_pListCtrl->GetHeaderCtrl();
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return false;
    }

    ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(pControl);
    ASSERT(pItem != nullptr);
    if (pItem == nullptr) {
        return false;
    }

    //�������Ƿ���ʾCheckBox    
    bool bShowCheckBox = m_pListCtrl->IsDataItemShowCheckBox();
    pItem->SetShowCheckBox(bShowCheckBox);
    pItem->SetEnableControlPadding(false);

    ASSERT(itemData.bVisible);
    bool bItemChecked = itemData.bChecked;
    int32_t nImageId = itemData.nImageId;
    if (!bShowCheckBox) {
        bItemChecked = false;
    }
    pItem->SetChecked(bItemChecked, false);
    pItem->SetImageId(nImageId);

    //��������ڱ߾࣬����CheckBox��ʾ��������ʾ�ص�
    int32_t nPaddingLeft = pItem->GetItemPaddingLeft();
    pHeaderCtrl->SetPaddingLeftValue(nPaddingLeft);

    //Header�ؼ����ڱ߾�, ��Ҫͬ����ÿ���б���ؼ�������������һ��
    const UiPadding rcHeaderPadding = pHeaderCtrl->GetPadding();
    UiPadding rcPadding = pItem->GetPadding();
    if (rcHeaderPadding.left != rcPadding.left) {
        rcPadding.left = rcHeaderPadding.left;
        pItem->SetPadding(rcPadding, false);
    }

    // �����ṹ: <ListCtrlItem> <ListCtrlSubItem/> ... <ListCtrlSubItem/>  </ListCtrlItem>
    // ����˵��: 1. ListCtrlItem �� HBox������;   
    //          2. ÿһ�У�����һ��ListCtrlSubItem�ؼ�
    //          3. ListCtrlSubItem ��LabelBox������

    std::map<size_t, ListCtrlSubItemData2Ptr> subItemDataMap;
    for (const ListCtrlSubItemData2Pair& dataPair : subItemList) {
        subItemDataMap[dataPair.nColumnId] = dataPair.pSubItemData;
    }

    //��ȡ��Ҫ��ʾ�ĸ����е�����
    struct ElementData
    {
        size_t nColumnId = Box::InvalidIndex;
        int32_t nColumnWidth = 0;
        ListCtrlSubItemData2Ptr pStorage;
    };
    std::vector<ElementData> elementDataList;
    const size_t nColumnCount = pHeaderCtrl->GetColumnCount();
    for (size_t nColumnIndex = 0; nColumnIndex < nColumnCount; ++nColumnIndex) {
        ListCtrlHeaderItem* pHeaderItem = pHeaderCtrl->GetColumn(nColumnIndex);
        if ((pHeaderItem == nullptr) || !pHeaderItem->IsColumnVisible()) {
            continue;
        }
        int32_t nColumnWidth = pHeaderCtrl->GetColumnWidth(nColumnIndex);
        if (nColumnWidth < 0) {
            nColumnWidth = 0;
        }
        ElementData data;
        data.nColumnId = pHeaderCtrl->GetColumnId(nColumnIndex);
        data.nColumnWidth = nColumnWidth;
        data.pStorage = subItemDataMap[data.nColumnId];
        elementDataList.push_back(data);
    }
    
    ASSERT(!elementDataList.empty());
    if (elementDataList.empty()) {
        return false;
    }

    const size_t showColumnCount = elementDataList.size(); //��ʾ������
    while (pItem->GetItemCount() > showColumnCount) {
        //�Ƴ��������
        if (!pItem->RemoveItemAt(pItem->GetItemCount() - 1)) {
            ASSERT(!"RemoveItemAt failed!");
            return false;
        }
    }

    //Ĭ������
    std::wstring defaultSubItemClass = m_pListCtrl->GetDataSubItemClass();
    ListCtrlSubItem defaultSubItem;
    defaultSubItem.SetWindow(m_pListCtrl->GetWindow());
    defaultSubItem.SetClass(defaultSubItemClass);

    for (size_t nColumn = 0; nColumn < showColumnCount; ++nColumn) {
        const ElementData& elementData = elementDataList[nColumn];
        ListCtrlSubItem* pSubItem = nullptr;
        if (nColumn < pItem->GetItemCount()) {
            pSubItem = dynamic_cast<ListCtrlSubItem*>(pItem->GetItemAt(nColumn));
            ASSERT(pSubItem != nullptr);
            if (pSubItem == nullptr) {
                return false;
            }
        }
        else {
            pSubItem = new ListCtrlSubItem;
            pSubItem->SetListCtrlItem(pItem);
            pItem->AddItem(pSubItem);
            if (!defaultSubItemClass.empty()) {
                pSubItem->SetClass(defaultSubItemClass);
            }
            pSubItem->SetMouseEnabled(false);
        }

        //������ݣ���������        
        pSubItem->SetFixedWidth(UiFixedInt(elementData.nColumnWidth), true, false);
        const ListCtrlSubItemData2Ptr& pStorage = elementData.pStorage;
        if (pStorage != nullptr) {
            pSubItem->SetText(pStorage->text.c_str());
            if (pStorage->nTextFormat != 0) {
                pSubItem->SetTextStyle(pStorage->nTextFormat, false);
            }
            else {
                pSubItem->SetTextStyle(defaultSubItem.GetTextStyle(), false);
            }
            pSubItem->SetTextPadding(defaultSubItem.GetTextPadding(), false);
            if (!pStorage->textColor.IsEmpty()) {
                pSubItem->SetStateTextColor(kControlStateNormal, pSubItem->GetColorString(pStorage->textColor));
            }
            else {
                pSubItem->SetStateTextColor(kControlStateNormal, defaultSubItem.GetStateTextColor(kControlStateNormal));
            }
            if (!pStorage->bkColor.IsEmpty()) {
                pSubItem->SetBkColor(pStorage->bkColor);
            }
            else {
                pSubItem->SetBkColor(defaultSubItem.GetBkColor());
            }
            if (pStorage->bShowCheckBox) {
                //���CheckBox
                pSubItem->SetCheckBoxVisible(true);
                CheckBox* pCheckBox = pSubItem->GetCheckBox();
                ASSERT(pCheckBox != nullptr);

                //����CheckBox���¼�����
                if (pCheckBox != nullptr) {
                    pCheckBox->DetachEvent(kEventSelect);
                    pCheckBox->DetachEvent(kEventUnSelect);
                    pCheckBox->SetSelected(pStorage->bChecked);
                    size_t nColumnId = elementData.nColumnId;
                    pCheckBox->AttachSelect([this, nColumnId, nElementIndex](const EventArgs& /*args*/) {
                        OnSubItemColumnChecked(nElementIndex, nColumnId, true);
                        return true;
                        });
                    pCheckBox->AttachUnSelect([this, nColumnId, nElementIndex](const EventArgs& /*args*/) {
                        OnSubItemColumnChecked(nElementIndex, nColumnId, false);
                        return true;
                        });
                }
            }
            else {
                pSubItem->SetCheckBoxVisible(false);
            }
            pSubItem->SetImageId(pStorage->nImageId);
        }
        else {
            pSubItem->SetTextStyle(defaultSubItem.GetTextStyle(), false);
            pSubItem->SetText(defaultSubItem.GetText());
            pSubItem->SetTextPadding(defaultSubItem.GetTextPadding(), false);
            pSubItem->SetStateTextColor(kControlStateNormal, defaultSubItem.GetStateTextColor(kControlStateNormal));
            pSubItem->SetBkColor(defaultSubItem.GetBkColor());
            pSubItem->SetCheckBoxVisible(false);
            pSubItem->SetImageId(-1);
        }
    }
    return true;
}

int32_t ListCtrlReportView::GetMaxDataItemWidth(const std::vector<ListCtrlSubItemData2Ptr>& subItemList)
{
    int32_t nMaxWidth = -1;
    if (m_pListCtrl == nullptr) {
        return nMaxWidth;
    }

    //Ĭ������
    ListCtrlItem defaultItem;
    defaultItem.SetListCtrl(m_pListCtrl);
    defaultItem.SetWindow(m_pListCtrl->GetWindow());
    defaultItem.SetClass(m_pListCtrl->GetDataItemClass());

    std::wstring defaultSubItemClass = m_pListCtrl->GetDataSubItemClass();
    ListCtrlSubItem defaultSubItem;
    defaultSubItem.SetWindow(m_pListCtrl->GetWindow());
    defaultSubItem.SetClass(defaultSubItemClass);
    defaultSubItem.SetListCtrlItem(&defaultItem);

    ListCtrlSubItem subItem;
    subItem.SetWindow(m_pListCtrl->GetWindow());
    subItem.SetClass(defaultSubItemClass);
    subItem.SetListCtrlItem(&defaultItem);

    for (const ListCtrlSubItemData2Ptr& pStorage : subItemList) {
        if (pStorage == nullptr) {
            continue;
        }
        if (pStorage->text.empty()) {
            continue;
        }

        subItem.SetText(pStorage->text.c_str());
        if (pStorage->nTextFormat != 0) {
            subItem.SetTextStyle(pStorage->nTextFormat, false);
        }
        else {
            subItem.SetTextStyle(defaultSubItem.GetTextStyle(), false);
        }
        subItem.SetTextPadding(defaultSubItem.GetTextPadding(), false);
        subItem.SetCheckBoxVisible(pStorage->bShowCheckBox);
        subItem.SetImageId(pStorage->nImageId);
        subItem.SetFixedWidth(UiFixedInt::MakeAuto(), false, false);
        subItem.SetFixedHeight(UiFixedInt::MakeAuto(), false, false);
        subItem.SetReEstimateSize(true);
        UiEstSize sz = subItem.EstimateSize(UiSize(0, 0));
        nMaxWidth = std::max(nMaxWidth, sz.cx.GetInt32());
    }

    if (nMaxWidth <= 0) {
        nMaxWidth = -1;
    }
    else {
        //����һ������
        nMaxWidth += GlobalManager::Instance().Dpi().GetScaleInt(4);
    }
    return nMaxWidth;
}

void ListCtrlReportView::AdjustSubItemWidth(const std::map<size_t, int32_t>& subItemWidths)
{
    if (subItemWidths.empty()) {
        return;
    }
    size_t itemCount = GetItemCount();
    for (size_t index = 1; index < itemCount; ++index) {
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(GetItemAt(index));
        if (pItem == nullptr) {
            continue;
        }
        size_t columnCount = pItem->GetItemCount();
        for (size_t nColumn = 0; nColumn < columnCount; ++nColumn) {
            auto iter = subItemWidths.find(nColumn);
            if (iter != subItemWidths.end()) {
                int32_t nColumnWidth = iter->second;
                if (nColumnWidth < 0) {
                    nColumnWidth = 0;
                }
                ListCtrlSubItem* pSubItem = dynamic_cast<ListCtrlSubItem*>(pItem->GetItemAt(nColumn));
                if (pSubItem != nullptr) {
                    pSubItem->SetFixedWidth(UiFixedInt(nColumnWidth), true, false);
                }
            }
        }
    }
}

void ListCtrlReportView::OnSubItemColumnChecked(size_t nElementIndex, size_t nColumnId, bool bChecked)
{
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider != nullptr) {
        pDataProvider->SetSubItemCheck(nElementIndex, nColumnId, bChecked, false);
        //���±�ͷ��Ӧ�еĹ�ѡ��״̬
        if (m_pListCtrl != nullptr) {
            m_pListCtrl->UpdateHeaderColumnCheckBox(nColumnId);
        }
    }
}

size_t ListCtrlReportView::GetDisplayItemCount(bool /*bIsHorizontal*/, size_t& nColumns, size_t& nRows) const
{
    nColumns = 1;
    size_t nDiplayItemCount = m_diplayItemIndexList.size();
    size_t nAtTopItemCount = m_atTopControlList.size();
    nRows = nDiplayItemCount;
    if (nRows > nAtTopItemCount) {
        nRows -= nAtTopItemCount;//��ȥ�ö���
    }
    if (nRows > 1) {
        if ((m_pListCtrl != nullptr) && (m_pListCtrl->GetHeaderHeight() > 0)) {
            nRows -= 1;//��ȥHeader
        }
    }
    return nRows * nColumns;
}

bool ListCtrlReportView::IsSelectableElement(size_t nElementIndex) const
{
    bool bSelectable = true;
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider != nullptr) {
        bSelectable = pDataProvider->IsSelectableElement(nElementIndex);
    }
    return bSelectable;
}

size_t ListCtrlReportView::FindSelectableElement(size_t nElementIndex, bool bForward) const
{
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return nElementIndex;
    }
    const size_t nElementCount = pDataProvider->GetElementCount();
    if ((nElementCount == 0) || (nElementIndex >= nElementCount)) {
        return Box::InvalidIndex;
    }
    if (!pDataProvider->IsSelectableElement(nElementIndex)) {
        size_t nStartIndex = nElementIndex;
        nElementIndex = Box::InvalidIndex;
        if (bForward) {
            //��ǰ������һ�������ö���
            for (size_t i = nStartIndex + 1; i < nElementCount; ++i) {
                if (pDataProvider->IsSelectableElement(i)) {
                    nElementIndex = i;
                    break;
                }
            }
        }
        else {
            //��������һ�������ö���
            for (int32_t i = (int32_t)nStartIndex - 1; i >= 0; --i) {
                if (pDataProvider->IsSelectableElement(i)) {
                    nElementIndex = i;
                    break;
                }
            }
        }        
    }
    return nElementIndex;
}

bool ListCtrlReportView::ButtonDown(const EventArgs& msg)
{
    bool bRet = __super::ButtonDown(msg);
    OnButtonDown(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlReportView::ButtonUp(const EventArgs& msg)
{
    bool bRet = __super::ButtonUp(msg);
    OnButtonUp(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlReportView::RButtonDown(const EventArgs& msg)
{
    bool bRet = __super::RButtonDown(msg);
    OnRButtonDown(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlReportView::RButtonUp(const EventArgs& msg)
{
    bool bRet = __super::RButtonUp(msg);
    OnRButtonUp(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlReportView::MouseMove(const EventArgs& msg)
{
    bool bRet = __super::MouseMove(msg);
    OnMouseMove(msg.ptMouse, msg.pSender);
    return bRet;
}

bool ListCtrlReportView::OnWindowKillFocus(const EventArgs& msg)
{
    bool bRet = __super::OnWindowKillFocus(msg);
    OnWindowKillFocus();
    return bRet;
}

void ListCtrlReportView::OnButtonDown(const UiPoint& ptMouse, Control* pSender)
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

void ListCtrlReportView::OnButtonUp(const UiPoint& /*ptMouse*/, Control* pSender)
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

void ListCtrlReportView::OnRButtonDown(const UiPoint& ptMouse, Control* pSender)
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

void ListCtrlReportView::OnRButtonUp(const UiPoint& /*ptMouse*/, Control* pSender)
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

void ListCtrlReportView::OnMouseMove(const UiPoint& ptMouse, Control* pSender)
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

void ListCtrlReportView::OnWindowKillFocus()
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

void ListCtrlReportView::OnCheckScrollView()
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
        UiRect viewRect = GetRect();
        if (m_nNormalItemTop > 0) {
            viewRect.top = m_nNormalItemTop;
            ASSERT(viewRect.top <= viewRect.bottom);
        }

        if (pt.cx <= viewRect.left) {
            //���������ͼ
            LineLeft();
            bScrollView = true;
        }
        else if (pt.cx >= viewRect.right) {
            //���ҹ�����ͼ
            LineRight();
            bScrollView = true;
        }

        int32_t deltaValue = DUI_NOSET_VALUE;
        if (m_pListCtrl != nullptr) {
            deltaValue = m_pListCtrl->GetDataItemHeight() * 2;            
        }
        if (deltaValue > 0) {
            deltaValue = std::max(GetRect().Height() / 3, deltaValue);
        }
        if (pt.cy <= viewRect.top) {
            //���Ϲ�����ͼ
            LineUp(deltaValue, false);
            bScrollView = true;
        }
        else if (pt.cy >= viewRect.bottom) {
            //���¹�����ͼ
            LineDown(deltaValue, false);
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
                                                            nbase::Bind(&ListCtrlReportView::OnCheckScrollView, this),
                                                            50, 1); //ִֻ��һ��
    }
    else {
        //ȡ����ʱ��
        m_scrollViewFlag.Cancel();
    }

    int64_t cx = std::min(m_ptMouseDown.cx, m_ptMouseMove.cx) - GetRect().left;
    //���Ŀ�ѡ������ʾ���ݷ�Χ�ڣ�ִ�п�ѡ����
    bool bInListItem = cx <= GetListCtrlWidth();
    int64_t top = std::min(m_ptMouseDown.cy, m_ptMouseMove.cy);
    int64_t bottom = std::max(m_ptMouseDown.cy, m_ptMouseMove.cy);
    int32_t offsetTop = GetRect().top;//��ǰ�ؼ����Ͻǵ�top����
    top -= offsetTop;
    bottom -= offsetTop;
    bool bRet = OnFrameSelection(top, bottom, bInListItem);
    Invalidate();
    if (bRet) {
        SendEvent(kEventSelChange);
    }
}

bool ListCtrlReportView::OnFrameSelection(int64_t top, int64_t bottom, bool bInListItem)
{
    if (!bInListItem) {
        //�ڿհ״���������ѡ����ֻ��ȡ������ѡ����
        bool bRet = SetSelectNone();
        if (bRet) {
            OnSelectStatusChanged();
        }        
        return bRet;
    }
    ASSERT(top <= bottom);
    if (top > bottom) {
        return false;
    }
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return false;
    }
    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return false;
    }
    const ListCtrlData::RowDataList& itemDataList = pDataProvider->GetItemDataList();
    const size_t dataItemCount = itemDataList.size();
    if (dataItemCount == 0) {
        return false;
    }

    const int32_t nDefaultItemHeight = m_pListCtrl->GetDataItemHeight(); //Ĭ���и�
    int32_t nTopItemHeights = m_pListCtrl->GetHeaderHeight(); //Header���ö�Ԫ����ռ�еĸ߶�

    std::vector<size_t> itemIndexList;

    int64_t totalItemHeight = 0;
    int32_t nItemHeight = 0;    
    for (size_t index = 0; index < dataItemCount; ++index) {
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = (rowData.nItemHeight < 0) ? nDefaultItemHeight : rowData.nItemHeight;
        if (!rowData.bVisible || (nItemHeight == 0)) {
            //���ɼ��ģ�����
            continue;
        }

        if (rowData.nAlwaysAtTop >= 0) {
            //�ö���Ԫ��
            nTopItemHeights += nItemHeight;
            continue;
        }
    }

    top -= nTopItemHeights;
    bottom -= nTopItemHeights;
    if (top < 0) {
        top = 0;
    }
    if (bottom < 0) {
        bottom = 0;
    }
    for (size_t index = 0; index < dataItemCount; ++index) {
        const ListCtrlItemData& rowData = itemDataList[index];
        nItemHeight = (rowData.nItemHeight < 0) ? nDefaultItemHeight : rowData.nItemHeight;
        if (!rowData.bVisible || (nItemHeight == 0)) {
            //���ɼ��ģ�����
            continue;
        }

        if (rowData.nAlwaysAtTop >= 0) {
            //�ö���Ԫ�أ��ų���
            continue;
        }
        totalItemHeight += nItemHeight;
        if (totalItemHeight > top) {
            //��ʼ
            itemIndexList.push_back(index);
        }
        if (totalItemHeight > bottom) {
            //����
            break;
        }
    }

    //ѡ���ѡ������
    bool bRet = SetSelectedElements(itemIndexList, true);
    if (bRet) {
        OnSelectStatusChanged();
    }    
    return bRet;
}

void ListCtrlReportView::SetNormalItemTop(int32_t nNormalItemTop)
{
    m_nNormalItemTop = nNormalItemTop;
}

void ListCtrlReportView::OnItemSelectedChanged(size_t iIndex, IListBoxItem* pListBoxItem)
{
    if (!IsEnableUpdateProvider()) {
        return;
    }
    __super::OnItemSelectedChanged(iIndex, pListBoxItem);
    OnSelectStatusChanged();
}

void ListCtrlReportView::OnItemCheckedChanged(size_t /*iIndex*/, IListBoxItem* pListBoxItem)
{
    if (!IsEnableUpdateProvider()) {
        return;
    }
    ASSERT(pListBoxItem != nullptr);
    if (pListBoxItem == nullptr) {
        return;
    }
    ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(pListBoxItem);
    if (pItem == nullptr) {
        return;
    }

    ListCtrlData* pDataProvider = dynamic_cast<ListCtrlData*>(GetDataProvider());
    ASSERT(pDataProvider != nullptr);
    if (pDataProvider == nullptr) {
        return;
    }
    size_t nElementIndex = pListBoxItem->GetElementIndex();
    bool bCheckChanged = false;
    bool bChecked = pItem->IsChecked();
    pDataProvider->SetDataItemChecked(nElementIndex, bChecked, bCheckChanged);
    if (bCheckChanged) {
        //���±�ͷ�Ĺ�ѡ��״̬
        if (m_pListCtrl != nullptr) {
            m_pListCtrl->UpdateHeaderCheckBox();
        }
    }
}

void ListCtrlReportView::OnSelectStatusChanged()
{
    if ((m_pListCtrl != nullptr) && m_pListCtrl->IsAutoCheckSelect()) {
        //���±�ͷ�Ĺ�ѡ��״̬
        m_pListCtrl->UpdateHeaderCheckBox();
    }
}

void ListCtrlReportView::SendEvent(EventType eventType, WPARAM wParam, LPARAM lParam, TCHAR tChar, const UiPoint& mousePos)
{
    __super::SendEvent(eventType, wParam, lParam, tChar, mousePos);
}

void ListCtrlReportView::SendEvent(const EventArgs& event)
{
    __super::SendEvent(event);
    if ((event.Type == kEventSelect) || (event.Type == kEventUnSelect)) {
        SendEvent(kEventSelChange);
    }
}

void ListCtrlReportView::SetRowGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nLineWidth);
    }
    if (nLineWidth < 0) {
        nLineWidth = 0;
    }
    if (m_nRowGridLineWidth != nLineWidth) {
        m_nRowGridLineWidth = nLineWidth;
        Invalidate();
    }
}

int32_t ListCtrlReportView::GetRowGridLineWidth() const
{
    return m_nRowGridLineWidth;
}

void ListCtrlReportView::SetRowGridLineColor(const std::wstring& color)
{
    if (m_rowGridLineColor != color) {
        m_rowGridLineColor = color;
        Invalidate();
    }
}

std::wstring ListCtrlReportView::GetRowGridLineColor() const
{
    return m_rowGridLineColor.c_str();
}

void ListCtrlReportView::SetColumnGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nLineWidth);
    }
    if (nLineWidth < 0) {
        nLineWidth = 0;
    }
    if (m_nColumnGridLineWidth != nLineWidth) {
        m_nColumnGridLineWidth = nLineWidth;
        Invalidate();
    }
}

int32_t ListCtrlReportView::GetColumnGridLineWidth() const
{
    return m_nColumnGridLineWidth;
}

void ListCtrlReportView::SetColumnGridLineColor(const std::wstring& color)
{
    if (m_columnGridLineColor != color) {
        m_columnGridLineColor = color;
        Invalidate();
    }
}

std::wstring ListCtrlReportView::GetColumnGridLineColor() const
{
    return m_columnGridLineColor.c_str();
}

////////////////////////////////////////////////////////////////////////
/// ListCtrlReportLayout ��ʵ��

ListCtrlReportLayout::ListCtrlReportLayout():
    m_pDataView(nullptr),
    m_bReserveSet(false)
{
    m_nReserveHeight = GlobalManager::Instance().Dpi().GetScaleInt(8);
}

void ListCtrlReportLayout::SetDataView(ListCtrlReportView* pDataView)
{
    ASSERT(pDataView != nullptr);
    m_pDataView = pDataView;
}

UiSize64 ListCtrlReportLayout::ArrangeChild(const std::vector<ui::Control*>& /*items*/, ui::UiRect rc)
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return UiSize64();
    }
    DeflatePadding(rc);
    const int32_t nHeaderHeight = GetHeaderHeight();
    int64_t nTotalHeight = GetElementsHeight(Box::InvalidIndex, true) + nHeaderHeight;
    UiSize64 sz(rc.Width(), rc.Height());
    sz.cy = std::max(nTotalHeight, sz.cy);
    m_bReserveSet = false;
    if (nTotalHeight > rc.Height()) {
        //��Ҫ���ֹ��������ײ�Ԥ���ռ�
        sz.cy += m_nReserveHeight;
        if (pDataView->GetHScrollBar() != nullptr) {
            sz.cy += pDataView->GetHScrollBar()->GetHeight();
        }
        m_bReserveSet = true;
    }
    sz.cx = std::max(GetItemWidth(), rc.Width()); //������ֺ��������
    LazyArrangeChild(rc);
    return sz;
}

UiSize ListCtrlReportLayout::EstimateSizeByChild(const std::vector<Control*>& /*items*/, ui::UiSize szAvailable)
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
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

void ListCtrlReportLayout::LazyArrangeChild(UiRect rc) const
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return;
    }
    UiRect orgRect = rc;

    //��ջ�������
    pDataView->SetAtTopControlIndex(std::vector<size_t>());
    pDataView->SetTopElementIndex(Box::InvalidIndex);
    pDataView->SetDisplayDataItems(std::vector<size_t>());
    pDataView->SetNormalItemTop(-1);

    if (pDataView->IsNormalMode()) {
        //����ģʽ
        LazyArrangeChildNormal(rc);
        return;
    }

    const size_t nItemCount = pDataView->GetItemCount();
    ASSERT(nItemCount > 0);
    if (nItemCount <= 1) {
        //��һ��Ԫ���Ǳ�ͷ        
        return;
    }

    //��һ��Ԫ���Ǳ�ͷ�ؼ���������λ�ô�С
    ListCtrlHeader* pHeaderCtrl = dynamic_cast<ListCtrlHeader*>(pDataView->GetItemAt(0));
    if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
        int32_t nHeaderHeight = pHeaderCtrl->GetFixedHeight().GetInt32();
        if (nHeaderHeight > 0) {
            //��ͷ�Ŀ��
            int32_t nHeaderWidth = std::max(GetItemWidth(), rc.Width());
            if (nHeaderWidth <= 0) {
                nHeaderWidth = rc.Width();
            }
            ui::UiRect rcTile(rc.left, rc.top, rc.left + nHeaderWidth, rc.top + nHeaderHeight);
            pHeaderCtrl->SetPos(rcTile);
            rc.top += nHeaderHeight;
        }
    }

    int32_t nNormalItemTop = rc.top; //��ͨ�б����Header�����ö�����top����

    //��¼�ɼ���Ԫ���������б�
    std::vector<size_t> diplayItemIndexList;

    //��������Y����λ��
    int64_t nScrollPosY = pDataView->GetScrollPos().cy;

    //���㵱ǰ�����ܹ���ʾ����������
    int32_t nCalcItemCount = pDataView->GetMaxDataItemsToShow(nScrollPosY, rc.Height());
    if (nCalcItemCount > (nItemCount - 1)) {
        //UI�ؼ��ĸ������㣬���µ���
        pDataView->AjustItemCount();
    }

    //ȡ����Ҫ��ʾ������Ԫ������б�
    std::vector<ListCtrlReportView::ShowItemInfo> showItemIndexList;
    std::vector<ListCtrlReportView::ShowItemInfo> atTopItemIndexList;
    int64_t nPrevItemHeights = 0;
    pDataView->GetDataItemsToShow(nScrollPosY, nItemCount - 1, 
                                  showItemIndexList, atTopItemIndexList, nPrevItemHeights);
    if (showItemIndexList.empty() && atTopItemIndexList.empty()) {
        //û����Ҫ��ʾ������
        return;
    }

    // ����Ԫ�ص�������
    size_t nTopElementIndex = Box::InvalidIndex;
    if (!atTopItemIndexList.empty()) {
        nTopElementIndex = atTopItemIndexList.front().nItemIndex;
    }
    else if (!showItemIndexList.empty()) {
        nTopElementIndex = showItemIndexList.front().nItemIndex;
    }
    pDataView->SetTopElementIndex(nTopElementIndex); 

    //��������ƫ�ƣ������������ϴ�ʱ��rc���32λ�ľ��εĸ߶Ȼ�Խ�磬��Ҫ64λ���Ͳ�������
    pDataView->SetScrollVirtualOffsetY(nScrollPosY);

    //��һ������Y�������ƫ�ƣ���Ҫ���֣��������λ�ñ䶯������ˢ�½������ƫ��������һ��������ʾ������
    int32_t yOffset = 0;
    if ((nScrollPosY > 0) && !showItemIndexList.empty()) {
        int32_t nFirstHeight = showItemIndexList.front().nItemHeight;
        if (nFirstHeight > 0) {            
            yOffset = std::abs(nScrollPosY - nPrevItemHeights) % nFirstHeight;
        }
    }
    if ((nScrollPosY > 0) && (nScrollPosY == pDataView->GetScrollRange().cy)) {
        //�������Ѿ�����
        if (!showItemIndexList.empty()) {
            size_t nLastItemIndex = showItemIndexList[showItemIndexList.size() - 1].nItemIndex;
            if (nLastItemIndex == pDataView->GetElementCount() - 1) {
                //�Ѿ������һ����¼��ȷ���ײ���ʾ����
                int32_t rcHeights = rc.Height();
                if (m_bReserveSet && (rcHeights > m_nReserveHeight)) {
                    rcHeights -= m_nReserveHeight;
                    if (pDataView->GetHScrollBar() != nullptr) {
                        rcHeights -= pDataView->GetHScrollBar()->GetHeight();
                    }                    
                }
                for (const auto& info : atTopItemIndexList) {
                    rcHeights -= info.nItemHeight;
                }
                for (int32_t nIndex = (int32_t)showItemIndexList.size() - 1; nIndex >= 0; --nIndex) {
                    const auto& info = showItemIndexList[nIndex];
                    if ((rcHeights - info.nItemHeight) > 0) {
                        rcHeights -= info.nItemHeight;
                    }
                }
                if (rcHeights > 0) {
                    int32_t nFirstHeight = 0;
                    if (!showItemIndexList.empty()) {
                        nFirstHeight = showItemIndexList.front().nItemHeight;
                    }
                    if (nFirstHeight > 0) {
                        yOffset = nFirstHeight - (rcHeights % nFirstHeight);
                    }
                }
            }
        }
    }

    struct ShowItemIndex
    {
        size_t nElementIndex;   //Ԫ������ 
        int32_t nItemHeight;    //Ԫ�صĸ߶�
        bool bAtTop;            //�Ƿ��ö�
        int32_t yOffset;        //Y��ƫ����
    };
    std::vector<ShowItemIndex> itemIndexList;
    for (const auto& info : atTopItemIndexList) {
        itemIndexList.push_back({ info.nItemIndex, info.nItemHeight, true, 0});
    }
    for (const auto& info : showItemIndexList) {
        itemIndexList.push_back({ info.nItemIndex, info.nItemHeight, false, yOffset });
        yOffset = 0;    //ֻ�е�һ��Ԫ������ƫ��
    }
    //Ԫ�صĿ�ȣ�����Ԫ�ؿ�ȶ���ͬ
    const int32_t cx = GetItemWidth(); 
    ASSERT(cx > 0);

    //�ؼ������Ͻ�����ֵ
    ui::UiPoint ptTile(rc.left, rc.top);

    VirtualListBox::RefreshDataList refreshDataList;
    VirtualListBox::RefreshData refreshData;

    UiSize szItem;
    size_t iCount = 0;
    std::vector<size_t> atTopUiItemIndexList;
    //��һ��Ԫ���Ǳ�ͷ�ؼ�������������ݣ����Դ�1��ʼ
    for (size_t index = 1; index < nItemCount; ++index) {
        Control* pControl = pDataView->GetItemAt(index);
        if (pControl == nullptr) {
            continue;
        }

        bool bAlwaysAtTop = false;
        size_t nElementIndex = Box::InvalidIndex;
        bool bFillElement = true;
        if (iCount < itemIndexList.size()) {
            //��ǰ����Ԫ�ص�������
            const ShowItemIndex& showItemIndex = itemIndexList[iCount];
            nElementIndex = showItemIndex.nElementIndex;
            bAlwaysAtTop = showItemIndex.bAtTop;
            szItem.cx = cx;
            szItem.cy = showItemIndex.nItemHeight;
            ASSERT(szItem.cy > 0);

            //���õ�ǰ�ؼ��Ĵ�С��λ��
            if (showItemIndex.yOffset > 0) {
                ptTile.y = ptTile.y - showItemIndex.yOffset;
            }
            UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + szItem.cx, ptTile.y + szItem.cy);
            pControl->SetPos(rcTile);
            if (rcTile.top > orgRect.bottom) {
                //���Ԫ���Ѿ����ɼ�����ֹͣ�������
                bFillElement = false;                
            }
        }
        else {
            //�����Ѿ�չʾ���
            bFillElement = false;
        }

        if (nElementIndex >= pDataView->GetElementCount()) {
            bFillElement = false;
        }
        
        if (bFillElement) {
            if (!pControl->IsVisible()) {
                pControl->SetVisible(true);
            }
            // �������
            //TODO: �Ż����룬����ÿ��ˢ�¶�Fill
            pDataView->FillElement(pControl, nElementIndex);
            diplayItemIndexList.push_back(nElementIndex);

            refreshData.nItemIndex = index;
            refreshData.pControl = pControl;
            refreshData.nElementIndex = nElementIndex;
            refreshDataList.push_back(refreshData);

            ListCtrlItem* pListCtrlItem = dynamic_cast<ListCtrlItem*>(pControl);
            if (pListCtrlItem != nullptr) {
                //�ö������ѡ��
                pListCtrlItem->SetSelectableType(bAlwaysAtTop ? false : true);
            }
            if (bAlwaysAtTop) {
                //��¼�ö���
                atTopUiItemIndexList.push_back(index);
                //��¼�ö���ĵײ�����
                nNormalItemTop = pControl->GetRect().bottom;
            }
        }
        else {
            if (pControl->IsVisible()) {
                pControl->SetVisible(false);
            }
        }

        //�л�����һ��
        ptTile.y += szItem.cy + GetChildMarginY();
        ++iCount;
    }

    pDataView->SetAtTopControlIndex(atTopUiItemIndexList);
    pDataView->SetDisplayDataItems(diplayItemIndexList);    
    pDataView->SetNormalItemTop(nNormalItemTop);

    if (!refreshDataList.empty()) {
        pDataView->OnRefreshElements(refreshDataList);
    }
}

void ListCtrlReportLayout::LazyArrangeChildNormal(UiRect rc) const
{    
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return;
    }

    UiRect orgRect = rc;
    const size_t nItemCount = pDataView->GetItemCount();
    if (nItemCount > 0) {
        //��һ��Ԫ���Ǳ�ͷ�ؼ���������λ�ô�С
        Control* pHeaderCtrl = pDataView->GetItemAt(0);
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
                //��¼��ͷ��bottomֵ
                pDataView->SetNormalItemTop(rc.top);
            }
        }
    }

    //Ԫ�صĿ�ȣ�����Ԫ�ؿ�ȶ���ͬ
    const int32_t cx = GetItemWidth();
    ASSERT(cx > 0);

    //Ԫ�صĸ߶ȣ�����Ԫ�ظ߶ȶ���ͬ
    const int32_t cy = GetItemHeight();
    ASSERT(cy > 0);

    //��¼�ɼ���Ԫ���������б�
    std::vector<size_t> diplayItemIndexList;

    // ����Ԫ�ص�������
    const size_t nTopElementIndex = GetTopElementIndex(orgRect);
    pDataView->SetTopElementIndex(nTopElementIndex);

    //��������Y����λ��
    int64_t nScrollPosY = pDataView->GetScrollPos().cy;

    //Y�������ƫ�ƣ���Ҫ���֣��������λ�ñ䶯������ˢ�½������ƫ��
    int32_t yOffset = 0;
    if (cy > 0) {
        yOffset = TruncateToInt32(nScrollPosY % cy);
    }

    //��������ƫ�ƣ������������ϴ�ʱ��rc���32λ�ľ��εĸ߶Ȼ�Խ�磬��Ҫ64λ���Ͳ�������
    pDataView->SetScrollVirtualOffsetY(nScrollPosY);

    //�ؼ������Ͻ�����ֵ
    ui::UiPoint ptTile(rc.left, rc.top - yOffset);

    VirtualListBox::RefreshDataList refreshDataList;
    VirtualListBox::RefreshData refreshData;

    size_t iCount = 0;
    //��һ��Ԫ���Ǳ�ͷ�ؼ�������������ݣ����Դ�1��ʼ
    for (size_t index = 1; index < nItemCount; ++index) {
        Control* pControl = pDataView->GetItemAt(index);
        if (pControl == nullptr) {
            continue;
        }
        //��ǰ����Ԫ�ص�������
        const size_t nElementIndex = nTopElementIndex + iCount;

        //���õ�ǰ�ؼ��Ĵ�С��λ��
        ui::UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + cx, ptTile.y + cy);
        pControl->SetPos(rcTile);

        // �������        
        if (nElementIndex < pDataView->GetElementCount()) {
            if (!pControl->IsVisible()) {
                pControl->SetVisible(true);
            }
            pDataView->FillElement(pControl, nElementIndex);
            diplayItemIndexList.push_back(nElementIndex);

            refreshData.nItemIndex = index;
            refreshData.pControl = pControl;
            refreshData.nElementIndex = nElementIndex;
            refreshDataList.push_back(refreshData);
        }
        else {
            if (pControl->IsVisible()) {
                pControl->SetVisible(false);
            }
        }

        ptTile.y += cy + GetChildMarginY();
        ++iCount;
    }
    pDataView->SetDisplayDataItems(diplayItemIndexList);
    if (!refreshDataList.empty()) {
        pDataView->OnRefreshElements(refreshDataList);
    }
}

size_t ListCtrlReportLayout::AjustMaxItem(UiRect rc) const
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return 1;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 1;
    }
    if (rc.IsEmpty()) {
        return 1;
    }
    int32_t nRows = 0;
    int32_t nHeaderHeight = GetHeaderHeight();
    if (nHeaderHeight > 0) {
        nRows += 1;
        rc.top += nHeaderHeight;
        rc.Validate();
    }

    nRows += rc.Height() / (nItemHeight + GetChildMarginY() / 2);
    //��֤������
    if (nRows > 1) {
        int32_t calcHeight = nRows * nItemHeight + (nRows - 1) * GetChildMarginY();
        if (calcHeight < rc.Height()) {
            nRows += 1;
        }
    }
    
    if (!pDataView->IsNormalMode()) {
        //�Ǳ�׼ģʽ����Ҫ���Ӽ���õ����
        int64_t nScrollPosY = pDataView->GetScrollPos().cy;
        int32_t nCalcRows = pDataView->GetMaxDataItemsToShow(nScrollPosY, rc.Height());
        if (nCalcRows > 0) {
            nCalcRows += 1;
            if (nCalcRows > nRows) {
                nRows = nCalcRows;
            }
        }
    }

    //��������1�У�ȷ����ʵ�ؼ��������������ʾ����
    nRows += 1;
    return nRows;
}

size_t ListCtrlReportLayout::GetTopElementIndex(UiRect /*rc*/) const
{
    size_t nTopElementIndex = 0;
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return nTopElementIndex;
    }
    int64_t nScrollPosY = pDataView->GetScrollPos().cy;
    if (!pDataView->IsNormalMode()) {
        //�Ǳ�׼ģʽ
        nTopElementIndex = pDataView->GetTopDataItemIndex(nScrollPosY);
    }
    else {
        int32_t nItemHeight = GetItemHeight();
        ASSERT(nItemHeight > 0);
        if (nItemHeight > 0) {
            nTopElementIndex = static_cast<size_t>(nScrollPosY / nItemHeight);
        }
    }
    return nTopElementIndex;
}

bool ListCtrlReportLayout::IsElementDisplay(UiRect rc, size_t iIndex) const
{
    if (!Box::IsValidItemIndex(iIndex)) {
        return false;
    }
    std::vector<size_t> itemIndexList;
    GetDisplayElements(rc, itemIndexList);
    return std::find(itemIndexList.begin(), itemIndexList.end(), iIndex) != itemIndexList.end();
}

void ListCtrlReportLayout::GetDisplayElements(UiRect rc, std::vector<size_t>& collection) const
{
    GetDisplayElements(rc, collection, nullptr);
}

void ListCtrlReportLayout::GetDisplayElements(UiRect rc, std::vector<size_t>& collection,
                                            std::vector<size_t>* pAtTopItemIndexList) const
{
    collection.clear();
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return;
    }
    if (pDataView->GetItemCount() <= 1) {
        return;
    }
    int64_t nScrollPosY = pDataView->GetScrollPos().cy;
    if (!pDataView->IsNormalMode()) {
        //�Ǳ�׼ģʽ
        pDataView->GetMaxDataItemsToShow(nScrollPosY, rc.Height(), 
                                         &collection, pAtTopItemIndexList);
        return;
    }

    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight >= 0);
    if (nItemHeight <= 0) {
        return;
    }
    rc.top += GetHeaderHeight();
    rc.Validate();

    size_t min = (size_t)(nScrollPosY / nItemHeight);
    size_t max = min + (size_t)(rc.Height() / nItemHeight);
    size_t nCount = pDataView->GetElementCount();
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

bool ListCtrlReportLayout::NeedReArrange() const
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return false;
    }
    size_t nItemCount = pDataView->GetItemCount();//�������ʾ�ؼ��������У�
    if (nItemCount == 0) {
        return false;
    }

    if (pDataView->GetElementCount() <= nItemCount) {
        return false;
    }

    UiRect rcThis = pDataView->GetPos();
    if (rcThis.IsEmpty()) {
        return false;
    }

    int64_t nScrollPosY = pDataView->GetScrollPos().cy;//�¹�����λ��
    int64_t nVirtualOffsetY = pDataView->GetScrollVirtualOffset().cy;//ԭ������λ��
    //ֻҪ����λ�÷����仯������Ҫ���²���
    return (nScrollPosY != nVirtualOffsetY);
}

void ListCtrlReportLayout::EnsureVisible(UiRect rc, size_t iIndex, bool bToTop) const
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return;
    }
    if (!Box::IsValidItemIndex(iIndex) || (iIndex >= pDataView->GetElementCount())) {
        return;
    }
    ScrollBar* pVScrollBar = pDataView->GetVScrollBar();
    if (pVScrollBar == nullptr) {
        return;
    }
    if (!bToTop) {
        std::vector<size_t> atTopItemIndexList;
        std::vector<size_t> itemIndexList;
        GetDisplayElements(rc, itemIndexList, &atTopItemIndexList);
        bool bDisplay = std::find(itemIndexList.begin(), itemIndexList.end(), iIndex) != itemIndexList.end();
        bool bFirst = false;
        bool bLast = false;
        if (!itemIndexList.empty()) {
            for (size_t i = 0; i < itemIndexList.size(); ++i) {
                if (std::find(atTopItemIndexList.begin(), 
                              atTopItemIndexList.end(), 
                              itemIndexList[i]) == atTopItemIndexList.end()) {
                    //��һ�����ö�������
                    bFirst = itemIndexList[i] == iIndex;
                    break;
                }
            }            
            bLast = itemIndexList[itemIndexList.size() - 1] == iIndex;
        }
        if (bDisplay && !bLast && !bFirst) {
            //�Ѿ�����ʾ״̬
            return;
        }
    }
    UiSize szElementSize = GetElementSize(0, iIndex); //Ŀ��Ԫ�صĴ�С
    int64_t nNewTopPos = 0;     //��������ʱ��λ��
    int64_t nNewBottomPos = 0;  //�ײ�����ʱ��λ��
    if (iIndex > 0) {
        nNewTopPos = GetElementsHeight(iIndex, false);
        if (!bToTop) {
            //�ײ�����
            nNewBottomPos = GetElementsHeight(iIndex, false);
            int64_t nNewPosWithTop = GetElementsHeight(iIndex, true);
            int64_t nTopHeights = GetHeaderHeight();
            if (nNewPosWithTop > nNewBottomPos) {
                nTopHeights += (nNewPosWithTop - nNewBottomPos);
            }
            //�۳��ö���ĸ߶ȡ�Header�ĸ߶Ⱥ�����߶�
            nNewBottomPos -= rc.Height();
            nNewBottomPos += nTopHeights;
            nNewBottomPos += szElementSize.cy;
        }
    }

    if (nNewTopPos < 0) {
        nNewTopPos = 0;
    }
    if (nNewTopPos > pVScrollBar->GetScrollRange()) {
        nNewTopPos = pVScrollBar->GetScrollRange();
    }
    if (nNewBottomPos < 0) {
        nNewBottomPos = 0;
    }
    if (nNewBottomPos > pVScrollBar->GetScrollRange()) {
        nNewBottomPos = pVScrollBar->GetScrollRange();
    }
    ui::UiSize64 sz = pDataView->GetScrollPos();
    int64_t nNewPos = sz.cy;
    if (bToTop) {
        //��������
        nNewPos = nNewTopPos;
    }
    else {
        //δָ�����룬�����ж�
        int64_t diff = sz.cy - nNewBottomPos;
        if (diff < 0) {
            //���Ϲ������ײ�����
            nNewPos = nNewBottomPos;
        }
        else {
            //���¹�������������
            nNewPos = nNewTopPos;
        }
    }
    sz.cy = nNewPos;
    pDataView->SetScrollPos(sz);
}

int64_t ListCtrlReportLayout::GetElementsHeight(size_t nCount, bool bIncludeAtTops) const
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return 0;
    }
    if (!Box::IsValidItemIndex(nCount)) {
        nCount = pDataView->GetElementCount();
    }
    if ((nCount == 0) || !Box::IsValidItemIndex(nCount)){
        return 0;
    }
    int64_t nTotalHeight = 0;
    if (!pDataView->IsNormalMode()) {
        //�Ǳ�׼ģʽ
        nTotalHeight = pDataView->GetDataItemTotalHeights(nCount, bIncludeAtTops);
    }
    else {
        int32_t nItemHeight = GetItemHeight();
        ASSERT(nItemHeight > 0);
        if (nItemHeight <= 0) {
            return 0;
        }
        if (nCount <= 1) {
            //ֻ��1��
            nTotalHeight = nItemHeight + GetChildMarginY();
        }
        else {
            int64_t iChildMargin = 0;
            if (GetChildMarginY() > 0) {
                iChildMargin = GetChildMarginY();
            }
            int64_t childMarginTotal = ((int64_t)nCount - 1) * iChildMargin;
            nTotalHeight = nItemHeight * nCount + childMarginTotal;
        }
    }
    return nTotalHeight;
}

UiSize ListCtrlReportLayout::GetElementSize(int32_t rcWidth, size_t nElementIndex) const
{
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView == nullptr) || !pDataView->HasDataProvider()) {
        ASSERT(FALSE);
        return UiSize();
    }
    UiSize szElementSize;
    szElementSize.cx = std::max(GetItemWidth(), rcWidth);
    if (!pDataView->IsNormalMode()) {
        //�Ǳ�׼ģʽ���и߿��ܸ�����ͬ
        szElementSize.cy = pDataView->GetDataItemHeight(nElementIndex);
    }
    else {
        //��׼ģʽ�������еĸ߶ȶ���ͬ��
        szElementSize.cy = GetItemHeight();
    }    
    return szElementSize;
}

int32_t ListCtrlReportLayout::GetItemWidth() const
{
    //������ͷ�Ŀ����ͬ
    int32_t nItemWidth = 0;
    ListCtrlReportView* pDataView = GetDataView();
    if (pDataView != nullptr) {
        nItemWidth = pDataView->GetListCtrlWidth();
    }
    return nItemWidth;
}

int32_t ListCtrlReportLayout::GetItemHeight() const
{
    //�����еĸ߶���ͬ�����Ҵ����ö�ȡ
    int32_t nItemHeight = 0;
    ListCtrlReportView* pDataView = GetDataView();
    if ((pDataView != nullptr) && (pDataView->m_pListCtrl != nullptr)) {
        nItemHeight = pDataView->m_pListCtrl->GetDataItemHeight();
    }
    return nItemHeight;
}

int32_t ListCtrlReportLayout::GetHeaderHeight() const
{
    int32_t nHeaderHeight = 0;
    ListCtrlReportView* pDataView = GetDataView();
    if (pDataView != nullptr) {
        Control* pHeaderCtrl = nullptr;
        size_t nItemCount = pDataView->GetItemCount();
        if (nItemCount > 0) {
            //��һ��Ԫ���Ǳ�ͷ�ؼ���������λ�ô�С
            pHeaderCtrl = pDataView->GetItemAt(0);            
        }
        if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
            nHeaderHeight = pHeaderCtrl->GetFixedHeight().GetInt32();
        }
    }
    return nHeaderHeight;
}

}//namespace ui

