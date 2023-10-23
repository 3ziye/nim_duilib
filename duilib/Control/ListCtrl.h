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
    * @param [in] columnIndex �ڵڼ����Ժ������У������-1����ʾ�����׷��һ��
    * @param [in] nColumnWidth �п�
    * @param [in] text ��ͷ���ı�
    * @param [in] bSortable �Ƿ���������
    * @param [in] bResizeable �Ƿ�����ͨ���϶������п�
    * @param [in] bNeedDpiScale �Ƿ���п�nColumnWidthֵ��DPI����Ӧ
    * @return ������һ�еı�ͷ�ؼ��ӿ�
    */
    ListCtrlHeaderItem* InsertColumn(int32_t columnIndex, int32_t nColumnWidth, const std::wstring& text,
                                     bool bSortable = false, bool bResizeable = false, 
                                     bool bNeedDpiScale = true);

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

    /** ��ȡ�е��������
    * @param [in] columnId �е�IDֵ��ͨ��ListCtrlHeaderItem::GetColomnId()������ȡ
    * @return �е���ţ�[0, GetColumnCount())������ڼ���
    */
    size_t GetColumnIndex(size_t columnId) const;

    /** ɾ��һ��
    * @param [in] columnIndex ��������ţ�[0, GetColumnCount())
    */
    bool DeleteColumn(size_t columnIndex);

    /** ��ȡ��ͷ�ؼ��ӿ�, �ڿؼ���ʼ���Ժ����ֵ
    */
    ListCtrlHeader* GetListCtrlHeader() const;

    /** �����Ƿ�֧���б�ͷ�϶��ı��е�˳��
    */
    void SetEnableHeaderDragOrder(bool bEnable);

    /** �Ƿ�֧���б�ͷ�϶��ı��е�˳��
    */
    bool IsEnableHeaderDragOrder() const;

public:


protected:
    /** �ؼ���ʼ��
    */
    virtual void DoInit() override;

    /** ����ListCtrlHeader������Class
    */
    void SetListCtrlHeaderClass(const std::wstring& className);

    /** ListCtrlHeaderItem������Class
    */
    void SetListCtrlHeaderItemClass(const std::wstring& className);
    std::wstring GetListCtrlHeaderItemClass() const;

    /** ListCtrlHeader/SplitBox������Class
    */
    void SetListCtrlHeaderSplitBoxClass(const std::wstring& className);
    std::wstring GetListCtrlHeaderSplitBoxClass() const;

    /** ListCtrlHeader/SplitBox/Control������Class
    */
    void SetListCtrlHeaderSplitControlClass(const std::wstring& className);
    std::wstring GetListCtrlHeaderSplitControlClass() const;

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

    /** ͨ���϶��б�ͷ���������е�˳��
    */
    void OnHeaderColumnOrderChanged();

    /** ɾ��һ��
    * @param [in] nColumnId �е�ID
    */
    void OnHeaderColumnDeleted(size_t nColumnId);

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

    /** ListCtrlHeader������Class
    */
    UiString m_headerClass;

    /** ListCtrlHeaderItem������Class
    */
    UiString m_headerItemClass;

    /** ListCtrlHeader/SplitBox������Class
    */
    UiString m_headerSplitBoxClass;

    /** ListCtrlHeader/SplitBox/Control������Class
    */
    UiString m_headerSplitControlClass;

    /** �Ƿ�֧���϶��ı��е�˳��
    */
    bool m_bEnableHeaderDragOrder;
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

    /** ����������ͼ��֮��ļ�������أ�
    */
    void SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale);

    /** ��ȡ������ͼ��֮��ļ�������أ�
    */
    int32_t GetIconSpacing() const;

private:
    /** ͬ���п���UI�ؼ����
    */
    void CheckColumnWidth();

protected:
    /** @name �϶���صĳ�Ա����
    * @{ */

    virtual bool ButtonDown(const EventArgs& msg) override;
    virtual bool ButtonUp(const EventArgs& msg) override;
    virtual bool MouseMove(const EventArgs& msg) override;
    virtual bool OnWindowKillFocus(const EventArgs& msg) override;//�ؼ������Ĵ���ʧȥ����

    /** �������λ�õ��������ؼ���λ��(�϶�������һ����)
    */
    void AdjustHeaderItemPos(const UiPoint& mousePt);

    /** ����϶�״̬
    */
    void ClearDragStatus();

    /** @} */

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

    /** ������ͼ��֮��ļ��
    */
    int32_t m_nIconSpacing;

private:
    /** @name �϶���صĳ�Ա����
    * @{ */

    /** �Ƿ�����������
    */
    bool m_bMouseDown;

    /** �Ƿ�����ק������
    */
    bool m_bInDragging;

    /** ԭ����͸����
    */
    uint8_t m_nOldAlpha;

    /** ��갴��ʱ�����λ��
    */
    UiPoint m_ptMouseDown;

    /** ��갴��ʱ�Ŀؼ���������
    */
    UiRect m_rcMouseDown;

    struct ItemStatus
    {
        Control* m_pItem = nullptr;
        UiRect m_rcPos;
        size_t m_index = Box::InvalidIndex;
    };

    /** ��갴��ʱ���������У�ÿ���ؼ���λ��
    */
    std::vector<ItemStatus> m_rcItemList;

    /** @} */
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
    * @param [in] columnIndex �ڵڼ����Ժ������У������-1����ʾ�����׷��һ��
    * @param [in] nColumnWidth �п�
    * @param [in] text ��ͷ���ı�
    * @param [in] bSortable �Ƿ���������
    * @param [in] bResizeable �Ƿ�����ͨ���϶������п�
    * @param [in] bNeedDpiScale �Ƿ���п�nColumnWidthֵ��DPI����Ӧ
    * @return ������һ�еı�ͷ�ؼ��ӿ�
    */
    ListCtrlHeaderItem* InsertColumn(int32_t columnIndex, int32_t nColumnWidth, const std::wstring& text,
                                     bool bSortable = false, bool bResizeable = false, 
                                     bool bNeedDpiScale = true);

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

private:
    /** ������ListCtrl�ӿ�
    */
    ListCtrl* m_pListCtrl;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
