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
    return L"dpi_aware";
}

std::wstring MainForm::GetSkinFile()
{
    return L"DpiAware.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
    return kClassName;
}

void MainForm::OnInitWindow()
{
    //���ڳ�ʼ����ɣ����Խ��б�Form�ĳ�ʼ��

}

void MainForm::OnCloseWindow()
{
    //�رմ��ں��˳����̵߳���Ϣѭ�����رճ���
    PostQuitMessage(0L);
}

