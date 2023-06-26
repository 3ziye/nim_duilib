/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef UI_RENDER_SKIA_SK_TEXT_BOX_H_
#define UI_RENDER_SKIA_SK_TEXT_BOX_H_

#include "include/core/SkCanvas.h"

//���ļ�ԭʼ�ļ��ĳ�����skia/chrome_67/include/utils/SkTextBox.h
//����ԭʼ�ļ������޸ģ��Լ������°汾��skia���루2023-06-25��
//ԭʼ�ļ���chrome 68�Ժ��ɾ���ˡ�

namespace ui
{

/** \class SkTextBox

    SkTextBox is a helper class for drawing 1 or more lines of text
    within a rectangle. The textbox is positioned and clipped by its Frame.
    The Margin rectangle controls where the text is drawn relative to
    the Frame. Line-breaks occur inside the Margin rectangle.

    Spacing is a linear equation used to compute the distance between lines
    of text. Spacing consists of two scalars: mul and add, and the spacing
    between lines is computed as: spacing = font.getSize() * mul + add
*/
class SkTextBox 
{
public:
    SkTextBox();

    //ԭʼ������SkPaint::breakText ������������µ�Skia���������Ѿ�ɾ���ˣ�������Լ�ʵ�ֵİ汾��
    /** Returns the bytes of text that fit within maxWidth.
        @param text           character codes or glyph indices to be measured
        @param byteLength     number of bytes of text to measure
        @param textEncoding   text encoding, SkTextEncoding::kUTF8 or kUTF16 or kUTF32
        @param maxWidth       advance limit; text is measured while advance is less than maxWidth
        @param measuredWidth  returns the width of the text less than or equal to maxWidth
        @return               bytes of text that fit, always less than or equal to length
    */
    static size_t breakText(const void* text, size_t byteLength, SkTextEncoding textEncoding,
                            const SkFont& font, const SkPaint& paint,
                            SkScalar maxWidth, SkScalar* measuredWidth = nullptr);

public:
    //����ģʽ
    enum Mode {
        kOneLine_Mode,   //Char Breakģʽ�����ַ�����
        kLineBreak_Mode, //Word Breakģʽ�������ʷ��У�Ĭ�ϣ�

        kModeCount
    };
    Mode    getMode() const { return (Mode)fMode; }
    void    setMode(Mode);

    //������뷽ʽ
    enum SpacingAlign {
        kStart_SpacingAlign,  //�϶��룬�൱�ڣ�top ����(Ĭ��)
        kCenter_SpacingAlign, //�ж��룬�൱�ڣ�vcener ����
        kEnd_SpacingAlign,    //�¶��룬�൱�ڣ�bottom ����

        kSpacingAlignCount
    };
    SpacingAlign getSpacingAlign() const { return (SpacingAlign)fSpacingAlign; }
    void         setSpacingAlign(SpacingAlign);

    //������뷽ʽ
    enum TextAlign {
        kLeft_Align,        //�����(Ĭ��)
        kCenter_Align,      //�ж���
        kRight_Align,       //�Ҷ���

        kAlignCount
    };
    TextAlign getTextAlign() const { return (TextAlign)fTextAlign; }
    void      setTextAlign(TextAlign);

    void getBox(SkRect*) const;
    void setBox(const SkRect&);
    void setBox(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom);

    //�м�ࣺmulΪ�м��ı�����add Ϊ���Ӷ���
    //���ú�ʵ�ʵ��м��Ϊ��fontHeight * mul + add;
    void getSpacing(SkScalar* mul, SkScalar* add) const;
    void setSpacing(SkScalar mul, SkScalar add);

    void draw(SkCanvas*, 
              const char text[], size_t len, SkTextEncoding, 
              const SkFont&, const SkPaint&);

    void setText(const char text[], size_t len, SkTextEncoding, 
                 const SkFont&, const SkPaint&);
    void draw(SkCanvas*);
    int  countLines() const;
    SkScalar getTextHeight() const;

    sk_sp<SkTextBlob> snapshotTextBlob(SkScalar* computedBottom) const;

    class Visitor {
    public:
        virtual ~Visitor() {}
        virtual void operator()(const char*, size_t, SkTextEncoding, 
                                SkScalar x, SkScalar y, 
                                const SkFont&, const SkPaint&) = 0;
    };

private:
    SkRect      fBox;
    SkScalar    fSpacingMul, fSpacingAdd;
    uint8_t     fMode, fSpacingAlign, fTextAlign;
    const char* fText;
    size_t      fLen;
    SkTextEncoding fTextEncoding;
    const SkPaint* fPaint;
    const SkFont* fFont;

    SkScalar visit(Visitor&, 
                   const char text[], size_t len, SkTextEncoding, 
                   const SkFont&, const SkPaint&) const;
};

class SkTextLineBreaker {
public:
    static int CountLines(const char text[], size_t len, SkTextEncoding, 
                          const SkFont&, const SkPaint&, 
                          SkScalar width);
};

} //namespace ui

#endif //UI_RENDER_SKIA_SK_TEXT_BOX_H_
