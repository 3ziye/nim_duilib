#include "ScrollBox.h"
#include "duilib/Render/IRender.h"
#include "duilib/Render/AutoClip.h"
#include "duilib/Core/Window.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Utils/AttributeUtil.h"

namespace ui
{
ScrollBox::ScrollBox(Layout* pLayout) :
	Box(pLayout),
	m_pVScrollBar(),
	m_pHScrollBar(),
	m_bScrollProcess(false),
	m_bScrollBarFloat(true),
	m_bVScrollBarLeftPos(false),
	m_bHoldEnd(false),
	m_rcScrollBarPadding()
{
	m_nVScrollUnitPixels = GlobalManager::Instance().Dpi().GetScaleInt(m_nVScrollUnitPixelsDefault);
	m_nHScrollUnitPixels = GlobalManager::Instance().Dpi().GetScaleInt(m_nHScrollUnitPixelsDefault);
	m_scrollAnimation = std::make_unique<AnimationPlayer>();
	m_renderOffsetYAnimation = std::make_unique<AnimationPlayer>();
}

std::wstring ScrollBox::GetType() const { return DUI_CTR_SCROLLBOX; }//ScrollBox

void ScrollBox::SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue)
{
	if (pstrName == L"vscrollbar") {
		EnableScrollBar(pstrValue == L"true", GetHScrollBar() != nullptr);
	}
	else if ((pstrName == L"vscrollbar_style") || (pstrName == L"vscrollbarstyle")) {
		EnableScrollBar(true, GetHScrollBar() != nullptr);
		if (GetVScrollBar() != nullptr) {
			GetVScrollBar()->ApplyAttributeList(pstrValue);
		}
	}
	else if (pstrName == L"hscrollbar") {
		EnableScrollBar(GetVScrollBar() != nullptr, pstrValue == L"true");
	}
	else if ((pstrName == L"hscrollbar_style") || (pstrName == L"hscrollbarstyle")) {
		EnableScrollBar(GetVScrollBar() != nullptr, true);
		if (GetHScrollBar() != nullptr) {
			GetHScrollBar()->ApplyAttributeList(pstrValue);
		}
	}
	else if ((pstrName == L"scrollbar_padding") || (pstrName == L"scrollbarpadding")) {
		UiRect rcScrollbarPadding;
		AttributeUtil::ParseRectValue(pstrValue.c_str(), rcScrollbarPadding);
		SetScrollBarPadding(rcScrollbarPadding);
	}
	else if ((pstrName == L"vscroll_unit") || (pstrName == L"vscrollunit")) {
		SetVerScrollUnitPixels(_wtoi(pstrValue.c_str()));
	}
	else if ((pstrName == L"hscroll_unit") || (pstrName == L"hscrollunit")) {
		SetHorScrollUnitPixels(_wtoi(pstrValue.c_str()));
	}
	else if ((pstrName == L"scrollbar_float") || (pstrName == L"scrollbarfloat")) {
		SetScrollBarFloat(pstrValue == L"true");
	}
	else if ((pstrName == L"vscrollbar_left") || (pstrName == L"vscrollbarleft")) {
		SetVScrollBarLeftPos(pstrValue == L"true");
	}
	else if ((pstrName == L"hold_end") || (pstrName == L"holdend")) {
		SetHoldEnd(pstrValue == L"true");
	}
	else {
		Box::SetAttribute(pstrName, pstrValue);
	}
}

void ScrollBox::SetPos(UiRect rc)
{
	bool bEndDown = false;
	if (IsHoldEnd() && IsVScrollBarValid() && GetScrollRange().cy - GetScrollPos().cy == 0) {
		bEndDown = true;
	}
	SetPosInternally(rc);
	if (bEndDown && IsVScrollBarValid()) {
		EndDown(false, false);
	}
}

void ScrollBox::SetPosInternally(UiRect rc)
{
	Layout* pLayout = GetLayout();
	ASSERT(pLayout != nullptr);
	Control::SetPos(rc);
	UiRect rcRaw = rc;
	rc.left += pLayout->GetPadding().left;
	rc.top += pLayout->GetPadding().top;
	rc.right -= pLayout->GetPadding().right;
	rc.bottom -= pLayout->GetPadding().bottom;

	UiSize64 requiredSize = CalcRequiredSize(rc);
	ProcessVScrollBar(rcRaw, requiredSize.cy);
	ProcessHScrollBar(rcRaw, requiredSize.cx);
}

