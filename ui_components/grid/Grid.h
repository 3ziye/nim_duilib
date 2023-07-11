#pragma once
#include "ui_components/grid/GridBase.h"
#include "duilib/Core/ScrollBox.h"

namespace ui
{
	class GridBody;
	class UILIB_API Grid : public ScrollBox
	{
		friend class GridBody;
	public:
		Grid();
		virtual ~Grid();
		virtual GridBody* CreateGridBody();		//�̳�����Ҫ��������ӿ� ��ʵ���Զ���GridBody
		virtual void Init() override;
	public:

		
		
	public: //pass to gridbody

		/**
		* @brief Ĭ���и�
		*/
		int GetDefaultRowHeight();
		void SetDefaultRowHeight(int height);

		/**
		* @brief Ĭ���п�
		*/
		int GetDefaultColWidth();
		void SetDefaultColWidth(int width);

		/**
		* @brief ������
		*/
		int GetColCount() const;
		void SetColCount(int count);

		/**
		* @brief ������
		*/
		int GetRowCount() const;
		void SetRowCount(int count);

		/**
		* @brief �̶�����
		*/
		int GetFixedColCount() const;
		void SetFixedColCount(int fixed);

		/**
		* @brief �̶�����
		*/
		int GetFixedRowCount() const;
		void SetFixedRowCount(int fixed);

		/**
		* @brief �п�
		* @param[in] col_index: �ڼ���,base on 0
		*/
		int GetColumnWidth(int col_index) const;
		void SetColumnWidth(int col_index, int width);

		/**
		* @brief �и�;
		* @param[in] row_index: �ڼ���,base on 0;
		*/
		int GetRowHeight(int row_index) const;
		void SetRowHeight(int row_index, int height);

		/**
		* @brief ��ͷ�߶�
		*/
		int GetHeaderHeight() const;
		void SetHeaderHeight(int height);

		int GetFixedColWidth() const;
		int GetFixedRowHeight() const;

		/**
		* @brief �̶����еı���ɫ
		*/
		const std::wstring& GetFixedBkColor() const;
		void SetFixedBkColor(const std::wstring& color);

		/**
		* @brief ѡ�е�Ԫ���ǰ��ɫ
		*/
		const std::wstring& GetSelForeColor() const;
		void SetSelForeColor(const std::wstring& color);

		/**
		* @brief ��������ɫ
		*/
		const std::wstring& GetGridLineColor() const;
		void SetGridLineColor(const std::wstring& color);

		/**
		* @brief ����/��ȡ��Ԫ���ı�����;
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		*/
		std::wstring GetGridItemText(int row_index, int col_index);
		bool SetGridItemText(const std::wstring& text, int row_index, int col_index);

		/**
		* @brief �жϵ�Ԫ���Ƿ�fixed;
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		*/
		bool IsGridItemFixed(int row_index, int col_index);

		/**
		* @brief �������ڱ�����;
		* @param[in] text: ��ͷ�ı�;
		* @param[in] width: �п��;
		* @return �����¼ӵı�ͷ��Ԫ�����ָ��,null��ʾδ�ɹ�;
		*/
		GridHeaderItem* AddCol(const std::wstring& text, int width = 80);

		/**
		* @brief �������ڱ�����;
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool AddRow();

		/**
		* @brief ��ȡ��ͷ��Ϣ
		* @return ���ر�ͷ��Ϣ,null��ʾδ�ҵ�;
		*/
		GridHeader *GetHeader();

		/**
		* @brief ��ȡ��Ԫ�����
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		* @return ���ص�Ԫ�����ָ��,null��ʾδ�ҵ�;
		*/
		GridItem *GetGridItem(int row_index, int col_index);

		/**
		* @brief �Ƴ�һ��
		* @param[in] row_index: �ڼ���,base on 0;
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool RemoveRow(int row_index);

		/**
		* @brief �Ƴ�һ��
		* @param[in] col_index: �ڼ���,base on 0;
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool RemoveCol(int col_index);

		/**
		* @brief �������
		* @param[in] include_header: true��ʾ���header, false��ʾ����header;
		*/
		void Clear(bool include_header);

		/**
		* @brief ������ݱ����Ļص�ע��
		* @param[in] callback
		*/
		void AttachTextChange(const EventCallback& callback);

		/**
		* @brief �и��������Զ��������
		* @param[in] col_index:	Ҫ�����������, base on 0;
		* @param[in] min_width:	��С���;
		* @param[in] max_width: �����, -1����û������
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool AutoFixColWidth(int col_index, int min_width = 30, int max_width = -1);

		/**
		* @brief ��ȡѡ����Ϣ
		* @return ���� GridSelRange
		*/
		const GridSelRange& GetSelRange() const;
	protected:
		

	protected:
		virtual bool ButtonDown(const EventArgs& msg) override;
		virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;
	protected:
		GridBody *m_pBody = nullptr;

		/* �Ƿ���ʾ���� */
		bool m_bPaintGridLine = true;

		/* �Ƿ�ɱ༭��� */
		bool m_bEditable = true;
	private:
		/* ��ֹvirtual void Init()�ظ����� */
		bool m_bIsInit = false;		
	};

	
}