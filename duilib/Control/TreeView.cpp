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
	m_expandCheckBoxPadding(0),
	m_expandIconPadding(0),
	m_expandTextPadding(0),
	m_checkBoxIconPadding(0),
	m_checkBoxTextPadding(0),
	m_iconTextPadding(0),
	m_pExpandImageRect(nullptr),
	m_pUnExpandImageRect(nullptr)
{
}

TreeNode::~TreeNode()
{
	if (m_pExpandImageRect != nullptr) {
		delete m_pExpandImageRect;
		m_pExpandImageRect = nullptr;
	}
	if (m_pUnExpandImageRect != nullptr) {
		delete m_pUnExpandImageRect;
		m_pUnExpandImageRect = nullptr;
	}
}

std::wstring TreeNode::GetType() const { return DUI_CTR_TREENODE; }

void TreeNode::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == L"expand_normal_image") {
		SetExpandStateImage(kControlStateNormal, strValue);
	}
	else if (strName == L"expand_hot_image") {
		SetExpandStateImage(kControlStateHot, strValue);
	}
	else if (strName == L"expand_pushed_image") {
		SetExpandStateImage(kControlStatePushed, strValue);
	}
	else if (strName == L"expand_disabled_image") {
		SetExpandStateImage(kControlStateDisabled, strValue);
	}
	else if (strName == L"unexpand_normal_image") {
		SetUnExpandStateImage(kControlStateNormal, strValue);
	}
	else if (strName == L"unexpand_hot_image") {
		SetUnExpandStateImage(kControlStateHot, strValue);
	}
	else if (strName == L"unexpand_pushed_image") {
		SetUnExpandStateImage(kControlStatePushed, strValue);
	}
	else if (strName == L"unexpand_disabled_image") {
		SetUnExpandStateImage(kControlStateDisabled, strValue);
	}
	else {
		__super::SetAttribute(strName, strValue);
	}
}

std::wstring TreeNode::GetExpandStateImage(ControlStateType stateType)
{
	Image* pImage = nullptr;
	if (m_expandImage != nullptr) {
		pImage = m_expandImage->GetStateImage(stateType);
	}
	if (pImage != nullptr) {
		return pImage->GetImageString();
	}
	return std::wstring();
}

void TreeNode::SetExpandStateImage(ControlStateType stateType, const std::wstring& strImage)
{
	if (m_expandImage == nullptr) {
		m_expandImage.reset(new StateImage);
		m_expandImage->SetControl(this);
	}
	m_expandImage->SetImageString(stateType, strImage);
}

std::wstring TreeNode::GetUnExpandStateImage(ControlStateType stateType)
{
	Image* pImage = nullptr;
	if (m_unexpandImage != nullptr) {
		pImage = m_unexpandImage->GetStateImage(stateType);
	}
	if (pImage != nullptr) {
		return pImage->GetImageString();
	}
	return std::wstring();
}

void TreeNode::SetUnExpandStateImage(ControlStateType stateType, const std::wstring& strImage)
{
	if (m_unexpandImage == nullptr) {
		m_unexpandImage.reset(new StateImage);
		m_unexpandImage->SetControl(this);
	}
	m_unexpandImage->SetImageString(stateType, strImage);
}

void TreeNode::PaintStateImages(IRender* pRender)
{
	__super::PaintStateImages(pRender);	
	if (IsExpand()) {
		//����չ��״̬ͼ�꣬���û���ӽڵ㣬����ֻ���ͼ��
		if ((m_expandImage != nullptr) && !m_aTreeNodes.empty()){
			if (m_pExpandImageRect == nullptr) {
				m_pExpandImageRect = new UiRect;
			}
			m_expandImage->PaintStateImage(pRender, GetState(), L"", m_pExpandImageRect);
		}
	}
	else {
		//����δչ��״̬ͼ��
		if (m_unexpandImage != nullptr) {
			if (m_pUnExpandImageRect == nullptr) {
				m_pUnExpandImageRect = new UiRect;
			}
			m_unexpandImage->PaintStateImage(pRender, GetState(), L"", m_pUnExpandImageRect);
		}
	}
}

