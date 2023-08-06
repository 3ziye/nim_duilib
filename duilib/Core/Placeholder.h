#ifndef UI_CORE_PLACE_HOLDER_H_
#define UI_CORE_PLACE_HOLDER_H_

#pragma once

#include "base/callback/callback.h"
#include "duilib/Core/UiTypes.h"
#include <string>
#include <memory>

namespace ui 
{
	class Box;
	class Window;

/** �ؼ���λ�á���С�������״�Ļ�����װ
*/
class UILIB_API PlaceHolder : public virtual nbase::SupportWeakCallback
{
public:
	PlaceHolder();
	PlaceHolder(const PlaceHolder& r) = delete;
	PlaceHolder& operator=(const PlaceHolder& r) = delete;
	virtual ~PlaceHolder();

	/** �ؼ�����
	*/
	virtual std::wstring GetType() const;

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

public:
	/** ��ȡ�ؼ����õĿ�Ⱥ͸߶ȣ���߾������ڱ߾࣬������������߾�
	*/
	const UiFixedSize& GetFixedSize() const;

	/** ��ȡ���õĿ�ȣ������ڱ߾࣬��������߾ࣩ����Ӧ xml �� width ����; ���δ���ã�Ĭ��ֵ������
	 */
	const UiFixedInt& GetFixedWidth() const;

	/** ��ȡ�̶��߶ȣ������ڱ߾࣬��������߾ࣩ����Ӧ xml �� height ����; ���δ���ã�Ĭ��ֵ������
	 */
	const UiFixedInt& GetFixedHeight() const;

	/**@brief ���ÿؼ��Ŀ��
	 * @param[in] cx Ҫ���õĿ�ȣ������ڱ߾࣬��������߾ࣩ
	 * @param[in] bArrange �Ƿ��������У�Ĭ��Ϊ true
	 * @param[in] bNeedDpiScale ���� DPI ���ţ�Ĭ��Ϊ true
	 */
	void SetFixedWidth(UiFixedInt cx, bool bArrange, bool bNeedDpiScale);

	/** ���ÿؼ��Ŀ�ȣ������ڱ߾࣬��������߾ࣩ��������ʹ�õĽӿ�
	*/
	void SetFixedWidth64(int64_t cx64);

	/**@brief ���ÿؼ��ĸ߶�
	 * @param[in] cy Ҫ���õĹ̶��߶ȣ������ڱ߾࣬��������߾ࣩ
	 * @param[in] bNeedDpiScale ���� DPI ���ţ�Ĭ��Ϊ true
	 */
	void SetFixedHeight(UiFixedInt cy, bool bNeedDpiScale);

	/** ���ÿؼ��ĸ߶ȣ������ڱ߾࣬��������߾ࣩ��������ʹ�õĽӿ�
	*/
	void SetFixedHeight64(int64_t cy64);

public:
	/** �ж��Ƿ���Ҫ����������С
	* @param [in] szAvailable ����ʱ��������δ�С
	*/
	bool IsReEstimateSize(const UiSize& szAvailable) const;

	/** �����Ƿ���Ҫ����������С
	* @param [in] bReEstimateSize �Ƿ���Ҫ���¹���
	*/
	void SetReEstimateSize(bool bReEstimateSize);

	/** ��ȡ�ؼ����ѹ����С�����ȺͿ�ȣ����൱��EstimateSize���������Ļ���ֵ
	*/
	const UiEstSize& GetEstimateSize() const;

	/**@brief ���ÿؼ����ѹ����С�����ȺͿ�ȣ����൱��EstimateSize���������Ļ���ֵ
	*@param [in] szEstimateSize ����Ľ������Ϊ���汣������
	*@param [in] szAvailable szAvailable ����ʱ��������δ�С
	*/
	void SetEstimateSize(const UiEstSize& szEstimateSize, const UiSize& szAvailable);

public:
	/** ��ȡ��С���
	* @return ������С��ȣ������ڱ߾࣬��������߾ࣩ������ֵ >= 0��
	*/
	int32_t GetMinWidth() const { ASSERT(m_cxyMin.cx >= 0); return m_cxyMin.cx; }

