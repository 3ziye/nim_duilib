#ifndef UI_CORE_CONTROL_FINDER_H_
#define UI_CORE_CONTROL_FINDER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>
#include <vector>
#include <map>

namespace ui 
{
// Flags for Control::GetControlFlags()
#define UIFLAG_DEFAULT       0x00000000		// Ĭ��״̬
#define UIFLAG_TABSTOP       0x00000001		// ��ʶ�ؼ��Ƿ����յ� TAB �л�����ʱ�������ý���

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

class Control;
class Box;

typedef struct tagFINDTABINFO
{
	Control* pFocus;
	Control* pLast;
	bool bForward;
	bool bNextIsIt;
} FINDTABINFO;

/** Window��������Control��ع��ܵķ�װ
*/
class UILIB_API ControlFinder
{
public:
    ControlFinder();
	~ControlFinder();
	
public:
	/** ����Box���ڵ�
	*/
	void SetRoot(Box* pRoot);

	/**
	 * @brief �����������ָ���ؼ�
	 * @param[in] pt ָ������
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindControl(const POINT& pt) const;

	/**
	 * @brief ����������ҿ�����ӦWM_CONTEXTMENU�Ŀؼ�
	 * @param[in] pt ָ������
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindContextMenuControl(const POINT* pt) const;

	/**
	 * @brief ���ݿؼ����Ʋ��ҿؼ�
	 * @param[in] strName �ؼ�����
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindControl2(const std::wstring& strName) const;

	/**
	 * @brief ������������ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] pt Ҫ���ҵ�����
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindSubControlByPoint(Control* pParent, const POINT& pt) const;

	/**
	 * @brief �������ֲ����ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] strName Ҫ���ҵ�����
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindSubControlByName(Control* pParent, const std::wstring& strName) const;

	/**@brief ���տؼ�
	 * @param[in] pControl �ؼ�ָ��
	 */
	void ReapObjects(Control* pControl);

public:
	static Control* CALLBACK __FindControlFromNameHash(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromCount(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromPoint(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromTab(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromUpdate(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromName(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindContextMenuControl(Control* pThis, LPVOID pData);

private:
    /** ���ڵ�
	*/
	Box* m_pRoot;

	/** �ؼ���name��ӿ�֮���ӳ��
	*/
	std::map<std::wstring, Control*> m_mNameHash;
};

} // namespace ui

#endif // UI_CORE_CONTROL_FINDER_H_
