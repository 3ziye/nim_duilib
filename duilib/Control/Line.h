#ifndef UI_CONTROL_LINE_H_
#define UI_CONTROL_LINE_H_

#pragma once

#include "duilib/Core/Control.h"

namespace ui
{
/** ���߿ؼ�
*/
class Line: public Control
{
public:
	explicit Line(Window* pWindow);

	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/** DPI�����仯�����¿ؼ���С�Ͳ���
	* @param [in] nOldDpiScale �ɵ�DPI���Űٷֱ�
	* @param [in] nNewDpiScale �µ�DPI���Űٷֱȣ���Dpi().GetScale()��ֵһ��
	*/
	virtual void ChangeDpiScale(uint32_t nOldDpiScale, uint32_t nNewDpiScale) override;

	/** ����ˮƽ���Ǵ�ֱ(true��ʾ��ֱ)
	*/
	void SetLineVertical(bool bVertical);

	/** ��ȡˮƽ���Ǵ�ֱ
	*/
	bool IsLineVertical() const;

	/** ���������Ŀ��
	* @param [in] lineWidth �������
	* @param [in] bNeedDpiScale �Ƿ�֧��DPI����
	*/
	void SetLineWidth(int32_t lineWidth, bool bNeedDpiScale);

	/** ��ȡ�����Ŀ��
	*/
	int32_t GetLineWidth() const;

	/** ����������ɫ
	*/
	void SetLineColor(const std::wstring& lineColor);

	/** ��ȡ������ɫ
	*/
	std::wstring GetLineColor() const;

	/** ������������
	*/
	void SetLineDashStyle(const std::wstring& dashStyle);

	/** ��ȡ��������
	*/
	std::wstring GetLineDashStyle() const;

protected:
	/** ���ƿؼ�����ں���
	* @param [in] pRender ָ����������
	* @param [in] rcPaint ָ����������
	*/
	virtual void Paint(IRender* pRender, const UiRect& rcPaint) override;

private:
	/** ˮƽ���Ǵ�ֱ(true��ʾ��ֱ)
	*/
	bool m_bLineVertical;

	/** ������ɫ
	*/
	UiString m_lineColor;

	/** �������
	*/
	int32_t m_lineWidth;

	/** ��������
	*/
	int32_t m_dashStyle;
};

}//namespace ui

#endif //UI_CONTROL_LINE_H_
