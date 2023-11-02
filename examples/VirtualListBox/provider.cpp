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
	ui::GlobalManager::Instance().FillBoxWithCache(item, L"virtual_list_box/item.xml");
	return item;
}

bool Provider::FillElement(ui::Control* pControl, size_t nElementIndex)
{
	nbase::NAutoLock auto_lock(&lock_);
	Item* pItem = dynamic_cast<Item*>(pControl);
	ASSERT(pItem != nullptr);
	ASSERT(nElementIndex < m_vTasks.size());
	if ((pItem == nullptr) || (nElementIndex >= m_vTasks.size())) {
		return false;
	}
	const DownloadTask& task = m_vTasks[nElementIndex];
	std::wstring img = L"icon.png";
	std::wstring title = nbase::StringPrintf(L"%s [%02d]", task.sName, task.nId);
	pItem->InitSubControls(img, title, nElementIndex);
	return true;
}

size_t Provider::GetElementCount() const
{
	// ����
	nbase::NAutoLock auto_lock(&lock_);
	return m_vTasks.size();
}

void Provider::SetElementSelected(size_t nElementIndex, bool bSelected)
{
	nbase::NAutoLock auto_lock(&lock_);
	if (nElementIndex < m_vTasks.size()) {
		m_vTasks[nElementIndex].bSelected = bSelected;
	}
}

bool Provider::IsElementSelected(size_t nElementIndex) const
{
	nbase::NAutoLock auto_lock(&lock_);
	bool bSelected = false;
	if (nElementIndex < m_vTasks.size()) {
		bSelected = m_vTasks[nElementIndex].bSelected;
	}
	return bSelected;
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
	bool bUpdated = false;
	if (nIndex < m_vTasks.size()) {
		auto iter = m_vTasks.begin() + nIndex;
		m_vTasks.erase(iter);
		bUpdated = true;
	}
	lock_.Unlock();

	if (bUpdated) {
		// ֪ͨTileBox���������䶯
		EmitCountChanged();
	}	
}

void Provider::ChangeTaskName(size_t nIndex, const std::wstring& sName)
{
	lock_.Lock();
	bool bUpdated = false;
	if (nIndex < m_vTasks.size()) {
		delete m_vTasks[nIndex].sName;
		m_vTasks[nIndex].sName = new wchar_t[sName.size() + 1];
		wcscpy_s(m_vTasks[nIndex].sName, sName.size() + 1, sName.c_str());
		bUpdated = true;
	}
	lock_.Unlock();

	// �������ݱ䶯֪ͨ
	if (bUpdated) {
		EmitDataChanged(nIndex, nIndex);
	}	
}
