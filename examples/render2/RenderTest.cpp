#include "RenderTest.h"

namespace ui {

RenderTest::RenderTest()
{

}

RenderTest::~RenderTest()
{

}

void RenderTest::AlphaPaint(IRender* pRender, const UiRect& rcPaint)
{
    __super::AlphaPaint(pRender, rcPaint);
}

void RenderTest::Paint(IRender* pRender, const UiRect& rcPaint)
{
    const int marginSize = 8;
    UiRect rect = GetRect();
    rect.left += marginSize;
    rect.top += 4;

    int nSize = 50;
    rect.right = rect.left + nSize;
    rect.bottom = rect.top + nSize;
    int currentBottom = rect.bottom;//��¼��ǰ��bottomֵ

    //svg��ʽ����
    Image svgImage;
    svgImage.SetImageString(L"svg_test.svg");
    LoadImageData(svgImage);
    UiRect svgRect = rect;
    svgRect.right = svgRect.left + svgImage.GetImageCache()->GetWidth();
    svgRect.bottom = svgRect.top + svgImage.GetImageCache()->GetHeight();
    UiRect svgImageSource(0, 0, svgImage.GetImageCache()->GetWidth(), svgImage.GetImageCache()->GetHeight());
   // pRender->DrawImage(rcPaint, svgImage.GetCurrentBitmap(), false, svgRect, svgImageSource, UiRect());

    //����
    rect = GetRect();
    rect.left += marginSize;
    rect.top = currentBottom + marginSize;
}

void RenderTest::PaintChild(IRender* pRender, const UiRect& rcPaint)
{
    __super::PaintChild(pRender, rcPaint);
}

} //end of namespace ui