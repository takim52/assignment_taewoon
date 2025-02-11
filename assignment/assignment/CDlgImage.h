#pragma once
#include "afxdialogex.h"
#include <vector>

class CassignmentDlg;
// CDlgImage 대화 상자

class CDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImage)

public:
	CDlgImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgImage();
	void initImage();
	CImage m_image;
	void setParentDlg(CassignmentDlg* pParent);
	void resetPoints();

private:
	void drawCircle(CPoint point);
	void drawCircleFromPoints();
	bool calculateCircleFromPoints(CPoint& center, int& radius);
	void drawEdgeCircle(CPoint center, int radius);

	static const int m_nWidth = 800;
	static const int m_nHeight = 600;
	static const int m_nBpp = 8;
	static const unsigned short m_grayScale = 0;
	static const int MAX_POINTS = 3;

	CassignmentDlg* m_pParentDlg;
	std::vector<CPoint> m_points;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
