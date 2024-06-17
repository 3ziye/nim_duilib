#ifndef EXAMPLES_CUSTOM_LAYOUT_H_
#define EXAMPLES_CUSTOM_LAYOUT_H_

// duilib
#include "duilib/duilib.h"

/** @class CustomLayout
  * @brief 用于会话窗口中容纳SessionBox的自定义TabBox，为了在任务栏显示缩略图和预览图，重写了对子控件的布局逻辑
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2019/3/20
  */
class CustomLayout : public ui::Layout
{
public:
    virtual ui::UiSize64 ArrangeChild(const std::vector<ui::Control*>& m_items, ui::UiRect rc) override;
    ui::UiSize SetCustomPos(ui::Control* pControl, const ui::UiRect& containerRect);
};

#endif //EXAMPLES_CUSTOM_LAYOUT_H_
