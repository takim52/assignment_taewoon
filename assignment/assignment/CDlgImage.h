﻿#pragma once
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
	bool randomMoving();

private:
	void drawCircle(CPoint point);
	void drawCircleFromPoints();
	bool calculateCircleFromPoints();
	bool calculateCircleFromPoints(CPoint& center, int& radius);
	void drawEdgeCircle(CPoint center, int radius);
	void randomMovingThread();

	static const int m_nWidth = 800;
	static const int m_nHeight = 600;
	static const int m_nBpp = 8;
	static const unsigned short m_grayScale = 0;
	static const int MAX_POINTS = 3;
	static const int MAX_RANDON_MOVING_COUNT = 10;
	static const int m_nRandomMovingThreadSleep = 500;

	CassignmentDlg* m_pParentDlg;
	std::vector<CPoint> m_points;
	bool m_bDragging = false;
	int m_nSelectedPointIndex;

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
