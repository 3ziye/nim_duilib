#include "MainForm.h"

const std::wstring MainForm::kClassName = L"MultiLang";

MainForm::MainForm()
{
}


MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
    return L"MultiLang";
}

std::wstring MainForm::GetSkinFile()
{
    return L"MultiLang.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
    return kClassName;
}

void MainForm::OnInitWindow()
{
    //���ڳ�ʼ����ɣ����Խ��б�Form�ĳ�ʼ��

	/* Show select language menu */
	ui::Button* select = static_cast<ui::Button*>(FindControl(L"language"));
	ASSERT(select != nullptr);
	if (select == nullptr) {
		return;
	}
	select->AttachClick([this](const ui::EventArgs& args) {
		ui::UiRect rect = args.pSender->GetPos();
		ui::UiPoint point;
		point.x = rect.left;
		point.y = rect.bottom;
		ClientToScreen(point);

		ShowPopupMenu(point);
		return true;
		});
}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    //���ڹر�ʱ����ֹ���̵߳���Ϣѭ�����Ӷ��˳�����
    PostQuitMessage(0L);
    return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void MainForm::ShowPopupMenu(const ui::UiPoint& point)
{
	ui::CMenuWnd* menu = new ui::CMenuWnd(GetHWND());//��Ҫ���ø����ڣ����ڲ˵�������ʱ�򣬳���״̬����̷Ǽ���״̬
	//���ò˵�xml���ڵ�Ŀ¼
	menu->SetSkinFolder(GetResourcePath());
	std::wstring xml(L"lang_menu.xml");
	menu->ShowMenu(xml, point);

	//���µ�ǰѡ�������
	ui::CheckBox* checkBoxChinese = dynamic_cast<ui::CheckBox*>(menu->FindControl(L"checkbox_lang_chinese"));
	if (checkBoxChinese != nullptr) {
		checkBoxChinese->Selected(false);
	}
	ui::CheckBox* checkBoxEnglish = dynamic_cast<ui::CheckBox*>(menu->FindControl(L"checkbox_lang_english"));
	if (checkBoxEnglish != nullptr) {
		checkBoxEnglish->Selected(true);
	}
}