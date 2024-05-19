#ifndef UI_CORE_IMAGEMANAGER_H_
#define UI_CORE_IMAGEMANAGER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace ui 
{
class ImageInfo;
class ImageLoadAttribute;
class DpiManager;
class Window;

/** ͼƬ������
 */
class UILIB_API ImageManager
{
public:
	ImageManager();
	~ImageManager();
	ImageManager(const ImageManager&) = delete;
	ImageManager& operator = (const ImageManager&) = delete;

public:
	/** ����ͼƬ ImageInfo ����
	 * @param [in] pWindow ͼƬ�����Ĵ��ڣ�����DPI���š�HICON���Ƶȣ�
	 * @param [in] loadAtrribute ͼƬ�ļ������ԣ�����ͼƬ·������Ϣ	 
	 * @return ����ͼƬ ImageInfo ���������ָ��
	 */
	std::shared_ptr<ImageInfo> GetImage(const Window* pWindow,
										const ImageLoadAttribute& loadAtrribute);

	/** �ӻ�����ɾ������ͼƬ
	 */
	void RemoveAllImages();

	/** �����Ƿ�Ĭ�϶�����ͼƬ�ڼ���ʱ����DPI�������ţ������ȫ�����ԣ�Ĭ��Ϊtrue��Ӧ��������ͼƬ
	   ������Ϊtrue��Ҳ����ͨ����xml�У�ʹ��"dpiscale='false'"���Թر�ĳ��ͼƬ��DPI�Զ����ţ�
	*/
	void SetDpiScaleAllImages(bool bEnable);

	/** �ж��Ƿ�Ĭ�϶�����ͼƬ�ڼ���ʱ����DPI��������
	*/
	bool IsDpiScaleAllImages() const;

	/** �����Ƿ�����ƥ���ٽ������Űٷֱ�ͼƬ
	*   ���統dpiScaleΪ120��ʱ�������ͼƬƥ�䣬���������Űٷֱ�Ϊ125��ͼƬ�����Զ�ƥ�䵽
	*   ������ܿ��ü��ٸ���DPI�µ�ͼƬ����������DPI�Ĺ�����
	*/
	void SetAutoMatchScaleImage(bool bAutoMatchScaleImage);

	/** ��ȡ�Ƿ�����ƥ���ٽ������Űٷֱ�ͼƬ
	*/
	bool IsAutoMatchScaleImage() const;

private:
	/** ͼƬ�����ٵĻص������������ͷ�ͼƬ��Դ
	 * @param[in] pImageInfo ͼƬ��Ӧ�� ImageInfo ����
	 */
	static void OnImageInfoDestroy(ImageInfo* pImageInfo);

	/** ����ָ��DPI���Űٷֱ��µ�ͼƬ������ÿ��DPI����һ��ͼƬ������߲�ͬDPI�µ�ͼƬ����
	*   ������DPI���Űٷֱ�Ϊ120�����Ŵ�120%����ͼƬ��"image.png" ��Ӧ�� "image@120.png"
	* @param [in] dpiScale ��Ҫ���ҵ�DPI���Űٷֱ�
	* @param [in] bIsUseZip �Ƿ�ʹ��zipѹ������Դ
	* @param [in] imageFullPath ͼƬ��Դ������·��
	* @param [out] dpiImageFullPath ����ָ��DPI�µ�ͼƬ��Դ·�������û�ҵ����򷵻ؿմ�
	* @param [out] nImageDpiScale ͼƬ��Ӧ��DPI���Űٷֱ�
	*/
	bool GetDpiScaleImageFullPath(uint32_t dpiScale, 
								  bool bIsUseZip,
							      const std::wstring& imageFullPath,
							      std::wstring& dpiImageFullPath,
		                          uint32_t& nImageDpiScale) const;

	/** ����ָ��DPI���Űٷֱ��µ�ͼƬ������ÿ��DPI����һ��ͼƬ������߲�ͬDPI�µ�ͼƬ����
	*   ������DPI���Űٷֱ�Ϊ120�����Ŵ�120%����ͼƬ��"image.png" ��Ӧ�� "image@120.png"
	* @param [in] dpiScale ��Ҫ���ҵ�DPI���Űٷֱ�
	* @param [in] bIsUseZip �Ƿ�ʹ��zipѹ������Դ
	* @param [in] imageFullPath ͼƬ��Դ������·��
	* @param [out] dpiImageFullPath ����ָ��DPI�µ�ͼƬ��Դ·�������û�ҵ����򷵻ؿմ�
	*/
	bool FindDpiScaleImageFullPath(uint32_t dpiScale, 
								  bool bIsUseZip,
							      const std::wstring& imageFullPath,
							      std::wstring& dpiImageFullPath) const;

	/** ��ȡָ��DPI���Űٷֱ��µ�ͼƬ��Դ·��
	*   ������DPI���Űٷֱ�Ϊ120�����Ŵ�120%����ͼƬ��"image.png" ��Ӧ�� "image@120.png"
	* @param [in] dpiScale ��Ҫ���ҵ�DPI���Űٷֱ�
	* @param [in] imageFullPath ͼƬ��Դ������·��
	* @return ����ָ��DPI�µ�ͼƬ��Դ·��, ���ʧ���򷵻ؿմ�
	*/
	std::wstring GetDpiScaledPath(uint32_t dpiScale, const std::wstring& imageFullPath) const;

#ifdef UILIB_IMPL_WINSDK
	/** ��HICON�������һ��ͼƬ
	*/
	void LoadIconData(const Window* pWindow,
		              const ImageLoadAttribute& loadAtrribute,
		              std::unique_ptr<ImageInfo>& imageInfo) const;
#endif

private:
	/** �Ƿ�Ĭ�϶�����ͼƬ�ڼ���ʱ����DPI�������ţ������ȫ�����ԣ�Ĭ��Ϊtrue��Ӧ��������ͼƬ
	   ������Ϊtrue��Ҳ����ͨ����xml�У�ʹ��"dpiscale='false'"���Թر�ĳ��ͼƬ��DPI�Զ����ţ�
	*/
	bool m_bDpiScaleAllImages;

	/** �Ƿ�����ƥ���ٽ������Űٷֱ�ͼƬ
	*/
	bool m_bAutoMatchScaleImage;

	/** ͼƬ��Դӳ���ͼƬ��Key��ͼƬ���ݣ�
	*/
	std::unordered_map<std::wstring, std::weak_ptr<ImageInfo>> m_imageMap;

	/** ͼƬ��ԴKeyӳ���ͼƬ�ļ���Key��ͼƬKey��
	*/
	std::unordered_map <std::wstring, std::wstring> m_loadKeyMap;
};

}
#endif //UI_CORE_IMAGEMANAGER_H_
