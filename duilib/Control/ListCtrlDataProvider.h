#ifndef UI_CONTROL_LIST_CTRL_DATA_PROVIDER_H_
#define UI_CONTROL_LIST_CTRL_DATA_PROVIDER_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/ListCtrlDefs.h"

namespace ui
{
/** �б�������ݹ�����
*/
class ListCtrl;
struct ListCtrlDataItem;
class ListCtrlDataProvider : public ui::VirtualListBoxElement
{
public:
    //���ڴ洢�����ݽṹ
    typedef ListCtrlData Storage;
    typedef std::shared_ptr<Storage> StoragePtr;
    typedef std::vector<StoragePtr> StoragePtrList;
    typedef std::unordered_map<size_t, StoragePtrList> StorageMap;
    typedef std::vector<ListCtrlRowData> RowDataList;

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
    virtual size_t GetElementCount() const override;

    /** ����ѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] bSelected true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual void SetElementSelected(size_t nElementIndex, bool bSelected) override;

    /** ��ȡѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @return true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual bool IsElementSelected(size_t nElementIndex) const override;

    /** ��ȡѡ���Ԫ���б�
    * @param [in] selectedIndexs ���ص�ǰѡ���Ԫ���б���Ч��Χ��[0, GetElementCount())
    */
    virtual void GetSelectedElements(std::vector<size_t>& selectedIndexs) const override;

    /** �Ƿ�֧�ֶ�ѡ
    */
    virtual bool IsMultiSelect() const override;

    /** �����Ƿ�֧�ֶ�ѡ���ɽ������ã����������ؼ�һ��
    * @return bMultiSelect true��ʾ֧�ֶ�ѡ��false��ʾ��֧�ֶ�ѡ
    */
    virtual void SetMultiSelect(bool bMultiSelect) override;

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

    /** ��ȡĳ�еĿ�����ֵ
    * @return ���ظ��п�ȵ����ֵ�����ص���DPI����Ӧ���ֵ�� ���ʧ�ܷ���-1
    */
    int32_t GetColumnWidthAuto(size_t columnId) const;

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

    /** ����ָ��<��,��>��������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] dataItem ָ������������ݣ��������dataItem.nColumnIndex��ָ��
    */
    bool SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ��ȡָ��<��,��>��������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] dataItem ָ�������������
    */
    bool GetDataItem(size_t itemIndex, size_t columnIndex, ListCtrlDataItem& dataItem) const;

    /** ɾ��ָ���е�������
    * @param [in] itemIndex �������������
    */
    bool DeleteDataItem(size_t itemIndex);

    /** ɾ�������е�������
    */
    bool DeleteAllDataItems();

    /** ���������������������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] itemData ����������
    * @param [out] bChanged ���������Ƿ�仯
    */
    bool SetDataItemRowData(size_t itemIndex, const ListCtrlRowData& itemData, bool& bChanged);

    /** ��ȡ�����������������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] itemData ����������
    */
    bool GetDataItemRowData(size_t itemIndex, ListCtrlRowData& itemData) const;

    /** ����������Ŀɼ���
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] bVisible �Ƿ�ɼ�
    * @param [out] bChanged ���������Ƿ�仯
    */
    bool SetDataItemVisible(size_t itemIndex, bool bVisible, bool& bChanged);

    /** ��ȡ������Ŀɼ���
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @return ��������������Ŀɼ���
    */
    bool IsDataItemVisible(size_t itemIndex) const;

    /** �����������ѡ������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] bSelected �Ƿ�ѡ��״̬
    * @param [out] bChanged ���������Ƿ�仯
    */
    bool SetDataItemSelected(size_t itemIndex, bool bSelected, bool& bChanged);

    /** ��ȡ�������ѡ������
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @return ���������������ѡ��״̬
    */
    bool IsDataItemSelected(size_t itemIndex) const;

    /** ��ȡѡ��״̬(bSelect)
    * @param [out] bSelected �Ƿ�ѡ��
    * @param [out] bPartSelected �Ƿ񲿷�ѡ��
    */
    void GetDataItemsSelectStatus(bool& bSelected, bool& bPartSelected) const;

    /** ����������Ĺ�ѡ���ԣ�ÿ��ǰ���CheckBox��
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] bChecked �Ƿ�ѡ״̬
    * @param [out] bChanged ���������Ƿ�仯
    */
    bool SetDataItemChecked(size_t itemIndex, bool bChecked, bool& bChanged);

    /** ��ȡ�������ѡ�����ԣ�ÿ��ǰ���CheckBox��
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @return ��������������Ĺ�ѡ״̬
    */
    bool IsDataItemChecked(size_t itemIndex) const;

    /** ���������еĹ�ѡ״̬��Checked����UnChecked��
    * @param [in] bChecked true��ʾѡ��false��ʾȡ��ѡ��
    */
    bool SetAllDataItemsCheck(bool bChecked);

