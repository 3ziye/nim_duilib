#ifndef UI_CORE_CONTROL_H_
#define UI_CORE_CONTROL_H_

#pragma once

#include "duilib/Core/PlaceHolder.h"
#include "duilib/Core/BoxShadow.h"
#include "duilib/Utils/Delegate.h"
#include <map>
#include <memory>

namespace ui 
{
	class Control;
	class Image;
	class IMatrix;
	class StateColorMap;
	class StateImageMap;
	class AnimationManager;
	class IRender;
	class IPath;

	typedef Control* (CALLBACK* FINDCONTROLPROC)(Control*, LPVOID);

class UILIB_API Control : 
	public PlaceHolder
{
	typedef std::map<int, CEventSource> GifEventMap;
public:
	Control();
	Control(const Control& r) = delete;
	Control& operator=(const Control& r) = delete;
    virtual ~Control();

	virtual std::wstring GetType() const;

    /// ͼ�����
	/**@brief ��ȡ������ɫ
	 * @return ���ر�����ɫ���ַ�������ֵ�� global.xml �ж���
	 */
	const std::wstring& GetBkColor() const { return m_strBkColor; }

	/** @brief ���ñ�����ɫ
	 * @param[in] strColor Ҫ���õı�����ɫֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetBkColor(const std::wstring& strColor);

	/**
	 * @brief ��ȡĳ��״̬�µ�������ɫ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫֵ���ο� ControlStateType ö��
	 * @return ָ��״̬���趨����ɫ�ַ�������Ӧ global.xml ��ָ��ɫֵ
	 */
	std::wstring GetStateColor(ControlStateType stateType) const;

	/**
	 * @brief ����ĳ��״̬�µ�������ɫ
	 * @param[in] stateType Ҫ���ú���״̬�µ���ɫֵ���ο� ControlStateType ö��
	 * @param[in] strColor Ҫ���õ���ɫֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetStateColor(ControlStateType stateType, const std::wstring& strColor);

	/**
	 * @brief ��ȡ����ͼƬλ��
	 * @return ����ͼƬλ��  
	 */
    std::wstring GetBkImage() const;

	/**
	 * @brief ��ȡ UTF8 ��ʽ�ı���ͼƬλ��
	 * @return ����ͼƬλ��
	 */
	std::string GetUTF8BkImage() const;

	/**
	 * @brief ���ñ���ͼƬ
	 * @param[in] strImage Ҫ���õ�ͼƬ·��
	 * @return ��
	 */
	void SetBkImage(const std::wstring& strImage);

	/**
	 * @brief ���ñ���ͼƬ��UTF8 ��ʽ�ַ�����
	 * @param[in] strImage Ҫ���õ�ͼƬ·��
	 * @return ��
	 */
	void SetUTF8BkImage(const std::string& strImage);

    /**
    * @brief ��ȡloading״̬ͼƬλ��
    * @return loadingͼƬλ��
    */
    std::wstring GetLoadingImage() const;

    /**
    * @brief ����loadingͼƬ
    * @param[in] strImage Ҫ���õ�ͼƬ·��
    * @return ��
    */
    void SetLoadingImage(const std::wstring& strImage);

    /**
    * @brief ����loading����ɫ
    * @param[in] strColor ����ɫ
    * @return ��
    */
    void SetLoadingBkColor(const std::wstring& strColor);

	/**
	 * @brief ��ȡָ��״̬�µ�ͼƬλ��
	 * @param[in] Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
	 * @return ָ��״̬�µ�ͼƬλ��
	 */
	std::wstring GetStateImage(ControlStateType stateType);

	/**
	 * @brief ����ĳ��״̬�µ�ͼƬ
	 * @param[in] stateType Ҫ���ú���״̬�µ�ͼƬ���ο� ControlStateType ö��
	 * @param[in] strImage Ҫ���õ�ͼƬ·��
	 * @return ��
	 */
	void SetStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**
	 * @brief ��ȡָ��״̬�µ�ǰ��ͼƬ
	 * @param[in] Ҫ��ȡ����״̬�µ�ͼƬ���ο� `ControlStateType` ö��
	 * @return ָ��״̬��ǰ��ͼƬλ��
	 */
	std::wstring GetForeStateImage(ControlStateType stateType);

	/**
	 * @brief ����ĳ��״̬��ǰ��ͼƬ
	 * @param[in] stateType Ҫ���ú���״̬�µ�ͼƬ���ο� `ControlStateType` ö��
	 * @param[in] strImage Ҫ���õ�ǰ��ͼƬ·��
	 * @return ��
	 */
	void SetForeStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**@brief ��ȡ�ؼ�״̬
	 * @return �ؼ���״̬����ο� `ControlStateType` ö��
	 */
	ControlStateType GetState() const;

	/**@brief ���ÿؼ�״̬
	 * @param[in] controlState Ҫ���õĿؼ�״̬����ο� `ControlStateType` ö��
	 */
	void SetState(ControlStateType controlState);

	/** ��ȡ���ڹ���ؼ���С����͸ߣ�ͼƬ�ӿ�
	 */
	virtual Image* GetEstimateImage();

	/// �߿����
	/**@brief ��ȡ�߿���ɫ
	 * @return �߿����ɫ�ַ�������Ӧ global.xml �еľ�����ɫֵ
	 */
	const std::wstring& GetBorderColor() const;

	/**
	 * @brief ���ñ߿���ɫ
	 * @param[in] strBorderColor ���ñ߿����ɫ�ַ���ֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetBorderColor(const std::wstring& strBorderColor);

	/** @brief ���ñ߿�Ĵ�С(left��top��right��bottom�ֱ��Ӧ��߿��С���ϱ߿��С���ұ߿��С���±߿��С)
	 * @param[in] rc һ�� `UiRect` �ṹ�ı߿��С����
	 */
	void SetBorderSize(UiRect rc);

	/**
	 * @brief ��ȡ���߿��С
	 * @return ���߿�Ĵ�С  
	 */
	int GetLeftBorderSize() const;

	/**
	 * @brief �������߿��С
	 * @param[in] nSize Ҫ���õ����߿��С
	 * @return ��
	 */
	void SetLeftBorderSize(int nSize);

	/**
	 * @brief ��ȡ�����߿��С
	 * @return �����߿��С
	 */
	int GetTopBorderSize() const;

	/**
	 * @brief ���ö����߿��С
	 * @param[in] nSize Ҫ���õ��Ϸ��߿��С
	 * @return ��
	 */
	void SetTopBorderSize(int nSize);

	/**
	 * @brief ��ȡ�Ҳ�߿��С
	 * @return �Ҳ�ı߿��С
	 */
	int GetRightBorderSize() const;

	/**
	 * @brief �����Ҳ�߿��С
	 * @param[in] nSize Ҫ���õ��Ҳ�߿��С
	 * @return ��
	 */
	void SetRightBorderSize(int nSize);

	/**
	 * @brief ��ȡ�·��߿��С
	 * @return �·��߿��С
	 */
	int GetBottomBorderSize() const;

	/**@brief �����·��߿��С
	 * @param[in] nSize Ҫ���õ��·��߿��С
	 */
	void SetBottomBorderSize(int nSize);

	/**@brief ��ȡ�߿��С
	 * @return �ĸ�����ı߿��С
	 */
	const UiSize& GetBorderRound() const;

	/**
	 * @brief ���ñ߿��С
	 * @param[in] cxyRound һ�� UiSize �ṹ��ʾ���ĸ�����߿�Ĵ�С
	 * @return ��
	 */
	void SetBorderRound(UiSize cxyRound);

	/**
	 * @brief ���ñ߿���Ӱ
	 * @param[in] Ҫ���õ���Ӱ����
	 * @return ��
	 */
	void SetBoxShadow(const std::wstring& strShadow);

    /// ������
	/**
	 * @brief ��ȡ���ָ������
	 */
	virtual CursorType GetCursorType() const;

	/**@brief ���õ�ǰ���ָ������
	 * @param[in] cursorType Ҫ���õ�������ͣ��ο� CursorType ö��
	 */
	void SetCursorType(CursorType cursorType);

	/**
	 * @brief ��ȡ�ؼ����������״̬�µ���ʾ�ı�
	 * @return ���ص�ǰ�������״̬��ʾ���ı�
	 */
    virtual std::wstring GetToolTipText() const;

	/**
	 * @brief ��ȡ�ؼ����������״̬�µ���ʾ�ı���UTF8 ��ʽ��
	 * @return ���ص�ǰ�������״̬��ʾ���ı�
	 */
	virtual std::string GetUTF8ToolTipText() const;

	/**
	 * @brief ��������������ؼ���ʾ����ʾ�ı�
	 * @param[in] strText Ҫ���õ��ı�
	 * @return ��
	 */
	virtual void SetToolTipText(const std::wstring& strText);

	/**
	 * @brief ��������������ؼ���ʾ����ʾ�ı���UTF8 ��ʽ��
	 * @param[in] strText Ҫ���õ��ı�
	 * @return ��
	 */
	virtual void SetUTF8ToolTipText(const std::string& strText);

	/**
	 * @brief ��������������ؼ���ʾ����ʾ�ı��������ļ��ж�Ӧ������
	 * @param[in] strTextId �������ļ��ж�Ӧ����ʾ���� ID
	 * @return ��
	 */
	virtual void SetToolTipTextId(const std::wstring& strTextId);

	/**
	 * @brief ��������������ؼ���ʾ����ʾ�ı��������ļ��ж�Ӧ�����֣�UTF8 ��ʽ��
	 * @param[in] strTextId �������ļ��ж�Ӧ����ʾ���� ID
	 * @return ��
	 */
	virtual void SetUTF8ToolTipTextId(const std::string& strTextId);

	/**
	 * @brief ��������������ؼ�����ʾ���ı����������
	 * @param[in] nWidth Ҫ���õĿ��ֵ
	 * @return ��
	 */
	virtual void SetToolTipWidth(int nWidth);

	/**
	 * @brief ��ȡ����������ؼ�����ʾ���ı����������
	 * @return ��ǰ��ʾ�ı������
	 */
	virtual int GetToolTipWidth(void) const;

    /// �˵�
	/**
	 * @brief �ؼ��Ƿ���Ӧ�����Ĳ˵���Ϣ
	 * @return ���ؽ����ʾ���Ƿ���Ӧ�����Ĳ˵���Ϣ��true ��Ӧ�����Ĳ˵���Ϣ��false ����Ӧ�����Ĳ˵���Ϣ
	 */
	virtual bool IsContextMenuUsed() const { return m_bContextMenuUsed; };

	/**
	 * @brief ���ÿؼ���Ӧ�����Ĳ˵���Ϣ
	 * @param[in] bMenuUsed �Ƿ���Ӧ�����Ĳ˵���Ϣ��true Ϊ�ǣ�false Ϊ��
	 * @return ��
	 */
    virtual void SetContextMenuUsed(bool bMenuUsed);

    /// �û����ݣ��������������û�ʹ��
	/**
	 * @brief ��ȡ�û��󶨵��ؼ��������ַ���
	 * @return ���ؾ��������ַ���
	 */
    virtual std::wstring GetDataID() const;

	/**
	 * @brief ��ȡ�û��󶨵��ؼ��������ַ�����UTF8 ��ʽ��
	 * @return ���ؾ��������ַ���
	 */
	virtual std::string GetUTF8DataID() const;				

	/**
	 * @brief ��һ���ַ������ݵ��ؼ�
	 * @param[in] strText Ҫ�󶨵��ַ�������
	 * @return ��
	 */
    virtual void SetDataID(const std::wstring& strText);

	/**
	 * @brief ��һ���ַ������ݵ��ؼ���UTF8 ��ʽ��
	 * @param[in] strText Ҫ�󶨵��ַ�������
	 * @return ��
	 */
	virtual void SetUTF8DataID(const std::string& strText);	

    /// һЩ��Ҫ������
	/**
	 * @brief �Ե��뵭���ȶ�����ʽ���ÿؼ��Ƿ�ɼ�, ���õĽ����SetVisible��ͬ��ֻ�ǹ��̰����˶���Ч����
	          ����SetFadeVisible�Ժ󣬲���Ҫ�ٵ���SetVisible�����޸Ŀɼ����ԡ�
			  �ú����ڲ������SetVisible����麯����
	 * @param[in] bVisible Ϊ true ʱ�ؼ��ɼ���Ϊ false ʱ�ؼ�������
	 * @return ��
	 */
    virtual void SetFadeVisible(bool bVisible);

	/** ���ÿؼ��Ƿ�ɼ�
	 * @param[in] @param[in] bVisible Ϊ true ʱ�ؼ��ɼ���Ϊ false ʱ�ؼ�������
	 */
	virtual void SetVisible(bool bVisible) override;

	/**
	 * @brief ���ؼ��Ƿ����
	 * @return �ؼ�����״̬������ true �ؼ����ã�����Ϊ false
	 */
	virtual bool IsEnabled() const { return m_bEnabled; };

	/**
	 * @brief ���ÿؼ�����״̬
	 * @param[in] bEnable Ϊ true ʱ�ؼ����ã�Ϊ false ʱ�ؼ�Ϊ����״̬�򲻿���
	 * @return ��
	 */
    virtual void SetEnabled(bool bEnable = true);

	/**
	 * @brief ���ؼ��Ƿ���Ӧ����¼�
	 * @return ���ؿؼ��Ƿ���Ӧ����¼������� true ��Ӧ����¼���false Ϊ����Ӧ
	 */
	virtual bool IsMouseEnabled() const { return m_bMouseEnabled; };

	/**
	 * @brief ���ÿؼ��Ƿ���Ӧ����¼�
	 * @param[in] bEnable Ϊ true ��Ӧ����¼���Ϊ false ʱ����Ӧ����¼�
	 * @return ��
	 */
    virtual void SetMouseEnabled(bool bEnable);

	/**
	 * @brief ���ؼ��Ƿ���Ӧ�����¼�
	 * @return ���ؿؼ��Ƿ���Ӧ�����¼������� true ��Ӧ�����¼���false ����Ӧ�����¼�
	 */
	virtual bool IsKeyboardEnabled() const { return m_bKeyboardEnabled; };

	/**
	 * @brief ���ÿؼ��Ƿ���Ӧ�����¼�
	 * @param[in] bEnable Ϊ true ��Ӧ�����¼���Ϊ false ʱ����Ӧ�����¼�
	 * @return ��
	 */
    virtual void SetKeyboardEnabled(bool bEnable);

	/**
	 * @brief ���ؼ��Ƿ���н���
	 * @return ���ؿؼ��Ƿ���м�㣬Ϊ true ʱ�ǵ�ǰ���н��㣬Ϊ false ʱ�ؼ�û�н���
	 */
    virtual bool IsFocused() const;

	/**
	 * @brief �ÿؼ���ȡ����
	 * @return ��
	 */
    virtual void SetFocus();

	/**
	 * @brief ���ؿؼ��ı�ʶ�������ж��Ƿ������Ӧ TAB �л��¼�
	 * @return ���ؿؼ��ı�ʶ����
	 */
	virtual UINT GetControlFlags() const;

	/**
	 * @brief �ÿؼ�������Զ��ȡ��������
	 * @return ��
	 */
	void SetNoFocus(); // �ؼ���Զ��Ҫ���㣬��KillFocus��һ��

	/**
	 * @brief �жϵ�ǰ��꽹���Ƿ��ڿؼ���
	 * @return ������꽹���Ƿ��ڿؼ��ϣ�true ��꽹���ڿؼ��ϣ�false ��꽹�㲻�ڿؼ���
	 */
	virtual bool IsMouseFocused() const { return m_bMouseFocused;}

	/**
	 * @brief �����Ƿ���꽹�㵽�ؼ���
	 * @param[in] bMouseFocused Ϊ true ʱ������꽹�㵽�ؼ��ϣ�Ϊ false ʱ�ÿؼ�ʧȥ��꽹��
	 * @return ��
	 */
	virtual void SetMouseFocused(bool bMouseFocused) { m_bMouseFocused = bMouseFocused; }

	/**
	 * @brief �жϿؼ���ǰ�Ƿ��ǿɼ���״̬
	 * @return ���ؿؼ�״̬��true �ؼ���ǰ�ǿɼ���״̬���ɼ������ã�false �ؼ���ǰ�ǿɼ���״̬�����ܲ��ɼ��򱻽���
	 */
	virtual bool IsActivatable() const;

	/**
	 * @brief ����ؼ���������ѡ�С�չ���Ȳ���
	 * @param[in] ������
	 * @return ������
	 */
	virtual void Activate();

	/// �ؼ�����
	/**
	 * @brief �����������ָ���ؼ�
	 * @param[in] Proc
	 * @param[in] pData
	 * @param[in] uFlags
	 * @param[in] scrollPos
	 * @return ���ؿؼ���ָ��
	 */
    virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos = UiPoint());