	/**@brief ������С���
	 * @param[in] cx Ҫ���õ���С��ȣ������ڱ߾࣬��������߾ࣩ��ֵ >= 0��
	 */
	void SetMinWidth(int32_t cx);

	/** ��ȡ�����
	* @return ��������ȣ������ڱ߾࣬��������߾ࣩ������ֵ >= 0��
	*/
	int32_t GetMaxWidth() const { ASSERT(m_cxyMax.cx >=0 ); return m_cxyMax.cx; }

	/** ���������
	 * @param[in] cx Ҫ���õ�����ȣ������ڱ߾࣬��������߾ࣩ��ֵ >= 0��
	 */
	void SetMaxWidth(int32_t cx);

	/** ��ȡ��С�߶�
	* @return ������С�߶ȣ������ڱ߾࣬��������߾ࣩ������ֵ >= 0��
	*/
	int32_t GetMinHeight() const { ASSERT(m_cxyMin.cy >= 0); return m_cxyMin.cy; }

	/** ������С�߶�
	 * @param[in] cy Ҫ���õ���С�߶ȣ������ڱ߾࣬��������߾ࣩ��ֵ >= 0��
	 */
	void SetMinHeight(int32_t cy);

	/** ��ȡ���߶�
	* @return �������߶ȣ������ڱ߾࣬��������߾ࣩ������ֵ >= 0��
	*/
	int32_t GetMaxHeight() const { ASSERT(m_cxyMax.cy >= 0); return m_cxyMax.cy; }

	/** �������߶�
	 * @param[in] cy Ҫ���õ����߶ȣ������ڱ߾࣬��������߾ࣩ��ֵ >= 0��
	 */
	void SetMaxHeight(int32_t cy);

	/** ��ȡʵ�ʿ�ȣ������ڱ߾࣬��������߾ࣩ
	 */
	int32_t GetWidth() const { return m_uiRect.Width(); }

	/** ��ȡʵ�ʸ߶ȣ������ڱ߾࣬��������߾ࣩ
	 */
	int32_t GetHeight() const { return m_uiRect.Height(); }

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

	/** ��ȡ�ؼ�����߾�
	 */
	const UiMargin& GetMargin() const;

	/** ���ÿؼ�����߾�
	 * @param[in] rcMargin �ؼ�����߾���Ϣ
	 * @param[in] bNeedDpiScale �Ƿ�����߾���� DPI ���䣬false ������ DPI
	 */
	void SetMargin(UiMargin rcMargin, bool bNeedDpiScale);

	/** ��ȡ�ؼ�λ�ã�����ɸı���Ϊ��
	* @return ���ؿؼ��ľ������򣬰����ڱ߾࣬��������߾�
	 */
	virtual	UiRect GetPos() const { return m_uiRect; }

	/** ���ÿؼ�λ�ã�����ɸı���Ϊ��
	 * @param [in] rc Ҫ���õľ���������Ϣ�������ڱ߾࣬��������߾�
	 */
	virtual void SetPos(UiRect rc);

	/** ��ȡ�ؼ���������(��������m_uiRectֵ)�������ڱ߾࣬��������߾�
	*/
	const UiRect& GetRect() const { return m_uiRect; }

	/**@brief ���ÿؼ���������(��������m_uiRectֵ)�������ڱ߾࣬��������߾�
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

	/** @brief ��ȡ������ƫ��
	 */
	UiPoint GetScrollOffsetInScrollBox() const;

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

	//�ⲿ���õĿؼ���С
	UiFixedSize m_cxyFixed;

	//����ؼ���С�Ľ��
	UiEstResult m_estResult;

	//�ؼ���С��Сֵ
	UiSize m_cxyMin;

	//�ؼ���С���ֵ
	UiSize m_cxyMax;

	//�ؼ�ˮƽ���뷽ʽ
	HorAlignType m_horAlignType;

	//�ؼ���ֱ���뷽ʽ
	VerAlignType m_verAlignType;

	//�ؼ�����߾����ԣ��ϣ��£����ұ߾ࣩ����߾���m_uiRect����Ŀռ䣬��������m_uiRect����
	UiMargin m_rcMargin;

	//�ؼ��Ƿ�Ϊ��������
	bool m_bFloat;

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
