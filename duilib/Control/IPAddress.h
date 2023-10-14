#ifndef UI_CONTROL_IPADDRESS_H_
#define UI_CONTROL_IPADDRESS_H_

#pragma once

#include "duilib/Box/HBox.h"

namespace ui
{
/** IP��ַ�ؼ�
*/
class RichEdit;
class IPAddress: public HBox
{
public:
	IPAddress();

	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/** ����IP��ַ
	*/
	void SetIPAddress(const std::wstring& ipAddress);

	/** ��ȡIP��ַ
	*/
	std::wstring GetIPAddress() const;

protected:
	virtual void DoInit() override;

private:
	//��ʼ����־
	bool m_bInited;

	/** �༭���б�
	*/
	std::vector<RichEdit*> m_editList;

	/** IP��ַ
	*/
	UiString m_ipAddress;
};

}//namespace ui

#endif //UI_CONTROL_IPADDRESS_H_
