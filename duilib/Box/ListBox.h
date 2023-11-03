#ifndef UI_BOX_LISTBOX_H_
#define UI_BOX_LISTBOX_H_

#pragma once

#include "duilib/Box/ListBoxItem.h"
#include "duilib/Box/ScrollBox.h"
#include "duilib/Control/Option.h"

namespace ui 
{

/** �û��Զ����������
@param [in] pControl1 ��һ���ؼ��ӿ�
@param [in] pControl2 �ڶ����ؼ��ӿ�
@param [in] pCompareContext �û��Զ�������������
@return < 0 �ؼ�1С�ڿؼ�2
        = 0 �ؼ�1���ڿؼ�2
		> 0 �ؼ�1���ڿؼ�2
*/
typedef int (CALLBACK *PFNCompareFunc)(Control* pControl1, Control* pControl2, void* pCompareContext);

/** ListBox�����߽ӿ�
*/
class UILIB_API IListBoxOwner
{
public:
	/** �����¼�
	*/
	virtual void SendEvent(const EventArgs& event) = 0;

	/** ��ȡ��ǰѡ���������(�������Ч�������򷵻�Box::InvalidIndex)
	*/
	virtual size_t GetCurSel() const = 0;

	/** ���õ�ǰѡ�������
	* @param [in] iIndex ����Ŀ��ID
	*/
	virtual void SetCurSel(size_t iIndex) = 0;

	/** ѡ������
	*  @param [in] iIndex ����Ŀ��ID
	*  @param [in] bTakeFocus �Ƿ�������ؼ���Ϊ����ؼ�
	*  @param [in] bTriggerEvent �Ƿ񴥷�ѡ���¼�, ���Ϊtrue���ᴥ��һ��kEventSelect�¼�
	*/
	virtual bool SelectItem(size_t iIndex, bool bTakeFocus, bool bTriggerEvent) = 0;

	/** ȡ��ѡ������
	*  @param [in] iIndex ����Ŀ��ID
	*  @param [in] bTriggerEvent �Ƿ񴥷�ѡ���¼�, ���Ϊtrue���ᴥ��һ��kEventUnSelect�¼�
	*/
	virtual bool UnSelectItem(size_t iIndex, bool bTriggerEvent) = 0;

	/** ȷ����������ɼ�
	*/
	virtual void EnsureVisible(const UiRect& rcItem) = 0;

	/** ֹͣ����������
	*/
	virtual void StopScroll() = 0;

	/** �Ƿ����ѡ��״̬�µı���ɫ���ṩ�麯����Ϊ��ѡ��
	   ������ListBox/TreeView�ڵ��ڶ�ѡʱ�������й�ѡ�������Ҫ����ѡ��״̬�ı���ɫ��
	   @param [in] bHasStateImages ��ǰ�б����Ƿ���CheckBox��ѡ��
	*/
	virtual bool CanPaintSelectedColors(bool bHasStateImages) const = 0;

	/** �Ƿ������ѡ
	*/
	virtual bool IsMultiSelect() const = 0;

	/** ѡ���������¼�����ѡʱ���ڱ�ֻ֤��һ��ѡ����
	*/
	virtual void EnsureSingleSelection() = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/** �б�����������չʾһ������ʹ��
*   ͨ���޸Ĳ��֣��γ� HListBox��VListBox��TileListBox��������
*/
class UILIB_API ListBox : public ScrollBox, public IListBoxOwner
{
public:
	explicit ListBox(Layout* pLayout);
	ListBox(const ListBox& r) = delete;
	ListBox& operator=(const ListBox& r) = delete;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void HandleEvent(const EventArgs& msg) override;
	virtual bool ButtonDown(const EventArgs& msg) override;
	virtual void SendEvent(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0, TCHAR tChar = 0,
						   const UiPoint& mousePos = UiPoint()) override;
	virtual void SendEvent(const EventArgs& event) override;

public:
	/** ��ȡ��ǰѡ���������(�������Ч�������򷵻�Box::InvalidIndex)
	*/
	virtual size_t GetCurSel() const override;

	/** ���õ�ǰѡ�������
	* @param [in] iIndex ����Ŀ��ID
	*/
	virtual void SetCurSel(size_t iIndex) override;

	/** ѡ������
	*  @param [in] iIndex ����Ŀ��ID����Χ�ǣ�[0, GetItemCount())
	*  @param [in] bTakeFocus �Ƿ�������ؼ���Ϊ����ؼ�
	*  @param [in] bTriggerEvent �Ƿ񴥷�ѡ���¼�, ���Ϊtrue���ᴥ��һ��kEventSelect�¼�
	*/
	virtual bool SelectItem(size_t iIndex, bool bTakeFocus, bool bTriggerEvent) override;

	/** ȡ��ѡ������
	*  @param [in] iIndex ����Ŀ��ID
	*  @param [in] bTriggerEvent �Ƿ񴥷�ѡ���¼�, ���Ϊtrue���ᴥ��һ��kEventUnSelect�¼�
	*/
	virtual bool UnSelectItem(size_t iIndex, bool bTriggerEvent) override;
	
