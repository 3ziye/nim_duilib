#ifndef UI_CORE_TABBOX_H_
#define UI_CORE_TABBOX_H_

#pragma once

#include "duilib/Core/Box.h"

namespace ui
{

class UILIB_API TabBox : public Box
{
public:
	explicit TabBox(Layout* pLayout = new Layout());

	// ���ڳ�ʼ��xml����
	virtual void DoInit() override;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual bool Add(Control* pControl) override;
	virtual bool AddAt(Control* pControl, size_t iIndex) override;
	virtual bool Remove(Control* pControl) override;
	virtual bool RemoveAt(size_t iIndex) override;
	virtual void RemoveAll() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief ��ȡ��ǰѡ����
	 * @return ���ص�ǰѡ��������
	 */
	int GetCurSel() const;

	/**
	 * @brief ���ݿؼ�����ѡ��һ������
	 * @param[in] iIndex Ҫѡ�����������
	 * @return �ɹ����� true�����򷵻� false
	 */
	bool SelectItem(int iIndex);

	/**
	 * @brief ���ݿؼ�ָ��ѡ��һ������
	 * @param[in] pControl Ҫѡ�������ָ��
	 * @return �ɹ����� true�����򷵻� false
	 */
	bool SelectItem(Control* pControl);

	/**
	 * @brief ���ݿؼ�����ѡ��һ������
	 * @param[in] pControlName Ҫѡ��Ŀؼ�����
	 * @return �ɹ����� true�����򷵻� false
	 */
	bool SelectItem(const std::wstring& pControlName);

	/**
	 * @brief �����Ƿ���ʾ����Ч��
	 * @param[in] bFadeSwitch ����Ϊ true ����ʾ��false Ϊ����ʾ����
	 * @return ��
	 */
	void SetFadeSwitch(bool bFadeSwitch);

	/**
	 * @brief �ж��Ƿ���Ҫ��ʾ����Ч��
	 * @return ���� true ��ʾ������false Ϊ����ʾ����Ч��
	 */
	bool IsFadeSwitch() const { return m_bFadeSwith; }
	
	/** @brief ����ѡ���¼�
	  * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	  * @return ��
	  */
	void AttachSelect(const EventCallback& callback) { m_OnEvent[kEventSelect] += callback; }
protected:
	/**
	 * @brief ��ʾһ�� TAB ��ʱ������һЩ����
	 * @param[in] it TAB ������
	 * @return ��
	 */
	void OnShowTabItem(size_t index);

	/**
	 * @brief ����һ�� TAB ��ʱ������һЩ����
	 * @param[in] it TAB ������
	 * @return ��
	 */
	void OnHideTabItem(size_t index);

	/** �������Ž���
	*@param[in] it TAB ������
	*/
	void OnAnimationComplete(size_t index);

private:
	//��ǰѡ���Item�±�
	int m_iCurSel;

	//��ʼ��ʱ��Ҫѡ���Item�±�
	int m_iInitSel;

	//�Ƿ��Ѿ���ɳ�ʼ��
	bool m_bIsInit;

	//�Ƿ���Ҫ�л�����
	bool m_bFadeSwith;
};

}
#endif // UI_CORE_TABBOX_H_
