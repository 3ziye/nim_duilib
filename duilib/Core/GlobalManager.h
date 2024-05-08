#ifndef UI_CORE_GLOBALMANAGER_H_
#define UI_CORE_GLOBALMANAGER_H_

#pragma once

#include "duilib/Core/WindowBuilder.h"
#include "duilib/Core/ColorManager.h"
#include "duilib/Core/FontManager.h"
#include "duilib/Core/ImageManager.h"
#include "duilib/Core/IconManager.h"
#include "duilib/Core/ZipManager.h"
#include "duilib/Core/LangManager.h"
#include "duilib/Core/DpiManager.h"
#include "duilib/Core/TimerManager.h"
#include "duilib/Core/ResourceParam.h"

#include <string>
#include <vector>
#include <map>

namespace ui 
{
/** ��Ⱦ���湤���ӿ�
*/
class IRenderFactory;

/** ȫ�����Թ�������
*   ���ڹ���һЩȫ�����ԵĹ����࣬����ȫ����ʽ��global.xml�����������õ�
*/
class UILIB_API GlobalManager
{
private:
	GlobalManager();
	~GlobalManager();
	GlobalManager(const GlobalManager&) = delete;
	GlobalManager& operator = (const GlobalManager&) = delete;

public:
	/** ��ȡ��������
	*/
	static GlobalManager& Instance();

public:
	/** ��ʼ��ȫ�����ú���
	 * @param [in] resParam ��Դ��صĲ�����������Դ���Ͳ�ͬ�������¿�ѡ��
	 *                      1. �����ļ�����ʽ��������Դ���ѱ����ļ�����ʽ����
	 *                         ʹ�� LocalFilesResParam ������Ϊ����
	 *                      2. ��Դ�ļ����Ϊzipѹ������Ȼ���Ա����ļ�����ʽ����
	 *                         ʹ�� ZipFileResParam ������Ϊ����
	 *                      3. ��Դ�ļ����Ϊzipѹ������Ȼ�����exe/dll����Դ�ļ���
	 *                         ʹ�� ResZipFileResParam ������Ϊ����
	 * @param [in] bAdaptDpi �Ƿ����� DPI �Զ�����
	 * @param [in] callback �����Զ���ؼ�ʱ��ȫ�ֻص�����
	 */
	bool Startup(const ResourceParam& resParam,
				 bool bAdaptDpi = true,
				 const CreateControlCallback& callback = nullptr);

	/** �ͷ�ȫ����Դ
	 */
	void Shutdown();

public:
	/** ����Ƥ����Դ����·��
	 *   ��� resType == kLocalFiles����Ҫ������Դ���ڵı���·��������·����
	 *   ��� resType == kZipFile ���� resType == kResZip��������Դ���ڵ���ʼĿ¼�����·���������磺L"resources\\"
	 */
	void SetResourcePath(const std::wstring& strPath);

	/** ��ȡ��ǰ��Դ����·��
	 */
	const std::wstring& GetResourcePath() const;

	/** ���¼���Ƥ����Դ����ͨ���˽ӿ�ʵ�ֶ�̬�������ܣ�
	* @param [in] resParam ��Դ��صĲ�����������Դ���Ͳ�ͬ�������¿�ѡ��
	 *                      1. �����ļ�����ʽ��������Դ���ѱ����ļ�����ʽ����
	 *                         ʹ�� LocalFilesResParam ������Ϊ����
	 *                      2. ��Դ�ļ����Ϊzipѹ������Ȼ���Ա����ļ�����ʽ����
	 *                         ʹ�� ZipFileResParam ������Ϊ����
	 *                      3. ��Դ�ļ����Ϊzipѹ������Ȼ�����exe/dll����Դ�ļ���
	 *                         ʹ�� ResZipFileResParam ������Ϊ����
	 * @param [in] bInvalidate �Ƿ�ˢ�½�����ʾ��true��ʾ�����������ļ���ˢ�½�����ʾ��false��ʾ��ˢ�½�����ʾ
	*/
	bool ReloadResource(const ResourceParam& resParam, bool bInvalidate = false);

public:
	/** ���������ļ�����·�������������·�������Ǿ���·���������԰�ʱ�����е������ļ����������Ŀ¼�У�
	*   ����Ǿ���·���������������·���в��������ļ�
	*   ��������·���������resType��resourcePath��������Դ·���£������·��������Դ�ļ�
	*/
	void SetLanguagePath(const std::wstring& strPath);

