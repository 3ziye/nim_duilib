#ifndef UI_CORE_GLOBALMANAGER_H_
#define UI_CORE_GLOBALMANAGER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Core/WindowBuilder.h"
#include "duilib/Core/ColorManager.h"
#include "duilib/Core/FontManager.h"
#include "duilib/Core/ImageManager.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace ui 
{
	class IRenderFactory;
;
/**
* @brief ȫ�����Թ�������
* ���ڹ���һЩȫ�����ԵĹ����࣬����ȫ����ʽ��global.xml�����������õ�
*/
class UILIB_API GlobalManager
{
public:
	/**
	 * @brief ��ʼ��ȫ�����ú���
	 * @param[in] strResourcePath ��Դ·��λ��
	 * @param[in] callback �����Զ���ؼ�ʱ��ȫ�ֻص�����
	 * @param[in] bAdaptDpi �Ƿ����� DPI ����
	 * @param[in] theme ����Ŀ¼����Ĭ��Ϊ themes\\default
	 * @param[in] language ʹ�����ԣ�Ĭ��Ϊ lang\\zh_CN
	 * @return ��
	 */
	static void Startup(const std::wstring& strResourcePath, const CreateControlCallback& callback, bool bAdaptDpi, const std::wstring& theme = L"themes\\default", const std::wstring& language = L"lang\\zh_CN");

	/**
	 * @brief �ͷ���Դ
	 * @return ��
	 */
	static void Shutdown();

	/**
	 * @brief ��ȡ��ǰ��Դ����Ŀ¼
	 * @return ���ص�ǰ��Դ����Ŀ¼
	 */
	static std::wstring GetResourcePath();

	/**
	* @brief ��ȡ��ǰ�����ļ�·��
	* @return ���ص�ǰ�����ļ�·��
	*/
	static std::wstring GetLanguagePath();

	/**
	 * @brief ����Ƥ����Դ����Ŀ¼
	 * @param[in] strPath Ҫ���õ�·��
	 * @return ��
	 */
	static void SetResourcePath(const std::wstring& strPath);

	/**
	* @brief ���õ�ǰ�����ļ�·��
	* @return ���õ�ǰ�����ļ�·��
	*/
	static void SetLanguagePath(const std::wstring& strPath);

	/**
	* ������
	*/
	static void AddPreMessage(Window* pWindow);

	/**
	* ������
	*/
	static void RemovePreMessage(Window* pWindow);

	/**
	 * @brief ���¼���Ƥ����Դ
	 * @param[in] resourcePath ��Դ·��
	 * @return ��
	 */
	static void ReloadSkin(const std::wstring& resourcePath);

	/**
	 * @brief ���¼���������Դ
	 * @param[in] languagePath ��Դ·��
	 * @return ��
	 */
	static void ReloadLanguage(const std::wstring& languagePath, bool invalidateAll = false);

	/**
	 * @brief ��ȡ���ƽӿ������
	 * @return ���ؽӿ������ָ��
	 */
	static IRenderFactory* GetRenderFactory();

	/**
	 * @brief ���һ��ȫ�� class ����
	 * @param[in] strClassName ȫ�� class ����
	 * @param[in] strControlAttrList �����б���Ҫ�� XML ת��
	 * @return ��
	 */
	static void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList);

	/**
	 * @brief ��ȡһ��ȫ�� class ���Ե�ֵ
	 * @param[in] strClassName ȫ�� class ����
	 * @return �����ַ�����ʽ�� class ����ֵ
	 */
	static std::wstring GetClassAttributes(const std::wstring& strClassName);

	/**
	 * @brief ��ȫ��������ɾ������ class ����
	 * @return ���ػ����������
	 */
	static void RemoveAllClasss();

	/** ��ȡ��ɫ������
	*/
	static ColorManager& GetColorManager();

	/** ��ȡ���������
	*/
	static FontManager& GetFontManager();

	/** ��ȡͼƬ������
	*/
	static ImageManager& GetImageManager();

	/**
	 * @brief ���� XML ����һ�� Box
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 * @return ָ������ģ��Ķ���ָ��
	 */
	static Box* CreateBox(const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/**
	 * @brief ���� XML �ڻ����в���ָ�� Box�����û���򴴽�
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 * @return ָ������ģ��Ķ���ָ��
	 */
	static Box* CreateBoxWithCache(const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/**
	 * @brief ʹ�� XML ���ָ�� Box
	 * @param[in] pUserDefinedBox Ҫ���� box ָ��
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 * @return ��
	 */
	static void FillBox(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/**
	 * @brief ʹ�ù������Ļ������ָ�� Box�����û�������¹���
	 * @param[in] pUserDefinedBox Ҫ���� box ָ��
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 * @return ��
	 */
	static void FillBoxWithCache(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/**
	 * @brief �Զ���ؼ��������ȫ�ֻص�����
	 * @param[in] strControlName �Զ���ؼ�����
	 * @return ����һ���Զ���ؼ��Ķ���ָ��
	 */
	static Control* CreateControl(const std::wstring& strControlName);

	static void AssertUIThread();

	/**
	 * @brief �жϵ�ǰ�Ƿ�ʹ���� zip ѹ����
	 * @return ���� true ��ʾʹ���� zip ѹ������Ϊ��Դ��false Ϊ��ͨĿ¼ģʽ
	 */
	static bool IsUseZip();

	/**
	 * @brief ��һ���ڴ�ѹ������Դ
	 * @param[in] resource_name ��Դ���ƣ��� MAKEINTRESOURCE ������
	 * @param[in] resource_type ��Դ���ͣ��Զ��嵼�����Դ��������
	 * @param[in] password ѹ��������
	 * @return ���� true �򿪳ɹ������� false Ϊ��ʧ��
	 */
	static bool OpenResZip(LPCTSTR  resource_name, LPCTSTR  resource_type, const std::string& password);

	/**
	 * @brief ��һ�������ļ�ѹ������Դ
	 * @param[in] path ѹ�����ļ�·��
	 * @param[in] password ѹ��������
	 * @return ���� true �򿪳ɹ������� false Ϊ��ʧ��
	 */
	static bool OpenResZip(const std::wstring& path, const std::string& password);

	/**
	 * @brief ��ȡѹ�����е����ݵ��ڴ�
	 * @param[in] path Ҫ��ȡ���ļ���·��
	 * @param[out] file_data Ҫ��ȡ���ļ���·��
	 * @return ���� true �򿪳ɹ������� false Ϊ��ʧ��
	 */
	static bool GetZipData(const std::wstring& path, std::vector<unsigned char>& file_data);

	/**
	 * @brief ��ȡ�ļ���ѹ�����е�λ��
	 * @param[in] path Ҫ��ȡ���ļ�·��
	 * @return ������ѹ�����е��ļ�λ��
	 */
	static std::wstring GetZipFilePath(const std::wstring& path);

	/**
	 * @brief ������Դ���ط�ʽ�����ض�Ӧ����Դ·��
	 * @param[in] path Ҫ��ȡ����Դ·��
	 * @return ���õ���Դ·��
	 */
	static std::wstring GetResFullPath(const std::wstring& window_res_path, const std::wstring& res_path);

	/**
	 * @brief �ж���Դ�Ƿ����zip����
	 * @param[in] path Ҫ�жϵ���Դ·��
	 * @return �Ƿ����
	 */
	static bool IsZipResExist(const std::wstring& path);

private:
	/**
	 * @brief ����ȫ����Դ
	 * @return ��
	 */
	static void LoadGlobalResource();

	/** �ӻ�����ɾ������ͼƬ
	 */
	static void RemoveAllImages();

private:
	static std::unique_ptr<IRenderFactory> m_renderFactory;

	static std::wstring m_pStrResourcePath; //ȫ�ֵ���Դ·����������ʱ���޸��������
	static std::wstring m_pStrLanguagePath; //ȫ�������ļ�·��
	static std::vector<Window*> m_aPreMessages;
	static std::map<std::wstring, std::unique_ptr<WindowBuilder>> m_builderMap;
	static CreateControlCallback m_createControlCallback;

	static std::map<std::wstring, std::wstring> m_mGlobalClass;

	static DWORD m_dwUiThreadId;

	/** ��ɫ������
	*/
	static ColorManager m_colorManager;

	/** ���������
	*/
	static FontManager m_fontManager;

	/** ͼƬ������
	*/
	static ImageManager m_imageManager;
};

} // namespace ui

#endif // UI_CORE_GLOBALMANAGER_H_
