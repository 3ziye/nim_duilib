// Copyright (c) 2010-2011, duilib develop team(www.duilib.com).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted provided that the 
// following conditions are met.
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above 
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials 
// provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DUILIB_DEFS_H_
#define DUILIB_DEFS_H_ 1

#pragma once

#include "duilib_config.h"

#define DUI_NOSET_VALUE		-1
#define DUI_LENGTH_STRETCH  -1
#define DUI_LENGTH_AUTO		-2

namespace ui
{
	//�������пؼ�����
	#define  DUI_CTR_CONTROL                         (L"Control")
	#define  DUI_CTR_LABEL                           (L"Label")
	#define  DUI_CTR_BUTTON                          (L"Button")
	#define  DUI_CTR_OPTION                          (L"Option")
	#define  DUI_CTR_CHECKBOX                        (L"CheckBox")

	#define  DUI_CTR_LABELBOX                        (L"LabelBox")
	#define  DUI_CTR_BUTTONBOX                       (L"ButtonBox")
	#define  DUI_CTR_OPTIONBOX                       (L"OptionBox")
	#define  DUI_CTR_CHECKBOXBOX                     (L"CheckBoxBox")

	#define  DUI_CTR_BOX							 (L"Box")
	#define  DUI_CTR_HBOX							 (L"HBox")
	#define  DUI_CTR_VBOX                            (L"VBox")
	#define  DUI_CTR_TABBOX                          (L"TabBox")
	#define  DUI_CTR_TILEBOX						 (L"TileBox")
	#define  DUI_CTR_CHILDBOX                        (L"ChildBox")

	#define  DUI_CTR_LISTCONTAINERELEMENT            (L"ListContainerElement")
	#define  DUI_CTR_LISTBOX						 (L"ListBox")
	#define  DUI_CTR_HLISTBOX						 (L"HListBox")
	#define  DUI_CTR_VLISTBOX                        (L"VListBox")
	#define  DUI_CTR_VIRTUALLISTBOX                  (L"VirtualListBox")
	#define  DUI_CTR_TILELISTBOX                     (L"TileListBox")
	#define  DUI_CTR_VIRTUALTILEBOX                  (L"VirtualTileBox")

	#define  DUI_CTR_TREENODE                        (L"TreeNode")
	#define  DUI_CTR_TREEVIEW                        (L"TreeView")

	#define  DUI_CTR_RICHEDIT                        (L"RichEdit")
	#define  DUI_CTR_COMBO                           (L"Combo")
	#define  DUI_CTR_SLIDER                          (L"Slider")
	#define  DUI_CTR_PROGRESS                        (L"Progress")
	#define  DUI_CTR_CIRCLEPROGRESS                  (L"CircleProgress")
	#define  DUI_CTR_SCROLLBAR                       (L"ScrollBar")

	#define  DUI_CTR_MENU							 (L"Menu")
	#define  DUI_CTR_SUB_MENU				   		 (L"SubMenu")
	#define  DUI_CTR_MENUELEMENT					 (L"MenuElement")

	// ���ڱ�������ť����󻯡���С�����رա���ԭ���ڵ����֣�������д����
	#define DUI_CTR_BUTTON_CLOSE					 (L"closebtn")
	#define DUI_CTR_BUTTON_MIN						 (L"minbtn")
	#define DUI_CTR_BUTTON_MAX						 (L"maxbtn")
	#define DUI_CTR_BUTTON_RESTORE					 (L"restorebtn")

	//EventType�¼����Ͷ�Ӧ���ַ�������
	#define	EVENTSTR_ALL				(L"all")
	#define	EVENTSTR_KEYDOWN			(L"keydown")
	#define	EVENTSTR_KEYUP				(L"keyup")
	#define	EVENTSTR_CHAR				(L"char")
	#define	EVENTSTR_SYSKEY				(L"syskey")

	#define EVENTSTR_SETFOCUS			(L"setfocus")
	#define EVENTSTR_KILLFOCUS			(L"killfocus")
	#define	EVENTSTR_SETCURSOR			(L"setcursor")

	#define EVENTSTR_MOUSEMOVE			(L"mousemove")
	#define	EVENTSTR_MOUSEENTER			(L"mouseenter")
	#define	EVENTSTR_MOUSELEAVE			(L"mouseleave")
	#define	EVENTSTR_MOUSEHOVER			(L"mousehover")

