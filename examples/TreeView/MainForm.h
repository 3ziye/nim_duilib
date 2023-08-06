#pragma once

#include "resource.h"

// base header
#include "base/base.h"

// duilib
#include "duilib/duilib.h"

#include "FileInfoList.h"

class MainForm : public ui::WindowImplBase
{
public:
	MainForm();
	~MainForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void OnInitWindow() override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

	static const std::wstring kClassName;

private:
	//��ʾ����ڵ�
	void ShowDesktopNode();

	//��ʾ���̽ڵ�
	void ShowAllDiskNode();

	//��ʾָ��Ŀ¼����Ŀ¼
	void ShowSubFolders(ui::TreeNode* pTreeNode, const std::wstring& path);

	//�����в���һ���ڵ�
	void InsertTreeNode(ui::TreeNode* pTreeNode, 
		                const std::wstring& displayName,
		                const std::wstring& path,
		                bool isFolder,
		                HICON hIcon);

	//��ʾָ��Ŀ¼������
	void ShowFolderContents(const std::wstring& path);

	/** ���ڵ�չ���¼�
	 * @param[in] args ��Ϣ��
	 * @return ʼ�շ��� true
	 */
	bool OnTreeNodeExpand(const ui::EventArgs& args);

	/** ���ڵ����¼�
	 * @param[in] args ��Ϣ��
	 * @return ʼ�շ��� true
	 */
	bool OnTreeNodeClick(const ui::EventArgs& args);

private:
	//���ڵ�Ľӿ�
	ui::TreeView* m_pTree;

	//Ŀ¼�б��������ʾ��
	struct FolderStatus
	{
		std::wstring path;
		bool bShow = false;
		HICON hIcon = nullptr;
	};
	std::vector<FolderStatus*> m_folderList;

	/** �ļ��б��Ҳ������ʾ��
	*/
	FileInfoList m_fileList;

	//�ļ��б�Ľӿ�
	ui::VirtualListBox* m_pListBox;
};


