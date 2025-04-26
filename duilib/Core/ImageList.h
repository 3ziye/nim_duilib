#ifndef UI_CORE_IMAGE_LIST_H_
#define UI_CORE_IMAGE_LIST_H_

#include "duilib/Image/Image.h"
#include "duilib/Core/UiTypes.h"
#include <map>
#include <string>
#include <vector>

namespace ui 
{
/** 图片资源的智能指针
*/
typedef std::shared_ptr<Image> ImagePtr;

/** Image列表
 */
class UILIB_API ImageList
{
public:
    ImageList();
    ~ImageList();
    ImageList(const ImageList&) = delete;
    ImageList& operator = (const ImageList&) = delete;

public:
    /** 设置图片大小（可选设置，如果不设置，则从每个图片获取）
    * @param [in] imageSize 图片大小, 内部不进行DPI缩放，需要由外部进行DPI缩放
    * @param [in] dpi DPI缩放管理器
    * @param [in] bNeedDpiScale 是否支持DPI缩放
    */
    void SetImageSize(UiSize imageSize, const DpiManager& dpi, bool bNeedDpiScale);

    /** 获取图片大小
    * @return 返回设置的图片大小，如果未设置，则返回(0,0)
    */
    UiSize GetImageSize() const;

    /** 设置当DPI变化时，图片大小是否需要DPI缩放，默认开启
    * @param [in] bEnableChangeImageSizeDpi true表示开启功能，false表示关闭功能
    */
    void EnableChangeImageSizeDpi(bool bEnableChangeImageSizeDpi);

    /** 当DPI变化时，图片大小是否需要DPI缩放，默认开启
    * @param [in] bEnableChangeImageSizeDpi true表示开启功能，false表示关闭功能
    */
    bool IsEnableChangeImageSizeDpi() const;

    /** DPI发生变化，更新图片大小
    * @param [in] nOldDpiScale 旧的DPI缩放百分比
    * @param [in] dpi DPI缩放管理器
    */
    void ChangeDpiScale(const DpiManager& dpi, uint32_t nOldDpiScale);

    /** 添加一个图片资源
    * @param [in] imageString 图片资源字符串，格式同：Image::SetImageString函数
    * @param [in] dpi DPI缩放管理接口
    * @return 返回图片资源的ID，如果失败返回-1，否则返回大于等于0的ID
    */
    int32_t AddImageString(const DString& imageString, const DpiManager& dpi);

    /** 获取图片资源字符串
    * @param [in] imageId 图片资源的ID，由AddImage函数返回的值
    * @return 如果没有关联资源，返回空串
    */
    DString GetImageString(int32_t imageId) const;

    /** 获取图片资源接口(共享资源)
    * @param [in] imageId 图片资源的ID，由AddImage函数返回的值
    * @return 如果没有关联资源，返回nullptr
    */
    ImagePtr GetImageData(int32_t imageId) const;

    /** 获取图片资源的ID
    * @param [in] imageString 图片资源字符串，格式同：Image::SetImageString函数
    * @return 返回关联图片资源的ID，如果没有关联资源，返回-1
    */
    int32_t GetImageStringId(const DString& imageString) const;

    /** 移除一个图片资源
    * @param [in] imageId 图片资源的ID，由AddImage函数返回的值
    */
    bool RemoveImageString(int32_t imageId);

    /** 获取图片资源个数
    */
    size_t GetImageCount() const;

    /** 获取所有图片资源的ID
    */
    void GetImageIdList(std::vector<int32_t>& imageIdList) const;

    /** 清空所有图片资源
    */
    void Clear();
 
private:
    /** 下一个ID
    */
    int32_t m_nNextID;

    /** 图片大小（可选设置，如果不设置，则从每个图片获取）
    */
    UiSize m_imageSize;

    /** 当DPI变化时，图片大小是否需要DPI缩放，默认开启
    */
    bool m_bEnableChangeImageSizeDpi;

    /** 图片资源映射表
    */
    std::map<int32_t, ImagePtr> m_imageMap;
};

typedef std::shared_ptr<ImageList> ImageListPtr;

} //namespace ui 

#endif //UI_CORE_IMAGE_LIST_H_
