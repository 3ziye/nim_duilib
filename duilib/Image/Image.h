#ifndef UI_IMAGE_IMAGE_H_
#define UI_IMAGE_IMAGE_H_

#pragma once

#include "duilib/Image/ImageInfo.h"
#include "duilib/Image/ImageAttribute.h"
#include "duilib/Image/ImageLoadAttribute.h"
#include "duilib/Image/StateImageMap.h"
#include <memory>

namespace ui 
{
class Control;

/** ͼƬ��ط�װ��֧�ֵ��ļ���ʽ��SVG/PNG/GIF/JPG/BMP/APNG/WEBP/ICO
*/
class UILIB_API Image
{
public:
	Image();
	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;

	/** ��ʼ��ͼƬ����
	*/
	void InitImageAttribute();

	/** ���ò���ʼ��ͼƬ����
	*@param [in] strImageString ͼƬ�����ַ���
	*/
	void SetImageString(const std::wstring& strImageString);

	/** ��ȡͼƬ���ԣ����ļ�������ͼƬ�������Եȣ�
	*/
	std::wstring GetImageString() const;

	/** �ж�ͼƬ�����Ƿ����
	*/
	bool EqualToImageString(const std::wstring& imageString) const;

	/** ��ȡͼƬ�ļ����������·��������ͼƬ���ԣ�
	*/
	std::wstring GetImagePath() const;

	/** ����ͼƬ���Ե��ڱ߾�
	*/
	void SetImagePadding(const UiPadding& newPadding);

	/** ��ȡͼƬ���Ե��ڱ߾�
	*/
	UiPadding GetImagePadding() const;

	/** �ж��Ƿ����ͼƬ����
	*/
	bool IsImagePaintEnabled() const;

	/** �����Ƿ��ֹ����ͼƬ����
	*/
	void SetImagePaintEnabled(bool bEnable);

	/** ��ȡͼƬ���ԣ�ֻ����
	*/
	const ImageAttribute& GetImageAttribute() const;

	/** ��ȡͼƬ��������
	*/
	ImageLoadAttribute GetImageLoadAttribute() const;

public:
	/** ��ȡͼƬ��Ϣ�ӿ�
	*/
	const std::shared_ptr<ImageInfo>& GetImageCache() const;

	/** ����ͼƬ��Ϣ�ӿ�
	*/
	void SetImageCache(const std::shared_ptr<ImageInfo>& imageInfo);

	/** ���ͼƬ��Ϣ��������, �ͷ���Դ
	*/
	void ClearImageCache();

public:
	/** ����ͼƬ���ԣ����Ŵ�����������֡ͼƬʱ��
	*/
	void SetImagePlayCount(int32_t nPlayCount);

	/** ����ͼƬ���ԣ�͸���ȣ�������֡ͼƬʱ��
	*/
	void SetImageFade(uint8_t nFade);

	/** �Ƿ����ڲ����У�������֡ͼƬʱ��
	*/
	bool IsPlaying() const { return m_bPlaying; }

	/** �����Ƿ����ڲ����У�������֡ͼƬʱ��
	*/
	void SetPlaying(bool bPlaying) { m_bPlaying = bPlaying; }

	/** ������һ֡��������֡ͼƬʱ��
	*/
	bool IncrementCurrentFrame();

	/** ���õ�ǰͼƬ֡��������֡ͼƬʱ��
	*/
	void SetCurrentFrame(uint32_t nCurrentFrame);

	/** ��ȡ��ǰͼƬ֡������������֡ͼƬʱ��
	*/
	uint32_t GetCurrentFrameIndex() const;

	/** ��ȡ��ǰͼƬ֡��ͼƬ����
	*/
	IBitmap* GetCurrentBitmap() const;

	/** ��ȡ��ǰͼƬ֡���ŵ�ʱ��������λ: ���룬������֡ͼƬʱ��
	*/
	int32_t GetCurrentInterval() const;

	/** ��ȡ��ǰ��ѭ�����ŵĴ�����������֡ͼƬʱ��
	*/
	int32_t GetCycledCount() const;

	/** ��յ�ǰ��ѭ�����ŵĴ�����������֡ͼƬʱ��
	*/
	void ClearCycledCount();

	/** �ж��Ƿ�Ӧ�ü������ţ�������֡ͼƬʱ��
	*/
	bool ContinuePlay() const;

private:

	//��ǰ���ڲ��ŵ�ͼƬ֡��������֡ͼƬʱ��
	uint32_t m_nCurrentFrame;

	//�Ƿ����ڲ��ţ�������֡ͼƬʱ��
	bool m_bPlaying;

	//�Ѳ��Ŵ�����������֡ͼƬʱ��
	int32_t m_nCycledCount;

	//ͼƬ����
	ImageAttribute m_imageAttribute;

	//ͼƬ��Ϣ
	std::shared_ptr<ImageInfo> m_imageCache;
};

} // namespace ui

#endif // UI_IMAGE_IMAGE_H_
