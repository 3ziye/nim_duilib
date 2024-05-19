#ifndef UI_CORE_IMAGE_LIST_H_
#define UI_CORE_IMAGE_LIST_H_

#pragma once

#include "duilib/Image/Image.h"
#include "duilib/Core/UiTypes.h"
#include <map>
#include <string>
#include <vector>

namespace ui 
{
/** ͼƬ��Դ������ָ��
*/
typedef std::shared_ptr<Image> ImagePtr;

/** Image�б�
 */
class UILIB_API ImageList
{
public:
	ImageList();
	~ImageList();
	ImageList(const ImageList&) = delete;
	ImageList& operator = (const ImageList&) = delete;

public:
	/** ����ͼƬ��С����ѡ���ã���������ã����ÿ��ͼƬ��ȡ��
	* @param [in] imageSize ͼƬ��С, �ڲ�������DPI���ţ���Ҫ���ⲿ����DPI����
	* @param [in] dpi DPI���Ź�����
	* @param [in] bNeedDpiScale �Ƿ�֧��DPI����
	*/
	void SetImageSize(UiSize imageSize, const DpiManager& dpi, bool bNeedDpiScale);

	/** ��ȡͼƬ��С
	* @return �������õ�ͼƬ��С�����δ���ã��򷵻�(0,0)
	*/
	UiSize GetImageSize() const;

	/** ���õ�DPI�仯ʱ��ͼƬ��С�Ƿ���ҪDPI���ţ�Ĭ�Ͽ���
	* @param [in] bEnableChangeImageSizeDpi true��ʾ�������ܣ�false��ʾ�رչ���
	*/
	void EnableChangeImageSizeDpi(bool bEnableChangeImageSizeDpi);

	/** ��DPI�仯ʱ��ͼƬ��С�Ƿ���ҪDPI���ţ�Ĭ�Ͽ���
	* @param [in] bEnableChangeImageSizeDpi true��ʾ�������ܣ�false��ʾ�رչ���
	*/
	bool IsEnableChangeImageSizeDpi() const;

	/** DPI�����仯������ͼƬ��С
	* @param [in] nOldDpiScale �ɵ�DPI���Űٷֱ�
	* @param [in] dpi DPI���Ź�����
	*/
	void ChangeDpiScale(const DpiManager& dpi, uint32_t nOldDpiScale);

	/** ���һ��ͼƬ��Դ
	* @param [in] imageString ͼƬ��Դ�ַ�������ʽͬ��Image::SetImageString����
	* @param [in] dpi DPI���Ź���ӿ�
	* @return ����ͼƬ��Դ��ID�����ʧ�ܷ���-1�����򷵻ش��ڵ���0��ID
	*/
	int32_t AddImageString(const std::wstring& imageString, const DpiManager& dpi);

	/** ��ȡͼƬ��Դ�ַ���
	* @param [in] imageId ͼƬ��Դ��ID����AddImage�������ص�ֵ
	* @return ���û�й�����Դ�����ؿմ�
	*/
	std::wstring GetImageString(int32_t imageId) const;

	/** ��ȡͼƬ��Դ�ӿ�(������Դ)
	* @param [in] imageId ͼƬ��Դ��ID����AddImage�������ص�ֵ
	* @return ���û�й�����Դ������nullptr
	*/
	ImagePtr GetImageData(int32_t imageId) const;

	/** ��ȡͼƬ��Դ��ID
	* @param [in] imageString ͼƬ��Դ�ַ�������ʽͬ��Image::SetImageString����
	* @return ���ع���ͼƬ��Դ��ID�����û�й�����Դ������-1
	*/
	int32_t GetImageStringId(const std::wstring& imageString) const;

	/** �Ƴ�һ��ͼƬ��Դ
	* @param [in] imageId ͼƬ��Դ��ID����AddImage�������ص�ֵ
	*/
	bool RemoveImageString(int32_t imageId);

	/** ��ȡͼƬ��Դ����
	*/
	size_t GetImageCount() const;

	/** ��ȡ����ͼƬ��Դ��ID
	*/
	void GetImageIdList(std::vector<int32_t>& imageIdList) const;

	/** �������ͼƬ��Դ
	*/
	void Clear();
 
private:
	/** ��һ��ID
	*/
	int32_t m_nNextID;

	/** ͼƬ��С����ѡ���ã���������ã����ÿ��ͼƬ��ȡ��
	*/
	UiSize m_imageSize;

	/** ��DPI�仯ʱ��ͼƬ��С�Ƿ���ҪDPI���ţ�Ĭ�Ͽ���
	*/
	bool m_bEnableChangeImageSizeDpi;

	/** ͼƬ��Դӳ���
	*/
	std::map<int32_t, ImagePtr> m_imageMap;
};

} //namespace ui 

#endif //UI_CORE_IMAGE_LIST_H_
