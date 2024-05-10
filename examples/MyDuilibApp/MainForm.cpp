//MainForm.cpp
#include "MainForm.h"

const std::wstring MainForm::kClassName = L"MainForm";

MainForm::MainForm()
{
}


MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
    return L"my_duilib_app";
}

std::wstring MainForm::GetSkinFile()
{
    return L"MyDuilibForm.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
    return kClassName;
}

void MainForm::OnInitWindow()
{
    //���ڳ�ʼ����ɣ����Խ��б�Form�ĳ�ʼ��

}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    //���ڹر�ʱ����ֹ���̵߳���Ϣѭ�����Ӷ��˳�����
    PostQuitMessage(0L);
    return __super::OnClose(uMsg, wParam, lParam, bHandled);
}