bool TreeNode::ButtonDown(const EventArgs& msg)
{
	bool bRet = __super::ButtonDown(msg);
	if (!IsEnabled()) {
		return bRet;
	}
	UiRect pos = GetPos();
	if (!pos.ContainsPt(msg.ptMouse)) {
		return bRet;
	}
	if (IsExpand()) {
		//չ��״̬
		if ((m_expandImage != nullptr) && !m_aTreeNodes.empty()) {
			//��������չ��ͼ���ϣ�������
			if ((m_pExpandImageRect != nullptr) && 
				m_pExpandImageRect->ContainsPt(msg.ptMouse)) {
				SetExpand(false, true);
			}
		}
	}
	else {
		//δչ��״̬
		if (m_unexpandImage != nullptr) {
			//��������չ��ͼ���ϣ���չ��
			if ((m_pUnExpandImageRect != nullptr) && 
				m_pUnExpandImageRect->ContainsPt(msg.ptMouse)) {
				SetExpand(true, true);
			}
		}
	}
	return bRet;
}

int32_t TreeNode::GetExpandImagePadding(void) const
{
	int32_t imageWidth = 0;
	Image* pImage = nullptr;
	if (m_unexpandImage != nullptr) {
		pImage = m_unexpandImage->GetStateImage(kControlStateNormal);
	}
	if(pImage == nullptr){
		if (m_expandImage != nullptr) {
			pImage = m_expandImage->GetStateImage(kControlStateNormal);
		}
	}
	if (pImage != nullptr) {
		if (pImage->GetImageCache() == nullptr) {
			LoadImageData(*pImage);
		}
		if (pImage->GetImageCache() != nullptr) {
			imageWidth = pImage->GetImageCache()->GetWidth();
		}
	}
	if (imageWidth > 0) {
		const int32_t extraPadding = ui::GlobalManager::Instance().Dpi().GetScaleInt(m_extraPadding);
		imageWidth += extraPadding;
	}
	return imageWidth;
}

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