	/** ��ȡ�����ļ�����·��
	*/
	const std::wstring& GetLanguagePath() const;

	/** ��ȡ�����ļ���������·��
	*/
	const std::wstring& GetLanguageFileName() const;

	/** ���¼���������Դ��ͨ���˽ӿ�ʵ�ֶ����Զ�̬�л����ܣ�
	 * @param [in] languagePath �����ļ�����·��
	               ����Ǿ���·�������ڴ˾���·����Ŀ¼�в��������ļ���
				   ���Ϊ�գ���ʾʹ��Startupʱ��ʼ���������ļ�·����
				   ���Ϊ���·�������Ӧ��ѹ�����е����·��
	 * @param [in] languageFileName ��ǰʹ�������ļ����ļ���������·����
	 * @param [in] bInvalidate �Ƿ�ˢ�½�����ʾ��true��ʾ�����������ļ���ˢ�½�����ʾ��false��ʾ��ˢ�½�����ʾ
	 */
	bool ReloadLanguage(const std::wstring& languagePath = L"",
				        const std::wstring& languageFileName = L"zh_CN.txt",
						bool bInvalidate = false);

	/** ��ȡ�����ļ��б����ʾ���ƣ���֧�ֶ������л���
	* @param [in] languageNameID ���ڶ�ȡ��ʾ���Ƶ��ַ���ID�����Ϊ���򲻶�ȡ��ʾ����
	* @param [out] languageList ���������ļ�����ʾ���Ƶ��б�
	*/
	bool GetLanguageList(std::vector<std::pair<std::wstring, std::wstring>>& languageList,
						 const std::wstring& languageNameID = L"LANGUAGE_DISPLAY_NAME") const;

public:
	/** ���һ�����ڽӿڣ���Ҫ���ڻ������л�����֮����ػ桢��Դͬ���Ȳ�����
	* @param [in] pWindow ���ڵĽӿ�
	*/
	void AddWindow(Window* pWindow);

	/** �Ƴ�һ������
	* @param [in] pWindow ���ڵĽӿ�
	*/
	void RemoveWindow(Window* pWindow);

	/** ���һ��ȫ�� Class ����
	 * @param[in] strClassName ȫ�� Class ����
	 * @param[in] strControlAttrList �����б���Ҫ�� XML ת��
	 */
	void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList);

	/** ��ȡһ��ȫ�� class ���Ե�ֵ
	 * @param[in] strClassName ȫ�� class ����
	 * @return �����ַ�����ʽ�� class ����ֵ
	 */
	std::wstring GetClassAttributes(const std::wstring& strClassName) const;

	/** ��ȫ��������ɾ������ class ����
	 * @return ���ػ����������
	 */
	void RemoveAllClasss();

public:
	/** ��ȡ���ƽӿ������
	*/
	IRenderFactory* GetRenderFactory();

	/** ��ȡ��ɫ������
	*/
	ColorManager& Color();

	/** ��ȡ���������
	*/
	FontManager& Font();

	/** ��ȡͼƬ������
	*/
	ImageManager& Image();

#ifdef UILIB_IMPL_WINSDK
	/** ��ȡICON��Դ������
	*/
	IconManager& Icon();

#endif

	/** ��ȡZip������
	*/
	ZipManager& Zip();

	/** ��ȡDPI������
	*/
	DpiManager& Dpi();

	/** ��ȡ��ʱ��������
	*/
	TimerManager& Timer();

