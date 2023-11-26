#ifndef UI_CONTROL_LIST_CTRL_DEFS_H_
#define UI_CONTROL_LIST_CTRL_DEFS_H_

#pragma once

#include "duilib/Control/ListCtrlView.h"
#include "duilib/Control/CheckBox.h"

namespace ui
{
/** �������
*/
enum class ListCtrlType
{
    Report = 0, //Report����
    Icon   = 1, //Icon����
    List   = 2  //List����
};

/** �еĻ�����Ϣ����������У�
*/
struct ListCtrlColumn
{
    std::wstring text;              //��ͷ���ı�
    int32_t nColumnWidth = 100;     //�п����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMin = 0;    //�п���Сֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMax = 0;    //�п����ֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    int32_t nImageId = -1;          //ͼ����ԴId�����Ϊ-1��ʾ����ʾͼ�꣬ͼ����ʾ���ı�ǰ��
    bool bSortable = true;          //�Ƿ�֧������
    bool bResizeable = true;        //�Ƿ�֧��ͨ���϶������п�
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox��֧���ڱ�ͷ����������ʾCheckBox��
    bool bNeedDpiScale = true;      //�Ƿ����ֵ��DPI����Ӧ
};

/** �е��������ݽṹ(�����ݣ�ÿ��1������)
*/
struct ListCtrlItemData
{
    bool bVisible = true;           //�Ƿ�ɼ�
    bool bSelected = false;         //�Ƿ���ѡ��״̬����ָ�����Ƿ�ѡ�У�
    bool bChecked = false;          //�Ƿ��ڹ�ѡ״̬����ָ����ǰ���CheckBox�Ƿ񱻴򹴣�
    int32_t nImageId = -1;          //ͼ����ԴId�����Ϊ-1��ʾ���ײ���ʾͼ��
    int8_t nAlwaysAtTop = -1;       //�Ƿ��ö���ʾ, -1��ʾ���ö�, 0 ���� ������ʾ�ö�����ֵԽ�����ȼ�Խ�ߣ�������ʾ��������
    int16_t nItemHeight = -1;       //�еĸ߶�, -1��ʾʹ��ListCtrl���õ�Ĭ���иߣ�ΪDPI����Ӧ������ֵ
    size_t nUserData = 0;           //�û��Զ�������
};

/** �б�������Ļ�����Ϣ�������ݣ�����������ݣ�
*/
struct ListCtrlSubItemData
{
    std::wstring text;              //�ı�����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    int32_t nImageId = -1;          //ͼ����ԴId�����Ϊ-1��ʾ����ʾͼ��
    UiColor textColor;              //�ı���ɫ
    UiColor bkColor;                //������ɫ
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox
    bool bChecked = false;          //�Ƿ��ڹ�ѡ״̬��CheckBox��ѡ״̬��
};

/** �б������������ڲ��洢�����ݽṹ(�����ݣ�ÿ<��,��>1������)
*/
struct ListCtrlSubItemData2
{
    UiString text;                  //�ı�����
    uint16_t nTextFormat = 0;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    int32_t nImageId = -1;          //ͼ����ԴId�����Ϊ-1��ʾ����ʾͼ��
    UiColor textColor;              //�ı���ɫ
    UiColor bkColor;                //������ɫ
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox  
    bool bChecked = false;          //�Ƿ��ڹ�ѡ״̬��CheckBox��ѡ״̬��

    //TODO: ��ʵ�ֹ����б�
    //Item���ı����Ա༭
    //����ͼ��֧�֣�Report��Icon�ȣ�������Windows��Դ������
    //�������͵�֧�֣��������ͣ������ͣ��������ַ������͵�
};

//�����ݵ�����ָ��
typedef std::shared_ptr<ListCtrlSubItemData2> ListCtrlSubItemData2Ptr;

struct ListCtrlSubItemData2Pair
{
    size_t nColumnId; //�е�ID
    ListCtrlSubItemData2Ptr pSubItemData; //�е�����
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
typedef std::function<bool(const ListCtrlSubItemData2& a, 
                           const ListCtrlSubItemData2& b, 
                           const ListCtrlCompareParam& param)> ListCtrlDataCompareFunc;

/** ��ͼ������ݵ�UI�ؼ�����ؽӿ�
*/
class IListCtrlView
{
public:
    /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual Control* CreateDataItem() = 0;

