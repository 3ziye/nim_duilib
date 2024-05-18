/** @file CircleProgress.h
* @brief ���ͽ������ؼ���Բ���м�������ı�����85%��
* @copyright (c) 2019-2022, NetEase Inc. All rights reserved
* @author Xuhuajie
* @date 2019/8/14
*/

#ifndef UI_CONTROL_CIRCLEPROGRESS_H_
#define UI_CONTROL_CIRCLEPROGRESS_H_

#pragma once

#include "duilib/Control/Progress.h"

namespace ui
{

class UILIB_API CircleProgress : public Progress
{
public:
	explicit CircleProgress(Window* pWindow);
	virtual ~CircleProgress();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStateImages(IRender* pRender) override;
	virtual void ClearImageCache() override;

	/** DPI�����仯�����¿ؼ���С�Ͳ���
	* @param [in] nOldDpiScale �ɵ�DPI���Űٷֱ�
	* @param [in] nNewDpiScale �µ�DPI���Űٷֱȣ���Dpi().GetScale()��ֵһ��
	*/
	virtual void ChangeDpiScale(uint32_t nOldDpiScale, uint32_t nNewDpiScale) override;

	/** ���û��ͽ�������Ĭ��Ϊ��ͨ������
	* @param [in] bCircular Ϊ true ʱ����Ϊ���ͽ�������false ʱ����Ϊ������������ʽ��Ĭ��Ϊ true
	*/
	void SetCircular(bool bCircular = true);

	/** ���õ�������
	* @param [in] bClockwise Ϊ true ʱ����Ϊ˳ʱ�룬false ʱ����Ϊ��ʱ�룬Ĭ��Ϊ true
	*/
	void SetClockwiseRotation(bool bClockwise = true);

	/** ����Բ�����
	* @param [in] nCircleWidth �����ֵ
	* @param [in] bNeedDpiScale �Ƿ�֧��DPI����
	*/
	void SetCircleWidth(int32_t nCircleWidth, bool bNeedDpiScale);

	/** ��ȡԲ�����
	*/
	int32_t GetCircleWidth() const;

	/** ���ý�����������ɫ
	* @param [in] strColorҪ���õı�����ɫ�ַ��������ַ��������� global.xml �д���
	*/
	void SetBackgroudColor(const std::wstring& strColor);

	/** ���ý�����ǰ����ɫ
	* @param [in] strColorҪ���õ�ǰ����ɫ�ַ��������ַ��������� global.xml �д���
	*/
	void SetForegroudColor(const std::wstring& strColor);

	/** ���ý�����ǰ��������ɫ���� SetForegroudColor ͬʱʹ�ã����Բ�����,���޽���Ч��
	* @param [in] strColorҪ���õ�ǰ��������ɫ�ַ��������ַ��������� global.xml �д���
	*/
	void SetCircleGradientColor(const std::wstring& strColor);

	/** ���ý���ָʾ�ƶ�ͼ��
	* @param [in] sIndicatorImage Ҫ���õ�ͼƬ
	*/
	void SetIndicator(const std::wstring& sIndicatorImage);

protected:
	bool m_bCircular;
	bool m_bClockwise;
	int m_nCircleWidth;
	UiColor m_dwBackgroundColor;
	UiColor m_dwForegroundColor;
	UiColor m_dwGradientColor;
	Image* m_pIndicatorImage;
};

} // namespace ui

#endif // UI_CONTROL_CIRCLEPROGRESS_H_
