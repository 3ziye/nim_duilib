#ifndef UI_CORE_PLACE_HOLDER_H_
#define UI_CORE_PLACE_HOLDER_H_

#pragma once

#include "base/callback/callback.h"
#include "duilib/Core/UiColor.h"
#include "duilib/Core/UiFont.h"
#include "duilib/Core/UiRect.h"
#include "duilib/Core/UiSize.h"
#include "duilib/Core/UiPoint.h"
#include <string>
#include <memory>

namespace ui 
{
	class Box;
	class Window;

class UILIB_API PlaceHolder : public virtual nbase::SupportWeakCallback
{
public:
	PlaceHolder();
	PlaceHolder(const PlaceHolder& r) = delete;
	PlaceHolder& operator=(const PlaceHolder& r) = delete;
	virtual ~PlaceHolder();

	/**@brief ��ȡ������ָ��
	 */
	Box* GetParent() const { return m_pParent;}

	/**@brief �������ƻ�ȡ��������ָ��
	 * @param[in] strName Ҫ��ȡ��������������
	 */
	Box* GetAncestor(const std::wstring& strName);

	/**@brief ��ȡ�ؼ����ƣ���Ӧ xml �� name ����
	 */
	const std::wstring& GetName() const { return m_sName; }

	/**@brief ��ȡ�ؼ����ƣ���Ӧ xml �� name ����
	 * @return ���ؿؼ����ƣ�UTF8 ���룩
	 */
	std::string GetUTF8Name() const;

	/**@brief ���ÿؼ����ƣ��ڴ������ò���д�� xml ��
	 * @param[in] strName Ҫ���õ�����
	 */
	void SetName(const std::wstring& strName);

	/**@brief ���ÿؼ����ƣ��ڴ������ò���д�� xml �У�UTF8 ���룩
	 * @param[in] strName Ҫ���õ�����
	 */
	void SetUTF8Name(const std::string& strName);

	/**@brief ��ȡ�����Ĵ���ָ��
	 * @return ���ع������ڵ�ָ��
	 */
	virtual Window* GetWindow() const { return m_pWindow; }

	/**@brief ����������������
	 * @param[in] pManager ����ָ��
	 * @param[in] pParent ������
	 * @param[in] bInit ���ú��Ƿ���� Init ��ʼ���ؼ�
	 */
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit);

	/**@brief ����������������
	 * @param[in] pManager ����ָ��
	 */
	virtual void SetWindow(Window* pManager);

	/**@brief ��ʼ������
	 */
	virtual void Init();

	/**@brief �� Init ���ã������� Init ��ͬ
	 */
	virtual void DoInit();

	/**@brief ���øÿؼ��Ƿ�ɼ�
	 */
	virtual void SetVisible(bool bVisible);

	/**@brief �ж��Ƿ�ɼ�
	 */
	virtual bool IsVisible() const { return m_bVisible; }

	/**@brief �жϿؼ��Ƿ񸡶�״̬����Ӧ xml �� float ����
	 */
	bool IsFloat() const { return m_bFloat; }

	/**@brief ���ÿؼ��Ƿ񸡶�
	 * @param[in] bFloat ����Ϊ true Ϊ������false Ϊ������
	 */
	void SetFloat(bool bFloat);

	/**@brief ��ȡ�̶���ȣ���Ӧ xml �� width ����
	 */
	int GetFixedWidth() const { return m_cxyFixed.cx; }

	/**@brief ���ÿؼ��̶����
	 * @param[in] cx Ҫ���õĿ��
	 * @param[in] bArrange �Ƿ��������У�Ĭ��Ϊ true
	 * @param[in] bNeedDpiScale ���� DPI ���ţ�Ĭ��Ϊ true
	 */
	void SetFixedWidth(int cx, bool bArrange, bool bNeedDpiScale);

	/**@brief ��ȡ�̶��߶�
	 */
	int GetFixedHeight() const { return m_cxyFixed.cy; }

	/**@brief ���ù̶��߶�
	 * @param[in] cy Ҫ���õĹ̶��߶�
	 * @param[in] bNeedDpiScale ���� DPI ���ţ�Ĭ��Ϊ true
	 */
	void SetFixedHeight(int cy, bool bNeedDpiScale);

