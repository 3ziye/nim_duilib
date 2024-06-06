#ifndef UI_CORE_WINDOW_H_
#define UI_CORE_WINDOW_H_

#pragma once

#include "duilib/Utils/Delegate.h"
#include "duilib/Core/Callback.h"
#include "duilib/Core/ControlFinder.h"
#include "duilib/Core/ColorManager.h"
#include "duilib/Core/DpiManager.h"
#include "duilib/Core/UiTypes.h"

#include <string>

namespace ui
{

class Box;
class Control;
class IRender;
class Shadow;
class ToolTip;
class WindowDropTarget;
class ControlDropTarget;

/**  窗口消息过滤接口，用于截获窗口过程的消息，优先于Window类进行消息处理
*/
class IUIMessageFilter
{
public:
    /**  消息处理函数，处理优先级高于Window类的消息处理函数
    * @param [in] uMsg 消息内容
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @param[out] bHandled 返回 false 则继续派发该消息，返回 true 表示不再派发该消息
    * @return 返回消息处理结果
    */
    virtual LRESULT FilterMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

#define UI_WNDSTYLE_FRAME       (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_DIALOG      (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_CLASSSTYLE_FRAME     (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_DIALOG    (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

/////////////////////////////////////////////////////////////////////////////////////
//

/** 窗口类
*  //外部调用需要初始化的基本流程:
*  1. 调用Window::CreateWnd创建窗口;
*  //以下内容，可用实现在OnCreate函数中:
*  2. Window::SetResourcePath;
*  3. WindowBuilder::Create, 得到Box* pRoot;
*  4. Window::AttachShadow(pRoot), 得到附加阴影的Box* pRoot, 以支持窗口阴影效果;
*  5. Window::AttachBox(pRoot);
*/
class UILIB_API Window : public virtual SupportWeakCallback
{
public:
    Window();
    Window(const Window& r) = delete;
    Window& operator=(const Window& r) = delete;
    virtual ~Window();

public:
    /** @name 窗口创建、销毁、事件监听等相关接口
    * @{
    */

    /** 创建窗口, 可使用 OnInitWindow 接口来实现窗口创建完成后的自定义需求
    * @param [in] pParentWindow 父窗口
    * @param [in] windowName 窗口名称
    * @param [in] dwStyle 窗口样式
    * @param [in] dwExStyle 窗口拓展样式, 可以设置层窗口（WS_EX_LAYERED）等属性
    * @param [in] rc 窗口大小
    */
    virtual bool CreateWnd(Window* pParentWindow,
                           const DString& windowName,
                           uint32_t dwStyle,
                           uint32_t dwExStyle,
                           const UiRect& rc = UiRect(0, 0, 0, 0));

    /** 是否含有有效的窗口句柄
    */
    bool IsWindow() const;

    /** 获取窗口所属的 Windows 句柄
    */
    HWND GetHWND() const;

    /** 获取父窗口
    */
    Window* GetParentWindow() const;

    /** 关闭窗口, 异步关闭，当函数返回后，IsClosing() 状态为true
    * @param [in] nRet 关闭消息, 含义如下：
                0 - 表示 "确认" 关闭本窗口
                1 - 表示点击窗口的 "关闭" 按钮关闭本窗口(默认值)
                2 - 表示 "取消" 关闭本窗口
    */
    virtual void CloseWnd(UINT nRet = 1);

    /** 是否将要关闭
    */
    bool IsClosingWnd() const { return m_bCloseing; }

    /** 监听窗口关闭事件
    * @param [in] callback 指定关闭后的回调函数，参数的wParam代表窗口关闭的触发情况：
                          0 - 表示 "确认" 关闭本窗口
                          1 - 表示点击窗口的 "关闭" 按钮关闭本窗口(默认值)
                          2 - 表示 "取消" 关闭本窗口
    */
    void AttachWindowClose(const EventCallback& callback);

protected:
    /** 创建窗口时被调用，由子类实现用以获取窗口唯一的类名称
    * @return 基类返回空串，在子类中需实现并返回窗口唯一的类名称
    */
    virtual DString GetWindowClassName() const;

    /** 获取控件窗口类(DateTime控件使用)
    * @return 返回控件窗口类，该基类返回空串
    */
    virtual DString GetSuperClassName() const;

    /** 获取窗口类的样式，该方法由实例化的子类实现，https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles
    * @return 返回窗口类的样式，该方法基类返回 CS_DBLCLKS
    */
    virtual UINT GetClassStyle() const;

    /** 获取资源的句柄
    * @return 默认返回当前进程exe的句柄
    */
    virtual HMODULE GetResModuleHandle() const;

    /** 获取窗口样式
    * @return 默认返回当前窗口的样式去掉WS_CAPTION属性
    *         如果子类重写该函数后，返回值为0，则不改变当前窗口的样式
    */
    virtual uint32_t GetWindowStyle() const;

    /** 当窗口创建完成以后调用此函数，供子类中做一些初始化的工作
    */
    virtual void OnInitWindow();

    /** 当窗口即将被关闭时调用此函数，供子类中做一些收尾工作
    */
    virtual void OnCloseWindow();

