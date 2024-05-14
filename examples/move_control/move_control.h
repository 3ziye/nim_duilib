#pragma once
#include "app_db.h"

enum ThreadId
{
	kThreadUI
};

class MoveControlForm : public ui::WindowImplBase
{
public:
	MoveControlForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);
	~MoveControlForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/** �����ڴ�������Ժ���ô˺���������������һЩ��ʼ���Ĺ���
	*/
	virtual void OnInitWindow() override;

	/** �����ڼ������ر�ʱ���ô˺���������������һЩ��β����
	*/
	virtual void OnCloseWindow() override;

private:
	std::wstring m_class_name;
	std::wstring m_theme_directory;
	std::wstring m_layout_xml;

	ui::Box* m_frequent_app=nullptr;
	ui::Box* m_my_app = nullptr;
};