	/**@brief ��ȡ�ؼ���С
	*/
	const UiSize& GetFixedSize() const { return m_cxyFixed; }

	/** ��ȡ��С���
	* @return ������С��ȣ� >= 0��
	*/
	int GetMinWidth() const { ASSERT(m_cxyMin.cx >= 0); return m_cxyMin.cx; }

	/**@brief ������С���
	 * @param[in] cx Ҫ���õ���С��ȣ� >= 0��
	 */
	void SetMinWidth(int cx);

	/** ��ȡ�����
	* @return ��������ȣ� >= 0��
	*/
	int GetMaxWidth() const { ASSERT(m_cxyMax.cx >=0 ); return m_cxyMax.cx; }

	/** ���������
	 * @param[in] cx Ҫ���õ�����ȣ� >= 0��
	 */
	void SetMaxWidth(int cx);

	/** ��ȡ��С�߶�
	* @return ������С�߶ȣ� >= 0��
	*/
	int GetMinHeight() const { ASSERT(m_cxyMin.cy >= 0); return m_cxyMin.cy; }

	/** ������С�߶�
	 * @param[in] cy Ҫ���õ���С�߶ȣ� >= 0��
	 */
	void SetMinHeight(int cy);

	/** ��ȡ���߶�
	* @return �������߶ȣ� >= 0��
	*/
	int GetMaxHeight() const { ASSERT(m_cxyMax.cy >= 0); return m_cxyMax.cy; }

	/** �������߶�
	 * @param[in] cy Ҫ���õ����߶ȣ� >= 0��
	 */
	void SetMaxHeight(int cy);

	/**@brief ��ȡʵ�ʿ��
	 */
	int GetWidth() const { return m_uiRect.Width(); }

	/**@brief ��ȡʵ�ʸ߶�
	 */
	int GetHeight() const { return m_uiRect.Height(); }

	/**@brief ��ȡˮƽ���뷽ʽ�� �ο� HorAlignType ö��
	 */
	HorAlignType GetHorAlignType() const { return m_horAlignType; }

	/**@brief ����ˮƽ���뷽ʽ
	 * @param[in] horAlignType Ҫ���õĶ��뷽ʽ���ο� HorAlignType ö��
	 */
	void SetHorAlignType(HorAlignType horAlignType);

	/**@brief ��ȡ��ֱ���뷽ʽ���μ� VerAlignType ö��
	 */
	VerAlignType GetVerAlignType() const { return m_verAlignType; }

	/**@brief ���ô�ֱ���뷽ʽ
	 * @param[in] vorAlignType Ҫ���õĶ��뷽ʽ���ο� VerAlignType ö��
	 */
	void SetVerAlignType(VerAlignType verAlignType);

	/**@brief �ж��Ƿ���Ҫ����������С
	 */
	bool IsReEstimateSize() const { return m_bReEstimateSize; }

	/**@brief �����Ƿ���Ҫ����������С
	 */
	void SetReEstimateSize(bool bReEstimateSize);

	/**@brief ��ȡ�ؼ����ѹ����С�����ȺͿ�ȣ����൱��EstimateSize���������Ļ���ֵ
	*/
	const UiSize& GetEstimateSize() const { return m_szEstimateSize; }

	/**@brief ���ÿؼ����ѹ����С�����ȺͿ�ȣ����൱��EstimateSize���������Ļ���ֵ
	*/
	void SetEstimateSize(const UiSize& szEstimateSize) { m_szEstimateSize = szEstimateSize; }

	/**@brief ����ؼ���С���������޾���ʵ��, �������õĹ̶���С��
	*  @param [in] szAvailable ���д�С
	 */
	virtual UiSize EstimateSize(UiSize /*szAvailable*/) { return m_cxyFixed; }

	/**@brief ��ȡ�ؼ�λ�ã�����ɸı���Ϊ��
	 * @param[in] bContainShadow �Ƿ������Ӱλ�ã�Ĭ��Ϊ true
	 */
	virtual	UiRect GetPos(bool /*bContainShadow*/) const { return m_uiRect; }

