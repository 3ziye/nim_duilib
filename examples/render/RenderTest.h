#pragma once

// duilib
#include "duilib/duilib.h"

namespace ui {

class RenderTest : public ui::Control
{
public:
	RenderTest();
	virtual ~RenderTest();

public:
	/**
	* @brief ������
	* @param[in] ������
	* @return ������
	*/
	virtual void AlphaPaint(IRenderContext* pRender, const UiRect& rcPaint);

	/**
	* @brief ���ƿؼ�����ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint);

	/**
	* @brief ���ƿؼ�������ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint);

};

} //end of namespace ui
