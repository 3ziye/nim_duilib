#include "Control.h"
#include "duilib/Core/ControlLoading.h"
#include "duilib/Core/Window.h"
#include "duilib/Core/Box.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/ColorManager.h"
#include "duilib/Image/Image.h"
#include "duilib/Render/IRender.h"
#include "duilib/Render/AutoClip.h"
#include "duilib/Animation/AnimationPlayer.h"
#include "duilib/Animation/AnimationManager.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Utils/AttributeUtil.h"

namespace ui 
{
Control::Control() :
	m_bContextMenuUsed(false),
	m_bEnabled(true),
	m_bMouseEnabled(true),
	m_bKeyboardEnabled(true),
	m_bMouseFocused(false),
	m_bNoFocus(false),
	m_bClip(true),
	m_bGifPlay(true),
	m_bAllowTabstop(true),
	m_renderOffset(),
	m_cxyBorderRound(),
	m_rcPaint(),
	m_rcBorderSize(),
	m_cursorType(kCursorArrow),
	m_controlState(kControlStateNormal),
	m_nTooltipWidth(300),
	m_nAlpha(255),
	m_nHotAlpha(0),
	m_sToolTipText(),
	m_sToolTipTextId(),
	m_sUserDataID(),
	m_strBkColor(),
	m_strBorderColor(),
	m_gifWeakFlag(),	
	m_loadBkImageWeakFlag(),
	m_pBoxShadow(nullptr),
	m_isBoxShadowPainted(false),
	m_uUserDataID((size_t)-1),
	m_pOnEvent(nullptr),
	m_pOnXmlEvent(nullptr),
	m_pOnBubbledEvent(nullptr),
	m_pOnXmlBubbledEvent(nullptr),
	m_pLoading(nullptr)
{
}

Control::~Control()
{
	//�ɷ����һ���¼�
	SendEvent(kEventLast);

	//�����������Դ�����ⶨʱ���ٲ����ص�����������
	if (m_animationManager != nullptr) {
		m_animationManager->Clear(this);
	}	
	m_animationManager.reset();

	Window* pWindow = GetWindow();
	if (pWindow) {
		pWindow->ReapObjects(this);
	}

	if (m_pLoading != nullptr) {
		delete m_pLoading;
		m_pLoading = nullptr;
	}

	if (m_pBoxShadow != nullptr) {
		delete m_pBoxShadow;
		m_pBoxShadow = nullptr;
	}
	if (m_pOnEvent != nullptr) {
		delete m_pOnEvent;
		m_pOnEvent = nullptr;
	}
	if (m_pOnXmlEvent != nullptr) {
		delete m_pOnXmlEvent;
		m_pOnXmlEvent = nullptr;
	}
	if (m_pOnBubbledEvent != nullptr) {
		delete m_pOnBubbledEvent;
		m_pOnBubbledEvent = nullptr;
	}
	if (m_pOnXmlBubbledEvent != nullptr) {
		delete m_pOnXmlBubbledEvent;
		m_pOnXmlBubbledEvent = nullptr;
	}
}

std::wstring Control::GetType() const { return DUI_CTR_CONTROL; }

AnimationManager& Control::GetAnimationManager()
{
	if (m_animationManager == nullptr) {
		m_animationManager = std::make_unique<AnimationManager>(),
		m_animationManager->Init(this);
	}
	return *m_animationManager;
}

void Control::SetBkColor(const std::wstring& strColor)
{
	ASSERT(strColor.empty() || HasUiColor(strColor));
	if (m_strBkColor == strColor) {
		return;
	}
	m_strBkColor = strColor;
	Invalidate();
}

std::wstring Control::GetStateColor(ControlStateType stateType) const
{
	if (m_pColorMap != nullptr) {
		return m_pColorMap->GetStateColor(stateType);
	}
	return std::wstring();
}

void Control::SetStateColor(ControlStateType stateType, const std::wstring& strColor)
{
	ASSERT(strColor.empty() || HasUiColor(strColor));
	if (m_pColorMap != nullptr) {
		if (m_pColorMap->GetStateColor(stateType) == strColor) {
			return;
		}
	}
	if (m_pColorMap == nullptr) {
		m_pColorMap = std::make_unique<StateColorMap>();
		m_pColorMap->SetControl(this);
	}
	m_pColorMap->SetStateColor(stateType, strColor);
	if (stateType == kControlStateHot) {
		GetAnimationManager().SetFadeHot(true);
	}
	Invalidate();
}

std::wstring Control::GetBkImage() const
{
	if (m_pBkImage != nullptr) {
		return m_pBkImage->GetImageString();
	}
	return std::wstring();
}

std::string Control::GetUTF8BkImage() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetBkImage(), strOut, CP_UTF8);
	return strOut;
}

void Control::SetBkImage(const std::wstring& strImage)
{
	StopGifPlay();
	if (!strImage.empty()) {
		if (m_pBkImage == nullptr) {
			m_pBkImage = std::make_unique<Image>();
		}
	}
	if (m_pBkImage != nullptr) {
		m_pBkImage->SetImageString(strImage);
		m_bGifPlay = m_pBkImage->GetImageAttribute().nPlayCount != 0;
	}
	else {
		m_bGifPlay = false;
	}
	RelayoutOrRedraw();
}

void Control::SetUTF8BkImage(const std::string& strImage)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strImage, strOut, CP_UTF8);
	SetBkImage(strOut);
}

void Control::SetLoadingImage(const std::wstring& strImage) 
{
	StopGifPlay();
	if (!strImage.empty()) {
		if (m_pLoading == nullptr) {
			m_pLoading = new ControlLoading(this);
		}
	}
	if (m_pLoading != nullptr) {
		if (m_pLoading->SetLoadingImage(strImage)) {
			Invalidate();
		}
	}	
}

void Control::SetLoadingBkColor(const std::wstring& strColor) 
{
	if (m_pLoading != nullptr) {
		if (m_pLoading->SetLoadingBkColor(strColor)) {
			Invalidate();
		}
	}    
}

void Control::StartLoading(int32_t fStartAngle)
{
	if ((m_pLoading != nullptr) && m_pLoading->StartLoading(fStartAngle)) {
		SetEnabled(false);
	}
}

void Control::StopLoading(GifStopType frame)
{
	if (m_pLoading != nullptr) {
		m_pLoading->StopLoading(frame);
	}
	SetEnabled(true);
}

bool Control::HasImageType(StateImageType stateImageType) const
{
	if (m_pImageMap != nullptr) {
		return m_pImageMap->HasImageType(stateImageType);
	}
	return false;
}

std::wstring Control::GetStateImage(StateImageType imageType, ControlStateType stateType)
{
	if (m_pImageMap != nullptr) {
		return m_pImageMap->GetImageString(imageType, stateType);
	}
	return std::wstring();
}

void Control::SetStateImage(StateImageType imageType, ControlStateType stateType, const std::wstring& strImage)
{
	if (m_pImageMap == nullptr) {
		m_pImageMap = std::make_unique<StateImageMap>();
		m_pImageMap->SetControl(this);
	}
	m_pImageMap->SetImageString(imageType, stateType, strImage);
}

bool Control::PaintStateImage(IRender* pRender, StateImageType stateImageType, ControlStateType stateType, const std::wstring& sImageModify)
{
	if (m_pImageMap != nullptr) {
		return m_pImageMap->PaintStateImage(pRender, stateImageType, stateType, sImageModify);
	}
	return false;
}

std::wstring Control::GetStateImage(ControlStateType stateType)
{
	return GetStateImage(kStateImageBk, stateType);
}

void Control::SetStateImage(ControlStateType stateType, const std::wstring& strImage)
{
	if (stateType == kControlStateHot) {
		GetAnimationManager().SetFadeHot(true);
	}
	SetStateImage(kStateImageBk, stateType, strImage);
	RelayoutOrRedraw();
}

std::wstring Control::GetForeStateImage(ControlStateType stateType)
{
	return GetStateImage(kStateImageFore, stateType);
}

void Control::SetForeStateImage(ControlStateType stateType, const std::wstring& strImage)
{
	if (stateType == kControlStateHot) {
		GetAnimationManager().SetFadeHot(true);
	}
	SetStateImage(kStateImageFore, stateType, strImage);
	Invalidate();
}

ControlStateType Control::GetState() const
{
	return m_controlState;
}

void Control::SetState(ControlStateType controlState)
{
	if (controlState == kControlStateNormal) {
		m_nHotAlpha = 0;
	}
	else if (controlState == kControlStateHot) {
		m_nHotAlpha = 255;
	}
	m_controlState = controlState;
	Invalidate();
}

std::wstring Control::GetBorderColor() const
{
    return m_strBorderColor.c_str();
}

void Control::SetBorderColor(const std::wstring& strBorderColor)
{
	if (m_strBorderColor == strBorderColor) {
		return;
	}
    m_strBorderColor = strBorderColor;
    Invalidate();
}

void Control::SetBorderSize(UiRect rc)
{
	GlobalManager::Instance().Dpi().ScaleRect(rc);
	rc.left = std::max(rc.left, 0);
	rc.top = std::max(rc.top, 0);
	rc.right = std::max(rc.right, 0);
	rc.bottom = std::max(rc.bottom, 0);
	m_rcBorderSize = rc;
	Invalidate();
}

int32_t Control::GetLeftBorderSize() const
{
	return m_rcBorderSize.left;
}

void Control::SetLeftBorderSize(int32_t nSize)
{
	GlobalManager::Instance().Dpi().ScaleInt(nSize);
	m_rcBorderSize.left = nSize;
	Invalidate();
}

int32_t Control::GetTopBorderSize() const
{
	return m_rcBorderSize.top;
}

void Control::SetTopBorderSize(int32_t nSize)
{
	GlobalManager::Instance().Dpi().ScaleInt(nSize);
	m_rcBorderSize.top = nSize;
	Invalidate();
}

int32_t Control::GetRightBorderSize() const
{
	return m_rcBorderSize.right;
}

void Control::SetRightBorderSize(int32_t nSize)
{
	GlobalManager::Instance().Dpi().ScaleInt(nSize);
	m_rcBorderSize.right = nSize;
	Invalidate();
}

int32_t Control::GetBottomBorderSize() const
{
	return m_rcBorderSize.bottom;
}

