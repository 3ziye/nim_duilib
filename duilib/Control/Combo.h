#ifndef UI_CONTROL_COMBO_H_
#define UI_CONTROL_COMBO_H_

#pragma once

#include "duilib/Core/Box.h"
#include "duilib/Control/TreeView.h"
#include "duilib/Control/RichEdit.h"

namespace ui 
{

class UILIB_API Combo : public Box
{
    friend class CComboWnd;
public:
	Combo();
	Combo(const Combo& r) = delete;
	Combo& operator=(const Combo& r) = delete;
	virtual ~Combo();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual bool CanPlaceCaptionBar() const override;
	virtual void DoInit() override;

public:
	/** Combo����
	*/
	enum ComboType
	{
		//�����б�, ���ɱ༭����
		kCombo_DropList = 0,

		//�༭�б��ɱ༭����
		kCombo_DropDown = 1
	};

	/** ����Combo����
	*/
	void SetComboType(ComboType comboType);

	/** ��ȡCombo����
	*/
	ComboType GetComboType() const;

	/** ��ȡ�������б��С(��Ⱥ͸߶�)
	*/
	UiSize GetDropBoxSize() const;

	/** �����������б��С(��Ⱥ͸߶�)
	 * @param [in] szDropBox Ҫ���õĴ�С��Ϣ
	 * @param [in] bNeedScaleDpi �Ƿ���Ҫ��DPI����Ӧ
	 */
	void SetDropBoxSize(UiSize szDropBox, bool bNeedScaleDpi);

	/** ���� Combobox �Ƿ����ϵ���
	 * @param[in] top Ϊ true �����ϵ�����false ΪĬ�����µ���
	 */
	void SetPopupTop(bool top) { m_bPopupTop = top; }

	/** �ж� Combobox ����ģʽ�Ƿ������ϵ���
	 * @return ���� true ��ʾ���ϵ���������Ϊ false
	 */
	bool IsPopupTop() const { return m_bPopupTop; }

	/** ����������TreeView��Class����
	*/
	void SetComboTreeClass(const std::wstring& classValue);

	/** ����������TreeView�Ľڵ�Class����
	*/
	void SetComboTreeNodeClass(const std::wstring& classValue);

	/** ������ʾͼ���Class����
	*/
	void SetIconControlClass(const std::wstring& classValue);

	/** ���ñ༭�ؼ���Class����
	*/
	void SetEditControlClass(const std::wstring& classValue);

	/** ���ð�ť�ؼ���Class����
	*/
	void SetButtonControlClass(const std::wstring& classValue);

public:
	/** ��ȡCombo�б��е��������
	*/
	size_t GetCount() const;

	/** ��ȡ��ǰѡ��������
	 * @return ���ص�ǰѡ��������, (�������Ч�������򷵻�Box::InvalidIndex)
	 */
	size_t GetCurSel() const;

	/** ѡ��һ������, ������ѡ���¼�
	 * @param[in] iIndex Ҫѡ�����������
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SetCurSel(size_t iIndex);

	/** ��ȡ�������������
	* @param [in] iIndex ����������
	* @return ���ظ������Ź���������
	*/
	size_t GetItemData(size_t iIndex) const;

	/** �����������������
	* @param [in] iIndex ����������
	* @param [in] itemData ��Ҫ���������
	*/
	bool SetItemData(size_t iIndex, size_t itemData);

	/** ��ȡ������ı�
	* @param [in] iIndex ����������
	*/
	std::wstring GetItemText(size_t iIndex) const;

	/** ����������ı�
	* @param [in] iIndex ����������
	* @param [in] itemText ������ı�����
	*/
	bool SetItemText(size_t iIndex, const std::wstring& itemText);

	/** ���һ�������ַ���
	* @param [in] itemText ������ı�����
	* @return ��������ӵ�����������
	*/
	size_t AddItemText(const std::wstring& itemText);

	/** ��ָ��������λ��, ����һ�������ַ���, ��������ӵ�����������
	* @param [in] iIndex ����������
	* @param [in] itemText ������ı�����
	*/
	size_t InsertItemText(size_t iIndex, const std::wstring& itemText);

	/** ɾ��һ������
	* @param [in] iIndex ����������
	*/
	bool DeleteItem(size_t iIndex);

	/** ��ȡ��ǰ�༭���ڵ��ı�
	 */
	std::wstring GetText() const;

	/** ���ñ༭���ڵ��ı�
	*/
	void SetText(const std::wstring& text);

public:
	/** ��ȡ�����б�����ӿ�
	*/
	TreeView* GetTreeView();

	/** ��ȡͼ��ؼ�
	*/
	Control* GetIconControl() const;

	/** �༭��ؼ�
	*/
	RichEdit* GetEditControl() const;

	/** ��ť�ؼ�
	*/
	Button* GetButtonContrl() const;

public:
	/** �������ѡ���¼�
	 * @param[in] callback ���ѡ��󴥷��Ļص�����
	 */
	void AttachSelect(const EventCallback& callback) { AttachEvent(kEventSelect, callback);}	//mod by djj

	/** �����������ر��¼�
	 * @param[in] callback �������رպ󴥷��Ļص�����
	 */
	void AttachWindowClose(const EventCallback& callback) { AttachEvent(kEventWindowClose, callback); }

private:
	/** Ĭ�ϵ����ѡ������
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnSelectItem(const EventArgs& args);

	/** �����򴰿ڹر�
	* @param [in] bCanceled true��ʾȡ���������ʾ�����ر�
	*/
	void OnComboWndClosed(bool bCanceled);

	/** ��갴�°�ť
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnButtonDown(const EventArgs& args);

	/** �����ť
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnButtonClicked(const EventArgs& args);

	/** �����Edit���水�°�ť
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnEditButtonDown(const EventArgs& args);

	/** �����Edit���浯��ť
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnEditButtonUp(const EventArgs& args);

	/** ��Edit�ϰ���
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnEditKeyDown(const EventArgs& args);

	/** ���������б�
	*/
	void ParseAttributeList(const std::wstring& strList,
		                    std::vector<std::pair<std::wstring, std::wstring>>& attributeList) const;

	/** ���ÿؼ��������б�
	*/
	void SetAttributeList(Control* pControl, const std::wstring& classValue);

	/** ѡ����仯��ͬ��Edit�ؼ����ı�
	*/
	void OnSelectedItemChanged();

	/** ����һ���µ����ڵ�
	*/
	TreeNode* CreateTreeNode(const std::wstring& itemText);

private:
	/** Combo����
	*/
	uint8_t m_comboType;

	/** �б����Ľӿ�
	*/
	TreeView m_treeView;

	/** �����б�Ĵ��ڽӿ�
	*/
	CComboWnd* m_pWindow;

	/** �����б�Ĵ�С����Ⱥ͸߶ȣ�
	*/
	UiSize m_szDropBox;

	/** �����б��Ƿ����ϵ���
	*/
	bool m_bPopupTop;

	/** ������TreeView�Ľڵ�Class����
	*/
	UiString m_treeNodeClass;

private:
	/** ͼ��ؼ�
	*/
	Control* m_pIconControl;

	/** �༭��ؼ�
	*/
	RichEdit* m_pEditControl;

	/** ��ť�ؼ�
	*/
	Button* m_pButtonControl;

	/** �Ƿ��Ѿ���ʼ��
	*/
	bool m_bInited;

	/** ��ǰѡ����������
	*/
	size_t m_iCurSel;

	/** ��갴�µ�ʱ���Ƿ�������ʾ�����б�
	*/
	bool m_bDropListShown;
};

} // namespace ui

#endif // UI_CONTROL_COMBO_H_
