#ifndef UI_CORE_WINDOW_H_
#define UI_CORE_WINDOW_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Utils/Delegate.h"
#include "base/callback/callback.h"
#include "duilib/Core/ControlFinder.h"

#include <string>

namespace ui 
{

class Box;
class Control;
class IRenderContext;
class Shadow;
class ToolTip;

/** @brief ������Ϣ���˽ӿڣ����ڽػ񴰿ڹ��̵���Ϣ��������Window�������Ϣ����
*/
class IUIMessageFilter
{
public:
	/** @brief ��Ϣ���������������ȼ�����Window�����Ϣ������
	 * @param[in] uMsg ��Ϣ����
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @param[out] bHandled ���� false ������ɷ�����Ϣ������ true ��ʾ�����ɷ�����Ϣ
	 * @return ������Ϣ������
	 */
	virtual LRESULT FilterMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
//

#if defined(ENABLE_UIAUTOMATION)
class UIAWindowProvider;
#endif

/**@brief ������
*/
class UILIB_API Window : public virtual nbase::SupportWeakCallback
{
public:
    Window();
	virtual ~Window();

public:
	/** @name ���ڴ��������١��¼���������ؽӿ�
	* @{ 
	*/

	/**@brief ��������
	 * @param[in] hwndParent �����ھ��
	 * @param[in] windowName ��������
	 * @param[in] dwStyle ������ʽ
	 * @param[in] dwExStyle ������չ��ʽ
	 * @param[in] isLayeredWindow �Ƿ���в㴰�����ԣ�Ĭ��Ϊ true
	 * @param[in] rc ���ڴ�С
	 * @return ���ش��ھ��
	 */
	virtual HWND CreateWnd(HWND hwndParent,
						   const wchar_t* windowName,
						   uint32_t dwStyle, 
						   uint32_t dwExStyle, 
						   bool isLayeredWindow = true, 
						   const UiRect& rc = UiRect(0, 0, 0, 0));

	/**@brief ��ȡ���������� Windows ���
	 */
    HWND GetHWND() const;

	/**@brief ���໯���ڣ����Ĵ��ڹ��̺�����
	 * @param[in] ���ھ��
	 * @return �µĴ��ھ��
	 */
	HWND Subclass(HWND hWnd);

	/**@brief ȡ�����໯���ڣ��ָ�ԭ���Ĵ��ڹ��̺�����
	 */
	void Unsubclass();

	/**@brief �رմ���, �첽�رգ����������غ�IsClosing() ״̬Ϊtrue
	 * @param[in] �ر���Ϣ
	 */
	virtual void CloseWnd(UINT nRet = IDOK);

	/**@brief �Ƿ�Ҫ�ر�
	 */
	bool IsClosingWnd() const { return m_bCloseing; };

	/**@brief �������ڹر��¼�
	 * @param[in] callback ָ���رպ�Ļص�����
	 */
	void AttachWindowClose(const EventCallback& callback);

protected:
	/**@brief ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ψһ��������
	 * @return ���෵�ؿմ�������������ʵ�ֲ����ش���Ψһ��������
	 */
	virtual std::wstring GetWindowClassName() const;

	/**@brief ��ȡ�ؼ�������
	 * @return ���ؿؼ������࣬�û��෵�ؿմ�
	 */
	virtual std::wstring GetSuperClassName() const;

	/**@brief ��ȡ���������ʽ���÷�����ʵ����������ʵ�֣�https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles
	 * @return ���ش��������ʽ���÷������෵�� CS_DBLCLKS
	 */
	virtual UINT GetClassStyle() const;

	/**@brief ��ȡ������ʽ
	* @return Ĭ�Ϸ��ص�ǰ���ڵ���ʽȥ��WS_CAPTION����
	*         ���������д�ú����󣬷���ֵΪ0���򲻸ı䵱ǰ���ڵ���ʽ
	*/
	virtual UINT GetStyle() const;

	/** ��ȡ��Դ�ľ��
	* @return Ĭ�Ϸ��ص�ǰ����exe�ľ��
	*/
	virtual HMODULE GetResModuleHandle() const;

	/**@brief ���ڽ��յ� WM_CREATE ��Ϣʱ�ᱻ����
	 * @param[in] hWnd ���ھ��
	 */
	void InitWnd(HWND hWnd);