void Control::SetBottomBorderSize(int32_t nSize)
{
	GlobalManager::Instance().Dpi().ScaleInt(nSize);
	m_rcBorderSize.bottom = nSize;
	Invalidate();
}

const UiSize& Control::GetBorderRound() const
{
    return m_cxyBorderRound;
}

void Control::SetBorderRound(UiSize cxyRound)
{
	int32_t cx = cxyRound.cx;
	int32_t cy = cxyRound.cy;
	ASSERT(cx >= 0);
	ASSERT(cy >= 0);
	if ((cx < 0) || (cy < 0)) {
		return;
	}
	//��������Ҫôͬʱ����0��Ҫôͬʱ����0�����������Ч
	ASSERT(((cx > 0) && (cy > 0)) || ((cx == 0) && (cy == 0)));
	if (cx == 0) {
		if (cy != 0) {
			return;
		}
	}
	else {
		if (cy == 0) {
			return;
		}
	}
	GlobalManager::Instance().Dpi().ScaleSize(cxyRound);
    m_cxyBorderRound = cxyRound;
    Invalidate();
}

void Control::SetBoxShadow(const std::wstring& strShadow)
{
	if (strShadow.empty()) {
		return;
	}
	if (m_pBoxShadow == nullptr) {
		m_pBoxShadow = new BoxShadow;
	}
	m_pBoxShadow->SetBoxShadowString(strShadow);
}

CursorType Control::GetCursorType() const
{
	return m_cursorType;
}

void Control::SetCursorType(CursorType cursorType)
{
	m_cursorType = cursorType;
}

std::wstring Control::GetToolTipText() const
{
	std::wstring strText = m_sToolTipText.c_str();
	if (strText.empty() && !m_sToolTipTextId.empty()) {
		strText = GlobalManager::Instance().Lang().GetStringViaID(m_sToolTipTextId.c_str());
	}
	return strText;
}

std::string Control::GetUTF8ToolTipText() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetToolTipText(), strOut, CP_UTF8);
	return strOut;
}

void Control::SetToolTipText(const std::wstring& strText)
{
	std::wstring strTemp(strText);
	StringHelper::ReplaceAll(L"<n>",L"\r\n", strTemp);
	m_sToolTipText = strTemp;

	Invalidate();
}

void Control::SetUTF8ToolTipText(const std::string& strText)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strText, strOut, CP_UTF8);
	if (strOut.empty()) {
		m_sToolTipText.clear();
		Invalidate();
		return ;
	}

	if (m_sToolTipText != strOut) {
		SetToolTipText(strOut);
	}
}

void Control::SetToolTipTextId(const std::wstring& strTextId)
{
	if (m_sToolTipTextId == strTextId) {
		return;
	}
	m_sToolTipTextId = strTextId;
	Invalidate();
}

void Control::SetUTF8ToolTipTextId(const std::string& strTextId)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strTextId, strOut, CP_UTF8);
	SetToolTipTextId(strOut);
}

void Control::SetToolTipWidth( int32_t nWidth )
{
	GlobalManager::Instance().Dpi().ScaleInt(nWidth);
	m_nTooltipWidth = nWidth;
}

int32_t Control::GetToolTipWidth(void) const
{
	return m_nTooltipWidth;
}

void Control::SetContextMenuUsed(bool bMenuUsed)
{
	m_bContextMenuUsed = bMenuUsed;
}

std::wstring Control::GetDataID() const
{
    return m_sUserDataID.c_str();
}

std::string Control::GetUTF8DataID() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetDataID(), strOut, CP_UTF8);
	return strOut;
}

void Control::SetDataID(const std::wstring& strText)
{
	m_sUserDataID = strText;
}

void Control::SetUTF8DataID(const std::string& strText)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strText, strOut, CP_UTF8);
	m_sUserDataID = strOut;
}

void Control::SetUserDataID(size_t dataID)
{
	m_uUserDataID = dataID;
}

size_t Control::GetUserDataID() const
{
	return m_uUserDataID;
}

void Control::SetFadeVisible(bool bVisible)
{
	if (bVisible) {
		GetAnimationManager().Appear();
	}
	else {
		GetAnimationManager().Disappear();
	}
}

void Control::SetVisible(bool bVisible)
{
	if (IsVisible() == bVisible) {
		return;
	}
	bool v = IsVisible();
	__super::SetVisible(bVisible);

	if (!IsVisible()) {
		EnsureNoFocus();
	}

	if (IsVisible() != v) {
		ArrangeAncestor();
	}

	if (!IsVisible()) {
		StopGifPlay();
	}

	SendEvent(kEventVisibleChange);
}

void Control::SetEnabled(bool bEnabled)
{
	if (m_bEnabled == bEnabled) {
		return;
	}

    m_bEnabled = bEnabled;
	if (m_bEnabled) {
		m_controlState = kControlStateNormal;
		m_nHotAlpha = 0;
	}
	else {
		m_controlState = kControlStateDisabled;
	}
    Invalidate();
}

void Control::SetMouseEnabled(bool bEnabled)
{
    m_bMouseEnabled = bEnabled;
}

void Control::SetKeyboardEnabled(bool bEnabled)
{
	m_bKeyboardEnabled = bEnabled ; 
}

bool Control::IsFocused() const
{
	Window* pWindow = GetWindow();
    return ((pWindow != nullptr) && (pWindow->GetFocus() == this) );
}

void Control::SetFocus()
{
	if (m_bNoFocus) {
		return;
	}
	Window* pWindow = GetWindow();
	if (pWindow != nullptr) {
		pWindow->SetFocus(this);
	}
}

UINT Control::GetControlFlags() const
{
	return IsAllowTabStop() ? UIFLAG_TABSTOP : UIFLAG_DEFAULT;
}

void Control::SetNoFocus()
{
    m_bNoFocus = true;
	EnsureNoFocus();
}

void Control::Activate()
{

}

bool Control::IsActivatable() const
{
	if (!IsVisible() || !IsEnabled()) {
		return false;
	}
	return true;
}

Control* Control::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint /*scrollPos*/)
{
	if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) {
		return nullptr;
	}
	if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) {
		return nullptr;
	}
	if ((uFlags & UIFIND_HITTEST) != 0 && 
		(!m_bMouseEnabled || ((pData != nullptr) && !GetRect().ContainsPt(*static_cast<UiPoint*>(pData))))) {
		return nullptr;
	}
    return Proc(this, pData);
}

UiRect Control::GetPos() const
{
	return __super::GetPos();
}

void Control::SetPos(UiRect rc)
{
	rc.Validate();
	SetArranged(false);
	if (GetRect().Equals(rc)) {		
		return;
	}

	UiRect invalidateRc = GetRect();
	if (invalidateRc.IsEmpty()) {
		invalidateRc = rc;
	}

	SetRect(rc);
	if (GetWindow() == nullptr) {
		return;
	}
	invalidateRc.Union(GetRect());
	bool needInvalidate = true;
	UiRect rcTemp;
	UiRect rcParent;
	UiPoint offset = GetScrollOffsetInScrollBox();
	invalidateRc.Offset(-offset.x, -offset.y);
	Control* pParent = GetParent();
	while (pParent != nullptr) {
		rcTemp = invalidateRc;
		rcParent = pParent->GetPos();
		if (!UiRect::Intersect(invalidateRc, rcTemp, rcParent)) {
			needInvalidate = false;
			break;
		}
		pParent = pParent->GetParent();
	}
	if (needInvalidate && (GetWindow() != nullptr)) {
		GetWindow()->Invalidate(invalidateRc);
	}

	SendEvent(kEventResize);
}

UiEstSize Control::EstimateSize(UiSize szAvailable)
{
	UiFixedSize fixedSize = GetFixedSize();
	if (!fixedSize.cx.IsAuto() && !fixedSize.cy.IsAuto()) {
		//�����߶�����auto���ԣ���ֱ�ӷ���
		return MakeEstSize(fixedSize);
	}
	szAvailable.Validate();
	if (!IsReEstimateSize(szAvailable)) {
		//ʹ�û����еĹ�����
		return GetEstimateSize();
	}

	//����ͼƬ�����С
	UiSize imageSize;
	std::shared_ptr<ImageInfo> imageCache;
	Image* image = GetEstimateImage();
	if (image != nullptr) {
		//����ͼƬ����Ҫ��ȡͼƬ�Ŀ�͸�
		LoadImageData(*image);
		imageCache = image->GetImageCache();		
	}
	if (imageCache != nullptr) {
		ImageAttribute imageAttribute = image->GetImageAttribute();
		UiRect rcDest;
		bool hasDestAttr = false;
		if (ImageAttribute::HasValidImageRect(imageAttribute.GetDestRect())) {
			//ʹ��������ָ����Ŀ������
			rcDest = imageAttribute.GetDestRect();
			hasDestAttr = true;
		}
		UiRect rcDestCorners;
		UiRect rcSource = imageAttribute.GetSourceRect();
		UiRect rcSourceCorners = imageAttribute.GetCorner();
		ImageAttribute::ScaleImageRect(imageCache->GetWidth(), imageCache->GetHeight(),
									   imageCache->IsBitmapSizeDpiScaled(),
									   rcDestCorners,
									   rcSource,
									   rcSourceCorners);
		if (rcDest.Width() > 0) {
			imageSize.cx = rcDest.Width();
		}
		else if (rcSource.Width() > 0) {
			imageSize.cx = rcSource.Width();
		}
		else {
			imageSize.cx = imageCache->GetWidth();
		}

		if (rcDest.Height() > 0) {
			imageSize.cy = rcDest.Height();
		}
		else if (rcSource.Height() > 0) {
			imageSize.cy = rcSource.Height();
		}
		else {
			imageSize.cy = imageCache->GetHeight();
		}
		if (!hasDestAttr) {
			//���û��rcDest���ԣ�����Ҫ����ͼƬ���ڱ߾�
			UiPadding rcPadding = imageAttribute.GetPadding();
			imageSize.cx += (rcPadding.left + rcPadding.right);
			imageSize.cy += (rcPadding.top + rcPadding.bottom);
		}
	}
	imageCache.reset();

	//�����ı������С
	UiSize textSize = EstimateText(szAvailable);

	//ѡȡͼƬ���ı�����߶ȺͿ�ȵ����ֵ
	if (fixedSize.cx.IsAuto()) {
		fixedSize.cx.SetInt32(std::max(imageSize.cx, textSize.cx));
	}
	if (fixedSize.cy.IsAuto()) {
		fixedSize.cy.SetInt32(std::max(imageSize.cy, textSize.cy));
	}
	//���ֽ�������棬����ÿ�ζ����¹���
	UiEstSize estSize = MakeEstSize(fixedSize);
	SetEstimateSize(estSize, szAvailable);
	SetReEstimateSize(false);
	return estSize;
}

