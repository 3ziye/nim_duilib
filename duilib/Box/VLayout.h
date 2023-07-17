#ifndef UI_BOX_VLAYOUT_H_
#define UI_BOX_VLAYOUT_H_

#pragma once

#include "duilib/Box/Layout.h"

namespace ui 
{

/** ��ֱ����
*/
class UILIB_API VLayout : public Layout
{
public:
	VLayout();
	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;
};

} // namespace ui

#endif // UI_BOX_VLAYOUT_H_
