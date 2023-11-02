#pragma once

// #include "VirtualListBox.h"
#include <map>

struct DownloadTask
{
	int nId; //ΨһID
	bool bSelected = false;//ѡ��״̬
	wchar_t* sName = nullptr;//����
};

class Provider : public ui::VirtualListBoxElement
{
public:
	Provider();
	~Provider();

	virtual ui::Control* CreateElement() override;

	/** ���ָ��������
	* @param[in] pControl ������ؼ�ָ��
	* @param[in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
	*/
	virtual bool FillElement(ui::Control* pControl, size_t nElementIndex) override;

	/**
	* @brief ��ȡ��������
	* @return ������������
	*/
	virtual size_t GetElementCount() const override;

	/** ����ѡ��״̬
	* @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
	* @param [in] bSelected true��ʾѡ��״̬��false��ʾ��ѡ��״̬
	*/
	virtual void SetElementSelected(size_t nElementIndex, bool bSelected) override;

	/** ��ȡѡ��״̬
	* @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
	* @return true��ʾѡ��״̬��false��ʾ��ѡ��״̬
	*/
	virtual bool IsElementSelected(size_t nElementIndex) const override;

public:
	void SetTotal(int nTotal);
	void RemoveTask(size_t nIndex);
	void ChangeTaskName(size_t nIndex, const std::wstring& sName);

private:
	int m_nTotal;
	std::vector<DownloadTask> m_vTasks;
	mutable nbase::NLock  lock_;
};

