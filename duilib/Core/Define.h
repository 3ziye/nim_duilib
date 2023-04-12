#ifndef UI_CORE_DEFINE_H_
#define UI_CORE_DEFINE_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>

namespace ui
{

class Control;

#define DUI_NOSET_VALUE  -1
#define DUI_LENGTH_STRETCH  -1
#define DUI_LENGTH_AUTO  -2

enum HorAlignType
{
	kHorAlignLeft,		// �������
	kHorAlignCenter,	// ˮƽ����
	kHorAlignRight,		// ���Ҷ���
};

enum VerAlignType
{
	kVerAlignTop,		// ���˶���
	kVerAlignCenter,	// ��ֱ����
	kVerAlignBottom,	// �ײ�����
};

enum StateImageType
{
	kStateImageBk,
	kStateImageFore,
	kStateImageSelectedBk,
	kStateImageSelectedFore,
};

// Flags used for controlling the paint
enum ControlStateType
{
	kControlStateNormal,	// ��ͨ״̬
	kControlStateHot,		// ����״̬
	kControlStatePushed,	// ����״̬
	kControlStateDisabled	// ����״̬
};

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

enum GifStopType
{
	kGifStopCurrent = -1,	// ��ǰ֡
	kGifStopFirst = 0,		// GIF ֹͣʱ��λ����һ֡	
	kGifStopLast = 0xFFFF,	// ���һ֡
};

enum CursorType
{
	kCursorArrow,		// ��ͷ
	kCursorHand,		// �ֱ�
	kCursorHandIbeam	// 
};

//����������Ϣ����
enum EventType
{
	kEventInternalDoubleClick,
	kEventInternalMenu,
	kEventInternalSetFocus,
	kEventInternalKillFocus ,

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


struct EventArgs
{
	EventArgs():
	 Type(kEventNone),
     pSender(nullptr),
	 dwTimestamp(0),
	 chKey(0),
	 wParam(0),
	 lParam(0),
	 pressure(0.0f)
	{
		ptMouse.x = ptMouse.y = 0;
	}

	EventType Type;
	Control* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	TCHAR chKey;
	WPARAM wParam;
	LPARAM lParam;
	FLOAT pressure;
};

EventType StringToEnum(const std::wstring& messageType);


//�������пؼ�����
#define  DUI_CTR_CONTROL                         (L"Control")
#define  DUI_CTR_LABEL                           (L"Label")
#define  DUI_CTR_BUTTON                          (L"Button")
#define  DUI_CTR_TEXT                            (L"Text")
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

/** ���ڱ�������ť����󻯡���С�����رա���ԭ���ڵ����֣�������д����
*/
#define DUI_CTR_BUTTON_CLOSE					 (L"closebtn")
#define DUI_CTR_BUTTON_MIN						 (L"minbtn")
#define DUI_CTR_BUTTON_MAX						 (L"maxbtn")
#define DUI_CTR_BUTTON_RESTORE					 (L"restorebtn")

}// namespace ui

#endif // UI_CORE_DEFINE_H_