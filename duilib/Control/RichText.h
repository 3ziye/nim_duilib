#ifndef UI_CONTROL_RICHTEXT_H_
#define UI_CONTROL_RICHTEXT_H_

#pragma once

#include "duilib/Core/Control.h"
#include "duilib/Render/IRender.h"

namespace ui 
{
class RichTextSlice
{
public:   
    /** �ڵ�����
    */
    UiString m_nodeName;

    /** ��������
    */
    UiString m_text;

    /** �����ӵ�URL: "href"
    */
    UiString m_linkUrl;

    /** ������ɫ: "color"
    */
    UiString m_textColor;

    /** ������ɫ: "color"
    */
    UiString m_bgColor;

    /** ������Ϣ
    */
    UiFont m_fontInfo;

public:
    /** �ӽڵ�
    */
    std::vector<RichTextSlice> m_childs;
};

class UILIB_API RichText : public Control
{
public:
    RichText();
    RichText(const RichText& r) = delete;
    RichText& operator=(const RichText& r) = delete;
    virtual ~RichText();

    /// ��д���෽�����ṩ���Ի����ܣ���ο���������
    virtual std::wstring GetType() const override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void PaintText(IRender* pRender) override;

    /** �����ı������С����͸ߣ�
     *  @param [in] szAvailable ���ô�С���������ڱ߾࣬��������߾�
     *  @return �ؼ����ı������С�������ڱ߾�(Box)����������߾�
     */
    virtual UiSize EstimateText(UiSize szAvailable) override;

public:
    /** ��ȡ�����ڱ߾�
     */
    UiPadding GetTextPadding() const;

    /** ���������ڱ߾���Ϣ, �����ڲ������DPI����Ӧ����
     * @param [in] padding ���ε��ĸ���ֵ�ֱ�����Ӧ���ĸ��ڱ߾�ֵ
     * @param [in] bNeedDpiScale ���� DPI ���ţ�Ĭ��Ϊ true
     */
    void SetTextPadding(UiPadding padding, bool bNeedDpiScale = true);

    /** ��ȡ��ǰ����ID
     * @return ��������ID���ñ���� global.xml �б�ʶ
     */
    std::wstring GetFontId() const;

    /** ���õ�ǰ����ID
     * @param [in] strFontId Ҫ���õ�����ID����ID���� global.xml �д���
     */
    void SetFontId(const std::wstring& strFontId);

    /** ��ȡĬ���ı���ɫ
     */
    std::wstring GetTextColor() const;

    /** ����Ĭ���ı���ɫ
     */
    void SetTextColor(const std::wstring& sTextColor);

    /** ��ȡ�м�౶��
    */
    float GetRowSpacingMul() const;

    /** �����м�౶��
    */
    void SetRowSpacingMul(float fRowSpacingMul);

public:
    /** ���ø�ʽ���ı�
    * @param [in] richText ���и�ʽ���ı�����
    */
    bool SetRichText(const std::wstring& richText);

    /** ׷��һ���ı�Ƭ��
    */
    void AppendTextSlice(const RichTextSlice&& textSlice);

    /** ׷��һ���ı�Ƭ��
    */
    void AppendTextSlice(const RichTextSlice& textSlice);

public:
    /** �������ʽ���ı�
    */
    std::wstring ToString() const;

private:
    /** ������ʽ���ı�, ���ɽ���������ݽṹ
    */
    bool ParseText(std::vector<RichTextData>& outTextData) const;

    /** �ı�Ƭ�ν���Ϊ���ƽṹ
    * @param [in] textSlice �ı�Ƭ��
    * @param [in] parentTextData ��������Ϣ
    * @param [out] textData ��������ı��ṹ
    */
    bool ParseTextSlice(const RichTextSlice& textSlice, 
                        const RichTextData& parentTextData,
                        std::vector<RichTextData>& textData) const;

    /** �������ʽ���ı�
    */
    std::wstring ToString(const RichTextSlice& textSlice, const std::wstring& indent) const;

private:
    /** �ı����Ƶ��ڱ߾�(�ֱ��Ӧ�ĸ��ߵ��ڱ߾��С)
    */
    UiPadding16 m_rcTextPadding;

    /** Ĭ������
    */
    UiString m_sFontId;

    /** Ĭ���ı���ɫ
    */
    UiString m_sTextColor;

    /** �ı����뷽ʽ
    */
    uint32_t m_uTextStyle;

    /** �м�౶��
    */
    float m_fRowSpacingMul;

    /** ���Ƶ��ı����ݣ�����ǰ��
    */
    std::vector<RichTextSlice> m_textSlice;

    /** ���Ƶ��ı����ݣ�������
    */
    std::vector<RichTextData> m_textData;
};

} // namespace ui

#endif // UI_CONTROL_RICHTEXT_H_
