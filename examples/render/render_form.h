#pragma once

// base header
#include "base/base.h"

// duilib
#include "duilib/duilib.h"

class RenderForm : public ui::WindowImplBase
{
public:
    RenderForm();
    ~RenderForm();

    /**
     * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
     * GetSkinFolder        接口设置你要绘制的窗口皮肤资源路径
     * GetSkinFile            接口设置你要绘制的窗口的 xml 描述文件
     * GetWindowClassName    接口设置窗口唯一的类名称
     */
    virtual std::wstring GetSkinFolder() override;
    virtual std::wstring GetSkinFile() override;
    virtual std::wstring GetWindowClassName() const override;

    /** 当窗口创建完成以后调用此函数，供子类中做一些初始化的工作
    */
    virtual void OnInitWindow() override;

    /** 当窗口即将被关闭时调用此函数，供子类中做一些收尾工作
    */
    virtual void OnCloseWindow() override;

    /**@brief 当要创建的控件不是标准的控件名称时会调用该函数
     * @param[in] strClass 控件名称
     * @return 返回一个自定义控件指针，一般情况下根据 strClass 参数创建自定义的控件
     */
    virtual ui::Control* CreateControl(const std::wstring& strClass) override;

    /** 窗口类名
    */
    static const std::wstring kClassName;

private:
    //测试PropertyGrid控件
    void TestPropertyGrid();

    //获取PropertyGrid控件的结果
    void CheckPropertyGridResult();
};

