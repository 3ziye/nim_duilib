#pragma once

#include "duilib/Utils/ShadowWnd.h"

namespace nim_comp
{
/** @class WindowEx
 * @brief ���д���Ļ���
 * @copyright (c) 2015, NetEase Inc. All rights reserved
 * @date 2015/9/16
 */
class WindowEx : public ui::ShadowWnd
{
public:
	WindowEx();
	virtual ~WindowEx();

	/**
	 * ��������
	 * @param[in] hwndParent �����ھ��
	 * @param[in] pstrName ��������
	 * @param[in] dwStyle ������ʽ
	 * @param[in] dwExStyle ������չ��ʽ
	 * @param[in] isLayeredWindow �Ƿ񴴽��ֲ㴰��
	 * @param[in] rc ����λ��
	 * @return HWND ���ھ��
	 */
	virtual HWND CreateWnd(HWND hwndParent,
						   const wchar_t* windowName,
						   uint32_t dwStyle,
						   uint32_t dwExStyle,
						   const ui::UiRect& rc = ui::UiRect(0, 0, 0, 0)) override;

	/**
	 * �����ڱ����ٵ���Ϣ
	 * @param[in] uMsg ��Ϣ
	 * @param[in] wParam ����
	 * @param[in] lParam ����
	 * @param[out] bHandled ��Ϣ�Ƿ񱻴���
	 * @return LRESULT ������
	 */
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

	/**
	 * ����ESC����������Ϣ
	 * @param[out] bHandled ��Ϣ�Ƿ񱻴���
	 * @return void �޷���ֵ
	 */
	virtual void OnEsc(bool &bHandled);

	/**
	 * ��ȡ���������Ľӿ�
	 * @return wstring ��������
	 */
	virtual std::wstring GetWindowClassName() const = 0;

	/**
	 * ��ȡ����id�Ľӿ�
	 * @return wstring ����id
	 */
	virtual std::wstring GetWindowId(void) const = 0;

	/**@brief ������Ϣ���ɷ�����
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @param[out] bHandled ��Ϣ�Ƿ��Ѿ�����
				   ���� true  �����Ѿ��ɹ�������Ϣ������Ҫ�ٴ��ݸ����ڹ��̣�
				   ���� false ��ʾ����Ϣ�������ݸ����ڹ��̴���
	 * @return ������Ϣ�Ĵ�����
	 */
	virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

private:
	/**
	 * ��WindowManager��ע���Լ�
	 * @return bool true ע��ɹ���false ע��ʧ��
	 */
	bool RegisterWnd();

	/**
	 * ��WindowManager�з�ע���Լ�
	 * @return void �޷���ֵ
	 */
	void UnRegisterWnd();
};

/**
* ��ȡ�������ڿ�����ʾ�����½�λ�õ�����
* @return POINT ��������
*/
POINT GetPopupWindowPos(WindowEx* window);

}