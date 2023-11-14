#ifndef UI_CONTROL_LIST_CTRL_ITEM_H_
#define UI_CONTROL_LIST_CTRL_ITEM_H_

#pragma once

#include "duilib/Box/ListBox.h"
#include "duilib/Control/Label.h"

namespace ui
{
/** ListCtrl�б�������UI�ؼ����У�
*    �����ṹ: <ListCtrlItem> <ListCtrlSubItem/> ... <ListCtrlSubItem/>  </ListCtrlItem>
     ����˵��: 1. ListCtrlItem ��HBox������;   
              2. ÿһ�У�����һ��ListCtrlSubItem�ؼ�
              3. ListCtrlSubItem ��LabelBox������
*/
class ListCtrl;
class ListCtrlItem : public ListBoxItemH
{
public:
    ListCtrlItem();
    virtual ~ListCtrlItem();

    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override;

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

protected:
    virtual void Activate();
    virtual bool ButtonDown(const EventArgs& msg) override;
    virtual bool ButtonUp(const EventArgs& msg) override;
    virtual bool ButtonDoubleClick(const EventArgs& msg) override;
    virtual bool RButtonDown(const EventArgs& msg) override;
    virtual bool RButtonUp(const EventArgs& msg) override;
    virtual bool RButtonDoubleClick(const EventArgs& msg) override;

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
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_ITEM_H_
