#ifndef UI_CONTROL_LIST_CTRL_ITEM_H_
#define UI_CONTROL_LIST_CTRL_ITEM_H_

#pragma once

#include "duilib/Box/ListBox.h"
#include "duilib/Control/Label.h"
#include "duilib/Core/ImageList.h"

namespace ui
{
/** ListCtrl�б�������UI�ؼ����У�
*    �����ṹ: <ListCtrlItem> <ListCtrlSubItem/> ... <ListCtrlSubItem/>  </ListCtrlItem>
     ����˵��: 1. ListCtrlItem ��HBox������;   
              2. ÿһ�У�����һ��ListCtrlSubItem�ؼ�
              3. ListCtrlSubItem ��LabelBox������
*/
class ListCtrl;
class ListCtrlSubItem;
class ListCtrlItem : public ListBoxItemH
{
    friend class ListCtrlData;//���Ų���protect�ӿ�
public:
    ListCtrlItem();
    virtual ~ListCtrlItem();

    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override;

    /** ��������
    */
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

    /** �жϿؼ������Ƿ�Ϊ��ѡ���
     * @return Ĭ�Ϸ���false
     */
    virtual bool IsSelectableType() const override;

    /** ���ÿؼ��Ƿ�ѡ��״̬
     * @param [in] bSelected Ϊ true ʱΪѡ��״̬��false ʱΪȡ��ѡ��״̬
     * @param [in] bTriggerEvent �Ƿ���״̬�ı��¼���true Ϊ���ͣ�����Ϊ false��Ĭ��Ϊ false
     */
    virtual void Selected(bool bSelect, bool bTriggerEvent) override;

    /** �����Ƿ����ѡ��(�ö����ݲ���ѡ���������ݿ�ѡ��)
    */
    void SetSelectableType(bool bSelectable);

public:
    /** ���ù�����ListCtrl�ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

    /** ��ȡ������ListCtrl�ӿ�
    */
    ListCtrl* GetListCtrl() const;

    /** ��ȡ�ӿؼ��ĸ���
    */
    size_t GetSubItemCount() const;

    /** ��ȡ��columnIndex���ӿؼ�
    * @param [in] columnIndex ��������ţ�[0, GetSubItemCount())
    */
    ListCtrlSubItem* GetSubItem(size_t columnIndex) const;

    /** ��ȡ�������λ�õ��ӿؼ�
    * @param [in] ptMouse ������ڵ�λ�ã���Ļ�����
    */
    ListCtrlSubItem* GetSubItem(const UiPoint& ptMouse) const;

    /** ��ȡ�������λ�õ��ӿؼ������������(��һ��)
    * @param [in] ptMouse ������ڵ�λ�ã���Ļ�����
    */
    size_t GetSubItemIndex(const UiPoint& ptMouse) const;

    /** �����Ƿ���������ʾCheckBox
    * @param [in] bShow true��ʾ��������ʾCheckBox��false��ʾ����ʾ
    */
    bool SetShowCheckBox(bool bShow);

    /** �ж��Ƿ�������ʾ��CheckBox
    */
    bool IsShowCheckBox() const;

    /** ���ù���ͼ��Id, ���Ϊ-1��ʾ����ʾͼ�꣬ͼ����ʾ���ı�ǰ��
    */
    void SetImageId(int32_t imageId);

    /** ��ȡ����ͼ��Id
    */
    int32_t GetImageId() const;

    /** ����ͼ��֮��ļ�������أ�
    */
    void SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale);

    /** ��ȡͼ��֮��ļ�������أ�
    */
    int32_t GetIconSpacing() const;

protected:
    /** ��ȡ����ͼ��/CheckBox����ռ�Ŀ��(���)
    */
    int32_t GetItemPaddingLeft();

    /** ����ͼ����Դ
    */
    ImagePtr LoadItemImage() const;

    /** ��ȡCheckBox��ͼƬ���
    */
    int32_t GetCheckBoxImageWidth();

protected:
    virtual void Activate();
    virtual bool ButtonDown(const EventArgs& msg) override;
    virtual bool ButtonUp(const EventArgs& msg) override;
    virtual bool ButtonDoubleClick(const EventArgs& msg) override;
    virtual bool RButtonDown(const EventArgs& msg) override;
    virtual bool RButtonUp(const EventArgs& msg) override;
    virtual bool RButtonDoubleClick(const EventArgs& msg) override;

    /** �Ƿ�֧�ֹ�ѡģʽ��Ŀǰ��TreeView/ListCtrl��ʹ�����ģʽ��
        ��ѡģʽ��ָ��
        ��1��ֻ�е����CheckBoxͼƬ�ϵ�ʱ�򣬹�ѡ��ͼƬ����ѡ��״̬���ǹ�ѡģʽ�£��ǵ���ڿؼ������ھ�ѡ��
        ��2����ѡ״̬��ѡ��״̬���룬��������ͬ��״̬
    */
    virtual bool SupportCheckedMode() const override;

    /** ���ƺ���
    */
    virtual void Paint(IRender* pRender, const UiRect& rcPaint) override;

protected:
    /** ִ��ѡ����
    * @param [in] vkFlag ������־, ȡֵ��Χ�μ� enum VKFlag �Ķ���
    */
    void SelectItem(uint64_t vkFlag);

private:
    /** �Ƿ����ѡ��Ӱ�췽����л�ѡ���
    */
    bool m_bSelectable;

    /** ������ListCtrl�ӿ�
    */
    ListCtrl* m_pListCtrl;

    /** ����ͼ��Id, ���Ϊ-1��ʾ����ʾͼ�꣬ͼ����ʾ���ı�ǰ��
    */
    int32_t m_imageId;

    /** CheckBox��ռ���
    */
    int32_t m_nCheckBoxWidth;

    /** ͼ��֮��ļ��
    */
    int32_t m_nIconSpacing;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_ITEM_H_
