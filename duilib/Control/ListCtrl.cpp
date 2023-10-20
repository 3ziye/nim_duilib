#include "ListCtrl.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Render/IRender.h"

namespace ui
{
/** �б�������ݹ�����
*/
class ListCtrlItemProvider : public ui::VirtualListBoxElement
{
public:
    ListCtrlItemProvider():
        m_pListCtrlHeader(nullptr)
    {
        for (int i = 0; i < 10; ++i) {
            m_listItems.push_back(2);
        }
    }

    /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual Control* CreateElement() override
    {
        ListCtrlItem* pItem = new ListCtrlItem;
        pItem->SetClass(L"list_ctrl_item");
        return pItem;
    }

    /** ���ָ��������
    * @param [in] pControl ������ؼ�ָ��
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    */
    virtual bool FillElement(ui::Control* pControl, size_t nElementIndex) override
    {
        if (nElementIndex == 0) {
            return true;
        }
        bool bFirstLine = (nElementIndex == 1);
        //return true;
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(pControl);
        if (pItem == nullptr) {
            return false;
        }
        //pItem->SetAttribute(L"border_size", L"1");
        //pItem->SetAttribute(L"border_color", L"orange");

        //�����ṹ��
        //  <HBox>
        //      <Label/>
        //      ...
        //  </HBox>

        HBox* pItemHBox = nullptr;
        if (pItem->GetItemCount() > 0) {
            pItemHBox = dynamic_cast<HBox*>(pItem->GetItemAt(0));
            ASSERT(pItemHBox != nullptr);
            if (pItemHBox == nullptr) {
                return false;
            }
        }
        if (pItemHBox == nullptr) {
            pItemHBox = new HBox;
            pItem->AddItem(pItemHBox);
        }

        if (m_pListCtrlHeader != nullptr) {
            size_t columnCount = m_pListCtrlHeader->GetColumnCount();
            for (size_t nColumn = 0; nColumn < columnCount; ++nColumn) {
                Label* pLabel = nullptr;
                if (nColumn < pItemHBox->GetItemCount()) {
                    pLabel = dynamic_cast<Label*>(pItemHBox->GetItemAt(nColumn));
                    ASSERT(pLabel != nullptr);
                    if (pLabel == nullptr) {
                        return false;
                    }
                }
                else {
                    pLabel = new Label;
                    pItemHBox->AddItem(pLabel);
                }


                ListCtrlColumn column;
                m_pListCtrlHeader->GetColumn(nColumn, column);
                int32_t width = column.m_width;

                pLabel->SetFixedWidth(UiFixedInt(width), true, false);
                pLabel->SetAttribute(L"height", L"100%");
                pLabel->SetAttribute(L"text_align", L"vcenter,hcenter");
                pLabel->SetBkColor((nElementIndex % 2) ? L"#10FF22FF" : L"#1022FFFF");
                pLabel->SetText(StringHelper::Printf((L"%d,%d"), (int32_t)nElementIndex, (int32_t)nColumn));

                //���Ʊ���
                bool bShowColumnLine = true; //�Ƿ���ʾ����ı���
                bool bShowRowLine = true;    //�Ƿ���ʾ����ı���
                int32_t mColumnLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);//������߿��
                int32_t mRowLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);   //������߿��
                if (!bShowColumnLine) {
                    mColumnLineWidth = 0;
                }
                if (!bShowRowLine) {
                    mRowLineWidth = 0;
                }
                if (bFirstLine) {
                    //��һ��
                    if (nColumn == 0) {
                        //��һ��
                        UiRect rc(mColumnLineWidth, mRowLineWidth, mColumnLineWidth, mRowLineWidth);
                        pLabel->SetBorderSize(rc, false);
                    }
                    else {
                        //�ǵ�һ��
                        UiRect rc(0, mRowLineWidth, mColumnLineWidth, mRowLineWidth);
                        pLabel->SetBorderSize(rc, false);
                    }
                }
                else {
                    //�ǵ�һ��
                    if (nColumn == 0) {
                        //��һ��
                        UiRect rc(mColumnLineWidth, 0, mColumnLineWidth, mRowLineWidth);
                        pLabel->SetBorderSize(rc, false);
                    }
                    else {
                        //�ǵ�һ��
                        UiRect rc(0, 0, mColumnLineWidth, mRowLineWidth);
                        pLabel->SetBorderSize(rc, false);
                    }
                }
                pLabel->SetAttribute(L"border_color", L"orange");
            }
        }