	/**@brief �ڴ����յ� WM_NCDESTROY ��Ϣʱ�ᱻ����
	 * @param[in] hWnd ���ھ��
	 */
	virtual void OnFinalMessage(HWND hWnd);

private:
	/**@brief ע�ᴰ����
	 */
    bool RegisterWindowClass();

	/**@brief ע��ؼ������ࣨ�봰�ڵĹ��̺�����ͬ��
	 */
    bool RegisterSuperClass();

	/**@brief ���ڹ��̺���(��GetSuperClassName()�������ز�Ϊ�մ�ʱʹ��)
	 * @param[in] hWnd ���ھ��
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ������Ϣ������
	 */
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**@brief ���ڹ��̺���(��GetSuperClassName()�������ؿմ�ʱʹ��)
	 * @param[in] hWnd ���ھ��
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ������Ϣ������
	 */
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/** @} */

public:
	/** @name ������ʾ�����ء���С��λ�õ�״̬��ؽӿ�
	* @{
	*/

	/**@brief ��ʾ�����ش���
	 * @param[in] bShow Ϊ true ʱ��ʾ���ڣ�Ϊ false ʱΪ���ش��ڣ�Ĭ��Ϊ true
	 * @param[in] bTakeFocus �Ƿ��ý��㣨����ڣ���Ĭ��Ϊ true
	*/
	virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	/**@brief ��ʾģ̬�Ի���
	 * @param[in] parent_hwnd �����ھ��
	 */
	void ShowModalFake(HWND parent_hwnd);

	/**@brief �Ƿ���ģ̬��ʾ
	*/
	bool IsFakeModal() const;

	/**@brief ���д��ڣ�֧����չ��Ļ
	*/
	void CenterWindow();

	/** @brief ����������Ϊ�ö�����
	*/
	void ToTopMost();

	/** @brief �����ڵ���Ϊ���㴰��
	*/
	void BringToTop();

	/** @brief �����
	*          �����ǰ����Ϊ��С��������л�ԭ��
	*          �����ǰ����Ϊ���أ�����ʾ��������Ϊǰ�˴���
	*          �����ǰ����Ϊ��ʾ��������Ϊǰ�˴���
	*/
	void ActiveWindow();

	/**@brief ��ȡ����λ����Ϣ
	 * @param[in] bContainShadow �Ƿ������Ӱ��true Ϊ������Ĭ��Ϊ false ������
	 */
	UiRect GetPos(bool bContainShadow = false) const;

	/**@brief ���ô���λ�ã��� SetWindowPos ��һ���װ��
	 * @param[in] rc ����λ��
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 * @param[in] uFlags ��Ӧ SetWindowPos �� uFlags ѡ��
	 * @param[in] hWndInsertAfter ��Ӧ SetWindowPos �� hWndInsertAfter ѡ��
	 * @param[in] bContainShadow rc�����Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 */
	void SetPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter = NULL, bool bContainShadow = false);

	/**@brief ���ô��ڴ�С
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ������ DPI ����
	 */
	void Resize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**@brief ���ô���ͼ��
	*  @param[in] nRes ����ͼ����Դ ID
	*/
	void SetIcon(UINT nRes);

	/**@brief ���ô��ڱ������ı�
	 * @param[in] strText ���ڱ������ı�
	 */
	void SetText(const std::wstring& strText);

	/**@brief ���������б��е��ı� ID�� ����ID���ô��ڱ������ı�
	 * @param[in] strTextId ���� ID���� ID �����������ļ��д���
	 */
	void SetTextId(const std::wstring& strTextId);

	/** @} */

public:
	/** @name ���ڳ�ʼ��С������������Ӱ����ؽӿ�
	* @{
	*/

	/**@brief ��ȡ�����ı߿����췶Χ�Ĵ�С
	 */
	const UiRect& GetSizeBox() const;

	/**@brief ���ô����ı߿����췶Χ�Ĵ�С
	 * @param[in] rcSizeBox Ҫ���õĴ�С
	 */
	void SetSizeBox(const UiRect& rcSizeBox);

