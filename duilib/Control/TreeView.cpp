#include "TreeView.h"
#include "duilib/Core/ScrollBar.h"

namespace ui
{

TreeNode::TreeNode() :
	m_bExpand(true),
	m_pTreeView(nullptr),
	m_pParentTreeNode(nullptr),
	m_iDepth(ROOT_NODE_DEPTH),
	m_extraPadding(3),
	m_checkBoxIconPadding(0),
	m_checkBoxTextPadding(0),
	m_iconTextPadding(0)
{	
}

std::wstring TreeNode::GetType() const { return DUI_CTR_TREENODE; }

void TreeNode::SetTreeView(TreeView* pTreeView)
{
    m_pTreeView = pTreeView;
}

bool TreeNode::OnDoubleClickItem(const EventArgs& args)
{
    TreeNode* pItem = dynamic_cast<TreeNode*>(args.pSender);
	ASSERT(pItem != nullptr);
	if (pItem != nullptr) {
		pItem->SetExpand(!pItem->IsExpand(), true);
	}
    return true;
}

bool TreeNode::IsVisible() const
{
	if (!ListBoxItem::IsVisible()) {
		return false;
	}
	if (m_pParentTreeNode != nullptr) {
		//������ڵ�δչ�������߸��ڵ㲻�ɼ�������ӽڵ�Ҳ���ɼ�
		if (!m_pParentTreeNode->IsExpand() || !m_pParentTreeNode->IsVisible()) {
			return false;
		}
	}
	return true;
}

void TreeNode::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	ListBoxItem::SetWindow(pManager, pParent, bInit);
}

void TreeNode::SetWindow(Window* pManager)
{
	__super::SetWindow(pManager);
}

TreeNode* TreeNode::GetParentNode()
{
	return m_pParentTreeNode;
}

void TreeNode::SetParentNode(TreeNode* pParentTreeNode)
{
	m_pParentTreeNode = pParentTreeNode;
}

bool TreeNode::AddChildNode(TreeNode* pTreeNode)
{
	return AddChildNodeAt(pTreeNode, GetChildNodeCount());
}

bool TreeNode::AddChildNodeAt(TreeNode* pTreeNode, size_t iIndex)
{
	ASSERT(pTreeNode != nullptr);
	if (pTreeNode == nullptr) {
		return false;
	}
	ASSERT(m_pTreeView != nullptr);
	if (m_pTreeView == nullptr) {
		return false;
	}
	ASSERT(iIndex <= m_aTreeNodes.size());
	if (iIndex > m_aTreeNodes.size()) {
		return false;
	}
	m_aTreeNodes.insert(m_aTreeNodes.begin() + iIndex, pTreeNode);
		
	pTreeNode->m_iDepth = m_iDepth + 1;
	pTreeNode->SetParentNode(this);
	pTreeNode->SetTreeView(m_pTreeView);
	if (GetWindow() != nullptr) {
		GetWindow()->InitControls(pTreeNode, nullptr);
	}
	pTreeNode->AttachEvent(kEventMouseDoubleClick, nbase::Bind(&TreeNode::OnDoubleClickItem, this, std::placeholders::_1));

	UiPadding padding = GetPadding();
	size_t nodeIndex = 0;
	if (m_iDepth != ROOT_NODE_DEPTH) {
		nodeIndex = GetListBoxIndex() + 1;
		padding.left += m_pTreeView->GetIndent();
	}
	pTreeNode->SetPadding(padding, false);

	size_t nGlobalIndex = iIndex;
	for (size_t i = 0; i < iIndex; i++)	{
		nGlobalIndex += ((TreeNode*)m_aTreeNodes[i])->GetDescendantNodeCount();
	}

	//CheckBoxѡ��
	std::wstring checkBoxClass = m_pTreeView->GetCheckBoxClass();
	pTreeNode->SetCheckBoxClass(checkBoxClass);

	//�Ƿ���ʾͼ��
	pTreeNode->SetEnableIcon(m_pTreeView->IsEnableIcon());

	//��ӵ�ListBox������
	return m_pTreeView->ListBox::AddItemAt(pTreeNode, nodeIndex + nGlobalIndex);
}

