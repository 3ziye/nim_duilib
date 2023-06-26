#ifndef UI_RENDER_SKIA_DRAW_TEXT_H_
#define UI_RENDER_SKIA_DRAW_TEXT_H_

#pragma once

#include "duilib/duilib_defs.h"

class SkFont;
class SkPaint;

namespace ui 
{

class UILIB_API DrawText_Skia
{
public:
	/** ����ָ���ı��ַ����Ŀ�Ⱥ͸߶�
	* @param [in] strText ��������
	* @param [in] skFont ����, ��������nullptr
	* @param [in] skPaint ���ƽӿڣ��������ֵ���ʽ��������Ϣ, ������nullptr
	* @param [in] isSingleLineMode true��ʾ����ģʽ������Ϊ����ģʽ
	* @param [in] spacingMul �м�࣬Ĭ��Ϊ1.0������ͨ������������ö���ģʽ�µ��м�࣬����1.2��ʾ1.2���м��
	* @param [in] width ��ǰ��������ƿ�ȣ��������������ʾ���ƿ�ȣ��������ƿ��	
	* @return �����ı��ַ����Ŀ�Ⱥ͸߶ȣ��Ծ��α�ʾ���
	*/
	static UiRect MeasureString(const std::wstring& strText, 
		                        const SkFont& skFont,
		                        const SkPaint& skPaint,
		                        bool isSingleLineMode,
		                        float spacingMul,
		                        int width);
};

} // namespace ui

#endif // UI_RENDER_SKIA_DRAW_TEXT_H_