	/// λ�����
	/**
	 * @brief ��ȡ�ؼ�λ��
	 * @param[in] bContainShadow �Ƿ������Ӱ��Ĭ��Ϊ true �������Ӱ��Χ������ false Ϊ������
	 * @return ���ؿؼ���λ����Ϣ
	 */
	virtual	UiRect GetPos(bool bContainShadow = true) const override;

	/**
	 * @brief ���ÿؼ�λ��
	 * @param[in] rc �ؼ�λ�õ�������Ϣ
	 * @return ��
	 */
	virtual void SetPos(UiRect rc) override;

	/**
	 * @brief ��ȡ�ؼ�����߾�
	 * @return ���ؿؼ�����߾�
	 */
	virtual UiRect GetMargin() const;

	/**
	 * @brief ���ÿؼ�����߾�
	 * @param[in] rcMargin �ؼ�����߾���Ϣ
	 * @param[in] bNeedDpiScale �Ƿ�����߾���� DPI ���䣬Ĭ��Ϊ true��false ������ DPI
	 * @return ��
	 */
	virtual void SetMargin(UiRect rcMargin, bool bNeedDpiScale);

	/** ����ؼ���С(��͸�)
	    ���������ͼƬ������ width �� height ����һ��Ϊ auto��������ͼƬ���������մ�С
	 * @param[in] szAvailable ���õĴ�С
	 * @return �ؼ������С
	 */
	virtual UiSize EstimateSize(UiSize szAvailable) override;

