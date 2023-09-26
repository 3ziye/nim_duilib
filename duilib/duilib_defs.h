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

#include "duilib/duilib_config.h"

#define DUI_NOSET_VALUE		-1

namespace ui
{
	//�������пؼ�����
	#define  DUI_CTR_CONTROL                         (L"Control")
	#define  DUI_CTR_LABEL                           (L"Label")
	#define  DUI_CTR_RICHTEXT                        (L"RichText")
	#define  DUI_CTR_BUTTON                          (L"Button")
	#define  DUI_CTR_OPTION                          (L"Option")
	#define  DUI_CTR_CHECKBOX                        (L"CheckBox")
	#define  DUI_CTR_SPLIT							 (L"Split")

	#define  DUI_CTR_LABELBOX                        (L"LabelBox")
	#define  DUI_CTR_BUTTONBOX                       (L"ButtonBox")
	#define  DUI_CTR_OPTIONBOX                       (L"OptionBox")
	#define  DUI_CTR_CHECKBOXBOX                     (L"CheckBoxBox")
	#define  DUI_CTR_SPLITBOX					     (L"SplitBox")

	#define  DUI_CTR_BOX							 (L"Box")
	#define  DUI_CTR_HBOX							 (L"HBox")
	#define  DUI_CTR_VBOX                            (L"VBox")
	#define  DUI_CTR_VTILE_BOX					     (L"VTileBox")
	#define  DUI_CTR_HTILE_BOX					     (L"HTileBox")

	#define  DUI_CTR_GROUP_BOX                       (L"GroupBox")
	#define  DUI_CTR_GROUP_HBOX                      (L"GroupHBox")
	#define  DUI_CTR_GROUP_VBOX                      (L"GroupVBox")

	#define  DUI_CTR_SCROLLBOX						 (L"ScrollBox")
	#define  DUI_CTR_HSCROLLBOX						 (L"HScrollBox")
	#define  DUI_CTR_VSCROLLBOX						 (L"VScrollBox")
	#define  DUI_CTR_HTILE_SCROLLBOX				 (L"HTileScrollBox")
	#define  DUI_CTR_VTILE_SCROLLBOX				 (L"VTileScrollBox")

	#define  DUI_CTR_LISTBOX_ITEM					 (L"ListBoxItem")
	#define  DUI_CTR_HLISTBOX						 (L"HListBox")
	#define  DUI_CTR_VLISTBOX                        (L"VListBox")
	#define  DUI_CTR_HTILE_LISTBOX                   (L"HTileListBox")
	#define  DUI_CTR_VTILE_LISTBOX                   (L"VTileListBox")

	#define  DUI_CTR_VIRTUAL_VLISTBOX				 (L"VirtualVListBox")
	#define  DUI_CTR_VIRTUAL_HLISTBOX				 (L"VirtualHListBox")
	#define  DUI_CTR_VIRTUAL_HTILE_LISTBOX			 (L"VirtualHTileListBox")
	#define  DUI_CTR_VIRTUAL_VTILE_LISTBOX			 (L"VirtualVTileListBox")

	#define  DUI_CTR_TABBOX                          (L"TabBox")

	#define  DUI_CTR_TREENODE                        (L"TreeNode")
	#define  DUI_CTR_TREEVIEW                        (L"TreeView")

	#define  DUI_CTR_RICHEDIT                        (L"RichEdit")
	#define  DUI_CTR_COMBO                           (L"Combo")
	#define  DUI_CTR_COMBO_BUTTON                    (L"ComboButton")
	#define  DUI_CTR_FILTER_COMBO                    (L"FilterCombo")
	#define  DUI_CTR_CHECK_COMBO                     (L"CheckCombo")
	#define  DUI_CTR_SLIDER                          (L"Slider")
	#define  DUI_CTR_PROGRESS                        (L"Progress")
	#define  DUI_CTR_CIRCLEPROGRESS                  (L"CircleProgress")
	#define  DUI_CTR_SCROLLBAR                       (L"ScrollBar")

