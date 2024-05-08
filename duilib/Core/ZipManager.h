#ifndef UI_CORE_ZIPMANAGER_H_
#define UI_CORE_ZIPMANAGER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>
#include <vector>

namespace ui 
{
/** ZIPѹ����������
 */
class UILIB_API ZipManager
{
public:
	ZipManager();
	~ZipManager();
	ZipManager(const ZipManager&) = delete;
	ZipManager& operator = (const ZipManager&) = delete;

public:
	/** �жϵ�ǰ�Ƿ�ʹ���� zip ѹ����
	 * @return ���� true ��ʾʹ���� zip ѹ������Ϊ��Դ��false Ϊ��ͨĿ¼ģʽ
	 */
	bool IsUseZip() const;

#ifdef UILIB_IMPL_WINSDK
	/** ��һ���ڴ�ѹ������Դ
	 * @param[in] hModule ��Դ��������Ϊnullptr������ǰ��ִ�г����ģ����
	 * @param[in] resourceName ��Դ���ƣ��� MAKEINTRESOURCE ������
	 * @param[in] resourceType ��Դ���ͣ��Զ��嵼�����Դ��������
	 * @param[in] password ѹ��������
	 */
	bool OpenResZip(HMODULE hModule, LPCTSTR resourceName, LPCTSTR resourceType, const std::string& password);
#endif

	/** ��һ�������ļ�ѹ������Դ
	 * @param[in] path ѹ�����ļ�·��
	 * @param[in] password ѹ��������
	 */
	bool OpenZipFile(const std::wstring& path, const std::string& password);

	/** ��ȡѹ�����е����ݵ��ڴ�
	 * @param[in] path Ҫ��ȡ���ļ���·��
	 * @param[out] file_data Ҫ��ȡ���ļ���·��
	 */
	bool GetZipData(const std::wstring& path, std::vector<unsigned char>& file_data) const;

	/** ��ȡ�ļ���ѹ�����е�λ��
	 * @param[in] path Ҫ��ȡ���ļ�·��
	 * @return ������ѹ�����е��ļ�λ��
	 */
	std::wstring GetZipFilePath(const std::wstring& path) const;

	/** �ж���Դ�Ƿ����zip����
	 * @param[in] path Ҫ�жϵ���Դ·��
	 */
	bool IsZipResExist(const std::wstring& path) const;

	/** �ر�ѹ����
	*/
	void CloseResZip();

private:
	
	/** �򿪵�ѹ�������
	*/
	void* m_hzip;
};

}
#endif //UI_CORE_ZIPMANAGER_H_
