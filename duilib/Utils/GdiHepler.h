#pragma once

#include "duilib/duilib_defs.h"

namespace Gdiplus {
    class Bitmap;
}

namespace ui 
{
    namespace GdiHelper 
    {

        Gdiplus::Bitmap* CreateBitmapFromHBITMAP(HBITMAP hBitmap);

        //��ͼ��������ΪԲ�㣬��ת�Ƕ�dAngle(0 - 360��)
        HBITMAP RotateBitmapAroundCenter(HBITMAP hBitmap, double dAngle);

        //��ȡBitmap�Ŀ�͸�
        bool GetBitmapWidthHeight(HBITMAP hBitmap, int& imageWidth, int& imageHeight);
    }

}