    /** 在窗口销毁时会被调用，这是该窗口的最后一个消息（该类默认实现是清理资源，并调用OnDeleteSelf函数销毁该窗口对象）
    */
    virtual void OnFinalMessage();

    /** 销毁自己（子类可用重载这个方法，避免自身被销毁）
    */
    virtual void OnDeleteSelf();

    /** @} */

public:
    /** @name 窗口显示、隐藏、大小、位置等状态相关接口
    * @{
    */

    /** 显示或隐藏窗口
     * @param [in] bShow 为 true 时显示窗口，为 false 时为隐藏窗口，默认为 true
     * @param [in] bTakeFocus 是否获得焦点（激活窗口），默认为 true
    */
    virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true);

    /** 显示模态对话框(父窗口在创建的时候指定)
    */
    void ShowModalFake();

    /** 是否是模态显示
    */
    bool IsFakeModal() const;

    /** 居中窗口，支持扩展屏幕
    */
    void CenterWindow();

    /** 将窗口设置为置顶窗口
    */
    void ToTopMost();

    /** 将窗口调整为顶层窗口
    */
    void BringToTop();

    /** 激活窗口
    *   如果当前窗口为最小化，则进行还原；
    *   如果当前窗口为隐藏，则显示，并设置为前端窗口
    *   如果当前窗口为显示，则设置为前端窗口
    */
    void ActiveWindow();

    /** 窗口最大化
    */
    bool Maximized();

    /** 还原窗口，退出最大化
    */
    bool Restore();

    /** 窗口最小化
    */
    bool Minimized();

    /** 使窗口进入全屏状态
    */
    bool EnterFullScreen();

    /** 使窗口退出全屏状态 (默认按ESC键时，退出全屏)
    */
    bool ExitFullScreen();

    /** 设置窗口为前端窗口
    */
    bool SetForeground();

    /** 使窗口成为焦点窗口
    */
    bool SetFocused();

    /** 关闭窗口, 同步关闭
    */
    void Close();

    /** 当前窗口是否为输入焦点窗口
    */
    bool IsWindowFocused() const;

    /** 当前窗口是否为前端窗口
    */
    bool IsWindowForeground() const;

    /** 窗口是否为最大化状态
    */
    bool IsWindowMaximized() const;

    /** 窗口是否为最小化状态
    */
    bool IsWindowMinimized() const;

    /** 窗口是否为全屏状态
    */
    bool IsWindowFullScreen() const;

    /** 更新窗口，执行重绘 
    */
    bool UpdateWindow() const;

    /** 窗口是否为最大化状态
    */
    bool IsZoomed() const;

    /** 窗口是否为最小花状态
    */
    bool IsIconic() const;

    /** 将窗口的Enable状态
    * @param [in] bEnable true表示设置为Enable状态，false表示设置为disable状态
    */
    bool EnableWindow(bool bEnable);

    /** 获取窗口的Enable状态
    */
    bool IsWindowEnabled() const;

    /** 设置窗口为焦点窗口
    */
    bool SetWindowFocus();

    /** 让窗口失去焦点
    */
    bool KillWindowFocus();

public:
    /** 获取窗口位置信息
     * @param [in] bContainShadow 是否包含阴影，true 为包含，默认为 false 不包含
     */
    UiRect GetWindowPos(bool bContainShadow /*= false*/) const;

    /** 设置窗口位置（对 ::SetWindowPos API 的一层封装）
    * @param [in] rc 窗口位置
    * @param [in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
    * @param [in] uFlags 对应 ::SetWindowPos API 的 uFlags 选项
    * @param [in] hWndInsertAfter 对应 SetWindowPos 的 hWndInsertAfter 选项
    * @param [in] bContainShadow rc区域是否包含阴影范围，默认为 false
    */
    bool SetWindowPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter = NULL, bool bContainShadow = false);

    /** 设置窗口位置（对 ::SetWindowPos API 的一层封装，内部无DPI缩放）
    * @param [in] hWndInsertAfter 对应 SetWindowPos 的 hWndInsertAfter 选项
    * @param [in] X 窗口的X坐标
    * @param [in] Y 窗口的Y坐标
    * @param [in] cx 窗口的宽度
    * @param [in] cy 窗口的高度
    * @param [in] uFlags 对应 ::SetWindowPos API 的 uFlags 选项 
    */
    bool SetWindowPos(HWND hWndInsertAfter, int32_t X, int32_t Y, int32_t cx, int32_t cy, UINT uFlags);

    /** 重置窗口大小
    * @param [in] cx 宽度
    * @param [in] cy 高度
    * @param [in] bContainShadow 为 false 表示 cx cy 不包含阴影
    * @param [in] bNeedDpiScale 为 false 表示不根据 DPI 调整
    */
    void Resize(int cx, int cy, bool bContainShadow /*= false*/, bool bNeedDpiScale);

    /** 设置窗口图标
    *  @param [in] nRes 窗口图标资源 ID
    */
    void SetIcon(UINT nRes);

    /** 设置窗口标题栏文本
    * @param [in] strText 窗口标题栏文本
    */
    void SetText(const DString& strText);

    /** 获取窗口标题栏文本
    */
    DString GetText() const;

