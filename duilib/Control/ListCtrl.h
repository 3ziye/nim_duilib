#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/Label.h"
#include "duilib/Control/Split.h"
#include "duilib/Box/ListBoxItem.h"

namespace ui
{

/** �б��������
*/
class ListCtrlColumn
{
public:
    /** ���캯��
    */
    ListCtrlColumn():
        m_width(0),
        m_bSortable(false),
        m_bResizeable(false)
    {

    }

    /** ���캯��
    */
    ListCtrlColumn(const std::wstring& text, int32_t width):
        ListCtrlColumn()
    {
        m_text = text;
        m_width = width;
    }
    /** ����
    */
    UiString m_text;

    /** ���
    */
    int32_t m_width;

    /** �Ƿ���������
    */
    bool m_bSortable;

    /** �Ƿ�����ı��п�
    */
    bool m_bResizeable;
};

/** ListCtrl����ؼ�
*/
class ListCtrlItem: public ListBoxItem
{
public:
	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override { return L"ListCtrlItem"; }

};

//�б����ݹ�����
class ListCtrlData;
class ListCtrlItemProvider;

/** ListCtrl�ؼ�
*/
class ListCtrl: public VBox
{
    friend class ListCtrlHeader;
public:
	ListCtrl();
	virtual ~ListCtrl();

	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

protected:
	/** �ؼ���ʼ��
	*/
	virtual void DoInit() override;

public:
    /** ��ָ��λ�����һ��
    */
    void InsertColumn(int32_t nCol, const ListCtrlColumn& column);

private:
    /** �����еĿ��
    */
    void OnColumnWidthChanged(size_t nColumn1, size_t nColumn2);

private:
	/** ��ʼ����־
	*/
	bool m_bInited;

	/** ��ͷ�ؼ�
	*/
	ListCtrlHeader* m_pListCtrlHeader;

	/** �б�����չʾ
	*/
	ListCtrlData* m_pListCtrlData;

	/** �б����ݹ���
	*/
	std::unique_ptr<ListCtrlItemProvider> m_spItemProvider;
};

/** ListCtrl�ı�ͷ�ؼ�����ʾ��
*/
class ListCtrlHeaderItem:
    public Label
{
public:

};

/** ListCtrl�ı�ͷ�ؼ�
*/
class ListCtrlHeader : public ListBoxItemH
{
public:
    ListCtrlHeader() :
        m_pListCtrl(nullptr)
    {
        m_nSplitWidth = ui::GlobalManager::Instance().Dpi().GetScaleInt(3);
    }

    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override { return L"ListCtrlHeader"; }

public:
    /** ���1��
    */
    void AddColumn(const ListCtrlColumn& column)
    {
        m_listColumns.push_back(column);
    }

    /** ��ȡ�еĸ���
    */
    size_t GetColumnCount() const
    {
        return m_listColumns.size();
    }

    /** ��ȡ������
    */
    bool GetColumn(size_t index, ListCtrlColumn& column) const
    {
        if (index < m_listColumns.size()) {
            column = m_listColumns[index];
            return true;
        }
        return false;
    }

public:

