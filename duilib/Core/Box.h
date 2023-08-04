#ifndef UI_CORE_BOX_H_
#define UI_CORE_BOX_H_

#pragma once

#include "duilib/Box/Layout.h"
#include "duilib/Core/Control.h"

namespace ui 
{

/////////////////////////////////////////////////////////////////////////////////////
//
class UILIB_API Box : public Control
{
public:
	explicit Box(Layout* pLayout = new Layout());
	Box(const Box& r) = delete;
	Box& operator=(const Box& r) = delete;
    virtual ~Box();

public:
	/** ��Ч����������
	*/
	static constexpr auto InvalidIndex{ static_cast<size_t>(-1) };

	/** @brief �Ƿ�Ϊ��Ч���ӿؼ�����
	*/
	static bool IsValidItemIndex(size_t index) { return index != Box::InvalidIndex; }

public:
	/// ��д����ӿڣ��ṩ���Ի����ܡ���������˵����鿴 Control �ؼ�
	virtual std::wstring GetType() const override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetWindow(Window* pManager) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;
	virtual void SetEnabled(bool bEnabled) override;
	virtual void SetVisible(bool bVisible) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos = UiPoint()) override;
	virtual void InvokeLoadImageCache() override;
	virtual void UnLoadImageCache() override;
	virtual void ClearImageCache() override;
	virtual UINT GetControlFlags() const override;

	/** ���ÿؼ�λ�ã�����ɸı���Ϊ��
	 * @param [in] rc Ҫ���õľ���������Ϣ�������ڱ߾࣬��������߾�
	 */
	virtual void SetPos(UiRect rc) override;

	/** ��ȡ�ؼ�ʵ�ʿ��þ������򣬰����ڱ߾࣬��������߾�
	*@return ���ؿؼ���ʵ�ʿ������򣬼�GetPos��ȥPadding�������
	*/
	virtual UiRect GetPosWithoutPadding() const;

	/** ��ȡ�ؼ�ʵ�ʿ��þ������򣬰����ڱ߾࣬��������߾�
	*@return ���ؿؼ���ʵ�ʿ������򣬼�GetRect��ȥPadding�������
	*/
	UiRect GetRectWithoutPadding() const;

	/** ����ؼ���С(��͸�)
		���������ͼƬ������ width �� height ����һ��Ϊ auto��������ͼƬ��С���ı���С���������մ�С
	 *  @param [in] szAvailable ���ô�С����������߾�
	 *  @return �ؼ��Ĺ����С�������ڱ߾�(Box)����������߾�
	 */
	virtual UiEstSize EstimateSize(UiSize szAvailable) override;

public:
	/** @name �����ӿؼ�(item)��صķ���
	* @{
	*/
	/**@brief ��ȡ�ӿؼ�����
	 */
	virtual size_t GetItemCount() const;

	/**
	 * @brief ������������ָ���ؼ�
	 * @param[in] iIndex �ؼ�����
	 * @return ���ؿؼ�ָ��
	 */
	virtual Control* GetItemAt(size_t iIndex) const;

	/**
	 * @brief ���ݿؼ�ָ���ȡ����
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� pControl ��ָ��Ŀؼ�����, ���û�ҵ����򷵻� Box::InvalidIndex
	 */
	virtual size_t GetItemIndex(Control* pControl) const;

	/**
	 * @brief ���ÿؼ��������ڲ����������пؼ�λ�ã�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex Ҫ���õ�����ֵ
	 * @return ���� true ���óɹ���false ����ʧ��
	 */
	virtual bool SetItemIndex(Control* pControl, size_t iIndex);

	/**
	 * @brief ���һ���ؼ���������
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� true ��ӳɹ���false Ϊ���ʧ��
	 */
	virtual bool AddItem(Control* pControl);

	/**
	 * @brief ��ָ��λ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex �ڸ�����֮�����ؼ�
	 * @return ���� true Ϊ��ӳɹ���false Ϊ���ʧ��
	 */
	virtual bool AddItemAt(Control* pControl, size_t iIndex);

	/**
	 * @brief ���ݿؼ�ָ����������Ƴ�һ���ؼ�
	 * @param[in] pControl �ؼ���ָ��
	 * @return ���� true Ϊ�Ƴ��ɹ���false Ϊ�Ƴ�ʧ�ܣ��ؼ����ܲ����ڣ�
	 */
	virtual bool RemoveItem(Control* pControl);