	/**@brief ���ÿؼ�λ�ã�����ɸı���Ϊ��
	 * @param[in] rc Ҫ���õ�λ����Ϣ
	 */
	virtual void SetPos(UiRect rc);

	/**@brief ��ȡ�ؼ�����(��������m_uiRectֵ)
	*/
	const UiRect& GetRect() const { return m_uiRect; }

	/**@brief ���ÿؼ�����(��������m_uiRectֵ)
	*/
	void SetRect(const UiRect& rc);

	/**@brief �ػ�ؼ�
	*/
	virtual void Invalidate();

	/**@brief ���в�������
	 */
	virtual void Arrange();

	/**@brief �ø���������
	 */
	virtual void ArrangeAncestor();

	/** ���ݿؼ����ԣ��ж���Ҫ���Ų��ֻ���ֻ���ػ�
	*/
	void RelayoutOrRedraw();

	/**@brief �ж��Ƿ��Ѿ����й�
	 */
	bool IsArranged() const { return m_bIsArranged; }

	/**@brief �����Ƿ��Ѿ����й�
	 */
	void SetArranged(bool bArranged);

	/**@brief �����Ƿ�ʹ�û���
	 */
	void SetUseCache(bool cache);

	/**@brief �ж��Ƿ�ʹ�û���
	 */
	bool IsUseCache() { return m_bUseCache; }

	/**@brief ���û������־λ
	 */
	void SetCacheDirty(bool dirty);

	/**@brief �жϻ������־λֵ
	 */
	bool IsCacheDirty() { return m_bCacheDirty; }

	/**@brief ��ȡ�ؼ�ʵ�ʵ�λ�ã�����λ�ü����������ƫ��λ�ã�
	 * @param[in] bContainShadow �Ƿ��Ǵ�����Ӱ�ߴ�
	 */
	UiRect GetPosWithScrollOffset(bool bContainShadow) const;

	/** @brief ��ȡ������ƫ��
	 */
	UiPoint GetScrollOffset() const;

	/** @brief �ж������ؼ��Ƿ���ڸ���/�����ϵ
	 */
	bool IsChild(PlaceHolder* pAncestor, PlaceHolder* pChild) const;

protected:
	/** @brief ���Լ�����
	 * @return void ��
	 */
	virtual void ArrangeSelf();

private:
	//�ؼ����ƣ����ڲ��ҿؼ��Ȳ���
	std::wstring m_sName;

	//�����Ĵ��ڶ���
	Window* m_pWindow;

	//���ؼ�����
	Box* m_pParent;

	//�ؼ�λ�����С
	UiRect m_uiRect;

	//�ؼ��Ĵ�С
	UiSize m_cxyFixed;

	//�ؼ���С��Сֵ
	UiSize m_cxyMin;

	//�ؼ���С���ֵ
	UiSize m_cxyMax;

	//�ؼ�ˮƽ���뷽ʽ
	HorAlignType m_horAlignType;

	//�ؼ���ֱ���뷽ʽ
	VerAlignType m_verAlignType;

	//�ؼ��Ƿ�Ϊ��������
	bool m_bFloat;

	//�Ƿ���Ҫ����������С
	bool m_bReEstimateSize;

	//�ؼ����ѹ����С�����ȺͿ�ȣ����൱�ڹ����Ļ���ֵ
	UiSize m_szEstimateSize;

	//�Ƿ���Ҫ��������
	bool m_bIsArranged;

	//�Ƿ�ʹ�û��ƻ���
	// ���Ϊtrue��ÿ���ؼ��Լ�����һ�ݻ��ƻ��棬��ռ�ý϶��ڴ棬�����ϻ������������ܣ���ʵ��δ���Գ�Ч����
	// ���Ϊfalse����ʾ�޻��ƻ��棬�ڴ�ռ�ñȽ��١�
	// TODO: ���ģʽ���ڴ�ռ���ʺܸߣ��Ի����������������ԣ�δ�����ܻ�ɾ��������߼����Լ򻯴��롣
	bool m_bUseCache;

	//�����Ƿ�������־ֵ
	bool m_bCacheDirty;

	//�Ƿ�ɼ�
	bool m_bVisible;
};

} // namespace ui

#endif // UI_CORE_PLACE_HOLDER_H_
