#ifndef UI_CONTROL_TREEVIEW_H_
#define UI_CONTROL_TREEVIEW_H_

#pragma once

#include "duilib/Box/ListBox.h"

namespace ui
{

#define ROOT_NODE_DEPTH  -1

class TreeView;
class UILIB_API TreeNode : public ListBoxElement
{
public:
	TreeNode();
	TreeNode(const TreeNode& r) = delete;
	TreeNode& operator=(const TreeNode& r) = delete;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual bool IsVisible() const override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;

	/**
	 * @brief ������ʱ����
	 * @param[in] param ��Ϣ��
	 * @return ʼ�շ��� true
	 */
	virtual bool OnClickItem(const EventArgs& param);

	/**
	 * @brief �������������������б�����
	 * @param[in] pTreeView ����ָ��
	 * @return ��
	 */
	void SetTreeView(TreeView* pTreeView);

	/**
	 * @brief ��ȡ���ڵ�ָ��
	 * @return ���ظ��ڵ�ָ��
	 */
	TreeNode* GetParentNode();

	/**
	 * @brief ���ø��ڵ�
	 * @param[in] pParentTreeNode ���ڵ�ָ��
	 * @return ��
	 */
	void SetParentNode(TreeNode* pParentTreeNode);

	/**
	 * @brief ����ӽڵ�
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool AddChildNode(TreeNode* pTreeNode);

	/**
	 * @brief ��ָ��λ������ӽڵ�
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @param[in] iIndex Ҫ�����ĸ�λ��֮��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool AddChildNodeAt(TreeNode* pTreeNode, size_t iIndex);

	/**
	 * @brief ��ָ��λ���Ƴ�һ���ӽڵ�
	 * @param[in] index Ҫ�Ƴ����ӽڵ�����
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool RemoveChildNodeAt(size_t index);

	/**
	 * @brief �����ӽڵ�ָ���Ƴ�һ���ӽڵ�
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool RemoveChildNode(TreeNode* pTreeNode);

	/**
	 * @brief �Ƴ������ӽڵ�
	 * @return ��
	 */
	void RemoveAllChildNode();

	/**
	 * @brief �ݹ��ȡ�����ӽڵ����
	 * @return ���������ӽڵ����
	 */
	int GetDescendantNodeCount();

	/**
	 * @brief ��ȡ��һ���ӽڵ����
	 * @return �����ӽڵ����
	 */
	size_t GetChildNodeCount();

	/**
	 * @brief ��ȡһ���ӽڵ�ָ��
	 * @param[in] iIndex Ҫ��ȡ���ӽڵ�����
	 * @return �����ӽڵ�ָ��
	 */
	TreeNode* GetChildNode(size_t iIndex);

	/**
	 * @brief �����ӽڵ�ָ���ȡ�ӽڵ�λ������
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @return ����λ������
	 */
	int	GetChildNodeIndex(TreeNode* pTreeNode);

	/**
	 * @brief �ж��Ƿ�չ��״̬
	 * @return ���� true Ϊչ��״̬������Ϊ false
	 */
	bool IsExpand() const; 

	/**
	 * @brief �����Ƿ�չ����ʾ�ӽڵ�
	 * @param[in] bExpand Ϊ true ʱչ����Ϊ false �ǲ�չ��
	 * @return ��
	 */
	void SetExpand(bool bExpand, bool bTriggerEvent = false);

	/**
	 * @brief ��ȡ����㼶
	 * @return ���ص�ǰ�㼶
	 */
	int GetDepth() const;

	/**
	 * @brief ��������չ���¼�
	 * @param[in] callback ����չ��ʱ�����Ļص�����
	 * @return ��
	 */
	void AttachExpand(const EventCallback& callback) { AttachEvent(kEventExpand, callback); }

	/**
	 * @brief �������������¼�
	 * @param[in] callback ��������ʱ�����Ļص�����
	 * @return ��
	 */
	void AttachUnExpand(const EventCallback& callback) { AttachEvent(kEventUnExpand, callback); }

private:
	/**
	 * @brief ɾ������
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool RemoveSelf();

private:
	int m_iDepth;
	bool m_bExpand;
	TreeView *m_pTreeView;
	TreeNode *m_pParentTreeNode;
	std::vector<TreeNode*> m_aTreeNodes;
};

class UILIB_API TreeView : public ListBox
{
public:
	TreeView(void);

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief ��ȡ���ڵ�
	 * @return ���ظ��ڵ�ָ��
	 */
	TreeNode* GetRootNode()	{ return m_rootNode.get(); }

	/**
	 * @brief ��ȡ�ӽڵ�����ֵ
	 * @return �����ӽڵ�����ֵ
	 */
	int GetIndent() { return m_iIndent;	}

	/**
	 * @brief �����ӽڵ�����ֵ
	 * @param[in] indent Ҫ���õ�����ֵ
	 * @return ��
	 */
	void SetIndent(int indent) { m_iIndent = indent; }

private:
	//���º�������˽�л���������ֹʹ�ã�Ӧ��ʹ��TreeNode�е���غ���
	bool AddItem(Control* pControl) override;
	bool AddItemAt(Control* pControl, size_t iIndex) override;
	bool RemoveItem(Control* pControl) override;
	bool RemoveItemAt(size_t iIndex) override;
	void RemoveAllItems() override;

	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;

private:
	int m_iIndent;
	std::unique_ptr<TreeNode> m_rootNode;
};

}

#endif // UI_CONTROL_TREEVIEW_H_
