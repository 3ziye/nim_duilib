#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/Label.h"
#include "duilib/Control/Split.h"
#include "duilib/Box/ListBoxItem.h"
#include "duilib/Image/Image.h"

namespace ui
{

//�б�������UI��ʾ�����
class ListCtrlItem;
class ListCtrlHeader;
class ListCtrlHeaderItem;

//�б����ݹ�����
class ListCtrlData;
class ListCtrlItemProvider;

/** ListCtrl�ؼ�
*/
class ListCtrl: public VBox
{
    friend class ListCtrlHeader;
public:
	ListCtrl();
	virtual ~ListCtrl();

	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

public:
    /** ��ָ��λ�����һ��
    * @param [in] nCol �ڵڼ����Ժ������У������-1����ʾ�����׷��һ��
    * @param [in] nColumnWidth �п�
    * @param [in] text ��ͷ���ı�
    * @param [in] bSortable �Ƿ���������
    * @param [in] bResizeable �Ƿ�����ͨ���϶������п�
    * @param [in] bNeedDpiScale �Ƿ���п�nColumnWidthֵ��DPI����Ӧ
    * @return ������һ�еı�ͷ�ؼ��ӿ�
    */
    ListCtrlHeaderItem* InsertColumn(int32_t nCol, int32_t nColumnWidth, const std::wstring& text,
                                     bool bSortable = false, bool bResizeable = false, 
                                     bool bNeedDpiScale = true);

    /** ��ȡ��ͷ�ؼ��ӿ�, �ڿؼ���ʼ���Ժ����ֵ
    */
    ListCtrlHeader* GetListCtrlHeader() const;

protected:
    /** �ؼ���ʼ��
    */
    virtual void DoInit() override;

protected:
    /** �����еĿ�ȣ��϶��п������ÿ�ε��������еĿ�ȣ�
    * @param [in] nColumnId1 ��һ�е�ID
    * @param [in] nColumnId2 �ڶ��е�ID
    */
    void OnColumnWidthChanged(size_t nColumnId1, size_t nColumnId2);

    /** ��������
    * @param [in] nColumnId �е�ID
    * @param [in] bSortedUp ���Ϊtrue��ʾ�������Ϊfalse��ʾ����
    */
    void OnColumnSorted(size_t nColumnId, bool bSortedUp);

private:
	/** ��ʼ����־
	*/
	bool m_bInited;

	/** ��ͷ�ؼ�
	*/
	ListCtrlHeader* m_pListCtrlHeader;

	/** �б�����չʾ
	*/
	ListCtrlData* m_pListCtrlData;