        return true;
    }

    /** ��ȡ����������
    * @return ��������������
    */
    virtual size_t GetElementCount() override
    {
        return m_listItems.size();
    }

    /** ����ѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] bSelected true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual void SetElementSelected(size_t nElementIndex, bool bSelected) override
    {

    }

    /** ��ȡѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @return true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual bool IsElementSelected(size_t nElementIndex) override
    {
        return false;
    }

public:

    /** ���ñ�ͷ�ӿ�
    */
    void SetListCtrlHeader(ListCtrlHeader* pListCtrlHeader)
    {
        m_pListCtrlHeader = pListCtrlHeader;
    }

private:



    /** �б�������
    */
    std::vector<int> m_listItems;

private:
    /** ��ͷ�ؼ�
    */
    ListCtrlHeader* m_pListCtrlHeader;
};

/** �б�������ʾ�ؼ�
*/
class ListCtrlData : public VirtualListBox
{
    friend class ListCtrlDataLayout;
public:
    ListCtrlData();
    virtual ~ListCtrlData();

public:
    /** ���ñ�ͷ�ӿ�
    */
    void SetListCtrlHeader(ListCtrlHeader* pListCtrlHeader);

private:
    /** ��ͷ�ؼ�
    */
    ListCtrlHeader* m_pListCtrlHeader;
};

/** �б�������ʾ�ؼ��Ĳ��ֹ���ӿ�
*/
class ListCtrlDataLayout: public VirtualVTileLayout
{
public:    
};

/** �б�������ʾ�ؼ�
*/
ListCtrlData::ListCtrlData() :
    VirtualListBox(new ListCtrlDataLayout),
    m_pListCtrlHeader(nullptr)
{
    VirtualLayout* pVirtualLayout = dynamic_cast<VirtualLayout*>(GetLayout());
    SetVirtualLayout(pVirtualLayout);
}

ListCtrlData::~ListCtrlData() {}

void ListCtrlData::SetListCtrlHeader(ListCtrlHeader* pListCtrlHeader)
{
    m_pListCtrlHeader = pListCtrlHeader;
}

ListCtrl::ListCtrl():
    m_bInited(false),
    m_pListCtrlHeader(nullptr),
    m_pListCtrlData(nullptr)
{
    m_spItemProvider = std::make_unique<ListCtrlItemProvider>();
}

ListCtrl::~ListCtrl()
{
    /*if (m_pListCtrlHeader != nullptr) {
        delete m_pListCtrlHeader;
        m_pListCtrlHeader = nullptr;
    }*/
}

std::wstring ListCtrl::GetType() const { return DUI_CTR_LISTCTRL; }

