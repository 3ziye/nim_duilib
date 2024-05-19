#include "ImageManager.h"
#include "duilib/Image/Image.h"
#include "duilib/Image/ImageDecoder.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/DpiManager.h"
#include "duilib/Core/Window.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Utils/FileUtil.h"

namespace ui 
{

ImageManager::ImageManager():
	m_bDpiScaleAllImages(true),
	m_bAutoMatchScaleImage(true)
{
}

ImageManager::~ImageManager()
{
}

std::shared_ptr<ImageInfo> ImageManager::GetImage(const Window* pWindow,
												  const ImageLoadAttribute& loadAtrribute)
{
	const DpiManager& dpi = (pWindow != nullptr) ? pWindow->Dpi() : GlobalManager::Instance().Dpi();
	//���Ҷ�Ӧ��ϵ��LoadKey ->(���һ) ImageKey ->(һ��һ) SharedImage
	std::wstring loadKey = loadAtrribute.GetCacheKey(dpi.GetScale());
	auto iter = m_loadKeyMap.find(loadKey);
	if (iter != m_loadKeyMap.end()) {
		const std::wstring& imageKey = iter->second;
		auto it = m_imageMap.find(imageKey);
		if (it != m_imageMap.end()) {
			std::shared_ptr<ImageInfo> sharedImage = it->second.lock();
			if (sharedImage) {
				//�ӻ����У��ҵ���ЧͼƬ��Դ��ֱ�ӷ���
				return sharedImage;
			}
		}
	}	

	//���¼�����Դ	
	std::unique_ptr<ImageInfo> imageInfo;
	std::wstring loadImageFullPath = loadAtrribute.GetImageFullPath();
	bool isIcon = false;
#ifdef UILIB_IMPL_WINSDK
	if (GlobalManager::Instance().Icon().IsIconString(loadImageFullPath)) {
		//����ICON
		isIcon = true;
		LoadIconData(pWindow, loadAtrribute, imageInfo);
	}
#endif

	bool isDpiScaledImageFile = false;
	if (!isIcon) {
		std::wstring imageFullPath = loadAtrribute.GetImageFullPath();
		bool isUseZip = GlobalManager::Instance().Zip().IsUseZip();
		std::wstring dpiImageFullPath;
		uint32_t nImageDpiScale = 0;
		//����DPI����ͼƬ���ܿ���������£����Ҷ�ӦDPI��ͼƬ�Ƿ����
		const bool bEnableImageDpiScale = IsDpiScaleAllImages();
		if (bEnableImageDpiScale && GetDpiScaleImageFullPath(dpi.GetScale(), isUseZip, imageFullPath,
									 dpiImageFullPath, nImageDpiScale)) {
			//���DPI����ӦͼƬ���ԣ����·����ͬ��˵���Ѿ�ѡ���˶�ӦDPI�µ��ļ�
			isDpiScaledImageFile = true;
			imageFullPath = dpiImageFullPath;
			ASSERT(!imageFullPath.empty());
			ASSERT(nImageDpiScale > 100);
		}
		else {
			nImageDpiScale = 100; //ԭʼͼƬ��δ��DPI����
			isDpiScaledImageFile = false;
		}
		//����ͼƬ��KEY
		ImageLoadAttribute realLoadAttribute = loadAtrribute;
		realLoadAttribute.SetImageFullPath(imageFullPath);
		std::wstring imageKey;
		if (isDpiScaledImageFile) {
			//�ж�ӦDPI��ͼƬ�ļ�
			imageKey = realLoadAttribute.GetCacheKey(nImageDpiScale);
		}
		else {
			//�޶�ӦDPI���űȵ�ͼƬ�ļ�
			imageKey = realLoadAttribute.GetCacheKey(0);
		}

		//����imageKey��ѯ����
		if (!imageKey.empty()) {
			auto it = m_imageMap.find(imageKey);
			if (it != m_imageMap.end()) {
				std::shared_ptr<ImageInfo> sharedImage = it->second.lock();
				if ((sharedImage != nullptr) && (sharedImage->GetLoadDpiScale() == dpi.GetScale())) {
					//�������DPI���Űٷֱ���ͬ
					return sharedImage;
				}
			}
		}

		//���ڴ����ݼ����ļ�
		std::vector<uint8_t> fileData;
		if (isUseZip) {
			GlobalManager::Instance().Zip().GetZipData(imageFullPath, fileData);
		}
		else {
			FileUtil::ReadFileData(imageFullPath, fileData);
		}
		ASSERT(!fileData.empty());

		imageInfo.reset();
		if (!fileData.empty()) {
			ImageDecoder imageDecoder;
			ImageLoadAttribute imageLoadAtrribute(loadAtrribute);
			if (isDpiScaledImageFile) {
				imageLoadAtrribute.SetNeedDpiScale(false);
			}			
			imageInfo = imageDecoder.LoadImageData(fileData, 
												   imageLoadAtrribute, 
												   bEnableImageDpiScale, nImageDpiScale, dpi);
			imageInfo->SetImageKey(imageKey);
		}
	}	
	std::shared_ptr<ImageInfo> sharedImage;
	if (imageInfo != nullptr) {
		std::wstring imageKey = imageInfo->GetImageKey();
		sharedImage.reset(imageInfo.release(), &OnImageInfoDestroy);
		sharedImage->SetLoadKey(loadKey);
		sharedImage->SetLoadDpiScale(dpi.GetScale());
		if (isDpiScaledImageFile) {
			//ʹ����DPI����Ӧ��ͼƬ������ǣ�����λtrueʱ�����޸����ֵ��
			sharedImage->SetBitmapSizeDpiScaled(isDpiScaledImageFile);
		}
		if (imageKey.empty()) {
			imageKey = loadKey;
		}

		//�����Ӧ��ϵ��LoadKey ->(���һ) ImageKey ->(һ��һ) SharedImage
		m_loadKeyMap[loadKey] = imageKey;
		m_imageMap[imageKey] = sharedImage;

#ifdef _DEBUG
		std::wstring log = L"Loaded Image: " + imageKey + L"\n";
		::OutputDebugString(log.c_str());
#endif
	}
	return sharedImage;
}

#ifdef UILIB_IMPL_WINSDK
void ImageManager::LoadIconData(const Window* pWindow, 
							    const ImageLoadAttribute& loadAtrribute,
								std::unique_ptr<ImageInfo>& imageInfo) const
{
	imageInfo.reset();
	//����HICON�������ΪͼƬ������Windowsƽ̨��������
	std::wstring iconString = loadAtrribute.GetImageFullPath();
	bool bEnableDpiScale = IsDpiScaleAllImages();
	std::vector<uint8_t> bitmapData;
	uint32_t imageWidth = 0;
	uint32_t imageHeight = 0;
	bool bDpiScaled = false;
	if (GlobalManager::Instance().Icon().LoadIconData(iconString, 
													  pWindow, loadAtrribute, bEnableDpiScale,
		                                              bitmapData, 
		                                              imageWidth, imageHeight, bDpiScaled)) {
		ASSERT(bitmapData.size() == (imageWidth * imageHeight * 4));
		IBitmap* pBitmap = nullptr;
		IRenderFactory* pRenderFactroy = GlobalManager::Instance().GetRenderFactory();
		ASSERT(pRenderFactroy != nullptr);
		if (pRenderFactroy != nullptr) {
			pBitmap = pRenderFactroy->CreateBitmap();		
		}
		ASSERT(pBitmap != nullptr);
		if (pBitmap != nullptr) {
			pBitmap->Init(imageWidth, imageHeight, true, bitmapData.data());
			std::vector<IBitmap*> frameBitmaps;
			frameBitmaps.push_back(pBitmap);
			imageInfo.reset(new ImageInfo);
			imageInfo->SetFrameBitmap(frameBitmaps);
			imageInfo->SetImageSize(imageWidth, imageHeight);
			imageInfo->SetImageFullPath(iconString);
			imageInfo->SetPlayCount(-1);
			imageInfo->SetBitmapSizeDpiScaled(bDpiScaled);
		}
	}
	ASSERT(imageInfo != nullptr);
}
#endif

void ImageManager::OnImageInfoDestroy(ImageInfo* pImageInfo)
{
	ASSERT(pImageInfo != nullptr);
	ImageManager& imageManager = GlobalManager::Instance().Image();
	if (pImageInfo != nullptr) {
		std::wstring imageKey;
		std::wstring loadKey = pImageInfo->GetLoadKey();
		if (!loadKey.empty()) {			
			auto iter = imageManager.m_loadKeyMap.find(loadKey);
			if (iter != imageManager.m_loadKeyMap.end()) {
				imageKey = iter->second;
				imageManager.m_loadKeyMap.erase(iter);
			}
			if (imageKey.empty()) {
				auto it = imageManager.m_imageMap.find(imageKey);
				if (it != imageManager.m_imageMap.end()) {
					imageManager.m_imageMap.erase(it);
				}
			}
		}
		delete pImageInfo;
#ifdef _DEBUG
		std::wstring log = L"Removed Image: " + imageKey + L"\n";
		::OutputDebugString(log.c_str());
#endif
	}	
}

void ImageManager::RemoveAllImages()
{
	m_imageMap.clear();
}

void ImageManager::SetDpiScaleAllImages(bool bEnable)
{
	m_bDpiScaleAllImages = bEnable;
}

bool ImageManager::IsDpiScaleAllImages() const
{
	return m_bDpiScaleAllImages;
}

void ImageManager::SetAutoMatchScaleImage(bool bAutoMatchScaleImage)
{
	m_bAutoMatchScaleImage = bAutoMatchScaleImage;
}

bool ImageManager::IsAutoMatchScaleImage() const
{
	return m_bAutoMatchScaleImage;
}

bool ImageManager::GetDpiScaleImageFullPath(uint32_t dpiScale,
										    bool bIsUseZip,
										    const std::wstring& imageFullPath,
										    std::wstring& dpiImageFullPath,
	                                        uint32_t& nImageDpiScale) const
{
	nImageDpiScale = 0;
	if (FindDpiScaleImageFullPath(dpiScale, bIsUseZip, imageFullPath, dpiImageFullPath)) {
		nImageDpiScale = dpiScale;
		return true;
	}
	dpiImageFullPath.clear();
	if (!IsAutoMatchScaleImage() || (dpiScale < 115)) {
		//����ƥ�书�ܹر�, ��ǰDPI��������ƥ�����Űٷֱ�
		return false;
	}

	std::wstring dpiFullPath;
	std::vector<uint32_t> allScales = {125, 150, 175, 200, 225, 250, 300};
	std::vector<std::pair<uint32_t, std::wstring>> allDpiImagePath;
	for (auto scale : allScales) {
		if (FindDpiScaleImageFullPath(scale, bIsUseZip, imageFullPath, dpiFullPath)) {
			allDpiImagePath.push_back({ scale, dpiFullPath });
		}
	}

	dpiImageFullPath.clear();
	size_t nCount = allDpiImagePath.size();
	for (size_t index = 0; index < nCount; ++index) {
		uint32_t nScale = allDpiImagePath[index].first;
		const std::wstring& sPath = allDpiImagePath[index].second;
		if (nScale > dpiScale) {
			if (index == 0) {
				//��һ��
				dpiImageFullPath = sPath;
				nImageDpiScale = nScale;
				break;
			}
			else {
				//�������м�, ѡһ����ӽ���
				uint32_t nLastScale = allDpiImagePath[index - 1].first;
				ASSERT(nLastScale <= dpiScale);
				float diffScaleLast = ((float)dpiScale - (float)nLastScale) / (float)nLastScale;
				float diffScale = ((float)nScale - (float)dpiScale) / (float)nScale;
				if (diffScaleLast < diffScale) {
					dpiImageFullPath = allDpiImagePath[index - 1].second;
					nImageDpiScale = allDpiImagePath[index - 1].first;
				}
				else {
					dpiImageFullPath = sPath;
					nImageDpiScale = nScale;
				}
				break;
			}
		}
		else if (index == (nCount - 1)) {
			//���һ��
			dpiImageFullPath = sPath;
			nImageDpiScale = nScale;
		}
	}
	return !dpiImageFullPath.empty();
}

bool ImageManager::FindDpiScaleImageFullPath(uint32_t dpiScale,
											 bool bIsUseZip,
											 const std::wstring& imageFullPath,
											 std::wstring& dpiImageFullPath) const
{
	dpiImageFullPath.clear();
	if ((dpiScale == 100) || (dpiScale == 0)) {
		//��ǰDPI��������
		return false;
	}
	dpiImageFullPath = GetDpiScaledPath(dpiScale, imageFullPath);
	if (dpiImageFullPath.empty()) {
		return false;
	}

	bool bExists = false;
	if (bIsUseZip) {
		bExists = GlobalManager::Instance().Zip().IsZipResExist(dpiImageFullPath);
	}
	else {
		bExists = StringHelper::IsExistsPath(dpiImageFullPath);
	}
	if (!bExists) {
		dpiImageFullPath.clear();
	}
	return bExists;
}

std::wstring ImageManager::GetDpiScaledPath(uint32_t dpiScale, const std::wstring& imageFullPath) const
{
	std::wstring strPathDir;
	std::wstring strPathFileName;
	std::list<std::wstring> strPathList = StringHelper::Split(imageFullPath, L"\\");
	for (auto it = strPathList.begin(); it != strPathList.end(); ++it) {
		auto itTemp = it;
		if (++itTemp == strPathList.end()) {
			strPathFileName = *it;
		}
		else {
			strPathDir += *it + L"\\";
		}
	}

	size_t iPointPos = strPathFileName.rfind('.');
	ASSERT(iPointPos != std::wstring::npos);
	if (iPointPos == std::wstring::npos) {
		return std::wstring();
	}
	std::wstring strFileExtension = strPathFileName.substr(iPointPos, strPathFileName.size() - iPointPos);
	std::wstring strFile = strPathFileName.substr(0, iPointPos);
	//����ָ��DPI�µ�ͼƬ������DPI���Űٷֱ�Ϊ120�����Ŵ�120%����ͼƬ��"image.png" ��Ӧ�� "image@120.png"
	strPathFileName = StringHelper::Printf(L"%s%s%d%s", strFile.c_str(), L"@", dpiScale, strFileExtension.c_str());
	std::wstring strNewFilePath = strPathDir + strPathFileName;
	return strNewFilePath;
}

}

