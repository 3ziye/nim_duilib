#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Box/VBox.h"
#include "duilib/Box/HBox.h"
#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/Label.h"
#include "duilib/Control/Split.h"

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
        m_widthMin(0),
        m_widthMax(0),
        m_bSortable(false),
        m_bResizeable(false),
        m_bAutoWidth(0),
        m_nSortIconSpace(0)
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

    /** ��С���
    */
    int32_t m_widthMin;

    /** �����
    */
    int32_t m_widthMax;

    /** �Ƿ���������
    */
    bool m_bSortable;

    /** �Ƿ�����ı��п�
    */
    bool m_bResizeable;

    /** �Ƿ��п�����Ӧ
    */
    bool m_bAutoWidth;

    /** ���������ڱ߾�
    */
    UiPadding m_textPadding;

    /** ���ֶ��뷽ʽ
    */
    UiString m_textAligh;

    /** ����ͼ�������ּ��
    */
    int32_t m_nSortIconSpace;
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

/** ListCtrl�ı�ͷ�ؼ�
*/
class ListCtrlHeader : public ListBoxItem
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
        //  <HBox>
        //      <Label/>
        //      <Split/>
        //  </HBox>
        HBox* pHeaderHBox = nullptr;
        if (GetItemCount() > 0) {
            pHeaderHBox = dynamic_cast<HBox*>(GetItemAt(0));
            ASSERT(pHeaderHBox != nullptr);
            if (pHeaderHBox == nullptr) {
                return;
            }
        }
        if (pHeaderHBox == nullptr) {
            pHeaderHBox = new HBox;
            AddItem(pHeaderHBox);
        }
        for (size_t index = 0; index < m_listColumns.size(); ++index) {
            const ListCtrlColumn& column = m_listColumns[index];
            Label* pLabel = nullptr;
            SplitBox* pSplit = nullptr;
            if (index < (pHeaderHBox->GetItemCount() / 2)) {
                pLabel = dynamic_cast<Label*>(pHeaderHBox->GetItemAt(index * 2));
                pSplit = dynamic_cast<SplitBox*>(pHeaderHBox->GetItemAt(index * 2 + 1));
                ASSERT((pLabel != nullptr) && (pSplit != nullptr));
                if ((pLabel == nullptr) || (pSplit == nullptr)) {
                    return;
                }
            }
            else {
                pLabel = new Label;
                pSplit = new SplitBox;
                pHeaderHBox->AddItem(pLabel);
                pHeaderHBox->AddItem(pSplit);

                Control* pSplitCtrl = new Control;
                pSplitCtrl->SetAttribute(L"width", L"1");
                pSplitCtrl->SetAttribute(L"height", L"100%");
                pSplitCtrl->SetAttribute(L"margin", L"2,4,0,2");
                pSplitCtrl->SetBkColor(L"splitline_level1");
                pSplitCtrl->SetMouseEnabled(false);
                pSplitCtrl->SetMouseFocused(false);
                pSplitCtrl->SetNoFocus();
                pSplit->AddItem(pSplitCtrl);
            }

            const int32_t nSplitWidth = m_nSplitWidth;
            pSplit->SetFixedWidth(UiFixedInt(nSplitWidth), true, false);
            pSplit->SetAttribute(L"height", L"32");
            pSplit->SetBkColor(L"Yellow");

            int32_t width = column.m_width - nSplitWidth;
            if (width < 0) {
                width = 0;
            }
            pLabel->SetText(column.m_text.c_str());
            pLabel->SetBkColor(L"Yellow");
            pLabel->SetFixedWidth(UiFixedInt(width), true, false);
            pLabel->SetAttribute(L"height", L"32");
            pLabel->SetAttribute(L"text_align", L"vcenter,hcenter");

            /*pLabel->SetAttribute(L"padding", L"1,1,1,1");
            pLabel->SetAttribute(L"border_size", L"1");
            pLabel->SetAttribute(L"border_color", L"blue");*/

            //���������
            pLabel->SetUserDataID(index);

            //�����϶���Ӧ�¼�
            pSplit->AttachSplitDraged([this](const EventArgs& args) {
                OnSplitDraged((Control*)args.wParam, (Control*)args.lParam);
                return true;
                });
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
                const int32_t nSplitWidth = m_nSplitWidth;
                column.m_width = pLeftControl->GetFixedWidth().GetInt32();
                column.m_width += nSplitWidth;
            }
        }
        if (pRightControl != nullptr) {
            nColumn2 = pRightControl->GetUserDataID();
            if (nColumn2 < m_listColumns.size()) {
                const int32_t nSplitWidth = m_nSplitWidth;
                ListCtrlColumn& column = m_listColumns[nColumn2];
                column.m_width = pRightControl->GetFixedWidth().GetInt32();
                column.m_width += nSplitWidth;
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