	/** �����ı������С����͸ߣ�
	   @param[in] szAvailable ���õĴ�С
	 * @return �ؼ������С
	 */
	virtual UiSize EstimateText(UiSize szAvailable);

	/**
	 * @brief ���ָ�������Ƿ��ڹ�������ǰ����λ�õķ�Χ��
	 * @param[in] point ��������
	 * @return �����Ƿ��ڷ�Χ�ڣ�true �ڹ�������ǰ����λ�÷�Χ�ڣ�false ���ڹ�������ǰ����λ�÷�Χ��
	 */
	virtual bool IsPointInWithScrollOffset(const UiPoint& point) const;

	// ��Ϣ����
	/** @brief �ؼ�ͳһ����Ϣ������ڣ�����ͳ Windows ��Ϣת��Ϊ�Զ����ʽ����Ϣ
	 * @param[in] eventType	��Ϣ����
	 * @param[in] wParam	��Ϣ��������
	 * @param[in] lParam	��Ϣ��������
	 * @param[in] tChar		������Ϣ
	 * @param[in] mousePos	�����Ϣ
	 */
	virtual void SendEvent(EventType eventType,
			        	   WPARAM wParam = 0, 
						   LPARAM lParam = 0, 
						   TCHAR tChar = 0, 
						   const UiPoint& mousePos = UiPoint());