UiSize64 ScrollBox::CalcRequiredSize(const UiRect& rc)
{
	UiSize64 requiredSize;
	if (!m_items.empty()) {
		UiRect childSize = rc;
		if (!m_bScrollBarFloat && m_pVScrollBar && m_pVScrollBar->IsValid()) {
			if (m_bVScrollBarLeftPos) {
				ASSERT(m_pVScrollBar->GetFixedWidth().GetInt32() > 0);
				childSize.left += m_pVScrollBar->GetFixedWidth().GetInt32();
			}
			else {
				ASSERT(m_pVScrollBar->GetFixedWidth().GetInt32() > 0);
				childSize.right -= m_pVScrollBar->GetFixedWidth().GetInt32();
			}
		}
		if (!m_bScrollBarFloat && m_pHScrollBar && m_pHScrollBar->IsValid()) {
			ASSERT(m_pHScrollBar->GetFixedHeight().GetInt32() > 0);
			childSize.bottom -= m_pHScrollBar->GetFixedHeight().GetInt32();
		}
		requiredSize = GetLayout()->ArrangeChild(m_items, childSize);
	}
	return requiredSize;
}

void ScrollBox::HandleEvent(const EventArgs& event)
{
	if( (!IsMouseEnabled() && (event.Type > kEventMouseBegin) && (event.Type < kEventMouseEnd)) || 
		(event.Type == kEventLast)) {
		if (GetParent() != nullptr) {
			GetParent()->SendEvent(event);
		}
		else {
			Box::HandleEvent(event);
		}
		return;
	}
		
	if( (m_pVScrollBar != nullptr) && m_pVScrollBar->IsValid() && m_pVScrollBar->IsEnabled() ) {
		if( event.Type == kEventKeyDown ) {
			switch( event.chKey ) {
			case VK_DOWN:
				LineDown();
				return;
			case VK_UP:
				LineUp();
				return;
			case VK_NEXT:
				PageDown();
				return;
			case VK_PRIOR:
				PageUp();
				return;
			case VK_HOME:
				HomeUp();
				return;
			case VK_END:
				EndDown();
				return;
			}
		}
		else if( event.Type == kEventMouseWheel ) {
			int deltaValue = static_cast<int>(event.wParam);
			if (event.lParam != 0) {
				//�����߼�����
				if (deltaValue > 0) {
					LineUp(abs(deltaValue));
				}
				else {
					LineDown(abs(deltaValue));
				}
			}
			else {
				//�ϸ��մ����������
				if (deltaValue > 0) {
					TouchUp(abs(deltaValue));
				}
				else {
					TouchDown(abs(deltaValue));
				}
			}
			return;
		}		
	}
	else if( (m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid() && m_pHScrollBar->IsEnabled() ) {
		if( event.Type == kEventKeyDown ) {
			switch( event.chKey ) {
			case VK_DOWN:
				LineRight();
				return;
			case VK_UP:
				LineLeft();
				return;
			case VK_NEXT:
				PageRight();
				return;
			case VK_PRIOR:
				PageLeft();
				return;
			case VK_HOME:
				HomeLeft();
				return;
			case VK_END:
				EndRight();
				return;
			}
		}
		else if( event.Type == kEventMouseWheel )	{
			int deltaValue = static_cast<int>(event.wParam);
			if (deltaValue > 0 ) {
				LineLeft();
				return;
			}
			else {
				LineRight();
				return;
			}
		}
	}
		
	Box::HandleEvent(event);
}

bool ScrollBox::MouseEnter(const EventArgs& msg)
{
	bool bRet = __super::MouseEnter(msg);
	if (bRet && (m_pVScrollBar != nullptr) && m_pVScrollBar->IsValid() && m_pVScrollBar->IsEnabled()) {
		if (m_pVScrollBar->IsAutoHideScroll()) {
			m_pVScrollBar->SetFadeVisible(true);
		}
	}
	if (bRet && (m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid() && m_pHScrollBar->IsEnabled()) {
		if (m_pHScrollBar->IsAutoHideScroll()) {
			m_pHScrollBar->SetFadeVisible(true);
		}
	}
	return bRet;
}

bool ScrollBox::MouseLeave(const EventArgs& msg)
{
	bool bRet = __super::MouseLeave(msg);
	if (bRet && (m_pVScrollBar != nullptr) && m_pVScrollBar->IsValid() && m_pVScrollBar->IsEnabled()) {
		if ((m_pVScrollBar->GetThumbState() == kControlStateNormal) && 
			 m_pVScrollBar->IsAutoHideScroll()) {
			m_pVScrollBar->SetFadeVisible(false);
		}
	}
	if (bRet && (m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid() && m_pHScrollBar->IsEnabled()) {
		if ((m_pHScrollBar->GetThumbState() == kControlStateNormal) && 
			 m_pHScrollBar->IsAutoHideScroll()) {
			m_pHScrollBar->SetFadeVisible(false);
		}
	}

	return bRet;
}

void ScrollBox::PaintChild(IRender* pRender, const UiRect& rcPaint)
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	UiRect rcTemp;
	if (!UiRect::Intersect(rcTemp, rcPaint, GetRect())) {
		return;
	}

	for (Control* pControl : m_items) {
		if (pControl == nullptr) {
			continue;
		}
		if (!pControl->IsVisible()) {
			continue;
		}
		if (pControl->IsFloat()) {
			pControl->AlphaPaint(pRender, rcPaint);	
		}
		else {
			UiSize scrollPos = GetScrollOffset();
			UiRect rcNewPaint = GetPaddingPos();
			AutoClip alphaClip(pRender, rcNewPaint, IsClip());
			rcNewPaint.Offset(scrollPos.cx, scrollPos.cy);
			rcNewPaint.Offset(GetRenderOffset().x, GetRenderOffset().y);

			UiPoint ptOffset(scrollPos.cx, scrollPos.cy);
			UiPoint ptOldOrg = pRender->OffsetWindowOrg(ptOffset);
			pControl->AlphaPaint(pRender, rcNewPaint);
			pRender->SetWindowOrg(ptOldOrg);
		}
	}

	if( (m_pHScrollBar != nullptr) && m_pHScrollBar->IsVisible()) {
		m_pHScrollBar->AlphaPaint(pRender, rcPaint);
	}
		
	if( (m_pVScrollBar != nullptr) && m_pVScrollBar->IsVisible()) {
		m_pVScrollBar->AlphaPaint(pRender, rcPaint);
	}
	
	static bool bFirstPaint = true;//TODO: static�������Ľ�
	if (bFirstPaint) {
		bFirstPaint = false;
		LoadImageCache(true);
	}
}

void ScrollBox::SetMouseEnabled(bool bEnabled)
{
	if (m_pVScrollBar != nullptr) {
		m_pVScrollBar->SetMouseEnabled(bEnabled);
	}
	if (m_pHScrollBar != nullptr) {
		m_pHScrollBar->SetMouseEnabled(bEnabled);
	}
	Box::SetMouseEnabled(bEnabled);
}

void ScrollBox::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	if (m_pVScrollBar != nullptr) {
		m_pVScrollBar->SetWindow(pManager, this, bInit);
	}
	if (m_pHScrollBar != nullptr) {
		m_pHScrollBar->SetWindow(pManager, this, bInit);
	}
	Box::SetWindow(pManager, pParent, bInit);
}

Control* ScrollBox::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint /*scrollPos*/)
{
	// Check if this guy is valid
	if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) {
		return nullptr;
	}
	if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) {
		return nullptr;
	}
	if ((uFlags & UIFIND_HITTEST) != 0) {
		ASSERT(pData != nullptr);
		if (pData == nullptr) {
			return nullptr;
		}
		UiPoint pt(*(static_cast<UiPoint*>(pData)));
		if (!GetRect().ContainsPt(pt)) {
			return nullptr;
		}
		if (!IsMouseChildEnabled()) {
			Control* pResult = nullptr;
			if (m_pVScrollBar != nullptr) {
				pResult = m_pVScrollBar->FindControl(Proc, pData, uFlags);
			}
			if (pResult == nullptr && m_pHScrollBar != nullptr) {
				pResult = m_pHScrollBar->FindControl(Proc, pData, uFlags);
			}
			if (pResult == nullptr) {
				pResult = Control::FindControl(Proc, pData, uFlags);
			}
			return pResult;
		}
	}

	Control* pResult = nullptr;
	if (m_pVScrollBar != nullptr) {
		pResult = m_pVScrollBar->FindControl(Proc, pData, uFlags);
	}
	if (pResult == nullptr && m_pHScrollBar != nullptr) {
		pResult = m_pHScrollBar->FindControl(Proc, pData, uFlags);
	}
	if (pResult != nullptr) {
		return pResult;
	}

	UiSize scrollPos = GetScrollOffset();
	UiPoint ptNewScrollPos(scrollPos.cx, scrollPos.cy);
	return Box::FindControl(Proc, pData, uFlags, ptNewScrollPos);
}

