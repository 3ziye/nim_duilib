#ifndef UI_IMAGE_IMAGE_ATTRIBUTE_H_
#define UI_IMAGE_IMAGE_ATTRIBUTE_H_

#include "duilib/Core/UiTypes.h"

namespace ui 
{
class DpiManager;

/** 图片属性
*/
class UILIB_API ImageAttribute
{
public:
    ImageAttribute();
    ~ImageAttribute();
    ImageAttribute(const ImageAttribute&);
    ImageAttribute& operator=(const ImageAttribute&);

    /** 对数据成员进行初始化
    */
    void Init();

    /** 根据图片参数进行初始化(先调用Init初始化成员变量，再按照传入参数进行更新部分属性)
    * @param [in] strImageString 图片参数字符串
    * @param [in] dpi DPI缩放接口
    */
    void InitByImageString(const DString& strImageString, const DpiManager& dpi);

    /** 根据图片参数修改属性值（仅更新新设置的图片属性, 未包含的属性不进行更新）
    * @param [in] strImageString 图片参数字符串
    * @param [in] dpi DPI缩放接口
    */
    void ModifyAttribute(const DString& strImageString, const DpiManager& dpi);

public:
    /** 判断rcDest区域是否含有有效值
    * @param [in] rcDest 需要判断的区域
    */
    static bool HasValidImageRect(const UiRect& rcDest);

    /** 对图片的源区域、目标区域、圆角大小进行校验修正和DPI自适应
    * @param [in] imageWidth 图片的宽度
    * @param [in] imageHeight 图片的高度
    * @param [in] dpi DPI缩放接口
    * @param [in] bImageDpiScaled 图片是否做过DPI自适应操作
    * @param [out] rcDestCorners 绘制目标区域的圆角信息，传出参数，内部根据rcImageCorners来设置，然后传出
    * @param [in/out] rcSource 图片区域
    * @param [in/out] rcSourceCorners 图片区域的圆角信息
    */
    static void ScaleImageRect(uint32_t imageWidth, uint32_t imageHeight, 
                               const DpiManager& dpi, bool bImageDpiScaled,
                               UiRect& rcDestCorners,
                               UiRect& rcSource, UiRect& rcSourceCorners);

public:
    /** 获取rcSource(未进行DPI缩放)
    */
    UiRect GetImageSourceRect() const;

    /** 获取rcCorner(未进行DPI缩放)
    */
    UiRect GetImageCorner() const;

    /** 获取rcDest(按配置决定是否进行DPI缩放)
    */
    UiRect GetImageDestRect(const DpiManager& dpi) const;

    /** 获取图片属性的内边距
    * @param [in] dpi DPI缩放管理器
    * @return 返回按照传入DPI缩放管理器适应的内边距数据
    */
    UiPadding GetImagePadding(const DpiManager& dpi) const;

    /** 设置图片属性的内边距(内部不做DPI自适应)
    * @param [in] newPadding 需要设置的内边距
    * @param [in] bNeedDpiScale 是否需要对newPadding进行DPI缩放
    * @param [in] dpi 与newPadding数据关联的DPI管理器
    */
    void SetImagePadding(const UiPadding& newPadding, bool bNeedDpiScale, const DpiManager& dpi);

public:
    //图片文件属性字符串
    UiString sImageString;

    //图片文件文件名，含相对路径，不包含属性
    UiString sImagePath;

    //设置图片宽度，可以放大或缩小图像：pixels或者百分比%，比如300，或者30%
    UiString srcWidth;

    //设置图片高度，可以放大或缩小图像：pixels或者百分比%，比如200，或者30%
    UiString srcHeight;

    //rcSource的DPI自适应属性（仅当bHasSrcDpiScale为true时有效）
    bool srcDpiScale;

    //加载图片时，是否设置了DPI自适应属性（"dpi_scale"）
    bool bHasSrcDpiScale;

    //rcDest属性的DPI自适应属性（仅当bHasDestDpiScale时有效）
    bool destDpiScale;

    //rcDest是否设置了DPI自适应属性（"dest_scale"）
    bool bHasDestDpiScale;

    //在绘制目标区域中横向对齐方式(如果指定了rcDest值，则此选项无效)
    UiString hAlign;

    //在绘制目标区域中纵向对齐方式(如果指定了rcDest值，则此选项无效)
    UiString vAlign;

    //透明度（0 - 255）
    uint8_t bFade;

    //横向平铺
    bool bTiledX;

    //横向完全平铺，仅当bTiledX为true时有效
    bool bFullTiledX;

    //纵向平铺
    bool bTiledY;

    //纵向完全平铺，仅当bTiledY为true时有效
    bool bFullTiledY;

    //平铺时的边距（仅当bTiledX为true或者bTiledY为true时有效）
    int32_t nTiledMargin;

    //如果是GIF等动画图片，可以指定播放次数 -1 ：一直播放，缺省值。
    int32_t nPlayCount;    

    //如果是ICO文件，用于指定需要加载的ICO图片的大小
    //(ICO文件中包含很多个不同大小的图片，常见的有256，48，32，16，并且每个大小都有32位真彩、256色、16色之分）
    //目前ICO文件在加载时，只会选择一个大小的ICO图片进行加载，加载后为单张图片
    uint32_t iconSize;

    //可绘制标志：true表示允许绘制，false表示禁止绘制
    bool bPaintEnabled;

private:
    //绘制目标区域位置和大小(相对于控件区域的位置, 未进行DPI缩放)
    UiRect* rcDest;

    //在绘制目标区域中的内边距(如果指定了rcDest值，则此选项无效)
    UiPadding16* rcPadding;

    //rcPadding对应的DPI缩放百分比
    uint16_t rcPaddingScale;

    //图片源区域位置和大小(未进行DPI缩放)
    UiRect* rcSource;

    //圆角属性(未进行DPI缩放)
    UiRect* rcCorner;
};

} // namespace ui

#endif // UI_IMAGE_IMAGE_ATTRIBUTE_H_
