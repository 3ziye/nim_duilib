#ifndef UI_CORE_CONTROL_H_
#define UI_CORE_CONTROL_H_

#pragma once

#include "duilib/Core/PlaceHolder.h"
#include "duilib/Utils/Delegate.h"
#include "duilib/Utils/BoxShadow.h"
#include <map>
#include <memory>

namespace ui 
{
	class Control;
	class Image;
	class StateColorMap;
	class StateImageMap;
	class AnimationManager;
	class IRenderContext;

	typedef Control* (CALLBACK* FINDCONTROLPROC)(Control*, LPVOID);

class UILIB_API UserDataBase
{
public:
	virtual ~UserDataBase() = default;
};

#if defined(ENABLE_UIAUTOMATION)
	class UIAControlProvider;
#endif

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
	/**
	 * @brief ��ȡ������ɫ
	 * @return ���ر�����ɫ���ַ�������ֵ�� global.xml �ж���
	 */
	std::wstring GetBkColor() const;

	/**
	 * @brief ���ñ�����ɫ
	 * @param[in] strColor Ҫ���õı�����ɫֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetBkColor(const std::wstring& strColor);

	/**
	 * @brief ��ȡĳ��״̬�µ�������ɫ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫֵ���ο� ControlStateType ö��
	 * @return ָ��״̬���趨����ɫ�ַ�������Ӧ global.xml ��ָ��ɫֵ
	 */
	std::wstring GetStateColor(ControlStateType stateType);

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

	/**
	 * @brief ��ȡ�ؼ�״̬
	 * @return �ؼ���״̬����ο� `ControlStateType` ö��
	 */
	ControlStateType GetState() const;

	/**
	 * @brief ���ÿؼ�״̬
	 * @param[in] pStrState Ҫ���õĿؼ�״̬����ο� `ControlStateType` ö��
	 * @return ��
	 */
	void SetState(ControlStateType pStrState);

	/**
	 * @brief ��ȡ�ؼ�ͼƬָ��
	 * @return ����ͼƬ����ָ��
	 */
	virtual Image* GetEstimateImage();

	/// �߿����
	/**
	 * @brief ��ȡ�߿��С
	 * @return ���ر߿�Ĵ�С
	 */
	int GetBorderSize() const;

	/**
	 * @brief ���ñ߿��С
	 * @param[in] nSize Ҫ���õı߿��С
	 * @return ��
	 */
	void SetBorderSize(int nSize);

	/**
	 * @brief ��ȡ�߿���ɫ
	 * @return �߿����ɫ�ַ�������Ӧ global.xml �еľ�����ɫֵ
	 */
	std::wstring GetBorderColor() const;

	/**
	 * @brief ���ñ߿���ɫ
	 * @param[in] strBorderColor ���ñ߿����ɫ�ַ���ֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetBorderColor(const std::wstring& strBorderColor);

	/**
	 * @brief ���ñ߿�Ĵ�С
	 * @param[in] rc һ�� `UiRect` �ṹ�ı߿��С����
	 * @return ��
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

	/**
	 * @brief �����·��߿��С
	 * @param[in] nSize Ҫ���õ��·��߿��С
	 * @return ��
	 */
	void SetBottomBorderSize(int nSize);

	/**
	 * @brief ��ȡ�߿��С
	 * @return �ĸ�����ı߿��С
	 */
	UiSize GetBorderRound() const;

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
	 * @return ��ǰ�������
	 */
	virtual CursorType GetCursorType() const;

	/**
	 * @brief ���õ�ǰ���ָ������
	 * @param[in] flag Ҫ���õ�������ͣ��ο� CursorType ö��
	 * @return ��
	 */
	void SetCursorType(CursorType flag);

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

	/**
	 * @brief ��ȡ�û��󶨵��Զ������ݽṹ
	 * @return �û��󶨵��Զ���ṹ����ָ��
	 */
	virtual UserDataBase* GetUserDataBase() const;

	/**
	 * @brief ���Զ������ݵ��ؼ����û��ɼ̳� UserDataBase ��������Ҫ�󶨵�����
	 * @param[in] pUserDataBase ����ָ��
	 * @return ��
	 */
	virtual void SetUserDataBase(UserDataBase* pUserDataBase);

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
    virtual void SetMouseEnabled(bool bEnable = true);

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
    virtual void SetKeyboardEnabled(bool bEnable = true);

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
	virtual void SetMargin(UiRect rcMargin, bool bNeedDpiScale = true);

	/**
	 * @brief ����ؼ���С
	 * @param[in] szAvailable ��������
	 * @return szAvailable �ؼ�ʵ�ʴ�С�����������ͼƬ������ width �� height ����һ��Ϊ auto��������ͼƬ���������մ�С��
	 */
	virtual UiSize EstimateSize(UiSize szAvailable) override;

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual UiSize EstimateText(UiSize szAvailable, bool& bReEstimateSize);