	/** ������֧�ֹ�����
	*/
	LangManager& Lang();

public:
	/** ������Դ���ط�ʽ�����ض�Ӧ����Դ·��
	 * @param[in] path Ҫ��ȡ����Դ·��
	 * @param [in] windowResPath ���ڶ�Ӧ����Դ���Ŀ¼�����磺"basic\"
	 * @param [in] resPath ��Դ�ļ�·�������磺"../public/button/btn_wnd_gray_min_hovered.png"
	 * @return ���õ���������Դ·�������磺
			  ��1�������ʹ��ZIPѹ���������أ�"resources\themes\default\public\button\btn_wnd_gray_min_hovered.png"
			  ��2�����δʹ��ZIPѹ���������أ�"D:\<��������Ŀ¼>\resources\themes\default\public\button\btn_wnd_gray_min_hovered.png"
	 */
	std::wstring GetResFullPath(const std::wstring& windowResPath, const std::wstring& resPath);

	/** ���� XML ����һ�� Box
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 * @return ָ������ģ��Ķ���ָ��
	 */
	Box* CreateBox(const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/** ���� XML �ڻ����в���ָ�� Box�����û���򴴽�
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 * @return ָ������ģ��Ķ���ָ��
	 */
	Box* CreateBoxWithCache(const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/** ʹ�� XML ���ָ�� Box
	 * @param[in] pUserDefinedBox Ҫ���� box ָ��
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 */
	void FillBox(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/** ʹ�ù������Ļ������ָ�� Box�����û�������¹���
	 * @param[in] pUserDefinedBox Ҫ���� box ָ��
	 * @param[in] strXmlPath XML �ļ�·��
	 * @param[in] callback �Զ���ؼ��Ļص�������
	 */
	void FillBoxWithCache(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback());

	/** �Զ���ؼ��������ȫ�ֻص�����
	 * @param[in] strControlName �Զ���ؼ�����
	 * @return ����һ���Զ���ؼ��Ķ���ָ��
	 */
	Control* CreateControl(const std::wstring& strControlName);

	/** ����ú���������UI�߳��е��õģ����ж��Ա���
	*/
	void AssertUIThread() const;

private:
	/** �ӻ�����ɾ������ͼƬ
	 */
	void RemoveAllImages();

private:

	/** ��Ⱦ�������ӿ�
	*/
	std::unique_ptr<IRenderFactory> m_renderFactory;

	/** ȫ�ֵ���Դ·����������ʱ���޸��������������·����
	*/
	std::wstring m_resourcePath; 

	/** ȫ�������ļ�·��������·����
	*/
	std::wstring m_languagePath; 

	/** ȫ�������ļ���������·����
	*/
	std::wstring m_languageFileName;
	
	/** ���ڹ�������ӿڣ�KEY��XML�ļ�·����VALUE�Ǵ��ڹ�������ӿڣ��Ѿ��������XML���ɱ����ظ�������
	*/
	std::map<std::wstring, std::unique_ptr<WindowBuilder>> m_builderMap;

	/** �ؼ����������������û��Զ���ؼ��Ĵ���
	*/
	CreateControlCallback m_pfnCreateControlCallback;

	/** ÿ��Class������(KEY)�������б�(VALUE)������global.xml�ж����Class��
	*/
	std::map<std::wstring, std::wstring> m_globalClass;

	/** ���߳�ID
	*/
	uint32_t m_dwUiThreadId;

	/** ���еĴ����б�
	*/
	std::vector<Window*> m_windowList;

	/** ��ɫ������
	*/
	ColorManager m_colorManager;

	/** ���������
	*/
	FontManager m_fontManager;

	/** ͼƬ������
	*/
	ImageManager m_imageManager;

	/** ZIPѹ����������
	*/
	ZipManager m_zipManager;

	/** DPI������
	*/
	DpiManager m_dpiManager;

	/** ��ʱ��������
	*/
	TimerManager m_timerManager;

	/** �����Թ�����
	*/
	LangManager m_langManager;

#ifdef UILIB_IMPL_WINSDK
	/** ICON��Դ������
	*/
	IconManager m_iconManager;
#endif
};

} // namespace ui

#endif // UI_CORE_GLOBALMANAGER_H_
