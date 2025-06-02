#ifndef UI_CORE_UISIZE16_H_
#define UI_CORE_UISIZE16_H_

#include "duilib/duilib_defs.h"
#include <cstdint>

namespace ui 
{

/** Size的封装
*/
class UILIB_API UiSize16
{
public:
    UiSize16()
    {
        cx = cy = 0;
    }

    UiSize16(const UiSize16& src)
    {
        cx = src.cx;
        cy = src.cy;
    }

    UiSize16(int16_t cxValue, int16_t cyValue)
    {
        cx = cxValue;
        cy = cyValue;
    }

    UiSize16& operator=(const UiSize16& src)
    {
        cx = src.cx;
        cy = src.cy;
        return *this;
    }

    /** 设置新的宽度和高度
    */
    void SetWidthHeight(int16_t cxValue, int16_t cyValue)
    {
        cx = cxValue;
        cy = cyValue;
    }

    /** 设置新的宽度
    */
    void SetWidth(int16_t cxValue) { cx = cxValue; }

    /** 设置新的高度
    */
    void SetHeight(int16_t cyValue) { cy = cyValue; }

    /** 返回宽度
    */
    constexpr int16_t Width() const { return cx; }

    /** 返回高度
    */
    constexpr int16_t Height() const { return cy; }

    /** 检查宽度和高度值是否为有效值，并修正
    */
    void Validate()
    {
        if (cx < 0) {
            cx = 0;
        }
        if (cy < 0) {
            cy = 0;
        }
    }

    /** 高度和宽度是否为有效值
    */
    bool IsValid() const { return cx >= 0 && cy >= 0; }

    /** 如果cx和cy都是0，返回true
    */
    bool IsEmpty() const { return cx == 0 && cy == 0; }

    /** cx和cy都设置为0
    */
    void Clear() 
    { 
        cx = 0; 
        cy = 0; 
    }

    /** 判断是否与另外一个Size相同
    */
    bool Equals(int16_t cxValue, int16_t cyValue) const
    {
        return cx == cxValue && cy == cyValue;
    }

    /** 判断是否与另外一个Size相同
    */
    bool Equals(const UiSize16& dst) const
    {
        return cx == dst.cx && cy == dst.cy;
    }

    /** 判断两个值是否相等
    */
    friend bool operator == (const UiSize16& a, const UiSize16& b)
    {
        return a.Equals(b);
    }

    /** 判断两个值是否不相等
    */
    friend bool operator != (const UiSize16& a, const UiSize16& b)
    {
        return !a.Equals(b);
    }

public:
    /** 宽度
    */
    int16_t cx;

    /** 高度
    */
    int16_t cy;
};

} // namespace ui

#endif // UI_CORE_UISIZE16_H_