	/** @brief ��ת�������Ϣ�ɷ�����Ϣ������
	 * @param[in] msg ��Ϣ����
	 */
    virtual void SendEvent(const EventArgs& msg);

	/**
	 * @brief �жϿؼ��Ƿ��� HOT ״̬
	 * @return ���� true �� HOT ״̬�� ����Ϊ false
	 */
	virtual bool HasHotState();

	// ��������
	/**
	 * @brief ���ÿؼ�ָ������
	 * @param[in] strName Ҫ���õ��������ƣ��� width��
	 * @param[in] strValue Ҫ���õ�����ֵ���� 100��
	 * @return ��
	 */
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief ���ÿؼ��� class ȫ������
	 * @param[in] strClass Ҫ���õ� class ���ƣ������Ʊ����� global.xml �д���
	 * @return ��
	 */
	void SetClass(const std::wstring& strClass);

	/**
	 * @brief Ӧ��һ�������б�
	 * @param[in] strList �����б���ַ�����ʾ���� `width="100" height="30"`
	 * @return ��
	 */
    void ApplyAttributeList(const std::wstring& strList);

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	bool OnApplyAttributeList(const std::wstring& strReceiver, const std::wstring& strList, const EventArgs& eventArgs);

	/// ���Ʋ���
	/**
	 * @brief ����ͼƬ
	 * @param[in] pRender ����������
	 * @param[in] duiImage ͼƬ����
	 * @param[in] isLoadingImage true��ʾLoading Image������ʱ����ת��ͼƬ;false��ʾ����ͼƬ
	 * @param[in] strModify ͼƬ�ĸ�������
	 * @param[in] nFade �ؼ���͸���ȣ�������ö���Ч����ֵ�ڻ���ʱ�ǲ��ϱ仯��
	 * @param[in] pMatrix ����ͼƬʱʹ�õı任����
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool PaintImage(IRender* pRender, Image& duiImage,
				    const std::wstring& strModify = L"",
		            int nFade = DUI_NOSET_VALUE,
		            IMatrix* pMatrix = nullptr);

	/**
	* @brief ��ȡ���������Ķ���
	* @return ���ػ��������Ķ���
	*/
	IRender* GetRender();

	/**
	* @brief ������������Ķ���
	* @return ��
	*/
	void ClearRender();

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual void AlphaPaint(IRender* pRender, const UiRect& rcPaint);
	
	/**
	* @brief ���ƿؼ�����ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void Paint(IRender* pRender, const UiRect& rcPaint);

	/**
	* @brief ���ƿؼ�������ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) { (void)pRender; (void)rcPaint; };

	/**
	* @brief �����Ƿ�Ի��Ʒ�Χ����������
	* @param[in] clip ���� true Ϊ��Ҫ������Ϊ����Ҫ�������ƺ���
	* @return ��
	*/
	void SetClip(bool clip) { m_bClip = clip; };

