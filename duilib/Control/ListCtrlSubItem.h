#ifndef UI_CONTROL_LIST_CTRL_SUB_ITEM_H_
#define UI_CONTROL_LIST_CTRL_SUB_ITEM_H_

#pragma once

#include "duilib/Box/ListBox.h"
#include "duilib/Control/Label.h"

namespace ui
{
/** �б��������
*/
class ListCtrlItem;
class ListCtrlCheckBox;
class ListCtrlSubItem : public LabelBox
{
public:
    ListCtrlSubItem();

    /** ���ù�����Item�ӿ�
    */
    void SetListCtrlItem(ListCtrlItem* pItem);

    /** ��ȡ������Item�ӿ�
    */
    ListCtrlItem* GetListCtrlItem() const;

    /** �����Ƿ���ʾCheckBox
    * @param [in] bVisible true��ʾ��ʾ��false��ʾ����
    */
    bool SetCheckBoxVisible(bool bVisible);

    /** �жϵ�ǰCheckBox�Ƿ�����ʾ״̬
    @return ����true��ʾCheckBox���ڣ����ҿɼ��� �������CheckBox������false
    */
    bool IsCheckBoxVisible() const;

    /** ����CheckBox�Ĺ�ѡ״̬
    * @param [in] bSelected true��ʾ��ѡ��false��ʾ����ѡ
    * @param [in] ���bSelected��bPartSelectͬʱΪtrue����ʾ����ѡ��
    */
    bool SetCheckBoxSelect(bool bSelected, bool bPartSelect);

    /** ��ȡCheckBox�Ĺ�ѡ״̬
    * @param [out] bSelected true��ʾ��ѡ��false��ʾ����ѡ
    * @param [out] ���bSelected��bPartSelectͬʱΪtrue����ʾ����ѡ��
    */
    bool GetCheckBoxSelect(bool& bSelected, bool& bPartSelect) const;

    /** ��ȡCheckBox�ӿ�
    */
    ListCtrlCheckBox* GetCheckBox() const;

private:
    /** ������Item�ӿ�
    */
    ListCtrlItem* m_pItem;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_SUB_ITEM_H_
