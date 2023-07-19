#ifndef UI_BOX_HLAYOUT_H_
#define UI_BOX_HLAYOUT_H_

#pragma once

#include "duilib/Box/Layout.h"

namespace ui 
{

/** ˮƽ����: �����ӿؼ����������top����ͬ��ˮƽ������������
*/
class UILIB_API HLayout : public Layout
{
public:
	HLayout();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;
};

} // namespace ui

#endif // UI_BOX_HLAYOUT_H_
