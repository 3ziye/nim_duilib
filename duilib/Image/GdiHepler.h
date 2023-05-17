#pragma once

#include "duilib/duilib_defs.h"

namespace ui 
{
    namespace GdiHelper 
    {

        //��ͼ��������ΪԲ�㣬��ת�Ƕ�dAngle(0 - 360��)
        HBITMAP RotateBitmapAroundCenter(HBITMAP hBitmap, double dAngle);

        //��ȡBitmap�Ŀ�͸�
        bool GetBitmapWidthHeight(HBITMAP hBitmap, int& imageWidth, int& imageHeight);
    }

}
