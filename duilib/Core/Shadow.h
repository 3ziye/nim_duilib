#ifndef UI_CORE_SHADOW_H_
#define UI_CORE_SHADOW_H_

#pragma once

#include "duilib/Core/UiSize.h"
#include "duilib/Core/UiRect.h"
#include <string>

namespace ui 
{

class Box;
class Control;
class Window;
class DpiManager;

/** ������Ӱ����
*/
class UILIB_API Shadow
{
public:
	/** Attach��BoxԲ�����ԣ�Attach����Ҫ�޸�ԭ��Box��Բ�����ԣ�
	* @param [in] pBox ���������������ڴ���DPI����
	*/
	static UiSize GetChildBoxBorderRound(Box* pBox);

public:
	/** ���캯��
	 @param [in] pWindow �����Ĵ���
	*/
	explicit Shadow(Window* pWindow);

	/** �����Ƿ�֧����ӰЧ��
	 * @param[in] bShadowAttached ���� true Ϊ֧����ӰЧ����false Ϊ��֧����ӰЧ��
	 */
	void SetShadowAttached(bool bShadowAttached);

	/** �ж��Ƿ��Ѿ�֧����ӰЧ��
	 */
	bool IsShadowAttached() const;

	/** ��ǰ��ӰЧ��ֵ���Ƿ�ΪĬ��ֵ
	*/
	bool IsUseDefaultShadowAttached() const;

	/** ���õ�ǰ��ӰЧ��ֵ���Ƿ�ΪĬ��ֵ
	*/
	void SetUseDefaultShadowAttached(bool isDefault);

	/** ������Ӱ�ľŹ�������
	 * @param[in] rc Ҫ���õľŹ�������
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 */
	void SetShadowCorner(const UiPadding&rc, bool bNeedDpiScale);

	/** ��ȡ��Ӱ�ľŹ�������
	 */
	UiPadding GetShadowCorner() const;

	/** ����ΪĬ����ӰЧ��
	*/
	void ResetDefaultShadow();

	/** ������ӰͼƬ����
	 */
	void SetShadowImage(const std::wstring& image);

	/** ��ȡ��ӰͼƬ����
	 */
	const std::wstring& GetShadowImage() const;

	/** ����Ӱ���ӵ�����
	 * @param[in] pRoot ���ڵĶ�������
	 */
	Box* AttachShadow(Box* pRoot);

	/** ���ô�����󻯻��ǻ�ԭ״̬
	 * @param[in] isMaximized ����Ϊ true ��ʾ��󻯣�false Ϊ��ԭ��ʼ״̬
	 */
	void MaximizedOrRestored(bool isMaximized);

	/** ��ȡ������Ӱ�������ָ��
	 */
	Control* GetRoot();

	/** ����ͼƬ����
	 */
	void ClearImageCache();

	/** DPI�����仯�����¿ؼ���С�Ͳ���
	* @param [in] dpi DPI������
	* @param [in] nOldDpiScale �ɵ�DPI���Űٷֱ�
	* @param [in] nNewDpiScale �µ�DPI���Űٷֱȣ���Dpi().GetScale()��ֵһ��
	*/
	void ChangeDpiScale(const DpiManager& dpi, uint32_t nOldDpiScale, uint32_t nNewDpiScale);

private:
	//�Ƿ�֧����ӰЧ��
	bool m_bShadowAttached;

	//��ǰ��ӰЧ��ֵ���Ƿ�ΪĬ��ֵ
	bool m_bUseDefaultShadowAttached;

	//��ӰͼƬ����
	std::wstring m_strImage;

	//��ǰ��ӰͼƬ���ԣ��Ƿ�ΪĬ��ֵ
	bool m_bUseDefaultImage;

	//��ӰԲ������
	UiPadding m_rcShadowCorner;
	UiPadding m_rcShadowCornerBackup;

	//Root�����ӿ�
	Box* m_pRoot;

	//�����Ĵ���
	Window* m_pWindow;
};

}

#endif // UI_CORE_SHADOW_H_