    /** �������ù�ѡ��������׵�CheckBox�򹴵����ݣ�
    * @param [in] itemIndexs ��Ҫ���ù�ѡ�������������ţ���Ч��Χ��[0, GetDataItemCount())
    * @param [in] bClearOthers ���Ϊtrue����ʾ��������ѡ��Ľ������ѡ��ֻ�����������õ�Ϊѡ����
    * @param [out] refreshIndexs ������Ҫˢ����ʾ��Ԫ��������
    */
    void SetCheckedDataItems(const std::vector<size_t>& itemIndexs,
                             bool bClearOthers,
                             std::vector<size_t>& refreshIndexs);

    /** ��ȡ��ѡ��Ԫ���б����׵�CheckBox�򹴵����ݣ�
    * @param [in] itemIndexs ���ص�ǰ��ѡ�������������ţ���Ч��Χ��[0, GetDataItemCount())
    */
    void GetCheckedDataItems(std::vector<size_t>& itemIndexs) const;

    /** ��ȡ��ѡ״̬(bChecked)
    * @param [out] bChecked �Ƿ�ѡ
    * @param [out] bPartChecked �Ƿ񲿷ֹ�ѡ
    */
    void GetDataItemsCheckStatus(bool& bChecked, bool& bPartChecked) const;

    /** ������������ö�״̬
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] nAlwaysAtTop �ö�״̬��-1��ʾ���ö�, 0 ���� ������ʾ�ö�����ֵԽ�����ȼ�Խ�ߣ�������ʾ��������
    * @param [out] bChanged ���������Ƿ�仯
    */
    bool SetDataItemAlwaysAtTop(size_t itemIndex, int8_t nAlwaysAtTop, bool& bChanged);

    /** ��ȡ��������ö�״̬
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @return ����������������ö�״̬��-1��ʾ���ö�, 0 ���� ������ʾ�ö�����ֵԽ�����ȼ�Խ�ߣ�������ʾ��������
    */
    int8_t GetDataItemAlwaysAtTop(size_t itemIndex) const;

    /** ������������и�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] nItemHeight �и�, -1��ʾʹ��ListCtrl���õ�Ĭ���иߣ�����ֵ��ʾ���е������и�
    * @param [in] bNeedDpiScale ���Ϊtrue��ʾ��Ҫ�Կ�Ƚ���DPI����Ӧ
    * @param [out] bChanged ���������Ƿ�仯
    */
    bool SetDataItemHeight(size_t itemIndex, int32_t nItemHeight, bool bNeedDpiScale, bool& bChanged);

    /** ��ȡ��������и�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @return ����������������и�, -1��ʾʹ��ListCtrl���õ�Ĭ���иߣ�����ֵ��ʾ���е������и�
    */
    int32_t GetDataItemHeight(size_t itemIndex) const;

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

public:
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

    /** ����ָ����������ı����ԣ��ı����뷽ʽ�ȣ�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] nTextFormat ��Ҫ���õ��ı�����
    */
    bool SetDataItemTextFormat(size_t itemIndex, size_t columnIndex, int32_t nTextFormat);

    /** ��ȡָ����������ı����ԣ��ı����뷽ʽ�ȣ�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ������������ı�����
    */
    int32_t GetDataItemTextFormat(size_t itemIndex, size_t columnIndex) const;

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

    /** �Ƿ���ʾCheckBox
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    */
    bool IsShowCheckBox(size_t itemIndex, size_t columnIndex) const;

    /** �����Ƿ���ʾCheckBox
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] bShowCheckBox true��ʾ��ʾ��false��ʾ����ʾ
    */
    bool SetShowCheckBox(size_t itemIndex, size_t columnIndex, bool bShowCheckBox);

    /** ����CheckBox�Ĺ�ѡ״̬
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] bSelected true��ʾ��ѡ��false��ʾ����ѡ
    */
    bool SetCheckBoxCheck(size_t itemIndex, size_t columnIndex, bool bSelected);

    /** ��ȡCheckBox�Ĺ�ѡ״̬
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] bSelected true��ʾ��ѡ��false��ʾ����ѡ
    */
    bool GetCheckBoxCheck(size_t itemIndex, size_t columnIndex, bool& bSelected) const;

    /** ��ȡѡ��״̬(bSelect)
    * @param [in] columnId �е�ID
    * @param [out] bChecked �Ƿ�ѡ��
    * @param [out] bPartChecked �Ƿ񲿷�ѡ��
    */
    void GetCheckBoxCheckStatus(size_t columnId, bool& bChecked, bool& bPartChecked) const;

