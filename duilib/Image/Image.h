#ifndef UI_CORE_IMAGEDECODE_H_
#define UI_CORE_IMAGEDECODE_H_

#pragma once

#include "duilib/Render/IRender.h"
#include "duilib/Core/UiTypes.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace ui 
{
	class IRender;
	class Control;

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
	std::wstring GetImageFullPath() const;

	/** ���ø�ͼƬ�Ĵ�С�Ƿ��Ѿ�������ӦDPI����
	*/
	void SetBitmapSizeDpiScaled(bool isDpiScaled) { m_bDpiScaled = isDpiScaled; }

	/** �жϸ�ͼƬ�Ĵ�С�Ƿ��Ѿ�������ӦDPI����
	*/
	bool IsBitmapSizeDpiScaled() const { return m_bDpiScaled; }

	/** ����ͼƬ�Ŀ�͸�
	*/
	void SetImageSize(int32_t nWidth, int32_t nHeight);

	/** ��ȡͼƬ���
	*/
	int32_t GetWidth() const { return m_nWidth; }

	/** ��ȡͼƬ�߶�
	*/
	int32_t GetHeight() const { return m_nHeight; }

	/** ���һ��ͼƬ֡����, ��Ӻ����Դ�ɸ����ڲ��й�
	*/
	void SetFrameBitmap(const std::vector<IBitmap*>& frameBitmaps);

	/** ��ȡһ��ͼƬ֡����
	*/
	IBitmap* GetBitmap(uint32_t nIndex) const;

	/** ����ͼƬ�Ķ�֡�����¼����������Ϊ��λ ��
	*/
	void SetFrameInterval(const std::vector<int32_t>& frameIntervals);

	/** ��ȡͼƬ֡��Ӧ�Ĳ���ʱ����������Ϊ��λ ��
	*/
	int32_t GetFrameInterval(uint32_t nIndex) const;

	/** ��ȡͼƬ��֡��
	*/
	uint32_t GetFrameCount() const;

	/** �Ƿ�λ��֡ͼƬ(����GIF��)
	*/
	bool IsMultiFrameImage() const;

	/** ����ѭ�����Ŵ���(���ڵ���0���������0����ʾ������ѭ�����ŵ�, APNG��ʽ֧������ѭ�����Ŵ���)
	*/
	void SetPlayCount(int32_t nPlayCount);

	/** ��ȡѭ�����Ŵ���
	*@return ����ֵ��-1 ��ʾδ����
	*               0  ��ʾ������һ��ѭ�����ŵ�
	*              > 0 ��ʾ����ѭ�����ŵľ������
	*/
	int32_t GetPlayCount() const;

	/** ����ͼƬ�Ļ���KEY, ����ͼƬ���������ڹ���
	*/
	void SetCacheKey(const std::wstring& cacheKey);

	/** ��ȡͼƬ�Ļ���KEY
	*/
	std::wstring GetCacheKey() const;

private:
	//ͼƬ������·��
	UiString m_imageFullPath;

	//��ͼƬ�Ĵ�С�Ƿ��Ѿ�������ӦDPI����
	bool m_bDpiScaled;

	//ͼƬ�Ŀ��
	int32_t m_nWidth;
	
	//ͼƬ�ĸ߶�
	int32_t m_nHeight;

	//ͼƬ֡��Ӧ�Ĳ���ʱ����������Ϊ��λ ��
	std::vector<int32_t>* m_pFrameIntervals;

	//ͼƬ֡����
	IBitmap** m_pFrameBitmaps;

	//ͼƬ֡����
	uint32_t m_nFrameCount;

	//ѭ�����Ŵ���(���ڵ���0���������0����ʾ������ѭ�����ŵ�, APNG��ʽ֧������ѭ�����Ŵ���)
	int32_t m_nPlayCount;

	/** ͼƬ�Ļ���KEY, ����ͼƬ���������ڹ���
	*/
	UiString m_cacheKey;
};

/** ͼƬ����
*/
class UILIB_API ImageAttribute
{
public:
	ImageAttribute();
	~ImageAttribute();
	ImageAttribute(const ImageAttribute&);
	ImageAttribute& operator=(const ImageAttribute&);

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
	/** �ж�rcDest�����Ƿ�����Чֵ
	* @param [in] rcDest ��Ҫ�жϵ�����
	*/
	static bool HasValidImageRect(const UiRect& rcDest);