void ListCtrl::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"") {
        
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrl::DoInit()
{
    if (m_bInited) {
        return;
    }
    m_bInited = true;

    //��ʼ��Header
    ASSERT(m_pListCtrlHeader == nullptr);
    m_pListCtrlHeader = new ListCtrlHeader;
    m_pListCtrlHeader->SetListCtrl(this);


    int32_t width = ui::GlobalManager::Instance().Dpi().GetScaleInt(200);
    m_pListCtrlHeader->AddColumn(ListCtrlColumn(L"1111", width));
    m_pListCtrlHeader->AddColumn(ListCtrlColumn(L"2222", width));
    m_pListCtrlHeader->AddColumn(ListCtrlColumn(L"3333", width));
    m_pListCtrlHeader->AddColumn(ListCtrlColumn(L"4444", width));
    m_pListCtrlHeader->AddColumn(ListCtrlColumn(L"5555", width));

    /*m_pListCtrlHeader->SetAttribute(L"padding", L"1,1,1,1");
    m_pListCtrlHeader->SetAttribute(L"border_size", L"1");
    m_pListCtrlHeader->SetAttribute(L"border_color", L"red");*/

    //m_pListCtrlHeader->OnColumnsChanged();

    //��ʼ��Body
    ASSERT(m_pListCtrlData == nullptr);
    m_pListCtrlData = new ListCtrlData;
    m_pListCtrlData->SetBkColor(L"white");
    m_pListCtrlData->SetAttribute(L"item_size", L"1200,32");
    m_pListCtrlData->SetAttribute(L"columns", L"1");
    m_pListCtrlData->SetAttribute(L"vscrollbar", L"true");
    m_pListCtrlData->SetAttribute(L"hscrollbar", L"true");
    m_pListCtrlData->SetAttribute(L"width", L"1200");
    m_pListCtrlData->SetDataProvider(m_spItemProvider.get());

    //TODO
    m_pListCtrlHeader->SetBkColor(L"blue");
    m_pListCtrlHeader->SetAttribute(L"width", L"1000");
    //
    m_spItemProvider->SetListCtrlHeader(m_pListCtrlHeader);
    m_pListCtrlData->SetListCtrlHeader(m_pListCtrlHeader);

    m_pListCtrlHeader->OnColumnsChanged();
    m_pListCtrlData->AddItem(m_pListCtrlHeader);
    AddItem(m_pListCtrlData);
}

void ListCtrl::OnColumnWidthChanged(size_t nColumn1, size_t nColumn2)
{
    if ((m_pListCtrlData == nullptr) || (m_pListCtrlHeader == nullptr)){
        return;
    }

    int32_t nColumnWidth1 = -1;
    int32_t nColumnWidth2 = -1;
    if (nColumn1 < m_pListCtrlHeader->GetColumnCount()) {
        ListCtrlColumn column;
        if (m_pListCtrlHeader->GetColumn(nColumn1, column)) {
            nColumnWidth1 = column.m_width;
        }
    }
    if (nColumn2 < m_pListCtrlHeader->GetColumnCount()) {
        ListCtrlColumn column;
        if (m_pListCtrlHeader->GetColumn(nColumn2, column)) {
            nColumnWidth2 = column.m_width;
        }
    }

    if ((nColumnWidth1 < 0) && (nColumnWidth2 < 0)) {
        return;
    }

    size_t itemCount = m_pListCtrlData->GetItemCount();
    for (size_t index = 1; index < itemCount; ++index) {
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(m_pListCtrlData->GetItemAt(index));
        if (pItem == nullptr) {
            continue;
        }
        HBox* pItemHBox = nullptr;
        if (pItem->GetItemCount() > 0) {
            pItemHBox = dynamic_cast<HBox*>(pItem->GetItemAt(0));
            ASSERT(pItemHBox != nullptr);            
        }
        if (pItemHBox == nullptr) {
            continue;
        }

        size_t columnCount = pItemHBox->GetItemCount();
        for (size_t nColumn = 0; nColumn < columnCount; ++nColumn) {
            if ((nColumn == nColumn1) && (nColumnWidth1 >= 0)) {
                Label* pLabel = dynamic_cast<Label*>(pItemHBox->GetItemAt(nColumn));
                if (pLabel != nullptr) {
                    pLabel->SetFixedWidth(UiFixedInt(nColumnWidth1), true, false);
                }
            }
            if ((nColumn == nColumn2) && (nColumnWidth2 >= 0)) {
                Label* pLabel = dynamic_cast<Label*>(pItemHBox->GetItemAt(nColumn));
                if (pLabel != nullptr) {
                    pLabel->SetFixedWidth(UiFixedInt(nColumnWidth2), true, false);
                }
            }
        }
    }
}

}//namespace ui

