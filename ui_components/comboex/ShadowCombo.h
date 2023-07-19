#pragma once

#include "duilib/Core/Window.h"
#include "duilib/Core/Box.h"
#include "duilib/Box/ListBox.h"

namespace nim_comp
{
class CShadowComboWnd;
class ShadowCombo;
 
class CShadowComboWnd : public ui::Window
{
public:
  CShadowComboWnd();
  ~CShadowComboWnd() override;

  void InitComboWnd(ShadowCombo* pOwner);
  virtual std::wstring GetWindowClassName() const override;
  virtual void OnFinalMessage(HWND hWnd) override;
  virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

  void OnSeleteItem();

private:
  ShadowCombo* m_pOwner = nullptr;
  size_t m_iOldSel = ui::Box::InvalidIndex;
};

class ShadowCombo : public ui::Box {
public:
    friend class CShadowComboWnd;

    ShadowCombo();
    ShadowCombo(const ShadowCombo& r) = delete;
    ShadowCombo& operator=(const ShadowCombo& r) = delete;
    ~ShadowCombo() override;

    // ��д���෽�����ṩ���Ի����ܣ���ο���������
    void DoInit() override;
    std::wstring GetType() const override;
    bool AddItem(Control* pControl) override;
    bool RemoveItem(Control* pControl) override;
    bool RemoveItemAt(size_t iIndex) override;
    void RemoveAllItems() override;
    virtual Control* GetItemAt(size_t iIndex) const override;
    virtual size_t GetItemCount() const override { return m_pLayout->GetItemCount(); }

    void Activate() override;
    void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    void PaintText(ui::IRender* pRender) override;
    void PaintChild(ui::IRender* pRender, const ui::UiRect& rcPaint) override;

    /**
     * @brief ��ȡ��ǰѡ�����ı�
     * @return ���ص�ǰѡ�����ı�
     */
    std::wstring GetText() const;

    /**
    * @brief ��ȡ���ֱ߾�
    * @return �������ֵı߾���Ϣ
    */
    const ui::UiPadding& GetTextPadding() const;

    /**
    * @brief �������ֱ߾���Ϣ
    * @param[in] rc �߾���Ϣ
    * @return ��
    */
    void SetTextPadding(ui::UiPadding padding);

    /**
     * @brief ��ȡ��ǰ������ List ����
     * @return ���������� List ����ָ��
     */
    ui::ListBox* GetListBox() { return m_pLayout.get(); }

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
     * @param[in] bTrigger �Ƿ񴥷�ѡ���¼�
     * @return ���� true ��ʾ�ɹ�������Ϊ false
     */
    bool SelectItem(size_t iIndex, bool bTrigger = false);

    /**
     * @brief ��ȡ��ǰѡ��������
     * @return ���ص�ǰѡ��������(�������Ч�������򷵻�Box::InvalidIndex)
     */
    size_t GetCurSel() const { return m_iCurSel; }

    /**
    * @brief ����������Ĵ���
    * @param[in] combo_wnd ���õ������򴰿�
     */
    void SetCShadowComboWnd(CShadowComboWnd* comboWnd) { m_pWindow = comboWnd; }

    /**
    * @brief ��ȡ������Ĵ���
    * @return ���������ʱ���������򴰿ڣ�������ʱ����nullptr
    */
    CShadowComboWnd* GetShadowComboWnd() { return m_pWindow; }

    /**
    * @brief ������������Ӱ�Ĵ�С
    * @param[in] padding ���õ���������Ӱ��С
    */
    void SetShadowCorner(ui::UiPadding padding, bool bNeedDpiScale = true);

    /**
    * @brief ��ȡ��������Ӱ��С
    * @return ������������Ӱ�Ĵ�С
    */
    const ui::UiPadding& GetShadowCorner() const { return m_rcShadowCorner; }

    /**
    * @brief ������������ӰͼƬ
    * @param[in] rect ���õ���������ӰͼƬ
    */
    void SetShadowImage(const std::wstring& ShadowImage) { m_sShadowImage = ShadowImage; }

    /**
    * @brief ��ȡ��������ӰͼƬ
    * @return ������������ӰͼƬ
    */
    std::wstring GetShadowImage() const { return m_sShadowImage; }

    /**
    * @brief ��ȡLayout
    * @return ����Layout
    */
    ui::ListBox* GetCustomLayout() const { return m_pLayout.get(); }

    void SetArrowOffset(int offset, bool bNeedDpiScale = true);

    /**
     * @brief �������ѡ���¼�
     * @param[in] callback ���ѡ��󴥷��Ļص�����
     * @return ��
     */
    void AttachSelect(const ui::EventCallback& callback) { AttachEvent(ui::kEventSelect, callback); /*m_pLayout->AttachSelect(callback);*/ }	//mod by djj

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

    /**
     * @brief ѡ��һ������
     * @param[in] iIndex Ҫѡ�����������
     * @return ���� true ��ʾ�ɹ�������Ϊ false
     */
    bool SelectItemInternal(size_t iIndex);

protected:
    CShadowComboWnd* m_pWindow;
    std::unique_ptr<ui::ListBox> m_pLayout;
    size_t m_iCurSel;
    ui::UiSize m_szDropBox;
    std::wstring m_sDropBoxAttributes;
    bool m_bPopupTop;
    ui::UiPadding	m_rcTextPadding;
    ui::UiPadding m_rcShadowCorner;
    std::wstring m_sShadowImage;
    ui::Control* m_cArrow;
    bool m_bInit;
    int m_iArrowOffset;
};

}