#ifdef UILIB_IMPL_WINSDK

void TreeNode::SetBkIcon(HICON hIcon)
{
	GlobalManager::Instance().Icon().AddIcon(hIcon);
	std::wstring iconString = GlobalManager::Instance().Icon().GetIconString(hIcon);
	std::wstring oldIconString = GetBkImagePath();
	if (iconString == oldIconString) {
		//û�б仯��ֱ�ӷ���
		return;
	}
	//�ɵ�ͼ���С
	HICON hOldIcon = GlobalManager::Instance().Icon().GetIcon(oldIconString);
	UiSize oldIconSize = GlobalManager::Instance().Icon().GetIconSize(hOldIcon);

	//�µ�ͼ���С
	UiSize newIconSize = GlobalManager::Instance().Icon().GetIconSize(hIcon);

	bool bAdjustIconPadding = true;
	if (!oldIconString.empty()) {
		if (newIconSize.cx != oldIconSize.cx) {
			//��ͼ����ڣ�����ͼ���С��ͬ���������ԭ����ͼ��
			SetBkImage(L"");
			AdjustIconPadding();
		}
		else {
			//�¾�ͼ���С��ͬ������Ҫ����ͼ���ڱ߾�
			bAdjustIconPadding = false;
		}
	}

	if (!iconString.empty()) {
		iconString = StringHelper::Printf(L"file='%s' halign='left' valign='center'", iconString.c_str());
		SetBkImage(iconString);
	}
	else {
		SetBkImage(L"");
	}

	if (bAdjustIconPadding) {
		AdjustIconPadding();
	}

	//�����޸��Ƿ���ʾͼ���־
	if (m_pTreeView != nullptr) {
		SetEnableIcon(m_pTreeView->IsEnableIcon());
	}
}

#endif //UILIB_IMPL_WINSDK

void TreeNode::SetCheckBoxClass(const std::wstring& checkBoxClass)
{
	if (!checkBoxClass.empty()) {
		//����CheckBox����
		SetClass(checkBoxClass);
	}
	else {
		//�ر�CheckBox����
		ClearStateImages();
	}
	AdjustCheckBoxPadding();
}

void TreeNode::AdjustCheckBoxPadding()
{
	if (HasStateImage(kStateImageBk)) {
		//��ʾCheckBox
		const int32_t extraPadding = ui::GlobalManager::Instance().Dpi().GetScaleInt(m_extraPadding);
		UiSize imageSize = GetStateImageSize(kStateImageBk, kControlStateNormal);
		uint16_t checkBoxPadding = TruncateToUInt16(imageSize.cx);
		if (checkBoxPadding > 0) {
			checkBoxPadding += (uint16_t)extraPadding;
		}
		
		if ((checkBoxPadding > 0) && (m_checkBoxIconPadding == 0)){
			//��CheckBox״̬ͼƬ, ��Ҫ���ñ���ͼƬ���ڱ߾࣬��������ͼƬ�ص�
			UiPadding rcBkPadding = GetBkImagePadding();
			rcBkPadding.left += checkBoxPadding;
			if (SetBkImagePadding(rcBkPadding, false)) {
				m_checkBoxIconPadding = checkBoxPadding;
			}			
		}

		if ((checkBoxPadding > 0) && (m_checkBoxTextPadding == 0)) {
			//�������ֵ��ڱ߾�
			UiPadding rcTextPadding = GetTextPadding();
			rcTextPadding.left += checkBoxPadding;
			SetTextPadding(rcTextPadding, false);
			m_checkBoxTextPadding = checkBoxPadding;
		}
	}
	else {
		//����CheckBox
		if (m_checkBoxIconPadding > 0) {
			UiPadding rcBkPadding = GetBkImagePadding();
			rcBkPadding.left -= (int32_t)m_checkBoxIconPadding;
			if (rcBkPadding.left >= 0) {
				SetBkImagePadding(rcBkPadding, false);
			}
			m_checkBoxIconPadding = 0;
		}
		if (m_checkBoxTextPadding > 0) {
			UiPadding rcTextPadding = GetTextPadding();
			rcTextPadding.left -= (int32_t)m_checkBoxTextPadding;
			if (rcTextPadding.left >= 0) {
				SetTextPadding(rcTextPadding, false);
			}
			m_checkBoxTextPadding = 0;
		}
	}
}

