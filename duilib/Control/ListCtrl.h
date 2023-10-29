#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Control/ListCtrlHeader.h"
#include "duilib/Control/ListCtrlHeaderItem.h"
#include "duilib/Control/ListCtrlDataProvider.h"
#include "duilib/Box/ListBoxItem.h"

namespace ui
{

/** �б�������UI�ؼ����У�
*/
class ListCtrlItem;

/** �б�������ʾUI�ؼ�
*/
class ListCtrlDataView;

/** �б����ݹ�������
*/
class ListCtrlDataProvider;

/** �еĻ�����Ϣ����������У�
*/
struct ListCtrlColumn
{
    std::wstring text;              //��ͷ���ı�
    int32_t nColumnWidth = 100;     //�п����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMin = 0;    //�п���Сֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMax = 0;    //�п����ֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    bool bSortable = true;          //�Ƿ�֧������
    bool bResizeable = true;        //�Ƿ�֧��ͨ���϶������п�
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox��֧���ڱ�ͷ����������ʾCheckBox��
    int32_t nCheckBoxWidth = 24;    //CheckBox�ؼ���ռ�Ŀ�ȣ����������ı�ƫ������������ʾ�ص�������bShowCheckBoxΪtrueʱ��Ч, ���bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    bool bNeedDpiScale = true;      //�Ƿ����ֵ��DPI����Ӧ
};

/** �б�������Ļ�����Ϣ������������ݣ�
*/
struct ListCtrlDataItem
{
    size_t nColumnIndex = 0;        //������ڼ��У���Ч��Χ��[0, GetColumnCount())
    std::wstring text;              //�ı�����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    int32_t nImageIndex = -1;       //ͼ����Դ�����ţ���ͼƬ�б�������±�ֵ�����Ϊ-1��ʾ����ʾͼ��
    UiColor textColor;              //�ı���ɫ
    UiColor bkColor;                //������ɫ
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox
    int32_t nCheckBoxWidth = 24;    //CheckBox�ؼ���ռ�Ŀ�ȣ�����bShowCheckBoxΪtrueʱ��Ч, ���bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    bool bNeedDpiScale = true;      //�Ƿ����ֵ��DPI����Ӧ
};

/** ListCtrl�ؼ�
*/
class ListCtrl: public VBox
{
    friend class ListCtrlHeader;
    friend class ListCtrlDataProvider;
    friend class ListCtrlDataView;
public:
	ListCtrl();
	virtual ~ListCtrl();

	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

public:
    /** ��ָ��λ�����һ��
    * @param [in] columnIndex �ڵڼ����Ժ������У������-1����ʾ�����׷��һ��
    * @param [in] columnInfo �еĻ�������
    * @return ������һ�еı�ͷ�ؼ��ӿ�
    */
    ListCtrlHeaderItem* InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo);

    /** ��ȡ�еĸ���
    */
    size_t GetColumnCount() const;

    /** ��ȡ�п��
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    int32_t GetColumnWidth(size_t columnIndex) const;

    /** ��ȡ�б�ͷ�Ŀؼ��ӿ�
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    ListCtrlHeaderItem* GetColumn(size_t columnIndex) const;

    /** ��ȡ�б�ͷ�Ŀؼ��ӿ�
    * @param [in] columnId �е�IDֵ��ͨ��ListCtrlHeaderItem::GetColomnId()������ȡ
    */
    ListCtrlHeaderItem* GetColumnById(size_t columnId) const;

    /** ��ȡ�е��������
    * @param [in] columnId �е�IDֵ��ͨ��ListCtrlHeaderItem::GetColomnId()������ȡ
    * @return �е���ţ�[0, GetColumnCount())������ڼ���
    */
    size_t GetColumnIndex(size_t columnId) const;

    /** ��ȡ�е�ID
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    size_t GetColumnId(size_t columnIndex) const;

    /** ɾ��һ��
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    bool DeleteColumn(size_t columnIndex);

    /** ��ȡ��ͷ�ؼ��ӿ�, �ڿؼ���ʼ���Ժ����ֵ
    */
    ListCtrlHeader* GetListCtrlHeader() const;

    /** �����Ƿ�֧���б�ͷ�϶��ı��е�˳��
    */
    void SetEnableHeaderDragOrder(bool bEnable);

    /** �Ƿ�֧���б�ͷ�϶��ı��е�˳��
    */
    bool IsEnableHeaderDragOrder() const;

    /** �����Ƿ���ʾ��ͷ�ؼ�
    */
    void SetHeaderVisible(bool bVisible);

    /** ��ǰ�Ƿ���ʾ��ͷ�ؼ�
    */
    bool IsHeaderVisible() const;

    /** ���ñ�ͷ�ؼ��ĸ߶�
    * @param[in] bNeedDpiScale ���Ϊtrue��ʾ��Ҫ�Կ�Ƚ���DPI����Ӧ
    */
    void SetHeaderHeight(int32_t nHeaderHeight, bool bNeedDpiScale);