bool TreeNode::OnItemSelectedChanged(const EventArgs& args)
{
	TreeNode* pItem = dynamic_cast<TreeNode*>(args.pSender);
	ASSERT(pItem != nullptr);
	if ((pItem != nullptr) && (m_pTreeView != nullptr)) {
		m_pTreeView->OnItemSelectedChanged(pItem);
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
	pTreeNode->AttachSelect(nbase::Bind(&TreeNode::OnItemSelectedChanged, this, std::placeholders::_1));
	pTreeNode->AttachUnSelect(nbase::Bind(&TreeNode::OnItemSelectedChanged, this, std::placeholders::_1));

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

	//[δչ��/չ��]ͼƬ��־
	std::wstring expandImageClass = m_pTreeView->GetExpandImageClass();
	pTreeNode->SetExpandImageClass(expandImageClass);

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

void TreeNode::SetExpandImageClass(const std::wstring& expandClass)
{
	if (!expandClass.empty()) {
		//����չ����־����
		SetClass(expandClass);
	}
	else {
		//�ر�չ����־����
		m_expandImage.reset();
		m_unexpandImage.reset();
		if (m_pExpandImageRect != nullptr) {
			delete m_pExpandImageRect;
			m_pExpandImageRect = nullptr;
		}
		if (m_pUnExpandImageRect != nullptr) {
			delete m_pUnExpandImageRect;
			m_pUnExpandImageRect = nullptr;
		}
	}
	AdjustExpandImagePadding();
}

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

void TreeNode::AdjustExpandImagePadding()
{
	uint16_t expandPadding = ui::TruncateToUInt8(GetExpandImagePadding());
	if (expandPadding != 0) {
		//��ʾ[չ��/δչ��]��־
		if (m_expandCheckBoxPadding == 0) {
			int32_t leftOffset = (int32_t)expandPadding;
			if (AdjustStateImagesPaddingLeft(leftOffset, false)) {
				m_expandCheckBoxPadding = expandPadding;
			}
		}

		if (m_expandIconPadding == 0) {
			//��CheckBox״̬ͼƬ, ��Ҫ���ñ���ͼƬ���ڱ߾࣬��������ͼƬ�ص�
			UiPadding rcBkPadding = GetBkImagePadding();
			rcBkPadding.left += expandPadding;
			if (SetBkImagePadding(rcBkPadding, false)) {
				m_expandIconPadding = expandPadding;
			}
		}

		if (m_expandTextPadding == 0) {
			//�������ֵ��ڱ߾�
			UiPadding rcTextPadding = GetTextPadding();
			rcTextPadding.left += expandPadding;
			SetTextPadding(rcTextPadding, false);
			m_expandTextPadding = expandPadding;
		}
	}
	else {
		//����ʾ[չ��/δչ��]��־
		if (m_expandCheckBoxPadding > 0) {
			int32_t leftOffset = -(int32_t)m_expandCheckBoxPadding;
			AdjustStateImagesPaddingLeft(leftOffset, false);
			m_expandCheckBoxPadding = 0;
		}

		if (m_expandIconPadding > 0) {
			UiPadding rcBkPadding = GetBkImagePadding();
			rcBkPadding.left -= (int32_t)m_expandIconPadding;
			if (rcBkPadding.left >= 0) {
				SetBkImagePadding(rcBkPadding, false);
			}
			m_expandIconPadding = 0;
		}
		if (m_expandTextPadding > 0) {
			UiPadding rcTextPadding = GetTextPadding();
			rcTextPadding.left -= (int32_t)m_expandTextPadding;
			if (rcTextPadding.left >= 0) {
				SetTextPadding(rcTextPadding, false);
			}
			m_expandTextPadding = 0;
		}		
	}
	Invalidate();
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
	AdjustExpandImagePadding();
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

void TreeNode::SetChildrenSelected(bool bSelected)
{
	if ((m_pTreeView == nullptr) || !m_pTreeView->IsMultiSelect()) {
		//��ѡ: ֱ�ӷ���
		return;
	}
	for (TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode) {
			pTreeNode->SetItemSelected(bSelected);
			pTreeNode->SetChildrenSelected(bSelected);
		}
	}
}

void TreeNode::UpdateParentSelected(bool bUpdateSelf)
{
	if ((m_pTreeView != nullptr) && !m_pTreeView->IsMultiSelect()) {
		//��ѡ: ֱ�ӷ���
		return;
	}
	if (bUpdateSelf) {
		UpdateTreeNodeSelect();
	}
	if (m_pParentTreeNode != nullptr) {
		m_pParentTreeNode->UpdateParentSelected(true);
	}
}

void TreeNode::UpdateTreeNodeSelect()
{
	if ((m_pTreeView != nullptr) && !m_pTreeView->IsMultiSelect()) {
		//��ѡ: ֱ�ӷ���
		return;
	}
	std::wstring text = this->GetText();
	bool bSelected = IsSelected();
	TreeNodeSelect nodeSelect = GetChildrenSelectStatus();//�����ӽڵ��ѡ��״̬���޸ĵ�ǰ�ڵ��ѡ��״̬
	if (nodeSelect == TreeNodeSelect::UnSelect) {
		if (!bSelected) {
			return;
		}
		else {
			//����Ϊ��TreeNodeSelect::UnSelect
			SetSelected(false);
			SetPartSelected(false);
			Invalidate();
		}
	}
	else if (nodeSelect == TreeNodeSelect::SelectAll) {
		//����Ϊ��TreeNodeSelect::SelectAll
		if (bSelected) {
			if (IsPartSelected()) {
				SetPartSelected(false);
				Invalidate();
			}
		}
		else {			
			SetSelected(true);
			SetPartSelected(false);
			Invalidate();
		}
	}
	else if (nodeSelect == TreeNodeSelect::SelectPart) {
		//����Ϊ��TreeNodeSelect::SelectPart
		SetSelected(true);
		SetPartSelected(true);
		Invalidate();
	}
}

TreeNodeSelect TreeNode::GetSelectStatus(void) const
{
	if ((m_pTreeView != nullptr) && !m_pTreeView->IsMultiSelect()) {
		//��ѡ: ֻ����ǰ�ڵ�״̬�жϽ��
		return IsSelected() ? TreeNodeSelect::SelectAll : TreeNodeSelect::UnSelect;
	}
	//��ѡ
	bool bSelected = IsSelected();
	for (TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode == nullptr) {
			continue;
		}
		bool bChildSelected = pTreeNode->IsSelected();
		if (bSelected != bChildSelected) {
			return TreeNodeSelect::SelectPart;
		}
	}

	for (TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode == nullptr) {
			continue;
		}
		TreeNodeSelect childSelect = pTreeNode->GetSelectStatus();
		if (bSelected) {
			if (childSelect == TreeNodeSelect::UnSelect) {
				return TreeNodeSelect::SelectPart;
			}
			else if (childSelect == TreeNodeSelect::SelectPart) {
				return TreeNodeSelect::SelectPart;
			}
		}
		else {
			if (childSelect == TreeNodeSelect::SelectAll) {
				return TreeNodeSelect::SelectPart;
			}
			else if (childSelect == TreeNodeSelect::SelectPart) {
				return TreeNodeSelect::SelectPart;
			}
		}
	}
	return bSelected ? TreeNodeSelect::SelectAll : TreeNodeSelect::UnSelect;
}