Image* Control::GetEstimateImage()
{
	Image* estimateImage = nullptr;
	if ((m_pBkImage != nullptr) && !m_pBkImage->GetImagePath().empty()) {
		estimateImage = m_pBkImage.get();
	}
	else if(m_pImageMap != nullptr){
		estimateImage = m_pImageMap->GetEstimateImage(kStateImageBk);
		if (estimateImage == nullptr) {
			estimateImage = m_pImageMap->GetEstimateImage(kStateImageSelectedBk);
		}
	}
	return estimateImage;
}

UiSize Control::EstimateText(UiSize /*szAvailable*/)
{
	return UiSize(0, 0);
}

bool Control::IsPointInWithScrollOffset(const UiPoint& point) const
{
	UiPoint scrollOffset = GetScrollOffsetInScrollBox();
	UiPoint newPoint = point;
	newPoint.Offset(scrollOffset);
	return GetRect().ContainsPt(newPoint);
}

void Control::SendEvent(EventType eventType, 
					    WPARAM wParam, 
					    LPARAM lParam, 
					    TCHAR tChar, 
					    const UiPoint& mousePos)
{
	EventArgs msg;
	msg.pSender = this;
	msg.Type = eventType;
	msg.chKey = tChar;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if ((mousePos.x == 0) && (mousePos.y == 0)) {
		Window* pWindow = GetWindow();
		if (pWindow != nullptr) {
			msg.ptMouse = pWindow->GetLastMousePos();
		}
	}
	else {
		msg.ptMouse = mousePos;
	}
	msg.dwTimestamp = ::GetTickCount();

	SendEvent(msg);
}

void Control::SendEvent(const EventArgs& msg)
{
	bool bRet = FireAllEvents(msg);	
    if(bRet) {
		HandleEvent(msg);
	}
}

void Control::HandleEvent(const EventArgs& msg)
{
	if( !IsMouseEnabled() && 
		(msg.Type > kEventMouseBegin) && 
		(msg.Type < kEventMouseEnd)) {
		//��ǰ�ؼ���ֹ���������Ϣʱ������������Ϣת�����ϲ㴦��
		Box* pParent = GetParent();
		if (pParent != nullptr) {
			pParent->SendEvent(msg);
		}
		return;
	}
	else if( msg.Type == kEventSetCursor ) {
		if (m_cursorType == kCursorHand) {
			if (IsEnabled()) {
				::SetCursor(::LoadCursor(NULL, IDC_HAND));
			}
			else {
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			}
			return;
		}
		else if (m_cursorType == kCursorArrow){
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			return;
		}
		else if (m_cursorType == kCursorHandIbeam){
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			return;
		}
		else {
			ASSERT(FALSE);
		}
	}
	else if (msg.Type == kEventSetFocus && m_controlState == kControlStateNormal) {
		SetState(kControlStateHot);
		Invalidate();
		return;
	}
	else if (msg.Type == kEventKillFocus && m_controlState == kControlStateHot) {
		SetState(kControlStateNormal);
		Invalidate();
		return;
	}
	else if( msg.Type == kEventMouseEnter ) {
		if (GetWindow()) {
			if (!IsChild(this, GetWindow()->GetHoverControl())) {
				return;
			}
		}
		if (!MouseEnter(msg))
			return;
	}
	else if( msg.Type == kEventMouseLeave ) {
		if (GetWindow()) {
			if (IsChild(this, GetWindow()->GetHoverControl())) {
				return;
			}
		}
		if (!MouseLeave(msg))
			return;
	}
	else if (msg.Type == kEventMouseButtonDown) {
		ButtonDown(msg);
		return;
	}
	else if (msg.Type == kEventMouseButtonUp) {
		ButtonUp(msg);
		return;
	}

	if (GetParent() != nullptr) {
		GetParent()->SendEvent(msg);
	}
}

bool Control::HasHotState()
{
	bool bState = false;
	if (m_pColorMap != nullptr) {
		bState = m_pColorMap->HasHotColor();
	}
	if (!bState && (m_pImageMap != nullptr)) {
		bState = m_pImageMap->HasHotImage();
	}
	return bState;
}

bool Control::MouseEnter(const EventArgs& /*msg*/)
{
	if( IsEnabled() ) {
		if (m_controlState == kControlStateNormal) {
			m_controlState = kControlStateHot;
			if (HasHotState()) {
				GetAnimationManager().MouseEnter();
				Invalidate();
			}
			return true;
		}
		else {
			return false;
		}
	}

	return true;
}

bool Control::MouseLeave(const EventArgs& /*msg*/)
{
	if( IsEnabled() ) {
		if (m_controlState == kControlStateHot) {
			m_controlState = kControlStateNormal;
			if (HasHotState()) {
				GetAnimationManager().MouseLeave();
				Invalidate();
			}
			return true;
		}
		else {
			return false;
		}
	}

	return true;
}

bool Control::ButtonDown(const EventArgs& /*msg*/)
{
	bool ret = false;
	if( IsEnabled() ) {
		m_controlState = kControlStatePushed;
		SetMouseFocused(true);
		Invalidate();
		ret = true;
	}

	return ret;
}

bool Control::ButtonUp(const EventArgs& msg)
{
	bool ret = false;
	if( IsMouseFocused() ) {
		SetMouseFocused(false);
		auto player = GetAnimationManager().GetAnimationPlayer(kAnimationHot);
		if (player)
			player->Stop();

		Invalidate();
		if( IsPointInWithScrollOffset(UiPoint(msg.ptMouse)) ) {
			m_controlState = kControlStateHot;
			m_nHotAlpha = 255;
			Activate();
			ret = true;
		}
		else {
			m_controlState = kControlStateNormal;
			m_nHotAlpha = 0;
		}
	}

	return ret;
}

