#ifndef UI_RENDER_FONT_GDI_H_
#define UI_RENDER_FONT_GDI_H_

#pragma once

#include "duilib/Render/IFont.h"

namespace ui 
{

/** GDI����ӿڵ�ʵ��
*/
class UILIB_API Font_GDI: public IFont
{
public:
    Font_GDI() : 
        m_logFont{ 0, }, 
        m_hFont(nullptr) 
    {
    }

    explicit Font_GDI(const LOGFONT& logFont): 
        m_logFont(logFont)
    {
        m_hFont = ::CreateFontIndirect(&m_logFont);
        ASSERT(m_hFont != nullptr);
    }

    virtual ~Font_GDI()
    {
        if (m_hFont != nullptr) {
            ::DeleteObject(m_hFont);
            m_hFont = nullptr;
        }
    }

    Font_GDI(const Font_GDI&) = delete;
    Font_GDI& operator=(const Font_GDI&) = delete;

    /**@brief ��ȡ������
     */
    virtual const wchar_t* FontName() const override { return m_logFont.lfFaceName; }

    /**@brief ��ȡ�����С
     */
    virtual int FontSize() const override { return -m_logFont.lfHeight; }

    /**@brief ��ȡ������
     */
    virtual int FontWidth() const override { return m_logFont.lfWidth; }

    /**@brief ��ȡ����Weightֵ
     */
    virtual int FontWeight() const override { return m_logFont.lfWeight; }

    /**@brief �Ƿ�Ϊ����
     */
    virtual bool IsBold() const override { return m_logFont.lfWeight >= FW_BOLD; }

    /**@brief �����»���״̬
     */
    virtual bool IsUnderline() const override { return m_logFont.lfUnderline; }

    /**@brief �����б��״̬
     */
    virtual bool IsItalic() const override { return m_logFont.lfItalic; }

    /**@brief �����ɾ����״̬
     */
    virtual bool IsStrikeOut() const override { return m_logFont.lfStrikeOut; }

    /** ��ȡ������
    */
    HFONT GetFontHandle() const { return m_hFont; }

private:
    //������Ϣ
    LOGFONT m_logFont;

    //������
    HFONT m_hFont;
};

} // namespace ui

#endif // UI_RENDER_FONT_GDI_H_
