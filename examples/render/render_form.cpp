#include "render_form.h"
#include "RenderTest1.h"
#include "RenderTest2.h"
#include "RenderTest3.h"

const std::wstring RenderForm::kClassName = L"render";

RenderForm::RenderForm()
{
}


RenderForm::~RenderForm()
{
}

std::wstring RenderForm::GetSkinFolder()
{
	return L"render";
}

std::wstring RenderForm::GetSkinFile()
{
	return L"render.xml";
}

std::wstring RenderForm::GetWindowClassName() const
{
	return kClassName;
}

void RenderForm::OnInitWindow()
{
	TestPropertyGrid();
}

LRESULT RenderForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	CheckPropertyGridResult();
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

ui::Control* RenderForm::CreateControl(const std::wstring& strClass)
{
	if (strClass == L"RenderTest1") {
		return new ui::RenderTest1;
	}
	else if (strClass == L"RenderTest2") {
		return new ui::RenderTest2;
	}
	if (strClass == L"RenderTest3") {
		return new ui::RenderTest3;
	}
	return nullptr;
}

void RenderForm::TestPropertyGrid()
{
	ui::PropertyGrid* pPropertyGrid = dynamic_cast<ui::PropertyGrid*>(FindControl(L"property_grid_test"));
	if (pPropertyGrid == nullptr) {
		return;
	}
	pPropertyGrid->SetEnableHeaderCtrl(true, L"Property", L"Value");
	pPropertyGrid->SetEnableDescriptionArea(true);

	ui::PropertyGridGroup* pGroup = nullptr;
	
	pGroup = pPropertyGrid->AddGroup(L"����һ", L"������Ϣһ", (size_t)this);
	ASSERT(pGroup->GetGroupData() == (size_t)this);
	auto p = pPropertyGrid->AddTextProperty(pGroup, L"����1", L"ֵ1", L"����1��������Ϣ", (size_t)this);
	ASSERT(p->GetPropertyData() == (size_t)this);

	pGroup = pPropertyGrid->AddGroup(L"�����", L"������Ϣ��");
	pPropertyGrid->AddTextProperty(pGroup, L"����2", L"ֵ2", L"����2��������Ϣ");

	pGroup = pPropertyGrid->AddGroup(L"������", L"������Ϣ��");
	auto p0 = pPropertyGrid->AddTextProperty(pGroup, L"����3-0(�ı�)", L"�ı�ֵ3-0", L"����3��������Ϣ");
	auto p1 = pPropertyGrid->AddTextProperty(pGroup, L"����3-1(����)", L"3", L"����3��������Ϣ������Spin�ؼ�������");
	p1->SetEnableSpin(true, -10, 10);

	auto p2 = pPropertyGrid->AddTextProperty(pGroup, L"����3-2(ֻ��)", L"ֵ3-2", L"����3��������Ϣ");
	p2->SetReadOnly(true);

	auto p3 = pPropertyGrid->AddTextProperty(pGroup, L"����3-3(����)", L"ֵ3-3", L"����3��������Ϣ");
	p3->SetPassword(true);
}

void RenderForm::CheckPropertyGridResult()
{
	ui::PropertyGrid* pPropertyGrid = dynamic_cast<ui::PropertyGrid*>(FindControl(L"property_grid_test"));
	if (pPropertyGrid == nullptr) {
		return;
	}

	struct PropertyData
	{
		std::wstring name;
		std::wstring oldValue;
		std::wstring newValue;
	};
	typedef std::vector<PropertyData> PropertyDataList;
	std::map<std::wstring, PropertyDataList> propertyMap;

	std::vector<ui::PropertyGridProperty*> properties;
	std::vector<ui::PropertyGridGroup*> groups;
	pPropertyGrid->GetGroups(groups);
	for (auto pGroup : groups) {
		if (pGroup != nullptr) {
			PropertyDataList& dataList = propertyMap[pGroup->GetGroupName()];
			pGroup->GetProperties(properties);
			for (auto pProperty : properties) {
				if (pProperty != nullptr) {
					PropertyData data;
					data.name = pProperty->GetPropertyName();
					data.oldValue = pProperty->GetPropertyValue();
					data.newValue = pProperty->GetPropertyNewValue();
					dataList.push_back(data);
				}
			}
		}
	}
	propertyMap.clear();
}