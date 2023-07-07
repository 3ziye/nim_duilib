#include "RichText.h"
#include "duilib/Animation/AnimationManager.h"
#include "duilib/Animation/AnimationPlayer.h"
#include "duilib/Core/WindowBuilder.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Utils/DpiManager.h"
#include "duilib/Render/IRender.h"

namespace ui 
{
RichTextSlice::RichTextSlice()
{
}

RichText::RichText()
{
}

RichText::~RichText()
{
}

std::wstring RichText::GetType() const { return DUI_CTR_RICHTEXT; }

UiSize RichText::EstimateText(UiSize szAvailable, bool& bReEstimateSize)
{
    return {600, 100};
    //return __super::EstimateText(szAvailable, bReEstimateSize);
}

void RichText::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    __super::SetAttribute(strName, strValue);
}

void RichText::PaintText(IRender* pRender)
{
    if (pRender == nullptr) {
        return;
    }
    UiRect rc = GetRect();
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;

    UiColor dwTextColor = UiColor(UiColors::Blue);
    pRender->DrawString(rc, L"RichText::PaintText��������δʵ�֡�", dwTextColor, L"", 0);
}

const UiRect& RichText::GetTextPadding() const
{
    return m_rcTextPadding;
}

void RichText::SetTextPadding(const UiRect& padding)
{
    ASSERT((padding.left >= 0) && (padding.top >= 0) && (padding.right >= 0) && (padding.bottom >= 0));
    if ((padding.left < 0) || (padding.top < 0) ||
        (padding.right < 0) || (padding.bottom < 0)) {
        return;
    }
    m_rcTextPadding = padding;
    DpiManager::GetInstance()->ScaleRect(m_rcTextPadding);
    RelayoutOrRedraw();
}

bool RichText::SetRichText(const std::wstring& richText)
{
    //XML���������ݣ�ȫ����װ��WindowBuilder������У��Ա��⵽��ʹ��XML���������Ӷ����ʹ���ά�����Ӷ�
    bool bResult = WindowBuilder::ParseRichTextXmlText(richText, this);
    if (bResult) {
        RelayoutOrRedraw();
    }
    return bResult;
}

void RichText::AppendTextSlice(const RichTextSlice&& textSlice)
{
    m_textSlice.emplace_back(textSlice);
}

void RichText::AppendTextSlice(const RichTextSlice& textSlice)
{
    m_textSlice.emplace_back(textSlice);
}

std::wstring RichText::ToString() const
{
    const std::wstring indentValue = L"    ";
    const std::wstring lineBreak = L"\r\n";
    std::wstring richText = L"<RichText>";
    richText += lineBreak;
    for (const RichTextSlice& textSlice : m_textSlice) {
        richText += ToString(textSlice, indentValue);
    }
    richText += L"</RichText>";
    return richText;
}

std::wstring RichText::ToString(const RichTextSlice& textSlice, const std::wstring& indent) const
{
    const std::wstring indentValue = L"    ";
    const std::wstring lineBreak = L"\r\n";
    std::wstring richText;
    if (textSlice.m_nodeName.empty()) {
        if (!textSlice.m_text.empty()) {
            richText += indent;
            richText += textSlice.m_text;
            richText += lineBreak;
        }
        //û�нڵ����Ƶ�����£���û�����Ժ��ӽڵ㣬ֱ�ӷ���
        return richText;
    }

    //���������б�
    std::wstring attrList;    
    if (!textSlice.m_linkUrl.empty()) {
        attrList += L"href=\"";
        std::wstring url = textSlice.m_linkUrl;
        attrList += textSlice.m_linkUrl;
        attrList += L"\"";
    }
    if (!textSlice.m_bgColor.empty()) {
        attrList += L"color=\"";
        attrList += textSlice.m_bgColor;
        attrList += L"\"";
    }
    if (!textSlice.m_textColor.empty()) {
        attrList += L"color=\"";
        attrList += textSlice.m_textColor;
        attrList += L"\"";
    }
    if (!textSlice.m_fontInfo.m_fontName.empty()) {
        attrList += L"face=\"";
        attrList += textSlice.m_fontInfo.m_fontName;
        attrList += L"\"";
    }
    if (textSlice.m_fontInfo.m_fontSize != 0) {
        attrList += L"size=\"";
        attrList += StringHelper::Printf(L"%d", textSlice.m_fontInfo.m_fontSize);
        attrList += L"\"";
    }

    if(!textSlice.m_childs.empty()) {
        //���ӽڵ㣺�ڵ㿪ʼ
        richText += indent;
        richText += L"<";
        richText += textSlice.m_nodeName;
        if (!attrList.empty()) {
            richText += L" ";
            richText += attrList;
        }
        richText += L">";
        richText += lineBreak;

        //����ӽڵ�
        for (const RichTextSlice& childSlice : textSlice.m_childs) {
            richText += ToString(childSlice, indent + indentValue);
        }

        //�ڵ����
        richText += indent;
        richText += L"</";
        richText += textSlice.m_nodeName;
        richText += L">";
        richText += lineBreak;
    }
    else if (!textSlice.m_linkUrl.empty()) {
        //�������ӽڵ㣺��Ҫ���⴦��
        richText += indent;
        richText += L"<";
        richText += textSlice.m_nodeName;
        if (!attrList.empty()) {
            richText += L" ";
            richText += attrList;
        }
        richText += L">";

        //��ӳ����ӵ��ı�
        richText += textSlice.m_text;

        //�ڵ����
        richText += L"</";
        richText += textSlice.m_nodeName;
        richText += L">";
        richText += lineBreak;
    }
    else {
        //û���ӽڵ㣺��һ���б�ʾ
        richText += indent;
        richText += L"<";
        richText += textSlice.m_nodeName;
        richText += L" ";
        if (!attrList.empty()) {            
            richText += attrList;
        }
        richText += L"/>";
        richText += lineBreak;
    }
    return richText;
}

} // namespace ui

