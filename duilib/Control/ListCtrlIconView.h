#ifndef UI_CONTROL_LIST_CTRL_ICON_VIEW_H_
#define UI_CONTROL_LIST_CTRL_ICON_VIEW_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/ListCtrlDefs.h"

namespace ui
{
/** �б�������ʾ�Ͳ��ֹ���(Icon����)
*/
class ListCtrl;
class ListCtrlData;
class ListCtrlIconView : public VirtualListBox, public IListCtrlView
{
public:
    /** �����Ƿ�ΪListģʽ
    * @param [in] bListMode true��ʾ�б���ͼ������Ϊͼ����ͼ
    */
    explicit ListCtrlIconView(bool bListMode);
    virtual ~ListCtrlIconView();

    virtual std::wstring GetType() const override { return L"ListCtrlIconView"; }
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue);

public:
    /** ����ListCtrl�ؼ��ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

    /** �������ݴ������
    * @param[in] pProvider ���ݴ������
    */
    virtual void SetDataProvider(VirtualListBoxElement* pProvider) override;

    /** ����ѡ������仯���¼�
     * @param[in] callback ѡ������ʱ�Ļص�����
     */
    void AttachSelChange(const EventCallback& callback) { AttachEvent(kEventSelChange, callback); }

    /** �����¼��ĺ���
    */
    virtual void SendEvent(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0, TCHAR tChar = 0,
                           const UiPoint& mousePos = UiPoint()) override;
    virtual void SendEvent(const EventArgs& event) override;

public:
    /** ���ò��ַ���Ĭ������£�Icon��ͼ�����򲼾֣�List��ͼ�Ǻ��򲼾֣�
    * @param [in] bHorizontal true��ʾ���򲼾֣�false��ʾ���򲼾�
    */
    void SetHorizontalLayout(bool bHorizontal);

    /** �жϵ�ǰ���ַ����Ƿ�Ϊ���򲼾�
    */
    bool IsHorizontalLayout() const;

public:
    /** ��ȡ����Ԫ�ص�������
    */
    size_t GetTopElementIndex() const;

    /** �ж�һ���������Ƿ�ɼ�
    * @param [in] itemIndex �������������, ��Ч��Χ��[0, GetDataItemCount())
    */
    bool IsDataItemDisplay(size_t itemIndex) const;

    /** ��ȡ��ǰ��ʾ���������б�˳���Ǵ��ϵ���
    * @param [in] itemIndexList ��ǰ��ʾ���������������б�
    */
    void GetDisplayDataItems(std::vector<size_t>& itemIndexList) const;

    /** ȷ������������ɼ�
    * @param [in] itemIndex �������������
    * @param [in] bToTop �Ƿ�ȷ�������Ϸ�
    */
    bool EnsureDataItemVisible(size_t itemIndex, bool bToTop);

protected:
    /** �ؼ���ʼ��
    */
    virtual void DoInit() override;

       /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual Control* CreateDataItem() override;

    /** ���ָ��������
    * @param [in] pControl ������ؼ�ָ��
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] itemData ����������е����ԣ�
    * @param [in] subItemList �����������ÿһ�е�����, ��1�����е�ID����2�����е����ݣ�
    */
    virtual bool FillDataItem(Control* pControl,
                              size_t nElementIndex,
                              const ListCtrlItemData& itemData,
                              const std::vector<ListCtrlSubItemData2Pair>& subItemList) override;


    /** ��ȡĳ�еĿ�����ֵ
    * @param [in] subItemList �����������ÿһ�е����ݣ�
    * @return ���ظ��п�ȵ����ֵ�����ص���DPI����Ӧ���ֵ�� ���ʧ�ܷ���-1
    */
    virtual int32_t GetMaxDataItemWidth(const std::vector<ListCtrlSubItemData2Ptr>& subItemList) override;

private:
    /** ListCtrl �ؼ��ӿ�
    */
    ListCtrl* m_pListCtrl;

    /** ���ݽӿ�
    */
    ListCtrlData* m_pData;

    /** �Ƿ�ΪListģʽ: true��ʾ�б���ͼ������Ϊͼ����ͼ 
    */
    const bool m_bListMode;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_ICON_VIEW_H_