    /** 根据语言列表中的文本 ID， 根据ID设置窗口标题栏文本
    * @param [in] strTextId 语言 ID，该 ID 必须在语言文件中存在
    */
    void SetTextId(const DString& strTextId);

    /** 获取窗口标题栏文本的文本ID
    */
    DString GetTextId() const;

    /** @} */

public:
    /** @name 窗口初始大小、标题栏等相关接口
    * @{
    */
    /** 获取窗口四边可拉伸范围的大小
    */
    const UiRect& GetSizeBox() const;

    /** 设置窗口四边可拉伸范围的大小
    * @param [in] rcSizeBox 要设置的大小
    * @param [in] bNeedDpiScale 是否支持DPI缩放
    */
    void SetSizeBox(const UiRect& rcSizeBox, bool bNeedDpiScale);

    /** 获取窗口标题栏区域（可拖动区域），对应 XML 中 caption 属性
    */
    const UiRect& GetCaptionRect() const;

    /** 设置窗口标题栏区域
    * @param [in] rcCaption 要设置的区域范围
    * @param [in] bNeedDpiScale 为 false 表示不根据 DPI 调整
    */
    void SetCaptionRect(const UiRect& rcCaption, bool bNeedDpiScale);

    /** 设置是否使用系统标题栏
    */
    void SetUseSystemCaption(bool bUseSystemCaption);

    /** 获取是否使用系统标题栏
    */
    bool IsUseSystemCaption() const;

    /** 获取窗口圆角大小，对应 XML 中 roundcorner 属性
    */
    const UiSize& GetRoundCorner() const;

    /** 设置窗口圆角大小
    * @param [in] cx 圆角宽
    * @param [in] cy 圆角高
    * @param [in] bNeedDpiScale 为 false 表示不根据 DPI 调整
    */
    void SetRoundCorner(int cx, int cy, bool bNeedDpiScale);

    /** 获取窗口最大化信息
    */
    const UiRect& GetMaximizeInfo() const;

    /** 设置窗口最大化信息
    * @param [in] rcMaximize 要设置的最大化数值
    * @param [in] bNeedDpiScale 是否进行DPI缩放
    */
    void SetMaximizeInfo(const UiRect& rcMaximize, bool bNeedDpiScale);

    /** 获取透明通道修补范围的的九宫格描述，对应 XML 中 alphafixcorner 属性
    */
    const UiRect& GetAlphaFixCorner() const;

    /** 设置透明通道修补范围的的九宫格描述
    * @param [in] rc 要设置的修补范围
    * @param [in] bNeedDpiScale 是否进行DPI缩放
    */
    void SetAlphaFixCorner(const UiRect& rc, bool bNeedDpiScale);

    /** 获取窗口最小范围，对应 XML 中 mininfo 属性
    * @param [in] bContainShadow 是否包含阴影范围，默认为 false
    */
    UiSize GetMinInfo(bool bContainShadow /*= false*/) const;

    /** 设置窗口最小范围
    * @param [in] cx 宽度
    * @param [in] cy 高度
    * @param [in] bContainShadow 为 false 表示 cx cy 不包含阴影
    * @param [in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
    */
    void SetMinInfo(int cx, int cy, bool bContainShadow /*= false*/, bool bNeedDpiScale);

    /** 获取窗口最大范围，对应 XML 中 maxinfo 属性
    * @param [in] bContainShadow 是否包含阴影范围，默认为 false
    */
    UiSize GetMaxInfo(bool bContainShadow /*= false*/) const;

    /** 设置窗口最大范围
    * @param [in] cx 宽度
    * @param [in] cy 高度
    * @param [in] bContainShadow 为 false 表示 cx cy 不包含阴影
    * @param [in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
    */
    void SetMaxInfo(int cx, int cy, bool bContainShadow /*= false*/, bool bNeedDpiScale);

    /** 设置窗口初始大小
    * @param [in] cx 宽度
    * @param [in] cy 高度
    * @param [in] bContainShadow 为 false 表示 cx cy 不包含阴影
    * @param [in] bNeedDpiScale 为 false 表示不根据 DPI 调整
    */
    void SetInitSize(int cx, int cy, bool bContainShadow /*= false*/, bool bNeedDpiScale);

    /** @}*/

public:
    /** @name 窗口布局相关接口
    * @{
    */
    /** 绑定窗口的顶层容器
    * @param [in] pRoot 容器指针
    */
    bool AttachBox(Box* pRoot);

    /** 获取窗口最外层的容器
    */
    Box* GetRoot() const;

    /** 获取窗口资源路径
    */
    const DString& GetResourcePath() const;

    /** 设置窗口资源路径
    * @param [in] strPath 要设置的路径
    */
    void SetResourcePath(const DString& strPath);

    /** 初始化控件，在容器中添加控件时会被调用（用于对控件名称做缓存）
    * @param [in] pControl 控件指针
    */
    bool InitControls(Control* pControl);

    /** 初始化布局
    */
    virtual void OnInitLayout();

    /** 回收控件
    * @param [in] pControl 控件指针
    */
    void ReapObjects(Control* pControl);

    /** 延迟销毁一个控件
    * @param [in] pControl 控件指针
    */
    void AddDelayedCleanup(Control* pControl);

