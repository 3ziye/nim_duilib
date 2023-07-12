#pragma once

// #include "VirtualTileBox.h"
#include <map>

struct DownloadTask
{
	int nId;
	std::wstring sName;
};

class Provider : public ui::VirtualTileBoxElement
{
public:
	Provider();
	~Provider();

	virtual ui::Control* CreateElement() override;

	/**
	* @brief ���ָ������
	* @param[in] control ����ؼ�ָ��
	* @param[in] index ����
	* @return ���ش����������ָ��
	*/
	virtual void FillElement(ui::Control *control, size_t index) override;

	/**
	* @brief ��ȡ��������
	* @return ������������
	*/
	virtual size_t GetElementCount() override;

public:
	void SetTotal(int nTotal);
	void RemoveTask(size_t nIndex);
	void ChangeTaskName(size_t nIndex, const std::wstring& sName);

private:
	int m_nTotal;
	std::vector<DownloadTask> m_vTasks;
	nbase::NLock  lock_;
};

