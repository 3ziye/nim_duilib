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

	/** ���˫��ʱ����
	 * @param[in] args ��Ϣ��
	 * @return ʼ�շ��� true
	 */
	virtual bool OnDoubleClickItem(const EventArgs& args);

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
	int32_t GetDepth() const;

	/** ����ͼƬ/����Ԫ��֮��Ĺ̶����
	*/
	void SetExtraPadding(int32_t extraPadding);

#ifdef UILIB_IMPL_WINSDK

	/** ���ñ���ͼƬ(HICON���)
	 * @param [in] hIcon Ҫ���õ�ͼ���������ú���ui::GlobalManager::Instance().Icon()������Դ����������
	               ���hIconΪnullptr, ��ɾ���ڵ��ͼ�꣬�������ui::GlobalManager::Instance().Icon()�Ƴ�ԭ��������ͼ����
	 */
	void SetBkIcon(HICON hIcon);

#endif

	/** ����CheckBox������Class�������Ϊ�ձ�ʾ����CheckBox���ܣ�Ϊ����ر�CheckBox����
	*   Ӧ�÷�Χ���ýڵ㱾��
	* @param [in] checkBoxClass ui::CheckBox��Class���ԣ�һ�����õ���Ҫ�����У�
				  normal_image������״̬��ͼƬ����ѡ����(������ʱ��ͼƬ)
				  selected_normal_image��ѡ��ʱ������״̬��ͼƬ����ѡ����(����ʱ��ͼƬ)
	*/
	void SetCheckBoxClass(const std::wstring& checkBoxClass);

	/** �����Ƿ���ʾͼ��
	*/
	void SetEnableIcon(bool bEnable);

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

	/** ���ݵ�ǰ�����ã�����CheckBox�������ڱ߾�(�����뺯������ε����޸�����)
	*/
	void AdjustCheckBoxPadding();

	/** ���ݵ�ǰ�����ã�����ͼ��������ڱ߾�(�����뺯������ε����޸�����)
	*/
	void AdjustIconPadding();

private:
	//����㼶
	int32_t m_iDepth;

	//�Ƿ�չ����ʾ�ӽڵ�
	bool m_bExpand;

	//����������������
	TreeView* m_pTreeView;

	//���ڵ�
	TreeNode *m_pParentTreeNode;

	//�ӽڵ��б�
	std::vector<TreeNode*> m_aTreeNodes;

	//ͼƬ/����Ԫ��֮��Ĺ̶����
	uint8_t m_extraPadding;

	//CheckBox������ͼ���ڱ߾�
	uint16_t m_checkBoxIconPadding;

	//CheckBox�����������ڱ߾�
	uint16_t m_checkBoxTextPadding;

	//ͼ������������ڱ߾�
	uint16_t m_iconTextPadding;
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
	 * @param [in] indent Ҫ���õ�����ֵ, ��λΪ����
	 * @param [in] bNeedDpiScale �Ƿ���ҪDPI����
	 */
	void SetIndent(int32_t indent, bool bNeedDpiScale);

	/** ����CheckBox������Class�������Ϊ�ձ�ʾ����CheckBox���ܣ�Ϊ����ر�CheckBox����
	*   Ӧ�÷�Χ�����������нڵ�
	* @param [in] className ui::CheckBox��Class���ԣ�һ�����õ���Ҫ�����У�
				  normal_image������״̬��ͼƬ����ѡ����(������ʱ��ͼƬ)
				  selected_normal_image��ѡ��ʱ������״̬��ͼƬ����ѡ����(����ʱ��ͼƬ)
	*/
	void SetCheckBoxClass(const std::wstring& className);

	/** ��ȡCheckBox������Class
	*/
	std::wstring GetCheckBoxClass() const;

	/** �����Ƿ���ʾͼ��
	*/
	void SetEnableIcon(bool bEnable);

	/** �ж��Ƿ���ʾͼ��
	*/
	bool IsEnableIcon() const;

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

	//CheckBox��Class
	UiString m_checkBoxClass;

	//�Ƿ���ʾͼ��
	bool m_bEnableIcon;

	//���ĸ��ڵ�
	std::unique_ptr<TreeNode> m_rootNode;
};

}

#endif // UI_CONTROL_TREEVIEW_H_