	#define	EVENTSTR_BUTTONDOWN			(L"buttondown")
	#define	EVENTSTR_BUTTONUP			(L"buttonup")
	#define	EVENTSTR_RBUTTONDOWN		(L"rbuttondown")
	#define	EVENTSTR_DOUBLECLICK		(L"doubleclick")

	#define EVENTSTR_SELECT				(L"select")
	#define EVENTSTR_UNSELECT			(L"unselect")
	#define	EVENTSTR_MENU				(L"menu")

	#define EVENTSTR_EXPAND				(L"expand")
	#define EVENTSTR_UNEXPAND			(L"unexpand")

	#define	EVENTSTR_SCROLLWHEEL		(L"scrollwheel")
	#define EVENTSTR_SCROLLCHANGE		(L"scrollchange")

	#define EVENTSTR_VALUECHANGE		(L"valuechange")
	#define EVENTSTR_RETURN				(L"return")
	#define EVENTSTR_TAB				(L"tab")
	#define EVENTSTR_WINDOWCLOSE		(L"windowclose")

	//ˮƽ���뷽ʽ
	enum HorAlignType
	{
		kHorAlignLeft,		// �������
		kHorAlignCenter,	// ˮƽ����
		kHorAlignRight,		// ���Ҷ���
	};

	//��ֱ���뷽ʽ
	enum VerAlignType
	{
		kVerAlignTop,		// ���˶���
		kVerAlignCenter,	// ��ֱ����
		kVerAlignBottom,	// �ײ�����
	};

	//�ؼ�ͼƬ����
	enum StateImageType
	{
		kStateImageBk,
		kStateImageFore,
		kStateImageSelectedBk,
		kStateImageSelectedFore,
	};

	//�ؼ�״̬
	enum ControlStateType
	{
		kControlStateNormal,	// ��ͨ״̬
		kControlStateHot,		// ����״̬
		kControlStatePushed,	// ����״̬
		kControlStateDisabled	// ����״̬
	};

	//�ؼ���������
	enum AnimationType
	{
		kAnimationNone,
		kAnimationAlpha,
		kAnimationHeight,
		kAnimationWidth,
		kAnimationHot,
		kAnimationInoutXFromLeft,
		kAnimationInoutXFromRight,
		kAnimationInoutYFromTop,
		kAnimationInoutYFromBottom,
	};

	//GIFͼƬֹͣ����
	enum GifStopType
	{
		kGifStopCurrent = -1,	// ��ǰ֡
		kGifStopFirst = 0,		// GIF ֹͣʱ��λ����һ֡	
		kGifStopLast = 0xFFFF,	// ���һ֡
	};

	//���
	enum CursorType
	{
		kCursorArrow,		// ��ͷ
		kCursorHand,		// ����
		kCursorHandIbeam	// ��I����״
	};

	//����������Ϣ����
	enum EventType
	{
		kEventInternalDoubleClick,
		kEventInternalMenu,
		kEventInternalSetFocus,
		kEventInternalKillFocus,

		kEventNone,

		kEventFirst,

		kEventAll,

		kEventKeyBegin,
		kEventKeyDown,
		kEventKeyUp,
		kEventChar,
		kEventSystemKey,
		kEventKeyEnd,

		kEventMouseBegin,
		kEventMouseMove,
		kEventMouseEnter,
		kEventMouseLeave,
		kEventMouseHover,
		kEventMouseButtonDown,
		kEventMouseButtonUp,
		kEventMouseRightButtonDown,
		kEventMouseRightButtonUp,
		kEventMouseDoubleClick,
		kEventMouseMenu,
		kEventMouseScrollWheel,
		kEventMouseEnd,

		kEventTouchBegin,
		kEventTouchDown,
		kEventTouchMove,
		kEventTouchUp,
		kEventTouchEnd,

		kEventPointBegin,
		kEventPointDown,
		kEventPointMove,
		kEventPointUp,
		kEventPointEnd,

		kEventSetFocus,
		kEventKillFocus,
		kEventWindowSize,
		kEventWindowClose,
		kEventSetCursor,

		kEventClick,
		kEventSelect,
		kEventUnSelect,

		kEventExpand,
		kEventUnExpand,

		kEventTextChange,
		kEventReturn,
		kEventTab,
		kEventCustomLinkClick,

		kEventImeStartComposition,
		kEventImeEndComposition,

		kEventScrollChange,
		kEventValueChange,
		kEventResize,

		kEventNotify,	//�����򵥵�֪ͨ���и���������ͨ��������ʽʵ��

		kEventVisibleChange,

		kEventLast,
	};

}// namespace ui

#endif //DUILIB_DEFS_H_


