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

    /**@brief ��������LOGFONT
     */
    virtual const LOGFONT* LogFont() const = 0;

    /**@brief ��ȡ������
     */
    virtual const wchar_t* FontName() const = 0;

    /**@brief ��ȡ�����С
     */
    virtual int FontSize() const = 0;

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

    /** ����������Ϣ
    */
    virtual bool UpdateFont(const LOGFONT& logFont) = 0;
};

} // namespace ui

#endif // UI_RENDER_IFONT_H_