	/**@brief ��ȡ���ڱ��������򣨿��϶����򣩣���Ӧ XML �� caption ����
	 */
	const UiRect& GetCaptionRect() const;

	/**@brief ���ô��ڱ���������
	 * @param[in] rcCaption Ҫ���õ�����Χ
	 */
	void SetCaptionRect(const UiRect& rcCaption);

	/**@brief ��ȡ����Բ�Ǵ�С����Ӧ XML �� roundcorner ����
	 */
	const UiSize& GetRoundCorner() const;

	/**@brief ���ô���Բ�Ǵ�С
	 * @param[in] cx Բ�ǿ�
	 * @param[in] cy Բ�Ǹ�
	 */
	void SetRoundCorner(int cx, int cy);

	/**@brief ��ȡ���������Ϣ
	 */
	const UiRect& GetMaximizeInfo() const;

	/**@brief ���ô��������Ϣ
	 * @param[in] rcMaximize Ҫ���õ������ֵ
	 */
	void SetMaximizeInfo(const UiRect& rcMaximize);

	/**@brief ��ȡ͸��ͨ���޲���Χ�ĵľŹ�����������Ӧ XML �� alphafixcorner ����
	 */
	const UiRect& GetAlphaFixCorner() const;

	/**@brief ����͸��ͨ���޲���Χ�ĵľŹ�������
	 * @param[in] rc Ҫ���õ��޲���Χ
	 */
	void SetAlphaFixCorner(const UiRect& rc);

	/// ��Ӱ��ز���
	/**@brief ���ô����Ƿ񸽼���ӰЧ��
	 * @param[in] bShadowAttached Ϊ true ʱ���ӣ�false ʱ������
	 */
	void SetShadowAttached(bool bShadowAttached);

	/**@brief ��ȡ�Ƿ񸽼���ӰЧ��
	*/
	bool IsShadowAttached() const;

	/**@brief ��ȡ��ӰͼƬ
	 */
	const std::wstring& GetShadowImage() const;

	/**@brief ���ô�����ӰͼƬ
	 * @param[in] strImage ͼƬλ��
	 */
	void SetShadowImage(const std::wstring& strImage);

	/**@brief ��ȡ��Ӱ�ľŹ���������Ϣ
	 */
	UiRect GetShadowCorner() const;

	/**@brief ָ����Ӱ�زĵľŹ�������
	 * @param[in] rect �Ź���������Ϣ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 */
	void SetShadowCorner(const UiRect& rect, bool bNeedDpiScale = true);

	/**@brief ��ȡ������С��Χ����Ӧ XML �� mininfo ����
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 */
	UiSize GetMinInfo(bool bContainShadow = false) const;

	/**@brief ���ô�����С��Χ
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 */
	void SetMinInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**@brief ��ȡ�������Χ����Ӧ XML �� maxinfo ����
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 */
	UiSize GetMaxInfo(bool bContainShadow = false) const;

	/**@brief ���ô������Χ
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 */
	void SetMaxInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**@brief ��ȡ���ڳ�ʼ��С
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 */
	UiSize GetInitSize(bool bContainShadow = false) const;

	/**@brief ���ô��ڳ�ʼ��С
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ������ DPI ����
	 */
	void SetInitSize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/** @}*/

public:
	/** @name ���ڲ�����ؽӿ�
	* @{
	*/
	/* @brief ����Ӱ���ӵ�����
	 * @param[in] pRoot ���ڵĶ�������
	 * @return ���ظ�����Ӱ�������ָ��
	 */
	Box* AttachShadow(Box* pRoot);

	/**@brief �󶨴��ڵĶ�������
	 * @param[in] pRoot ����ָ��
	 */
	bool AttachBox(Box* pRoot);

	/**@brief ��ȡ��������������
	*/
	Box* GetRoot() const;

	/**@brief ��ȡ������Դ·��
	*/
	const std::wstring& GetResourcePath() const;

	/**@brief ���ô�����Դ·��
	 * @param[in] strPath Ҫ���õ�·��
	 */
	void SetResourcePath(const std::wstring& strPath);

	/**@brief ��ʼ���ؼ�������������ӿؼ�ʱ�ᱻ���ã����ڶԿؼ����������棩
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] pParent �ؼ�������
	 */
	bool InitControls(Control* pControl, Box* pParent = NULL);

