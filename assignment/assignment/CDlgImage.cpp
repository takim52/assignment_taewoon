// CDlgImage.cpp: 구현 파일
//

#include "pch.h"
#include "assignment.h"
#include "afxdialogex.h"
#include "CDlgImage.h"
#include "assignmentDlg.h"
#include <cmath>


// CDlgImage 대화 상자

IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgImage, pParent)
{

}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgImage 메시지 처리기


BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, m_nWidth, m_nHeight);

	initImage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgImage::initImage()
{
	if (!m_image.IsNull()) {
		m_image.Destroy();
	}
	m_image.Create(m_nWidth, -m_nHeight, m_nBpp);

	if (m_nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; ++i) {
			rgb[i].rgbBlue = rgb[i].rgbGreen = rgb[i].rgbRed = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	unsigned char* pImageBits = (unsigned char*)m_image.GetBits();
	memset(pImageBits, 0xff, m_nWidth * m_nHeight);
}


void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	if (m_image) {
		m_image.Draw(dc, 0, 0);
	}
}


void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_points.size() < MAX_POINTS) {
		m_points.push_back(point);
		drawCircle(point);
	}	

	if (m_points.size() == MAX_POINTS) {
		drawCircleFromPoints();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgImage::drawCircle(CPoint point) {
	int radius = m_pParentDlg->m_nRadius;
	int centerX = point.x;
	int centerY = point.y;
	int rectStartX = centerX - radius;
	int rectStartY = centerY - radius;

	unsigned char* pImageBits = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();

	for (int i = -radius; i <= radius; ++i) {
		for (int j = -radius; j <= radius; ++j) {
			if (i * i + j * j <= radius * radius) {
				int targetX = centerX + j;
				int targetY = centerY + i;

				if ((0 <= targetX && targetX < m_nWidth) && (0 <= targetY && targetY < m_nHeight)) {
					pImageBits[targetY * nPitch + targetX] = 0;
				}
			}
		}
	}

	Invalidate();
}

void CDlgImage::setParentDlg(CassignmentDlg* pParent) {
	m_pParentDlg = pParent;
}

// 세 점으로 원의 중심과 반지름을 계산하는 함수
bool CDlgImage::calculateCircleFromPoints(CPoint& center, int& radius) {
	int x1 = m_points[0].x;
	int y1 = m_points[0].y;
	int x2 = m_points[1].x;
	int y2 = m_points[1].y;
	int x3 = m_points[2].x;
	int y3 = m_points[2].y;

	// 참고 : https://minstudyroom.tistory.com/4

	// 세 점이 일직선 상에 있는지 확인
	if ((y2 - y1) * (x3 - x2) == (y3 - y2) * (x2 - x1)) {
		return false; // 세 점이 일직선 상에 있어 원을 만들 수 없음
	}

	// 세 점의 중점
	int midX1 = (x1 + x2) / 2;
	int midY1 = (y1 + y2) / 2;
	int midX2 = (x2 + x3) / 2;
	int midY2 = (y2 + y3) / 2;

	// 중점에서 수직 이등분선의 기울기 계산
	double slope1 = -(double)(x2 - x1) / (y2 - y1);
	double slope2 = -(double)(x3 - x2) / (y3 - y2);

	// 수직 이등분선의 교점을 계산하여 원의 중심을 구함
	center.x = (int)((slope1 * midX1 - slope2 * midX2 + midY2 - midY1) / (slope1 - slope2));
	center.y = (int)(midY1 + slope1 * (center.x - midX1));

	// 원의 반지름을 계산
	radius = (int)std::sqrt((center.x - x1) * (center.x - x1) + (center.y - y1) * (center.y - y1));

	return true;
}

// 세 점으로 원을 그리는 함수
void CDlgImage::drawCircleFromPoints() {
	if (m_points.size() == MAX_POINTS) {
		CPoint center;
		int radius;
		if (calculateCircleFromPoints(center, radius)) {
			drawEdgeCircle(center, radius);
		}
	}
}

void CDlgImage::drawEdgeCircle(CPoint center, int radius) {
	int edgeSize = 5;
	int rectStartX = center.x - radius;
	int rectStartY = center.y - radius;

	unsigned char* pImageBits = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();

	for (int i = -radius; i <= radius; ++i) {
		for (int j = -radius; j <= radius; ++j) {
			int distance = i * i + j * j;
			if (distance <= radius * radius && distance >= (radius - edgeSize) * (radius - edgeSize)) {
				int targetX = center.x + j;
				int targetY = center.y + i;

				if ((0 <= targetX && targetX < m_nWidth) && (0 <= targetY && targetY < m_nHeight)) {
					pImageBits[targetY * nPitch + targetX] = 0;
				}
			}
		}
	}

	Invalidate();
}

void CDlgImage::resetPoints() {
	initImage();
	m_points.clear();
	Invalidate();
}
