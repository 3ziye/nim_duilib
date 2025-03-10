/**@brief 封装Cef浏览器对象为duilib控件
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#ifndef UI_CEF_CONTROL_CEF_CONTROL_H_
#define UI_CEF_CONTROL_CEF_CONTROL_H_

#include "duilib/Core/Window.h"
#include "duilib/CEFControl/CefControlBase.h"

namespace ui {

class CefMemoryDC;

/** duilib的CEF控件，离屏渲染模式
*/
class CefControlOffScreen :public CefControlBase, public ui::IUIMessageFilter
{
    typedef CefControlBase BaseClass;
public:
    explicit CefControlOffScreen(ui::Window* pWindow);
    ~CefControlOffScreen(void);    

    /// 重写父类接口，提供个性化功能
    virtual void Init() override;
    virtual void SetPos(ui::UiRect rc) override;
    virtual void HandleEvent(const ui::EventArgs& msg) override;
    virtual void SetVisible(bool bVisible) override;
    virtual void Paint(ui::IRender* pRender, const ui::UiRect& rcPaint) override;
    virtual void SetWindow(ui::Window* pWindow) override;

    virtual LRESULT FilterMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override; // 处理窗体消息，转发到Cef浏览器对象

    /**
    * @brief 打开开发者工具
    * @param[in] view 一个 CefControlOffScreen 控件实例(仅在CefControlOffScreen类里需要传入)
    * @return 成功返回 true，失败返回 false
    */
    virtual bool AttachDevTools(Control* view) override;

    /**
    * @brief 关闭开发者工具
    * @return 无
    */
    virtual void DettachDevTools() override;

protected:
    virtual void ReCreateBrowser() override;

    // 在非UI线程中被调用
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;

protected:
    /**
     * @brief 转发鼠标按下消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendButtonDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发鼠标双击消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendButtonDoubleDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发鼠标弹起消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendButtonUpEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发鼠标移动消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendMouseMoveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发鼠标滚动消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendMouseWheelEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发鼠标离开消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendMouseLeaveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发键盘相关消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /**
     * @brief 转发捕获焦点消息到 BrowserHost
     * @param[in] uMsg 消息
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 是否继续传递消息
     * @return 返回消息处理结果
     */
    LRESULT SendCaptureLostEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    
    /**
     * @brief 判断是否有按键按下
     * @param[in] wparam 消息附加参数
     * @return 返回 true 表示有按键按下，false 表示没有按键按下
     */
    static bool IsKeyDown(WPARAM wparam);

    /**
     * @brief 转换普通键盘消息到 CEF 可识别的键盘消息
     * @param[in] wparam 消息附加参数
     * @param[in] lparam 消息附加参数
     * @return 返回转换后的结果
     */
    static int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);

    /**
     * @brief 转换普通鼠标消息到 CEF 可识别的鼠标消息
     * @param[in] wparam 消息附加参数
     * @return 返回转换后的结果
     */
    static int GetCefMouseModifiers(WPARAM wparam);

private:
    /** CefRenderHandler接口, 在非UI线程中被调用
    *   当浏览器渲染数据变化时，会触发此接口，此时把渲染数据保存到内存dc
        并且通知窗体刷新控件，在控件的Paint函数里把内存dc的位图画到窗体上
        由此实现离屏渲染数据画到窗体上
    */
    virtual void OnPaint(CefRefPtr<CefBrowser> browser, CefRenderHandler::PaintElementType type,
                         const CefRenderHandler::RectList& dirtyRects, const std::string* buffer,
                         int width, int height) override;//CefRenderHandler接口
    virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;//CefRenderHandler接口
    virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;//CefRenderHandler接口

    /** 客户区坐标转换为控件坐标
    */
    virtual void ClientToControl(UiPoint& pt) override;

    //处理DPI自适应（离屏渲染模式与正常模式不同）
    void AdaptDpiScale(CefMouseEvent& mouse_event);

private:
    // 内存dc,把cef离屏渲染的数据保存到dc中
    std::unique_ptr<CefMemoryDC> m_pCefDC;

    // 内存dc,把cef的popup窗口的离屏渲染数据保存到dc中
    std::unique_ptr<CefMemoryDC> m_pCefPopupDC;

    // 当网页的组合框一类的控件弹出时，记录弹出的位置
    CefRect m_rectPopup;

    //开发者工具对应的控件
    CefControlOffScreen* m_pDevToolView;
};

}

#endif //UI_CEF_CONTROL_CEF_CONTROL_H_