void Control::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == L"class") {
		SetClass(strValue);
	}
	else if (strName == L"halign") {
		if (strValue == L"left") {
			SetHorAlignType(kHorAlignLeft);
		}
		else if (strValue == L"center") {
			SetHorAlignType(kHorAlignCenter);
		}
		else if (strValue == L"right") {
			SetHorAlignType(kHorAlignRight);
		}
		else {
			ASSERT(FALSE);
		}
	}
	else if (strName == L"valign") {
		if (strValue == L"top") {
			SetVerAlignType(kVerAlignTop);
		}
		else if (strValue == L"center") {
			SetVerAlignType(kVerAlignCenter);
		}
		else if (strValue == L"bottom") {
			SetVerAlignType(kVerAlignBottom);
		}
		else {
			ASSERT(FALSE);
		}
	}
	else if (strName == L"margin") {
		UiMargin rcMargin;
		AttributeUtil::ParseMarginValue(strValue.c_str(), rcMargin);
		SetMargin(rcMargin, true);
	}
	else if (strName == L"bkcolor" || strName == L"bkcolor1") {
		SetBkColor(strValue);
	}
	else if ((strName == L"border_size") || (strName == L"bordersize")){
		std::wstring nValue = strValue;
		if (nValue.find(L',') == std::wstring::npos) {
			int32_t nBorderSize = _wtoi(strValue.c_str());
			if (nBorderSize < 0) {
				nBorderSize = 0;
			}
			UiRect rcBorder(nBorderSize, nBorderSize, nBorderSize, nBorderSize);
			SetBorderSize(rcBorder);
		}
		else {
			UiMargin rcMargin;
			AttributeUtil::ParseMarginValue(strValue.c_str(), rcMargin);
			UiRect rcBorder(rcMargin.left, rcMargin.top, rcMargin.right, rcMargin.bottom);
			SetBorderSize(rcBorder);
		}
	}
	else if ((strName == L"border_round") || (strName == L"borderround")) {
		UiSize cxyRound;
		AttributeUtil::ParseSizeValue(strValue.c_str(), cxyRound);
		SetBorderRound(cxyRound);
	}
	else if ((strName == L"box_shadow") || (strName == L"boxshadow")) {
		SetBoxShadow(strValue);
	}
	else if(strName == L"width") {
		if (strValue == L"stretch") {
			SetFixedWidth(UiFixedInt::MakeStretch(), true, true);
		}
		else if (strValue == L"auto") {
			SetFixedWidth(UiFixedInt::MakeAuto(), true, true);
		}
		else {
			ASSERT(_wtoi(strValue.c_str()) >= 0);
			SetFixedWidth(UiFixedInt(_wtoi(strValue.c_str())), true, true);
		}
	}
	else if(strName == L"height") {
		if (strValue == L"stretch") {
			SetFixedHeight(UiFixedInt::MakeStretch(), true);
		}
		else if (strValue == L"auto") {
			SetFixedHeight(UiFixedInt::MakeAuto(), true);
		}
		else {
			ASSERT(_wtoi(strValue.c_str()) >= 0);
			SetFixedHeight(UiFixedInt(_wtoi(strValue.c_str())), true);
		}
	}
	else if(strName == L"state") {
		if (strValue == L"normal") {
			SetState(kControlStateNormal);
		}
		else if (strValue == L"hot") {
			SetState(kControlStateHot);
		}
		else if (strValue == L"pushed") {
			SetState(kControlStatePushed);
		}
		else if (strValue == L"disabled") {
			SetState(kControlStateDisabled);
		}
		else {
			ASSERT(FALSE);
		}
	}
	else if (strName == L"cursortype") {
		if (strValue == L"arrow") {
			SetCursorType(kCursorArrow);
		}
		else if (strValue == L"hand") {
			SetCursorType(kCursorHand);
		}
		else if (strValue == L"ibeam") {
			SetCursorType(kCursorHandIbeam);
		}
		else {
			ASSERT(FALSE);
		}
	}
	else if ((strName == L"render_offset") || (strName == L"renderoffset")) {
		UiPoint renderOffset;
		AttributeUtil::ParsePointValue(strValue.c_str(), renderOffset);
		GlobalManager::Instance().Dpi().ScalePoint(renderOffset);
		SetRenderOffset(renderOffset);
	}
	else if ((strName == L"normal_color") || (strName == L"normalcolor")) {
		SetStateColor(kControlStateNormal, strValue);
	}
	else if ((strName == L"hot_color") || (strName == L"hotcolor")) {
		SetStateColor(kControlStateHot, strValue);
	}
	else if ((strName == L"pushed_color") || (strName == L"pushedcolor")) {
		SetStateColor(kControlStatePushed, strValue);
	}
	else if ((strName == L"disabled_color") || (strName == L"disabledcolor")) {
		SetStateColor(kControlStateDisabled, strValue);
	}
	else if ((strName == L"border_color") || (strName == L"bordercolor")) {
		SetBorderColor(strValue);
	}
	else if ((strName == L"left_border_size") || (strName == L"leftbordersize")) {
		SetLeftBorderSize(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"top_border_size") || (strName == L"topbordersize")){
		SetTopBorderSize(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"right_border_size") || (strName == L"rightbordersize")) {
		SetRightBorderSize(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"bottom_border_size") || (strName == L"bottombordersize")) {
		SetBottomBorderSize(_wtoi(strValue.c_str()));
	}
	else if (strName == L"bkimage") {
		SetBkImage(strValue);
	}
	else if ((strName == L"min_width") || (strName == L"minwidth")) {
		SetMinWidth(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"max_width") || (strName == L"maxwidth")) {
		SetMaxWidth(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"min_height") || (strName == L"minheight")) {
		SetMinHeight(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"max_height") || (strName == L"maxheight")) {
		SetMaxHeight(_wtoi(strValue.c_str()));
	}
	else if (strName == L"name") {
		SetName(strValue);
	}
	else if ((strName == L"tooltip_text") || (strName == L"tooltiptext")) {
		SetToolTipText(strValue);
	}
	else if ((strName == L"tooltip_textid") || (strName == L"tooltiptextid")) {
		SetToolTipTextId(strValue);
	}
	else if (strName == L"dataid") {
		SetDataID(strValue);
	}
	else if (strName == L"enabled") {
		SetEnabled(strValue == L"true");
	}
	else if (strName == L"mouse") {
		SetMouseEnabled(strValue == L"true");
	}
	else if (strName == L"keyboard") {
		SetKeyboardEnabled(strValue == L"true");
	}
	else if (strName == L"visible") {
		SetVisible(strValue == L"true");
	}
	else if ((strName == L"fade_visible") || (strName == L"fadevisible")) {
		SetFadeVisible(strValue == L"true");
	}
	else if (strName == L"float") {
		SetFloat(strValue == L"true");
	}
	else if (strName == L"cache") {
		SetUseCache(strValue == L"true");
	}
	else if (strName == L"nofocus") {
		SetNoFocus();
	}
	else if (strName == L"alpha") {
		SetAlpha(_wtoi(strValue.c_str()));
	}
	else if ((strName == L"normal_image") || (strName == L"normalimage")) {
		SetStateImage(kControlStateNormal, strValue);
	}
	else if ((strName == L"hot_image") || (strName == L"hotimage")) {
		SetStateImage(kControlStateHot, strValue);
	}
	else if ((strName == L"pushed_image") || (strName == L"pushedimage")) {
		SetStateImage(kControlStatePushed, strValue);
	}
	else if ((strName == L"disabled_image") || (strName == L"disabledimage")) {
		SetStateImage(kControlStateDisabled, strValue);
	}
	else if ((strName == L"fore_normal_image") || (strName == L"forenormalimage")) {
		SetForeStateImage(kControlStateNormal, strValue);
	}
	else if ((strName == L"fore_hot_image") || (strName == L"forehotimage")) {
		SetForeStateImage(kControlStateHot, strValue);
	}
	else if ((strName == L"fore_pushed_image") || (strName == L"forepushedimage")) {
		SetForeStateImage(kControlStatePushed, strValue);
	}
	else if ((strName == L"fore_disabled_image") || (strName == L"foredisabledimage")) {
		SetForeStateImage(kControlStateDisabled, strValue);
	}
	else if ((strName == L"fade_alpha") || (strName == L"fadealpha")) {
		GetAnimationManager().SetFadeAlpha(strValue == L"true");
	}
	else if ((strName == L"fade_hot") || (strName == L"fadehot")) {
		GetAnimationManager().SetFadeHot(strValue == L"true");
	}
	else if ((strName == L"fade_width") || (strName == L"fadewidth")) {
		GetAnimationManager().SetFadeWidth(strValue == L"true");
	}
	else if ((strName == L"fade_height") || (strName == L"fadeheight")) {
		GetAnimationManager().SetFadeHeight(strValue == L"true");
	}
	else if ((strName == L"fade_in_out_x_from_left") || (strName == L"fadeinoutxfromleft")) {
		GetAnimationManager().SetFadeInOutX(strValue == L"true", false);
	}
	else if ((strName == L"fade_in_out_x_from_right") || (strName == L"fadeinoutxfromright")) {
		GetAnimationManager().SetFadeInOutX(strValue == L"true", true);
	}
	else if ((strName == L"fade_in_out_y_from_top") || (strName == L"fadeinoutyfromtop")) {
		GetAnimationManager().SetFadeInOutY(strValue == L"true", false);
	}
	else if ((strName == L"fade_in_out_y_from_bottom") || (strName == L"fadeinoutyfrombottom")) {
		GetAnimationManager().SetFadeInOutY(strValue == L"true", true);
	}
	else if ((strName == L"tab_stop") || (strName == L"tabstop")) {
		SetTabStop(strValue == L"true");
	}
	else if ((strName == L"loading_image") || (strName == L"loadingimage")) {
		SetLoadingImage(strValue);
	}
	else if ((strName == L"loading_bkcolor") || (strName == L"loadingbkcolor")){
		SetLoadingBkColor(strValue);
	}
	else {
		ASSERT(!"Control::SetAttributeʧ��: ���ֲ���ʶ�������");
	}
}

void Control::SetClass(const std::wstring& strClass)
{
	std::list<std::wstring> splitList = StringHelper::Split(strClass, L" ");
	for (auto it = splitList.begin(); it != splitList.end(); it++) {
		std::wstring pDefaultAttributes = GlobalManager::Instance().GetClassAttributes((*it));
		Window* pWindow = GetWindow();
		if (pDefaultAttributes.empty() && (pWindow != nullptr)) {
			pDefaultAttributes = pWindow->GetClassAttributes(*it);
		}

		ASSERT(!pDefaultAttributes.empty());
		if( !pDefaultAttributes.empty() ) {
			ApplyAttributeList(pDefaultAttributes);
		}
	}
}

void Control::ApplyAttributeList(const std::wstring& strList)
{
	//�����б��Ƚ�����Ȼ����Ӧ��
	std::vector<std::pair<std::wstring, std::wstring>> attributeList;
	AttributeUtil::ParseAttributeList(strList, L'\"', attributeList);
	for (const auto& attribute : attributeList) {
		SetAttribute(attribute.first, attribute.second);
	}
}

bool Control::OnApplyAttributeList(const std::wstring& strReceiver, const std::wstring& strList, const EventArgs& /*eventArgs*/)
{
	bool isFindSubControl = false;
	std::wstring receiverName = strReceiver;
	if (receiverName.size() >= 2) {
		if (receiverName.substr(0, 2) == L".\\" || receiverName.substr(0, 2) == L"./") {
			receiverName = receiverName.substr(2);
			isFindSubControl = true;
		}
	}
	Control* pReceiverControl = nullptr;
	if (isFindSubControl) {
		Box* pBox = dynamic_cast<Box*>(this);
		if (pBox != nullptr) {
			pReceiverControl = pBox->FindSubControl(receiverName);
		}
	}
	else {
		pReceiverControl = GetWindow()->FindControl(receiverName);
	}

	if (pReceiverControl != nullptr) {
		std::wstring strValueList = strList;
		//������ֹ�д������ԣ��Ի�����{}����˫���ţ���д��ʱ��Ͳ���Ҫת���ַ��ˣ�
		StringHelper::ReplaceAll(L"{", L"\"", strValueList);
		StringHelper::ReplaceAll(L"}", L"\"", strValueList);
		pReceiverControl->ApplyAttributeList(strValueList);
		return true;
	}
	else {
		ASSERT(!"Control::OnApplyAttributeList error!");
		return false;
	}	
}