	/** ȷ����������ɼ�
	*/
	virtual void EnsureVisible(const UiRect& rcItem) override;

	/** ֹͣ����������
	*/
	virtual void StopScroll() override;

	/** �Ƿ����ѡ��״̬�µı���ɫ���ṩ�麯����Ϊ��ѡ��
	   ������ListBox/TreeView�ڵ��ڶ�ѡʱ�������й�ѡ�������Ҫ����ѡ��״̬�ı���ɫ��
	   @param [in] bHasStateImages ��ǰ�б����Ƿ���CheckBox��ѡ��
	*/
	virtual bool CanPaintSelectedColors(bool bHasStateImages) const override;

public:
	/** ���������λ������
	 * @param [in] pControl ����ָ��
	 * @param [in] iIndex �����ţ���Χ�ǣ�[0, GetItemCount())
	 */
	virtual bool SetItemIndex(Control* pControl, size_t iIndex) override;

	/** ׷��һ�����ĩβ
	 * @param [in] pControl ����ָ��
	 */
	virtual bool AddItem(Control* pControl) override;

	/** ��ָ��λ��֮�����һ������
	 * @param [in] pControl ����ָ��
	 * @param[in] iIndex Ҫ�����λ����������Χ�ǣ�[0, GetItemCount())
	 */
    virtual bool AddItemAt(Control* pControl, size_t  iIndex) override;

	/** ��������ָ��
	 * @param [in] pControl ����ָ��
	 */
    virtual bool RemoveItem(Control* pControl) override;

	/** ���������Ƴ�һ������
	 * @param [in] iIndex ������������Χ�ǣ�[0, GetItemCount())
	 */
    virtual bool RemoveItemAt(size_t iIndex) override;

	/** �Ƴ���������
	 */
    virtual void RemoveAllItems() override;

public:
	/** ȷ������ɼ�
	* @param [in] iIndex ������������Χ�ǣ�[0, GetItemCount())
	*/
	void EnsureVisible(size_t iIndex);

	/** ȷ������ɼ�
	* @param [in] iIndex ������������Χ�ǣ�[0, GetItemCount())
	* @param [in] bAtCenter ȷ����������ʾ�ڿɼ�������м�
	*/
	void EnsureVisible(size_t iIndex, bool bAtCenter);

	/** ѡ�����ѡ�к�������ؼ���Ϊ����ؼ���������һ��kEventSelect�¼�
	*   Ϊ���η�װ�������൱�ڣ�SelectItem(iIndex, true, true);
	*  @param [in] iIndex ����Ŀ��ID����Χ�ǣ�[0, GetItemCount())
	*/
	bool SelectItem(size_t iIndex);

	/** ѡ����һ��
	*/
	void SelectPreviousItem();

	/** ѡ����һ��
	 */
	void SelectNextItem();

	/** ��ȡ��ǰѡ�������
	* @param [out] selectedIndexs ����ѡ�������ID������Χ�ǣ�[0, GetItemCount())
	*/
	void GetSelectedItems(std::vector<size_t>& selectedIndexs) const;

	/** ������ָ������λ��
	 * @param [in] iIndex ����Ŀ��ID����Χ�ǣ�[0, GetItemCount())
	 */
	bool ScrollItemToTop(size_t iIndex);

	/** ������ָ������λ��
	 * @param [in] itemName ��������(����Control::GetName())
	 */
	bool ScrollItemToTop(const std::wstring& itemName);

	/** ��ȡ��ǰ��������(Control::GetPos())�еĵ�һ������
	 */
	Control* GetTopItem() const;

public:
	/** �Ƿ������ѡ
	*/
	virtual bool IsMultiSelect() const override;

	/** �����Ƿ������ѡ
	*/
	virtual void SetMultiSelect(bool bMultiSelect);

	/** ѡ���������¼�����ѡʱ���ڱ�ֻ֤��һ��ѡ����
	*/
	virtual void EnsureSingleSelection() override;

	/** ��ȡ�Ƿ�������ı�ѡ��������
	 * @return ���� true ��ʾ����������ı�ѡ�������Ϊ false
	 */
	bool GetScrollSelect() const;

	/** �����Ƿ�������ı�ѡ��������
	 * @param[in] bScrollSelect Ϊ true ��Ϊ����������ı�ѡ���false Ϊ������
	 */
	void SetScrollSelect(bool bScrollSelect);

	/** ���Ƴ�һ�������, ������Ƴ�����ѡ������Զ�ѡ����һ��
	 * @param[in] bSelectNextItem Ϊ true ʱ�Զ�ѡ����һ�false Ϊ���Զ�ѡ��
	 */
	void SetSelectNextWhenActiveRemoved(bool bSelectNextItem);

	/** ����������
	 * @param [in] pfnCompare �Զ���������
	 * @param [in] pCompareContext ���ݸ��ȽϺ������û��Զ�������
	 */
	bool SortItems(PFNCompareFunc pfnCompare, void* pCompareContext);

