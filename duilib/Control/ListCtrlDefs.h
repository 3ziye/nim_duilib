#ifndef UI_CONTROL_LIST_CTRL_DEFS_H_
#define UI_CONTROL_LIST_CTRL_DEFS_H_

#pragma once

#include "duilib/Control/CheckBox.h"

namespace ui
{
/** �еĻ�����Ϣ����������У�
*/
struct ListCtrlColumn
{
    std::wstring text;              //��ͷ���ı�
    int32_t nColumnWidth = 100;     //�п����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMin = 0;    //�п���Сֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nColumnWidthMax = 0;    //�п����ֵ��0��ʾ��Ĭ�����ã����bNeedDpiScaleΪtrue����ִ��DPI����Ӧ����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    bool bSortable = true;          //�Ƿ�֧������
    bool bResizeable = true;        //�Ƿ�֧��ͨ���϶������п�
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox��֧���ڱ�ͷ����������ʾCheckBox��
    bool bNeedDpiScale = true;      //�Ƿ����ֵ��DPI����Ӧ
};

/** �б�������Ļ�����Ϣ������������ݣ�
*/
struct ListCtrlDataItem
{
    size_t nColumnIndex = 0;        //������ڼ��У���Ч��Χ��[0, GetColumnCount())
    std::wstring text;              //�ı�����
    int32_t nTextFormat = -1;       //�ı����뷽ʽ������, �����Խ�Ӧ����Header, ȡֵ�ɲο���IRender.h�е�DrawStringFormat�����Ϊ-1����ʾ��Ĭ�����õĶ��뷽ʽ
    int32_t nImageIndex = -1;       //ͼ����Դ�����ţ���ͼƬ�б�������±�ֵ�����Ϊ-1��ʾ����ʾͼ��
    UiColor textColor;              //�ı���ɫ
    UiColor bkColor;                //������ɫ
    bool bShowCheckBox = true;      //�Ƿ���ʾCheckBox
};

/** �б���ʹ�õ�CheckBox
*/
class ListCtrlCheckBox: public CheckBox
{
public:
    /** ��ȡ�ؼ����ͺ���������
    */
    virtual std::wstring GetType() const override { return L"ListCtrlCheckBox"; }
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override
    {
        if (strName == L"check_box_width") {
            SetCheckBoxWidth(_wtoi(strValue.c_str()), true);
        }
        else {
            __super::SetAttribute(strName, strValue);
        }
    }

    /** ����CheckBox��ռ�Ŀ��ֵ
    */
    void SetCheckBoxWidth(int32_t nWidth, bool bNeedDpiScale)
    {
        if (bNeedDpiScale) {
            GlobalManager::Instance().Dpi().ScaleInt(nWidth);
        }
        if (nWidth < 0) {
            nWidth = 0;
        }
        m_nCheckBoxWidth = nWidth;
    }

    /** ��ȡCheckBox��ռ�Ŀ��ֵ
    */
    int32_t GetCheckBoxWidth()
    {
        if (m_nCheckBoxWidth > 0) {
            return m_nCheckBoxWidth;
        }
        //���û���ã���ѯͼƬ�Ĵ�С������ס
        UiEstSize sz = EstimateSize(UiSize(0, 0));
        int32_t nWidth = sz.cx.GetInt32();
        if (nWidth > 0) {
            UiMargin rcMargin = GetMargin();
            UiPadding rcPadding = GetPadding();
            nWidth += rcMargin.left + rcMargin.right;
            nWidth += rcPadding.left + rcPadding.right;
            m_nCheckBoxWidth = nWidth;

            m_nCheckBoxWidth += GlobalManager::Instance().Dpi().GetScaleInt(2);
        }
        return m_nCheckBoxWidth;
    }

private:
    /** ��ʾCheckBox��ռ�Ŀ�ȣ��������ø��ؼ���Paddingֵ
    */
    int32_t m_nCheckBoxWidth = 0;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_DEFS_H_
