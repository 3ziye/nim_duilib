#ifndef UI_IMAGE_IMAGE_DECODER_H_
#define UI_IMAGE_IMAGE_DECODER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <memory>
#include <vector>
#include <string>

namespace ui 
{
class ImageInfo;

/** ͼƬ��ʽ������
*/
class UILIB_API ImageDecoder
{
public:    
    /** ���ļ��м���ͼƬ������ͼƬ����
    * @param [in] imageFullPath �ļ�������·��
    */
    std::unique_ptr<ImageInfo> LoadImageFile(const std::wstring& imageFullPath);

    /** ���ڴ��ļ������м���ͼƬ������ͼƬ����
    * @param [in] file_data ͼƬ�ļ������ݣ��ڲ�������β0��д����
    * @param [in] imageFullPath �ļ�������·��
    */
	std::unique_ptr<ImageInfo> LoadImageData(std::vector<unsigned char>& file_data, const std::wstring& imageFullPath); 
};

} // namespace ui

#endif // UI_IMAGE_IMAGE_DECODER_H_
