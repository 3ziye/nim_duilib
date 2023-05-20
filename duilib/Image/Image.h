#ifndef UI_CORE_IMAGEDECODE_H_
#define UI_CORE_IMAGEDECODE_H_

#pragma once

#include "duilib/Core/Define.h"
#include "duilib/duilib_defs.h"
#include "duilib/Render/IRender.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace ui 
{
	class IRender;

/** ͼƬ��Ϣ
*/
class UILIB_API ImageInfo
{
public:
	ImageInfo();
	~ImageInfo();

	ImageInfo(const ImageInfo&) = delete;
	ImageInfo& operator = (const ImageInfo&) = delete;

public:
	/** ����ͼƬ·��
	*/
	void SetImageFullPath(const std::wstring& path);

	/** ��ȡͼƬ·��
	*/
	const std::wstring& GetImageFullPath() const;

	/** �����Ƿ���Alphaͨ��
	*/
	void SetAlpha(bool bAlphaChannel) {	m_bAlphaChannel = bAlphaChannel; }

	/** �ж��Ƿ���Alphaͨ��
	*/
	bool IsAlpha() const { return m_bAlphaChannel; }
	
	/** �����Ƿ񱣴��ڻ�����
	*/
	void SetCached(bool bCached) { m_bCached = bCached; }

	/** �ж��Ƿ񱣴��ڻ�����
	*/
	bool IsCached() const { return m_bCached; }

	/** ���ø�ͼƬ�Ĵ�С�Ƿ��Ѿ�������ӦDPI����
	*/
	void SetBitmapSizeDpiScaled(bool isDpiScaled) { m_bDpiScaled = isDpiScaled; }

	/** �жϸ�ͼƬ�Ĵ�С�Ƿ��Ѿ�������ӦDPI����
	*/
	bool IsBitmapSizeDpiScaled() const { return m_bDpiScaled; }

	/** ���һ��ͼƬ֡����, ��Ӻ����Դ�ɸ����ڲ��й�
	*/
	void PushBackHBitmap(IBitmap* pBitmap);

	/** ��ȡһ��ͼƬ֡����
	*/
	IBitmap* GetBitmap(size_t nIndex) const;

	/** ����ͼƬ�Ŀ�͸�
	*/
	void SetImageSize(int nWidth, int nHeight);

	/** ��ȡͼƬ���
	*/
	int GetWidth() const { return m_nWidth; }

	/** ��ȡͼƬ�߶�
	*/
	int GetHeight() const { return m_nHeight; }

	/** ��ȡͼƬ��֡��
	*/
	size_t GetFrameCount() const;

	/** �Ƿ�λ��֡ͼƬ(����GIF��)
	*/
	bool IsMultiFrameImage() const;

	/** ����ͼƬ�Ķ�֡�����¼����������Ϊ��λ ��
	*/
	void SetFrameInterval(const std::vector<int>& frameIntervals);

	/** ��ȡͼƬ֡��Ӧ�Ĳ���ʱ����������Ϊ��λ ��
	*/
	int GetFrameInterval(size_t nIndex);

private:
	//ͼƬ������·��
	std::wstring m_imageFullPath;

	//�Ƿ����Alphaͨ��
	bool m_bAlphaChannel;

	//�Ƿ񱣴��ڻ�����
	bool m_bCached;

	//��ͼƬ�Ĵ�С�Ƿ��Ѿ�������ӦDPI����
	bool m_bDpiScaled;

	//ͼƬ�Ŀ��
	int m_nWidth;
	
	//ͼƬ�ĸ߶�
	int m_nHeight;

	//ͼƬ֡��Ӧ�Ĳ���ʱ����������Ϊ��λ ��
	std::vector<int> m_frameIntervals;

	//ͼƬ֡����
	std::vector<IBitmap*> m_frameBitmaps;
};

/** ͼƬ����
*/
class UILIB_API ImageAttribute
{
public:
	ImageAttribute();

	/** �����ݳ�Ա���г�ʼ��
	*/
	void Init();

	/** ����ͼƬ�������г�ʼ��(�ȵ���Init��ʼ����Ա�������ٰ��մ���������и��²�������)
	* @param [in] strImageString ͼƬ�����ַ���
	*/
	void InitByImageString(const std::wstring& strImageString);

	/** ����ͼƬ�����޸�����ֵ�������������õ�ͼƬ����, δ���������Բ����и��£�
	* @param [in] strImageString ͼƬ�����ַ���
	*/
	void ModifyAttribute(const std::wstring& strImageString);

public:
	//ͼƬ�ļ������ַ���
	std::wstring sImageString;

	//ͼƬ�ļ��ļ����������·��������������
	std::wstring sImagePath;

	//����Ŀ������λ�úʹ�С������ڿؼ������λ�ã�
	UiRect rcDest;

	//ͼƬԴ����λ�úʹ�С
	UiRect rcSource;

	//Բ������
	UiRect rcCorner;

	//͸���ȣ�0 - 255��
	BYTE bFade;

	//����ƽ��
	bool bTiledX;

	//������ȫƽ�̣�δ�õ�
	bool bFullTiledX;

	//����ƽ��
	bool bTiledY;

	//������ȫƽ��
	bool bFullTiledY;

	//ƽ��ʱ�ı߾�
	int nTiledMargin;

	//�����GIF�ȶ���ͼƬ������ָ�����Ŵ��� -1 ��һֱ���ţ�ȱʡֵ��
	int nPlayCount;	
};

/** ͼƬ��ط�װ��֧�ֵ��ļ���ʽ��SVG/PNG/GIF/JPG/BMP/
*/
class UILIB_API Image
{
public:
	Image();

	/** ��ʼ��ͼƬ����
	*/
	void InitImageAttribute();

	/** ����ͼƬ����
	*@param [in] strImageString ͼƬ�����ַ���
	*/
	void SetImageString(const std::wstring& strImageString);

	/** ��ȡͼƬ���ԣ����ļ�������ͼƬ�������Եȣ�
	*/
	const std::wstring& GetImageString() const;

	/** ��ȡͼƬ�ļ����������·��������ͼƬ���ԣ�
	*/
	const std::wstring GetImagePath() const;

	/** ��ȡͼƬ���ԣ�ֻ����
	*/
	const ImageAttribute& GetImageAttribute() const;

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
	void SetImagePlayCount(int nPlayCount);

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
	void SetCurrentFrame(size_t nCurrentFrame);

	/** ��ȡ��ǰͼƬ֡������������֡ͼƬʱ��
	*/
	size_t GetCurrentFrameIndex() const;

	/** ��ȡ��ǰͼƬ֡��ͼƬ����
	*/
	IBitmap* GetCurrentBitmap() const;

	/** ��ȡ��ǰͼƬ֡���ŵ�ʱ��������λ: ���룬������֡ͼƬʱ��
	*/
	int GetCurrentInterval() const;

	/** ��ȡ��ǰ��ѭ�����ŵĴ�����������֡ͼƬʱ��
	*/
	int GetCycledCount() const;

	/** ��յ�ǰ��ѭ�����ŵĴ�����������֡ͼƬʱ��
	*/
	void ClearCycledCount();

	/** �ж��Ƿ�Ӧ�ü������ţ�������֡ͼƬʱ��
	*/
	bool ContinuePlay() const;

private:

	//��ǰ���ڲ��ŵ�ͼƬ֡��������֡ͼƬʱ��
	size_t m_nCurrentFrame;

	//�Ƿ����ڲ��ţ�������֡ͼƬʱ��
	bool m_bPlaying;

	//�Ѳ��Ŵ�����������֡ͼƬʱ��
	int m_nCycledCount;

	//ͼƬ����
	ImageAttribute m_imageAttribute;

	//ͼƬ��Ϣ
	std::shared_ptr<ImageInfo> m_imageCache;
};

/** �ؼ�״̬��ͼƬ��ӳ��
*/
class UILIB_API StateImage
{
public:
	StateImage();

	/** ���ù����Ŀؼ��ӿ�
	*/
	void SetControl(Control* control) {	m_pControl = control; }

	/** ����ͼƬ����
	*@param [in] stateType ͼƬ����
	*@param [in] strImageString ͼƬ�����ַ���
	*/
	void SetImageString(ControlStateType stateType, const std::wstring& strImageString);

	/** ��ȡͼƬ����
	*@param [in] stateType ͼƬ����
	*/
	std::wstring GetImageString(ControlStateType stateType) const;

	/** ��ȡͼƬ�ļ���
	*@param [in] stateType ͼƬ����
	*/
	std::wstring GetImagePath(ControlStateType stateType) const;

	/** ��ȡͼƬ��Դ�����С
	*@param [in] stateType ͼƬ����
	*/
	UiRect GetImageSourceRect(ControlStateType stateType) const;

	/** ��ȡͼƬ��͸����
	*@param [in] stateType ͼƬ����
	*/
	int GetImageFade(ControlStateType stateType) const;

	/** ��ȡͼƬ�ӿ�(�ɶ�����д)
	*/
	Image& GetStateImage(ControlStateType stateType) { return m_stateImageMap[stateType]; }

public:
	/** �Ƿ����Hot״̬��ͼƬ
	*/
	bool HasHotImage() const;

	/** �Ƿ����״̬ͼƬ
	*/
	bool HasImage() const;

	/** ����ָ��״̬��ͼƬ
	*/
	bool PaintStateImage(IRender* pRender, ControlStateType stateType, const std::wstring& sImageModify = L"");

	/** ��ȡ���ڹ����ͼƬ�ӿ�
	*/
	Image* GetEstimateImage() ;

	/** ���ͼƬ���棬�ͷ���Դ
	*/
	void ClearImageCache();

private:
	//�����Ŀؼ��ӿ�
	Control* m_pControl;

	//ÿ��״̬��ͼƬ�ӿ�
	std::map<ControlStateType, Image> m_stateImageMap;
};

/** �ؼ�ͼƬ������״̬ͼƬ��ӳ��
*/
class UILIB_API StateImageMap
{
public:
	StateImageMap();

	/** ���ù����Ŀؼ��ӿ�
	*/
	void SetControl(Control* control);

	/** ����ͼƬ����
	*@param [in] stateImageType ͼƬ���ͣ���������״̬ǰ��ͼƬ������ͼƬ��ѡ��״̬��ǰ��ͼƬ������ͼƬ��
	*@param [in] stateType ͼƬ״̬���������������㡢���¡�����״̬��
	*@param [in] strImagePath ͼƬ�����ַ���
	*/
	void SetImageString(StateImageType stateImageType, ControlStateType stateType, const std::wstring& strImagePath);

	/** ��ȡͼƬ����
	*@param [in] stateImageType ͼƬ���ͣ���������״̬ǰ��ͼƬ������ͼƬ��ѡ��״̬��ǰ��ͼƬ������ͼƬ��
	*@param [in] stateType ͼƬ״̬���������������㡢���¡�����״̬��
	*/
	std::wstring GetImageString(StateImageType stateImageType, ControlStateType stateType) const;

	/** �Ƿ���Hot״̬��ͼƬ
	*/
	bool HasHotImage() const;

	/** �Ƿ���ָ�����͵�ͼƬ
	*/
	bool HasImageType(StateImageType stateImageType) const;

	/** ����ָ��ͼƬ���ͺ�״̬��ͼƬ
	*/
	bool PaintStateImage(IRender* pRender, StateImageType stateImageType, ControlStateType stateType, const std::wstring& sImageModify = L"");
	
	/** ��ȡ���ڹ����ͼƬ�ӿ�
	*/
	Image* GetEstimateImage(StateImageType stateImageType);

	/** �������ͼƬ���͵Ļ��棬�ͷ���Դ
	*/
	void ClearImageCache();

private:
	//�����Ŀؼ��ӿ�
	Control* m_pControl;

	//ÿ��ͼƬ���͵�״̬ͼƬ(����״̬ǰ��ͼƬ������ͼƬ��ѡ��״̬��ǰ��ͼƬ������ͼƬ)
	std::map<StateImageType, StateImage> m_stateImageMap;
};

/** �ؼ�״̬����ɫֵ��ӳ��
*/
class UILIB_API StateColorMap
{
public:
	StateColorMap();

	/** ���ù���Ŀؼ��ӿ�
	*/
	void SetControl(Control* control);

	/** ��ȡ��ɫֵ���������������ɫ���򷵻ؿ�
	*/
	std::wstring GetStateColor(ControlStateType stateType) const;

	/** ������ɫֵ
	*/
	void SetStateColor(ControlStateType stateType, const std::wstring& color);

	/** �Ƿ����Hot״̬����ɫ
	*/
	bool HasHotColor() const;

	/** �Ƿ���ָ����ɫֵ
	*/
	bool HasStateColor(ControlStateType stateType) const;

	/** �Ƿ�����ɫֵ
	*/
	bool HasStateColors() const ;

	/** ����ָ��״̬����ɫ
	*/
	void PaintStateColor(IRender* pRender, UiRect rcPaint, ControlStateType stateType) const;

private:
	//�����Ŀؼ��ӿ�
	Control* m_pControl;

	//״̬����ɫֵ��ӳ���
	std::map<ControlStateType, std::wstring> m_stateColorMap;
};

} // namespace ui

#endif // UI_CORE_IMAGEDECODE_H_