bool Control::PaintImage(IRender* pRender,  Image& duiImage,
					    const std::wstring& strModify, int32_t nFade, IMatrix* pMatrix)
{
	//ע�⣺strModify������Ŀǰ�ⲿ�������Ҫ�ǣ�"destscale='false' dest='%d,%d,%d,%d'"
	//                   Ҳ��һ���ഫ���ˣ�L" corner='%d,%d,%d,%d'"��
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return false;
	}
	if (duiImage.GetImagePath().empty()) {
		return false;
	}
	LoadImageData(duiImage);
	std::shared_ptr<ImageInfo> imageInfo = duiImage.GetImageCache();
	ASSERT(imageInfo != nullptr);
	if (imageInfo == nullptr) {
		return false;
	}

	bool isPlayingGif = false;
	if ( m_bGifPlay && (m_pBkImage != nullptr) && !m_pBkImage->IsPlaying()   &&
		(m_pBkImage->GetImageCache() != nullptr) &&
		 m_pBkImage->GetImageCache()->IsMultiFrameImage()) {
		isPlayingGif = GifPlay();
	}
	if (isPlayingGif) {
		//������ڲ��ű����������򲻻�������ͼƬ
		return true;
	}

	IBitmap* pBitmap = duiImage.GetCurrentBitmap();
	ASSERT(pBitmap != nullptr);
	if (pBitmap == nullptr) {
		return false;
	}

	ImageAttribute newImageAttribute = duiImage.GetImageAttribute();
	if (!strModify.empty()) {
		newImageAttribute.ModifyAttribute(strModify);
	}
	bool hasDestAttr = false; // �ⲿ�Ƿ�������rcDest����
	UiRect rcDest = GetRect();
	if (ImageAttribute::HasValidImageRect(newImageAttribute.GetDestRect())) {
		//ʹ��������ָ����Ŀ������
		if ((newImageAttribute.GetDestRect().Width() <= rcDest.Width()) &&
			(newImageAttribute.GetDestRect().Height() <= rcDest.Height())) {
			rcDest = newImageAttribute.GetDestRect();
			rcDest.Offset(GetRect().left, GetRect().top);
			hasDestAttr = true;
		}
	}

	UiRect rcDestCorners;
	UiRect rcSource = newImageAttribute.GetSourceRect();
	UiRect rcSourceCorners = newImageAttribute.GetCorner();
	ImageAttribute::ScaleImageRect(pBitmap->GetWidth(), pBitmap->GetHeight(), 
								   imageInfo->IsBitmapSizeDpiScaled(),
		                           rcDestCorners,
		                           rcSource,
		                           rcSourceCorners);
	
	if (!hasDestAttr) {
		//����rcPadding��hAlign��vAlign ����ͼƬ����
		rcDest.Deflate(newImageAttribute.GetPadding());
		rcDest.Validate();
		rcSource.Validate();
		const int32_t imageWidth = rcSource.Width();
		const int32_t imageHeight = rcSource.Height();

		//Ӧ�ö��뷽ʽ��ͼƬ���������죬���ǰ�ԭ��Сչʾ
		if (!newImageAttribute.hAlign.empty()) {
			if (newImageAttribute.hAlign == L"left") {
				rcDest.right = rcDest.left + imageWidth;
			}
			else if (newImageAttribute.hAlign == L"center") {
				rcDest.left = rcDest.CenterX() - imageWidth / 2;
				rcDest.right = rcDest.left + imageWidth;
			}
			else if (newImageAttribute.hAlign == L"right") {
				rcDest.left = rcDest.right - imageWidth;
			}
			else {
				rcDest.right = rcDest.left + imageWidth;
			}

			if (newImageAttribute.vAlign.empty()) {
				rcDest.bottom = rcDest.top + imageHeight;
			}				
		}
		if (!newImageAttribute.vAlign.empty()) {
			if (newImageAttribute.vAlign == L"top") {
				rcDest.bottom = rcDest.top + imageHeight;
			}
			else if (newImageAttribute.vAlign == L"center") {
				rcDest.top = rcDest.CenterY() - imageHeight / 2;
				rcDest.bottom = rcDest.top + imageHeight;
			}
			else if (newImageAttribute.vAlign == L"right") {
				rcDest.top = rcDest.bottom - imageHeight;
			}
			else {
				rcDest.bottom = rcDest.top + imageHeight;
			}

			if (newImageAttribute.hAlign.empty()) {
				rcDest.right = rcDest.left + imageWidth;
			}
		}
	}

	//ͼƬ͸��������
	uint8_t iFade = (nFade == DUI_NOSET_VALUE) ? newImageAttribute.bFade : static_cast<uint8_t>(nFade);
	if (pMatrix != nullptr) {
		//�������: �Բ�֧�ֵ����ԣ����Ӷ��ԣ��������
		ASSERT(newImageAttribute.GetCorner().IsEmpty());
		ASSERT(!newImageAttribute.bTiledX);
		ASSERT(!newImageAttribute.bTiledY);
		pRender->DrawImageRect(m_rcPaint, pBitmap, rcDest, rcSource, iFade, pMatrix);
	}
	else{
		pRender->DrawImage(m_rcPaint, pBitmap, rcDest, rcDestCorners, rcSource, rcSourceCorners,
						   iFade, newImageAttribute.bTiledX, newImageAttribute.bTiledY, 
						   newImageAttribute.bFullTiledX, newImageAttribute.bFullTiledY,
						   newImageAttribute.nTiledMargin);
	}
	return true;
}

IRender* Control::GetRender()
{
	if (m_render == nullptr) {
		IRenderFactory* pRenderFactory = GlobalManager::Instance().GetRenderFactory();
		ASSERT(pRenderFactory != nullptr);
		if (pRenderFactory != nullptr) {
			m_render.reset(pRenderFactory->CreateRender());
		}
	}
	return m_render.get();
}


void Control::ClearRender()
{
	if (m_render) {
		m_render.reset();
	}
}

void Control::AlphaPaint(IRender* pRender, const UiRect& rcPaint)
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	if (m_nAlpha == 0) {
		//�ؼ���ȫ͸����������
		return;
	}

	//���Ƽ�������
	UiRect rcUnion;
	if (!UiRect::Intersect(rcUnion, rcPaint, GetRect())) {
		return;
	}

	//�Ƿ�ΪԲ�Ǿ�������ü�
	bool bRoundClip = ShouldBeRoundRectFill();

	//��ǰ�ؼ��Ƿ�������͸���ȣ�͸����ֵ����255��
	const bool isAlpha = IsAlpha();

	//�Ƿ�ʹ�û��ƻ���(�������box-shadow���Ͳ���ʹ�û��ƻ��棬��Ϊbox-shadow���Ƶ�ʱ���ǳ���GetRect�������ⲿ��Ӱ��)
	const bool isUseCache = IsUseCache() && !HasBoxShadow();

	if (isAlpha || isUseCache) {
		//�������򣨾ֲ����ƣ�
		UiRect rcUnionRect = rcUnion;
		if (isUseCache) {
			//���ʹ�û��ƻ��棬���Ƶ�ʱ�򣬱����������������Ϊ�ֲ�����ÿ��������Ƶ������ǲ�ͬ�ģ������б���ı����������Ļ���ͼ
			rcUnionRect = GetRect();
		}
		UiSize size{GetRect().Width(), GetRect().Height() };
		IRender* pCacheRender = GetRender();
		ASSERT(pCacheRender != nullptr);
		if (pCacheRender == nullptr) {
			return;
		}
		bool isSizeChanged = (size.cx != pCacheRender->GetWidth()) || (size.cy != pCacheRender->GetHeight());
		if (!pCacheRender->Resize(size.cx, size.cy)) {
			//���ڴ��󣬻���ʧ��
			ASSERT(!"pCacheRender->Resize failed!");
			return;
		}
		if (isSizeChanged) {
			//Render������С�����仯����Ҫ���û�������
			SetCacheDirty(true);
		}			
		if (IsCacheDirty()) {
			//���»��ƣ��������ԭ����
			pCacheRender->Clear();
			pCacheRender->SetRenderTransparent(true);

			UiPoint ptOffset(GetRect().left + m_renderOffset.x, GetRect().top + m_renderOffset.y);
			UiPoint ptOldOrg = pCacheRender->OffsetWindowOrg(ptOffset);

			bool hasBoxShadowPainted = HasBoxShadow();
			if (hasBoxShadowPainted) {
				//�Ȼ���box-shadow�����ܻᳬ��rect�߽����(���ʹ�òü������ܻ���ʾ��ȫ)
				m_isBoxShadowPainted = false;
				PaintShadow(pCacheRender);
				m_isBoxShadowPainted = true;
			}

			UiRect rcClip = { 0, 0, size.cx,size.cy};
			rcClip.Offset((GetRect().left + m_renderOffset.x), (GetRect().top + m_renderOffset.y));
			AutoClip alphaClip(pCacheRender, rcClip, IsClip());
			AutoClip roundAlphaClip(pCacheRender, rcClip, m_cxyBorderRound.cx, m_cxyBorderRound.cy, bRoundClip);		

			//���Ȼ����Լ�
			Paint(pCacheRender, rcUnionRect);
			if (hasBoxShadowPainted) {
				//Paint���ƺ�������λ��־������Ӱ�����������߼�
				m_isBoxShadowPainted = false;
			}
			if (isAlpha) {
				//������͸���ȣ���Ҫ�Ȼ����ӿؼ������Ƶ�pCacheRender���棩��Ȼ������AlphaBlend��pRender
				PaintChild(pCacheRender, rcUnionRect);
			}		
			pCacheRender->SetWindowOrg(ptOldOrg);
			SetCacheDirty(false);
		}

		pRender->AlphaBlend(rcUnionRect.left,
							rcUnionRect.top,
							rcUnionRect.Width(),
							rcUnionRect.Height(),
							pCacheRender,
							rcUnionRect.left - GetRect().left,
							rcUnionRect.top - GetRect().top,
							rcUnionRect.Width(),
							rcUnionRect.Height(),
				            static_cast<uint8_t>(m_nAlpha));
		if (!isAlpha) {
			//û������͸���ȣ�������ӿؼ���ֱ�ӻ��Ƶ�pRender���棩
			PaintChild(pRender, rcUnionRect);
		}
		if (isAlpha) {
			SetCacheDirty(true);
			m_render.reset();
		}
	}
	else {
		UiPoint ptOldOrg = pRender->OffsetWindowOrg(m_renderOffset);
		bool hasBoxShadowPainted = HasBoxShadow();
		if (hasBoxShadowPainted) {
			//�Ȼ���box-shadow�����ܻᳬ��rect�߽����(���ʹ�òü������ܻ���ʾ��ȫ)
			m_isBoxShadowPainted = false;
			PaintShadow(pRender);
			m_isBoxShadowPainted = true;
		}
		UiRect rcClip = GetRect();
		AutoClip clip(pRender, rcClip, IsClip());
		AutoClip roundClip(pRender, rcClip, m_cxyBorderRound.cx, m_cxyBorderRound.cy, bRoundClip);		
		Paint(pRender, rcPaint);
		if (hasBoxShadowPainted) {
			//Paint���ƺ�������λ��־������Ӱ�����������߼�
			m_isBoxShadowPainted = false;
		}
		PaintChild(pRender, rcPaint);
		pRender->SetWindowOrg(ptOldOrg);
	}
}

void Control::SetPaintRect(const UiRect& rect)
{ 
	m_rcPaint = rect; 
}

void Control::Paint(IRender* pRender, const UiRect& rcPaint)
{
	if (!UiRect::Intersect(m_rcPaint, rcPaint, GetRect())) {
		return;
	}	
	if (!m_isBoxShadowPainted) {
		//����box-shadow�����ܻᳬ��rect�߽����(���ʹ�òü������ܻ���ʾ��ȫ)
		PaintShadow(pRender);
	}	

	//������������
	PaintBkColor(pRender);		//������ɫ
	PaintBkImage(pRender);		//����ͼƬ����״̬
	PaintStateColors(pRender);	//�ؼ�ָ��״̬����ɫ����ͨ״̬������״̬������״̬������״̬
	PaintStateImages(pRender);	//�Ȼ��Ʊ���ͼƬ��Ȼ�����ǰ��ͼƬ��ÿ��ͼƬ��ָ����״̬����ͨ״̬������״̬������״̬������״̬
	PaintText(pRender);			//�����ı�
	PaintBorder(pRender);		//���Ʊ߿�
    PaintLoading(pRender);		//����LoadingͼƬ����״̬
}

