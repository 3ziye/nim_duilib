#ifndef UI_CONTROL_LIST_CTRL_DATA_PROVIDER_H_
#define UI_CONTROL_LIST_CTRL_DATA_PROVIDER_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"

namespace ui
{
/** ���ڴ洢�����ݽṹ
*/
struct ListCtrlData
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

    //TODO: ��ʵ�ֹ����б�
    //bool bVisible;                  //�Ƿ�ɼ�
    //bool bAlwaysAtTop;              //�ö���ʾ
    //int32_t nBkImageIndex = -1;     //����ͼƬ��Դ������
    //Item���ı����Ա༭
    //ѡ�񣺵�ѡ����ѡ������ѡ�У��ṩ�ӿ�
    //�¼���Ӧ��������Ҽ���
    //�����иߣ����֧��ÿ�е��и߲�ͬ��Header���иߵ������ã�
    //����ͼ��֧�֣�Report��Icon�ȣ�������Windows��Դ������
    //�������͵�֧�֣��������ͣ������ͣ��������ַ������͵�
    //����ͼƬ�б�ͼƬ�б���Ҫ����ʵ��
    //���Margin��֧��

};

/** �Ƚ����ݵĸ�����Ϣ
*/
struct ListCtrlCompareParam
{
    size_t nColumnIndex; //���ݹ����ڼ��У���Ч��Χ��[0, GetColumnCount())
    size_t nColumnId;    //���ݹ����е�ID
    void* pUserData;     //�û��Զ������ݣ����ñȽϺ�����ʱ��һͬ����
};

/** �洢���ݵıȽϺ�����ԭ��, ʵ������ıȽ�(a < b)
* @param [in] a ��һ���Ƚ�����
* @param [in] b �ڶ����Ƚ�����
* @param [in] param ���ݹ����Ĳ���
* @return ��� (a < b)������true�����򷵻�false
*/
typedef std::function<bool(const ListCtrlData& a, const ListCtrlData& b, const ListCtrlCompareParam& param)>
    ListCtrlDataCompareFunc;

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

    /** ɾ�������е�������
    */
    bool DeleteAllDataItems();

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
    bool SetCheckBoxSelect(size_t itemIndex, size_t columnIndex, bool bSelected);

    /** ��ȡCheckBox�Ĺ�ѡ״̬
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] bSelected true��ʾ��ѡ��false��ʾ����ѡ
    */
    bool GetCheckBoxSelect(size_t itemIndex, size_t columnIndex, bool& bSelected) const;

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
    StoragePtr GetDataItemStorage(size_t itemIndex, size_t columnIndex) const;

    /** ��ȡָ�������������, д��
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ���ʧ���򷵻�nullptr
    */
    StoragePtr GetDataItemStorageForWrite(size_t itemIndex, size_t columnIndex);

    /** ��ȡ�����е����ݣ�����UIչʾ
    * @param [in] nDataItemIndex ����Item���±꣬������
    * @param [in] columnIdList ��ID�б�
    * @param [out] storageList ���������б�
    */
    bool GetDataItemStorageList(size_t nDataItemIndex,
                                std::vector<size_t>& columnIdList,
                                StoragePtrList& storageList) const;

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

private:
    /** ��ͷ�ؼ�
    */
    ListCtrl* m_pListCtrl;

    /** ���ݣ����б��棬ÿ����һ������
    */
    StorageMap m_dataMap;

    /** �ⲿ���õ�������
    */
    ListCtrlDataCompareFunc m_pfnCompareFunc;

    /** �ⲿ���õ���������������
    */
    void* m_pUserData;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_DATA_PROVIDER_H_
