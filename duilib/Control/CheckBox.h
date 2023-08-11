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
    virtual ~CheckBoxTemplate();

    /// ��д���෽�����ṩ���Ի����ܣ���ο���������
    virtual std::wstring GetType() const override;
    virtual void Activate() override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void PaintStateColors(IRender* pRender) override;
    virtual void PaintStateImages(IRender* pRender) override;
    virtual void PaintText(IRender* pRender) override;
    virtual bool HasHotState() override;

    /** �ر�CheckBox���ܣ����CheckBox������ͼƬ����(�������ڵ㣬CheckBox�����ǿ���ͨ�����ؿ������߹رյ�)
    */
    void ClearStateImages();

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

    /** ����ѡ��״̬�����������¼��������½���
    * @param [in] bSelected trueΪѡ��״̬��falseΪ��ѡ��״̬
    */
    virtual void SetSelected(bool bSelected);

    /**
     * @brief ���ÿؼ��Ƿ�ѡ��״̬
     * @param[in] bSelected Ϊ true ʱΪѡ��״̬��false ʱΪȡ��ѡ��״̬
     * @param[in] bTriggerEvent �Ƿ���״̬�ı��¼���true Ϊ���ͣ�����Ϊ false��Ĭ��Ϊ false
     * @return ��
     */
    virtual void Selected(bool bSelected, bool bTriggerEvent = false);

    /** �Ƿ����ѡ��״̬�µı���ɫ���ṩ�麯����Ϊ��ѡ��
       ������ListBox/TreeView�ڵ��ڶ�ѡʱ�������й�ѡ�������Ҫ����ѡ��״̬�ı���ɫ��
    */
    virtual bool CanPaintSelectedColors() const { return true; }

    /** ���ò���ѡ���־��֧����̬ѡ���־��ȫ��ѡ��/����ѡ��/δѡ��
    */
    void SetPartSelected(bool bPartSelected);

    /** ��ǰ�Ƿ�Ϊ����ѡ��
    */
    bool IsPartSelected() const;

    /** ��ȡ��ѡ��ʱ��ͼƬ
     * @param [in] stateType Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
     * @return ����ͼƬ·��������
     */
    std::wstring GetSelectedStateImage(ControlStateType stateType);

    /** ���ñ�ѡ��ʱ��ͼƬ
     * @param [in] stateType Ҫ��������״̬�µ�ͼƬ
     * @param [in] strImage ͼƬ·��������
     */
    void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

    /** ��ȡ��ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ��ȡ����״̬�µ�ǰ��ͼƬ
     */
    std::wstring GetSelectedForeStateImage(ControlStateType stateType);

    /** ���ñ�ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ���ú���״̬�µ�ǰ��ͼƬ
     * @param[in] strImage ͼƬλ��
     */
    void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& strImage);

    /** ��ȡ����ѡ��ʱ��ͼƬ
     * @param [in] stateType Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
     * @return ����ͼƬ·��������
     */
    std::wstring GetPartSelectedStateImage(ControlStateType stateType);

    /** ���ò���ѡ��ʱ��ͼƬ
     * @param [in] stateType Ҫ��������״̬�µ�ͼƬ
     * @param [in] strImage ͼƬ·��������
     */
    void SetPartSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

    /** ��ȡ����ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ��ȡ����״̬�µ�ǰ��ͼƬ
     */
    std::wstring GetPartSelectedForeStateImage(ControlStateType stateType);

    /** ���ò���ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ���ú���״̬�µ�ǰ��ͼƬ
     * @param[in] strImage ͼƬλ��
     */
    void SetPartSelectedForeStateImage(ControlStateType stateType, const std::wstring& strImage);

    /** ��ȡ��ѡ��ʱ���ı���ɫ
     */
    std::wstring GetSelectedTextColor();

    /** ���ñ�ѡ��ʱ���ı���ɫ
     * @param[in] dwTextColor Ҫ���õ���ɫ�ַ���������ɫ������ global.xml �д���
     */
    void SetSelectedTextColor(const std::wstring& dwTextColor);

    /** ��ȡ��ѡ��ʱָ��״̬�µ��ı���ɫ
     * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetSelectedStateTextColor(ControlStateType stateType);

    /** ���ñ�ѡ��ʱָ��״̬�µ��ı���ɫ
     * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
     * @param[in] stateColor Ҫ���õ���ɫ
     */
    void SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

    /** ��ȡ��ѡ��ʱָ��״̬�µ�ʵ�ʱ���Ⱦ�ı���ɫ
     * @param [in] buttonStateType Ҫ��ȡ����״̬�µ���ɫ
     * @param [out] stateType ʵ�ʱ���Ⱦ��״̬
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType);

    /** ��ȡ��ѡ��ʱ�Ŀؼ���ɫ
     * @param [in] stateType Ҫ��ȡ����״̬�µ���ɫ
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetSelectStateColor(ControlStateType stateType);

    /** ���ñ�ѡ��ʱ�Ŀؼ���ɫ
     * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
     * @param[in] stateColor Ҫ���õ���ɫ
     */
    void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor);

    /** ������ѡ��ʱ���¼�
     * @param [in] callback ��ѡ��ʱ�����Ļص�����
     */
    void AttachSelect(const EventCallback& callback) { this->AttachEvent(kEventSelect, callback); }

    /** ����ȡ��ѡ��ʱ���¼�
     * @param [in] callback ȡ��ѡ��ʱ�����Ļص�����
     */
    void AttachUnSelect(const EventCallback& callback) { this->AttachEvent(kEventUnSelect, callback); }

