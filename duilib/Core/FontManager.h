#ifndef UI_CORE_FONTMANAGER_H_
#define UI_CORE_FONTMANAGER_H_

#pragma once

#include "duilib/Core/UiFont.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace ui 
{
/** @class FontManager
  * @brief �Զ������������
  * @copyright (c) 2021, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2021/2/26
  */
class IFont;
class DpiManager;
class UILIB_API FontManager
{
public:
	FontManager();
	~FontManager();
	FontManager(const FontManager&) = delete;
	FontManager& operator = (const FontManager&) = delete;

public:
	/** ������Ӣ������ת��(Skiaֻ֧������Ӣ�����ƣ���֧����������)
	*/
	static std::wstring GetFontEnglishName(const std::wstring& fontName);

	/** �������Ӣ������ת��(ѡ������ʱ��ֻ��ʹ����������)
	*/
	static std::wstring GetFontSystemName(const std::wstring& fontName);

public:
	/** ���һ��������Ϣ�������Сδ��DPI����
	 * @param [in] fontId ָ�������ID���
	 * @param [in] fontInfo ����������Ϣ
	 * @param [in] bDefault �Ƿ�����ΪĬ������
	 */
	bool AddFont(const std::wstring& fontId, const UiFont& fontInfo, bool bDefault);

	/** ��ȡ����ӿ�, ���ͨ��fontId�Ҳ�������ӿڣ���ô���������m_defaultFontId����ӿ�
	* @param [in] fontId ����ID
	* @param [in] dpi DPI���Ź����������ڶ������С��������
	* @return �ɹ���������ӿڣ��ⲿ���ò���Ҫ�ͷ���Դ�����ʧ���򷵻�nullptr
	*/
	IFont* GetIFont(const std::wstring& fontId, const DpiManager& dpi);

	/** ɾ����������, �������Ѿ����ص������ļ�
	 */
	void RemoveAllFonts();

public:
	/** @brief ���һ�������ļ�, ��Ӻ���԰�����������ʹ��
	  * @param[in] strFontFile �����ļ���, ���·���������ļ��ı���·����Ŀ¼��"<��Դ·��>\font\"
	  * @param[in] strFontName ������
	  * @return �޷���ֵ
	  */
	void AddFontFile(const std::wstring& strFontFile, const std::wstring& strFontName);

	/** @brief ����������ӵ������ļ�
	  * @return �޷���ֵ
	  */
	void RemoveAllFontFiles();

private:
	/** ��ȡDPI���ź�ʵ�ʵ�����ID
	*/
	std::wstring GetDpiFontId(const std::wstring& fontId, const DpiManager& dpi) const;

private:
	/** �Զ����������ݣ�KeyʱFontID��Value������������Ϣ
	*/
	std::unordered_map<std::wstring, UiFont> m_fontIdMap;

	/** �Զ���������Ϣ��Key��FontId
	*/
	std::unordered_map<std::wstring, IFont*> m_fontMap;

	/** Ĭ������ID
	*/
	std::wstring m_defaultFontId;

	/** �����ļ��ļ�����Ϣ
	*/
	struct FontFileInfo
	{
		//����·��
		std::wstring m_fontFilePath;

		//��������
		std::wstring m_fontName;

		//���������ļ���ľ��
		HANDLE m_hFontFile = nullptr;
	};
	std::vector<FontFileInfo> m_fontFileInfo;
};

}
#endif //UI_CORE_FONTMANAGER_H_
