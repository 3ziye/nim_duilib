#ifndef UI_CONTROL_CHECKBOX_H_
#define UI_CONTROL_CHECKBOX_H_

#pragma once

#include "duilib/Control/Button.h"

namespace ui {

template<typename InheritType = Control>
class UILIB_API CheckBoxTemplate : public ButtonTemplate<InheritType>
{
public:
    CheckBoxTemplate();

    /// ��д���෽�����ṩ���Ի����ܣ���ο���������
    virtual std::wstring GetType() const override;
#if defined(ENABLE_UIAUTOMATION)
    virtual UIAControlProvider* GetUIAProvider() override;
#endif
    virtual void Activate() override;
    virtual Image* GetEstimateImage() override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void PaintStatusColor(IRender* pRender) override;
    virtual void PaintStatusImage(IRender* pRender) override;
    virtual void PaintText(IRender* pRender) override;
    virtual bool HasHotState() override;

    /**
     * @brief ѡ��״̬�£�û�����ñ���ɫ�򱳾�ͼʱ���Ƿ��÷�ѡ��״̬�Ķ�Ӧ����������
     * @return ���� true Ϊѡ��״̬������Ϊ false
     */
    bool IsPaintNormalFirst() const { return m_bPaintNormalFirst; }

    /**
     * @brief ���ÿؼ�ѡ��״̬�£�û�����ñ���ɫ�򱳾�ͼʱ���÷�ѡ��״̬�Ķ�Ӧ����������
     * @param[in] bFirst Ϊ true ���Ʒ�ѡ��״̬���ԣ�false ������
     * @return ��
     */
    void SetPaintNormalFirst(bool bFirst) { m_bPaintNormalFirst = bFirst; }

    /**
     * @brief �жϵ�ǰ�Ƿ���ѡ��״̬
     * @return ���� true Ϊѡ��״̬������Ϊ false
     */
    bool IsSelected() const { return m_bSelected; }

    /**
     * @brief ���ÿؼ��Ƿ�ѡ��״̬
     * @param[in] bSelected Ϊ true ʱΪѡ��״̬��false ʱΪȡ��ѡ��״̬
     * @param[in] bTriggerEvent �Ƿ���״̬�ı��¼���true Ϊ���ͣ�����Ϊ false��Ĭ��Ϊ false
     * @return ��
     */
    virtual void Selected(bool bSelected, bool bTriggerEvent = false);

    /**
     * @brief ��ȡ��ѡ��ʱ��ͼƬ
     * @param[in] stateType Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
     * @return ����ͼƬλ��
     */
    std::wstring GetSelectedStateImage(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱ��ͼƬ
     * @param[in] stateType Ҫ��������״̬�µ�ͼƬ
     * @param[in] strImage ͼƬ��ַ
     * @return ��
     */
    void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

    /**
     * @brief ��ȡ��ѡ��ʱ���ı���ɫ
     * @return ���ر�ѡ��ʱ���ı���ɫ
     */
    std::wstring GetSelectedTextColor();

    /**
     * @brief ���ñ�ѡ��ʱ���ı���ɫ
     * @param[in] dwTextColor Ҫ���õ���ɫ�ַ���������ɫ������ global.xml �д���
     * @return ��
     */
    void SetSelectedTextColor(const std::wstring& dwTextColor);

    /**
     * @brief ��ȡ��ѡ��ʱָ��״̬�µ��ı���ɫ
     * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetSelectedStateTextColor(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱָ��״̬�µ��ı���ɫ
     * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
     * @param[in] stateColor Ҫ���õ���ɫ
     * @return ��
     */
    void SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

    /**
     * @brief ��ȡ��ѡ��ʱָ��״̬�µ�ʵ�ʱ���Ⱦ�ı���ɫ
     * @param[in] buttonStateType Ҫ��ȡ����״̬�µ���ɫ
     * @param[out] stateType ʵ�ʱ���Ⱦ��״̬
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType);

    /**
     * @brief ��ȡ��ѡ��ʱ�Ŀؼ���ɫ
     * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetSelectStateColor(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱ�Ŀؼ���ɫ
     * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
     * @param[in] stateColor Ҫ���õ���ɫ
     * @return ��
     */
    void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor);

