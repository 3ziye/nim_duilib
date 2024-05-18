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

void MainForm::OnCloseWindow()
{
	//�رմ��ں��˳����̵߳���Ϣѭ�����رճ���
	PostQuitMessage(0L);
}

void MainForm::ShowPopupMenu(const ui::UiPoint& point)
{
	ui::Menu* menu = new ui::Menu(GetHWND());//��Ҫ���ø����ڣ����ڲ˵�������ʱ�򣬳���״̬����̷Ǽ���״̬
	//���ò˵�xml���ڵ�Ŀ¼
	menu->SetSkinFolder(GetResourcePath());
	std::wstring xml(L"lang_menu.xml");
	menu->ShowMenu(xml, point);

	//��ǰ�����ļ�
	std::wstring currentLangFileName = ui::GlobalManager::Instance().GetLanguageFileName();

	//���������ļ��б����ʾ����
	std::vector<std::pair<std::wstring, std::wstring>> languageList;
	ui::GlobalManager::Instance().GetLanguageList(languageList);
	if (languageList.empty()) {
		languageList.push_back({ currentLangFileName , L""});
	}

	//��̬��Ӳ˵���
	for (auto& lang : languageList) {
		const std::wstring fileName = lang.first;
		std::wstring& displayName = lang.second;

		ui::MenuItem* pMenuItem = new ui::MenuItem(this);
		pMenuItem->SetClass(L"menu_element");
		ui::CheckBox* pCheckBox = new ui::CheckBox(this);
		pCheckBox->SetClass(L"menu_checkbox");
		pCheckBox->SetAttribute(L"margin", L"0,5,0,10");
		pCheckBox->SetText(!displayName.empty() ? displayName : fileName);
		pMenuItem->AddItem(pCheckBox);
		menu->AddMenuItem(pMenuItem);

		if (ui::StringHelper::IsEqualNoCase(fileName, currentLangFileName)) {
			pCheckBox->Selected(true);
		}

		//����ѡ�������¼�
		pMenuItem->AttachClick([fileName](const ui::EventArgs& args) {
			//�л�����
			ui::GlobalManager& globalManager = ui::GlobalManager::Instance();
			if (globalManager.GetLanguageFileName() != fileName) {
				globalManager.ReloadLanguage(L"", fileName, true);
			}			
			return true;
			});
	}
}