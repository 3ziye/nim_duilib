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
    virtual void HandleEvent(const EventArgs& msg) override;

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

protected:
    //�����Ϣ������true����ʾ��Ϣ�Ѵ�������false�����ʾ��Ϣδ������ת�������ؼ���
    virtual bool ButtonDown(const EventArgs& msg) override;
    virtual bool ButtonUp(const EventArgs& msg) override;
    virtual bool RButtonDown(const EventArgs& msg) override;
    virtual bool RButtonUp(const EventArgs& msg) override;
    virtual bool MouseMove(const EventArgs& msg) override;
    virtual bool OnWindowKillFocus(const EventArgs& msg) override;//�ؼ������Ĵ���ʧȥ����

    /** �����ӿؼ�
    */
    virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;

private:
    void OnButtonDown(const UiPoint& ptMouse, Control* pSender);
    void OnButtonUp(const UiPoint& ptMouse, Control* pSender);
    void OnRButtonDown(const UiPoint& ptMouse, Control* pSender);
    void OnRButtonUp(const UiPoint& ptMouse, Control* pSender);
    void OnMouseMove(const UiPoint& ptMouse, Control* pSender);
    void OnWindowKillFocus();

private:
    /** ����ͼ�հ״������������/�Ҽ�
    */
    bool OnListCtrlClickedBlank();

    /** ����Ƿ���Ҫ������ͼ
    */
    void OnCheckScrollView();

    /** ִ��������ѡ����
    * @param [in] left ��ѡ��X����leftֵ
    * @param [in] right ��ѡ��X����rightֵ
    * @param [in] top ��ѡ��Y����topֵ
    * @param [in] bottom ��ѡ��Y����bottomֵ
    * @return �����ѡ��仯����true�����򷵻�false
    */
    bool OnFrameSelection(int64_t left, int64_t right, int64_t top, int64_t bottom);

    /** ���򲼾֣���������
    */
    int32_t CalcRows() const;

    /** ���򲼾֣���������
    */
    int32_t CalcColumns() const;

    /** ����һ��Ԫ�صľ�������
    */
    void CalcElementRectV(size_t nElemenetIndex, const UiSize& szItem, 
                          int32_t nColumns, int32_t childMarginX, int32_t childMarginY,
                          int64_t& iLeft, int64_t& iTop, int64_t& iRight, int64_t& iBottom) const;

    /** ����һ��Ԫ�صľ�������
    */
    void CalcElementRectH(size_t nElemenetIndex, const UiSize& szItem, 
                          int32_t nRows, int32_t childMarginX, int32_t childMarginY,
                          int64_t& iLeft, int64_t& iTop, int64_t& iRight, int64_t& iBottom) const;

    /** ��������ѡ�ı߿�������ɫ
    */
    void PaintFrameSelection(IRender* pRender);

    /** ��ȡ������ͼ�Ĺ�������
    */
    void GetScrollDeltaValue(int32_t& nHScrollValue, int32_t& nVScrollValue) const;

    /** ��ӦKeyDown��Ϣ
    * @return ����true��ʾ�ɹ���������false��ʾδ�������Ϣ
    */
    bool OnListCtrlKeyDown(const EventArgs& msg);

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

private:
    /** �Ƿ��������ͼ�а�����������Ҽ�
    */
    bool m_bMouseDownInView;

    /** �Ƿ�����������
    */
    bool m_bMouseDown;

    /** �Ƿ�����Ҽ�����
    */
    bool m_bRMouseDown;

    /** �Ƿ�����껬��������
    */
    bool m_bInMouseMove;

    /** ��갴��ʱ�����λ��
    */
    UiSize64 m_ptMouseDown;

    /** ��껬��ʱ�����λ��
    */
    UiSize64 m_ptMouseMove;

    /** ��갴��ʱ�Ŀؼ��ӿ�
    */
    Control* m_pMouseSender;

    /** ��ʱ��������ͼʱ��ȡ������
    */
    nbase::WeakCallbackFlag m_scrollViewFlag;

    /** ����ѡ���ܵ�����
    */
    bool m_bEnableFrameSelection; //�Ƿ�֧������ѡ����
    UiString m_frameSelectionColor; //��ѡ�����ɫ
    uint8_t m_frameSelectionAlpha;  //��ѡ�����ɫ��Alphaֵ
    UiString m_frameSelectionBorderColor; //��ѡ�߿���ɫ
    uint8_t m_frameSelectionBorderSize; //��ѡ�߿��С

private:
    /** û��Shift��ʱ�����һ��ѡ�����Ч��Χ��[0, GetElementCount())
    */
    size_t m_nLastNoShiftIndex;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_ICON_VIEW_H_