void TreeNode::AdjustIconPadding()
{
	std::wstring iconString = GetBkImagePath();
	if (!IsBkImagePaintEnabled()) {
		//��ֹ����ͼ��
		iconString.clear();
	}
	if (!iconString.empty()) {
		//��ʾͼ��
		if (m_iconTextPadding == 0) {
			const int32_t extraPadding = ui::GlobalManager::Instance().Dpi().GetScaleInt(m_extraPadding);
			UiSize imageSize = GetBkImageSize();
			uint16_t iconTextPadding = TruncateToUInt16(imageSize.cx);
			if (iconTextPadding > 0) {
				iconTextPadding += (uint16_t)extraPadding;
			}
			if (iconTextPadding > 0) {
				//�������ֵ��ڱ߾�
				UiPadding rcTextPadding = GetTextPadding();
				rcTextPadding.left += iconTextPadding;
				SetTextPadding(rcTextPadding, false);
				m_iconTextPadding = iconTextPadding;
			}
		}
	}
	else {
		//����ͼ��
		if (m_iconTextPadding > 0) {
			UiPadding rcTextPadding = GetTextPadding();
			rcTextPadding.left -= (int32_t)m_iconTextPadding;
			if (rcTextPadding.left >= 0) {
				SetTextPadding(rcTextPadding, false);
			}
			m_iconTextPadding = 0;
		}
	}
	AdjustCheckBoxPadding();
}

void TreeNode::SetEnableIcon(bool bEnable)
{
	if (IsBkImagePaintEnabled() != bEnable) {
		SetBkImagePaintEnabled(bEnable);
		AdjustIconPadding();
	}
}

bool TreeNode::RemoveChildNodeAt(size_t iIndex)
{
	ASSERT(iIndex < m_aTreeNodes.size());
	if (iIndex >= m_aTreeNodes.size()) {
		return false;
	}

	TreeNode* pTreeNode = ((TreeNode*)m_aTreeNodes[iIndex]);
	m_aTreeNodes.erase(m_aTreeNodes.begin() + iIndex);
	if (pTreeNode) {
		return pTreeNode->RemoveSelf();
	}
	return false;
}

bool TreeNode::RemoveChildNode(TreeNode* pTreeNode)
{
	auto it = std::find(m_aTreeNodes.begin(), m_aTreeNodes.end(), pTreeNode);
	if (it == m_aTreeNodes.end()) {
		return false;
	}		
	size_t iIndex = it - m_aTreeNodes.begin();
	return RemoveChildNodeAt(iIndex);
}
	
void TreeNode::RemoveAllChildNode()
{
	while (m_aTreeNodes.size() > 0) {
		RemoveChildNodeAt(0);
	}
}

bool TreeNode::RemoveSelf()
{
	for(TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode != nullptr) {
			pTreeNode->RemoveSelf();
		}
	}
	m_aTreeNodes.clear();
	if (m_iDepth != ROOT_NODE_DEPTH) {
		return m_pTreeView->ListBox::RemoveItemAt(GetListBoxIndex());
	}
	return false;
}

size_t TreeNode::GetDescendantNodeCount()
{
	size_t nodeCount = GetChildNodeCount();
	for (TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode != nullptr) {
			nodeCount += pTreeNode->GetDescendantNodeCount();
		}
	}
	return nodeCount;
}

size_t TreeNode::GetChildNodeCount()
{
	return m_aTreeNodes.size();
}
	
TreeNode* TreeNode::GetChildNode(size_t iIndex)
{
	if (iIndex >= m_aTreeNodes.size()) {
		return nullptr;
	}
	return m_aTreeNodes[iIndex];
}
	
size_t TreeNode::GetChildNodeIndex(TreeNode* pTreeNode)
{
	auto it = std::find(m_aTreeNodes.begin(), m_aTreeNodes.end(), pTreeNode);
	if (it == m_aTreeNodes.end()) {
		return Box::InvalidIndex;
	}
	return it - m_aTreeNodes.begin();
}

bool TreeNode::IsExpand() const
{
	return m_bExpand;
}