	/** ����ѡ��������¼�
	* @param[in] callback ѡ������ʱ�Ļص�����
	*            ����˵��:
	                    wParam: ��ǰ��ѡ�������ID
						lParam: ԭ����ѡ�������ID������Ϊ��ЧֵBox::InvalidIndex
	*/
	void AttachSelect(const EventCallback& callback) { AttachEvent(kEventSelect, callback); }

	/** ����ѡ��������¼�
	* @param[in] callback ȡ��ѡ������ʱ�Ļص�����
	*            ����˵��:
	                    wParam: ȡ��ѡ�������ID
						lParam: ��ЧֵBox::InvalidIndex
	*/
	void AttachUnSelect(const EventCallback& callback) { AttachEvent(kEventUnSelect, callback); }

protected:
	/** ���Ӷ�ѡ�л�Ϊ��ѡģʽ��ʱ����Ҫȷ���б���ֻ��һ��ѡ����
	* @return ����б仯����true�����򷵻�false
	*/
	virtual bool OnSwitchToSingleSelect();

	/** ͬ����ǰѡ�����ѡ��״̬
	* @return ����б仯����true�����򷵻�false
	*/
	bool UpdateCurSelItemSelectStatus();

	/** ȷ����������ɼ�
	*/
	void EnsureVisible(const UiRect& rcItem, bool bAtCenter);

private:
	/**
	 * @brief Ĭ�ϵ�����Աȷ���
	 * @param[in] pvlocale ���� List ָ��
	 * @param[in] item1 ����1
	 * @param[in] item2 ����2
	 * @return ���ضԱȽ��
	 */
	static int __cdecl ItemComareFunc(void* pvlocale, const void* item1, const void* item2);
	int __cdecl ItemComareFunc(const void* item1, const void* item2);

	/** ѡ������(��ѡ)
	*  @param [in] iIndex ����Ŀ��ID����Χ�ǣ�[0, GetItemCount())
	*  @param [in] bTakeFocus �Ƿ�������ؼ���Ϊ����ؼ�
	*  @param [in] bTriggerEvent �Ƿ񴥷�ѡ���¼�, ���Ϊtrue���ᴥ��һ��kEventSelect�¼�
	*/
	bool SelectItemSingle(size_t iIndex, bool bTakeFocus, bool bTriggerEvent);

	/** ѡ������(��ѡ)
	*  @param [in] iIndex ����Ŀ��ID����Χ�ǣ�[0, GetItemCount())
	*  @param [in] bTakeFocus �Ƿ�������ؼ���Ϊ����ؼ�
	*  @param [in] bTriggerEvent �Ƿ񴥷�ѡ���¼�, ���Ϊtrue���ᴥ��һ��kEventSelect�¼�
	*/
	bool SelectItemMulti(size_t iIndex, bool bTakeFocus, bool bTriggerEvent);

	/** ��ȡ��ǰ���ַ���
	* @return ����true��ʾΪ���򲼾֣�����Ϊ���򲼾�
	*/
	bool IsHorizontalLayout() const;

private:
	//�Ƿ�������ı�ѡ����
	bool m_bScrollSelect;

	//���Ƴ�һ��������Զ�ѡ����һ��
	bool m_bSelNextWhenRemoveActive;

	//�Ƿ������ѡ��Ĭ��Ϊ��ѡ��
	bool m_bMultiSelect;

	//��ѡ��ʱ���Ƿ���ʾѡ�񱳾�ɫ: 0 - Ĭ�Ϲ���; 1 - ��ʾ����ɫ; 2: ����ʾ����ɫ
	uint8_t m_uPaintSelectedColors;

	//��ǰѡ�������ID, ����Ƕ�ѡ��ָ�����һ��ѡ����
	size_t m_iCurSel;

	//�û��Զ��������ȽϺ���
	PFNCompareFunc m_pCompareFunc;

	//�û��Զ��������ȽϺ����е�����������
	void* m_pCompareContext;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/** ���򲼾ֵ�ListBox
*/
class UILIB_API HListBox : public ListBox
{
public:
	HListBox() :
		ListBox(new HLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_HLISTBOX; }
};

/** ���򲼾ֵ�ListBox
*/
class UILIB_API VListBox : public ListBox
{
public:
	VListBox() :
		ListBox(new VLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_VLISTBOX; }
};

/** ��Ƭ���ֵ�ListBox(���򲼾�)
*/
class UILIB_API HTileListBox : public ListBox
{
public:
	HTileListBox() :
		ListBox(new HTileLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_HTILE_LISTBOX; }
};

/** ��Ƭ���ֵ�ListBox(���򲼾�)
*/
class UILIB_API VTileListBox : public ListBox
{
public:
	VTileListBox() :
		ListBox(new VTileLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_VTILE_LISTBOX; }
};

} // namespace ui

#endif // UI_BOX_LISTBOX_H_
