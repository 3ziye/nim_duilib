#ifndef UI_CORE_RESOURCE_PARAM_H_
#define UI_CORE_RESOURCE_PARAM_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>

namespace ui 
{
/** ��Դ������
*/
enum class ResourceType
{
	/** �����ļ�����ʽ��������Դ���ѱ����ļ�����ʽ����
	*/
	kLocalFiles,

	/** ��Դ�ļ����Ϊzipѹ������Ȼ���Ա����ļ�����ʽ����
	*/
	kZipFile,

	/** ��Դ�ļ����Ϊzipѹ������Ȼ�����exe/dll����Դ�ļ���
	*/
	kResZipFile
};

/** ����ȫ����Դ����Ļ������������࣬ʹ��ʱ��Ҫʹ�����࣬���·����壩
*/
class UILIB_API ResourceParam
{
protected:
	explicit ResourceParam(ResourceType resourceType): resType(resourceType) { }
	~ResourceParam() { }

protected:
	/** ��Դ������
	*/
	ResourceType resType;

public:
	/** ��ȡ��Դ����
	*/
	ResourceType GetResType() const { return resType; }

	/** ��Դ·��������λ��
	*   ��� resType == kLocalFiles����Ҫ������Դ���ڵı���·��������·����
	*   ��� resType == kZipFile ���� resType == kResZipFile��������Դ���ڵ���ʼĿ¼�����·���������磺L"resources\\"
	*/
	std::wstring resourcePath;

public:
	/** ����·�����ƣ����·����
	*/
	std::wstring themePath = L"themes\\default";

	/** �����ļ�����·�������������·�������Ǿ���·���������԰�ʱ�����е������ļ����������Ŀ¼�У�
	*   ����Ǿ���·���������������·���в��������ļ�
	*   ��������·���������resType��resourcePath��������Դ·���£������·��������Դ�ļ�
	*/
	std::wstring languagePath = L"lang";

	/** ��ǰʹ�������ļ����ļ���������·����
	*/
	std::wstring languageFileName = L"zh_CN.txt";

	/** ȫ����Դ����XML�ļ����ļ�����Ĭ��Ϊ��"global.xml"
	*/
	std::wstring globalXmlFileName = L"global.xml";
};

/** ����ȫ����Դ����Ĳ����������ļ���ʽ����Ӧ��Դ���ͣ�kLocalFiles��
*/
class UILIB_API LocalFilesResParam: public ResourceParam
{
public:
	LocalFilesResParam() : ResourceParam(ResourceType::kLocalFiles)
	{
	}

	/** ����һ�������ļ���ʽ�Ĳ���
	* @param [in] resPath ������Դ�ļ����ڵ�·��������·����
	*/
	explicit LocalFilesResParam(const std::wstring& resPath) : 
		ResourceParam(ResourceType::kLocalFiles)
	{
		resourcePath = resPath;
	}
};

/** ����ȫ����Դ����Ĳ�����Zipѹ�����ļ���ʽ����Ӧ��Դ���ͣ�kZipFile��
*/
class UILIB_API ZipFileResParam : public ResourceParam
{
public:
	ZipFileResParam() : ResourceParam(ResourceType::kZipFile)
	{
	}

	/** ѹ�����ı���·��������·����
	*/
	std::wstring zipFilePath;

	/** ��ѡ��������������ѹ�����Ľ�ѹ���룻���ѹ���������룬����Ҫ����
	*   ע�������Ҫʹ��"ZIP legacy encryption"ģʽ�����룬�����޷���ѹ
	*/
	std::string zipPassword;
};

#ifdef UILIB_IMPL_WINSDK

/** ����ȫ����Դ����Ĳ�������Դ�ļ����Ϊzipѹ������Ȼ�����exe/dll����Դ�ļ��У���Ӧ��Դ���ͣ�kResZipFile��
*/
class UILIB_API ResZipFileResParam : public ResourceParam
{
public:
	ResZipFileResParam() : ResourceParam(ResourceType::kResZipFile)
	{
	}

	/** Zipѹ����������Դ��ģ����������Ϊnullptr
	*/
	HMODULE hResModule = nullptr;

	/** Zipѹ��������Դ���ƣ��� MAKEINTRESOURCE �����ɣ��μ�FindResource����
	*/
	LPCTSTR resourceName = L"";

	/** Zipѹ��������Դ���ͣ��Զ��嵼�����Դ�������ƣ��μ�FindResource����
	*/
	LPCTSTR resourceType = L"";

	/** ��ѡ��������������ѹ�����Ľ�ѹ���룻���ѹ���������룬����Ҫ����
	*   ע�������Ҫʹ��"ZIP legacy encryption"ģʽ�����룬�����޷���ѹ
	*/
	std::string zipPassword;
};

#endif

} // namespace ui

#endif // UI_CORE_RESOURCE_PARAM_H_