	/** ��ͼƬ��Դ����Ŀ������Բ�Ǵ�С����У��������DPI����Ӧ
	* @param [in] imageWidth ͼƬ�Ŀ��
	* @param [in] imageHeight ͼƬ�ĸ߶�
	* @param [in] bImageDpiScaled ͼƬ�Ƿ�����DPI����Ӧ����
	* @param [out] rcDestCorners ����Ŀ�������Բ����Ϣ�������������ڲ�����rcImageCorners�����ã�Ȼ�󴫳�
	* @param [in/out] rcSource ͼƬ����
	* @param [in/out] rcSourceCorners ͼƬ�����Բ����Ϣ
	*/
	static void ScaleImageRect(uint32_t imageWidth, uint32_t imageHeight, bool bImageDpiScaled,
		                       UiRect& rcDestCorners,
		                       UiRect& rcSource, UiRect& rcSourceCorners);

public:
	/** ��ȡrcSource
	*/
	UiRect GetSourceRect() const;

	/** ��ȡrcDest
	*/
	UiRect GetDestRect() const;

	/** rcPadding;
	*/
	UiPadding GetPadding() const;

	/** ��ȡrcCorner;
	*/
	UiRect GetCorner() const;

	/** ����ͼƬ���Ե��ڱ߾�(�ڲ�����DPI����Ӧ)
	*/
	void SetPadding(const UiPadding& newPadding);

public:
	//ͼƬ�ļ������ַ���
	UiString sImageString;

	//ͼƬ�ļ��ļ����������·��������������
	UiString sImagePath;

	//����ͼƬ��ȣ����ԷŴ����Сͼ��pixels���߰ٷֱ�%������300������30%
	UiString srcWidth;

	//����ͼƬ�߶ȣ����ԷŴ����Сͼ��pixels���߰ٷֱ�%������200������30%
	UiString srcHeight;

	//����ͼƬʱ��DPI����Ӧ���ԣ�������DPI����ͼƬ��С
	bool srcDpiScale;

	//����ͼƬʱ���Ƿ�������DPI����Ӧ����
	bool bHasSrcDpiScale;

	//�ڻ���Ŀ�������к�����뷽ʽ(���ָ����rcDestֵ�����ѡ����Ч)
	UiString hAlign;

	//�ڻ���Ŀ��������������뷽ʽ(���ָ����rcDestֵ�����ѡ����Ч)
	UiString vAlign;

	//͸���ȣ�0 - 255��
	uint8_t bFade;

	//����ƽ��
	bool bTiledX;

	//������ȫƽ�̣�����bTiledXΪtrueʱ��Ч
	bool bFullTiledX;

	//����ƽ��
	bool bTiledY;

	//������ȫƽ�̣�����bTiledYΪtrueʱ��Ч
	bool bFullTiledY;

	//ƽ��ʱ�ı߾ࣨ����bTiledXΪtrue����bTiledYΪtrueʱ��Ч��
	int32_t nTiledMargin;

	//�����GIF�ȶ���ͼƬ������ָ�����Ŵ��� -1 ��һֱ���ţ�ȱʡֵ��
	int32_t nPlayCount;	

	//�����ICO�ļ�������ָ����Ҫ���ص�ICOͼƬ�Ĵ�С
	//(ICO�ļ��а����ܶ����ͬ��С��ͼƬ����������256��48��32��16������ÿ����С����32λ��ʡ�256ɫ��16ɫ֮�֣�
	//ĿǰICO�ļ��ڼ���ʱ��ֻ��ѡ��һ����С��ICOͼƬ���м��أ����غ�Ϊ����ͼƬ
	uint32_t iconSize;

	//�ɻ��Ʊ�־��true��ʾ������ƣ�false��ʾ��ֹ����
	bool bPaintEnabled;

private:
	//����Ŀ������λ�úʹ�С������ڿؼ������λ�ã�
	UiRect* rcDest;

	//�ڻ���Ŀ�������е��ڱ߾�(���ָ����rcDestֵ�����ѡ����Ч)
	UiPadding16* rcPadding;

	//ͼƬԴ����λ�úʹ�С
	UiRect* rcSource;

	//Բ������
	UiRect* rcCorner;
};

/** ͼƬ�������ԣ����ڼ���һ��ͼƬ
*/
class UILIB_API ImageLoadAttribute
{
public:
	ImageLoadAttribute(std::wstring srcWidth,
					   std::wstring srcHeight,
		               bool srcDpiScale,
		               bool bHasSrcDpiScale,
		               uint32_t iconSize);

	/** ����ͼƬ·�������ؾ���·������ѹ�����ڵ����·����
	*/
	void SetImageFullPath(const std::wstring& imageFullPath);

	/** ��ȡͼƬ·�������ؾ���·������ѹ�����ڵ����·����
	*/
	std::wstring GetImageFullPath() const;

	/** ��ȡ����ͼƬ�Ļ���KEY
	*/
	std::wstring GetCacheKey() const;

	/** ���ü���ͼƬʱ���Ƿ���Ҫ����DPI����ͼƬ��С
	*/
	void SetNeedDpiScale(bool bNeedDpiScale);

	/** ��ȡ����ͼƬʱ���Ƿ���Ҫ����DPI����ͼƬ��С
	*/
	bool NeedDpiScale() const;

