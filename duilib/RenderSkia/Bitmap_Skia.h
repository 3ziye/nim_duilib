#ifndef UI_RENDER_SKIA_BITMAP_H_
#define UI_RENDER_SKIA_BITMAP_H_

#include "duilib/Render/IRender.h"

//Skia相关类的前置声明
class SkBitmap;

namespace ui
{
/** 位图的实现：Skia绘制引擎
*/
class UILIB_API Bitmap_Skia: public IBitmap
{
public:
    Bitmap_Skia();
    virtual ~Bitmap_Skia() override;

public:
    /** 从数据初始化（ARGB格式）
    @param [in] nWidth 宽度
    @param [in] nHeight 高度
    @param [in] pPixelBits 位图数据, 如果为nullptr表示窗口空位图，如果不为nullptr，其数据长度为：nHeight * nWidth * 4
    @param [in] fImageSizeScale 图片的缩放比例，1.0f表示原值
    @param [in] alphaType 位图的Alpha类型，只有Skia引擎需要此参数
    */
    virtual bool Init(uint32_t nWidth, uint32_t nHeight,
                      const void* pPixelBits, float fImageSizeScale = 1.0f,
                      BitmapAlphaType alphaType = BitmapAlphaType::kPremul_SkAlphaType) override;

    /** 获取图片宽度
    */
    virtual uint32_t GetWidth() const override;

    /** 获取图片高度
    */
    virtual uint32_t GetHeight() const override;

    /** 获取图片大小
    @return 图片大小
    */
    virtual UiSize GetSize() const override;

    /** 锁定位图数据，数据长度 = GetWidth() * GetHeight() * 4
    */
    virtual void* LockPixelBits() override;

    /** 释放位图数据
    */
    virtual void UnLockPixelBits() override;

    /** 克隆生成新的的位图
    *@return 返回新生成的位图接口，由调用方释放资源
    */
    virtual IBitmap* Clone() override;

public:
    /** 获取Skia 位图
    */
    const SkBitmap& GetSkBitmap() const;

private:
    /** 更新图片的透明通道标志
    */
    void UpdateAlphaFlag(uint8_t* pPixelBits);

    /** 执行数据初始化
    */
    bool InitImage(uint32_t nWidth, uint32_t nHeight, const void* pPixelBits, BitmapAlphaType alphaType);

private:
    /** Skia 位图
    */
    std::unique_ptr<SkBitmap> m_pSkBitmap;
};

} // namespace ui

#endif // UI_RENDER_SKIA_BITMAP_H_