	/**
	 * @brief ���ָ�������Ƿ��ڹ�������ǰ����λ�õķ�Χ��
	 * @param[in] point ��������
	 * @return �����Ƿ��ڷ�Χ�ڣ�true �ڹ�������ǰ����λ�÷�Χ�ڣ�false ���ڹ�������ǰ����λ�÷�Χ��
	 */
	virtual bool IsPointInWithScrollOffset(const UiPoint& point) const;

#if defined(ENABLE_UIAUTOMATION)
	/**
	 * @brief Get ui automation provider 
	 * @return nullptr or pointer
	 */
	virtual UIAControlProvider* GetUIAProvider();
#endif

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
	 * @brief ����ͼƬ·������ͼƬ��Ϣ
	 * @param[in��out] duiImage ����ʱ��עͼƬ��·����Ϣ������ɹ���Ỻ��ͼƬ����¼���ò����ĳ�Ա��
	 * @return ��
	 */
	void GetImage(Image& duiImage) const;

	/**
	 * @brief ����ͼƬ
	 * @param[in] pRender ����������
	 * @param[in] duiImage ͼƬ����
	 * @param[in] strModify ͼƬ�ĸ�������
	 * @param[in] nFade �ؼ���͸���ȣ�������ö���Ч����ֵ�ڻ���ʱ�ǲ��ϱ仯��
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	bool DrawImage(IRenderContext* pRender, Image& duiImage, const std::wstring& strModify = L"", int nFade = DUI_NOSET_VALUE);

	/**
	* @brief ��ȡ���������Ķ���
	* @return ���ػ��������Ķ���
	*/
	IRenderContext* GetRenderContext();

	/**
	* @brief ������������Ķ���
	* @return ��
	*/
	void ClearRenderContext();

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual void AlphaPaint(IRenderContext* pRender, const UiRect& rcPaint);
	
	/**
	* @brief ���ƿؼ�����ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint);

	/**
	* @brief ���ƿؼ�������ں���
	* @param[in] pRender ָ����������
	* @param[in] rcPaint ָ����������
	* @return ��
	*/
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) { (void)pRender; (void)rcPaint; };

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

	/// ��������
	/**
	 * @brief ��ȡ����������ָ��
	 * @return ���ض���������ָ��
	 */
	AnimationManager& GetAnimationManager();

	/// ͼƬ����
	/**
	 * @brief ����ͼƬ
	 * @return ��
	 */
	virtual void InvokeLoadImageCache();

	/**
	 * @brief ȡ������ͼƬ����
	 * @return ��
	 */
	virtual void UnLoadImageCache();

	/**
	 * @brief ����ͼƬ����
	 * @return ��
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


	/**
	* @brief ��ȡĳ����ɫ��Ӧ��ֵ�����Ȼ�ȡ������ɫ
	* @param[in] strName ��ɫ����
	* @return DWORD ARGB��ɫֵ
	*/
    DWORD GetWindowColor(const std::wstring& strName);

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
	
	/// GifͼƬ
	void GifPlay();
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
	virtual void PaintShadow(IRenderContext* pRender);
	virtual void PaintBkColor(IRenderContext* pRender);
	virtual void PaintBkImage(IRenderContext* pRender);
	virtual void PaintStatusColor(IRenderContext* pRender);
	virtual void PaintStatusImage(IRenderContext* pRender);
	virtual void PaintText(IRenderContext* pRender);
	virtual void PaintBorder(IRenderContext* pRender);
	virtual void PaintLoading(IRenderContext* pRender);

private:
	void BroadcastGifEvent(int nVirtualEvent);
	int GetGifFrameIndex(GifStopType frame);

protected:

	GifEventMap m_OnGifEvent;
	std::unique_ptr<UserDataBase> m_pUserDataBase;
	
	bool m_bClip;
	bool m_bGifPlay;
	bool m_bAllowTabstop;
	bool m_bIsLoading;
	int m_nBorderSize;
	int m_nTooltipWidth;
	int m_nAlpha;
	int m_nHotAlpha;
	int m_fCurrrentAngele;
	UiSize m_szEstimateSize;
	UiPoint m_renderOffset;
	UiSize m_cxyBorderRound;
	UiRect m_rcMargin;
	UiRect m_rcPaint;
	UiRect m_rcBorderSize;
	CursorType m_cursorType;	//Ӱ��ؼ��������״
	ControlStateType m_uButtonState;
	std::wstring m_sToolTipText;
	std::wstring m_sToolTipTextId;
	std::wstring m_sUserData;
	std::wstring m_strBkColor;
    std::wstring m_strLoadingBkColor;
	std::unique_ptr<StateColorMap> m_colorMap;
	std::unique_ptr<Image> m_bkImage;
	std::unique_ptr<Image> m_loadingImage;
	std::unique_ptr<StateImageMap> m_imageMap;
	std::wstring m_strBorderColor;
	nbase::WeakCallbackFlag m_gifWeakFlag;
	std::unique_ptr<AnimationManager> m_animationManager;
	nbase::WeakCallbackFlag m_loadBkImageWeakFlag;
    nbase::WeakCallbackFlag m_loadingImageFlag;
	static const int m_nVirtualEventGifStop;	
	BoxShadow m_boxShadow;
	std::unique_ptr<IRenderContext> m_renderContext;

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

#if defined(ENABLE_UIAUTOMATION)
	UIAControlProvider* m_pUIAProvider;
#endif
};

} // namespace ui

#endif // UI_CORE_CONTROL_H_