    /** 添加一个通用样式
    * @param [in] strClassName 通用样式的名称
    * @param [in] strControlAttrList 通用样式的 XML 转义格式数据
    */
    void AddClass(const DString& strClassName, const DString& strControlAttrList);

    /** 获取指定通用样式的内容
    * @param [in] strClassName 通用样式名称
    * @return 返回指定名称的通用样式内容，XML 转义格式数据
    */
    DString GetClassAttributes(const DString& strClassName) const;

    /** 删除一个通用样式
    * @param [in] strClassName 要删除的通用样式名称
    */
    bool RemoveClass(const DString& strClassName);

    /** 删除所有通用样式
    */
    void RemoveAllClass();

    /** 添加一个颜色值提供窗口内使用
    * @param [in] strName 颜色名称（如 white）
    * @param [in] strValue 颜色具体数值（如 #FFFFFFFF）
    */
    void AddTextColor(const DString& strName, const DString& strValue);

    /** 添加一个颜色值提供窗口内使用
    * @param [in] strName 颜色名称（如 white）
    * @param [in] argb 颜色具体数值, 以ARGB格式表示
    */
    void AddTextColor(const DString& strName, UiColor argb);

    /** 根据名称获取一个颜色的具体数值
    * @param [in] strName 要获取的颜色名称
    * @return 返回 DWORD 格式的颜色描述值
    */
    UiColor GetTextColor(const DString& strName) const;

    /** 添加一个选项组
    * @param [in] strGroupName 组名称
    * @param [in] pControl 控件指针
    */
    bool AddOptionGroup(const DString& strGroupName, Control* pControl);

    /** 获取指定选项组中控件列表
    * @param [in] strGroupName 指定组名称
    * @return 返回该组下的所有控件列表
    */
    std::vector<Control*>* GetOptionGroup(const DString& strGroupName);

    /** 删除一个选项组
    * @param [in] strGroupName 组名称
    * @param [in] pControl 控件名称
    */
    void RemoveOptionGroup(const DString& strGroupName, Control* pControl);

    /** 删除所有选项组
    */
    void RemoveAllOptionGroups();

    /** @}*/

public:
    /** @name 窗口消息相关的接口
    * @{
    */
    /** 添加一个消息过滤器，此时消息已经派发，该接口的消息处理优先级高于Window类的消息处理函数
    * @param [in] pFilter 一个继承了 IUIMessageFilter 的对象实例，需要实现接口中的方法
    */
    bool AddMessageFilter(IUIMessageFilter* pFilter);

    /** 移除一个消息过滤器
    * @param [in] pFilter 一个继承了 IUIMessageFilter 的对象实例
    */
    bool RemoveMessageFilter(IUIMessageFilter* pFilter);

    /** 发送消息，对 Windows SendMessage 的一层封装
    * @param [in] uMsg 消息类型
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回窗口对消息的处理结果
    */
    LRESULT SendMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

    /** 投递一个消息到消息队列
    * @param [in] uMsg 消息类型
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回窗口对消息的处理结果
    */
    LRESULT PostMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

    /** 主动发起一个消息, 发送给该窗口的事件回调管理器（m_OnEvent）中注册的消息处理函数
    * @param [in] eventType 转化后的消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    */
    bool SendNotify(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0);

    /** 调用系统默认的窗口处理函数，对 CallWindowProc API 的一层封装
    * @param [in] uMsg 消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回消息处理结果
    */
    LRESULT CallDefaultWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

    /** 窗口消息的处理函数, 从系统接收到消息后，进入的第一个处理函数
    * @param [in] uMsg 消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回消息的处理结果
    */
    LRESULT WindowMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    /** 窗口消息的派发函数
    * @param [in] uMsg 消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @param[out] bHandled 消息是否已经处理，
                返回 true  表明已经成功处理消息，不需要再传递给窗口过程；
                返回 false 表示将消息继续传递给窗口过程处理
    * @return 返回消息的处理结果
    */
    virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /** 窗口消息的派发函数，优先调用内部处理函数，如果内部无处理逻辑，则调用Windows默认处理函数处理此消息
    * @param [in] uMsg 消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @param[out] bHandled 消息是否已经处理，返回 true 表明已经成功处理消息，否则将消息继续传递给窗口过程
    * @return 返回消息的处理结果
    */
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    /** @}*/

    /** 进入全屏状态
    */
    virtual void OnWindowEnterFullScreen() {}

    /** 退出全屏状态
    */
    virtual void OnWindowExitFullScreen() {}

    /** 进入最大化状态
    */
    virtual void OnWindowMaximized() {}

    /** 从最大化还原
    */
    virtual void OnWindowRestored() {}

    /** 进入最小化状态
    */
    virtual void OnWindowMinimized() {}

    /** 切换系统标题栏与自绘标题栏
    */
    virtual void OnUseSystemCaptionBarChanged();

