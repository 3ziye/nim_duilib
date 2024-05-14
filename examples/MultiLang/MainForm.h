#ifndef MAIN_FORM_H_
#define MAIN_FORM_H_

#pragma once

// duilib
#include "duilib/duilib.h"

/** Ӧ�ó����������ʵ��
*/
class MainForm : public ui::WindowImplBase
{
public:
    MainForm();
    virtual ~MainForm();

    /**  ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ƥ��Ŀ¼
    * @return ������ʵ�ֲ����ش���Ƥ��Ŀ¼
    */
    virtual std::wstring GetSkinFolder() override;

    /**  ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ƥ�� XML �����ļ�
    * @return ������ʵ�ֲ����ش���Ƥ�� XML �����ļ�
    *         ���ص����ݣ�������XML�ļ����ݣ����ַ�'<'Ϊ��ʼ���ַ�������
    *         �������ļ�·����������'<'�ַ���ʼ���ַ��������ļ�Ҫ��GetSkinFolder()·�����ܹ��ҵ�
    */
    virtual std::wstring GetSkinFile() override;

    /** ��������ʱ�����ã�������ʵ�����Ի�ȡ����Ψһ��������
    * @return ������ʵ�ֲ����ش���Ψһ��������
    */
    virtual std::wstring GetWindowClassName() const override;

    /** �����ڴ�������Ժ���ô˺���������������һЩ��ʼ���Ĺ���
    */
    virtual void OnInitWindow() override;

    /** �����ڼ������ر�ʱ���ô˺���������������һЩ��β����
    */
    virtual void OnCloseWindow() override;

    /** ���ڵ�����
    */
    static const std::wstring kClassName;

private:
    /** ��ʾ�˵�
    * @param [in] point ��ʾλ�����꣬��Ļ����
    */
    void ShowPopupMenu(const ui::UiPoint& point);
};

#endif //MAIN_FORM_H_