	/** �б����ݹ���
	*/
	std::unique_ptr<ListCtrlItemProvider> m_spItemProvider;
};

/** ListCtrl����ؼ�
*/
class ListCtrlItem : public ListBoxItem
{
public:
    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override { return L"ListCtrlItem"; }

};

/** ListCtrl�ı�ͷ�ؼ�����ʾ��
*/
class ListCtrlHeaderItem:
    public Button
{
public:
    ListCtrlHeaderItem();
    virtual ~ListCtrlHeaderItem();

    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override;

    /** ��������
    */
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

    /** ��������
    */
    virtual void PaintText(IRender* pRender) override;

    /** ��ť����¼�
    */
    virtual void Activate() override;

public:
    /** ����ʽ
    */
    enum class SortMode
    {
        kNone, //��֧������
        kUp,   //����
        kDown  //����
    };

    /** ��������ʽ
    */
    void SetSortMode(SortMode sortMode);

    /** ��ȡ����ʽ
    */
    SortMode GetSortMode() const;

    /** ��������ͼ�꣺����
    */
    void SetSortedDownImage(const std::wstring& sImageString);

    /** ��������ͼ�꣺����
    */
    void SetSortedUpImage(const std::wstring& sImageString);

    /** ��ȡΨһID
    */
    size_t GetColomnId() const;

    /** ���ù�����Split�ؼ��ӿ�
    */
    void SetSplitBox(SplitBox* pSplitBox);

    /** ��ȡ������Split�ؼ��ӿ�
    */
    SplitBox* GetSplitBox() const;

    /** �����Ƿ���������п�
    */
    void SetColumnResizeable(bool bResizeable);

    /** ��ȡ�Ƿ���������п�
    */
    bool IsColumnResizeable() const;

    /** �����п�
    * @param [in] nWidth �п�ֵ
    * @param [in] bNeedDpiScale �Ƿ���Ҫ���п�ֵ����DPI����Ӧ
    */
    void SetColumnWidth(int32_t nWidth, bool bNeedDpiScale);

    /** ��ȡ�п�ֵ
    */
    int32_t GetColumnWidth() const;

private:
    /** ͬ���п���UI�ؼ����
    */
    void CheckColumnWidth();

private:
    /** ����ͼ�꣺����
    */
    Image* m_pSortedDownImage;

    /** ����ͼ�꣺����
    */
    Image* m_pSortedUpImage;

    /** ������Split�ؼ��ӿ�
    */
    SplitBox* m_pSplitBox;

    /** ����ʽ
    */
    SortMode m_sortMode;

    /** �п�
    */
    int32_t m_nColumnWidth;

    /** �Ƿ�����ı��п�
    */
    bool m_bColumnResizeable;
};

/** ListCtrl�ı�ͷ�ؼ�
*/
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
    *   <ListCtrlHeader>
    *       <ListCtrlHeaderItem/>
    *       <SplitBox> <Control/> </SplitBox>
    *       ..
    *       <ListCtrlHeaderItem/>
    *       <SplitBox> <Control/> </SplitBox>
    *   </ListCtrlHeader>
    * @param [in] nCol �ڵڼ����Ժ������У������-1����ʾ�����׷��һ��
    * @param [in] nColumnWidth �п�
    * @param [in] text ��ͷ���ı�
    * @param [in] bSortable �Ƿ���������
    * @param [in] bResizeable �Ƿ�����ͨ���϶������п�
    * @param [in] bNeedDpiScale �Ƿ���п�nColumnWidthֵ��DPI����Ӧ
    * @return ������һ�еı�ͷ�ؼ��ӿ�
    */
    ListCtrlHeaderItem* InsertColumn(int32_t nCol, int32_t nColumnWidth, const std::wstring& text,
                                     bool bSortable = false, bool bResizeable = false, 
                                     bool bNeedDpiScale = true);

    /** ��ȡ�еĸ���
    */
    size_t GetColumnCount() const;

    /** ��ȡ�п��
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    int32_t GetColumnWidth(size_t columnIndex) const;

    /** ��ȡ�п�Ⱥ����������
    * @param [in] columnId �е�IDֵ��ͨ��ListCtrlHeaderItem::GetColomnId()������ȡ
    * @param [out] columnIndex �е���ţ�[0, GetColumnCount())������ڼ���
    * @param [out] nColumnWidth �еĿ��ֵ
    */
    bool GetColumnInfo(size_t columnId, size_t& columnIndex, int32_t& nColumnWidth) const;

public:

    /** ���ù�����ListCtrl�ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

protected:
    /** �϶��б�ͷ�ı��п���¼���Ӧ����
    * @param [in] pLeftHeaderItem �����б�ͷ�ؼ��ӿ�
    * @param [in] pRightHeaderItem �Ҳ���б�ͷ�ؼ��ӿ�
    */
    void OnHeaderColumnResized(Control* pLeftHeaderItem, Control* pRightHeaderItem);

    /** ����б�ͷ����������¼���Ӧ����
    * @param [in] pHeaderItem �б�ͷ�ؼ��ӿ�
    */
    void OnHeaderColumnSorted(ListCtrlHeaderItem* pHeaderItem);

private:
    /** ������ListCtrl�ӿ�
    */
    ListCtrl* m_pListCtrl;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
