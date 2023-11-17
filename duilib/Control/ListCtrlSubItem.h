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

    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override;

    /** ��������
    */
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

public:
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

    /** ���ù���ͼ��Id, ���Ϊ-1��ʾ����ʾͼ�꣬ͼ����ʾ���ı�ǰ��
    */
    void SetImageId(int32_t imageId);

    /** ��ȡ����ͼ��Id
    */
    int32_t GetImageId() const;

    /** ����������ͼ��֮��ļ�������أ�
    */
    void SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale);

    /** ��ȡ������ͼ��֮��ļ�������أ�
    */
    int32_t GetIconSpacing() const;

protected:
    /** ��������
    */
    virtual void PaintText(IRender* pRender) override;

private:
    /** ������Item�ӿ�
    */
    ListCtrlItem* m_pItem;

    /** ����ͼ��Id, ���Ϊ-1��ʾ����ʾͼ�꣬ͼ����ʾ���ı�ǰ��
    */
    int32_t m_imageId;

    /** ������ͼ��֮��ļ��
    */
    int32_t m_nIconSpacing;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_SUB_ITEM_H_
