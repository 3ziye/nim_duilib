#ifndef EXAMPLES_TASKBAR_MANAGER_H_
#define EXAMPLES_TASKBAR_MANAGER_H_

// duilib
#include "duilib/duilib.h"

interface ITaskbarList4;

/** @class TaskbarTabItem
  * @brief 在任务栏附加到某窗口中显示的一个任务栏缩略图Tab(只在Win7及以上系统有)
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2019/3/20
  */
class TaskbarManager;
class TaskbarTabItem final : public ui::Window
{
    typedef ui::Window BaseClass;
public:
    /**
    * @param[in] bind_control 与TaskbarTabItem绑定在一起的提供缩略图和预览图的控件的指针
    * 构造函数
    */
    explicit TaskbarTabItem(ui::Control* bind_control);

    /**
    * 获取与TaskbarTabItem绑定在一起的控件指针
    * @return ui::Control* 控件指针
    */
    ui::Control* GetBindControl();

    /**
    * 获取缩略图Tab的唯一id
    * @return std::string& id
    */
    std::string& GetId();

    /**
    * 初始化函数，TaskbarTabItem被new后立即调用
    * @param[in] taskbar_title 在任务栏缩略图上显示的标题
    * @param[in] id 缩略图Tab的唯一id
    * @return void    无返回值
    */
    void Init(const DString &taskbar_title, const std::string &id);

    /**
    * 反初始化函数
    * @return void    无返回值
    */
    void UnInit();

    /**
    * 设置本Tab缩略图的标题
    * @param[in] title 标题
    * @return void    无返回值
    */
    void SetTaskbarTitle(const DString &title);

    /**
    * 设置本Tab所属的窗体对应的TaskbarManager
    * @param[in] taskbar_manager TaskbarManager指针
    * @return void    无返回值
    */
    void SetTaskbarManager(TaskbarManager *taskbar_manager);

    /**
    * 获取本Tab所属的窗体对应的TaskbarManager
    * @return TaskbarManager* TaskbarManager指针
    */
    TaskbarManager* GetTaskbarManager();

    /**
    * 把TaskbarTabItem在任务栏的缩略图设为无效状态，用于重绘缩略图
    * @return bool true 成功，false 失败
    */
    bool InvalidateTab();

private:
    /**
    * 拦截并处理底层窗体消息
    * @param[in] uMsg 消息类型
    * @param[in] wParam 附加参数
    * @param[in] lParam 附加参数
    * @param[in out] bHandled 是否处理了消息，如果处理了不继续传递消息
    * @return LRESULT 处理结果
    */
    virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

    /**
    * 响应WM_DWMSENDICONICTHUMBNAIL消息
    * @param[in] width 需要的位图宽度
    * @param[in] height 需要的位图高度
    * @return void    无返回值
    */
    void OnSendThumbnail(int width, int height);

    /**
    * 响应WM_DWMSENDICONICLIVEPREVIEWBITMAP消息
    * @return void    无返回值
    */
    void OnSendPreview();

private:
    bool            is_win7_or_greater_;
    ui::Control        *bind_control_;
    std::string        id_;
    TaskbarManager    *taskbar_manager_;
};

/** @class TaskbarManager
  * @brief 管理某个会话窗口在任务栏显示的预览效果(只在Win7及以上系统有),其中包含多个TaskbarItem
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2019/3/20
  */
class SessionForm;
class TaskbarManager final : public virtual ui::SupportWeakCallback
{
public:
    /** @class ITaskbarDelegate
      * @brief TaskbarManager的事件委托类，只有继承该类的类可以使用TaskbarManager功能
      * @date 2019/3/20
      */
    class ITaskbarDelegate
    {
    public:
        /**
        * 获取窗体句柄
        * @return HWND    窗体句柄
        */
        virtual HWND GetHandle() const = 0;

        /**
        * 获取渲染接口
        * @return 渲染接口
        */
        virtual ui::IRender* GetTaskbarRender() const = 0;

        /**
        * 关闭一个任务栏项
        * @param[in] id 任务栏项id
        * @return void    无返回值
        */
        virtual void CloseTaskbarItem(const std::string &id) = 0;

        /**
        * 激活并切换到一个任务栏项
        * @param[in] id 任务栏项id
        * @return void 无返回值
        */
        virtual void SetActiveTaskbarItem(const std::string &id) = 0;
    };

    /**
    * 构造函数
    */
    TaskbarManager();

    /**
    * 初始化ITaskbarList4接口
    * @param[in] taskbar_delegate 作为任务栏缩略图主窗口
    * @return void    无返回值
    */
    void Init(ITaskbarDelegate *taskbar_delegate);

    /**
    * 把一个TaskbarTabItem注册为主窗口的一个Tab页
    * @param[in] tab_item TaskbarItem对象
    * @return bool true 成功，false 失败
    */
    bool RegisterTab(TaskbarTabItem &tab_item);

    /**
    * 把一个TaskbarTabItem从主窗口中反注册为
    * @param[in] tab_item TaskbarItem对象
    * @return bool true 成功，false 失败
    */
    bool UnregisterTab(TaskbarTabItem &tab_item);

    /**
    * 把一个TaskbarTabItem在主窗口的缩略图中的显示顺序设置到另一个TaskbarTabItem前面
    * @param[in] tab_item 被设置顺序的TaskbarItem对象
    * @param[in] tab_item_insert_before 被插入的TaskbarItem对象
    * @return bool true 成功，false 失败
    */
    bool SetTabOrder(const TaskbarTabItem &tab_item, const TaskbarTabItem &tab_item_insert_before);

    /**
    * 把一个TaskbarTabItem在任务栏设置为激活状态
    * @param[in] tab_item TaskbarItem对象
    * @return bool true 成功，false 失败
    */
    bool SetTabActive(const TaskbarTabItem &tab_item);

    /**
    * 生成某个控件对应的位图（返回位图包含窗口，且尺寸为窗口大小）
    * @param[in] control 控件指针
    * @return HBITMAP    生成的位图
    */
    ui::IBitmap* GenerateBindControlBitmapWithForm(ui::Control *control);

    /**
    * 生成某个控件对应的位图
    * @param[in] control 控件指针
    * @param[in] dest_width 目标宽度
    * @param[in] dest_height 目标高度
    * @return HBITMAP    生成的位图
    */
    ui::IBitmap* GenerateBindControlBitmap(ui::Control *control, const int dest_width, const int dest_height);

    /**
    * 缩放一个内存位图
    * @param[in] dest_width 目标宽度
    * @param[in] dest_height 目标高度
    * @param[in] pSrcRender 源Render接口
    * @param[in] src_x 源横坐标
    * @param[in] src_y 源纵坐标
    * @param[in] src_width 源宽度
    * @param[in] src_height 源高度
    * @return HBITMAP    生成的位图
    */
    ui::IBitmap* ResizeBitmap(int dest_width, int dest_height, ui::IRender* pSrcRender, int src_x, int src_y, int src_width, int src_height);

    /**
    * 处理任务栏Tab的缩略图的WM_CLOSE消息
    * @param[in] tab_item 触发消息的Tab
    * @return void    无返回值
    */
    void OnTabItemClose(TaskbarTabItem &tab_item);

    /**
    * 处理任务栏Tab的缩略图的WM_ACTIVATE消息
    * @param[in] tab_item 触发消息的Tab
    * @return void    无返回值
    */
    void OnTabItemClicked(TaskbarTabItem &tab_item);
private:
    ITaskbarDelegate    *taskbar_delegate_;
    ITaskbarList4        *taskbar_list_;
};

#endif //EXAMPLES_TASKBAR_MANAGER_H_