    /** ����������
    * @param [in] columnId �е�ID
    * @param [in] bSortedUp true��ʾ����false��ʾ����
    * @param [in] pfnCompareFunc ���ݱȽϺ���
    * @param [in] pUserData �û��Զ������ݣ����ñȽϺ�����ʱ��ͨ���������ظ��ȽϺ���
    */
    bool SortDataItems(size_t nColumnId, bool bSortedUp,
                       ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData);

    /** �����ⲿ�Զ����������, �滻Ĭ�ϵ�������
    * @param [in] pfnCompareFunc ���ݱȽϺ���
    * @param [in] pUserData �û��Զ������ݣ����ñȽϺ�����ʱ��ͨ���������ظ��ȽϺ���
    */
    void SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData);

private:
    /** ����ת��Ϊ�洢���ݽṹ
    */
    void DataItemToStorage(const ListCtrlDataItem& item, Storage& storage) const;

    /** �洢����ת��Ϊ�ṹ����
    */
    void StorageToDataItem(const Storage& storage, ListCtrlDataItem& item) const;

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
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    */
    bool IsValidDataItemIndex(size_t itemIndex) const;

    /** �ж�һ����ID�����ݴ洢���Ƿ���Ч
    */
    bool IsValidDataColumnId(size_t nColumnId) const;

    /** ��ȡָ�������������, ��ȡ
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ���ʧ���򷵻�nullptr
    */
    StoragePtr GetDataItemStorage(size_t itemIndex, size_t columnIndex) const;

    /** ��ȡָ�������������, д��
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ���ʧ���򷵻�nullptr
    */
    StoragePtr GetDataItemStorageForWrite(size_t itemIndex, size_t columnIndex);

    /** ��ȡ�����е����ݣ�����UIչʾ
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIdList ��ID�б�
    * @param [out] storageList ���������б�
    */
    bool GetDataItemStorageList(size_t itemIndex,
                                std::vector<size_t>& columnIdList,
                                StoragePtrList& storageList) const;

    /** ĳ���������Check��ѡ״̬�仯(�м�)
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] nColumnId ��ID
    * @param [in] bChecked �Ƿ�ѡ
    */
    void OnDataItemColumnChecked(size_t itemIndex, size_t nColumnId, bool bChecked);

    /** ͬ��UI��Check״̬(�м����CheckBox)
    * @param [in] nColumnId ��ID
    */
    void UpdateHeaderColumnCheckBox(size_t nColumnId);

public:
    /** ��ȡ����������
    */
    const RowDataList& GetItemDataList() const;

    /** �Ƿ�Ϊ��׼ģʽ���и߶�ΪĬ���иߣ��������У����ö��У�
    */
    bool IsNormalMode() const;

private:
    /** ��������
    */
    struct StorageData
    {
        size_t index;       //ԭ��������������
        StoragePtr pStorage;
    };

    /** ����������
    * @param [in] dataList �����������
    * @param [in] nColumnId �е�ID
    * @param [in] bSortedUp true��ʾ����false��ʾ����
    * @param [in] pfnCompareFunc ���ݱȽϺ���
    * @param [in] pUserData �û��Զ������ݣ����ñȽϺ�����ʱ��ͨ���������ظ��ȽϺ���
    */
    bool SortStorageData(std::vector<StorageData>& dataList,
                         size_t nColumnId, bool bSortedUp,
                         ListCtrlDataCompareFunc pfnCompareFunc,
                         void* pUserData);

    /** Ĭ�ϵ����ݱȽϺ���
    * @param [in] a ��һ���Ƚ�����
    * @param [in] b �ڶ����Ƚ�����
    * @return ��� (a < b)������true�����򷵻�false
    */
    bool SortDataCompareFunc(const ListCtrlData& a, const ListCtrlData& b) const;

    /** ���¸��Ի����ݣ������С��иߡ��ö��ȣ�
    */
    void UpdateNormalMode();

private:
    /** ��ͷ�ؼ�
    */
    ListCtrl* m_pListCtrl;

    /** ���ݣ����б��棬ÿ����һ������
    */
    StorageMap m_dataMap;

    /** �е���������
    */
    RowDataList m_rowDataList;

    /** �ⲿ���õ�������
    */
    ListCtrlDataCompareFunc m_pfnCompareFunc;

    /** �ⲿ���õ���������������
    */
    void* m_pUserData;

    /** �����еĸ���
    */
    int32_t m_hideRowCount;

    /** ��Ĭ���и��еĸ���
    */
    int32_t m_heightRowCount;

    /** �ö��еĸ���
    */
    int32_t m_atTopRowCount;

    /** �Ƿ�֧�ֶ�ѡ
    */
    bool m_bMultiSelect;

    /** ��ѡ��ʱ��ѡ���Ԫ��������
    */
    size_t m_nSelectedIndex;

    /** ��ǰĬ�ϵ��ı�����
    */
    int32_t m_nDefaultTextStyle;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_DATA_PROVIDER_H_