	/**
	 * @brief ���ݿؼ��������������Ƴ�һ���ؼ�
	 * @param[in] iIndex Ҫ�Ƴ��Ŀؼ�����
	 * @return ���� true Ϊ�Ƴ��ɹ���false Ϊ�Ƴ�ʧ�ܣ�����ֵ̫С�򳬳����ӿؼ�������
	 */
	virtual bool RemoveItemAt(size_t iIndex);

	/**
	 * @brief �Ƴ������ӿؼ�
	 * @return ��
	 */
	virtual void RemoveAllItems();

	/** @} */

public:
	/**
	 * @brief ������һ����ѡ�ؼ������������� list��combo��
	 * @param[in] iIndex ָ��Ҫ��ʼ���ҵ�����
	 * @param[in] bForward true Ϊ�������ң� false Ϊ�ݼ�����
	 * @return ��һ����ѡ�ؼ������������� Box::InvalidIndex Ϊû�п�ѡ�ؼ�
	 */
	size_t FindSelectable(size_t iIndex, bool bForward = true) const;

	/**
	 * @brief ����ָ���ӿؼ�
	 * @param[in] pstrSubControlName �ӿؼ�����
	 * @return �����ӿؼ�ָ��
	 */
	Control* FindSubControl(const std::wstring& pstrSubControlName);

	/**
	 * @brief �ж��Ƿ��Զ�����
	 * @return true Ϊ�Զ����٣�false Ϊ���Զ�����
	 */
    bool IsAutoDestroyChild() const { return m_bAutoDestroyChild; }

	/**
	 * @brief ���ÿؼ��Ƿ��Զ�����
	 * @param[in] bAuto true Ϊ�Զ����٣�false Ϊ���Զ�����
	 * @return ��
	 */
    void SetAutoDestroyChild(bool bAuto) { m_bAutoDestroyChild = bAuto; }

	/**
	 * @brief �жϴ��ڹرպ��Ƿ��Զ�����
	 * @return true Ϊ�Զ����٣�false Ϊ���Զ�����
	 */
    bool IsDelayedDestroy() const { return m_bDelayedDestroy; }

	/**
	 * @brief ���ô��ڹرպ��Ƿ��Զ�����
	 * @param[in] bDelayedDestroy true Ϊ�Զ����٣�false Ϊ���Զ�����
	 * @return ��
	 */
	void SetDelayedDestroy(bool bDelayedDestroy) { m_bDelayedDestroy = bDelayedDestroy; }

	/**
	 * @brief ��ȡ�����Ƿ���Ӧ������
	 * @return true Ϊ��Ӧ��false Ϊ����Ӧ
	 */
    bool IsMouseChildEnabled() const { return m_bMouseChildEnabled; }

	/**
	 * @brief ����������Ӧ������
	 * @param[in] bEnable ����Ϊ true Ϊ��Ӧ������������Ϊ false Ϊ����Ӧ��Ĭ��Ϊ true
	 */
	void SetMouseChildEnabled(bool bEnable) { m_bMouseChildEnabled = bEnable; }

	/**
	 * @brief ��ȡ�������ֶ���ָ��
	 * @return �������������Ĳ��ֶ���ָ��
	 */
	Layout* GetLayout() const { return m_pLayout.get(); }

	/**
	 * @brief ���¹������ֶ���
	 * @param[in] pLayout ���ֶ���ָ��
	 * @return ��
	 */
	void ReSetLayout(Layout* pLayout);

private:
	/**@brief ��ָ��λ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex �ڸ�����֮�����ؼ�
	 */
	bool DoAddItemAt(Control* pControl, size_t iIndex);

	/**@brief ���ݿؼ�ָ����������Ƴ�һ���ؼ�
	 * @param[in] pControl �ؼ���ָ��
	 */
	bool DoRemoveItem(Control* pControl);

protected:

	//�����е��ӿؼ��б�
	std::vector<Control*> m_items;

private:

	//�Ƿ��Զ�ɾ��item�Ķ������Ϊtrue���ڴ�m_items�Ƴ�Ԫ��ʱ����delete������������Ϊfalse����delete��
	bool m_bAutoDestroyChild;

	//�Ƿ��ӳ�ɾ��item�������Ϊtrue����Ԫ���Ƴ��󣬻�ŵ�Window�����У��ӳ�delete������󣬽���m_bAutoDestroyChildʱ��Ч
	bool m_bDelayedDestroy;
	
	//���ֹ���ӿ�
	std::unique_ptr<Layout> m_pLayout;

	//�Ƿ�������Ӧ�ӿؼ��������Ϣ
	bool m_bMouseChildEnabled;
};

} // namespace ui

#endif // UI_CORE_BOX_H_
