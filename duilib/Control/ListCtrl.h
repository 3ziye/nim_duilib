#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/Button.h"
#include "duilib/Control/CheckBox.h"
#include "duilib/Control/Split.h"
#include "duilib/Box/ListBoxItem.h"
#include "duilib/Image/Image.h"

namespace ui
{

//�б�������UI��ʾ�����
class ListCtrlItem;
class ListCtrlHeader;
class ListCtrlHeaderItem;

/** �б�������ʾUI�ؼ�
*/
class ListCtrlDataView;

/** �б����ݹ�������
*/
class ListCtrlData;

/** �еĻ�����Ϣ, ���������
*/
struct ListCtrlColumn
{
    std::wstring text;              //��ͷ���ı�
    int32_t nColumnWidth = 100;     //�п����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMin = 0;    //�п���Сֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMax = 0;    //�п����ֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    bool bSortable = true;          //�Ƿ�֧������
    bool bResizeable = true;        //�Ƿ�֧��ͨ���϶������п�
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox��֧���ڱ�ͷ����������ʾCheckBox��
    int32_t nCheckBoxWidth = 24;    //CheckBox�ؼ���ռ�Ŀ�ȣ����������ı�ƫ������������ʾ�ص�������bShowCheckBoxΪtrueʱ��Ч, ���bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    bool bNeedDpiScale = true;      //�Ƿ����ֵ��DPI����Ӧ
};

/** �б�������Ļ�����Ϣ
*/
struct ListCtrlDataItem
{
    size_t nColumnIndex = 0;        //������ڼ��У���Ч��Χ��[0, GetColumnCount())
    std::wstring text;              //�ı�����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    int32_t nTextLeftPadding = 0;   //�ı������Padding��ֵ�����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����   
    int32_t nImageIndex = -1;       //ͼ����Դ�����ţ���ͼƬ�б�������±�ֵ�����Ϊ-1��ʾ����ʾͼ��
    UiColor textColor;              //�ı���ɫ
    UiColor bkColor;                //������ɫ
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox
    int32_t nCheckBoxWidth = 24;    //CheckBox�ؼ���ռ�Ŀ�ȣ�����bShowCheckBoxΪtrueʱ��Ч, ���bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    bool bNeedDpiScale = true;      //�Ƿ����ֵ��DPI����Ӧ
};

/** ListCtrl�ؼ�
*/
class ListCtrl: public VBox
{
    friend class ListCtrlHeader;
    friend class ListCtrlData;
    friend class ListCtrlDataView;
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
    /** ��ȡ�������ܸ���
    */
    size_t GetDataItemCount() const;

    /** �����������ܸ���
    * @param [in] itemCount �����������������ÿ������������ݣ�ͨ���ص��ķ�ʽ������䣨�ڲ�Ϊ���ʵ�֣�
    */
    bool SetDataItemCount(size_t itemCount);

    /** ��������һ��������
    * @param [in] dataItem �����������
    * @return �ɹ��������������������(rowIndex)��ʧ���򷵻�Box::InvalidIndex
    */
    size_t AddDataItem(const ListCtrlDataItem& dataItem);

    /** ��ָ����λ�����һ��������
    * @param [in] itemIndex �������������
    * @param [in] dataItem �����������
    */
    bool InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ����ָ���е�������
    * @param [in] itemIndex �������������
    * @param [in] dataItem �����������
    */
    bool SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** ɾ��ָ���е�������
    * @param [in] itemIndex �������������
    */
    bool DeleteDataItem(size_t itemIndex);

    /** ������������Զ�������
    * @param [in] itemIndex �������������
    * @param [in] itemData ������������Զ�������
    */
    bool SetDataItemData(size_t itemIndex, size_t itemData);

    /** ��ȡ��������Զ�������
    * @param [in] itemIndex �������������
    * @return ����������������Զ�������
    */
    size_t GetDataItemData(size_t itemIndex) const;

    /** ����ָ����������ı�
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] text ��Ҫ���õ��ı�����
    */
    bool SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text);

    /** ��ȡָ����������ı�
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @return ������������ı�����
    */
    std::wstring GetDataItemText(size_t itemIndex, size_t columnIndex) const;

