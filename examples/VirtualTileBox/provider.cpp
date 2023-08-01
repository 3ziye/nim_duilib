#include "stdafx.h"
#include "provider.h"
#include "item.h"


int g_index = 1;

Provider::Provider()
:m_nTotal(0)
{

}


Provider::~Provider()
= default;

ui::Control* Provider::CreateElement()
{
	Item* item = new Item;
	ui::GlobalManager::Instance().FillBoxWithCache(item, L"virtual_tile_box/item.xml");
	return item;
}

void Provider::FillElement(ui::Control *control, size_t index)
{
	Item* pItem = dynamic_cast<Item*>(control);

	
	std::wstring img = L"icon.png";
	std::wstring title = nbase::StringPrintf(L"%s [%02d]", m_vTasks[index].sName, m_vTasks[index].nId);
	pItem->InitSubControls(img, title, index);
}

size_t Provider::GetElementCount()
{
	// ����
	nbase::NAutoLock auto_lock(&lock_);
	return m_vTasks.size();
}

void Provider::SetTotal(int nTotal)
{
	if (nTotal == m_nTotal) return;
	if (nTotal <= 0) return;
	m_nTotal = nTotal;

	// ����
	lock_.Lock();
	for (auto task : m_vTasks) {
		delete [] task.sName;
	}
	m_vTasks.clear();
	std::wstring name = L"��������";
	m_vTasks.reserve(nTotal);
	for (auto i=1; i <= nTotal; i++)
	{
		DownloadTask task;
		task.nId = i;
		//������std::wstring����Ϊ��ռ�õ��ڴ�ܶ࣬���������ﵽǧ�򼶱�����ʱ��ռ���ڴ�̫��
		task.sName = new wchar_t[name.size() + 1];
		wcscpy_s(task.sName, name.size() + 1, name.c_str());
		m_vTasks.emplace_back(std::move(task));
	}
	lock_.Unlock();

	// ֪ͨTileBox���������䶯
	EmitCountChanged();
}

void Provider::RemoveTask(size_t nIndex)
{	
	lock_.Lock();
	if (nIndex < m_vTasks.size()) {
		auto iter = m_vTasks.begin() + nIndex;
		m_vTasks.erase(iter);
	}
	lock_.Unlock();

	// ֪ͨTileBox���������䶯
	EmitCountChanged();
}

void Provider::ChangeTaskName(size_t nIndex, const std::wstring& sName)
{
	lock_.Lock();
	if (nIndex < m_vTasks.size()) {
		delete m_vTasks[nIndex].sName;
		m_vTasks[nIndex].sName = new wchar_t[sName.size() + 1];
		wcscpy_s(m_vTasks[nIndex].sName, sName.size() + 1, sName.c_str());
	}
	lock_.Unlock();

	// �������ݱ䶯֪ͨ
	EmitDataChanged(nIndex, nIndex);
}