UiSize64 ScrollBox::GetScrollPos() const
{
	UiSize64 sz;
	if ((m_pVScrollBar != nullptr) && m_pVScrollBar->IsValid()) {
		sz.cy = m_pVScrollBar->GetScrollPos();
	}
	if ((m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid()) {
		sz.cx = m_pHScrollBar->GetScrollPos();
	}
	return sz;
}

UiSize64 ScrollBox::GetScrollRange() const
{
	UiSize64 sz;
	if ((m_pVScrollBar != nullptr) && m_pVScrollBar->IsValid()) {
		sz.cy = m_pVScrollBar->GetScrollRange();
	}
	if ((m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid()) {
		sz.cx = m_pHScrollBar->GetScrollRange();
	}
	return sz;
}

void ScrollBox::SetScrollPos(UiSize64 szPos)
{
	if (szPos.cy < 0) {
		szPos.cy = 0;
		m_scrollAnimation->Reset();
	}
	else if (szPos.cy > GetScrollRange().cy) {
		szPos.cy = GetScrollRange().cy;
		m_scrollAnimation->Reset();
	}

	int64_t cx = 0;
	int64_t cy = 0;
	if( (m_pVScrollBar != nullptr) && m_pVScrollBar->IsValid() ) {
		int64_t iLastScrollPos = m_pVScrollBar->GetScrollPos();
		m_pVScrollBar->SetScrollPos(szPos.cy);
		cy = m_pVScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( (m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid() ) {
		int64_t iLastScrollPos = m_pHScrollBar->GetScrollPos();
		m_pHScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if (cx == 0 && cy == 0) {
		return;
	}
	LoadImageCache(cy > 0);
	Invalidate();
	SendEvent(kEventScrollChange, (cy == 0) ? 0 : 1, (cx == 0) ? 0 : 1);
}

void ScrollBox::LoadImageCache(bool bFromTopLeft)
{
	UiSize scrollPos = GetScrollOffset();
	UiRect rcImageCachePos = GetPos();
	rcImageCachePos.Offset(scrollPos.cx, scrollPos.cy);
	rcImageCachePos.Offset(GetRenderOffset().x, GetRenderOffset().y);
	rcImageCachePos.Inflate(0, 730, 0, 730);//TODO: �������߼�

	auto forEach = [this, scrollPos, rcImageCachePos](ui::Control* pControl) {
		if (pControl == nullptr) {
			return;
		}
		if (!pControl->IsVisible()) {
			return;
		}
		if (pControl->IsFloat()) {
			return;
		}
		UiRect rcTemp;
		UiRect controlPos = pControl->GetPos();
		if (!UiRect::Intersect(rcTemp, rcImageCachePos, controlPos)) {
			pControl->UnLoadImageCache();
		}
		else {
			pControl->InvokeLoadImageCache();
		}
	};

	if (!bFromTopLeft) {
		std::for_each(m_items.rbegin(), m_items.rend(), forEach);
	}
	else {
		std::for_each(m_items.begin(), m_items.end(), forEach);
	}
}

void ScrollBox::SetScrollPosY(int64_t y)
{
	UiSize64 scrollPos = GetScrollPos();
	scrollPos.cy = y;
	SetScrollPos(scrollPos);
}

void ScrollBox::SetScrollPosX(int64_t x)
{
	UiSize64 scrollPos = GetScrollPos();
    scrollPos.cx = x;
    SetScrollPos(scrollPos);
}

void ScrollBox::LineUp(int deltaValue, bool withAnimation)
{
	int cyLine = GetVerScrollUnitPixels();
	if (cyLine == 0) {
		cyLine = GlobalManager::Instance().Dpi().GetScaleInt(m_nVScrollUnitPixelsDefault);
	}
	if (deltaValue != DUI_NOSET_VALUE) {
		cyLine = std::min(cyLine, deltaValue);
	}

	UiSize64 scrollPos = GetScrollPos();
	if (scrollPos.cy <= 0) {
		return;
	}

	if (!withAnimation) {
		scrollPos.cy -= cyLine;
		if (scrollPos.cy < 0)
		{
			scrollPos.cy = 0;
		}
		SetScrollPos(scrollPos);
	}
	else {
		//TODO:
		m_scrollAnimation->SetStartValue(scrollPos.cy);
		if (m_scrollAnimation->IsPlaying()) {
			if (m_scrollAnimation->GetEndValue() > m_scrollAnimation->GetStartValue()) {
				m_scrollAnimation->SetEndValue(scrollPos.cy - cyLine);
			}
			else {
				m_scrollAnimation->SetEndValue(m_scrollAnimation->GetEndValue() - cyLine);
			}
		}
		else {
			m_scrollAnimation->SetEndValue(scrollPos.cy - cyLine);
		}
		m_scrollAnimation->SetSpeedUpRatio(0);
		m_scrollAnimation->SetSpeedDownfactorA(-0.012);
		m_scrollAnimation->SetSpeedDownRatio(0.5);
		m_scrollAnimation->SetTotalMillSeconds(DUI_NOSET_VALUE);
		m_scrollAnimation->SetCallback(nbase::Bind(&ScrollBox::SetScrollPosY, this, std::placeholders::_1));
		m_scrollAnimation->Start();
	}
}

void ScrollBox::LineDown(int deltaValue, bool withAnimation)
{
	int cyLine = GetVerScrollUnitPixels();
	if (cyLine == 0) {
		cyLine = GlobalManager::Instance().Dpi().GetScaleInt(m_nVScrollUnitPixelsDefault);
	}
	if (deltaValue != DUI_NOSET_VALUE) {
		cyLine = std::min(cyLine, deltaValue);
	}

	UiSize64 scrollPos = GetScrollPos();
	if (scrollPos.cy >= GetScrollRange().cy) {
		return;
	}

	if (!withAnimation) {
		scrollPos.cy += cyLine;
		if (scrollPos.cy < 0)
		{
			scrollPos.cy = 0;
		}
		SetScrollPos(scrollPos);
	}
	else {
		m_scrollAnimation->SetStartValue(scrollPos.cy);
		if (m_scrollAnimation->IsPlaying()) {
			if (m_scrollAnimation->GetEndValue() < m_scrollAnimation->GetStartValue()) {
				m_scrollAnimation->SetEndValue(scrollPos.cy + cyLine);
			}
			else {
				m_scrollAnimation->SetEndValue(m_scrollAnimation->GetEndValue() + cyLine);
			}
		}
		else {
			m_scrollAnimation->SetEndValue(scrollPos.cy + cyLine);
		}
		m_scrollAnimation->SetSpeedUpRatio(0);
		m_scrollAnimation->SetSpeedDownfactorA(-0.012);
		m_scrollAnimation->SetSpeedDownRatio(0.5);
		m_scrollAnimation->SetTotalMillSeconds(DUI_NOSET_VALUE);
		m_scrollAnimation->SetCallback(nbase::Bind(&ScrollBox::SetScrollPosY, this, std::placeholders::_1));
		m_scrollAnimation->Start();
	}
}
void ScrollBox::LineLeft(int detaValue)
{
    int cxLine = GetHorScrollUnitPixels();
    if (cxLine == 0) {
        cxLine = GlobalManager::Instance().Dpi().GetScaleInt(m_nHScrollUnitPixelsDefault);
    }
    if (detaValue != DUI_NOSET_VALUE) {
        cxLine = std::min(cxLine, detaValue);
    }

    UiSize64 scrollPos = GetScrollPos();
    if (scrollPos.cx <= 0) {
        return;
    }
    scrollPos.cx -= cxLine;
    if (scrollPos.cx < 0)
    {
        scrollPos.cx = 0;
    }
    SetScrollPos(scrollPos);
    /*m_scrollAnimation.SetStartValue(scrollPos.cx);
    if (m_scrollAnimation.IsPlaying()) {
        if (m_scrollAnimation.GetEndValue() > m_scrollAnimation.GetStartValue()) {
            m_scrollAnimation.SetEndValue(scrollPos.cx - cxLine);
        }
        else {
            m_scrollAnimation.SetEndValue(m_scrollAnimation.GetEndValue() - cxLine);
        }
    }
    else {
        m_scrollAnimation.SetEndValue(scrollPos.cx - cxLine);
    }
    m_scrollAnimation.SetSpeedUpRatio(0);
    m_scrollAnimation.SetSpeedDownfactorA(-0.012);
    m_scrollAnimation.SetSpeedDownRatio(0.5);
    m_scrollAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
    m_scrollAnimation.SetCallback(nbase::Bind(&ScrollBox::SetScrollPosX, this, std::placeholders::_1));
    m_scrollAnimation.Start();*/

}

void ScrollBox::LineRight(int detaValue)
{
    int cxLine = GetHorScrollUnitPixels();
    if (cxLine == 0) {
        cxLine = GlobalManager::Instance().Dpi().GetScaleInt(m_nHScrollUnitPixelsDefault);
    }
    if (detaValue != DUI_NOSET_VALUE) {
        cxLine = std::min(cxLine, detaValue);
    }

    UiSize64 scrollPos = GetScrollPos();
    if (scrollPos.cx >= GetScrollRange().cx) {
        return;
    }
    scrollPos.cx += cxLine;
    if (scrollPos.cx > GetScrollRange().cx)
    {
        scrollPos.cx = GetScrollRange().cx;
    }
    SetScrollPos(scrollPos);
    //m_scrollAnimation.SetStartValue(scrollPos.cx);
    //if (m_scrollAnimation.IsPlaying()) {
    //    if (m_scrollAnimation.GetEndValue() < m_scrollAnimation.GetStartValue()) {
    //        m_scrollAnimation.SetEndValue(scrollPos.cx + cxLine);
    //    }
    //    else {
    //        m_scrollAnimation.SetEndValue(m_scrollAnimation.GetEndValue() + cxLine);
    //    }
    //}
    //else {
    //    m_scrollAnimation.SetEndValue(scrollPos.cx + cxLine);
    //}
    //m_scrollAnimation.SetSpeedUpRatio(0);
    //m_scrollAnimation.SetSpeedDownfactorA(-0.012);
    //m_scrollAnimation.SetSpeedDownRatio(0.5);
    //m_scrollAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
    //m_scrollAnimation.SetCallback(nbase::Bind(&ScrollBox::SetScrollPosX, this, std::placeholders::_1));
    //m_scrollAnimation.Start();
}
void ScrollBox::PageUp()
{
	UiSize64 sz = GetScrollPos();
	int iOffset = GetRect().bottom - GetRect().top - GetLayout()->GetPadding().top - GetLayout()->GetPadding().bottom;
	if (m_pHScrollBar && m_pHScrollBar->IsValid()) {
		ASSERT(m_pHScrollBar->GetFixedHeight().GetInt32() > 0);
		iOffset -= m_pHScrollBar->GetFixedHeight().GetInt32();
	}
	sz.cy -= iOffset;
	SetScrollPos(sz);
}

void ScrollBox::PageDown()
{
	UiSize64 sz = GetScrollPos();
	int iOffset = GetRect().bottom - GetRect().top - GetLayout()->GetPadding().top - GetLayout()->GetPadding().bottom;
	if ((m_pHScrollBar != nullptr) && m_pHScrollBar->IsValid()) {
		ASSERT(m_pHScrollBar->GetFixedHeight().GetInt32() > 0);
		iOffset -= m_pHScrollBar->GetFixedHeight().GetInt32();
	}
	sz.cy += iOffset;
	SetScrollPos(sz);
}

void ScrollBox::HomeUp()
{
	UiSize64 sz = GetScrollPos();
	sz.cy = 0;
	SetScrollPos(sz);
}

void ScrollBox::EndDown(bool arrange, bool withAnimation)
{
	if (arrange) {
		SetPosInternally(GetPos());
	}
	
	int64_t renderOffsetY = GetScrollRange().cy - GetScrollPos().cy + (m_renderOffsetYAnimation->GetEndValue() - GetRenderOffset().y);
	if (withAnimation == true && IsVScrollBarValid() && renderOffsetY > 0) {
		PlayRenderOffsetYAnimation(-renderOffsetY);
	}

	UiSize64 sz = GetScrollPos();
	sz.cy = GetScrollRange().cy;
	SetScrollPos(sz);
}

void ScrollBox::PageLeft()
{
	UiSize64 sz = GetScrollPos();
	int iOffset = GetRect().right - GetRect().left - GetLayout()->GetPadding().left - GetLayout()->GetPadding().right;
	//if( m_pVScrollBar && m_pVScrollBar->IsValid() ) iOffset -= m_pVScrollBar->GetFixedWidth();
	sz.cx -= iOffset;
	SetScrollPos(sz);
}

void ScrollBox::PageRight()
{
	UiSize64 sz = GetScrollPos();
	int iOffset = GetRect().right - GetRect().left - GetLayout()->GetPadding().left - GetLayout()->GetPadding().right;
	//if( m_pVScrollBar && m_pVScrollBar->IsValid() ) iOffset -= m_pVScrollBar->GetFixedWidth();
	sz.cx += iOffset;
	SetScrollPos(sz);
}

void ScrollBox::HomeLeft()
{
	UiSize64 sz = GetScrollPos();
	sz.cx = 0;
	SetScrollPos(sz);
}

void ScrollBox::EndRight()
{
	UiSize64 sz = GetScrollPos();
	sz.cx = GetScrollRange().cx;
	SetScrollPos(sz);
}

void ScrollBox::TouchUp(int deltaValue)
{
	UiSize64 scrollPos = GetScrollPos();
	if (scrollPos.cy <= 0) {
		return;
	}

	scrollPos.cy = scrollPos.cy - deltaValue;
	SetScrollPos(scrollPos);
}

void ScrollBox::TouchDown(int deltaValue)
{
	UiSize64 scrollPos = GetScrollPos();
	if (scrollPos.cy >= GetScrollRange().cy) {
		return;
	}

	scrollPos.cy = scrollPos.cy + deltaValue;
	SetScrollPos(scrollPos);
}

void ScrollBox::EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
{
	if( bEnableVertical && (m_pVScrollBar == nullptr) ) {
		m_pVScrollBar.reset(new ScrollBar);
		m_pVScrollBar->SetVisible(false);
		m_pVScrollBar->SetScrollRange(0);
		m_pVScrollBar->SetOwner(this);
		m_pVScrollBar->SetWindow(GetWindow(), nullptr, false);
		m_pVScrollBar->SetClass(L"vscrollbar");
	}
	else if( !bEnableVertical && (m_pVScrollBar != nullptr) ) {
		m_pVScrollBar.reset();
	}

	if( bEnableHorizontal && (m_pHScrollBar == nullptr)) {
		m_pHScrollBar.reset(new ScrollBar);
		m_pHScrollBar->SetVisible(false);
		m_pHScrollBar->SetScrollRange(0);
		m_pHScrollBar->SetHorizontal(true);
		m_pHScrollBar->SetOwner(this);
		m_pHScrollBar->SetWindow(GetWindow(), nullptr, false);
		m_pHScrollBar->SetClass(L"hscrollbar");
	}
	else if( !bEnableHorizontal && (m_pHScrollBar != nullptr)) {
		m_pHScrollBar.reset();
	}

	Arrange();
}

ScrollBar* ScrollBox::GetVScrollBar() const
{
	return m_pVScrollBar.get();
}

ScrollBar* ScrollBox::GetHScrollBar() const
{
	return m_pHScrollBar.get();
}

void ScrollBox::ProcessVScrollBar(UiRect rc, int64_t cyRequired)
{
	UiRect rcScrollBarPos = rc;
	rcScrollBarPos.left += m_rcScrollBarPadding.left;
	rcScrollBarPos.top += m_rcScrollBarPadding.top;
	rcScrollBarPos.right -= m_rcScrollBarPadding.right;
	rcScrollBarPos.bottom -= m_rcScrollBarPadding.bottom;

	if (m_pVScrollBar == nullptr) {
		return;
	}

	rc.left += GetLayout()->GetPadding().left;
	rc.top += GetLayout()->GetPadding().top;
	rc.right -= GetLayout()->GetPadding().right;
	rc.bottom -= GetLayout()->GetPadding().bottom;
	int32_t nHeight = rc.Height();
	if (cyRequired > nHeight && !m_pVScrollBar->IsValid()) {
		m_pVScrollBar->SetScrollRange(cyRequired - nHeight);
		m_pVScrollBar->SetScrollPos(0);
		m_bScrollProcess = true;
		SetPos(GetRect());
		m_bScrollProcess = false;
		return;
	}
	// No scrollbar required
	if (!m_pVScrollBar->IsValid()) {
		return;
	}

	// Scroll not needed anymore?
	int64_t cyScroll = cyRequired - nHeight;
	if( cyScroll <= 0 && !m_bScrollProcess) {
		m_pVScrollBar->SetScrollPos(0);
		m_pVScrollBar->SetScrollRange(0);
		SetPos(GetRect());
	}
	else {
		if (m_bVScrollBarLeftPos) {
			ASSERT(m_pVScrollBar->GetFixedWidth().GetInt32() > 0);
			UiRect rcVerScrollBarPos(rcScrollBarPos.left, 
									 rcScrollBarPos.top, 
									 rcScrollBarPos.left + m_pVScrollBar->GetFixedWidth().GetInt32(),
									 rcScrollBarPos.bottom);
			m_pVScrollBar->SetPos(rcVerScrollBarPos);
		}
		else {
			ASSERT(m_pVScrollBar->GetFixedWidth().GetInt32() > 0);
			UiRect rcVerScrollBarPos(rcScrollBarPos.right - m_pVScrollBar->GetFixedWidth().GetInt32(),
				                     rcScrollBarPos.top, 
				                     rcScrollBarPos.right, 
				                     rcScrollBarPos.bottom);
			m_pVScrollBar->SetPos(rcVerScrollBarPos);
		}

		if( m_pVScrollBar->GetScrollRange() != cyScroll ) {
			int64_t iScrollPos = m_pVScrollBar->GetScrollPos();
			m_pVScrollBar->SetScrollRange(::abs(cyScroll));
			if( !m_pVScrollBar->IsValid() ) {
				m_pVScrollBar->SetScrollPos(0);
			}

			if( iScrollPos > m_pVScrollBar->GetScrollPos() ) {
				SetPos(GetRect());
			}
		}
	}
}

void ScrollBox::ProcessHScrollBar(UiRect rc, int64_t cxRequired)
{
	UiRect rcScrollBarPos = rc;
	rcScrollBarPos.left += m_rcScrollBarPadding.left;
	rcScrollBarPos.top += m_rcScrollBarPadding.top;
	rcScrollBarPos.right -= m_rcScrollBarPadding.right;
	rcScrollBarPos.bottom -= m_rcScrollBarPadding.bottom;

	if (m_pHScrollBar == nullptr) {
		return;
	}

	rc.left += GetLayout()->GetPadding().left;
	rc.top += GetLayout()->GetPadding().top;
	rc.right -= GetLayout()->GetPadding().right;
	rc.bottom -= GetLayout()->GetPadding().bottom;
	int32_t nWidth = rc.Width();
	if (cxRequired > nWidth && !m_pHScrollBar->IsValid()) {
		m_pHScrollBar->SetScrollRange(cxRequired - nWidth);
		m_pHScrollBar->SetScrollPos(0);
		m_bScrollProcess = true;
		SetPos(GetRect());
		m_bScrollProcess = false;
		return;
	}
	// No scrollbar required
	if (!m_pHScrollBar->IsValid()) {
		return;
	}

	// Scroll not needed anymore?
	int64_t cxScroll = cxRequired - nWidth;
	if (cxScroll <= 0 && !m_bScrollProcess) {
		m_pHScrollBar->SetScrollPos(0);
		m_pHScrollBar->SetScrollRange(0);
		SetPos(GetRect());
	}
	else {
		ASSERT(m_pHScrollBar->GetFixedHeight().GetInt32() > 0);
		UiRect rcVerScrollBarPos(rcScrollBarPos.left, 
								 rcScrollBarPos.bottom - m_pHScrollBar->GetFixedHeight().GetInt32(),
			                     rcScrollBarPos.right, 
			                     rcScrollBarPos.bottom);
		m_pHScrollBar->SetPos(rcVerScrollBarPos);

		if (m_pHScrollBar->GetScrollRange() != cxScroll) {
			int64_t iScrollPos = m_pHScrollBar->GetScrollPos();
			m_pHScrollBar->SetScrollRange(::abs(cxScroll));
			if (!m_pHScrollBar->IsValid()) {
				m_pHScrollBar->SetScrollPos(0);
			}

			if (iScrollPos > m_pHScrollBar->GetScrollPos()) {
				SetPos(GetRect());
			}
		}
	}
}

bool ScrollBox::IsVScrollBarValid() const
{
	if (m_pVScrollBar != nullptr) {
		return m_pVScrollBar->IsValid();
	}
	return false;
}

bool ScrollBox::IsHScrollBarValid() const
{
	if (m_pHScrollBar != nullptr) {
		return m_pHScrollBar->IsValid();
	}
	return false;
}

void ScrollBox::PlayRenderOffsetYAnimation(int64_t nRenderY)
{
	m_renderOffsetYAnimation->SetStartValue(nRenderY);
	m_renderOffsetYAnimation->SetEndValue(0);
	m_renderOffsetYAnimation->SetSpeedUpRatio(0.3);
	m_renderOffsetYAnimation->SetSpeedUpfactorA(0.003);
	m_renderOffsetYAnimation->SetSpeedDownRatio(0.7);
	m_renderOffsetYAnimation->SetTotalMillSeconds(DUI_NOSET_VALUE);
	m_renderOffsetYAnimation->SetMaxTotalMillSeconds(650);
	auto playCallback = nbase::Bind(&ScrollBox::SetRenderOffsetY, this, std::placeholders::_1);
	m_renderOffsetYAnimation->SetCallback(playCallback);
	m_renderOffsetYAnimation->Start();
}

bool ScrollBox::IsAtEnd() const
{
	return GetScrollRange().cy <= GetScrollPos().cy;
}

bool ScrollBox::IsHoldEnd() const
{
	return m_bHoldEnd;
}

void ScrollBox::SetHoldEnd(bool bHoldEnd)
{
	m_bHoldEnd = bHoldEnd;
}

int ScrollBox::GetVerScrollUnitPixels() const
{
	return m_nVScrollUnitPixels;
}

void ScrollBox::SetVerScrollUnitPixels(int nUnitPixels)
{
	GlobalManager::Instance().Dpi().ScaleInt(nUnitPixels);
	m_nVScrollUnitPixels = nUnitPixels;
}

int ScrollBox::GetHorScrollUnitPixels() const
{
    return m_nHScrollUnitPixels;
}

void ScrollBox::SetHorScrollUnitPixels(int nUnitPixels)
{
    GlobalManager::Instance().Dpi().ScaleInt(nUnitPixels);
    m_nHScrollUnitPixels = nUnitPixels;
}

bool ScrollBox::GetScrollBarFloat() const
{
	return m_bScrollBarFloat;
}

void ScrollBox::SetScrollBarFloat(bool bScrollBarFloat)
{
	m_bScrollBarFloat = bScrollBarFloat;
}

bool ScrollBox::GetVScrollBarLeftPos() const
{
	return m_bVScrollBarLeftPos;
}

void ScrollBox::SetVScrollBarLeftPos(bool bLeftPos)
{
	m_bVScrollBarLeftPos = bLeftPos;
}

ui::UiRect ScrollBox::GetScrollBarPadding() const
{
	return m_rcScrollBarPadding;
}

void ScrollBox::SetScrollBarPadding(UiRect rcScrollBarPadding)
{
	GlobalManager::Instance().Dpi().ScaleRect(rcScrollBarPadding);
	m_rcScrollBarPadding = rcScrollBarPadding;
}

void ScrollBox::ClearImageCache()
{
	__super::ClearImageCache();

	if (m_pHScrollBar != nullptr) {
		m_pHScrollBar->ClearImageCache();
	}
	if (m_pVScrollBar != nullptr) {
		m_pVScrollBar->ClearImageCache();
	}
}

void ScrollBox::StopScrollAnimation()
{
	m_scrollAnimation->Reset();
}

UiSize ScrollBox::GetScrollOffset() const
{
	//�������������λ�õ����룬��Ϊ�˽��UiRect��32λ����ֵ����֧�ֳ������ǧ���������������ϣ�������
	UiSize64 scrollPos = GetScrollPos();
	UiSize64 scrollVirtualOffset = GetScrollVirtualOffset();
	int64_t scrollPosX = scrollPos.cx - scrollVirtualOffset.cx;
	int64_t scrollPosY = scrollPos.cy - scrollVirtualOffset.cy;

	UiSize realcrollPos;
	realcrollPos.cx = TruncateToInt32(scrollPosX);
	realcrollPos.cy = TruncateToInt32(scrollPosY);
	return realcrollPos;
}

UiSize64 ScrollBox::GetScrollVirtualOffset() const
{
	return m_scrollVirtualOffset;
}

void ScrollBox::SetScrollVirtualOffset(UiSize64 szOffset)
{
	ASSERT(szOffset.cx >= 0);
	ASSERT(szOffset.cy >= 0);
	if ((szOffset.cx >= 0) && (szOffset.cy)) {
		m_scrollVirtualOffset = szOffset;
	}
}

void ScrollBox::SetScrollVirtualOffsetY(int64_t yOffset)
{
	ASSERT(yOffset >= 0);
	if (yOffset >= 0) {
		m_scrollVirtualOffset.cy = yOffset;
	}
}

void ScrollBox::SetScrollVirtualOffsetX(int64_t xOffset)
{
	ASSERT(xOffset >= 0);
	if (xOffset >= 0) {
		m_scrollVirtualOffset.cx = xOffset;
	}
}

} // namespace ui
