#ifndef UI_BOX_LISTBOX_H_
#define UI_BOX_LISTBOX_H_

#pragma once

#include "duilib/Box/ScrollBox.h"
#include "duilib/Box/HBox.h"
#include "duilib/Box/VBox.h"
#include "duilib/Box/TileBox.h"
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
	/**@brief �����¼�
	*/
	virtual void SendEvent(const EventArgs& event) = 0;

	/**@brief ��ȡ��ǰѡ���������(�������Ч�������򷵻�Box::InvalidIndex)
	*/
	virtual size_t GetCurSel() const = 0;

	/**@brief ѡ������
	*  @param [in] iIndex ����Ŀ��ID
	*  @param [in] bTakeFocus �Ƿ�������ؼ���Ϊ����ؼ�
	*  @param [in] bTrigger �Ƿ񴥷�ѡ���¼� 
	*/
	virtual bool SelectItem(size_t iIndex, bool bTakeFocus = false, bool bTrigger = true) = 0;

	/**@brief ȷ������ɼ�
	*/
	virtual void EnsureVisible(const UiRect& rcItem) = 0;

	/**@brief ֹͣ����������
	*/
	virtual void StopScroll() = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/** �б�����������չʾһ������ʹ��
*   ͨ���޸Ĳ��֣��γ� HListBox��VListBox��TileListBox��������
*/
class UILIB_API ListBox : public ScrollBox, public IListBoxOwner
{
public:
	/** Ĭ��Ϊ��ֱ���ֵ�ListBox
	*/
	explicit ListBox(Layout* pLayout = new VLayout);
	ListBox(const ListBox& r) = delete;
	ListBox& operator=(const ListBox& r) = delete;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void HandleEvent(const EventArgs& event) override;
	virtual bool ButtonDown(const EventArgs& msg) override;
	virtual void SendEvent(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0, TCHAR tChar = 0,
						   const UiPoint& mousePos = UiPoint()) override;
	virtual void SendEvent(const EventArgs& event) override;

	/**@brief ��ȡ��ǰѡ���������(�������Ч�������򷵻�Box::InvalidIndex)
	*/
	virtual size_t GetCurSel() const override;
	virtual bool SelectItem(size_t iIndex, bool bTakeFocus = false, bool bTrigger = true) override;
	virtual void EnsureVisible(const UiRect& rcItem) override;
	virtual void StopScroll() override;
	
	/**@brief ������ָ������λ��
	 * @param[in] strItemName ��������
	 */
	virtual bool ScrollItemToTop(const std::wstring& strItemName);

	/**@brief ��ȡ��ǰλ�õ�һ������
	 */
	virtual Control* GetTopItem();
	
	/**@brief ���������λ������
	 * @param[in] pControl ����ָ��
	 * @param[in] iIndex ������
	 */
	virtual bool SetItemIndex(Control* pControl, size_t iIndex) override;

	/**@brief ׷��һ�����ĩβ
	 * @param[in] pControl ����ָ��
	 */
	virtual bool AddItem(Control* pControl) override;

	/**@brief ��ָ��λ��֮�����һ������
	 * @param[in] pControl ����ָ��
	 * @param[in] iIndex Ҫ�����λ������
	 */
    virtual bool AddItemAt(Control* pControl, size_t  iIndex) override;

	/**@brief ��������ָ��
	 * @param[in] pControl ����ָ��
	 */
    virtual bool RemoveItem(Control* pControl) override;

	/**@brief ���������Ƴ�һ������
	 * @param[in] iIndex ��������
	 */
    virtual bool RemoveItemAt(size_t  iIndex) override;

	/**@brief �Ƴ���������
	 */
    virtual void RemoveAllItems() override;

	/**@brief ѡ����һ��
	*/
	void SelectPreviousItem();

	/**@brief ѡ����һ��
	 */
	void SelectNextItem();

	/**@brief ��������
	 * @param [in] pfnCompare �Զ���������
	 * @param [in] pCompareContext ���ݸ��ȽϺ������û��Զ�������
	 */
	bool SortItems(PFNCompareFunc pfnCompare, void* pCompareContext);

	/**@brief ��ȡ�Ƿ�������ı�ѡ��������
	 * @return ���� true ��ʾ����������ı�ѡ�������Ϊ false
	 */
	bool GetScrollSelect() const;

	/**@brief �����Ƿ�������ı�ѡ��������
	 * @param[in] bScrollSelect Ϊ true ��Ϊ����������ı�ѡ���false Ϊ������
	 */
	void SetScrollSelect(bool bScrollSelect);

	/**@brief ����ѡ��������¼�
	 * @param[in] callback ѡ������ʱ�Ļص�����
	 */
	void AttachSelect(const EventCallback& callback) { AttachEvent(kEventSelect, callback); }

	/**@brief ���Ƴ�һ��������Զ�ѡ����һ��
	 * @param[in] bSelectNextItem Ϊ true ʱ�Զ�ѡ����һ�false Ϊ���Զ�ѡ��
	 */
	void SelectNextWhenActiveRemoved(bool bSelectNextItem);

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

protected:
	//�Ƿ�������ı�ѡ����
	bool m_bScrollSelect;

	//��ǰѡ�������ID
	size_t m_iCurSel;

	//���Ƴ�һ��������Զ�ѡ����һ��
	bool m_bSelNextWhenRemoveActive;

	//�û��Զ��������ȽϺ���
	PFNCompareFunc m_pCompareFunc;

	//�û��Զ��������ȽϺ����е�����������
	void* m_pCompareContext;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/// �б���������б���չʾ���ݵ�����
class UILIB_API ListBoxElement: 
	public OptionTemplate<Box>
{
public:
	ListBoxElement();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void Selected(bool bSelect, bool trigger) override;
	virtual void HandleEvent(const EventArgs& event) override;

	/** �жϿؼ������Ƿ�Ϊ��ѡ���
	 * @return ����true
	 */
	virtual bool IsSelectableType() const override;

	/**
	 * @brief ��ȡ������
	 * @return ���ظ�����ָ��
	 */
	IListBoxOwner* GetOwner();

	/**
	 * @brief ���ø�����
	 * @param[in] pOwner ������ָ��
	 * @return ��
	 */
	void SetOwner(IListBoxOwner* pOwner);

	/**
	 * @brief ��ȡ��ǰ����
	 * @return ���ص�ǰ����
	 */
	size_t GetIndex() const;

	/**
	 * @brief ��������
	 * @param[in] iIndex ����ֵ
	 * @return ��
	 */
	void SetIndex(size_t iIndex);

	/**
	 * @brief �����ؼ�˫���¼�
	 * @param[in] callback �յ�˫����Ϣʱ�Ļص�����
	 * @return ��
	 */
	void AttachDoubleClick(const EventCallback& callback) { AttachEvent(kEventMouseDoubleClick, callback); }

	/**
	 * @brief �����س��¼�
	 * @param[in] callback �յ��س�ʱ�Ļص�����
	 * @return ��
	 */
	void AttachReturn(const EventCallback& callback) { AttachEvent(kEventReturn, callback); }

private:
	/** ��ListBox�����е�����������
	*/
	size_t m_iIndex;

	/** ��ListBox�����ӿ�
	*/
	IListBoxOwner* m_pOwner;
};

/** ��Ƭ���ֵ�ListBox
*/
class UILIB_API TileListBox : public ListBox
{
public:
	TileListBox() :
		ListBox(new TileLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_TILELISTBOX; }
};

/** ˮƽ���ֵ�ListBox
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

/** ��ֱ���ֵ�ListBox
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

} // namespace ui

#endif // UI_BOX_LISTBOX_H_
