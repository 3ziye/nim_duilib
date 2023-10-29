#ifndef UI_CONTROL_LIST_CTRL_HEADER_H_
#define UI_CONTROL_LIST_CTRL_HEADER_H_

#pragma once

#include "duilib/Box/ListBoxItem.h"

namespace ui
{

/** ListCtrl�ı�ͷ�ؼ�
*/
class ListCtrl;
class ListCtrlHeaderItem;
struct ListCtrlColumn;
class ListCtrlHeader : public ListBoxItemH
{
    friend class ListCtrlHeaderItem;
public:
    ListCtrlHeader();

    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override;

public:
    /** ��ָ��λ�����һ��
    *  ��ͷ�ؼ��Ļ����ṹ���£�
    * 
    *   <ListCtrlHeader>
    *       <ListCtrlHeaderItem> CheckBox[��ѡ] </ListCtrlHeaderItem>
    *       <SplitBox> <Control/> </SplitBox>
    *       ..
    *       <ListCtrlHeaderItem> CheckBox[��ѡ] </ListCtrlHeaderItem>
    *       <SplitBox> <Control/> </SplitBox>
    *   </ListCtrlHeader>
    * 
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

    /** ��ȡ�п�Ⱥ����������
    * @param [in] columnId �е�IDֵ��ͨ��ListCtrlHeaderItem::GetColomnId()������ȡ
    * @param [out] columnIndex �е���ţ�[0, GetColumnCount())������ڼ���
    * @param [out] nColumnWidth �еĿ��ֵ
    */
    bool GetColumnInfo(size_t columnId, size_t& columnIndex, int32_t& nColumnWidth) const;

    /** ��ȡ�е��������
    * @param [in] columnId �е�IDֵ��ͨ��ListCtrlHeaderItem::GetColomnId()������ȡ
    * @return �е���ţ�[0, GetColumnCount())������ڼ���
    */
    size_t GetColumnIndex(size_t columnId) const;

    /** ��ȡ�е��������
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    * @return �е�ID�����ƥ�䲻�����򷵻�Box::InvalidIndex
    */
    size_t GetColumnId(size_t columnIndex) const;

    /** ɾ��һ��
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    bool DeleteColumn(size_t columnIndex);

public:

    /** ���ù�����ListCtrl�ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

protected:
    /** �Ƿ�֧���б�ͷ�϶��ı��е�˳��
    */
    bool IsEnableHeaderDragOrder() const;

    /** �϶��б�ͷ�ı��п���¼���Ӧ����
    * @param [in] pLeftHeaderItem �����б�ͷ�ؼ��ӿ�
    * @param [in] pRightHeaderItem �Ҳ���б�ͷ�ؼ��ӿ�
    */
    void OnHeaderColumnResized(Control* pLeftHeaderItem, Control* pRightHeaderItem);

    /** ����б�ͷ����������¼���Ӧ����
    * @param [in] pHeaderItem �б�ͷ�ؼ��ӿ�
    */
    void OnHeaderColumnSorted(ListCtrlHeaderItem* pHeaderItem);

    /** ͨ���϶��б�ͷ���������е�˳��
    */
    void OnHeaderColumnOrderChanged();

    /** CheckBox�Ĺ�ѡ�����
    * @param [in] pHeaderItem �б�ͷ�ؼ��ӿ�
    * @param [in] bChecked true��ʾ��ѡ��Checked״̬����false��ʾȡ����ѡ��UnChecked״̬��
    */
    void OnHeaderColumnCheckStateChanged(ListCtrlHeaderItem* pHeaderItem, bool bChecked);

    /** ��ͷ�е���ʾ���Է����仯
    */
    void OnHeaderColumnVisibleChanged();

private:
    /** ������ListCtrl�ӿ�
    */
    ListCtrl* m_pListCtrl;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_HEADER_H_