    /** ��ȡ��ͷ�ؼ��ĸ߶�
    */
    int32_t GetHeaderHeight() const;

public:
    /** ����������ĸ߶�
    * @param[in] bNeedDpiScale ���Ϊtrue��ʾ��Ҫ�Կ�Ƚ���DPI����Ӧ
    */
    void SetDataItemHeight(int32_t nItemHeight, bool bNeedDpiScale);

    /** ��ȡ������ĸ߶�
    */
    int32_t GetDataItemHeight() const;

    /** ��ȡ�������ܸ���
    */
    size_t GetDataItemCount() const;

    /** �����������ܸ���(��Ӧ����)
    * @param [in] itemCount �����������������ÿ������������ݣ�ͨ���ص��ķ�ʽ������䣨�ڲ�Ϊ���ʵ�֣�
    */
    bool SetDataItemCount(size_t itemCount);

    /** ��������һ��������
    * @param [in] dataItem �����������
    * @return �ɹ��������������������(rowIndex)��ʧ���򷵻�Box::InvalidIndex
    */
    size_t AddDataItem(const ListCtrlDataItem& dataItem);

    /** ��ָ����λ�����һ��������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] dataItem �����������
    */
    bool InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ����ָ���е�������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] dataItem �����������
    */
    bool SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ɾ��ָ���е�������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    */
    bool DeleteDataItem(size_t itemIndex);

    /** ɾ�������е�������
    */
    bool DeleteAllDataItems();

    /** ������������Զ�������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] itemData ������������Զ�������
    */
    bool SetDataItemData(size_t itemIndex, size_t itemData);

    /** ��ȡ��������Զ�������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @return ����������������Զ�������
    */
    size_t GetDataItemData(size_t itemIndex) const;

    /** ����ָ����������ı�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] text ��Ҫ���õ��ı�����
    */
    bool SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text);

    /** ��ȡָ����������ı�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ������������ı�����
    */
    std::wstring GetDataItemText(size_t itemIndex, size_t columnIndex) const;

    /** ����ָ����������ı���ɫ
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] textColor ��Ҫ���õ��ı���ɫ
    */
    bool SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor);

    /** ��ȡָ����������ı���ɫ
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] textColor ������������ı���ɫ
    */
    bool GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const;

    /** ����ָ��������ı�����ɫ
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] bkColor ��Ҫ���õı�����ɫ
    */
    bool SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor);

    /** ��ȡָ��������ı�����ɫ
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] bkColor ����������ı�����ɫ
    */
    bool GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const;

    /** ����������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] bSortedUp true��ʾ����false��ʾ����
    * @param [in] pfnCompareFunc �Զ���ıȽϺ��������Ϊnullptr, ��ʹ��Ĭ�ϵıȽϺ���
    * @param [in] pUserData �û��Զ������ݣ����ñȽϺ�����ʱ��ͨ���������ظ��ȽϺ���
    */
    bool SortDataItems(size_t columnIndex, bool bSortedUp, 
                       ListCtrlDataCompareFunc pfnCompareFunc = nullptr,
                       void* pUserData = nullptr);

    /** �����ⲿ�Զ����������, �滻Ĭ�ϵ�������
    * @param [in] pfnCompareFunc ���ݱȽϺ���
    * @param [in] pUserData �û��Զ������ݣ����ñȽϺ�����ʱ��ͨ���������ظ��ȽϺ���
    */
    void SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData);

public:
    /** ��ȡ��ǰ��ʾ���������б�˳���Ǵ��ϵ���
    * @param [in] itemIndexList ��ǰ��ʾ���������������б�, ÿһԪ�صĵ���Ч��Χ��[0, GetDataItemCount())
    */
    void GetDisplayDataItems(std::vector<size_t>& itemIndexList) const;

    /** �õ��ɼ���Χ�ڵ�һ���������������
    * @return ����������������ţ� ��Ч��Χ��[0, GetDataItemCount())
    */
    size_t GetTopDataItem() const;

    /** �ж�һ���������Ƿ�ɼ�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    */
    bool IsDataItemDisplay(size_t itemIndex) const;

    /** ȷ������������ɼ�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] bToTop �Ƿ�ȷ�������Ϸ�
    */
    bool EnsureDataItemVisible(size_t itemIndex, bool bToTop);