void Control::PaintShadow(IRender* pRender)
{
	if (!HasBoxShadow()) {
		return;
	}
	BoxShadow boxShadow;
	if (m_pBoxShadow != nullptr) {
		boxShadow = *m_pBoxShadow;
	}

	ASSERT(pRender != nullptr);
	if (pRender != nullptr) {
		pRender->DrawBoxShadow(m_rcPaint,
							   m_cxyBorderRound,
							   boxShadow.m_cpOffset,
							   boxShadow.m_nBlurRadius,
							   boxShadow.m_nSpreadRadius,
							   GlobalManager::Instance().Color().GetColor(boxShadow.m_strColor));
	}	
}

void Control::PaintBkColor(IRender* pRender)
{
	if (m_strBkColor.empty()) {
		return;
	}
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}

	UiColor dwBackColor = GetUiColor(m_strBkColor.c_str());
	if(dwBackColor.GetARGB() != 0) {
		int32_t nBorderSize = 0;
		if ((m_rcBorderSize.left > 0) &&
			(m_rcBorderSize.left == m_rcBorderSize.right) &&
			(m_rcBorderSize.left == m_rcBorderSize.top) &&
			(m_rcBorderSize.left == m_rcBorderSize.bottom)) {
			//�ĸ��߶����ڣ��Ҵ�С��ͬ
			nBorderSize = m_rcBorderSize.left;
		}
		nBorderSize /= 2;

		//���������η�Χ
		UiRect fillRect = GetRect();
		if (nBorderSize > 0) {
			//������ڱ��ߣ�������ʱ�򣬲�����������λ�ã�������ֱ���ɫ�ľ������
			UiRect borderRect(nBorderSize, nBorderSize, nBorderSize, nBorderSize);
			fillRect.Deflate(borderRect.left, borderRect.top, borderRect.right, borderRect.bottom);
		}
		if (ShouldBeRoundRectFill()) {
			//��Ҫ����Բ�Ǿ��Σ����Ҳ��Ҫ���Բ�Ǿ���
			FillRoundRect(pRender, fillRect, m_cxyBorderRound, dwBackColor);
		}
		else {
			pRender->FillRect(fillRect, dwBackColor);
		}
	}
}

void Control::PaintBorder(IRender* pRender)
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	UiColor dwBorderColor((UiColor::ARGB)0);
	if (!m_strBorderColor.empty()) {
		dwBorderColor = GetUiColor(m_strBorderColor.c_str());
	}
	if (dwBorderColor.GetARGB() == 0) {
		return;
	}
	if ((m_rcBorderSize.left > 0) &&
		(m_rcBorderSize.left == m_rcBorderSize.right) &&
		(m_rcBorderSize.left == m_rcBorderSize.top) &&
		(m_rcBorderSize.left == m_rcBorderSize.bottom)) {
		//�ĸ��߶����ڣ��Ҵ�С��ͬ����ֱ�ӻ��ƾ���, ֧��Բ�Ǿ���
		PaintBorders(pRender, GetRect(), (int32_t)m_rcBorderSize.left, dwBorderColor);
	}
	else {
		//�ĸ��߷ֱ������û��Ʊ���
		if (m_rcBorderSize.left > 0) {
			UiRect rcBorder = GetRect();
			rcBorder.right = rcBorder.left = GetRect().left + m_rcBorderSize.left / 2;
			if (m_rcBorderSize.left == 1) {
				rcBorder.bottom -= 1;
			}
			pRender->DrawLine(UiPoint(rcBorder.left, rcBorder.top), UiPoint(rcBorder.right, rcBorder.bottom), dwBorderColor, m_rcBorderSize.left);
		}
		if (m_rcBorderSize.top > 0) {
			UiRect rcBorder = GetRect();
			rcBorder.bottom = rcBorder.top = GetRect().top + m_rcBorderSize.top / 2;
			if (m_rcBorderSize.top == 1) {
				rcBorder.right -= 1;
			}
			pRender->DrawLine(UiPoint(rcBorder.left, rcBorder.top), UiPoint(rcBorder.right, rcBorder.bottom), dwBorderColor, m_rcBorderSize.top);
		}
		if (m_rcBorderSize.right > 0) {
			UiRect rcBorder = GetRect();
			rcBorder.left = rcBorder.right = GetRect().right - (m_rcBorderSize.right + 1) / 2;
			if (m_rcBorderSize.right == 1) {
				rcBorder.bottom -= 1;
			}
			pRender->DrawLine(UiPoint(rcBorder.left, rcBorder.top), UiPoint(rcBorder.right, rcBorder.bottom), dwBorderColor, m_rcBorderSize.right);
		}
		if (m_rcBorderSize.bottom > 0) {
			UiRect rcBorder = GetRect();
			rcBorder.top = rcBorder.bottom = GetRect().bottom - (m_rcBorderSize.bottom + 1) / 2;
			if (m_rcBorderSize.bottom == 1) {
				rcBorder.right -= 1;
			}
			pRender->DrawLine(UiPoint(rcBorder.left, rcBorder.top), UiPoint(rcBorder.right, rcBorder.bottom), dwBorderColor, m_rcBorderSize.bottom);
		}
	}
}

void Control::PaintBorders(IRender* pRender, UiRect rcDraw,
	                       int32_t nBorderSize, UiColor dwBorderColor) const
{
	if ((pRender == nullptr) || rcDraw.IsEmpty() || (nBorderSize < 1) || (dwBorderColor.GetARGB() == 0)) {
		return;
	}
	int32_t nDeltaValue = nBorderSize / 2;
	rcDraw.top += nDeltaValue;
	rcDraw.bottom -= nDeltaValue;
	if (nBorderSize % 2 != 0) {
		rcDraw.bottom -= 1;
	}
	rcDraw.left += nDeltaValue;
	rcDraw.right -= nDeltaValue;
	if (nBorderSize % 2 != 0) {
		rcDraw.right -= 1;
	}
	if (ShouldBeRoundRectBorders()) {
		DrawRoundRect(pRender, rcDraw, m_cxyBorderRound, dwBorderColor, nBorderSize);
	}
	else {
		pRender->DrawRect(rcDraw, dwBorderColor, nBorderSize);
	}
}

bool Control::ShouldBeRoundRectFill() const
{
	bool isRoundRect = false;
	if ((m_rcBorderSize.left >= 0) &&
		(m_rcBorderSize.left == m_rcBorderSize.right) &&
		(m_rcBorderSize.left == m_rcBorderSize.top)   &&
		(m_rcBorderSize.left == m_rcBorderSize.bottom)) {
		//�ĸ��ߴ�С��ͬ(�������㻹�Ǵ�����)��֧��Բ�Ǿ���
		if (m_cxyBorderRound.cx > 0 && m_cxyBorderRound.cy > 0) {
			isRoundRect = true;
		}
	}
	if (isRoundRect) {
		//�������ʱ������Root Box�ؼ���������Բ�Ǳ���
		if (IsRootBox() && (GetWindow() != nullptr)) {
			if (GetWindow()->IsWindowMaximized()) {
				isRoundRect = false;
			}
		}
	}	
	return isRoundRect;
}

bool Control::ShouldBeRoundRectBorders() const
{
	bool isRoundRect = ShouldBeRoundRectFill();
	return isRoundRect && (m_rcBorderSize.left > 0);
}

bool Control::IsRootBox() const
{
	bool isRootBox = false;
	Window* pWindow = GetWindow();
	if (pWindow != nullptr) {
		Box* pRoot = pWindow->GetRoot();
		if ((Control*)pRoot == this) {
			//û��Attach����Ӱ�����
			isRootBox = true;
		}
		else if ((pRoot != nullptr) && pWindow->IsShadowAttached() && (pRoot->GetItemAt(0) == this)) {
			//�Ѿ�Attach����Ӱ�����
			isRootBox = true;
		}
	}
	return isRootBox;
}

bool Control::IsWindowRoundRect() const
{
	bool isWindowRoundRect = false;
	Window* pWindow = GetWindow();
	if (pWindow != nullptr) {
		UiSize roundCorner = pWindow->GetRoundCorner();
		if ((roundCorner.cx > 0) && (roundCorner.cy > 0)) {
			isWindowRoundRect = true;
		}
	}
	return isWindowRoundRect;
}

void Control::AddRoundRectPath(IPath* path, const UiRect& rc, const UiSize& roundSize) const
{
	ASSERT(path != nullptr);
	if (path == nullptr) {
		return;
	}
	//���ֻ�����Բ����״����CreateRoundRectRgn������Բ����״������һ�µ�
	path->AddArc(UiRect((INT)rc.left, rc.top, rc.left + roundSize.cx, rc.top + roundSize.cy), 180, 90);
	path->AddLine(rc.left + roundSize.cx / 2, (INT)rc.top, rc.right - roundSize.cx / 2, rc.top);
	path->AddArc(UiRect(rc.right - roundSize.cx, (INT)rc.top, rc.right, rc.top + roundSize.cy), 270, 90);
	path->AddLine((INT)rc.right, rc.top + roundSize.cy / 2, rc.right, rc.bottom - roundSize.cy / 2);
	path->AddArc(UiRect(rc.right - roundSize.cx, rc.bottom - roundSize.cy, rc.right, rc.bottom), 0, 90);
	path->AddLine(rc.right - roundSize.cx / 2, (INT)rc.bottom, rc.left + roundSize.cx / 2, rc.bottom);
	path->AddArc(UiRect(rc.left, rc.bottom - roundSize.cy, rc.left + roundSize.cx, rc.bottom), 90, 90);
	path->AddLine((INT)rc.left, rc.bottom - roundSize.cy / 2, rc.left, rc.top + roundSize.cy / 2);
	path->Close();
}

