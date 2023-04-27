#ifndef UI_UTILS_WINIMPLBASE_H_
#define UI_UTILS_WINIMPLBASE_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Core/Window.h"

namespace ui
{

#define UI_WNDSTYLE_FRAME		(WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_DIALOG		(WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_CLASSSTYLE_FRAME		(CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_DIALOG	(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

/** ʵ��һ�����б������Ĵ��壬������󻯡���С������ԭ��ť��֧��
*/
class UILIB_API WindowImplBase: public Window
{
public:
	WindowImplBase();

protected:
    //���������������ⲿ���ã�����OnFinalMessage�����У���delete this����
	virtual ~WindowImplBase();

public:
    /** @brief ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ƥ��Ŀ¼
     * @return ������ʵ�ֲ����ش���Ƥ��Ŀ¼
     */
    virtual std::wstring GetSkinFolder() = 0;

    /** @brief ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ƥ�� XML �����ļ�
     * @return ������ʵ�ֲ����ش���Ƥ�� XML �����ļ�
     *         ���ص����ݣ�������XML�ļ����ݣ����ַ�'<'Ϊ��ʼ���ַ�������
     *         �������ļ�·����������'<'�ַ���ʼ���ַ��������ļ�Ҫ��GetSkinFolder()·�����ܹ��ҵ�
     */
    virtual std::wstring GetSkinFile() = 0;

    /**@brief ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ψһ��������
     * @return ������ʵ�ֲ����ش���Ψһ��������
     */
    virtual std::wstring GetWindowClassName() const = 0;

public:

    /**@brief ��Ҫ�����Ŀؼ����Ǳ�׼�Ŀؼ�����ʱ����øú���
     * @param[in] strClass �ؼ�����
     * @return ����һ���Զ���ؼ�ָ�룬һ������¸��� strClass ���������Զ���Ŀؼ�
     */
    virtual Control* CreateControl(const std::wstring& strClass);

protected:
    /**@brief �����յ����ڴ�����Ϣʱ�����ã�����������һЩ��ʼ���Ĺ���
     */
    virtual void OnInitWindow();

    /**@brief ������Ϣ���ɷ�����, ����̳иú�������д�����У���Ҫ���û���ĺ���
     * @param[in] uMsg ��Ϣ��
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ��Ϣ�Ƿ��Ѿ�����
                   ���� true  �����Ѿ��ɹ�������Ϣ������Ҫ�ٴ��ݸ����ڹ��̣�
                   ���� false ��ʾ����Ϣ�������ݸ����ڹ��̴���
     * @return ������Ϣ�Ĵ�����
     */
    virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

    /**@brief �յ����ڴ�����Ϣ��WM_CREATE��ʱ�����ã���ʹ�� OnInitWindow �ӿ���ʵ���Զ�������
     * @param[in] uMsg ��ϢID
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ��Ϣ�Ƿ��Ѿ�������
     * @return ������Ϣ������
     */
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief �ڴ����յ� WM_NCDESTROY ��Ϣʱ�ᱻ����
     * @param[in] hWnd Ҫ���ٵĴ��ھ��
     */
    virtual void OnFinalMessage(HWND hWnd) override;

	/**@brief �����յ�����������˫����Ϣ��WM_NCLBUTTONDBLCLK��ʱ������
	 * @param[in] uMsg ��Ϣ����
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
	 * @return ������Ϣ������
	 */
	virtual LRESULT OnNcLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

     /**@brief ���մ��ڿ���������Ϣʱ��WM_SYSCOMMAND��������
     *  @param[in] uMsg ��Ϣ����
     *  @param[in] wParam ��Ϣ���Ӳ���
     *  @param[in] lParam ��Ϣ���Ӳ���
     *  @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     *  @return ������Ϣ������
     */
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

protected:
    /**@brief ���յ����ڹر���Ϣ(WM_CLOSE)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ����ڱ�������Ϣ(WM_DESTROY)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�����ƶ���Ϣ(WM_MOUSEMOVE)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief �����յ�(WM_MOUSEWHEEL)��Ϣʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief �����յ�(WM_MOUSEHOVER)��Ϣʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�������������Ϣ(WM_LBUTTONDOWN)ʱ������
    *  @param[in] uMsg ��Ϣ����
    *  @param[in] wParam ��Ϣ���Ӳ���
    *  @param[in] lParam ��Ϣ���Ӳ���
    *  @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
    *  @return ������Ϣ������
    */
    virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�������������Ϣ(WM_LBUTTONUP)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�������˫����Ϣ(WM_LBUTTONDBLCLK)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnLButtonDbClk(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�������������Ϣ(WM_RBUTTONDOWN)ʱ������
    *  @param[in] uMsg ��Ϣ����
    *  @param[in] wParam ��Ϣ���Ӳ���
    *  @param[in] lParam ��Ϣ���Ӳ���
    *  @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
    *  @return ������Ϣ������
    */
    virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�������������Ϣ(WM_RBUTTONUP)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���յ�������˫����Ϣ(WM_RBUTTONDBLCLK)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnRButtonDbClk(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief �����յ��ַ�������Ϣʱ��WM_CHAR��������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���ռ��̰���������Ϣ(WM_KEYDOWN)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**@brief ���ռ��̰���������Ϣ(WM_KEYUP)ʱ������
     * @param[in] uMsg ��Ϣ����
     * @param[in] wParam ��Ϣ���Ӳ���
     * @param[in] lParam ��Ϣ���Ӳ���
     * @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
     * @return ������Ϣ������
     */
    virtual LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

private:    
    /**@brief ���ڹ��ܰ�ť�����ʱ����
     * @param[in] param Я���Ĳ���
     * @return ʼ�շ��� true
     */
	bool OnButtonClick(EventArgs* param);
};

}

#endif // UI_UTILS_WINIMPLBASE_H_