void TreeNode::SetExpand(bool bExpand, bool bTriggerEvent)
{
	if(m_bExpand == bExpand) {
		return;
	}
	m_bExpand = bExpand;

	if (bTriggerEvent) {
		SendEvent(m_bExpand ? kEventExpand : kEventUnExpand);
	}
	if (m_pTreeView != nullptr) {
		m_pTreeView->Arrange();
	}	
}

int32_t TreeNode::GetDepth() const
{
	return m_iDepth;
}

void TreeNode::SetExtraPadding(int32_t extraPadding)
{
	m_extraPadding = TruncateToUInt8(extraPadding);
}

TreeView::TreeView() :
	ListBox(new VLayout),
	m_iIndent(0),
	m_rootNode(),
	m_bEnableIcon(true)
{
	m_rootNode.reset(new TreeNode());
	m_rootNode->SetTreeView(this);
	//����Ĭ������Ϊ20������
	SetIndent(20, true);
}

std::wstring TreeView::GetType() const { return DUI_CTR_TREEVIEW; }

void TreeView::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	//֧�ֵ������б�: ����ʵ�ֵ�ֱ��ת��
	if (strName == L"indent") {
		//���ڵ��������ÿ��ڵ�����һ��indent��λ��
		SetIndent(_wtoi(strValue.c_str()), true);
	}
	else if (strName == L"multi_select") {
		//��ѡ��Ĭ���ǵ�ѡ���ڻ���ʵ��
		ListBox::SetAttribute(strName, strValue);
	}
	else if (strName == L"check_box_class") {
		//�Ƿ���ʾCheckBox
		SetCheckBoxClass(strValue);
	}
	else if (strName == L"show_icon") {
		//�Ƿ���ʾͼ��
		SetEnableIcon(strValue == L"true");
	}
	else {
		ListBox::SetAttribute(strName, strValue);
	}
}

void TreeView::SetIndent(int32_t indent, bool bNeedDpiScale)
{
	ASSERT(indent >= 0);
	if (bNeedDpiScale) {
		GlobalManager::Instance().Dpi().ScaleInt(indent);
	}
	if (indent >= 0) {
		m_iIndent = indent;
	}	
}

void TreeView::SetCheckBoxClass(const std::wstring& className)
{
	bool isChanged = m_checkBoxClass != className;
	m_checkBoxClass = className;
	if (isChanged) {
		for (Control* pControl : m_items) {
			TreeNode* pTreeNode = dynamic_cast<TreeNode*>(pControl);
			if (pTreeNode != nullptr) {
				pTreeNode->SetCheckBoxClass(className);
			}
		}
	}
}

std::wstring TreeView::GetCheckBoxClass() const
{
	return m_checkBoxClass.c_str();
}

void TreeView::SetEnableIcon(bool bEnable)
{
	bool isChanged = m_bEnableIcon != bEnable;
	m_bEnableIcon = bEnable;
	if (isChanged) {
		for (Control* pControl : m_items) {
			TreeNode* pTreeNode = dynamic_cast<TreeNode*>(pControl);
			if (pTreeNode != nullptr) {
				pTreeNode->SetEnableIcon(bEnable);
			}
		}
	}	
}

bool TreeView::IsEnableIcon() const
{
	return m_bEnableIcon;
}

bool TreeView::AddItem(Control* /*pControl*/)
{
	ASSERT(FALSE);
	return false;
}

bool TreeView::AddItemAt(Control* /*pControl*/, size_t /*iIndex*/)
{
	ASSERT(FALSE);
	return false;
}

bool TreeView::RemoveItem(Control* /*pControl*/)
{
	ASSERT(FALSE);
	return false;
}

bool TreeView::RemoveItemAt(size_t /*iIndex*/)
{
	ASSERT(FALSE);
	return false;
}

void TreeView::RemoveAllItems()
{
	ASSERT(FALSE);
}

void TreeView::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	ListBox::SetWindow(pManager, pParent, bInit);
	m_rootNode->SetWindow(pManager, pParent, bInit);
}

void TreeView::SetWindow(Window* pManager)
{
	__super::SetWindow(pManager);
	m_rootNode->SetWindow(pManager);
}

}