private:
    //ѡ��״̬
    bool m_bSelected;

    //�Ƿ�Ϊ����ѡ��
    bool m_bPartSelected;

    //�Ƿ����Ȼ���Normal״̬
    bool m_bPaintNormalFirst;

    //ѡ��״̬���ı���ɫ
    UiString m_dwSelectedTextColor;

    //ѡ��״̬���ı���ɫ
    StateColorMap* m_pSelectedTextColorMap;

    //ѡ��״̬�ı�����ɫ
    StateColorMap* m_pSelectedColorMap;
};

template<typename InheritType>
CheckBoxTemplate<InheritType>::CheckBoxTemplate() : 
    m_bSelected(false), 
    m_bPartSelected(false),
    m_bPaintNormalFirst(false), 
    m_dwSelectedTextColor(), 
    m_pSelectedTextColorMap(nullptr),
    m_pSelectedColorMap(nullptr)
{
}

template<typename InheritType>
CheckBoxTemplate<InheritType>::~CheckBoxTemplate()
{
    if (m_pSelectedTextColorMap != nullptr) {
        delete m_pSelectedTextColorMap;
        m_pSelectedTextColorMap = nullptr;
    }
    if (m_pSelectedColorMap != nullptr) {
        delete m_pSelectedColorMap;
        m_pSelectedColorMap = nullptr;
    }
}

