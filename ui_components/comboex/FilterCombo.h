#pragma once
#include "duilib/Control/ListBox.h"
#include "duilib/Core/Box.h"
#include "duilib/Box/VBox.h"
#include "duilib/Control/RichEdit.h"

namespace nim_comp
{
class CFilterComboWnd;

class UILIB_API IMatch {
	virtual bool StringMatch(const std::string& utf8str) = 0;
};

class UILIB_API ListElementMatch : public ui::ListBoxElement, public IMatch
{
public:
	virtual bool StringMatch(const std::string& utf8str) override;
};

class FilterListBox : public ui::ListBox
{
public:
	explicit FilterListBox(ui::Layout* pLayout = new ui::VLayout) : ui::ListBox(pLayout), m_pFilterComboWnd(nullptr){};
	virtual ~FilterListBox(){};

	virtual bool SelectItem(size_t iIndex, bool bTakeFocus = false, bool bTrigger = true) override;
	void Filter(const std::string& utf8_str);

	void SetFilterComboWnd(CFilterComboWnd *pFilterComboWnd){ m_pFilterComboWnd = pFilterComboWnd; };
	CFilterComboWnd *GetFilterComboWnd(){ return m_pFilterComboWnd; };
private:
	CFilterComboWnd *m_pFilterComboWnd;
};

class UILIB_API FilterCombo : public ui::Box
{
	friend class CFilterComboWnd;
public:
	FilterCombo();
	FilterCombo(const FilterCombo& r) = delete;
	FilterCombo& operator=(const FilterCombo& r) = delete;
	virtual ~FilterCombo(){};

	virtual void HandleEvent(const ui::EventArgs& args) override;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual bool AddItem(Control* pControl) override;
	virtual bool RemoveItem(Control* pControl) override;
	virtual bool RemoveItemAt(size_t iIndex) override;
	virtual void RemoveAllItems() override;
	virtual Control* GetItemAt(size_t iIndex) const override;
	virtual size_t GetItemCount() const override;

	virtual void Activate() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	//virtual void PaintText(IRenderContext* pRender) override;

	/**
	 * @brief ��ȡ��ǰѡ�����ı�
	 * @return ���ص�ǰѡ�����ı�
	 */
    std::wstring GetText() const;

	/**
	 * @brief ��ȡ��ǰ������ List ����
	 * @return ���������� List ����ָ��
	 */
	FilterListBox* GetListBox();

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
	ui::UiSize GetDropBoxSize() const;

	/**
	 * @brief ����������������С
	 * @param[in] szDropBox Ҫ���õĴ�С��Ϣ
	 * @return ��
	 */
    void SetDropBoxSize(ui::UiSize szDropBox);
	
	/**
	 * @brief ���� Combobox �Ƿ����ϵ���
	 * @param[in] top Ϊ true �����ϵ�����false ΪĬ�����µ���
	 * @return ��
	 */
	void SetPopupTop(bool top) { m_bPopupTop = top; };
	
	/**
	 * @brief �ж� Combobox ����ģʽ�Ƿ������ϵ���
	 * @return ���� true ��ʾ���ϵ���������Ϊ false
	 */
	bool IsPopupTop() const { return m_bPopupTop; };

	/**
	 * @brief ѡ��һ������
	 * @param[in] iIndex Ҫѡ�����������
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SelectItem(size_t iIndex);

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
	void AttachSelect(const ui::EventCallback& callback);

	/**
   * @brief �����������ر��¼�
   * @param[in] callback �������رպ󴥷��Ļص�����
   * @return ��
   */
	void AttachWindowClose(const ui::EventCallback& callback) { AttachEvent(ui::kEventWindowClose, callback); }

private:
	/**
	 * @brief Ĭ�ϵ����ѡ������
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnSelectItem(const ui::EventArgs& args);

	bool OnRichEditTextChanged(const ui::EventArgs& args);
	bool OnRichEditButtonDown(const ui::EventArgs& args);

protected:
	CFilterComboWnd *m_pComboWnd;
	std::unique_ptr<FilterListBox> m_pLayout;

	ui::RichEdit* m_pRichEdit;
    size_t m_iCurSel;
	ui::UiSize m_szDropBox;
	std::wstring m_sDropBoxAttributes;
	bool m_bPopupTop;
};

} // namespace ui

