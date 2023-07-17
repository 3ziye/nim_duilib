#ifndef UI_CORE_VBOX_H_
#define UI_CORE_VBOX_H_

#pragma once

#include "duilib/Core/Box.h"

namespace ui
{

class UILIB_API VLayout : public Layout
{
public:
	VLayout();
	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;
};

class UILIB_API VBox : public Box
{
public:
	VBox();

	virtual std::wstring GetType() const override;
};

}
#endif // UI_CORE_VBOX_H_