	/**
	* @brief �ж��Ƿ�Ի��Ʒ�Χ����������
	* @return ���� true Ϊ��Ҫ��false Ϊ����Ҫ
	*/
	bool IsClip() const { return m_bClip; }

	/**
	 * @brief ���ÿؼ�͸����
	 * @param[in] alpha 0 ~ 255 ��͸����ֵ��255 Ϊ��͸��
	 * @return ��
	 */
	void SetAlpha(int alpha);

	/**
	 * @brief ��ȡ�ؼ�͸����
	 * @return ���ؿؼ���͸����
	 */
	int GetAlpha() const { return m_nAlpha;	}

	/**
	 * @brief ���ؼ��Ƿ���͸������
	 * @return ���ؿؼ��Ƿ�͸����true �ؼ���ǰ��͸�����ԣ�false �ؼ�û��͸������
	 */
	bool IsAlpha() const { return m_nAlpha != 255; }

	/**
	 * @brief ���ý���״̬͸����
	 * @param[in] alpha 0 ~ 255 ��͸����ֵ��255 Ϊ��͸��
	 * @return ��
	 */
	void SetHotAlpha(int nHotAlpha);

	/**
	 * @brief �����Ƿ����TAB���л�����
	 * @param[in] enable
	 * @return ��
	 */
	void SetTabStop(bool enable);

	/**
	 * @brief ����Ƿ����TAB���л�����
	 * @return ���ؿؼ��Ƿ����TAB���л�����
	 */
	bool IsAllowTabStop() const { return m_bAllowTabstop; }

	/**
	 * @brief ��ȡ����״̬͸����
	 * @return ���ؿؼ�����״̬��͸����
	 */
	int GetHotAlpha() const { return m_nHotAlpha; }

	/**
	 * @brief ��ȡ�ؼ�����ƫ����
	 * @return ���ص�ǰ�ؼ��Ļ���ƫ����
	 */
	UiPoint GetRenderOffset() const { return m_renderOffset;	}

	/**
	 * @brief ���ÿؼ�����ƫ����
	 * @param[in] renderOffset �ؼ�ƫ������
	 * @return ��
	 */
	void SetRenderOffset(UiPoint renderOffset);

	/**
	 * @brief ���ÿؼ�ƫ�Ƶ� X ����
	 * @param[in] renderOffsetX X ����ֵ
	 * @return ��
	 */
	void SetRenderOffsetX(int renderOffsetX);

	/**
	 * @brief ���ÿؼ�ƫ�Ƶ� Y ����
	 * @param[in] renderOffsetY Y ����ֵ
	 * @return ��
	 */
	void SetRenderOffsetY(int renderOffsetY);

	/// GifͼƬ
	/**
	 * @brief ���� GIF
	 * @param[in] �������ֹͣ����һ֡�������õ�һ֡����ǰ֡�����һ֡����ο� GifStopType ö��
	 * @return ��
	 */
	void StartGifPlayForUI(GifStopType frame = kGifStopFirst,int playcount = -1);

	/**
	 * @brief ֹͣ���� GIF
	 * @param[in] transfer �Ƿ�ֹͣ�¼�֪ͨ�������ߣ��ο� AttachGifPlayStop ����
	 * @param[frame] frame ���Ž���ֹͣ����һ֡�������õ�һ֡����ǰ֡�����һ֡����ο� GifStopType ö��
	 * @return ��
	 */
	void StopGifPlayForUI(bool transfer = false, GifStopType frame = kGifStopCurrent);

	/**
	 * @brief ���� GIF �������֪ͨ
	 * @param[in] callback Ҫ���� GIF ֹͣ���ŵĻص�����
	 * @return ��
	 */
	void AttachGifPlayStop(const EventCallback& callback){ m_OnGifEvent[m_nVirtualEventGifStop] += callback; };

    /**
    * @brief ����loading״̬
    * @param[in] start_angle loadingͼƬ��ת�ĽǶ�
    * @return ��
    */
    void StartLoading(int fStartAngle = -1);

    /**
    * @brief �ر�loading״̬
    * @param[in] frame �������ֹͣ����һ֡�������õ�һ֡����ǰ֡�����һ֡����ο� GifStopType ö��
    * @return ��
    */
    void StopLoading(GifStopType frame = kGifStopFirst);

    /**
    * @brief ����loadingͼƬ����ת�Ƕ�
    * @return ��
    */
    void Loading();
    /**
    * @brief �Ƿ�����loading
    * @return ��loading����true, ��֮����false
    */
    bool IsLoading();

	/** @brief ��ȡ�����������ӿ�
	 */
	AnimationManager& GetAnimationManager() const;

	/// ͼƬ����
	/**@brief ����ͼƬ·��, ����ͼƬ��Ϣ�������С�
	 *        ���ز��ԣ����ͼƬû�м�����ִ�м���ͼƬ�����ͼƬ·�������仯�������¼��ظ�ͼƬ��
	 * @param[in��out] duiImage ����ʱ��עͼƬ��·����Ϣ������ɹ���Ỻ��ͼƬ����¼���ò����ĳ�Ա��
	 */
	bool LoadImageData(Image& duiImage) const;

	/**@brief ����ͼƬ������
	 */
	virtual void InvokeLoadImageCache();

	/**@brief ȡ������ͼƬ����
	 */
	virtual void UnLoadImageCache();

	/**@brief ����ͼƬ����
	 */
	virtual void ClearImageCache();

