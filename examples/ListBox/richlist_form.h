#pragma once

class RichlistForm : public ui::WindowImplBase
{
public:
	RichlistForm();
	~RichlistForm();

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

	/** ��������
	*/
	static const std::wstring kClassName;

private:
	bool OnSelected(const ui::EventArgs& args);

private:
	ui::ListBox* m_pListBox;
};

