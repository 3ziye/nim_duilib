#ifndef UI_CORE_WINDOWHELPER_H_
#define UI_CORE_WINDOWHELPER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Core/WindowBuilder.h"
#include "duilib/Render/UiColor.h"
#include "duilib/Render/IFont.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace ui 
{
	class IRenderFactory;
	class ImageInfo;
	class ImageLoadAttribute;

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

	/**
	 * @brief ���һ��ȫ����ɫֵ�ṩ����ʹ��
	 * @param[in] strName ��ɫ���ƣ��� white��
	 * @param[in] strValue ��ɫ������ֵ���� #FFFFFFFF��
	 * @return ��
	 */
	static void AddTextColor(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief ���һ��ȫ����ɫֵ�ṩ����ʹ��
	 * @param[in] strName ��ɫ���ƣ��� white��
	  *@param[in] argb ��ɫ������ֵ, ��ARGB��ʽ��ʾ
	 * @return ��
	 */
	static void AddTextColor(const std::wstring& strName, UiColor argb);

	/**
	 * @brief �������ƻ�ȡһ����ɫ�ľ�����ֵ
	 * @param[in] strName Ҫ��ȡ����ɫ����
	 * @return ���� DWORD ��ʽ����ɫ����ֵ
	 */
	static UiColor GetTextColor(const std::wstring& strName);

	/**
	 * @brief ɾ������ȫ����ɫ����
	 * @return ��
	 */
	static void RemoveAllTextColors();

	/**
	 * @brief ���ָ��ͼƬ�Ƿ��Ѿ�������
	 * @param[in] loadAtrribute ͼƬ�ļ������ԣ�����ͼƬ·������Ϣ
	 * @return ����Ѿ������棬�򷵻� ImageInfo ������ָ�����
	 */
	static std::shared_ptr<ImageInfo> GetCachedImage(const ImageLoadAttribute& loadAtrribute);

	/**
	 * @brief ����ͼƬ ImageInfo ����
	 * @param[in] loadAtrribute ͼƬ�ļ������ԣ�����ͼƬ·������Ϣ
	 * @return ����ͼƬ ImageInfo ���������ָ��
	 */
	static std::shared_ptr<ImageInfo> GetImage(const ImageLoadAttribute& loadAtrribute);

	/**
	 * @brief ͼƬ�����ٵĻص�
	 * @param[in] pImageInfo ͼƬ��Ӧ�� ImageInfo ����
	 * @return ��
	 */
	static void OnImageInfoDestroy(ImageInfo* pImageInfo);

	/**
	 * @brief �ӻ�����ɾ������ͼƬ
	 * @return ��
	 */
	static void RemoveAllImages();

	/** �����Ƿ�Ĭ�϶�����ͼƬ�ڼ���ʱ����DPI�������ţ������ȫ�����ԣ�Ĭ��Ϊtrue��Ӧ��������ͼƬ
	   ������Ϊtrue��Ҳ����ͨ����xml�У�ʹ��"dpiscale='false'"���Թر�ĳ��ͼƬ��DPI�Զ����ţ�
	*/
	static void SetDpiScaleAllImages(bool bEnable);

	/**�ж��Ƿ�Ĭ�϶�����ͼƬ�ڼ���ʱ����DPI��������
	*/
	static bool IsDpiScaleAllImages();

	/**
	 * @brief ���һ������
	 * @param[in] strFontId ָ�������ID���
	 * @param[in] strFontName ��������
	 * @param[in] nSize �����С
	 * @param[in] bBold �Ƿ����
	 * @param[in] bUnderline �Ƿ����»���
	 * @param[in] bStrikeout �Ƿ����ɾ����
	 * @param[in] bItalic �Ƿ���б
	 * @param[in] bDefault �Ƿ�Ĭ��
	 * @param[in] nWeight �����ϸ�����أ�Ĭ��Ϊ FW_NORMAL(400)
	 */
	static bool AddFont(const std::wstring& strFontId, 
						const std::wstring& strFontName, 
					    int nSize, 
					    bool bBold, 
					    bool bUnderline, 
					    bool bStrikeout, 
					    bool bItalic, 
					    bool bDefault, 
					    int nWeight = 0);

	/**
	 * @brief ��������ID����һ���������
	 * @param[in] strFontId ����ID
	 * @return ��������� HFONT ���
	 */
	static HFONT GetFont(const std::wstring& strFontId);

	/** ��ȡ����ӿ�
	*/
	static IFont* GetIFont(const std::wstring& strFontId);

	/**
	 * @brief ɾ����������
	 * @return ��
	 */
	static void RemoveAllFonts();

	/**
	 * @brief ��ȡĬ�Ͻ���״̬��������ɫ
	 * @return Ĭ�Ͻ���״̬��ɫ���ַ�����ʾ����Ӧ global.xml ��ָ����ɫֵ
	 */
	static std::wstring GetDefaultDisabledTextColor();

	/**
	 * @brief ����Ĭ�Ͻ���״̬�µ�������ɫ
	 * @param[in] dwColor ������ɫ�ַ���ֵ���� white��
	 * @return ��
	 */
	static void SetDefaultDisabledTextColor(const std::wstring& strColor);

	/**
	 * @brief ��ȡĬ��������ɫ
	 * @return Ĭ��������ɫ�ַ�����ʾ����Ӧ global.xml ��ָ����ɫֵ
	 */
	static std::wstring GetDefaultTextColor();

	/**
	 * @brief ����Ĭ��������ɫ
	 * @param[in] ������ɫ���ַ���ֵ����Ӧ global.xml ��ָ����ɫֵ
	 * @return ��
	 */
	static void SetDefaultTextColor(const std::wstring& strColor);

	/**
	 * @brief ��ȡĬ������������ɫ
	 * @return ����������ɫ DWORD ��ʽ��ɫֵ
	 */
	static UiColor GetDefaultLinkFontColor();

	/**
	 * @brief ����Ĭ������������ɫ
	 * @param[in] dwColor Ĭ������������ɫ
	 * @return ��
	 */
	static void SetDefaultLinkFontColor(UiColor dwColor);

	/**
	 * @brief ��ȡ�������ӵ�Ĭ��������ɫ
	 * @return ���ؽ������ӵ�Ĭ��������ɫ
	 */
	static UiColor GetDefaultLinkHoverFontColor();

	/**
	 * @brief ���ý������ӵ�Ĭ��������ɫ
	 * @param[in] dwColor �������ӵ�Ĭ��������ɫ
	 * @return ��
	 */
	static void SetDefaultLinkHoverFontColor(UiColor dwColor);

	/**
	 * @brief ��ȡĬ��ѡ��״̬������ɫ
	 * @return ����Ĭ��ѡ��״̬��������ɫ
	 */
	static UiColor GetDefaultSelectedBkColor();

	/**
	 * @brief ����Ĭ��ѡ��״̬��������ɫ
	 * @param[in] dwColor ������ɫֵ
	 * @return ��
	 */
	static void SetDefaultSelectedBkColor(UiColor dwColor);

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

private:
	static std::unique_ptr<IRenderFactory> m_renderFactory;
	typedef std::map<std::wstring, std::weak_ptr<ImageInfo>> MapStringToImagePtr;

	static std::wstring m_pStrResourcePath; //ȫ�ֵ���Դ·����������ʱ���޸��������
	static std::wstring m_pStrLanguagePath; //ȫ�������ļ�·��
	static std::vector<Window*> m_aPreMessages;
	static std::map<std::wstring, std::unique_ptr<WindowBuilder>> m_builderMap;
	static CreateControlCallback m_createControlCallback;

	static MapStringToImagePtr m_mImageHash;

	/** �����Ƿ�Ĭ�϶�����ͼƬ�ڼ���ʱ����DPI�������ţ������ȫ�����ԣ�Ĭ��Ϊtrue��Ӧ��������ͼƬ
	   ������Ϊtrue��Ҳ����ͨ����xml�У�ʹ��"dpiscale='false'"���Թر�ĳ��ͼƬ��DPI�Զ����ţ�
	*/
	static bool m_bDpiScaleAllImages;

	static std::map<std::wstring, UiColor> m_mapTextColor;
	static std::map<std::wstring, std::wstring> m_mGlobalClass;

	static std::wstring m_strDefaultDisabledColor;
	static std::wstring m_strDefaultFontColor;
	static UiColor m_dwDefaultLinkFontColor;
	static UiColor m_dwDefaultLinkHoverFontColor;
	static UiColor m_dwDefaultSelectedBkColor;

	static DWORD m_dwUiThreadId;

	/** ��ѡ�����б�
	*/
	static std::map<std::wstring, IFont*> m_mCustomFonts;

	/** Ĭ������ID
	*/
	static std::wstring m_sDefaultFontId;
};

} // namespace ui

#endif // UI_CORE_WINDOWHELPER_H_