	#define  DUI_CTR_MENU							 (L"Menu")
	#define  DUI_CTR_SUB_MENU				   		 (L"SubMenu")
	#define  DUI_CTR_MENUELEMENT					 (L"MenuElement")

	#define  DUI_CTR_DATETIME                        (L"DateTime")

	//��ɫ��صĲ��ֿؼ�
	#define  DUI_CTR_COLOR_CONTROL					 (L"ColorControl")
	#define  DUI_CTR_COLOR_SLIDER					 (L"ColorSlider")
	#define  DUI_CTR_COLOR_PICKER_REGULAR			 (L"ColorPickerRegular")
	#define  DUI_CTR_COLOR_PICKER_STANDARD			 (L"ColorPickerStatard")
	#define  DUI_CTR_COLOR_PICKER_STANDARD_GRAY		 (L"ColorPickerStatardGray")
	#define  DUI_CTR_COLOR_PICKER_CUSTOM			 (L"ColorPickerCustom")

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
	
	#define EVENTSTR_SETFOCUS			(L"setfocus")
	#define EVENTSTR_KILLFOCUS			(L"killfocus")
	#define	EVENTSTR_SETCURSOR			(L"setcursor")

	#define EVENTSTR_MOUSEMOVE			(L"mousemove")
	#define	EVENTSTR_MOUSEENTER			(L"mouseenter")
	#define	EVENTSTR_MOUSELEAVE			(L"mouseleave")
	#define	EVENTSTR_MOUSEHOVER			(L"mousehover")

	#define	EVENTSTR_BUTTONDOWN			(L"buttondown")
	#define	EVENTSTR_BUTTONUP			(L"buttonup")
	#define	EVENTSTR_DOUBLECLICK		(L"doubleclick")
	#define	EVENTSTR_RBUTTONDOWN		(L"rbuttondown")
	#define	EVENTSTR_RBUTTONUP			(L"rbuttonup")
	#define	EVENTSTR_RDOUBLECLICK		(L"rdoubleclick")

	#define EVENTSTR_SELECT				(L"select")
	#define EVENTSTR_UNSELECT			(L"unselect")

	#define EVENTSTR_CHECKED			(L"checked")
	#define EVENTSTR_UNCHECK			(L"uncheck")
	
	#define EVENTSTR_TAB_SELECT			(L"tab_select")

	#define	EVENTSTR_MENU				(L"menu")

