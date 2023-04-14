#ifndef UI_UTILS_MULTILANGSUPPORT_H_
#define UI_UTILS_MULTILANGSUPPORT_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>
#include <vector>
#include <map>

namespace ui 
{

class UILIB_API MultiLangSupport
{
public:
	static MultiLangSupport* GetInstance();

	/**
	 * @brief ����ID��ȡָ�����Ե��ַ���
	 * @param[in] id ָ���ַ��� ID
	 * @return ���� ID ��Ӧ�������ַ���
	 */
	std::wstring GetStringViaID(const std::wstring& id);

	/**
	 * @brief ������������ӳ���
	 * @param[in] strFilePath �����ļ�·��
	 * @return ���� true ��ʾ�ɹ������� false ��ʾʧ��
	 */
	bool LoadStringTable(const std::wstring& strFilePath);

	/**
	 * @brief ���ڴ��м�����������ӳ���
	 * @param[in] hGlobal Ҫ���ص�����ӳ����ڴ��׵�ַ
	 * @return ���� true ��ʾ�ɹ������� false ��ʾʧ��
	 */
	bool LoadStringTable(const std::vector<unsigned char>& file_data);

private:
	MultiLangSupport() {};
	~MultiLangSupport() {};
	MultiLangSupport(const MultiLangSupport&) = delete;
	MultiLangSupport& operator = (const MultiLangSupport&) = delete;

	/**
	 * @brief ��������
	 * @return ��
	 */
	void ClearAll();

	/**
	 * @brief ��������ӳ�������
	 * @param[in] list ��ȡ������ӳ��������б�
	 * @return �ɹ����� true�����򷵻� false
	 */
	bool AnalyzeStringTable(const std::vector<std::wstring>& list);

private: 
	std::map<std::wstring, std::wstring>  m_stringTable;
};

}
#endif //UI_UTILS_MULTILANGSUPPORT_H_