void Control::DrawRoundRect(IRender* pRender, const UiRect& rc, const UiSize& roundSize, UiColor dwBorderColor, int32_t nBorderSize) const
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	if (pRender->GetRenderType() != RenderType::kRenderType_Skia) {
		pRender->DrawRoundRect(rc, roundSize, dwBorderColor, nBorderSize);
		return;
	}
	bool isDrawOk = false;
	if (IsRootBox() && IsWindowRoundRect()) {
		//ʹ����Windowsһ�µĻ��Ʒ�ʽ��������Windows�Ĳ�һ��
		//�μ���Window::OnSizeMsg�е�CreateRoundRectRgn��Skia��Բ�ǻ�����CreateRoundRectRgn��һ����
		IRenderFactory* pRenderFactory = GlobalManager::Instance().GetRenderFactory();
		if (pRenderFactory != nullptr) {
			std::unique_ptr<IPen> pen(pRenderFactory->CreatePen(dwBorderColor, nBorderSize));
			std::unique_ptr<IPath> path(pRenderFactory->CreatePath());
			if (pen && path) {
				//���ֻ�����Բ����״����CreateRoundRectRgn������Բ����״������һ�µ�
				AddRoundRectPath(path.get(), rc, roundSize);
				pRender->DrawPath(path.get(), pen.get());
				isDrawOk = true;
			}
		}
	}
	if(!isDrawOk) {
		pRender->DrawRoundRect(rc, roundSize, dwBorderColor, nBorderSize);
	}
}

void Control::FillRoundRect(IRender* pRender, const UiRect& rc, const UiSize& roundSize, UiColor dwColor) const
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	if (pRender->GetRenderType() != RenderType::kRenderType_Skia) {
		pRender->FillRoundRect(rc, roundSize, dwColor);
		return;
	}

	bool isDrawOk = false;
	if (IsRootBox() && IsWindowRoundRect()) {
		//ʹ����Windowsһ�µĻ��Ʒ�ʽ��������Windows�Ĳ�һ��
		//�μ���Window::OnSizeMsg�е�CreateRoundRectRgn��Skia��Բ�ǻ�����CreateRoundRectRgn��һ����
		IRenderFactory* pRenderFactory = GlobalManager::Instance().GetRenderFactory();
		if (pRenderFactory != nullptr) {
			std::unique_ptr<IBrush> brush(pRenderFactory->CreateBrush(dwColor));
			std::unique_ptr<IPath> path(pRenderFactory->CreatePath());
			if (brush && path) {
				//���ֻ�����Բ����״����CreateRoundRectRgn������Բ����״������һ�µ�
				AddRoundRectPath(path.get(), rc, roundSize);
				pRender->FillPath(path.get(), brush.get());
				isDrawOk = true;
			}
		}
	}
	if (!isDrawOk) {
		pRender->FillRoundRect(rc, roundSize, dwColor);
	}	
}

void Control::PaintBkImage(IRender* pRender)
{
	if (m_pBkImage != nullptr) {
		PaintImage(pRender, *m_pBkImage);
	}	
}

void Control::PaintStateColor(IRender* pRender, const UiRect& rcPaint, ControlStateType stateType) const
{
	if (m_pColorMap != nullptr) {
		m_pColorMap->PaintStateColor(pRender, rcPaint, stateType);
	}
}

void Control::PaintStateColors(IRender* pRender)
{
	if (m_pColorMap != nullptr) {
		m_pColorMap->PaintStateColor(pRender, m_rcPaint, m_controlState);
	}	
}

void Control::PaintStateImages(IRender* pRender)
{
	if (m_pImageMap != nullptr) {
		m_pImageMap->PaintStateImage(pRender, kStateImageBk, m_controlState);
		m_pImageMap->PaintStateImage(pRender, kStateImageFore, m_controlState);
	}	
}

void Control::PaintText(IRender* /*pRender*/)
{
    return;
}

void Control::PaintLoading(IRender* pRender)
{
	if (m_pLoading != nullptr) {
		m_pLoading->PaintLoading(pRender);
	}
}

void Control::SetAlpha(int64_t alpha)
{
	ASSERT(alpha >= 0 && alpha <= 255);
	m_nAlpha = TruncateToInt32(alpha);
	Invalidate();
}

void Control::SetHotAlpha(int64_t nHotAlpha)
{
	ASSERT(nHotAlpha >= 0 && nHotAlpha <= 255);
	m_nHotAlpha = TruncateToInt32(nHotAlpha);
	Invalidate();
}

void Control::SetTabStop(bool enable)
{
	m_bAllowTabstop = enable;
}

void Control::SetRenderOffset(UiPoint renderOffset)
{
	m_renderOffset = renderOffset;
	Invalidate();
}

void Control::SetRenderOffsetX(int64_t renderOffsetX)
{
	m_renderOffset.x = TruncateToInt32(renderOffsetX);
	Invalidate();
}

void Control::SetRenderOffsetY(int64_t renderOffsetY)
{
	m_renderOffset.y = TruncateToInt32(renderOffsetY);
	Invalidate();
}

bool Control::GifPlay()
{
	if (m_pBkImage == nullptr) {
		return false;
	}
	if (!m_pBkImage->GetImageCache() || 
		!m_pBkImage->GetImageCache()->IsMultiFrameImage() || 
		!m_pBkImage->ContinuePlay()) {
		m_pBkImage->SetPlaying(false);
		m_gifWeakFlag.Cancel();
		return false;
	}

	if (!m_pBkImage->IsPlaying()) {
		m_pBkImage->SetCurrentFrame(0);
		m_gifWeakFlag.Cancel();
		int32_t timerInterval = m_pBkImage->GetCurrentInterval();//���ż��������
		if (timerInterval <= 0) {
			return false;
		}
		m_pBkImage->SetPlaying(true);
		auto gifPlayCallback = nbase::Bind(&Control::GifPlay, this);
		GlobalManager::Instance().Timer().AddCancelableTimer(m_gifWeakFlag.GetWeakFlag(),
													    gifPlayCallback,
														timerInterval, 
													    TimerManager::REPEAT_FOREVER);
	}
	else {
		int32_t preInterval = m_pBkImage->GetCurrentInterval();
		m_pBkImage->IncrementCurrentFrame();
		int32_t nowInterval = m_pBkImage->GetCurrentInterval();
		if (!m_pBkImage->ContinuePlay()) {
			StopGifPlayForUI(true, kGifStopLast);
		}
		else
		{
			if ((preInterval <= 0) || (nowInterval <= 0)) {
				m_pBkImage->SetPlaying(false);
				m_gifWeakFlag.Cancel();
				return false;
			}

			if (preInterval != nowInterval) {
				m_gifWeakFlag.Cancel();
				m_pBkImage->SetPlaying(true);
				auto gifPlayCallback = nbase::Bind(&Control::GifPlay, this);
				GlobalManager::Instance().Timer().AddCancelableTimer(m_gifWeakFlag.GetWeakFlag(),
																gifPlayCallback,
																nowInterval, 
															    TimerManager::REPEAT_FOREVER);
			}
		}			
	}
	Invalidate();
	return m_pBkImage->IsPlaying();
}

void Control::StopGifPlay(GifStopType frame)
{
	if ((m_pBkImage == nullptr) || (m_pBkImage->GetImageCache() == nullptr) ){
		return;
	}
	if (m_pBkImage->GetImageCache()->IsMultiFrameImage()) {
		m_pBkImage->SetPlaying(false);
		m_gifWeakFlag.Cancel();
		uint32_t index = GetGifFrameIndex(frame);
		m_pBkImage->SetCurrentFrame(index);
		Invalidate();
	}
}

void Control::StartGifPlayForUI(GifStopType frame, int32_t playcount)
{
	if (m_pBkImage == nullptr) {
		return;
	}
	LoadImageData(*m_pBkImage);
	if (!m_pBkImage->GetImageCache() || !m_pBkImage->GetImageCache()->IsMultiFrameImage()) {
		m_bGifPlay = false;
		m_pBkImage->SetPlaying(false);
		m_gifWeakFlag.Cancel();
		return;
	}
	if (playcount == 0)	{
		StopGifPlayForUI(false);
	}		
	else
	{
		m_gifWeakFlag.Cancel();
		m_bGifPlay = true;
		m_pBkImage->SetCurrentFrame(GetGifFrameIndex(frame));
		int32_t timerInterval = m_pBkImage->GetCurrentInterval();
		if (timerInterval <= 0) {
			m_bGifPlay = false;
			return;
		}
		m_pBkImage->SetPlaying(true);
		m_pBkImage->SetImagePlayCount(playcount);
		m_pBkImage->ClearCycledCount();
		auto gifPlayCallback = nbase::Bind(&Control::GifPlay, this);
		GlobalManager::Instance().Timer().AddCancelableTimer(m_gifWeakFlag.GetWeakFlag(),
													    gifPlayCallback,
													    timerInterval, 
													    TimerManager::REPEAT_FOREVER);
		Invalidate();
	}	
}

void Control::StopGifPlayForUI(bool transfer, GifStopType frame)
{
	m_bGifPlay = false;
	StopGifPlay(frame);
	if (transfer) {
		BroadcastGifEvent(m_nVirtualEventGifStop);
	}
}

uint32_t Control::GetGifFrameIndex(GifStopType frame)
{
	if ((m_pBkImage == nullptr) || (m_pBkImage->GetImageCache() == nullptr)) {
		return 0;
	}
	uint32_t ret = frame;
	switch (frame)
	{
	case kGifStopCurrent:
		ret = m_pBkImage->GetCurrentFrameIndex();
		break;
	case kGifStopFirst:
		ret = 0;
		break;
	case kGifStopLast:
	{
		uint32_t nFrameCount = m_pBkImage->GetImageCache()->GetFrameCount();
		ret = nFrameCount > 0 ? nFrameCount - 1 : 0;		
	}
	break;
	}
	return ret;
}
void Control::BroadcastGifEvent(int32_t nVirtualEvent)
{
	auto callback = m_OnGifEvent.find(nVirtualEvent);
	if (callback != m_OnGifEvent.end()) {
		EventArgs param;
		param.pSender = this;
		callback->second(param);
	}
}