    /** ���ָ��������
    * @param [in] pControl ������ؼ�ָ��
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] itemData ����������е����ԣ�
    * @param [in] subItemList �����������ÿһ�е�����, ��1�����е�ID����2�����е����ݣ�
    */
    virtual bool FillDataItem(ui::Control* pControl,
                              size_t nElementIndex,
                              const ListCtrlItemData& itemData,
                              const std::vector<ListCtrlSubItemData2Pair>& subItemList) = 0;


    /** ��ȡĳ�еĿ�����ֵ
    * @param [in] subItemList �����������ÿһ�е����ݣ�
    * @return ���ظ��п�ȵ����ֵ�����ص���DPI����Ӧ���ֵ�� ���ʧ�ܷ���-1
    */
    virtual int32_t GetMaxDataItemWidth(const std::vector<ListCtrlSubItemData2Ptr>& subItemList) = 0;
};

/** Icon��ͼ���б�������(��ֱ����)
*   �����ṹ��<ListCtrlIconViewItem> <Control/><Label/> </ListCtrlListViewItem>
*   ���е�Control��Label�����ԣ�֧�ִ������ļ���ȡ
*/
class ListCtrlIconViewItem : public ListCtrlItemBaseV
{
public:
    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override { return L"ListCtrlIconViewItem"; }
};

/** List��ͼ���б�������(ˮƽ����)
*   �����ṹ��<ListCtrlListViewItem> <Control/><Label/> </ListCtrlListViewItem>
*   ���е�Control��Label�����ԣ�֧�ִ������ļ���ȡ
*/
class ListCtrlListViewItem : public ListCtrlItemBaseH
{
    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override { return L"ListCtrlListViewItem"; }
};

/** �б���ʹ�õ�CheckBox
*/
class ListCtrlCheckBox: public CheckBox
{
public:
    /** ��ȡ�ؼ����ͺ���������
    */
    virtual std::wstring GetType() const override { return L"ListCtrlCheckBox"; }
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override
    {
        if (strName == L"check_box_width") {
            SetCheckBoxWidth(_wtoi(strValue.c_str()), true);
        }
        else {
            __super::SetAttribute(strName, strValue);
        }
    }

    /** ����CheckBox��ռ�Ŀ��ֵ
    */
    void SetCheckBoxWidth(int32_t nWidth, bool bNeedDpiScale)
    {
        if (bNeedDpiScale) {
            GlobalManager::Instance().Dpi().ScaleInt(nWidth);
        }
        if (nWidth < 0) {
            nWidth = 0;
        }
        m_nCheckBoxWidth = nWidth;
    }

    /** ��ȡCheckBox��ռ�Ŀ��ֵ
    */
    int32_t GetCheckBoxWidth()
    {
        if (m_nCheckBoxWidth > 0) {
            return m_nCheckBoxWidth;
        }
        //���û���ã���ѯͼƬ�Ĵ�С������ס
        UiEstSize sz;
        if (GetWindow() != nullptr) {
            sz = EstimateSize(UiSize(0, 0));
        }
        int32_t nWidth = sz.cx.GetInt32();
        if (nWidth > 0) {
            UiMargin rcMargin = GetMargin();
            UiPadding rcPadding = GetPadding();
            nWidth += rcMargin.left + rcMargin.right;
            nWidth += rcPadding.left + rcPadding.right;
            m_nCheckBoxWidth = nWidth;

            m_nCheckBoxWidth += GlobalManager::Instance().Dpi().GetScaleInt(2);
        }
        return m_nCheckBoxWidth;
    }

private:
    /** ��ʾCheckBox��ռ�Ŀ�ȣ��������ø��ؼ���Paddingֵ
    */
    int32_t m_nCheckBoxWidth = 0;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_DEFS_H_