	/**
	 * @brief �����ؼ������¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachAllEvents(const EventCallback& callback)	{ AttachEvent(kEventAll, callback); }

	/**
	 * @brief �����������¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachMouseEnter(const EventCallback& callback) { AttachEvent(kEventMouseEnter, callback); }

	/**
	 * @brief ��������뿪�¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachMouseLeave(const EventCallback& callback) { AttachEvent(kEventMouseLeave, callback); }

	/**
	 * @brief ������������¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachMouseHover(const EventCallback& callback) { AttachEvent(kEventMouseHover, callback); }

	/**
	 * @brief ������갴���¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachButtonDown(const EventCallback& callback) { AttachEvent(kEventMouseButtonDown, callback); }

	/**
	 * @brief ������굯���¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachButtonUp(const EventCallback& callback) { AttachEvent(kEventMouseButtonUp, callback); }

	/**
	 * @brief ������ý����¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachSetFocus(const EventCallback& callback) { AttachEvent(kEventSetFocus, callback); }

	/**
	 * @brief ����ʧȥ�����¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachKillFocus(const EventCallback& callback) { AttachEvent(kEventKillFocus, callback); }

	/**
	 * @brief �����Ҽ��˵��¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachMenu(const EventCallback& callback) { AttachEvent(kEventMouseMenu, callback); }

	/**
	 * @brief �����ؼ���С�ı��¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachResize(const EventCallback& callback) { AttachEvent(kEventResize, callback); }

	/**
	 * @brief ����˫���¼�
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 * @return ��
	 */
	void AttachDoubleClick(const EventCallback& callback) { AttachEvent(kEventMouseDoubleClick, callback); }

	/**
	* @brief �������������
	* @param[in] callback Ҫ�󶨵Ļص�����
	* @return ��
	*/
	void AttachClick(const EventCallback& callback) { AttachEvent(kEventClick, callback); }

	/**
	* @brief �����ؼ��ر�ǰ���һ����Ϣ
	* @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	* @return ��
	*/
	void AttachLastEvent(const EventCallback& callback) { AttachEvent(kEventLast, callback); }

	/**
	* @brief �����ؼ���ʾ�������¼�
	* @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	* @return ��
	*/
	void AttachVisibleChange(const EventCallback& callback) { AttachEvent(kEventVisibleChange, callback); }

	/** �ж���ɫ�����Ƿ���Ч
	* @param [in] colorName ��ɫ�����ƣ���Ч����ɫ���ƿ��������¶��壨����ȡ���ȼ�˳�򣩣�
	*           (1) ���ȼ�1����'#'�ַ���ͷ��ֱ��ָ����ɫֵ��������#FFFFFFFF       
	*		    (2) ���ȼ�2��������XML�е�<Window>�ڵ��ж����ӽڵ㣬������<TextColor name="wnd_darkcolor" value="#FF00BB96"/>
	*           (3) ���ȼ�3����global.xml�е�<Global>�ڵ��ж����ӽڵ㣬������<TextColor name="white" value="#FFFFFFFF"/>                    
	*           (4) ���ȼ�4���μ�ui::UiColors::UiColorConsts�����еĶ���
	*/
	bool HasUiColor(const std::wstring& colorName) const;

	/** ��ȡĳ����ɫ��Ӧ��ֵ
	* @param [in] colorName ��ɫ�����ƣ���Ч����ɫ���ƿ��������¶��壨����ȡ���ȼ�˳�򣩣�
	*           (1) ���ȼ�1����'#'�ַ���ͷ��ֱ��ָ����ɫֵ��������#FFFFFFFF
	*		    (2) ���ȼ�2��������XML�е�<Window>�ڵ��ж����ӽڵ㣬������<TextColor name="wnd_darkcolor" value="#FF00BB96"/>
	*           (3) ���ȼ�3����global.xml�е�<Global>�ڵ��ж����ӽڵ㣬������<TextColor name="white" value="#FFFFFFFF"/>                    
	*           (4) ���ȼ�4��ֱ��ָ��Ԥ�������ɫ�������μ�ui::UiColors::UiColorConsts�����еĶ���
	* @return ARGB��ɫֵ
	*/
    UiColor GetUiColor(const std::wstring& colorName) const;

	/** �жϿؼ������Ƿ�Ϊ��ѡ���
	 * @return Ĭ�Ϸ���false
	 */
	virtual bool IsSelectableType() const;

	/** �ж��Ƿ���� TAB ������Ϣ
	 * @return ���� true ��ʾ���ܣ�false ��ʾ�����ܣ� Ĭ�Ϸ���false
	 */
	virtual bool IsWantTab() const;

	/** �ÿؼ��Ƿ���Է����ڱ������ϣ������ڴ���NC��Ϣ��Ӧ��
	 * @return ���� true ��ʾ���ԣ�false ��ʾ�����ԣ� Ĭ�Ϸ���false
	 */
	virtual bool CanPlaceCaptionBar() const;

	/** ��ǰ�ؼ��Լ������������Ƿ�ɼ����ӵ�ǰ�ؼ�����㸸�ؼ���ֻҪ��һ��VisibleΪfalse���򷵻�false��
	* @return true ��ʾ��ǰ�ؼ��Լ����и��ؼ������ǿɼ�״̬
	*         false ��ʾ��ǰ�ؼ��������и��ؼ��У��зǿɼ�״̬�Ŀؼ�
	 */
	bool CheckVisibleAncestor(void) const;

public:
	/**@name �¼�������ؽӿ�
	* @{
	*/

	/**@brief (m_OnEvent)����ָ���¼�
	 * @param[in] type �¼����ͣ��� EventType ö��
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 */
	void AttachEvent(EventType type, const EventCallback& callback);