    /** 窗口的DPI发生了变化
    * @param [in] nOldDPI 旧的DPI值
    * @param [in] nNewDPI 新的DPI值
    */
    virtual void OnWindowDpiChanged(uint32_t nOldDPI, uint32_t nNewDPI);

private:
    /** @name 私有窗口消息处理相关
    * @{
    */
    //部分消息处理函数，以实现基本功能    
    LRESULT OnNcActivateMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnNcCalcSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnNcHitTestMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnNcLButtonDbClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    LRESULT OnCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnGetMinMaxInfoMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnWindowPosChangingMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnDpiChangedMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnMoveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnEraseBkGndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnPaintMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    LRESULT OnMouseHoverMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnMouseLeaveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnMouseMoveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnMouseWheelMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnLButtonDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnRButtonDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnLButtonDoubleClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnRButtonDoubleClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnLButtonUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnRButtonUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnContextMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    LRESULT OnIMEStartCompositionMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnIMEEndCompositionMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnSetFocusMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnKillFocusMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnCharMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnKeyDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnKeyUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnSysKeyDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnSysKeyUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnSetCusorMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnNotifyMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnCommandMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnSysCommandMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnHotKeyMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnCtlColorMsgs(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    LRESULT OnTouchMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT OnPointerMsgs(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

private:
    //鼠标等按下消息处理函数
    void OnButtonDown(EventType eventType, WPARAM wParam, LPARAM lParam, const UiPoint& pt);

    //鼠标等弹起消息处理函数
    void OnButtonUp(EventType eventType, WPARAM wParam, LPARAM lParam, const UiPoint& pt);

    //鼠标移动消息处理函数
    void OnMouseMove(WPARAM wParam, LPARAM lParam, const UiPoint& pt);

    /** 鼠标滚轮消息处理函数
    *  @param [in] wParam 滚轮旋转的距离，正值表示滚轮向前旋转，远离用户;负值表示滚轮向后向用户旋转。
    *  @param [in] lParam 含有特殊处理， 0表示严格按照wParam指定的距离滚动, 非0表示按照常规逻辑处理滚轮旋转的距离
    *  @param [in] pt 鼠标所在的坐标值，为窗口客户区坐标
    */
    void OnMouseWheel(WPARAM wParam, LPARAM lParam, const UiPoint& pt);

    //清除鼠标键盘操作状态
    void ClearStatus();

    /** 判断是否需要发送鼠标进入或离开消息
    * @param [in] pt 鼠标当前位置
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回 true 需要发送鼠标进入或离开消息，返回 false 为不需要
    */
    bool HandleMouseEnterLeave(const UiPoint& pt, WPARAM wParam, LPARAM lParam);

    /** @}*/

public:
    /** @name 窗口焦点相关接口
    * @{
    */
    /** 获取当前持有焦点的控件
    */
    Control* GetFocus() const;

    /** 获取当前鼠标事件的控件
    */
    Control* GetEventClick() const;

    /** 设置焦点到指定控件上(设置窗口为焦点窗口，并设置该控件为焦点控件)
    * @param [in] pControl 控件指针
    */
    void SetFocus(Control* pControl);

    /** 让控件失去焦点（不影响窗口焦点）
    */
    void KillFocus();

    /** 设置当要捕获的鼠标窗口句柄为当前绘制窗口
    * @param [in]
    */
    void SetCapture();

    /** 当窗口不需要鼠标输入时释放资源
    */
    void ReleaseCapture();

    /** 判断当前是否捕获鼠标输入
    */
    bool IsCaptured() const;

    /** 获取当前鼠标在哪个控件上
    */
    Control* GetHoverControl() const;

    /** 获取鼠标最后的坐标
    */
    const UiPoint& GetLastMousePos() const;

    /** 切换控件焦点到下一个（或上一个）控件
    * @param [in] bForward true 为上一个控件，否则为 false，默认为 true
    */
    bool SetNextTabControl(bool bForward = true);

    /** @}*/

public:
    /** @name 窗口阴影、层窗口透明度相关接口
    * @{
    */
    /** 附加窗口阴影
    */
    virtual Box* AttachShadow(Box* pRoot);

    /** 设置窗口是否附加阴影效果
    * @param [in] bShadowAttached 为 true 时附加，false 时不附加
    */
    void SetShadowAttached(bool bShadowAttached);

    /** 获取是否附加阴影效果
    */
    bool IsShadowAttached() const;

    /** 当前阴影效果值，是否为默认值
    */
    bool IsUseDefaultShadowAttached() const;

    /** 设置当前阴影效果值，是否为默认值
    */
    void SetUseDefaultShadowAttached(bool isDefault);

    /** 获取阴影图片
    */
    DString GetShadowImage() const;

    /** 设置窗口阴影图片
    * @param [in] strImage 图片位置
    */
    void SetShadowImage(const DString& strImage);

    /** 获取阴影的九宫格描述信息
    */
    UiPadding GetShadowCorner() const;

    /** 指定阴影素材的九宫格描述
    * @param [in] padding 九宫格描述信息
    * @param [in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
    */
    void SetShadowCorner(const UiPadding& padding, bool bNeedDpiScale);

    /** 设置窗口透明度
    * @param [in] nAlpha 透明度数值[0, 255]
    */
    void SetWindowAlpha(int nAlpha);

    /** 获取窗口透明度
    * @param [in] nAlpha 透明度数值[0, 255]
    */
    uint8_t GetWindowAlpha() const;

    /** 设置是否为层窗口
    */
    void SetLayeredWindow(bool bIsLayeredWindow);

    /** 是否为层窗口
    */
    bool IsLayeredWindow() const;

    /** @}*/

public:
    /** @name 窗口绘制相关接口
    * @{
    */
    /** 设置控件是否已经布局
    * @param [in] bArrange true 为已经排列，否则为 false
    */
    void SetArrange(bool bArrange);

    /** 发出重绘消息
    * @param [in] rcItem 重绘范围
    */
    void Invalidate(const UiRect& rcItem);

    /** 获取绘制区域 DC
    */
    HDC GetPaintDC() const;

    /** 获取绘制对象
    */
    IRender* GetRender() const;

    /** 绘制函数体
    */
    void Paint();

    /** 判断当前是否渲染透明图层
    */
    bool IsRenderTransparent() const;

    /** 设置渲染透明图层
    * @param [in] bCanvasTransparent 设置 true 为渲染透明图层，否则为 false
    */
    bool SetRenderTransparent(bool bCanvasTransparent);

    /** 清理图片缓存
    */
    void ClearImageCache();

    /** @} */

public:

    /**@name 控件查找相关接口
    * @{
    */
    /** 根据坐标查找指定控件，采用默认属性：UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST
    * @param [in] pt 指定坐标
    */
    Control* FindControl(const UiPoint& pt) const;

    /**
    *  根据坐标查找可以响应WM_CONTEXTMENU的控件
    * @param [in] pt 指定坐标
    */
    Control* FindContextMenuControl(const UiPoint* pt) const;

    /** 查找可以支持拖放的Box容器
    * @param [in] pt 指定坐标
    * @param [in] nDropInId 拖放的ID值（每个控件可以设置一个ID，来接收拖放）
    */
    Box* FindDroppableBox(const UiPoint& pt, uint8_t nDropInId) const;

    /** 根据控件名称查找控件
    * @param [in] strName 控件名称
    */
    Control* FindControl(const DString& strName) const;

    /** 根据坐标查找子控件
    * @param [in] pParent 要搜索的控件
    * @param [in] pt 要查找的坐标
    */
    Control* FindSubControlByPoint(Control* pParent, const UiPoint& pt) const;

    /** 根据名字查找子控件
    * @param [in] pParent 要搜索的控件
    * @param [in] strName 要查找的名称
    */
    Control* FindSubControlByName(Control* pParent, const DString& strName) const;

    /** @} */

public:
    /** 获取当前窗口的客户区矩形
    */
    void GetClientRect(UiRect& rcClient) const;

    /** 获取当前窗口的窗口区矩形
    */
    void GetWindowRect(UiRect& rcWindow) const;

    /** 将屏幕坐标转换为当前窗口的客户区坐标
    */
    void ScreenToClient(UiPoint& pt) const;

    /** 将当前窗口的客户区坐标转换为屏幕坐标
    */
    void ClientToScreen(UiPoint& pt) const;

    /** 获取当前鼠标所在坐标
    */
    void GetCursorPos(UiPoint& pt) const;

    /* 将rc的左上角坐标和右下角坐标点从相对于当前窗口的坐标空间转换为相对于桌面窗口的坐标空间
    */
    void MapWindowRect(UiRect& rc) const;

    /** 获取一个点对应的窗口接口
    */
    Window* WindowFromPoint(const UiPoint& pt);

    /** 获取当前窗口所在显示器的工作区矩形，以虚拟屏幕坐标表示。
        请注意，如果显示器不是主显示器，则一些矩形的坐标可能是负值。
    */
    bool GetMonitorWorkRect(UiRect& rcWork) const;

    /** 获取指定点所在显示器的工作区矩形，以虚拟屏幕坐标表示。
        请注意，如果显示器不是主显示器，则一些矩形的坐标可能是负值。
    */
    bool GetMonitorWorkRect(const UiPoint& pt, UiRect& rcWork) const;

    /** 注册一个拖放接口
    */
    bool RegisterDragDrop(ControlDropTarget* pDropTarget);

    /** 注销一个拖放接口
    */
    bool UnregisterDragDrop(ControlDropTarget* pDropTarget);

    /** 更新ToolTip信息（此时ToolTip的信息已经发生变化）
    */
    void UpdateToolTip();

    /** 获取该窗口对应的DPI管理器
    */
    const DpiManager& Dpi() const;

    /** 主动调整窗口的DPI
    *   注意事项：该函数可用改变窗口的显示DPI，从而改变窗口、控件的界面百分比；
    *            但如果进程的DPI感知类型是kPerMonitorDpiAware/kPerMonitorDpiAware_V2时，
    *            当系统DPI改变或者跨越不同DPI的多屏幕时，窗口的大小和DPI会被系统调整，进而会有冲突，导致窗口控件大小显示异常。
    * @param [in] nNewDPI 新的DPI值，比如96代表界面DPI缩放比为100%
    */
    bool ChangeDpi(uint32_t nNewDPI);

public:
    /** 设置系统激活窗口热键，注册后按此热键，系统可以自动激活本窗口
    * @param [in] wVirtualKeyCode 虚拟键盘码，比如：kVK_DOWN等，可参考：https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes
    *             如果wVirtualKeyCode为0，表示取消管理窗口激活热键
    * @param [in] wModifiers 热键组合键标志位，参见HotKeyModifiers枚举类型的值
    * @return 返回值说明:
       -1: 函数不成功;热键无效。
        0: 函数不成功;窗口无效。
        1: 函数成功，并且没有其他窗口具有相同的热键。
        2: 函数成功，但另一个窗口已具有相同的热键。
    */
    int32_t SetWindowHotKey(uint8_t wVirtualKeyCode, uint8_t wModifiers);

    /** 获取系统激活窗口热键
    * @param [out] wVirtualKeyCode 虚拟键盘码，比如：kVK_DOWN等
    * @param [out] wModifiers 热键组合键标志位，参见HotKeyModifiers枚举类型的值
    * @return 如果返回false表示没有注册窗口激活热键，否则表示有注册窗口激活热键
    */
    bool GetWindowHotKey(uint8_t& wVirtualKeyCode, uint8_t& wModifiers) const;

    /** 注册系统全局热键，注册成功后，按此热键后，该窗口会收到WM_HOTKEY消息
    * @param [in] wVirtualKeyCode 虚拟键盘码，比如：kVK_DOWN等
    * @param [in] wModifiers 热键组合键标志位，参见HotKeyModifiers枚举类型的值
    * @param [in] id 命令ID，应用程序必须在0x0000到0xBFFF的范围内指定 ID 值。 
                  为了避免与其他共享 DLL 定义的热键标识符冲突，DLL 应使用 GlobalAddAtom 函数获取热键标识符。
    */
    bool RegisterHotKey(uint8_t wVirtualKeyCode, uint8_t wModifiers, int32_t id);

    /** 注销系统全局热键
    * @param [in] id 命令ID，即注册时使用的命令ID 
    */
    bool UnregisterHotKey(int32_t id);

private:
    /**@name 动画效果相关接口
    * @{
    */
    /** 设置绘制偏移
    * @param [in] renderOffset 偏移值
    */
    void SetRenderOffset(UiPoint renderOffset);

    /** 设置绘制偏移 x 坐标
    * @param [in] renderOffsetX 坐标值
    */
    void SetRenderOffsetX(int renderOffsetX);

    /** 设置绘制偏移 y 坐标
    * @param [in] renderOffsetY 坐标值
    */
    void SetRenderOffsetY(int renderOffsetY);

    /** @} */

private:
    /** 将屏幕坐标转换为指定窗口的客户区坐标
    */
    void ScreenToClient(HWND hWnd, UiPoint& pt) const;

    /** 将指定窗口的客户区坐标转换为屏幕坐标
    */
    void ClientToScreen(HWND hWnd, UiPoint& pt) const;

    /** 获取指定窗口的客户区矩形
    */
    void GetClientRect(HWND hWnd, UiRect& rcClient) const;

    /** 获取指定窗口的窗口区矩形
    */
    void GetWindowRect(HWND hWnd, UiRect& rcWindow) const;

    /** 获取指定窗口所在显示器的工作区矩形，以虚拟屏幕坐标表示。
        请注意，如果显示器不是主显示器，则一些矩形的坐标可能是负值。
    */
    bool GetMonitorWorkRect(HWND hWnd, UiRect& rcWork) const;

    /** 获取指定窗口所在显示器的显示器矩形和工作区矩形
    */
    bool GetMonitorRect(HWND hWnd, UiRect& rcMonitor, UiRect& rcWork) const;

    /** 替换窗口函数
    */
    WNDPROC SubclassWindow(HWND hWnd, WNDPROC pfnWndProc) const;

    /** 获取当前窗口Owner窗口句柄
    */
    HWND GetWindowOwner() const;

    /* 将rc的左上角坐标和右下角坐标点从相对于一个窗口的坐标空间转换为相对于另一个窗口的坐标空间
    */
    void MapWindowRect(HWND hwndFrom, HWND hwndTo, UiRect& rc) const;

    /** 根据root自动调整窗口大小
    */
    void AutoResizeWindow(bool bRepaint);

    /** 对控件进行布局调整
    */
    void ArrangeRoot();

private:
    /** 检查并确保当前窗口为焦点窗口
    */
    void CheckSetFocusWindow();

    /** 注册窗口类
    */
    bool RegisterWindowClass();

    /** 注册控件窗口类（与窗口的过程函数不同）
    */
    bool RegisterSuperClass();

    /** 窗口过程函数(当GetSuperClassName()函数返回不为空串时使用)
    * @param [in] hWnd 窗口句柄
    * @param [in] uMsg 消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回消息处理结果
    */
    static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /** 窗口过程函数(当GetSuperClassName()函数返回空串时使用) 
    * @param [in] hWnd 窗口句柄
    * @param [in] uMsg 消息体
    * @param [in] wParam 消息附加参数
    * @param [in] lParam 消息附加参数
    * @return 返回消息处理结果
    */
    static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /** 获取窗口对应的Window对象接口
    * @param [in] hWnd 窗口句柄
    * @return 返回窗口句柄对应的Window对象接口
    */
    static Window* GetWindowObject(HWND hWnd);

private:
    /** 初始化窗口数据
    */
    void InitWindow();

    /** 清理窗口资源
    * @param [in] bSendClose 是否发送关闭事件
    */
    void ClearWindow(bool bSendClose);

    /** 窗口的DPI发生变化，更新控件大小和布局
    * @param [in] nOldDpiScale 旧的DPI缩放百分比
    * @param [in] nNewDpiScale 新的DPI缩放百分比，与Dpi().GetScale()的值一致
    */
    void OnDpiScaleChanged(uint32_t nOldDpiScale, uint32_t nNewDpiScale);

private:
    //窗口句柄
    HWND m_hWnd;

    /** 父窗口
    */
    Window* m_pParentWindow;

    /** 父窗口的WeakFlag
    */
    std::weak_ptr<WeakFlag> m_parentFlag;

    //原来的窗口过程函数
    WNDPROC m_OldWndProc;

    //事件回调管理器
    EventMap m_OnEvent;

private:
    //窗口最小信息
    UiSize m_szMinWindow;

    //窗口最大信息
    UiSize m_szMaxWindow;

    //窗口最大化信息
    UiRect m_rcMaximizeInfo;

    //窗口四边可拉伸范围信息
    UiRect m_rcSizeBox;

    //透明通道修补范围的的九宫格描述
    UiRect m_rcAlphaFix;

    //窗口圆角信息
    UiSize m_szRoundCorner;

    //标题栏区域信息
    UiRect m_rcCaption;

    //布局是否变化，如果变化(true)则需要重新计算布局
    bool m_bIsArranged;

    //布局是否需要初始化
    bool m_bFirstLayout;

    //是否使用系统的标题栏
    bool m_bUseSystemCaption;

private:
    //绘制DC
    HDC m_hDcPaint;

    //是否为层窗口
    bool m_bIsLayeredWindow;

    //窗口透明度(仅当使用层窗口时有效)
    uint8_t m_nWindowAlpha;

    //绘制时的偏移量（动画用）
    UiPoint m_renderOffset;

    //绘制引擎
    std::unique_ptr<IRender> m_render;

private:

    //绘制引擎
    std::unique_ptr<ToolTip> m_toolTip;

    //焦点控件
    Control* m_pFocus;

    //鼠标在悬停控件
    Control* m_pEventHover;

    /** 点击的控件：
        在 WM_LBUTTONDOWN/WM_RBUTTONDOWN/WM_LBUTTONDBLCLK 赋值
        在 WM_LBUTTONUP中/WM_KILLFOCUS 清空
    */
    Control* m_pEventClick;

    /** 键盘按下的控件
    *   在 WM_KEYDOWN / WM_SYSKEYDOWN 赋值
    *   在 WM_KEYUP / WM_SYSKEYUP清空
    */
    Control* m_pEventKey;

    //鼠标所在位置
    UiPoint m_ptLastMousePos;

    //鼠标事件的捕获状态
    bool m_bMouseCapture;

private:
    //每个窗口的资源路径(相对于资源根目录的路径)
    DString m_strResourcePath;

    //窗口配置中class名称与属性映射关系
    std::map<DString, DString> m_defaultAttrHash;

    //窗口颜色字符串与颜色值（ARGB）的映射关系
    ColorMap m_colorMap;

    //该窗口下每个Option group下的控件（即单选控件是分组的）
    std::map<DString, std::vector<Control*>> m_mOptionGroup;

    //该窗口消息过滤器列表
    std::vector<IUIMessageFilter*> m_aMessageFilters;

    //延迟释放的控件接口
    std::vector<Control*> m_aDelayedCleanup;

    //异步关闭窗口回调函数资源有效性标志
    WeakCallbackFlag m_closeFlag;

    //窗口阴影
    std::unique_ptr<Shadow> m_shadow;

    //当前窗口是否显示为模态对话框
    bool m_bFakeModal;

    //窗口已经延迟关闭：add by djj 20200428 调用Close时会延迟Post WM_CLOSE, 这个期间需要有一个标识此种'待关闭状态'
    bool m_bCloseing;

    //窗口自己的DPI管理器
    std::unique_ptr<DpiManager> m_dpi;

private:
    /**@name 全屏相关状态
    * @{

    /** 窗口是否为全屏状态
    */
    bool m_bFullScreen;

    /** 全屏前的窗口风格
    */
    DWORD m_dwLastStyle;

    /** 全屏前的窗口位置/窗口大小等信息
    */
    WINDOWPLACEMENT m_rcLastWindowPlacement;

    /** @} */

private:
    /** 控件查找辅助类
    */
    ControlFinder m_controlFinder;

    /** 窗口关联的容器，根节点
    */
    Box* m_pRoot;

    /** 窗口的拖放操作管理接口
    */
    WindowDropTarget* m_pWindowDropTarget;

    /** 系统全局热键的ID
    */
    std::vector<int32_t> m_hotKeyIds;

private:
    /** 窗口标题栏文本的文本ID
    */
    DString m_text;

    /** 窗口标题栏文本的文本ID
    */
    DString m_textId;
};

} // namespace ui

#endif // UI_CORE_WINDOW_H_
