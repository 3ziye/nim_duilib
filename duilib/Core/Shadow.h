#ifndef UI_CORE_SHADOW_H_
#define UI_CORE_SHADOW_H_

#pragma once

#include "duilib/Core/UiSize.h"
#include "duilib/Core/UiRect.h"
#include <string>

namespace ui 
{

class Box;
class Control;
class Window;
class DpiManager;

/** 窗口阴影属性
*/
class UILIB_API Shadow
{
public:
    /** Attach的Box圆角属性（Attach后，需要修改原来Box的圆角属性）
    * @param [in] pBox 关联的容器，用于处理DPI缩放
    */
    static UiSize GetChildBoxBorderRound(Box* pBox);

public:
    /** 构造函数
     @param [in] pWindow 关联的窗口
    */
    explicit Shadow(Window* pWindow);

    /** 设置是否支持阴影效果
     * @param[in] bShadowAttached 设置 true 为支持阴影效果，false 为不支持阴影效果
     */
    void SetShadowAttached(bool bShadowAttached);

    /** 判断是否已经支持阴影效果
     */
    bool IsShadowAttached() const;

    /** 当前阴影效果值，是否为默认值
    */
    bool IsUseDefaultShadowAttached() const;

    /** 设置当前阴影效果值，是否为默认值
    */
    void SetUseDefaultShadowAttached(bool isDefault);

    /** 设置阴影的九宫格属性
     * @param[in] rc 要设置的九宫格属性
     * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
     */
    void SetShadowCorner(const UiPadding&rc, bool bNeedDpiScale);

    /** 获取阴影的九宫格属性
     */
    UiPadding GetShadowCorner() const;

    /** 重置为默认阴影效果
    */
    void ResetDefaultShadow();

    /** 设置阴影图片属性
     */
    void SetShadowImage(const std::wstring& image);

    /** 获取阴影图片属性
     */
    const std::wstring& GetShadowImage() const;

    /** 将阴影附加到窗口
     * @param[in] pRoot 窗口的顶层容器
     */
    Box* AttachShadow(Box* pRoot);

    /** 设置窗口最大化还是还原状态
     * @param[in] isMaximized 设置为 true 表示最大化，false 为还原初始状态
     */
    void MaximizedOrRestored(bool isMaximized);

    /** 获取附加阴影后的容器指针
     */
    Control* GetRoot();

    /** 清理图片缓存
     */
    void ClearImageCache();

    /** DPI发生变化，更新控件大小和布局
    * @param [in] dpi DPI管理器
    * @param [in] nOldDpiScale 旧的DPI缩放百分比
    * @param [in] nNewDpiScale 新的DPI缩放百分比，与Dpi().GetScale()的值一致
    */
    void ChangeDpiScale(const DpiManager& dpi, uint32_t nOldDpiScale, uint32_t nNewDpiScale);

private:
    //是否支持阴影效果
    bool m_bShadowAttached;

    //当前阴影效果值，是否为默认值
    bool m_bUseDefaultShadowAttached;

    //阴影图片属性
    std::wstring m_strImage;

    //当前阴影图片属性，是否为默认值
    bool m_bUseDefaultImage;

    //阴影圆角属性
    UiPadding m_rcShadowCorner;
    UiPadding m_rcShadowCornerBackup;

    //Root容器接口
    Box* m_pRoot;

    //关联的窗口
    Window* m_pWindow;
};

}

#endif // UI_CORE_SHADOW_H_