    /** ����ָ����������ı���ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] textColor ��Ҫ���õ��ı���ɫ
    */
    bool SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor);

    /** ��ȡָ����������ı���ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] textColor ������������ı���ɫ
    */
    bool GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const;

    /** ����ָ��������ı�����ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [in] bkColor ��Ҫ���õı�����ɫ
    */
    bool SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor);

    /** ��ȡָ��������ı�����ɫ
    * @param [in] itemIndex �������������
    * @param [in] columnIndex �е������ţ���Ч��Χ��[0, GetColumnCount())
    * @param [out] bkColor ����������ı�����ɫ
    */
    bool GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const;

protected:
    /** �ؼ���ʼ��
    */
    virtual void DoInit() override;

    /** ����ListCtrlHeader������Class
    */
    void SetHeaderClass(const std::wstring& className);

    /** ListCtrlHeaderItem������Class
    */
    void SetHeaderItemClass(const std::wstring& className);
    std::wstring GetHeaderItemClass() const;

    /** ListCtrlHeader/SplitBox������Class
    */
    void SetHeaderSplitBoxClass(const std::wstring& className);
    std::wstring GetHeaderSplitBoxClass() const;

    /** ListCtrlHeader/SplitBox/Control������Class
    */
    void SetHeaderSplitControlClass(const std::wstring& className);
    std::wstring GetHeaderSplitControlClass() const;

    /** CheckBox��Class����(Ӧ����Header��ListCtrl����)
    */
    void SetCheckBoxClass(const std::wstring& className);
    std::wstring GetCheckBoxClass() const;

    /** ListCtrlItem��Class����
    */
    void SetDataItemClass(const std::wstring& className);
    std::wstring GetDataItemClass() const;

protected:
    /** ����һ��
    * @param [in] nColumnId �е�ID
    */
    void OnHeaderColumnAdded(size_t nColumnId);

    /** ɾ��һ��
    * @param [in] nColumnId �е�ID
    */
    void OnHeaderColumnRemoved(size_t nColumnId);

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

    /** ��ͷ��CheckBox��ѡ����
    * @param [in] nColumnId �е�ID
    * @param [in] bChecked true��ʾ��ѡ��Checked״̬����false��ʾȡ����ѡ��UnChecked״̬��
    */
    void OnHeaderColumnCheckStateChanged(size_t nColumnId, bool bChecked);

    /** ͬ��UI��Check״̬
    */
    void UpdateControlCheckStatus(size_t nColumnId);

private:
	/** ��ʼ����־
	*/
	bool m_bInited;

	/** ��ͷ�ؼ�
	*/
	ListCtrlHeader* m_pHeaderCtrl;

	/** �б�����չʾ
	*/
    ListCtrlDataView* m_pDataView;

	/** �б����ݹ���
	*/
    ListCtrlData* m_pListData;

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

    /** CheckBox��Class
    */
    UiString m_checkBoxClass;

    /** �Ƿ�֧���϶��ı��е�˳��
    */
    bool m_bEnableHeaderDragOrder;

    /** ListCtrlItem��Class����
    */
    UiString m_dataItemClass;

    /** ��ǰ�Ƿ���Ը���Header��Check״̬
    */
    bool m_bCanUpdateHeaderCheckStatus;
};

/** ListCtrl����ؼ�
*/
class ListCtrlItem : public ListBoxItemH
{
public:
    /** ��ȡ�ؼ�����
    */
    virtual std::wstring GetType() const override { return L"ListCtrlItem"; }

};

/** ListCtrl�ı�ͷ�ؼ�����ʾ��
*/
class ListCtrlHeaderItem:
    public ButtonBox
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

public:
    /** �����Ƿ���ʾCheckBox
    * @param [in] bVisible true��ʾ��ʾ��false��ʾ����
    */
    bool SetCheckBoxVisible(bool bVisible);

    /** �жϵ�ǰCheckBox�Ƿ�����ʾ״̬
    @return ����true��ʾCheckBox���ڣ����ҿɼ��� �������CheckBox������false
    */
    bool IsCheckBoxVisible() const;

    /** �Ƿ���CheckBox
    */
    bool HasCheckBox() const;

    /** ����CheckBox�Ĺ�ѡ״̬
    * @param [in] bSelected true��ʾ��ѡ��false��ʾ����ѡ
    * @param [in] ���bSelected��bPartSelectͬʱΪtrue����ʾ����ѡ��
    */
    bool SetCheckBoxSelect(bool bSelected, bool bPartSelect);

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

private:
    /** ������ListCtrl�ӿ�
    */
    ListCtrl* m_pListCtrl;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
