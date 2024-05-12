#ifndef UI_RENDER_RENDER_CONFIG_H_
#define UI_RENDER_RENDER_CONFIG_H_

#pragma once

namespace ui
{

/** Skia����
*/
#define duilib_kRenderType_Skia     0

/** GdiPlus����
*/
#define duilib_kRenderType_GdiPlus  1

/** ��ǰѡ�����Ⱦ����(ͨ���޸Ĵ�ֵ��������֧���ĸ�����)
*/
#define duilib_kRenderType          duilib_kRenderType_Skia

//��������꣬��ʾʹ��LLVM����Skia�������ʾʹ��Visual Studio 2022����Skia
#define DUILIB_RENDER_SKIA_BY_LLVM  1

}//namespace ui

#endif // UI_RENDER_RENDER_CONFIG_H_