	/**@brief ��ʼ������
	*/
	virtual void OnInitLayout();

	/**@brief ���տؼ�
	 * @param[in] pControl �ؼ�ָ��
	 */
	void ReapObjects(Control* pControl);

	/**@brief �ӳ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 */
	void AddDelayedCleanup(Control* pControl);

	/**@brief ��ȡĬ��������Ϣ
	 */
	TFontInfo* GetDefaultFontInfo();

	/**@brief ���һ��ͨ����ʽ
	 * @param[in] strClassName ͨ����ʽ������
	 * @param[in] strControlAttrList ͨ����ʽ�� XML ת���ʽ����
	 */
	void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList);

	/**@brief ��ȡָ��ͨ����ʽ������
	 * @param[in] strClassName ͨ����ʽ����
	 * @return ����ָ�����Ƶ�ͨ����ʽ���ݣ�XML ת���ʽ����
	 */
	std::wstring GetClassAttributes(const std::wstring& strClassName) const;

	/**@brief ɾ��һ��ͨ����ʽ
	 * @param[in] strClassName Ҫɾ����ͨ����ʽ����
	 */
	bool RemoveClass(const std::wstring& strClassName);

	/**@brief ɾ������ͨ����ʽ
	 */
	void RemoveAllClass();

	/**@brief ���һ����ɫֵ�ṩ������ʹ��
	 * @param[in] strName ��ɫ���ƣ��� white��
	 * @param[in] strValue ��ɫ������ֵ���� #FFFFFFFF��
	 */
	void AddTextColor(const std::wstring& strName, const std::wstring& strValue);

	/**@brief ���һ����ɫֵ�ṩ������ʹ��
	 * @param[in] strName ��ɫ���ƣ��� white��
	 * @param[in] strValue ��ɫ������ֵ���� #FFFFFFFF��
	 */
	void AddTextColor(const std::wstring& strName, DWORD argb);

	/**@brief �������ƻ�ȡһ����ɫ�ľ�����ֵ
	 * @param[in] strName Ҫ��ȡ����ɫ����
	 * @return ���� DWORD ��ʽ����ɫ����ֵ
	 */
	DWORD GetTextColor(const std::wstring& strName) const;

	/**@brief ���һ��ѡ����
	 * @param[in] strGroupName ������
	 * @param[in] pControl �ؼ�ָ��
	 */
	bool AddOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**@brief ��ȡָ��ѡ�����пؼ��б�
	 * @param[in] strGroupName ָ��������
	 * @return ���ظ����µ����пؼ��б�
	 */
	std::vector<Control*>* GetOptionGroup(const std::wstring& strGroupName);

	/**@brief ɾ��һ��ѡ����
	 * @param[in] strGroupName ������
	 * @param[in] pControl �ؼ�����
	 */
	void RemoveOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**@brief ɾ������ѡ����
	 */
	void RemoveAllOptionGroups();

	/** @}*/

public:
	/** @name ������Ϣ��صĽӿ�
	* @{
	*/
	/**@brief ���һ����Ϣ����������ʱ��Ϣ�Ѿ��ɷ����ýӿڵ���Ϣ�������ȼ�����Window�����Ϣ������
	 * @param[in] pFilter һ���̳��� IUIMessageFilter �Ķ���ʵ������Ҫʵ�ֽӿ��еķ���
	 */
	bool AddMessageFilter(IUIMessageFilter* pFilter);

	/**@brief �Ƴ�һ����Ϣ������
	 * @param[in] pFilter һ���̳��� IUIMessageFilter �Ķ���ʵ��
	 */
	bool RemoveMessageFilter(IUIMessageFilter* pFilter);

	/**@brief ������Ϣ���� Windows SendMessage ��һ���װ
	 * @param[in] uMsg ��Ϣ����
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���ش��ڶ���Ϣ�Ĵ�����
	 */
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**@brief Ͷ��һ����Ϣ����Ϣ����
	 * @param[in] uMsg ��Ϣ����
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���ش��ڶ���Ϣ�Ĵ�����
	 */
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**@brief ��������һ����Ϣ, ���͸��ô��ڵ��¼��ص���������m_OnEvent����ע�����Ϣ������
	 * @param[in] eventType ת�������Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 */
	bool SendNotify(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0);