TreeNodeSelect TreeNode::GetChildrenSelectStatus(void) const
{
	if ((m_pTreeView != nullptr) && !m_pTreeView->IsMultiSelect()) {
		//��ѡ: ֻ����ǰ�ڵ�״̬�жϽ��
		return IsSelected() ? TreeNodeSelect::SelectAll : TreeNodeSelect::UnSelect;
	}
	if (m_aTreeNodes.empty()) {
		//û���ӽڵ㣺���ص�ǰ�ڵ��״̬
		return IsSelected() ? TreeNodeSelect::SelectAll : TreeNodeSelect::UnSelect;
	}
	//��ѡ: ��ɨ��һ���ӽڵ�
	bool bLastSelected = false;
	bool bSetLastSelected = false;
	for (TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode == nullptr) {
			continue;
		}
		bool bChildSelected = pTreeNode->IsSelected();
		if (!bSetLastSelected) {
			bLastSelected = bChildSelected;
			bSetLastSelected = true;
		}
		else {
			if (bLastSelected != bChildSelected) {
				return TreeNodeSelect::SelectPart;
			}
		}		
	}

	//��ɨ��༶�ӽڵ�
	TreeNodeSelect lastChildSelect = TreeNodeSelect::UnSelect;
	bool bSetLastChildrenSelect = false;
	for (TreeNode* pTreeNode : m_aTreeNodes) {
		if (pTreeNode == nullptr){
			continue;
		}
		TreeNodeSelect childSelect = pTreeNode->GetSelectStatus();//��ȡ��������/�ӽڵ��ѡ��״̬
		if (childSelect == TreeNodeSelect::SelectPart) {
			return TreeNodeSelect::SelectPart;
		}
		if (!bSetLastChildrenSelect) {
			lastChildSelect = childSelect;
			bSetLastChildrenSelect = true;
		}
		else {
			if (childSelect != lastChildSelect) {
				return TreeNodeSelect::SelectPart;
			}
		}
	}

	//���״̬��һ�������ص�һ���ӽڵ��״̬
	TreeNode* pTreeNode = m_aTreeNodes.front();
	if (pTreeNode != nullptr) {
		return pTreeNode->IsSelected() ? TreeNodeSelect::SelectAll : TreeNodeSelect::UnSelect;
	}
	else {
		return IsSelected() ? TreeNodeSelect::SelectAll : TreeNodeSelect::UnSelect;
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
	else if (strName == L"expand_image_class") {
		//�Ƿ���ʾ[չ��/δչ��]ͼ��
		SetExpandImageClass(strValue);
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

void TreeView::SetExpandImageClass(const std::wstring& className)
{
	bool isChanged = m_expandImageClass != className;
	m_expandImageClass = className;
	if (isChanged) {
		for (Control* pControl : m_items) {
			TreeNode* pTreeNode = dynamic_cast<TreeNode*>(pControl);
			if (pTreeNode != nullptr) {
				pTreeNode->SetExpandImageClass(className);
			}
		}
	}
}

std::wstring TreeView::GetExpandImageClass() const
{
	return m_expandImageClass.c_str();
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

void TreeView::OnItemSelectedChanged(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr) {
		return;
	}
	if (!IsMultiSelect()) {
		//��ѡ��ֱ�ӷ���
		return;
	}

	//��ѡ
	bool isSelected = pTreeNode->IsSelected();
	//ͬ���ӽڵ��ѡ��״̬�����浱ǰ�ڵ�
	pTreeNode->SetChildrenSelected(isSelected);

	//ͬ�����ڵ��ѡ��״̬
	pTreeNode->UpdateParentSelected(false);
}

}