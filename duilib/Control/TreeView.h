#ifndef UI_CONTROL_TREEVIEW_H_
#define UI_CONTROL_TREEVIEW_H_

#pragma once

#include "duilib/Box/ListBox.h"

namespace ui
{

#define ROOT_NODE_DEPTH  -1

class TreeView;
class UILIB_API TreeNode : public ListBoxItem
{
public:
	TreeNode();
	TreeNode(const TreeNode& r) = delete;
	TreeNode& operator=(const TreeNode& r) = delete;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual bool IsVisible() const override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetWindow(Window* pManager) override;

	/** ������ʱ����
	 * @param[in] param ��Ϣ��
	 * @return ʼ�շ��� true
	 */
	virtual bool OnClickItem(const EventArgs& param);

	/** ��������������������
	 * @param[in] pTreeView ����ָ��
	 */
	void SetTreeView(TreeView* pTreeView);

	/** ��ȡ���ڵ�ָ��
	 * @return ���ظ��ڵ�ָ��
	 */
	TreeNode* GetParentNode();

	/** ���ø��ڵ�
	 * @param[in] pParentTreeNode ���ڵ�ָ��
	 */
	void SetParentNode(TreeNode* pParentTreeNode);

	/** ����ӽڵ�
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool AddChildNode(TreeNode* pTreeNode);

	/** ��ָ��λ������ӽڵ�
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @param[in] iIndex Ҫ�����ĸ�λ��֮��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool AddChildNodeAt(TreeNode* pTreeNode, size_t iIndex);

	/** ��ָ��λ���Ƴ�һ���ӽڵ�
	 * @param[in] iIndex Ҫ�Ƴ����ӽڵ�����
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool RemoveChildNodeAt(size_t iIndex);

	/** �����ӽڵ�ָ���Ƴ�һ���ӽڵ�
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool RemoveChildNode(TreeNode* pTreeNode);

	/** �Ƴ������ӽڵ�
	 */
	void RemoveAllChildNode();

	/** �ݹ��ȡ�����ӽڵ����
	 * @return ���������ӽڵ����
	 */
	size_t GetDescendantNodeCount();

	/** ��ȡ��һ���ӽڵ����
	 * @return �����ӽڵ����
	 */
	size_t GetChildNodeCount();

	/** ��ȡһ���ӽڵ�ָ��
	 * @param[in] iIndex Ҫ��ȡ���ӽڵ�����
	 * @return �����ӽڵ�ָ��
	 */
	TreeNode* GetChildNode(size_t iIndex);

	/** �����ӽڵ�ָ���ȡ�ӽڵ�λ������
	 * @param[in] pTreeNode �ӽڵ�ָ��
	 * @return ����λ������
	 */
	size_t GetChildNodeIndex(TreeNode* pTreeNode);

	/** �ж��Ƿ�չ��״̬
	 * @return ���� true Ϊչ��״̬������Ϊ false
	 */
	bool IsExpand() const; 

	/** �����Ƿ�չ����ʾ�ӽڵ�
	 * @param[in] bExpand Ϊ true ʱչ����Ϊ false �ǲ�չ��
	 */
	void SetExpand(bool bExpand, bool bTriggerEvent = false);

	/** ��ȡ����㼶
	 * @return ���ص�ǰ�㼶
	 */
	int GetDepth() const;

	/** ��������չ���¼�
	 * @param[in] callback ����չ��ʱ�����Ļص�����
	 */
	void AttachExpand(const EventCallback& callback) { AttachEvent(kEventExpand, callback); }

	/** �������������¼�
	 * @param[in] callback ��������ʱ�����Ļص�����
	 */
	void AttachUnExpand(const EventCallback& callback) { AttachEvent(kEventUnExpand, callback); }

private:
	/** ɾ������
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool RemoveSelf();

private:
	//����㼶
	int m_iDepth;
	//�Ƿ�չ����ʾ�ӽڵ�
	bool m_bExpand;
	//����������������
	TreeView* m_pTreeView;
	//���ڵ�
	TreeNode *m_pParentTreeNode;
	//�ӽڵ��б�
	std::vector<TreeNode*> m_aTreeNodes;
};

class UILIB_API TreeView : public ListBox
{
public:
	TreeView(void);

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/** ��ȡ���ڵ�
	 * @return ���ظ��ڵ�ָ��
	 */
	TreeNode* GetRootNode()	const{ return m_rootNode.get(); }

	/** ��ȡ�ӽڵ�����ֵ
	 * @return �����ӽڵ�����ֵ
	 */
	int32_t GetIndent() const { return m_iIndent;	}

	/** �����ӽڵ�����ֵ
	 * @param[in] indent Ҫ���õ�����ֵ, ��λΪ����
	 */
	void SetIndent(int32_t indent);

private:
	//���º�������˽�л���������ֹʹ�ã�Ӧ��ʹ��TreeNode�е���غ���
	bool AddItem(Control* pControl) override;
	bool AddItemAt(Control* pControl, size_t iIndex) override;
	bool RemoveItem(Control* pControl) override;
	bool RemoveItemAt(size_t iIndex) override;
	void RemoveAllItems() override;

	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetWindow(Window* pManager) override;

private:
	//�ӽڵ������ֵ����λΪ����
	int32_t m_iIndent;

	//���ĸ��ڵ�
	std::unique_ptr<TreeNode> m_rootNode;
};

}

#endif // UI_CONTROL_TREEVIEW_H_