	/**@brief (m_OnEvent)ȡ������ָ���¼�
	 * @param[in] type �¼����ͣ��� EventType ö��
	 */
	void DetachEvent(EventType type);

	/**@brief (m_OnXmlEvent)ͨ��XML�У�����<Event��ǩ��ӵ���Ӧ�¼���������Control::OnApplyAttributeList������Ӧ�������
	 * @param[in] type �¼����ͣ��� EventType ö��
	 * @param[in] callback �¼�����Ļص���������ο� EventCallback ����
	 */
	void AttachXmlEvent(EventType eventType, const EventCallback& callback);

	/**@brief (m_OnXmlEvent)ȡ������ָ���¼�
	 * @param[in] type �¼����ͣ��� EventType ö��
	 */
	void DetachXmlEvent(EventType type);

	/**@brief (m_OnBubbledEvent)���¼�������
	 * @param[in] eventType �¼�����
	 * @param[in] callback ָ���ص�����
	 */
	void AttachBubbledEvent(EventType eventType, const EventCallback& callback);

	/**@brief (m_OnBubbledEvent)����¼�������
	 * @param[in] eventType �¼�����
	 */
	void DetachBubbledEvent(EventType eventType);

	/** @brief �� XML �б�д�� Event �� BubbleEvent �¼��Ĵ�����
	 * @param[in] eventType �¼�����
	 * @param[in] callback ָ���ص�����
	 */
	void AttachXmlBubbledEvent(EventType eventType, const EventCallback& callback);

	/** @brief ���XML�¼�������
	 * @param[in] eventType �¼�����
	 */
	void DetachXmlBubbledEvent(EventType eventType);

	/** �����¼��������������ļ����߷����¼���m_OnEvent��m_OnXmlEvent��m_OnBubbledEvent�� m_OnXmlBubbledEvent��
	* @param [in] msg ��Ϣ����
	* @return ������м����߻ص���������true����ú�������true�����򷵻�false
	*/
	bool FireAllEvents(const EventArgs& msg);

	/** @} */

protected:
	
	/// Gif�ȶ�֡ͼƬ�����Ŷ���
	bool GifPlay();
	void StopGifPlay(GifStopType frame = kGifStopCurrent);

	//��������ؼ���������߼�
	void EnsureNoFocus();
	
	/// ��Ϣ����ı�����Ա�������������ⲿֱ�ӵ���
	virtual void HandleEvent(const EventArgs& msg);
	virtual bool MouseEnter(const EventArgs& msg);
	virtual bool MouseLeave(const EventArgs& msg);
	virtual bool ButtonDown(const EventArgs& msg);
	virtual bool ButtonUp(const EventArgs& msg);

	/// ������ر�����Ա�������������ⲿֱ�ӵ���
	virtual void PaintShadow(IRender* pRender);
	virtual void PaintBkColor(IRender* pRender);
	virtual void PaintBkImage(IRender* pRender);
	virtual void PaintStatusColor(IRender* pRender);
	virtual void PaintStatusImage(IRender* pRender);
	virtual void PaintText(IRender* pRender);
	virtual void PaintBorder(IRender* pRender);
	virtual void PaintLoading(IRender* pRender);

protected:
	/** @brief ��ȡ�ؼ�ͼƬ������״̬ͼƬ��ӳ��ӿ�
	*/
	StateImageMap* GetImageMap() const { return m_imageMap.get(); }

	/** @brief ��ȡ�ؼ�״̬����ɫֵ��ӳ��ӿ�
	*/
	StateColorMap* GetColorMap() const { return m_colorMap.get(); }

	/** @brief ��ȡ�ؼ��Ļ�������
	*/
	const UiRect& GetPaintRect() const { return m_rcPaint; }

	/** @brief ���ÿؼ��Ļ�������
	*/
	void SetPaintRect(const UiRect& rect);

private:
	void BroadcastGifEvent(int nVirtualEvent);
	size_t GetGifFrameIndex(GifStopType frame);

	/** ���Ʊ߿򣺸��������жϻ���Բ�Ǿ��α߿�����ͨ���α߿�
	*/
	void PaintBorders(IRender* pRender, UiRect rcDraw, 
		              int nBorderSize, UiColor dwBorderColor) const;

	/** ����Բ�Ǿ���
	*/
	void DrawRoundRect(IRender* pRender, const UiRect& rc, const UiSize& roundSize, UiColor dwBorderColor, int nBorderSize) const;

	/** ���Բ�Ǿ���
	*/
	void FillRoundRect(IRender* pRender, const UiRect& rc, const UiSize& roundSize, UiColor dwColor) const;

	/** ���·��, �γ�Բ�Ǿ���
	*/
	void AddRoundRectPath(IPath* path, const UiRect& rc, const UiSize& roundSize) const;

	/** ��ǰ�ؼ��Ƿ�Ϊ���ڵ�Root�ڵ�
	*/
	bool IsRootBox() const;

	/** ��ǰWindow�Ƿ�ΪԲ�ǵ�(����ȷ���Ƿ������Windowsһ����Բ�ǻ��Ʒ�ʽ)
	*/
	bool IsWindowRoundRect() const;

	/** �ж��Ƿ���Ҫ����Բ�Ǿ��λ��Ʊ߿�
	*/
	bool ShouldBeRoundRectBorders() const;

public:
	/** �ж��Ƿ���Ҫ����Բ�Ǿ�����䱳��ɫ
	*/
	bool ShouldBeRoundRectFill() const;

private:

