#ifndef UI_CONTROL_COMBO_H_
#define UI_CONTROL_COMBO_H_

#pragma once

#include "duilib/Core/Box.h"

namespace ui 
{
	class ListBox;
	class CComboWnd;
class UILIB_API Combo : public Box
{
    friend class CComboWnd;
public:
	Combo();
	Combo(const Combo& r) = delete;
	Combo& operator=(const Combo& r) = delete;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual bool AddItem(Control* pControl) override;
	virtual bool RemoveItem(Control* pControl) override;
	virtual bool RemoveItemAt(size_t iIndex) override;
	virtual void RemoveAllItems() override;
	virtual Control* GetItemAt(size_t iIndex) const override;
	virtual size_t GetItemCount() const override;

	virtual void Activate() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintText(IRender* pRender) override;

	/** �ÿؼ��Ƿ���Է����ڱ������ϣ������ڴ���NC��Ϣ��Ӧ��
	 * @return ���� true ��ʾ���ԣ�false ��ʾ������
	 */
	virtual bool CanPlaceCaptionBar() const override;

	/**
	 * @brief ��ȡ��ǰѡ�����ı�
	 * @return ���ص�ǰѡ�����ı�
	 */
    std::wstring GetText() const;

	/**
	* @brief ��ȡ���ֱ߾�
	* @return �������ֵı߾���Ϣ
	*/
	const UiPadding& GetTextPadding() const;

	/** ���������ڱ߾���Ϣ, �����ڲ������DPI����Ӧ����
	 * @param [in] padding ���ε��ĸ���ֵ�ֱ�����Ӧ���ĸ��ڱ߾�ֵ
	 * @param[in] bNeedDpiScale ���� DPI ���ţ�Ĭ��Ϊ true
	 */
	void SetTextPadding(UiPadding padding, bool bNeedDpiScale = true);

	/**
	 * @brief ��ȡ��ǰ������ List ����
	 * @return ���������� List ����ָ��
	 */
	ListBox* GetListBox() { return m_pLayout.get(); }

	/**
	 * @brief ��ȡ������������Ϣ
	 * @return �����ַ�����ʽ��������Ϣ
	 */
    std::wstring GetDropBoxAttributeList();

	/**
	 * @brief �����������������Ϣ
	 * @param[in] pstrList ת���� XML ��ʽ�����б�
	 * @return ��
	 */
    void SetDropBoxAttributeList(const std::wstring& pstrList);

	/**
	 * @brief ��ȡ������������С
	 * @return ����������С
	 */
    UiSize GetDropBoxSize() const;

	/**
	 * @brief ����������������С
	 * @param[in] szDropBox Ҫ���õĴ�С��Ϣ
	 * @return ��
	 */
    void SetDropBoxSize(UiSize szDropBox);
	
	/**
	 * @brief ���� Combobox �Ƿ����ϵ���
	 * @param[in] top Ϊ true �����ϵ�����false ΪĬ�����µ���
	 * @return ��
	 */
	void SetPopupTop(bool top) { m_bPopupTop = top; }
	
	/**
	 * @brief �ж� Combobox ����ģʽ�Ƿ������ϵ���
	 * @return ���� true ��ʾ���ϵ���������Ϊ false
	 */
	bool IsPopupTop() const { return m_bPopupTop; }

	/**
	 * @brief ѡ��һ������
	 * @param[in] iIndex Ҫѡ�����������
	 * @param[in] bTrigger �Ƿ񴥷�ѡ���¼�
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SelectItem(size_t iIndex, bool bTrigger = false);

	/**
	 * @brief ��ȡ��ǰѡ��������
	 * @return ���ص�ǰѡ��������, (�������Ч�������򷵻�Box::InvalidIndex)
	 */
	size_t GetCurSel() const { return m_iCurSel; }
   
	/**
	 * @brief �������ѡ���¼�
	 * @param[in] callback ���ѡ��󴥷��Ļص�����
	 * @return ��
	 */
	void AttachSelect(const EventCallback& callback) { AttachEvent(kEventSelect, callback);}	//mod by djj

	/**
	 * @brief �����������ر��¼�
	 * @param[in] callback �������رպ󴥷��Ļص�����
	 * @return ��
	 */
	void AttachWindowClose(const EventCallback& callback) { AttachEvent(kEventWindowClose, callback); }

private:
	/**
	 * @brief Ĭ�ϵ����ѡ������
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnSelectItem(const EventArgs& args);

	/**
	 * @brief ѡ��һ������
	 * @param[in] iIndex Ҫѡ�����������
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SelectItemInternal(size_t iIndex);

protected:
	CComboWnd *m_pWindow;
	std::unique_ptr<ListBox> m_pLayout;
	size_t m_iCurSel;
	UiSize m_szDropBox;
	UiString m_sDropBoxAttributes;
	bool m_bPopupTop;
	UiPadding m_rcTextPadding;	//add by djj
};

} // namespace ui

#endif // UI_CONTROL_COMBO_H_