	/** ��ȡ����ͼƬʱ���Ƿ�������DPI����Ӧ���ԣ�����XML�ļ��У�����ͨ�����ã�"file='test.png' dpiscale='false'"��
	*/
	bool HasSrcDpiScale() const;

	/** ��ȡͼƬ���غ�Ӧ�����ŵĿ�Ⱥ͸߶�
	* @param [in,out] nImageWidth ����ԭʼͼƬ�Ŀ�ȣ����ؼ����Ŀ��
	* @param [in,out] nImageHeight ԭʼͼƬ�ĸ߶ȣ����ؼ����ĸ߶�
	* @return ����true��ʾͼƬ��С�����ţ�����false��ʾͼƬ��С������
	*/
	bool CalcImageLoadSize(uint32_t& nImageWidth, uint32_t& nImageHeight) const;

	/** �����ICO�ļ�������ָ����Ҫ���ص�ICOͼƬ�Ĵ�С
	*/
	uint32_t GetIconSize() const;

private:
	/** ��ȡ���õ����ź�Ĵ�С
	*/
	uint32_t GetScacledSize(const std::wstring& srcSize, uint32_t nImageSize) const;

private:
	//���ؾ���·������ѹ�����ڵ����·��������������
	UiString m_srcImageFullPath;

	//����ͼƬ��ȣ����ԷŴ����Сͼ��pixels���߰ٷֱ�%������300������30%
	UiString m_srcWidth;

	//����ͼƬ�߶ȣ����ԷŴ����Сͼ��pixels���߰ٷֱ�%������200������30%
	UiString m_srcHeight;

	//����ͼƬʱ������DPI����ͼƬ��С
	bool m_srcDpiScale;

	//����ͼƬʱ���Ƿ�������DPI����Ӧ����
	bool m_bHasSrcDpiScale;

	//�����ICO�ļ�������ָ����Ҫ���ص�ICOͼƬ�Ĵ�С
	//(ICO�ļ��а����ܶ����ͬ��С��ͼƬ����������256��48��32��16������ÿ����С����32λ��ʡ�256ɫ��16ɫ֮�֣�
	//ĿǰICO�ļ��ڼ���ʱ��ֻ��ѡ��һ����С��ICOͼƬ���м��أ����غ�Ϊ����ͼƬ
	uint32_t m_iconSize;
};

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
	int32_t GetImageFade(ControlStateType stateType) const;

	/** ��ȡͼƬ�ӿ�(�ɶ�����д)
	*/
	Image* GetStateImage(ControlStateType stateType);

public:
	/** �Ƿ����Hot״̬��ͼƬ
	*/
	bool HasHotImage() const;

	/** �Ƿ����״̬ͼƬ
	*/
	bool HasImage() const;

	/** ����ָ��״̬��ͼƬ
	* @param [in] pRender ���ƽӿ�
	* @param [in] stateType �ؼ�״̬������ѡ������ĸ�ͼƬ
	* @param [in] sImageModify ͼƬ�ĸ�������
	* @param [out] pDestRect ����ͼƬ���Ƶ�����Ŀ���������
	* @return ���Ƴɹ�����true, ���򷵻�false
	*/
	bool PaintStateImage(IRender* pRender, ControlStateType stateType, 
					     const std::wstring& sImageModify = L"",
		                 UiRect* pDestRect = nullptr);

	/** ��ȡ���ڹ���Control�ؼ���С����͸ߣ���ͼƬ�ӿ�
	*/
	Image* GetEstimateImage();

	/** ��ȡ����ͼƬ�ӿ�
	*/
	void GetAllImages(std::vector<Image*>& allImages);

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

	/** �Ƿ�״̬ͼƬ
	*/
	bool HasStateImages(void) const;

	/** �Ƿ���ָ�����͵�ͼƬ
	*/
	bool HasStateImage(StateImageType stateImageType) const;

	/** ����ָ��ͼƬ���ͺ�״̬��ͼƬ
	*/
	bool PaintStateImage(IRender* pRender, StateImageType stateImageType, ControlStateType stateType, const std::wstring& sImageModify = L"");
	
	/** ��ȡ���ڹ���Control�ؼ���С����͸ߣ���ͼƬ�ӿ�
	*/
	Image* GetEstimateImage(StateImageType stateImageType);

	/** ��ȡָ��ͼƬ���ͺ�״̬��ͼƬ�ӿ�
	*/
	Image* GetStateImage(StateImageType stateImageType, ControlStateType stateType);

	/** ��ȡ����ͼƬ�ӿ�
	*/
	void GetAllImages(std::vector<Image*>& allImages);

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
	std::map<ControlStateType, UiString> m_stateColorMap;
};

} // namespace ui

#endif // UI_CORE_IMAGEDECODE_H_