	/**@brief ����ϵͳĬ�ϵĴ��ڴ��������� CallWindowProc API ��һ���װ
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ������Ϣ������
	 */
	LRESULT CallDefaultWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	/**@brief ������Ϣ�Ĵ�����, ��ϵͳ���յ���Ϣ�󣬽���ĵ�һ��������
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ������Ϣ�Ĵ�����
	 */
	LRESULT WindowMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**@brief ������Ϣ���ɷ�����
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @param[out] bHandled ��Ϣ�Ƿ��Ѿ�����
	               ���� true  �����Ѿ��ɹ�������Ϣ������Ҫ�ٴ��ݸ����ڹ��̣�
				   ���� false ��ʾ����Ϣ�������ݸ����ڹ��̴���
	 * @return ������Ϣ�Ĵ�����
	 */
	virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	/**@brief ������Ϣ���ɷ����������ȵ����ڲ�������������ڲ��޴����߼��������WindowsĬ�ϴ������������Ϣ
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @param[out] bHandled ��Ϣ�Ƿ��Ѿ��������� true �����Ѿ��ɹ�������Ϣ��������Ϣ�������ݸ����ڹ���
	 * @return ������Ϣ�Ĵ�����
	 */
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	/** @}*/

private:
	/** @name ˽�д�����Ϣ�������
	* @{
	*/

	/**@brief �ж��Ƿ���Ҫ������������뿪��Ϣ
	 * @param[in] pt ��굱ǰλ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���� true ��Ҫ������������뿪��Ϣ������ false Ϊ����Ҫ
	 */
	bool HandleMouseEnterLeave(const POINT& pt, WPARAM wParam, LPARAM lParam);