protected:
    /** �ؼ���ʼ��
    */
    virtual void DoInit() override;

    /** ����ListCtrlHeader������Class
    */
    void SetHeaderClass(const std::wstring& className);

    /** ListCtrlHeaderItem������Class
    */
    void SetHeaderItemClass(const std::wstring& className);
    std::wstring GetHeaderItemClass() const;

    /** ListCtrlHeader/SplitBox������Class
    */
    void SetHeaderSplitBoxClass(const std::wstring& className);
    std::wstring GetHeaderSplitBoxClass() const;

    /** ListCtrlHeader/SplitBox/Control������Class
    */
    void SetHeaderSplitControlClass(const std::wstring& className);
    std::wstring GetHeaderSplitControlClass() const;

    /** CheckBox��Class����(Ӧ����Header��ListCtrl����)
    */
    void SetCheckBoxClass(const std::wstring& className);
    std::wstring GetCheckBoxClass() const;

    /** ������ͼ�е�ListBox��Class����
    */
    void SetDataViewClass(const std::wstring& className);
    std::wstring GetDataViewClass() const;

    /** ListCtrlItem��Class����
    */
    void SetDataItemClass(const std::wstring& className);
    std::wstring GetDataItemClass() const;

    /** ListCtrlItem/LabelBox��Class����
    */
    void SetDataItemLabelClass(const std::wstring& className);
    std::wstring GetDataItemLabelClass() const;

    /** ���������ߵĿ��
    * @param [in] nLineWidth �����ߵĿ�ȣ����Ϊ0��ʾ����ʾ����������
    * @param [in] bNeedDpiScale ���Ϊtrue��ʾ��Ҫ�Կ�Ƚ���DPI����Ӧ
    */
    void SetRowGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale);
    int32_t GetRowGridLineWidth() const;

    /** ���������ߵĿ��
    * @param [in] nLineWidth �����ߵĿ�ȣ����Ϊ0��ʾ����ʾ����������
    * @param [in] bNeedDpiScale ���Ϊtrue��ʾ��Ҫ�Կ�Ƚ���DPI����Ӧ
    */
    void SetColumnGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale);
    int32_t GetColumnGridLineWidth() const;

protected:
    /** ����һ��
    * @param [in] nColumnId �е�ID
    */
    void OnHeaderColumnAdded(size_t nColumnId);

    /** ɾ��һ��
    * @param [in] nColumnId �е�ID
    */
    void OnHeaderColumnRemoved(size_t nColumnId);

    /** �����еĿ�ȣ��϶��п������ÿ�ε��������еĿ�ȣ�
    * @param [in] nColumnId1 ��һ�е�ID
    * @param [in] nColumnId2 �ڶ��е�ID
    */
    void OnColumnWidthChanged(size_t nColumnId1, size_t nColumnId2);

    /** ��������
    * @param [in] nColumnId �е�ID
    * @param [in] bSortedUp ���Ϊtrue��ʾ�������Ϊfalse��ʾ����
    */
    void OnColumnSorted(size_t nColumnId, bool bSortedUp);

    /** ͨ���϶��б�ͷ���������е�˳��
    */
    void OnHeaderColumnOrderChanged();

    /** ��ͷ��CheckBox��ѡ����
    * @param [in] nColumnId �е�ID
    * @param [in] bChecked true��ʾ��ѡ��Checked״̬����false��ʾȡ����ѡ��UnChecked״̬��
    */
    void OnHeaderColumnCheckStateChanged(size_t nColumnId, bool bChecked);

    /** ��ͷ�е���ʾ���Է����仯
    */
    void OnHeaderColumnVisibleChanged();

    /** ͬ��UI��Check״̬
    */
    void UpdateControlCheckStatus(size_t nColumnId);

private:
	/** ��ʼ����־
	*/
	bool m_bInited;

	/** ��ͷ�ؼ�
	*/
	ListCtrlHeader* m_pHeaderCtrl;

	/** �б�����չʾ
	*/
    ListCtrlDataView* m_pDataView;

	/** �б����ݹ���
	*/
    ListCtrlDataProvider* m_pDataProvider;

    /** ListCtrlHeader������Class
    */
    UiString m_headerClass;

    /** ListCtrlHeaderItem������Class
    */
    UiString m_headerItemClass;

    /** ListCtrlHeader/SplitBox������Class
    */
    UiString m_headerSplitBoxClass;

    /** ListCtrlHeader/SplitBox/Control������Class
    */
    UiString m_headerSplitControlClass;

    /** CheckBox��Class
    */
    UiString m_checkBoxClass;

    /** �Ƿ�֧���϶��ı��е�˳��
    */
    bool m_bEnableHeaderDragOrder;

    /** ListCtrlItem��Class����
    */
    UiString m_dataItemClass;

    /** ListCtrlItem/LabelBox��Class����
    */
    UiString m_dataItemLabelClass;

    /** ��ǰ�Ƿ���Ը���Header��Check״̬
    */
    bool m_bCanUpdateHeaderCheckStatus;

    /** ���������ߵĿ��
    */
    int32_t m_nRowGridLineWidth;

    /** ���������ߵĿ��
    */
    int32_t m_nColumnGridLineWidth;

    /** ListBox��Class����
    */
    UiString m_dataViewClass;

    /** ��ͷ�ĸ߶�
    */
    int32_t m_nHeaderHeight;

    /** �Ƿ���ʾ��ͷ�ؼ�
    */
    bool m_bShowHeaderCtrl;

    /** ������ĸ߶�
    */
    int32_t m_nItemHeight;
};

/** ListCtrl����ؼ�
*/
class ListCtrlItem : public ListBoxItemH
{
public:
    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override { return L"ListCtrlItem"; }

};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
