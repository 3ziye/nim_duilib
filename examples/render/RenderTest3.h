#pragma once

// duilib
#include "duilib/duilib.h"

namespace ui {

class RenderTest3 : public ui::Control
{
public:
	RenderTest3();
	virtual ~RenderTest3();

public:
	/**
	* @brief ������
	* @param[in] ������
	* @return ������
	*/
	virtual void AlphaPaint(IRender* pRender, const UiRect& rcPaint);

	/**
	* @brief ���ƿؼ�����ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void Paint(IRender* pRender, const UiRect& rcPaint);

	/**
	* @brief ���ƿؼ�������ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void PaintChild(IRender* pRender, const UiRect& rcPaint);

private:
	int DpiScaledInt(int iValue);
};

} //end of namespace ui
