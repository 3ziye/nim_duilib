#ifndef EXAMPLES_ITEM_H_
#define EXAMPLES_ITEM_H_

// duilib
#include "duilib/duilib.h"

// 从 ui::ListBoxItem 中继承所有可用功能
class Item : public ui::ListBoxItem
{
public:
    explicit Item(ui::Window* pWindow);
    ~Item();

    // 提供外部调用来初始化 item 数据
    void InitSubControls(const std::wstring& img, const std::wstring& title);
    
private:
    bool OnRemove(const ui::EventArgs& args);

private:
    ui::ListBox*    list_box_;

    ui::Control*    control_img_;
    ui::Label*        label_title_;
    ui::Progress*    progress_;
    ui::Button*        btn_del_;
};

#endif //#define EXAMPLES_ITEM_H_