	/** ��ȡ��ɫ���ƶ�Ӧ����ɫֵ
	*/
	UiColor GetUiColorByName(const std::wstring& colorName) const;

private:
	//�ؼ�����߾����ԣ��ϣ��£����ұ߾ࣩ
	UiRect m_rcMargin;

private:
	/** �߿�Բ�Ǵ�С(��m_rcBorderSize����Ӧ��)������Ӱ��Բ�Ǵ�С(��m_boxShadow����Ӧ��)
	    ���� m_rcBorderSize �ĸ��߿�ֵ����Ч, ���Ҷ���ͬʱ
	*/
	UiSize m_cxyBorderRound;

	//�ؼ���Ӱ����Բ�Ǵ�Сͨ��m_cxyBorderRound��������
	BoxShadow m_boxShadow;

	//�߿���ɫ
	std::wstring m_strBorderColor;

	//�ؼ��ıߵı߿��С���ɷֱ�����top/bottom/left/right�ĸ��ߵ�ֵ��
	UiRect m_rcBorderSize;

private:
	//�ؼ��ı�����ɫ
	std::wstring m_strBkColor;

	//�ؼ��ı���ͼƬ
	std::unique_ptr<Image> m_bkImage;

	//������״̬ͼƬ(m_bkImage)���������ڹ���ȡ������
	nbase::WeakCallbackFlag m_loadBkImageWeakFlag;

	//�Ƿ�Ϊ����GIF��״̬��������ͼƬm_bkImage��GIF�ļ�ʱ���������߼���
	bool m_bGifPlay;

	//GIF����ͼƬ���ŵ�ȡ������
	nbase::WeakCallbackFlag m_gifWeakFlag;

    //GIF�����¼��Ļص�ע���������(Ŀǰֻ�в������һ���¼�)
	GifEventMap m_OnGifEvent;

	//GIF����ͼƬ��������¼���ID
	const int m_nVirtualEventGifStop = 1;	

private:
	/** �ؼ�״̬
	*/
	ControlStateType m_controlState;

	/** ״̬����ɫֵMAP
	*/
	std::unique_ptr<StateColorMap> m_colorMap;

	/** �ؼ�ͼƬ������״̬ͼƬ��MAP
	*/
	std::unique_ptr<StateImageMap> m_imageMap;

private:
	//�Ƿ��ڼ����е�״̬
	bool m_bIsLoading;

	//������ͼƬ��ת�ĽǶȣ�0-360��
	int m_fCurrrentAngele;

	//������״̬ʱ��ʾ��ͼƬ
	std::unique_ptr<Image> m_loadingImage;

	//������״̬ʱ�ı�����ɫ
	std::wstring m_strLoadingBkColor;

	//������״̬ͼƬ(m_loadingImage)���������ڹ���ȡ������
	nbase::WeakCallbackFlag m_loadingImageFlag;

private:
	//�ؼ��������Ź�����
	std::unique_ptr<AnimationManager> m_animationManager;

	//�ؼ����Ŷ���ʱ����Ⱦƫ��(X����ƫ�ƺ�Y����ƫ��)
	UiPoint m_renderOffset;
	
	//�ؼ���͸���ȣ�0 - 255��0Ϊ��ȫ͸����255Ϊ��͸����
	int m_nAlpha;

	//�ؼ�ΪHot״̬ʱ��͸���ȣ�0 - 255��0Ϊ��ȫ͸����255Ϊ��͸����
	int m_nHotAlpha;

	//�Ƿ�Ի��Ʒ�Χ����������
	bool m_bClip;

	//�ؼ��Ļ�������
	UiRect m_rcPaint;

	//������Ⱦ����ӿ�
	std::unique_ptr<IRender> m_render;

	//box-shadow�Ƿ��Ѿ����ƣ�����box-shadow���ƻᳬ��GetRect()��Χ��������Ҫ���⴦��
	bool m_isBoxShadowPainted;

private:
	//ToolTip�Ŀ��
	int m_nTooltipWidth;

	//ToolTip���ı�����
	std::wstring m_sToolTipText;

	//ToolTip���ı�ID
	std::wstring m_sToolTipTextId;
	
	//�û�����ID
	std::wstring m_sUserDataID;

private:
	//ͨ��AttachXXX�ӿڣ���ӵļ����¼�
	EventMap m_OnEvent;

	//ͨ��XML�У�����<Event��ǩ��ӵ���Ӧ�¼���������Control::OnApplyAttributeList������Ӧ�������
	EventMap m_OnXmlEvent;

	//ͨ��AttachBubbledEvent�ӿ���ӵ��¼�
	EventMap m_OnBubbledEvent;

	//ͨ��XML�У�����<BubbledEvent��ǩ��ӵ���Ӧ�¼���������Control::OnApplyAttributeList������Ӧ�������
	EventMap m_OnXmlBubbledEvent;

private:
	//�ؼ���Enable״̬����Ϊfalse��ʱ�򣬲���Ӧ��ꡢ���̵�������Ϣ��
	bool m_bEnabled;

	//�����Ϣ��Enable״̬����Ϊfalse��ʱ�򣬲���Ӧ�����Ϣ��
	bool m_bMouseEnabled;

	//������Ϣ��Enable״̬����Ϊfalse��ʱ�򣬲���Ӧ������Ϣ��
	bool m_bKeyboardEnabled;

	//��꽹���Ƿ��ڿؼ���
	bool m_bMouseFocused;

	//�ؼ��Ƿ���Ӧ�����Ĳ˵�
	bool m_bContextMenuUsed;

	//�ؼ�����Ҫ���㣨���Ϊtrue����ؼ������ý��㣩
	bool m_bNoFocus;

	//�Ƿ�����TAB�л�����
	bool m_bAllowTabstop;

	//�ؼ��Ĺ������
	CursorType m_cursorType;
};

} // namespace ui

#endif // UI_CORE_CONTROL_H_