	#define EVENTSTR_EXPAND				(L"expand")
	#define EVENTSTR_COLLAPSE			(L"collapse")

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
		kStateImageBk,					//����ͼƬ��Control���ã�
		kStateImageFore,				//ǰ��ͼƬ��Control���ã�
		kStateImageSelectedBk,			//ѡ��ʱ�ı���ͼƬ��CheckBox���ã�
		kStateImageSelectedFore,		//ѡ��ʱ��ǰ��ͼƬ��CheckBox���ã�
		kStateImagePartSelectedBk,		//����ѡ��ʱ�ı���ͼƬ��CheckBox���ã�
		kStateImagePartSelectedFore,	//����ѡ��ʱ��ǰ��ͼƬ��CheckBox���ã�
	};

	//�ؼ�״̬
	enum ControlStateType
	{
		kControlStateNormal,	// ��ͨ״̬
		kControlStateHot,		// ��ͣ״̬
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
		kCursorHandIbeam,	// ��I����״
		kCursorSizeWE,		// �����϶�
		kCursorSizeNS		// �����϶�
	};

	//����������Ϣ����
	enum EventType
	{
		kEventNone,
		kEventFirst,				//��һ����Ϣ����ʱû�õ���
		kEventAll,					//����������Ϣ

		//������Ϣ
		kEventKeyBegin,
		kEventKeyDown,				//Window�ࣺ���յ�WM_KEYDOWN��Ϣʱ���������͸�Focus�ؼ�
		kEventKeyUp,				//Window�ࣺ���յ�WM_KEYUP��Ϣʱ���������͸�WM_KEYDOWN�¼��е��Ǹ�Focus�ؼ�
		kEventChar,					//Window�ࣺ���յ�WM_CHAR��Ϣʱ���������͸�WM_KEYDOWN�¼��е��Ǹ�Focus�ؼ�
		kEventKeyEnd,

		//�����Ϣ
		kEventMouseBegin,		
		kEventMouseEnter,			//Window�ࣺ���յ�WM_MOUSEMOVE��Ϣʱ���������͸�MouseMoveʱ����������Ǹ��ؼ������������뵽����ؼ���
		kEventMouseLeave,			//Window�ࣺ���յ�WM_MOUSEMOVE��Ϣʱ���������͸�MouseMoveʱ����������Ǹ��ؼ�����������Ѿ��뿪����ؼ�
		kEventMouseMove,			//Window�ࣺ���յ�WM_MOUSEMOVE��Ϣʱ���������͸����������Ҽ���˫��ʱ��¼���Ǹ��ؼ�
		kEventMouseHover,			//Window�ࣺ���յ�WM_MOUSEHOVER��Ϣʱ���������͸�MouseMoveʱ����������Ǹ��ؼ����������������ؼ�����ͣ
		kEventMouseWheel,			//Window�ࣺ���յ�WM_MOUSEWHEEL��Ϣʱ���������͸���ǰ�������λ�ö�Ӧ�Ŀؼ�
		kEventMouseButtonDown,		//Window�ࣺ���յ�WM_LBUTTONDOWN��Ϣʱ���������͸�����������ʱ��Ӧ�Ŀؼ�
		kEventMouseButtonUp,		//Window�ࣺ���յ�WM_LBUTTONUP��Ϣʱ���������͸�����������ʱ��Ӧ�Ŀؼ�
		kEventMouseDoubleClick,		//Window�ࣺ���յ�WM_LBUTTONDBLCLK��Ϣʱ���������͸���ǰ�������λ�ö�Ӧ�Ŀؼ�		
		kEventMouseRButtonDown,		//Window�ࣺ���յ�WM_RBUTTONDOWN��Ϣʱ���������͸�����Ҽ�����ʱ��Ӧ�Ŀؼ�
		kEventMouseRButtonUp,		//Window�ࣺ���յ�WM_RBUTTONUP��Ϣʱ���������͸�����Ҽ�����ʱ��Ӧ�Ŀؼ�
		kEventMouseRDoubleClick,	//Window�ࣺ���յ�WM_RBUTTONDBLCLK��Ϣʱ���������͸���ǰ�������λ�ö�Ӧ�Ŀؼ�	
		kEventMouseMenu,			//Window�ࣺ���յ�WM_CONTEXTMENU��Ϣʱ���������͸�����ע��ص������Ŀؼ�
		kEventMouseEnd,

		//����/���/���뷨�����Ϣ
		kEventSetFocus,				//Window�ࣺ���͸�Focus�ؼ������ؼ���ý���ʱ�����¼����ؼ����㲻�Ǵ��ڽ��㣬������ȫ��ͬ��
		kEventKillFocus,			//Window�ࣺ���͸�Focus�ؼ������ؼ�ʧȥ����ʱ�����¼����ؼ����㲻�Ǵ��ڽ��㣬������ȫ��ͬ��		
		kEventSetCursor,			//Window�ࣺ���͸���ǰ������ڿؼ������ù��
		kEventImeStartComposition,	//Window�ࣺ���͸�Focus�ؼ������յ�ϵͳWM_IME_STARTCOMPOSITION��Ϣʱ����
		kEventImeEndComposition,	//Window�ࣺ���͸�Focus�ؼ������յ�ϵͳWM_IME_ENDCOMPOSITION��Ϣʱ����

		kEventWindowKillFocus,		//Window�ࣺ���͸����������Ҽ�����ʱ��¼�Ŀؼ�������ؼ���������ʧȥ����ʱ�����¼�����Ҫ���ڻָ�һЩ�ڲ�״̬��
		kEventWindowSize,			//Window�ࣺ���͸�Focus�ؼ��������ڴ�С�����仯ʱ�����¼�
		kEventWindowMove,			//Window�ࣺ���͸�Focus�ؼ��������ڴ�С�����仯ʱ�����¼�
		kEventWindowClose,			//Window�࣬Combo�ؼ��������ڹرգ�����Combo�������򴰿ڹرգ�ʱ����

		//������/�Ҽ�����¼�
		kEventClick,				//Button�ࡢListBoxItem��Option��CheckBox�ȣ��������ť�����߼��̻س���ʱ����
		kEventRClick,				//Control�ࣺ������Ҽ�����ʱ����

		//ѡ��/ȡ��ѡ��
		kEventSelect,				//CheckBox�ࡢOption�ࡢListBox�ࡢCombo�ࣺ�����ѡ��״̬ʱ����
		kEventUnSelect,			    //CheckBox�ࡢOption�ࡢListBox�ࡢCombo�ࣺ����ɷ�ѡ��״̬ʱ����

		//��ѡ/ȡ����ѡ
		kEventChecked,				//CheckBox�ࣺ�����Checked״̬ʱ����
		kEventUnCheck,			    //CheckBox�ࣺ�����UnCheck״̬ʱ����

		//TABҳ��ѡ��
		kEventTabSelect,			//TabBox�࣬ѡ��TABҳ��, WPARAM����ҳ��ID��LPARAM�Ǿ�ҳ��ID

		//���ڵ�չ��/����
		kEventExpand,				//TreeNode�ࣺ�����ڵ�չ��ʱ����
		kEventCollapse,				//TreeNode�ࣺ�����ڵ�����ʱ����

		kEventZoom,					//RichEdit�ࣺ�����ű��������仯ʱ�������ű�������/��ĸ��ʾ�����ţ�1/64 < (wParam / lParam) < 64
		kEventTextChange,			//RichEdit�ࣺ���ı����ݷ����仯ʱ����
		kEventSelChange,			//RichEdit�ࣺ���ı�ѡ�����ݷ����仯ʱ����
		kEventReturn,				//ListBoxItem��RichEdit�ࣺ���յ��س���ʱ����
		kEventTab,					//RichEdit�ࣺ��WantTabΪfalseʱ�����յ�TAB��ʱ����
		kEventCustomLinkClick,		//RichEdit�ࣺ��������Զ���link��������ʱ����, ����ͨ��WPARAM��ȡ�����URL������Ϊconst wchar_t*

		kEventScrollChange,			//ScrollBox�ࣺ��������λ�÷����仯ʱ����
		kEventValueChange,			//DateTime��Slider�ࣺ��ֵ�����仯ʱ����, Slider�ࣺWPARAM����ֵ��LPARAM�Ǿ�ֵ
		kEventResize,				//Control�ࣺ���ؼ��Ĵ�С��λ�÷����仯ʱ����
		kEventVisibleChange,		//Control�ࣺ���ؼ���Visible���Է����仯ʱ����
		kEventStateChange,			//Control�ࣺ���ؼ���State���Է����仯ʱ����, WPARAM����״̬��LPARAM�Ǿ�״̬
		kEventSelectColor,			//ColorPicker�ࣺ��ѡ��������ɫʱ����, WPARAM���µ���ɫֵ��LPARAM�Ǿɵ���ɫֵ

		kEventLast					//Control�ࣺ�ÿؼ������һ����Ϣ��������ؼ���������ʱ����
	};

}// namespace ui

#endif //DUILIB_DEFS_H_


