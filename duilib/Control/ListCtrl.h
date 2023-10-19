#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Box/VBox.h"
#include "duilib/Box/HBox.h"
#include "duilib/Box/VirtualListBox.h"
namespace ui
{

/** ListCtrl�ı�ͷ�ؼ�
*/
class ListCtrlHeader: public HBox
{
public:
	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override { return L"ListCtrlHeader"; }

};

/** ListCtrl�ı�ͷ����ؼ�
*/
class ListCtrlHeaderItem: public HBox
{
public:
	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override { return L"ListCtrlHeaderItem"; }

};

/** ListCtrl����ؼ�
*/
class ListCtrlItem: public ListBoxItem
{
public:
	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override { return L"ListCtrlItem"; }

};

//�б����ݹ�����
class ListCtrlData;
class ListCtrlItemProvider;

/** ListCtrl�ؼ�
*/
class ListCtrl: public VBox
{
public:
	ListCtrl();
	virtual ~ListCtrl();

	/** ��ȡ�ؼ�����
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

protected:
	/** �ؼ���ʼ��
	*/
	virtual void DoInit() override;

private:
	/** ��ʼ����־
	*/
	bool m_bInited;

	/** ��ͷ�ؼ�
	*/
	ListCtrlHeader* m_pListCtrlHeader;

	/** �б�����չʾ
	*/
	ListCtrlData* m_pListCtrlData;

	/** �б����ݹ���
	*/
	std::unique_ptr<ListCtrlItemProvider> m_spItemProvider;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