bool Control::LoadImageData(Image& duiImage) const
{
	Window* pWindow = GetWindow();
	ASSERT(pWindow != nullptr);
	if (pWindow == nullptr) {
		return false;
	}

	std::wstring sImagePath = duiImage.GetImagePath();
	if (sImagePath.empty()) {
		return false;
	}
	std::wstring imageFullPath = GlobalManager::Instance().GetResFullPath(pWindow->GetResourcePath(), sImagePath);
	ImageLoadAttribute imageLoadAttr = duiImage.GetImageLoadAttribute();
	imageLoadAttr.SetImageFullPath(imageFullPath);
	std::shared_ptr<ImageInfo> imageCache = duiImage.GetImageCache();
	if ((imageCache == nullptr) || 
		(imageCache->GetCacheKey() != imageLoadAttr.GetCacheKey())) {
		//���ͼƬû�м�����ִ�м���ͼƬ�����ͼƬ�����仯�������¼��ظ�ͼƬ
		imageCache = GlobalManager::Instance().Image().GetImage(imageLoadAttr);
		duiImage.SetImageCache(imageCache);
	}
	return imageCache ? true : false;
}

void Control::InvokeLoadImageCache()
{
	if (m_loadBkImageWeakFlag.HasUsed()) {
		return;
	}
	if (m_pBkImage == nullptr) {
		return;
	}
	std::wstring sImagePath = m_pBkImage->GetImagePath();
	if (sImagePath.empty()) {
		return;
	}
	Window* pWindow = GetWindow();
	if (pWindow == nullptr) {
		return;
	}
	std::wstring imageFullPath = GlobalManager::Instance().GetResFullPath(pWindow->GetResourcePath(), sImagePath);
	ImageLoadAttribute imageLoadAttr = m_pBkImage->GetImageLoadAttribute();
	imageLoadAttr.SetImageFullPath(imageFullPath);
	if (!m_pBkImage->GetImageCache() || m_pBkImage->GetImageCache()->GetCacheKey() != imageLoadAttr.GetCacheKey()) {
		auto shared_image = GlobalManager::Instance().Image().GetCachedImage(imageLoadAttr);
		if (shared_image) {
			m_pBkImage->SetImageCache(shared_image);
			return;
		}
	}
}

void Control::UnLoadImageCache()
{
	m_loadBkImageWeakFlag.Cancel();
	if (m_pBkImage != nullptr) {
		m_pBkImage->ClearImageCache();
	}	
}

void Control::ClearImageCache()
{
	if (m_pImageMap != nullptr) {
		m_pImageMap->ClearImageCache();
	}
	if (m_pBkImage != nullptr) {
		m_pBkImage->ClearImageCache();
	}
}

void Control::AttachEvent(EventType type, const EventCallback& callback)
{
	if (m_pOnEvent == nullptr) {
		m_pOnEvent = new EventMap;
	}
	(*m_pOnEvent)[type] += callback;
	if ((type == kEventMouseMenu) || (type == kEventAll)) {
		SetContextMenuUsed(true);
	}
}

void Control::DetachEvent(EventType type)
{
	if (m_pOnEvent == nullptr) {
		return;
	}
	auto event = m_pOnEvent->find(type);
	if (event != m_pOnEvent->end()) {
		m_pOnEvent->erase(event);
	}
	if ((type == kEventMouseMenu) || (type == kEventAll)) {
		if ((m_pOnEvent->find(kEventAll) == m_pOnEvent->end()) &&
			(m_pOnEvent->find(kEventMouseMenu) == m_pOnEvent->end())) {
			SetContextMenuUsed(false);
		}
	}
}

void Control::AttachXmlEvent(EventType eventType, const EventCallback& callback)
{
	if (m_pOnXmlEvent == nullptr) {
		m_pOnXmlEvent = new EventMap;
	}
	(*m_pOnXmlEvent)[eventType] += callback; 
}

void Control::DetachXmlEvent(EventType type)
{
	if (m_pOnXmlEvent == nullptr) {
		return;
	}
	auto event = m_pOnXmlEvent->find(type);
	if (event != m_pOnXmlEvent->end()) {
		m_pOnXmlEvent->erase(event);
	}
}

void Control::AttachBubbledEvent(EventType eventType, const EventCallback& callback)
{
	if (m_pOnBubbledEvent == nullptr) {
		m_pOnBubbledEvent = new EventMap;
	}
	(*m_pOnBubbledEvent)[eventType] += callback;
}

void Control::DetachBubbledEvent(EventType eventType)
{
	if (m_pOnBubbledEvent == nullptr) {
		return;
	}
	auto event = m_pOnBubbledEvent->find(eventType);
	if (event != m_pOnBubbledEvent->end()) {
		m_pOnBubbledEvent->erase(eventType);
	}
}

void Control::AttachXmlBubbledEvent(EventType eventType, const EventCallback& callback)
{
	if (m_pOnXmlBubbledEvent == nullptr) {
		m_pOnXmlBubbledEvent = new EventMap;
	}
	(*m_pOnXmlBubbledEvent)[eventType] += callback;
}

void Control::DetachXmlBubbledEvent(EventType eventType)
{
	if (m_pOnXmlBubbledEvent == nullptr) {
		return;
	}
	auto event = m_pOnXmlBubbledEvent->find(eventType);
	if (event != m_pOnXmlBubbledEvent->end())	{
		m_pOnXmlBubbledEvent->erase(eventType);
	}
}

bool Control::FireAllEvents(const EventArgs& msg)
{
	std::weak_ptr<nbase::WeakFlag> weakflag = GetWeakFlag();
	bool bRet = true;//��ֵΪfalseʱ���Ͳ��ٵ��ûص������ʹ�����

	if (msg.pSender == this) {
		if (bRet && (m_pOnEvent != nullptr) && !m_pOnEvent->empty()) {
			auto callback = m_pOnEvent->find(msg.Type);
			if (callback != m_pOnEvent->end()) {
				bRet = callback->second(msg);
			}
			if (weakflag.expired()) {
				return false;
			}

			callback = m_pOnEvent->find(kEventAll);
			if (callback != m_pOnEvent->end()) {
				bRet = callback->second(msg);
			}
			if (weakflag.expired()) {
				return false;
			}
		}

		if (bRet && (m_pOnXmlEvent != nullptr) && !m_pOnXmlEvent->empty()) {
			auto callback = m_pOnXmlEvent->find(msg.Type);
			if (callback != m_pOnXmlEvent->end()) {
				bRet = callback->second(msg);
			}
			if (weakflag.expired()) {
				return false;
			}

			callback = m_pOnXmlEvent->find(kEventAll);
			if (callback != m_pOnXmlEvent->end()) {
				bRet = callback->second(msg);
			}
			if (weakflag.expired()) {
				return false;
			}
		}
	}

	if (bRet && (m_pOnBubbledEvent != nullptr) && !m_pOnBubbledEvent->empty()) {
		auto callback = m_pOnBubbledEvent->find(msg.Type);
		if (callback != m_pOnBubbledEvent->end()) {
			bRet = callback->second(msg);
		}
		if (weakflag.expired()) {
			return false;
		}

		callback = m_pOnBubbledEvent->find(kEventAll);
		if (callback != m_pOnBubbledEvent->end()) {
			bRet = callback->second(msg);
		}
		if (weakflag.expired()) {
			return false;
		}
	}

	if (bRet && (m_pOnXmlBubbledEvent != nullptr) && !m_pOnXmlBubbledEvent->empty()) {
		auto callback = m_pOnXmlBubbledEvent->find(msg.Type);
		if (callback != m_pOnXmlBubbledEvent->end()) {
			bRet = callback->second(msg);
		}
		if (weakflag.expired()) {
			return false;
		}

		callback = m_pOnXmlBubbledEvent->find(kEventAll);
		if (callback != m_pOnXmlBubbledEvent->end()) {
			bRet = callback->second(msg);
		}
		if (weakflag.expired()) {
			return false;
		}
	}
	return bRet && !weakflag.expired();
}

bool Control::HasUiColor(const std::wstring& colorName) const
{
	UiColor color = GetUiColorByName(colorName);
	return color.GetARGB() != 0;
}

UiColor Control::GetUiColor(const std::wstring& colorName) const
{
	UiColor color = GetUiColorByName(colorName);
	ASSERT(color.GetARGB() != 0);
	return color;
}

UiColor Control::GetUiColorByName(const std::wstring& colorName) const
{
	UiColor color;
	if (colorName.empty()) {
		return color;
	}
	if (colorName.at(0) == L'#') {
		//���ȼ�1����'#'�ַ���ͷ��ֱ��ָ����ɫֵ��������#FFFFFFFF
		color = ColorManager::ConvertToUiColor(colorName);
	}
	if (color.GetARGB() == 0) {
		Window* pWindow = GetWindow();
		if (pWindow != nullptr) {
			//���ȼ�2����ȡ������XML�е�<Window>�ڵ��ж����ӽڵ�<TextColor>ָ������ɫ
			color = pWindow->GetTextColor(colorName);
		}
	}
	if (color.GetARGB() == 0) {
		//���ȼ�3����ȡ��global.xml�е�<Global>�ڵ��ж����ӽڵ�<TextColor>ָ������ɫ
		color = GlobalManager::Instance().Color().GetColor(colorName);
	}
	if (color.GetARGB() == 0) {
		//���ȼ�4��ֱ��ָ��Ԥ�������ɫ����
		color = GlobalManager::Instance().Color().GetStandardColor(colorName);
	}
	ASSERT(color.GetARGB() != 0);
	return color;
}

bool Control::HasBoxShadow() const
{
	if (m_pBoxShadow != nullptr) {
		return m_pBoxShadow->HasShadow();
	}
	return false;
}

bool Control::IsSelectableType() const
{
	return false;
}

bool Control::IsWantTab() const
{
	return false;
}

bool Control::CanPlaceCaptionBar() const
{
	return false;
}

bool Control::CheckVisibleAncestor(void) const
{
	bool isVisible = IsVisible();
	if (isVisible) {
		Control* parent = GetParent();
		while (parent != nullptr)
		{
			if (!parent->IsVisible()) {
				isVisible = false;
				break;
			}
			parent = parent->GetParent();
		}
	}
	return isVisible;
}

void Control::EnsureNoFocus()
{
	Window* pWindow = GetWindow();
	if ((pWindow != nullptr) && pWindow->GetFocus() != nullptr) {
		if (pWindow->GetFocus() == this) {
			pWindow->SetFocus(nullptr);
		}
		/*
		else if (IsChild(this, pWindow->GetFocus())) {
			pWindow->SetFocus(nullptr);
		}
		*/
	}
}

} // namespace ui
