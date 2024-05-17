#ifndef UI_IMAGE_IMAGE_ATTRIBUTE_H_
#define UI_IMAGE_IMAGE_ATTRIBUTE_H_

#pragma once

#include "duilib/Core/UiTypes.h"

namespace ui 
{
class DpiManager;

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
	* @param [in] dpi DPI���Žӿ�
	*/
	void InitByImageString(const std::wstring& strImageString, const DpiManager& dpi);

	/** ����ͼƬ�����޸�����ֵ�������������õ�ͼƬ����, δ���������Բ����и��£�
	* @param [in] strImageString ͼƬ�����ַ���
	* @param [in] dpi DPI���Žӿ�
	*/
	void ModifyAttribute(const std::wstring& strImageString, const DpiManager& dpi);

public:
	/** �ж�rcDest�����Ƿ�����Чֵ
	* @param [in] rcDest ��Ҫ�жϵ�����
	*/
	static bool HasValidImageRect(const UiRect& rcDest);

	/** ��ͼƬ��Դ����Ŀ������Բ�Ǵ�С����У��������DPI����Ӧ
	* @param [in] imageWidth ͼƬ�Ŀ��
	* @param [in] imageHeight ͼƬ�ĸ߶�
	* @param [in] dpi DPI���Žӿ�
	* @param [in] bImageDpiScaled ͼƬ�Ƿ�����DPI����Ӧ����
	* @param [out] rcDestCorners ����Ŀ�������Բ����Ϣ�������������ڲ�����rcImageCorners�����ã�Ȼ�󴫳�
	* @param [in/out] rcSource ͼƬ����
	* @param [in/out] rcSourceCorners ͼƬ�����Բ����Ϣ
	*/
	static void ScaleImageRect(uint32_t imageWidth, uint32_t imageHeight, 
							   const DpiManager& dpi, bool bImageDpiScaled,
		                       UiRect& rcDestCorners,
		                       UiRect& rcSource, UiRect& rcSourceCorners);

public:
	/** ��ȡrcSource(δ����DPI����)
	*/
	UiRect GetImageSourceRect() const;

	/** ��ȡrcCorner(δ����DPI����)
	*/
	UiRect GetImageCorner() const;

	/** ��ȡrcDest(�����þ����Ƿ����DPI����)
	*/
	UiRect GetImageDestRect(const DpiManager& dpi) const;

	/** ��ȡͼƬ���Ե��ڱ߾�
	* @param [in] dpi DPI���Ź�����
	* @return ���ذ��մ���DPI���Ź�������Ӧ���ڱ߾�����
	*/
	UiPadding GetImagePadding(const DpiManager& dpi) const;

	/** ����ͼƬ���Ե��ڱ߾�(�ڲ�����DPI����Ӧ)
	* @param [in] newPadding ��Ҫ���õ��ڱ߾�
	* @param [in] bNeedDpiScale �Ƿ���Ҫ��newPadding����DPI����
	* @param [in] dpi ��newPadding���ݹ�����DPI������
	*/
	void SetImagePadding(const UiPadding& newPadding, bool bNeedDpiScale, const DpiManager& dpi);

public:
	//ͼƬ�ļ������ַ���
	UiString sImageString;

	//ͼƬ�ļ��ļ����������·��������������
	UiString sImagePath;

	//����ͼƬ��ȣ����ԷŴ����Сͼ��pixels���߰ٷֱ�%������300������30%
	UiString srcWidth;

	//����ͼƬ�߶ȣ����ԷŴ����Сͼ��pixels���߰ٷֱ�%������200������30%
	UiString srcHeight;

	//rcSource��DPI����Ӧ���ԣ�����bHasSrcDpiScaleΪtrueʱ��Ч��
	bool srcDpiScale;

	//����ͼƬʱ���Ƿ�������DPI����Ӧ���ԣ�"dpi_scale"��
	bool bHasSrcDpiScale;

	//rcDest���Ե�DPI����Ӧ���ԣ�����bHasDestDpiScaleʱ��Ч��
	bool destDpiScale;

	//rcDest�Ƿ�������DPI����Ӧ���ԣ�"dest_scale"��
	bool bHasDestDpiScale;

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
	//����Ŀ������λ�úʹ�С(����ڿؼ������λ��, δ����DPI����)
	UiRect* rcDest;

	//�ڻ���Ŀ�������е��ڱ߾�(���ָ����rcDestֵ�����ѡ����Ч)
	UiPadding16* rcPadding;

	//rcPadding��Ӧ��DPI���Űٷֱ�
	uint16_t rcPaddingScale;

	//ͼƬԴ����λ�úʹ�С(δ����DPI����)
	UiRect* rcSource;

	//Բ������(δ����DPI����)
	UiRect* rcCorner;
};

} // namespace ui

#endif // UI_IMAGE_IMAGE_ATTRIBUTE_H_