template<typename InheritType>
inline std::wstring CheckBoxTemplate<InheritType>::GetType() const { return DUI_CTR_CHECKBOX; }

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Activate()
{
    if (!this->IsActivatable()) {
        return;
    }
    Selected(!m_bSelected, true);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelected(bool bSelected)
{ 
    m_bSelected = bSelected; 
    if (!bSelected) {
        //��ѡ��״̬ʱ���Բ���ѡ���Ǹ�λ
        m_bPartSelected = false;
    }
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Selected(bool bSelected, bool bTriggerEvent)
{
    if (m_bSelected == bSelected) {
        return;
    }
    m_bSelected = bSelected;
    if (!bSelected) {
        //��ѡ��״̬ʱ���Բ���ѡ���Ǹ�λ
        m_bPartSelected = false;
    }

    if (bTriggerEvent) {
        if (m_bSelected) {
            this->SendEvent(kEventSelect);
        }
        else {
            this->SendEvent(kEventUnSelect);
        }
    }
    this->Invalidate();
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetPartSelected(bool bPartSelected)
{ 
    if (m_bPartSelected != bPartSelected) {
        m_bPartSelected = bPartSelected;
        this->Invalidate();
    }
}

template<typename InheritType>
bool CheckBoxTemplate<InheritType>::IsPartSelected() const
{
    return m_bPartSelected;
}

template<typename InheritType>
bool CheckBoxTemplate<InheritType>::HasHotState()
{
    if (__super::HasHotState()) {
        return true;
    }
    if ((m_pSelectedColorMap != nullptr) && m_pSelectedColorMap->HasHotColor()) {
        return true;
    }
    if ((m_pSelectedTextColorMap != nullptr) && m_pSelectedTextColorMap->HasHotColor()) {
        return true;
    }
    return false;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"selected") {
        Selected(strValue == L"true", true);
    }
    else if ((strName == L"normal_first") || (strName == L"normalfirst")) {
        SetPaintNormalFirst(strValue == L"true");
    }
    else if ((strName == L"selected_normal_image") || (strName == L"selectednormalimage")){
        SetSelectedStateImage(kControlStateNormal, strValue);
    }
    else if ((strName == L"selected_hot_image") || (strName == L"selectedhotimage")) {
        SetSelectedStateImage(kControlStateHot, strValue);
    }
    else if ((strName == L"selected_pushed_image") || (strName == L"selectedpushedimage")) {
        SetSelectedStateImage(kControlStatePushed, strValue);
    }
    else if ((strName == L"selected_disabled_image") || (strName == L"selecteddisabledimage")) {
        SetSelectedStateImage(kControlStateDisabled, strValue);
    }
    else if ((strName == L"selected_fore_normal_image") || (strName == L"selectedforenormalimage")) {
        SetSelectedForeStateImage(kControlStateNormal, strValue);
    }
    else if ((strName == L"selected_fore_hot_image") || (strName == L"selectedforehotimage")) {
        SetSelectedForeStateImage(kControlStateHot, strValue);
    }
    else if ((strName == L"selected_fore_pushed_image") || (strName == L"selectedforepushedimage")) {
        SetSelectedForeStateImage(kControlStatePushed, strValue);
    }
    else if ((strName == L"selected_fore_disabled_image") || (strName == L"selectedforedisabledimage")) {
        SetSelectedForeStateImage(kControlStateDisabled, strValue);
    }
    else if (strName == L"part_selected_normal_image") {
        SetPartSelectedStateImage(kControlStateNormal, strValue);
    }
    else if (strName == L"part_selected_hot_image") {
        SetPartSelectedStateImage(kControlStateHot, strValue);
    }
    else if (strName == L"part_selected_pushed_image") {
        SetPartSelectedStateImage(kControlStatePushed, strValue);
    }
    else if (strName == L"part_selected_disabled_image") {
        SetPartSelectedStateImage(kControlStateDisabled, strValue);
    }
    else if (strName == L"part_selected_fore_normal_image") {
        SetPartSelectedForeStateImage(kControlStateNormal, strValue);
    }
    else if (strName == L"part_selected_fore_hot_image") {
        SetPartSelectedForeStateImage(kControlStateHot, strValue);
    }
    else if (strName == L"part_selected_fore_pushed_image") {
        SetPartSelectedForeStateImage(kControlStatePushed, strValue);
    }
    else if (strName == L"part_selected_fore_disabled_image") {
        SetPartSelectedForeStateImage(kControlStateDisabled, strValue);
    }
    else if ((strName == L"selected_text_color") || (strName == L"selectedtextcolor") ){
        SetSelectedTextColor(strValue);
    }
    else if ((strName == L"selected_normal_text_color") || (strName == L"selectednormaltextcolor")) {
        SetSelectedStateTextColor(kControlStateNormal, strValue);
    }
    else if ((strName == L"selected_hot_text_color") || (strName == L"selectedhottextcolor") ){
        SetSelectedStateTextColor(kControlStateHot, strValue);
    }
    else if ((strName == L"selected_pushed_text_color") || (strName == L"selectedpushedtextcolor")) {
        SetSelectedStateTextColor(kControlStatePushed, strValue);
    }
    else if ((strName == L"selected_disabled_text_color") || (strName == L"selecteddisabledtextcolor") ){
        SetSelectedStateTextColor(kControlStateDisabled, strValue);
    }
    else if ((strName == L"selected_normal_color") || (strName == L"selectednormalcolor") ){
        SetSelectedStateColor(kControlStateNormal, strValue);
    }
    else if ((strName == L"selected_hot_color") || (strName == L"selectedhotcolor") ){
        SetSelectedStateColor(kControlStateHot, strValue);
    }
    else if ((strName == L"selected_pushed_color") || (strName == L"selectedpushedcolor") ){
        SetSelectedStateColor(kControlStatePushed, strValue);
    }
    else if ((strName == L"selected_disabled_color") || (strName == L"selecteddisabledcolor") ){
        SetSelectedStateColor(kControlStateDisabled, strValue);
    }
    else if ((strName == L"switch_select") || (strName == L"switchselect")) {
        Selected(!IsSelected());
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::ClearStateImages()
{
    __super::ClearStateImages();
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStateColors(IRender* pRender)
{
    if (!IsSelected() || !CanPaintSelectedColors()) {
        __super::PaintStateColors(pRender);
        return;
    }

    if (m_pSelectedColorMap == nullptr) {
        if (IsPaintNormalFirst()) {
            this->PaintStateColor(pRender, this->GetPaintRect(), this->GetState());
        }
    }
    else {
        if (IsPaintNormalFirst() && !m_pSelectedColorMap->HasStateColors()) {
            this->PaintStateColor(pRender, this->GetPaintRect(), this->GetState());
        }
        else {
            m_pSelectedColorMap->PaintStateColor(pRender, this->GetPaintRect(), this->GetState());
        }
    }
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStateImages(IRender* pRender)
{
    if (!IsSelected()) {
        //δѡ��״̬
        __super::PaintStateImages(pRender);
        return;
    }

    if (this->IsPartSelected()) {
        //����ѡ��״̬
        bool bPainted = false;
        if (this->HasStateImage(kStateImagePartSelectedBk)) {
            this->PaintStateImage(pRender, kStateImagePartSelectedBk, this->GetState());
            bPainted = true;
        }
        if (this->HasStateImage(kStateImagePartSelectedFore)) {
            this->PaintStateImage(pRender, kStateImagePartSelectedFore, this->GetState());
            bPainted = true;
        }
        if (bPainted) {
            //����Ѿ������˲���ѡ��״̬������
            return;
        }
    }

    //ȫ��ѡ��״̬
    if (IsPaintNormalFirst() && !this->HasStateImage(kStateImageSelectedBk)) {
        this->PaintStateImage(pRender, kStateImageBk, this->GetState());
    }
    else {
        this->PaintStateImage(pRender, kStateImageSelectedBk, this->GetState());
    }

    if (IsPaintNormalFirst() && !this->HasStateImage(kStateImageSelectedFore)) {
        this->PaintStateImage(pRender, kStateImageFore, this->GetState());
    }
    else {
        this->PaintStateImage(pRender, kStateImageSelectedFore, this->GetState());
    }
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintText(IRender* pRender)
{
    if (!IsSelected()) {
        __super::PaintText(pRender);
        return;
    }
    std::wstring textValue = this->GetText();
    if (textValue.empty() || (pRender == nullptr)) {
        return;
    }
    UiRect rc = this->GetRect();
    UiPadding rcPadding = this->GetControlPadding();
    rc.Deflate(rcPadding);
    rc.Deflate(this->GetTextPadding());

    auto stateType = this->GetState();
    std::wstring clrColor = GetPaintSelectedStateTextColor(this->GetState(), stateType);
    if (clrColor.empty()) {
        clrColor = m_dwSelectedTextColor.empty() ? this->GetPaintStateTextColor(this->GetState(), stateType) : m_dwSelectedTextColor.c_str();
    }
    UiColor dwClrColor = this->GetUiColor(clrColor);

    uint32_t uTextStyle = this->GetTextStyle();
    if (this->IsSingleLine()) {        
        uTextStyle |= TEXT_SINGLELINE;        
    }
    else {
        uTextStyle &= ~TEXT_SINGLELINE;
    }
    this->SetTextStyle(uTextStyle, false);

    if (this->GetAnimationManager().GetAnimationPlayer(kAnimationHot)) {
        if ((stateType == kControlStateNormal || stateType == kControlStateHot)
            && !GetSelectedStateTextColor(kControlStateHot).empty()) {
            std::wstring clrStateColor = GetSelectedStateTextColor(kControlStateNormal);
            if (!clrStateColor.empty()) {
                UiColor dwWinColor = this->GetUiColor(clrStateColor);
                pRender->DrawString(rc, textValue, dwWinColor, this->GetFontId(), this->GetTextStyle());
            }

            if (this->GetHotAlpha() > 0) {
                std::wstring textColor = GetSelectedStateTextColor(kControlStateHot);
                if (!textColor.empty()) {
                    UiColor dwTextColor = this->GetUiColor(textColor);
                    pRender->DrawString(rc, textValue, dwTextColor, this->GetFontId(), this->GetTextStyle(), (BYTE)this->GetHotAlpha());
                }
            }

            return;
        }
    }

    pRender->DrawString(rc, textValue, dwClrColor, this->GetFontId(), this->GetTextStyle());
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedStateImage(ControlStateType stateType)
{
    return this->GetStateImage(kStateImageSelectedBk, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage)
{
    this->SetStateImage(kStateImageSelectedBk, stateType, strImage);
    this->RelayoutOrRedraw();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedForeStateImage(ControlStateType stateType)
{
    return this->GetStateImage(kStateImageSelectedFore, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& strImage)
{
    this->SetStateImage(kStateImageSelectedFore, stateType, strImage);
    this->RelayoutOrRedraw();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetPartSelectedStateImage(ControlStateType stateType)
{
    return this->GetStateImage(kStateImagePartSelectedBk, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetPartSelectedStateImage(ControlStateType stateType, const std::wstring& strImage)
{
    this->SetStateImage(kStateImagePartSelectedBk, stateType, strImage);
    this->RelayoutOrRedraw();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetPartSelectedForeStateImage(ControlStateType stateType)
{
    return this->GetStateImage(kStateImagePartSelectedFore, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetPartSelectedForeStateImage(ControlStateType stateType, const std::wstring& strImage)
{
    this->SetStateImage(kStateImagePartSelectedFore, stateType, strImage);
    this->RelayoutOrRedraw();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedTextColor()
{
    return m_dwSelectedTextColor.c_str();
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
    if (m_pSelectedTextColorMap != nullptr) {
        return m_pSelectedTextColorMap->GetStateColor(stateType);
    }
    return std::wstring();
}

template<typename InheritType /*= Control*/>
void ui::CheckBoxTemplate<InheritType>::SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor)
{
    if (m_pSelectedTextColorMap == nullptr) {
        m_pSelectedTextColorMap = new StateColorMap;
        m_pSelectedTextColorMap->SetControl(this);
    }
    m_pSelectedTextColorMap->SetStateColor(stateType, dwTextColor);
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
    if (m_pSelectedColorMap != nullptr) {
        return m_pSelectedColorMap->GetStateColor(stateType);
    }
    return std::wstring();
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor)
{
    if (m_pSelectedColorMap == nullptr) {
        m_pSelectedColorMap = new StateColorMap;
        m_pSelectedColorMap->SetControl(this);
    }
    m_pSelectedColorMap->SetStateColor(stateType, stateColor);
    this->Invalidate();
}

typedef CheckBoxTemplate<Control> CheckBox;
typedef CheckBoxTemplate<Box> CheckBoxBox;

} // namespace ui

#endif // UI_CONTROL_CHECKBOX_H_