	//������Ϣ����������ʵ�ֻ�������
	LRESULT OnCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcActivateMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcCalcSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcHitTestMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnGetMinMaxInfoMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnWindowPosChangingMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnEraseBkGndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnPaintMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	LRESULT OnMouseHoverMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseLeaveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseMoveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseWheelMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnLButtonDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnRButtonDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnLButtonDoubleClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnLButtonUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnRButtonUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	LRESULT OnIMEStartCompositionMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnIMEEndCompositionMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSetFocusMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKillFocusMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnContextMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnCharMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKeyDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKeyUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSetCusorMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNotifyMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnCommandMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnCtlColorMsgs(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	LRESULT OnTouchMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnPointerMsgs(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);	

	/** @}*/

public:
	/** @name ���ڽ�����ؽӿ�
	* @{
	*/
	/**@brief ��ȡ��ǰ���н���Ŀؼ�
	 */
	Control* GetFocus() const;

	/**@brief ���ý��㵽ָ���ؼ���(���ô���Ϊ���㴰�ڣ������øÿؼ�Ϊ����ؼ�)
	 * @param[in] pControl �ؼ�ָ��
	 */
	void SetFocus(Control* pControl);

	/**@brief �ÿؼ�ʧȥ���㣨��Ӱ�촰�ڽ��㣩
	 */
	void KillFocus();

	/**@brief ���õ�Ҫ�������괰�ھ��Ϊ��ǰ���ƴ���
	 * @param[in]
	 */
	void SetCapture();

	/**@brief �����ڲ���Ҫ�������ʱ�ͷ���Դ
	 */
	void ReleaseCapture();

	/**@brief �ж�ָ���ؼ��Ƿ񱻵���򴥿�
	 * @paran[in] pControl �ؼ�ָ��
	 */
	bool IsCaptureControl(const Control* pControl) const;

	/**@brief �жϵ�ǰ�Ƿ񲶻��������
	 */
	bool IsCaptured() const;

	/**@brief ��ȡ��ǰ������ĸ��ؼ���
	 */
	Control* GetHoverControl() const;

	/**@brief ��ȡ�����������
	 */
	const POINT& GetLastMousePos() const;

	/**@brief �����Ƿ�������Ϣ(������֧��)
	 * @param[in] bHandle ����Ϊ true ��ʾ����false Ϊ������
	 */
	void SetHandlePointerMsg(bool bHandle) { m_bHandlePointerMsg = bHandle; };

	/**@brief �л��ؼ����㵽��һ��������һ�����ؼ�
	 * @param[in] bForward true Ϊ��һ���ؼ�������Ϊ false��Ĭ��Ϊ true
	 */
	bool SetNextTabControl(bool bForward = true);

protected:
	/**@brief �ͷ�ָ���ؼ��İ���״̬
	 * @param[in] bClickOrPointer �����ؼ����Ǵ����ؼ�
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 */
	void ReleaseEventClick(bool bClickOrPointer, WPARAM wParam, LPARAM lParam);

	/** @}*/

public:
	/** @name ���ڻ�����ؽӿ�
	* @{
	*/
	/**@brief ���ÿؼ��Ƿ��Ѿ�����
	 * @param[in] bArrange true Ϊ�Ѿ����У�����Ϊ false
	 */
	void SetArrange(bool bArrange);

	/**@brief �����ػ���Ϣ
	 * @param[in] rcItem �ػ淶Χ
	 */
	void Invalidate(const UiRect& rcItem);

	/**@brief ��ȡ�������� DC
	 */
	HDC GetPaintDC() const;

	/**@brief ��ȡ���ƶ���
	 */
	IRenderContext* GetRenderContext() const;

	/**@brief ���ƺ�����
	 */
	void Paint();

	/**@brief ����͸����
	 * @param[in] nAlpha ͸������ֵ[0, 255]
	 */
	void SetAlpha(int nAlpha);

	/**@brief �жϵ�ǰ�Ƿ���Ⱦ͸��ͼ��
	 */
	bool IsRenderTransparent() const;

	/**@brief ������Ⱦ͸��ͼ��
	 * @param[in] bCanvasTransparent ���� true Ϊ��Ⱦ͸��ͼ�㣬����Ϊ false
	 */
	bool SetRenderTransparent(bool bCanvasTransparent);

	/**@brief �Ƿ�Ϊ�㴰��
	*/
	bool IsLayeredWindow() const;

	/**@brief ����ͼƬ����
	*/
	void ClearImageCache();

	/** @} */

private:
	/**@name ����Ч����ؽӿ�
	* @{
	*/
	/**@brief ���û���ƫ��
	 * @param[in] renderOffset ƫ��ֵ
	 */
	void SetRenderOffset(UiPoint renderOffset);

	/**@brief ���û���ƫ�� x ����
	 * @param[in] renderOffsetX ����ֵ
	 */
	void SetRenderOffsetX(int renderOffsetX);

	/**@brief ���û���ƫ�� y ����
	 * @param[in] renderOffsetY ����ֵ
	 */
	void SetRenderOffsetY(int renderOffsetY);

	/** @} */

public:

	/**@name �ؼ�������ؽӿ�
	* @{
	*/
	/**@brief �����������ָ���ؼ�������Ĭ�����ԣ�UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST
	 * @param[in] pt ָ������
	 */
	Control* FindControl(const POINT& pt) const;

	/**
	 * @brief ����������ҿ�����ӦWM_CONTEXTMENU�Ŀؼ�
	 * @param[in] pt ָ������
	 */
	Control* FindContextMenuControl(const POINT* pt) const;

	/**@brief ���ݿؼ����Ʋ��ҿؼ�
	 * @param[in] strName �ؼ�����
	 */
	Control* FindControl(const std::wstring& strName) const;

	/**@brief ������������ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] pt Ҫ���ҵ�����
	 */
	Control* FindSubControlByPoint(Control* pParent, const POINT& pt) const;

	/**@brief �������ֲ����ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] strName Ҫ���ҵ�����
	 */
	Control* FindSubControlByName(Control* pParent, const std::wstring& strName) const;

	/** @} */

private:
	/** ��鲢ȷ����ǰ����Ϊ���㴰��
	*/
	void CheckSetFocusWindow();

#if defined(ENABLE_UIAUTOMATION)
public:
	/** @brief Get ui automation provider
	 */
	UIAWindowProvider* GetUIAProvider();
#endif

private:
	//���ھ��
	HWND m_hWnd;

	//ԭ���Ĵ��ڹ��̺���
	WNDPROC m_OldWndProc;

	//�Ƿ����໯��Subclass������
	bool m_bSubclassed;

	//�¼��ص�������
	EventMap m_OnEvent;

private:
	//���ڵĳ�ʼ����С
	UiSize m_szInitWindowSize;

	//������С��Ϣ
	UiSize m_szMinWindow;

	//���������Ϣ
	UiSize m_szMaxWindow;	

	//���������Ϣ
	UiRect m_rcMaximizeInfo;

	//�����ı߿����췶Χ��Ϣ
	UiRect m_rcSizeBox;

	//͸��ͨ���޲���Χ�ĵľŹ�������
	UiRect m_rcAlphaFix;

	//����Բ����Ϣ
	UiSize m_szRoundCorner;

	//������������Ϣ
	UiRect m_rcCaption;

	//�����Ƿ�仯������仯(true)����Ҫ���¼��㲼��
	bool m_bIsArranged;

	//�����Ƿ���Ҫ��ʼ��
	bool m_bFirstLayout;

private:
	//����DC
	HDC m_hDcPaint;

	//�Ƿ�Ϊ�㴰��
	bool m_bIsLayeredWindow;

	//����͸����
	int m_nAlpha;

	//����ʱ��ƫ�����������ã�
	UiPoint m_renderOffset;

	//��������
	std::unique_ptr<IRenderContext> m_renderContext;

private:

	//��������
	std::unique_ptr<ToolTip> m_toolTip;
	
	//����ؼ�
	Control* m_pFocus;
	
	//�������ͣ�ؼ�
	Control* m_pEventHover;

	/** ����Ŀؼ���
		�� WM_LBUTTONDOWN/WM_RBUTTONDOWN/WM_LBUTTONDBLCLK ��ֵ
		�� WM_LBUTTONUP��/WM_KILLFOCUS ���
	*/
	Control* m_pEventClick;

	/** ���̰��µĿؼ�
	*   �� WM_KEYDOWN ��ֵ
	*   �� WM_KEYUP ���
	*/
	Control* m_pEventKey;

	//�������¼���صĿؼ�
	Control* m_pEventPointer;

	//�����Ƿ�������Ϣ(������֧��)
	bool m_bHandlePointerMsg;

	//�������λ��
	UiPoint m_ptLastMousePos;

	//����¼��Ĳ���״̬
	bool m_bMouseCapture;

private:
	//ÿ�����ڵ���Դ·��(�������Դ��Ŀ¼��·��)
	std::wstring m_strResourcePath;

	//Ĭ����������
	TFontInfo m_defaultFontInfo;

	//����������class����������ӳ���ϵ
	std::map<std::wstring, std::wstring> m_defaultAttrHash;

	//������ɫ�ַ�������ɫֵ��ӳ���ϵ
	std::map<std::wstring, DWORD> m_mapTextColor;

	//�ô�����ÿ��Option group�µĿؼ�������ѡ�ؼ��Ƿ���ģ�
	std::map<std::wstring, std::vector<Control*>> m_mOptionGroup;

	//�ô�����Ϣ�������б�
	std::vector<IUIMessageFilter*> m_aMessageFilters;

	//�ӳ��ͷŵĿؼ��ӿ�
	std::vector<Control*> m_aDelayedCleanup;

	//�첽�رմ��ڻص�������Դ��Ч�Ա�־
	nbase::WeakCallbackFlag m_closeFlag;
	
	//������Ӱ
	std::unique_ptr<Shadow> m_shadow;

	//��ǰ�����Ƿ���ʾΪģ̬�Ի���
	bool m_bFakeModal = false;
	
	//�����Ѿ��ӳٹرգ�add by djj 20200428 ����Closeʱ���ӳ�Post WM_CLOSE, ����ڼ���Ҫ��һ����ʶ����'���ر�״̬'
	bool m_bCloseing = false;

private:
	/** �ؼ����Ҹ�����
	*/
	ControlFinder m_controlFinder;

	/** ���ڹ��������������ڵ�
	*/
	Box* m_pRoot;

protected:
#if defined(ENABLE_UIAUTOMATION)
	UIAWindowProvider* m_pUIAProvider;
#endif
};

} // namespace ui

#endif // UI_CORE_WINDOW_H_
