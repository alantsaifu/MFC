
// SoGameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SoGame.h"
#include "SoGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <gdiplus.h>          // ����gdiplusͷ�ļ�
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;      // ʹ��Gdiplus�����ռ䣬������������
ULONG_PTR m_pGdiToken_inDlg;

#include <time.h>
#include <math.h>

Image *bkImage;
Image *WallImage;
Image *Player1HPImage;
Image *Player2HPImage;
Image *Player1State1Image;
Image *Player2State1Image;
Image *Player1State2Image;
Image *Player2State2Image;
Image *Player1BulletImage;
Image *Player2BulletImage;

Rect ButtleRc; //�ӵ�·��

//����Դ����ͼƬ
BOOL ImageFromIDResource(UINT resourceID, LPCTSTR resourceType, Image*& image)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(resourceID),resourceType);
	if (hRsrc == NULL)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (lpRsrc == NULL)
		return FALSE;

	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

	// load from stream
	image = Gdiplus::Image::FromStream(pstm);

	// free/release stuff
	GlobalUnlock(m_hMem);
	GlobalFree(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	return TRUE;
}

// CSoGameDlg �Ի���
CSoGameDlg::CSoGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoGameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSoGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSoGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CSoGameDlg ��Ϣ�������

BOOL CSoGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//GDI+�ĳ�ʼ��
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken_inDlg, &m_gdiplusStartupInput, NULL);

	//����ͼƬ
	ImageFromIDResource(IDR_BK_PNG, L"PNG", bkImage);
	ImageFromIDResource(IDR_WALL, L"PNG", WallImage);
	ImageFromIDResource(IDR_PLAYER1_HP_PNG, L"PNG", Player1HPImage);
	ImageFromIDResource(IDR_PLAYER2_HP_PNG, L"PNG", Player2HPImage);
	ImageFromIDResource(IDR_PLAYER1_1_PNG, L"PNG", Player1State1Image);
	ImageFromIDResource(IDR_PLAYER1_2_PNG, L"PNG", Player1State2Image);
	ImageFromIDResource(IDR_PLAYER2_1_PNG, L"PNG", Player2State1Image);
	ImageFromIDResource(IDR_PLAYER2_2_PNG, L"PNG", Player2State2Image);
	ImageFromIDResource(IDR_PLAYER1_BULLET, L"PNG", Player1BulletImage);
	ImageFromIDResource(IDR_PLAYER2_BULLET, L"PNG", Player2BulletImage);

	//��ʼ������״̬
	Player1StateOld = IDR_PLAYER1_1_PNG;
	Player2StateOld = IDR_PLAYER2_1_PNG;
	Player1StateCur = Player1StateOld;
	Player2StateCur = Player2StateOld;
	//�ӵ��Ƿ���
	ButtleSpeedFlag = FALSE;
	ButtleFireFlag = FALSE;
	ButtleFlag = FALSE;
	
	WindSpeed = 0;
	WindSpeedTmp = 0; //����
	GameGravity = 2;  //����

	//��ʼ��Ѫ��
	PlayerHP[0] = 250;
	PlayerHP[1] = 250;

	SetTimer(1, 100, NULL);  //�ж�����״̬�Ƿ�ı�
	SetTimer(2, 200, NULL);  //�ӵ��ٶ�
	SetTimer(3, 50, NULL);  //�ӵ�λ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSoGameDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	RectF imageRc;
	Graphics g(dc.m_hDC);
	g.SetInterpolationMode(InterpolationModeNearestNeighbor);
	g.SetPixelOffsetMode(PixelOffsetModeHalf);
	//����
	imageRc.X = 0;
	imageRc.Y = 0;
	imageRc.Width = (IN REAL)bkImage->GetWidth();
	imageRc.Height = (IN REAL)bkImage->GetHeight();
	g.DrawImage(bkImage, imageRc, 0, 0, (IN REAL)bkImage->GetWidth(), (IN REAL)bkImage->GetHeight(), UnitPixel);
	//ǽ��
	imageRc.X = 280;
	imageRc.Y = 290;
	imageRc.Width = (IN REAL)WallImage->GetWidth()/2;
	imageRc.Height = (IN REAL)WallImage->GetHeight()/2;
	g.DrawImage(WallImage, imageRc, 0, 0, (IN REAL)WallImage->GetWidth(), (IN REAL)WallImage->GetHeight(), UnitPixel);
	//���һѪ��
	imageRc.X = 30+(250-(IN REAL)PlayerHP[0]);
	imageRc.Y = 30;
	imageRc.Width = (IN REAL)PlayerHP[0];
	imageRc.Height = 10;
	g.DrawImage(Player1HPImage, imageRc, 0, 0, (IN REAL)Player1HPImage->GetWidth(), (IN REAL)Player1HPImage->GetHeight(), UnitPixel);
	//��Ҷ�Ѫ��
	imageRc.X = 352;
	imageRc.Y = 30;
	imageRc.Width = (IN REAL)PlayerHP[1];
	imageRc.Height = 10;
	g.DrawImage(Player2HPImage, imageRc, 0, 0, (IN REAL)Player2HPImage->GetWidth(), (IN REAL)Player2HPImage->GetHeight(), UnitPixel);
	//���һ״̬
	imageRc.X = 30;
	imageRc.Y = 320;
	imageRc.Width = (IN REAL)Player1State1Image->GetWidth();
	imageRc.Height = (IN REAL)Player1State1Image->GetHeight();
	if (IDR_PLAYER1_1_PNG == Player1StateCur)
		g.DrawImage(Player1State1Image, imageRc, 0, 0, (IN REAL)Player1State1Image->GetWidth(), (IN REAL)Player1State1Image->GetHeight(), UnitPixel);
	else
		if (FALSE == ButtleFlag)
			g.DrawImage(Player1State2Image, imageRc, 0, 0, (IN REAL)Player1State1Image->GetWidth(), (IN REAL)Player1State1Image->GetHeight(), UnitPixel);
		else
			g.DrawImage(Player1State1Image, imageRc, 0, 0, (IN REAL)Player1State1Image->GetWidth(), (IN REAL)Player1State1Image->GetHeight(), UnitPixel);
	//��Ҷ�״̬
	imageRc.X = 480;
	imageRc.Y = 320;
	imageRc.Width = (IN REAL)Player2State1Image->GetWidth();
	imageRc.Height = (IN REAL)Player2State1Image->GetHeight();
	if (IDR_PLAYER2_1_PNG == Player2StateCur)
		g.DrawImage(Player2State1Image, imageRc, 0, 0, (IN REAL)Player2State1Image->GetWidth(), (IN REAL)Player2State1Image->GetHeight(), UnitPixel);
	else
		if (TRUE == ButtleFlag)
			g.DrawImage(Player2State2Image, imageRc, 0, 0, (IN REAL)Player2State1Image->GetWidth(), (IN REAL)Player2State1Image->GetHeight(), UnitPixel);
		else
			g.DrawImage(Player2State1Image, imageRc, 0, 0, (IN REAL)Player2State1Image->GetWidth(), (IN REAL)Player2State1Image->GetHeight(), UnitPixel);
	//�ӵ�����
	if (TRUE == ButtleFireFlag)
	{
		if (FALSE == ButtleFlag)
		{
			imageRc.X = (IN REAL)ButtleRc.X;
			imageRc.Y = (IN REAL)ButtleRc.Y;
			imageRc.Width = (IN REAL)Player1BulletImage->GetWidth();
			imageRc.Height = (IN REAL)Player1BulletImage->GetHeight();
			g.DrawImage(Player1BulletImage, imageRc, 0, 0, (IN REAL)Player1BulletImage->GetWidth(), (IN REAL)Player1BulletImage->GetHeight(), UnitPixel);
		}
		else
		{
			imageRc.X = (IN REAL)ButtleRc.X;
			imageRc.Y = (IN REAL)ButtleRc.Y;
			imageRc.Width = (IN REAL)Player2BulletImage->GetWidth();
			imageRc.Height = (IN REAL)Player2BulletImage->GetHeight();
			g.DrawImage(Player2BulletImage, imageRc, 0, 0, (IN REAL)Player2BulletImage->GetWidth(), (IN REAL)Player2BulletImage->GetHeight(), UnitPixel);
		}
	}
	CFont NormalFont;
	NormalFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, TRUE, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����");

	CFont *OldFont;
	OldFont = dc.SelectObject(&NormalFont);

	dc.SelectObject(&NormalFont);
	dc.SetBkMode(TRANSPARENT);

	CRect TextRect;
	CString NumTmp;

	TextRect.top = 50;
	TextRect.left = 240;
	TextRect.right = 380;
	TextRect.bottom = 80;
	DrawText(dc.m_hDC, L"������Է�Ϊ��", -1, TextRect, DT_LEFT);

	TextRect.top = 80;
	TextRect.left = 300;
	TextRect.right = 330;
	TextRect.bottom = 100;
	NumTmp.Format(L"%d", WindSpeedTmp);
	DrawText(dc.m_hDC, NumTmp, -1, TextRect, DT_LEFT);

	if (TRUE == ButtleSpeedFlag)
	{
		if (FALSE == ButtleFlag)
		{
			TextRect.top = 280;
			TextRect.left = 75;
			TextRect.right = 115;
			TextRect.bottom = 320;
			NumTmp.Format(L"%d", (int)ButtleSpeedH);
			DrawText(dc.m_hDC, NumTmp, -1, TextRect, DT_CENTER | DT_VCENTER);
		}
		else
		{
			TextRect.top = 280;
			TextRect.left = 525;
			TextRect.right = 565;
			TextRect.bottom = 320;
			NumTmp.Format(L"%d", (int)ButtleSpeedH);
			DrawText(dc.m_hDC, NumTmp, -1, TextRect, DT_CENTER | DT_VCENTER);
		}
	}

	CDialogEx::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSoGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSoGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
		if (Player1StateCur != Player1StateOld)
		{
			Player1StateOld = Player1StateCur;
			if (FALSE == ButtleFlag)
			{
				InvalidateRect(CRect(30,320,160,450));
			}
		}
		if (Player2StateCur != Player2StateOld)
		{
			Player2StateOld = Player2StateCur;
			if (TRUE == ButtleFlag)
			{
				InvalidateRect(CRect(480,320,610,450));
			}
		}
		break;
	case 2:
		if (TRUE == ButtleSpeedFlag)
		{
			if ((IDR_PLAYER1_2_PNG==Player1StateCur&&FALSE==ButtleFlag) || (IDR_PLAYER2_2_PNG==Player2StateCur&&TRUE==ButtleFlag))
			{
				ButtleSpeedH++; //�ӵ��ٶ�����

				if (FALSE==ButtleFlag)
				{
					InvalidateRect(CRect(75,280,115,300));
				}
				else
				{
					InvalidateRect(CRect(525,280,565,300));
				}
			}
		}
		break;
	case 3:
		if (TRUE == ButtleFireFlag)
		{
			ButtleFireTime++; //�ӵ�����ʱ��

			if (FALSE==ButtleFlag)  //����״̬Ϊ��Ҷ�,�ӵ�Ϊ���һ,���෴��
			{
				ButtleRc.X = ButtleX-ButtleSpeedH*ButtleFireTime;
			}
			else
			{
				ButtleRc.X = ButtleX+ButtleSpeedH*ButtleFireTime;
			}
			ButtleRc.Y = ButtleY-(ButtleSpeedV*ButtleFireTime-0.5*GameGravity*ButtleFireTime*ButtleFireTime);

			InvalidateRect(CRect(ButtleRc.X,ButtleRc.Y,ButtleRc.X+40,ButtleRc.Y+40));

			ButtleEdge(ButtleRc.X, ButtleRc.Y);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CSoGameDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ((point.x>=30 && point.x<=160)&&(point.y>=320 && point.y<=450))
	{
		Player1StateCur = IDR_PLAYER1_2_PNG;
		Player2StateCur = IDR_PLAYER2_1_PNG;
	}
	else if ((point.x>=480 && point.x<=610)&&(point.y>=320 && point.y<=450))
	{
		Player1StateCur = IDR_PLAYER1_1_PNG;
		Player2StateCur = IDR_PLAYER2_2_PNG;
	}
	else
	{
		Player1StateCur = IDR_PLAYER1_1_PNG;
		Player2StateCur = IDR_PLAYER2_1_PNG;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CSoGameDlg::BulletPathWay()
{
	if (IDR_PLAYER1_2_PNG==Player1StateCur && FALSE==ButtleFlag)
	{
		InvalidateRect(CRect(30,320,160,450));
		ButtleFlag = !ButtleFlag;
		ButtleX = 95;
		ButtleY = 280;
	}
	else if (IDR_PLAYER2_2_PNG==Player2StateCur && TRUE==ButtleFlag)
	{
		InvalidateRect(CRect(480,320,610,450));
		ButtleFlag = !ButtleFlag;
		ButtleX = 505;
		ButtleY = 280;
	}
	else
	{
		return;
	}

	ButtleFireFlag = TRUE;
	ButtleSpeedV = 3*8;
	ButtleSpeedH = ButtleSpeedH + WindSpeed;
	ButtleSpeedH = 2*ButtleSpeedH;

}

void CSoGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDR_PLAYER1_2_PNG==Player1StateCur || IDR_PLAYER2_2_PNG==Player2StateCur)
	{
		ButtleSpeedFlag = TRUE;
		ButtleSpeedH = 0;
		ButtleFireTime = 0;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSoGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDR_PLAYER1_2_PNG==Player1StateCur || IDR_PLAYER2_2_PNG==Player2StateCur)
	{
		if (TRUE == ButtleSpeedFlag)
		{
			ButtleSpeedFlag = FALSE;
			BulletPathWay();

			srand((unsigned) time(NULL));//�õ�ǰʱ�䣬�������� 
			WindSpeedTmp = rand()%10; //���������
			WindSpeedTmp -= 10;
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CSoGameDlg::ButtleEdge(int x, int y)
{
	if (TRUE == ButtleFlag)   //����״̬Ϊ��Ҷ�,�ӵ�Ϊ���һ,���෴��
	{
		if ((x>300 && x<330) && y>256)  //ǽ��
		{
			ButtleFireFlag = FALSE;
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();
		}
		if (x>633)  //���ұ߽�
		{
			ButtleFireFlag = FALSE;
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();
		}
		if (y<0 || y>457) //�����±߽�
		{
			ButtleFireFlag = FALSE;
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();
		}
		if (x>480 && y>320)  //��Ҷ�
		{
			ButtleFireFlag = FALSE;
			PlayerHP[1] -= (70-abs(x-545));
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();

			if (0 >= PlayerHP[1])
			{
				MessageBox(L"���һӮ��~", L"��ϲ", MB_OK);

				//��ʼ������״̬
				Player1StateOld = IDR_PLAYER1_1_PNG;
				Player2StateOld = IDR_PLAYER2_1_PNG;
				Player1StateCur = Player1StateOld;
				Player2StateCur = Player2StateOld;
				//�ӵ��Ƿ���
				ButtleSpeedFlag = FALSE;
				ButtleFireFlag = FALSE;
				ButtleFlag = FALSE;

				WindSpeed = 0;
				WindSpeedTmp = 0; //����
				GameGravity = 2;  //����

				//��ʼ��Ѫ��
				PlayerHP[0] = 250;
				PlayerHP[1] = 250;

				Invalidate();
			}
		}
	}
	else
	{
		if ((x>300 && x<330) && y>256)  //ǽ��
		{
			ButtleFireFlag = FALSE;
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();
		}
		if (x<0)  //����߽�
		{
			ButtleFireFlag = FALSE;
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();
		}
		if (y<0 || y>457) //�����±߽�
		{
			ButtleFireFlag = FALSE;
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();
		}
		if (x<160 && y>320)  //���һ
		{
			ButtleFireFlag = FALSE;
			PlayerHP[0] -= (70-abs(x-95));
			WindSpeed = WindSpeedTmp;
			Sleep(500);
			Invalidate();

			if (0 >= PlayerHP[0])
			{
				MessageBox(L"��Ҷ�Ӯ��~", L"��ϲ", MB_OK);
				
				//��ʼ������״̬
				Player1StateOld = IDR_PLAYER1_1_PNG;
				Player2StateOld = IDR_PLAYER2_1_PNG;
				Player1StateCur = Player1StateOld;
				Player2StateCur = Player2StateOld;
				//�ӵ��Ƿ���
				ButtleSpeedFlag = FALSE;
				ButtleFireFlag = FALSE;
				ButtleFlag = FALSE;

				WindSpeed = 0;
				WindSpeedTmp = 0; //����
				GameGravity = 2;  //����

				//��ʼ��Ѫ��
				PlayerHP[0] = 250;
				PlayerHP[1] = 250;

				Invalidate();
			}
		}
	}
}
