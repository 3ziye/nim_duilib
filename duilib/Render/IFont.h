#ifndef UI_RENDER_IFONT_H_
#define UI_RENDER_IFONT_H_

#pragma once

#include "duilib/duilib_defs.h"

namespace ui 
{

/** ����ӿڵķ�װ
*/
class UILIB_API IFont
{
public:
    virtual ~IFont() = default;

    /**@brief ��ȡ������
     */
    virtual const wchar_t* FontName() const = 0;

    /**@brief ��ȡ�����С(����߶�)
     */
    virtual int FontSize() const = 0;

    /**@brief ��ȡ������
     */
    virtual int FontWidth() const = 0;

    /**@brief ��ȡ����Weightֵ
     */
    virtual int FontWeight() const = 0;

    /**@brief �Ƿ�Ϊ����
     */
    virtual bool IsBold() const = 0;

    /**@brief �����»���״̬
     */
    virtual bool IsUnderline() const = 0;

    /**@brief �����б��״̬
     */
    virtual bool IsItalic() const = 0;

    /**@brief �����ɾ����״̬
     */
    virtual bool IsStrikeOut() const = 0;
};

/** ����ӿڵĻ������ݽṹ
*/
class UILIB_API UiFont
{
public:
    UiFont():
        m_fontSize(0),
        m_bBold(false),
        m_bUnderline(false),
        m_bItalic(false),
        m_bStrikeOut(false)
    {}

    /** ��������
    */
    std::wstring m_fontName;

    /** �����С����λ�����أ�
    */
    int32_t m_fontSize;

    /** �Ƿ�Ϊ����
     */
    bool m_bBold;

    /** �����»���״̬
     */
    bool m_bUnderline;

    /** �����б��״̬
     */
    bool m_bItalic;

    /** �����ɾ����״̬
     */
    bool m_bStrikeOut;
};

} // namespace ui

#endif // UI_RENDER_IFONT_H_
