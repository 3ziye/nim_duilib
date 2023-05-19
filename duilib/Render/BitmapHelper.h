#ifndef UI_RENDER_BITMAP_HELPER_H_
#define UI_RENDER_BITMAP_HELPER_H_

#pragma once

#include "duilib/Render/IRender.h"

namespace ui 
{
    namespace BitmapHelper 
    {

        /** ��ͼ��������ΪԲ�㣬��ת�Ƕ�dAngle(0 - 360��)
        * @param [in] pBitmap ��Ҫ��ת��λͼ�ӿ�
        * @param [in] dAngle ��Ҫ��ת�ĽǶ�(0 - 360��)
        * @return ���������ɵ�λͼ�ӿڣ���Դ�ɵ��÷������ͷ�
        */
        IBitmap* RotateBitmapAroundCenter(const IBitmap* pBitmap, double dAngle);
    }
}

#endif //UI_RENDER_BITMAP_HELPER_H_