    /**
     * @brief ��ȡ��ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ��ȡ����״̬�µ�ǰ��ͼƬ
     * @return ����ͼƬλ��
     */
    std::wstring GetSelectedForeStateImage(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ���ú���״̬�µ�ǰ��ͼƬ
     * @param[in] pStrImage ͼƬλ��
     * @return ��
     */
    void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage);

    /**
     * @brief ������ѡ��ʱ���¼�
     * @param[in] callback ��ѡ��ʱ�����Ļص�����
     * @return ��
     */
    void AttachSelect(const EventCallback& callback) { this->AttachEvent(kEventSelect, callback); }

    /**
     * @brief ����ȡ��ѡ��ʱ���¼�
     * @param[in] callback ȡ��ѡ��ʱ�����Ļص�����
     * @return ��
     */
    void AttachUnSelect(const EventCallback& callback) { this->AttachEvent(kEventUnSelect, callback); }

protected:
    bool			m_bSelected;
    bool			m_bPaintNormalFirst;
    std::wstring	m_dwSelectedTextColor;
    StateColorMap	m_selectedTextColorMap;
    StateColorMap	m_selectedColorMap;
};

template<typename InheritType>
CheckBoxTemplate<InheritType>::CheckBoxTemplate() : m_bSelected(false), m_bPaintNormalFirst(false), m_dwSelectedTextColor(), m_selectedColorMap()
{
    m_selectedColorMap.SetControl(this);
    m_selectedTextColorMap.SetControl(this);
}

template<typename InheritType>
inline std::wstring CheckBoxTemplate<InheritType>::GetType() const { return DUI_CTR_CHECKBOX; }
#if defined(ENABLE_UIAUTOMATION)
template<typename InheritType>
inline UIAControlProvider* CheckBoxTemplate<InheritType>::GetUIAProvider()
{
    if (this->m_pUIAProvider == nullptr)
    {
        this->m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIACheckBoxProvider(this));
    }
    return this->m_pUIAProvider;
}
#endif

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Activate()
{
    if (!this->IsActivatable()) {
        return;
    }
    Selected(!m_bSelected, true);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Selected(bool bSelected, bool bTriggerEvent)
{
    if (m_bSelected == bSelected) {
        return;
    }
    m_bSelected = bSelected;

    if (bTriggerEvent) {
        if (m_bSelected) {
            this->SendEvent(kEventSelect);
        }
        else {
            this->SendEvent(kEventUnSelect);
        }
    }

#if defined(ENABLE_UIAUTOMATION)
    if (this->m_pUIAProvider != nullptr && UiaClientsAreListening()) {
        VARIANT vtOld = { 0 }, vtNew = { 0 };
        vtOld.vt = vtNew.vt = VT_I4;
        vtOld.lVal = m_bSelected ? ToggleState_Off : ToggleState_On;
        vtNew.lVal = m_bSelected ? ToggleState_On : ToggleState_Off;

        UiaRaiseAutomationPropertyChangedEvent(this->m_pUIAProvider, UIA_ToggleToggleStatePropertyId, vtOld, vtNew);

    }
#endif

    this->Invalidate();
}

template<typename InheritType>
bool CheckBoxTemplate<InheritType>::HasHotState()
{
    return m_selectedColorMap.HasHotColor() || m_selectedTextColorMap.HasHotColor() || __super::HasHotState();
}

template<typename InheritType>
Image* CheckBoxTemplate<InheritType>::GetEstimateImage()
{
    Image* estimateImage = __super::GetEstimateImage();
    if (estimateImage == nullptr) {
        estimateImage = this->GetImageMap()->GetEstimateImage(kStateImageSelectedBk);
    }

    return estimateImage;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"selected") Selected(strValue == L"true", true);
    else if (strName == L"normalfirst") SetPaintNormalFirst(strValue == L"true");
    else if (strName == L"selectednormalimage") SetSelectedStateImage(kControlStateNormal, strValue);
    else if (strName == L"selectedhotimage") SetSelectedStateImage(kControlStateHot, strValue);
    else if (strName == L"selectedpushedimage") SetSelectedStateImage(kControlStatePushed, strValue);
    else if (strName == L"selecteddisabledimage") SetSelectedStateImage(kControlStateDisabled, strValue);
    else if (strName == L"selectedtextcolor") SetSelectedTextColor(strValue);
    else if (strName == L"selectednormaltextcolor") SetSelectedStateTextColor(kControlStateNormal, strValue);
    else if (strName == L"selectedhottextcolor")	SetSelectedStateTextColor(kControlStateHot, strValue);
    else if (strName == L"selectedpushedtextcolor") SetSelectedStateTextColor(kControlStatePushed, strValue);
    else if (strName == L"selecteddisabledtextcolor") SetSelectedStateTextColor(kControlStateDisabled, strValue);
    else if (strName == L"selectednormalcolor") SetSelectedStateColor(kControlStateNormal, strValue);
    else if (strName == L"selectedhotcolor") SetSelectedStateColor(kControlStateHot, strValue);
    else if (strName == L"selectedpushedcolor") SetSelectedStateColor(kControlStatePushed, strValue);
    else if (strName == L"selecteddisabledcolor") SetSelectedStateColor(kControlStateDisabled, strValue);
    else if (strName == L"selectedforenormalimage") SetSelectedForeStateImage(kControlStateNormal, strValue);
    else if (strName == L"selectedforehotimage") SetSelectedForeStateImage(kControlStateHot, strValue);
    else if (strName == L"selectedforepushedimage") SetSelectedForeStateImage(kControlStatePushed, strValue);
    else if (strName == L"selectedforedisabledimage") SetSelectedForeStateImage(kControlStateDisabled, strValue);
    else if (strName == L"switchselect") Selected(!IsSelected());
    else __super::SetAttribute(strName, strValue);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusColor(IRender* pRender)
{
    if (!IsSelected()) {
        __super::PaintStatusColor(pRender);
        return;
    }

    if (IsPaintNormalFirst() && !m_selectedColorMap.HasStateColors()) {
        this->GetColorMap()->PaintStateColor(pRender, this->GetPaintRect(), this->GetState());
    }
    else {
        m_selectedColorMap.PaintStateColor(pRender, this->GetPaintRect(), this->GetState());
    }
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusImage(IRender* pRender)
{
    if (!IsSelected()) {
        __super::PaintStatusImage(pRender);
        return;
    }

    if (IsPaintNormalFirst() && !this->GetImageMap()->HasImageType(kStateImageSelectedBk)) {
        this->GetImageMap()->PaintStateImage(pRender, kStateImageBk, this->GetState());
    }
    else {
        this->GetImageMap()->PaintStateImage(pRender, kStateImageSelectedBk, this->GetState());
    }

    if (IsPaintNormalFirst() && !this->GetImageMap()->HasImageType(kStateImageSelectedFore)) {
        this->GetImageMap()->PaintStateImage(pRender, kStateImageFore, this->GetState());
    }
    else {
        this->GetImageMap()->PaintStateImage(pRender, kStateImageSelectedFore, this->GetState());
    }
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintText(IRender* pRender)
{
    if (!IsSelected()) {
        __super::PaintText(pRender);
        return;
    }

    if (this->GetText().empty()) {
        return;
    }
    UiRect rc = this->GetRect();
    rc.left += this->m_rcTextPadding.left;
    rc.right -= this->m_rcTextPadding.right;
    rc.top += this->m_rcTextPadding.top;
    rc.bottom -= this->m_rcTextPadding.bottom;

    auto stateType = this->GetState();
    std::wstring clrColor = GetPaintSelectedStateTextColor(this->GetState(), stateType);
    if (clrColor.empty()) {
        clrColor = m_dwSelectedTextColor.empty() ? this->GetPaintStateTextColor(this->GetState(), stateType) : m_dwSelectedTextColor;
    }
    UiColor dwClrColor = this->GetWindowColor(clrColor);

    if (this->m_bSingleLine) {
        this->m_uTextStyle |= TEXT_SINGLELINE;
    }
    else {
        this->m_uTextStyle &= ~~TEXT_SINGLELINE;
    }

    if (this->GetAnimationManager().GetAnimationPlayer(kAnimationHot)) {
        if ((stateType == kControlStateNormal || stateType == kControlStateHot)
            && !GetSelectedStateTextColor(kControlStateHot).empty()) {
            std::wstring clrStateColor = GetSelectedStateTextColor(kControlStateNormal);
            if (!clrStateColor.empty()) {
                UiColor dwWinColor = this->GetWindowColor(clrStateColor);
                pRender->DrawString(rc, this->GetText(), dwWinColor, this->m_sFontId, this->m_uTextStyle);
            }

            if (this->GetHotAlpha() > 0) {
                std::wstring textColor = GetSelectedStateTextColor(kControlStateHot);
                if (!textColor.empty()) {
                    UiColor dwTextColor = this->GetWindowColor(textColor);
                    pRender->DrawString(rc, this->GetText(), dwTextColor, this->m_sFontId, this->m_uTextStyle, (BYTE)this->GetHotAlpha());
                }
            }

            return;
        }
    }

    pRender->DrawString(rc, this->GetText(), dwClrColor, this->m_sFontId, this->m_uTextStyle);
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedStateImage(ControlStateType stateType)
{
    return this->GetImageMap()->GetImagePath(kStateImageSelectedBk, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
    this->GetImageMap()->SetImageString(kStateImageSelectedBk, stateType, pStrImage);
    if (this->GetFixedWidth() == DUI_LENGTH_AUTO || this->GetFixedHeight() == DUI_LENGTH_AUTO) {
        this->ArrangeAncestor();
    }
    else {
        this->Invalidate();
    }
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedTextColor()
{
    return m_dwSelectedTextColor;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedTextColor(const std::wstring& dwTextColor)
{
    m_dwSelectedTextColor = dwTextColor;
    this->Invalidate();
}

template<typename InheritType /*= Control*/>
std::wstring ui::CheckBoxTemplate<InheritType>::GetSelectedStateTextColor(ControlStateType stateType)
{
    return m_selectedTextColorMap.GetStateColor(stateType);
}

template<typename InheritType /*= Control*/>
void ui::CheckBoxTemplate<InheritType>::SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor)
{
    m_selectedTextColorMap.SetStateColor(stateType, dwTextColor);
    this->Invalidate();
}

template<typename InheritType /*= Control*/>
std::wstring ui::CheckBoxTemplate<InheritType>::GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType)
{
    stateType = buttonStateType;
    if (stateType == kControlStatePushed && GetSelectedStateTextColor(kControlStatePushed).empty()) {
        stateType = kControlStateHot;
    }
    if (stateType == kControlStateHot && GetSelectedStateTextColor(kControlStateHot).empty()) {
        stateType = kControlStateNormal;
    }
    if (stateType == kControlStateDisabled && GetSelectedStateTextColor(kControlStateDisabled).empty()) {
        stateType = kControlStateNormal;
    }
    return GetSelectedStateTextColor(stateType);
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectStateColor(ControlStateType stateType)
{
    return m_selectedColorMap.GetStateColor(stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor)
{
    m_selectedColorMap.SetStateColor(stateType, stateColor);
    this->Invalidate();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedForeStateImage(ControlStateType stateType)
{
    return this->GetImageMap()->GetImagePath(kStateImageSelectedFore, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
    this->GetImageMap()->SetImageString(kStateImageSelectedFore, stateType, pStrImage);
    if (this->GetFixedWidth() == DUI_LENGTH_AUTO || this->GetFixedHeight() == DUI_LENGTH_AUTO) {
        this->ArrangeAncestor();
    }
    else {
        this->Invalidate();
    }
}

typedef CheckBoxTemplate<Control> CheckBox;
typedef CheckBoxTemplate<Box> CheckBoxBox;

} // namespace ui

#endif // UI_CONTROL_CHECKBOX_H_