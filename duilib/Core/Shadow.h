#ifndef UI_CORE_SHADOW_H_
#define UI_CORE_SHADOW_H_

#pragma once

#include "duilib/duilib_defs.h"

namespace ui 
{

class Box;
class Control;

/** ������Ӱ����
*/
class UILIB_API Shadow
{
public:
	/** Attach��BoxԲ�����ԣ�Attach����Ҫ�޸�ԭ��Box��Բ�����ԣ�
	*/
	static UiSize GetChildBoxBorderRound();

public:
	Shadow();

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
	 * @param[in] rect Ҫ���õľŹ�������
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 */
	void SetShadowCorner(const UiRect &rect, bool bNeedDpiScale = true);

	/** ��ȡ��Ӱ�ľŹ�������
	 */
	UiRect GetShadowCorner() const;

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
	UiRect m_rcShadowCorner;
	UiRect m_rcShadowCornerBackup;

	//Root�����ӿ�
	Box* m_pRoot;
};

}

#endif // UI_CORE_SHADOW_H_