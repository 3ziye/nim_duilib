#ifndef UI_CORE_ICONMANAGER_H_
#define UI_CORE_ICONMANAGER_H_

#pragma once

#include "duilib/Core/UiTypes.h"
#include <map>
#include <string>
#include <vector>

#ifdef UILIB_IMPL_WINSDK

namespace ui 
{
/** HICON���������
 */
class Window;
class ImageLoadAttribute;
class UILIB_API IconManager
{
public:
	IconManager();
	~IconManager();
	IconManager(const IconManager&) = delete;
	IconManager& operator = (const IconManager&) = delete;

public:
	/** ��ȡICON����Դ�ַ�����������ΪͼƬ�ļ�·��ʹ�ã�
	* @return ������Դ�ַ��������磺"icon:1"
	*/
	std::wstring GetIconString(HICON hIcon) const;

	/** ��ȡICON����Դ�ַ�����������ΪͼƬ�ļ�·��ʹ�ã�
	* @return ������Դ�ַ��������磺"icon:1"
	*/
	std::wstring GetIconString(uint32_t id) const;

	/** �ж��Ƿ�ΪICON����Դ�ַ���
	*@param [in] str ��Դ�ַ�������ȷ��ʽ���磺"icon:1"
	*/
	bool IsIconString(const std::wstring& str) const;

	/** ��ȡ��Դ�ַ�����Ӧ��ICON��ͼ���������û�и���Դ���򷵻�nullptr
	*@param [in] str ��Դ�ַ�������ȷ��ʽ���磺"icon:1"
	*/
	HICON GetIcon(const std::wstring& str) const;

	/** ��ȡICON��Դ�ַ�����Ӧͼ��Ĵ�С
	*@param [in] str ��Դ�ַ�������ȷ��ʽ���磺"icon:1"
	*@return ����ͼ��Ĵ�С�����ʧ�ܷ��ؿ�
	*/
	UiSize GetIconSize(const std::wstring& str) const;

	/** ��ICON��Դ�ַ�����Ӧ��HICON���������Ϊλͼ���ݣ�32λ��ARGBλͼ��
	* @param [in] str ��Դ�ַ�������ȷ��ʽ���磺"icon:1"
	* @param [in] pWindow ���ƹ����Ĵ��ڣ����Ϊnullptr����ʹ��Ĭ������DC������ͼ���ʱ����Ҫ�õ����ڹ������豸������DC��
	* @param [in] ImageLoadAttribute ͼƬ������Ϣ������ָ��ͼ��Ŀ�Ⱥ͸߶ȣ�������ߴ�����ͼ��λͼ����ͬ��DPI�£����ɵ�λͼ��С��ͬ����ICONͼԴ��Сֻ��һ����
	* @param [in] bEnableDpiScale �Ƿ�������DPI��ͼƬ��С�������ţ���Ϊ���ܿ��أ�
	* @param [out] bitmapData �ɹ�ʱ������λͼ������
	* @param [out] bitmapWidth �ɹ�ʱ������λͼ�Ŀ��
	* @param [out] bitmapHeight �ɹ�ʱ������λͼ�ĸ߶�
	* @param [out] bDpiScaled ����ͼƬ�Ƿ�����DPI����
	*/
	bool LoadIconData(const std::wstring& str,
		              const Window* pWindow,
					  const ImageLoadAttribute& loadAtrribute,
		              bool bEnableDpiScale,
		              std::vector<uint8_t>& bitmapData,
		              uint32_t& bitmapWidth,
					  uint32_t& bitmapHeight,
		              bool& bDpiScaled) const;

public:
	/** ���һ��HICON���
	* @param [in] hIcon ��Ҫ�����ICON���, ���������Դ���������ɸ������
	* @return ���ظþ����Ӧ��ID�����ʧ���򷵻�0
	*/
	uint32_t AddIcon(HICON hIcon);

	/** ��ȡ�����Ӧ��ID
	* @param [in] hIcon ICON���
	* @return ���ظþ����Ӧ��ID������Ҳ����򷵻�0
	*/
	uint32_t GetIconID(HICON hIcon) const;

	/** ����ID��ȡ���
	* @param [in] id ICON���ID
	*/
	HICON GetIcon(uint32_t id) const;

	/** ɾ��һ��ICON���
	* @param [in] hIcon ��Ҫɾ����ICON�����������ɾ���󣬸þ�������������ɵ��÷�����
	*/
	void RemoveIcon(HICON hIcon);

	/** ����IDɾ��һ��ICON���
	* @param [in] id ��Ҫɾ����ICON���ID��������ɾ���󣬸þ�������������ɵ��÷�����
	*/
	void RemoveIcon(uint32_t id);

	/** ��ȡICON�Ĵ�С����Ⱥ͸߶ȣ�
	* @param [in] hIcon ICON���
	*/
	UiSize GetIconSize(HICON hIcon) const;

private:
	/** ICON�����Դӳ���
	*/
	std::map<uint32_t, HICON> m_iconMap;

	/** ��һ��ID
	*/
	uint32_t m_nNextID;

	/** ICON��Դ�ַ���ǰ׺
	*/
	std::wstring m_prefix;
};

} //namespace ui 

#endif //UILIB_IMPL_WINSDK

#endif //UI_CORE_ICONMANAGER_H_
