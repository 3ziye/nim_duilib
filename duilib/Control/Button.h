#ifndef UI_CONTROL_BUTTON_H_
#define UI_CONTROL_BUTTON_H_

#pragma once

#include "duilib/Control/Label.h"

namespace ui {

template<typename InheritType = Control>
class UILIB_API ButtonTemplate : public LabelTemplate<InheritType>
{
public:
    ButtonTemplate();

    /// ��д���෽�����ṩ���Ի����ܣ���ο���������
    virtual std::wstring GetType() const override;
    virtual void Activate() override;
    virtual void HandleEvent(const EventArgs& event) override;
    virtual UINT GetControlFlags() const override;

    /** �ÿؼ��Ƿ���Է����ڱ������ϣ������ڴ���NC��Ϣ��Ӧ��
     * @return ���� true ��ʾ���ԣ�false ��ʾ�����ԣ� Ĭ�Ϸ���false
     */
    virtual bool CanPlaceCaptionBar() const override;
};

template<typename InheritType>
UINT ui::ButtonTemplate<InheritType>::GetControlFlags() const
{
    return this->IsKeyboardEnabled() && this->IsEnabled() && this->IsAllowTabStop() ? UIFLAG_TABSTOP : UIFLAG_DEFAULT;
}

template<typename InheritType>
ButtonTemplate<InheritType>::ButtonTemplate()
{
    this->m_uTextStyle = TEXT_VCENTER | TEXT_CENTER | TEXT_END_ELLIPSIS | TEXT_NOCLIP | TEXT_SINGLELINE;
}

template<typename InheritType>
void ButtonTemplate<InheritType>::HandleEvent(const EventArgs& event)
{
    if (!this->IsMouseEnabled() && 
        (event.Type > kEventMouseBegin) && 
        (event.Type < kEventMouseEnd)) {
        //��ǰ�ؼ���ֹ���������Ϣʱ������������Ϣת�����ϲ㴦��
        if (this->GetParent() != nullptr) {
            this->GetParent()->SendEvent(event);
        }
        else {
            __super::HandleEvent(event);
        }
        return;
    }
    if (event.Type == kEventKeyDown) {
        if (this->IsKeyboardEnabled()) {
            if (event.chKey == VK_SPACE || event.chKey == VK_RETURN) {
                Activate();
                return;
            }
        }
    }
    __super::HandleEvent(event);
}

template<typename InheritType>
inline std::wstring ButtonTemplate<InheritType>::GetType() const { return DUI_CTR_BUTTON; }

template<typename InheritType>
void ButtonTemplate<InheritType>::Activate()
{
    if (!this->IsActivatable()) {
        return;
    }
    this->SendEvent(kEventClick);
}

template<typename InheritType>
bool ButtonTemplate<InheritType>::CanPlaceCaptionBar() const
{
    return true;
}

typedef ButtonTemplate<Control> Button;
typedef ButtonTemplate<Box> ButtonBox;

}	// namespace ui

#endif // UI_CONTROL_BUTTON_H_