    /** ���ù�����ListCtrl�ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl)
    {
        m_pListCtrl = pListCtrl;
    }

    /** ͬ�������ݶ�Ӧ�Ŀؼ�
    */
    void OnColumnsChanged()
    {
        //�����ṹ��
        //  <ListCtrlHeader>
        //      <ListCtrlHeaderItem/>
        //      <SplitBox> <Control/> </SplitBox>
        //      ..
        //      <ListCtrlHeaderItem/>
        //  </ListCtrlHeader>
        for (size_t index = 0; index < m_listColumns.size(); ++index) {
            const ListCtrlColumn& column = m_listColumns[index];
            ListCtrlHeaderItem* pHeaderItem = nullptr;
            SplitBox* pHeaderSplit = nullptr;
            if (index < (GetItemCount() / 2)) {
                pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2));
                pHeaderSplit = dynamic_cast<SplitBox*>(GetItemAt(index * 2 + 1));
                ASSERT((pHeaderItem != nullptr) && (pHeaderSplit != nullptr));
                if ((pHeaderItem == nullptr) || (pHeaderSplit == nullptr)) {
                    return;
                }
            }
            else {
                pHeaderItem = new ListCtrlHeaderItem;
                AddItem(pHeaderItem);

                if (index != (m_listColumns.size() - 1)) {
                    pHeaderSplit = new SplitBox;
                    AddItem(pHeaderSplit);

                    Control* pSplitCtrl = new Control;
                    pSplitCtrl->SetAttribute(L"width", L"1");
                    pSplitCtrl->SetAttribute(L"height", L"100%");
                    pSplitCtrl->SetAttribute(L"margin", L"2,4,0,2");
                    pSplitCtrl->SetBkColor(L"splitline_level1");
                    pSplitCtrl->SetMouseEnabled(false);
                    pSplitCtrl->SetMouseFocused(false);
                    pSplitCtrl->SetNoFocus();
                    pHeaderSplit->AddItem(pSplitCtrl);
                }
            }

            int32_t nSplitWidth = m_nSplitWidth;
            if (pHeaderSplit == nullptr) {
                nSplitWidth = 0;
            }
            int32_t width = column.m_width - nSplitWidth;
            if (width < 0) {
                width = 0;
            }
            pHeaderItem->SetText(column.m_text.c_str());
            pHeaderItem->SetBkColor(L"Yellow");
            pHeaderItem->SetFixedWidth(UiFixedInt(width), true, false);
            pHeaderItem->SetAttribute(L"height", L"32");
            pHeaderItem->SetAttribute(L"text_align", L"vcenter,hcenter");

            //���������
            pHeaderItem->SetUserDataID(index);

            if (pHeaderSplit != nullptr) {
                pHeaderSplit->SetFixedWidth(UiFixedInt(nSplitWidth), true, false);
                pHeaderSplit->SetAttribute(L"height", L"32");
                pHeaderSplit->SetBkColor(L"Yellow");

                //�����϶���Ӧ�¼�
                pHeaderSplit->AttachSplitDraged([this](const EventArgs& args) {
                    OnSplitDraged((Control*)args.wParam, (Control*)args.lParam);
                    return true;
                    });
            }
        }
    }

    /** �϶���Ӧ�¼�
    */
    void OnSplitDraged(const Control* pLeftControl, const Control* pRightControl)
    {
        size_t nColumn1 = Box::InvalidIndex;
        size_t nColumn2 = Box::InvalidIndex;
        if (pLeftControl != nullptr) {
            nColumn1 = pLeftControl->GetUserDataID();
            if (nColumn1 < m_listColumns.size()) {
                ListCtrlColumn& column = m_listColumns[nColumn1];                
                column.m_width = pLeftControl->GetFixedWidth().GetInt32();
                if (nColumn1 != (m_listColumns.size() - 1)) {
                    const int32_t nSplitWidth = m_nSplitWidth;
                    column.m_width += nSplitWidth;
                }
            }
        }
        if (pRightControl != nullptr) {
            nColumn2 = pRightControl->GetUserDataID();
            if (nColumn2 < m_listColumns.size()) {                
                ListCtrlColumn& column = m_listColumns[nColumn2];
                column.m_width = pRightControl->GetFixedWidth().GetInt32();
                if (nColumn2 != (m_listColumns.size() - 1)) {
                    const int32_t nSplitWidth = m_nSplitWidth;
                    column.m_width += nSplitWidth;
                }                
            }
        }

        if ((nColumn1 != Box::InvalidIndex) || (nColumn2 != Box::InvalidIndex)){
            if (m_pListCtrl != nullptr) {
                m_pListCtrl->OnColumnWidthChanged(nColumn1, nColumn2);
            }
        }
    }

protected:
    /** ������
    */
    std::vector<ListCtrlColumn> m_listColumns;

    /** ������ListCtrl�ӿ�
    */
    ListCtrl* m_pListCtrl;

    /** �ָ����Ŀ��
    */
    int32_t m_nSplitWidth;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
