
// PlantsVsZombiesPlusDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlantsVsZombiesPlus.h"
#include "PlantsVsZombiesPlusDlg.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <WinUser.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#pragma comment (lib, "Uxtheme.lib")
#pragma comment (lib, "dwmapi.lib")

#include <gdiplus.h>          // ����gdiplusͷ�ļ�
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;      // ʹ��Gdiplus�����ռ䣬������������
ULONG_PTR m_pGdiToken;

#define	unlimitedSunKey			 WM_USER+100
#define	unlimitedMoneyKey		 WM_USER+101
#define	noRechargeKey			 WM_USER+102
#define	unlimitedFertilizerKey	 WM_USER+103
#define	unlimitedSprayKey		 WM_USER+104
#define	unlimitedChocolateKey	 WM_USER+105 
#define	unlimitedTreeKey		 WM_USER+106
#define	bottleTransparentKey	 WM_USER+107


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

//���Ʒ�����
void DrawGlowingText(HDC hDC, LPWSTR szText, RECT &rcArea,
	DWORD dwTextFlags = DT_LEFT | DT_VCENTER | DT_SINGLELINE, int iGlowSize = 10)
{
	//��ȡ������
	HTHEME hThm = OpenThemeData(GetDesktopWindow(), L"TextStyle");
	//����DIB
	HDC hMemDC = CreateCompatibleDC(hDC);
	BITMAPINFO bmpinfo = {0};
	bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biWidth = rcArea.right - rcArea.left;
	bmpinfo.bmiHeader.biHeight = -(rcArea.bottom - rcArea.top);
	HBITMAP hBmp = CreateDIBSection(hMemDC, &bmpinfo, DIB_RGB_COLORS, 0, NULL, 0);
	if (hBmp == NULL) return;
	HGDIOBJ hBmpOld = SelectObject(hMemDC, hBmp);
	//����ѡ��
	DTTOPTS dttopts = {0};
	dttopts.dwSize = sizeof(DTTOPTS);
	dttopts.dwFlags = DTT_GLOWSIZE | DTT_COMPOSITED;
	dttopts.iGlowSize = iGlowSize;  //����ķ�Χ��С
	//�����ı�
	RECT rc = {0, 0, rcArea.right-rcArea.left, rcArea.bottom-rcArea.top};
	HRESULT hr = DrawThemeTextEx(hThm, hMemDC,
		TEXT_LABEL, 0, szText, -1, dwTextFlags , &rc, &dttopts);
	if(FAILED(hr)) return;
	BitBlt(hDC, rcArea.left, rcArea.top, rcArea.right - rcArea.left,
		rcArea.bottom - rcArea.top, hMemDC, 0, 0, SRCCOPY | CAPTUREBLT);
	//Clear
	SelectObject(hMemDC, hBmpOld);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);
	CloseThemeData(hThm);
}


// CPlantsVsZombiesPlusDlg �Ի���
CPlantsVsZombiesPlusDlg::CPlantsVsZombiesPlusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlantsVsZombiesPlusDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlantsVsZombiesPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPlantsVsZombiesPlusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDCANCEL, &CPlantsVsZombiesPlusDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BUTTONMISSION, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonMission)
	ON_BN_CLICKED(IDC_BUTTONPLANTS, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonPlants)
	ON_BN_CLICKED(IDC_BUTTONGAMEBACK, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonGameBack)
	ON_BN_CLICKED(IDC_BUTTONGLASSHOUSE, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonGlassHouse)
	ON_BN_CLICKED(IDC_BUTTONROUND, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonRound)
END_MESSAGE_MAP()


// CPlantsVsZombiesPlusDlg ��Ϣ�������

BOOL CPlantsVsZombiesPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	//������͸������
// 	DWM_BLURBEHIND dbb = {0};
// 	dbb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
// 	dbb.fEnable = true;
// 	dbb.hRgnBlur = CreateEllipticRgn(130,130,170,170);
// 	DwmEnableBlurBehindWindow(this->GetSafeHwnd(), &dbb);
	
	//����͸��ɫ
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, 
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	SetLayeredWindowAttributes(0xEEEEE1, 255, LWA_ALPHA | LWA_COLORKEY);

	//GDI+�ĳ�ʼ��
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	//��Ϸ�Ƿ����� static text λ��
	GameRunFlagTextRect.top = 125;
	GameRunFlagTextRect.left = 20;
	GameRunFlagTextRect.right = 170;
	GameRunFlagTextRect.bottom = 140;

	//�趨��ַ��ֵ
	ZW_BASE = (int*)0x6A9EC0;

	//������ҿ�ݼ�
	RegisterHotKey(this->GetSafeHwnd(), unlimitedSunKey, NULL, VK_NUMPAD1);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedMoneyKey, NULL, VK_NUMPAD2);
	RegisterHotKey(this->GetSafeHwnd(), noRechargeKey ,NULL, VK_NUMPAD3);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedFertilizerKey, NULL, VK_NUMPAD4);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedSprayKey, NULL, VK_NUMPAD5);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedChocolateKey, NULL, VK_NUMPAD6);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedTreeKey, NULL, VK_NUMPAD7);
	RegisterHotKey(this->GetSafeHwnd(), bottleTransparentKey, NULL, VK_NUMPAD8);

	//��ʼ����Ϸ����,��������,����ȴ,����͸��,����,����ֲ��,��Ϸ��̨
	gameProcess = NULL;
	gameProcessFlag = FALSE;
	unlimitedSunFlag = FALSE;
	noRechargeFlag = FALSE;
	bottleTFlag = FALSE;

	initComboPlants();

	GetDlgItem(IDC_BUTTONMISSION)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITMISSION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONPLANTS)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOPLANTS)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOPLANTSRL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONGAMEBACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONGLASSHOUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONROUND)->EnableWindow(FALSE);

	SetTimer(1, 500, NULL);    //GameRunFlagλ��
	SetTimer(2, 1000, NULL);   //GameRunFlag�ַ���
	SetTimer(3, 500, NULL);   //��������
	SetTimer(4, 200, NULL);	  //����ȴ
	SetTimer(5, 500, NULL);   //����͸��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPlantsVsZombiesPlusDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	CRect PVZRect;
	GetClientRect(PVZRect);
	dc.FillSolidRect(PVZRect, 0xEEEEE1);  //���ڱ���ɫ

	//ë������Χ
	MARGINS mar={485,5,355,5};
	DwmExtendFrameIntoClientArea(this->GetSafeHwnd(), &mar);

	//������д��
//	RECT rcText1 = {30, 180, 400, 210};
//	DrawGlowingText(dc.m_hDC, L"���� win7 �ķ�������~~�ܿ�������", rcText1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 15);


	//��������
	CFont GameRunFlagFont;
	GameRunFlagFont.CreateFont(40, 20, 0, 0, 400, FALSE, FALSE, FALSE, FALSE, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 3, DEFAULT_PITCH | FF_DONTCARE, L"΢���ź�");

	CFont GameRunTextFont;
	GameRunTextFont.CreateFont(12, 6, 0, 0, 400, FALSE, FALSE, FALSE, TRUE, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 3, DEFAULT_PITCH | FF_DONTCARE, L"����");

	CFont *OldFont;
	OldFont = dc.SelectObject(&GameRunFlagFont);

	CRect TextRect;

	TextRect.top = 90;
	TextRect.left = 0;
	TextRect.right = PVZRect.Width();
	TextRect.bottom = 120;
	DrawText(dc.m_hDC,
		L"------------------------------",
		-1,
		TextRect,
		DT_CENTER | DT_VCENTER);

	TextRect.top = 125;
	TextRect.left = 0;
	TextRect.right = PVZRect.Width();
	TextRect.bottom = 170;
	DrawText(dc.m_hDC,
		L"------------------------------",
		-1,
		TextRect,
		DT_CENTER | DT_VCENTER);

	dc.SelectObject(&GameRunTextFont);
	dc.SetTextColor(0x0000ff);
	DrawText(dc.m_hDC,
		GameRunFlagText,
		-1,
		GameRunFlagTextRect,
		DT_LEFT | DT_VCENTER);

// 	dc.SetTextColor(0x000000);
// 	TextRect.top = 295;
// 	TextRect.left = 250;
// 	TextRect.right = PVZRect.Width();
// 	TextRect.bottom = 450;
// 	DrawText(dc.m_hDC,
// 		L"    �������������Ŀ\n\n��ֲ���ս��ʬ�����\n\n",
// 		-1,
// 		TextRect,
// 		DT_LEFT | DT_VCENTER);

	dc.SelectObject(OldFont);

	//������ϻ���ͼƬ
	Image *image;
	RectF imageRc;
	Graphics LogoGraphics(dc.m_hDC);

	ImageFromIDResource(IDR_PNG_PLANTS, L"PNG", image);
	imageRc.X = 15;
	imageRc.Y = 0;
	imageRc.Width = 85;
	imageRc.Height = 110;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;

	ImageFromIDResource(IDR_PNG_LOGO, L"PNG", image);
	imageRc.X = 110;
	imageRc.Y = 0;
	imageRc.Width = 425;
	imageRc.Height = 110;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;

	ImageFromIDResource(IDR_PNG_ZOMBIES, L"PNG", image);
	imageRc.X = 345;
	imageRc.Y = 165;
	imageRc.Width = 215;
	imageRc.Height = 255;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;

	ImageFromIDResource(IDR_PNG_UNLIMITED, L"PNG", image);
	imageRc.X = 15;
	imageRc.Y = 165;
	imageRc.Width = 220;
	imageRc.Height = 255;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;


	CDialogEx::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPlantsVsZombiesPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlantsVsZombiesPlusDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	_CrtDumpMemoryLeaks();

	gameProcess = NULL;
	if (TRUE == gameProcessFlag)
		CloseHandle(gameProcess);

	UnregisterHotKey(this->GetSafeHwnd(), unlimitedSunKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedMoneyKey);
	UnregisterHotKey(this->GetSafeHwnd(), noRechargeKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedFertilizerKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedSprayKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedChocolateKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedTreeKey);
	UnregisterHotKey(this->GetSafeHwnd(), bottleTransparentKey);

	GdiplusShutdown(m_pGdiToken);

	CDialogEx::OnCancel();
}

void CPlantsVsZombiesPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
		if (380 == GameRunFlagTextRect.left)
			GameRunFlagTextRectFlag = false;
		if (20 == GameRunFlagTextRect.left)
			GameRunFlagTextRectFlag = true;
		if (GameRunFlagTextRectFlag)
		{
			GameRunFlagTextRect.left += 10;
			GameRunFlagTextRect.right += 10;
		}
		else
		{
			GameRunFlagTextRect.left -= 10;
			GameRunFlagTextRect.right -= 10;
		}
		
		InvalidateRect(CRect(GameRunFlagTextRect.left-20,
							 GameRunFlagTextRect.top,
							 GameRunFlagTextRect.right+20,
							 GameRunFlagTextRect.bottom));
		break;

	case 2:
		GameHwnd = ::FindWindow(NULL,L"ֲ���ս��ʬ���İ�");
		if (GameHwnd)
		{
			GameRunFlagText.Empty();
			GameRunFlagText = CString(L"The Game Is Running!");
			GetWindowThreadProcessId(GameHwnd, &GameProcessId);
			gameProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, GameProcessId);
			if (gameProcess)
				gameProcessFlag = TRUE;

			GetDlgItem(IDC_BUTTONMISSION)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDITMISSION)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONPLANTS)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBOPLANTS)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBOPLANTSRL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONGAMEBACK)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONGLASSHOUSE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONROUND)->EnableWindow(TRUE);
		}
		else
		{
			GameRunFlagText.Empty();
			GameRunFlagText = CString(L"The Game Is Not Running!");
			gameProcess = NULL;
			if (TRUE == gameProcessFlag)
				CloseHandle(gameProcess);

			GetDlgItem(IDC_BUTTONMISSION)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITMISSION)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONPLANTS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBOPLANTS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBOPLANTSRL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONGAMEBACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONGLASSHOUSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONROUND)->EnableWindow(FALSE);
		}
		break;

	case 3:
		nSun = 9999;
		if (TRUE==unlimitedSunFlag && TRUE==gameProcessFlag)
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x768),&nBase,sizeof(int),0);
			for (int i=0; i<10; i++)
				WriteProcessMemory(gameProcess,(int *)(nBase+0x5560),&nSun,sizeof(int),0);
		}
		break;
	case 4:
		nRecharge = 1;
		if (TRUE==noRechargeFlag && TRUE==gameProcessFlag)
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x768),&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x144),&nBase,sizeof(int),0);
			for (int i=0; i<10; i++)
				WriteProcessMemory(gameProcess,(int *)(nBase+0x50+i*0x50),&nRecharge,sizeof(int),0);
		}
		break;
	case 5:
		nBottleT = 100;
		if (TRUE==bottleTFlag && TRUE==gameProcessFlag)
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x768),&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x11C),&nBase,sizeof(int),0);
			for (int i=0; i<35; i++)
				WriteProcessMemory(gameProcess,(int *)(nBase+0x4C+i*0xEC),&nBottleT,sizeof(int),0);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPlantsVsZombiesPlusDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nHotKeyId)
	{
	case unlimitedSunKey:
		if (FALSE==unlimitedSunFlag && TRUE==gameProcessFlag)
			unlimitedSunFlag = TRUE;
		else if(TRUE==unlimitedSunFlag && TRUE==gameProcessFlag)
			unlimitedSunFlag = FALSE;
		break;
	case unlimitedMoneyKey:
		nMoney = 99999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x28),&nMoney,sizeof(int),0);
		break;
	case noRechargeKey:
		if (FALSE==noRechargeFlag && TRUE==gameProcessFlag)
			noRechargeFlag = TRUE;
		else if(TRUE==noRechargeFlag && TRUE==gameProcessFlag)
			noRechargeFlag = FALSE;
		break;
	case unlimitedFertilizerKey:
		nFertilizer = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x1F8),&nFertilizer,sizeof(int),0);
		break;
	case unlimitedSprayKey:
		nSpray = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x1FC),&nSpray,sizeof(int),0);
		break;
	case unlimitedChocolateKey:
		nChocolate = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x228),&nChocolate,sizeof(int),0);
		break;
	case unlimitedTreeKey:
		nTree = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x230),&nTree,sizeof(int),0);
		break;
	case bottleTransparentKey:
		if (FALSE==bottleTFlag && TRUE==gameProcessFlag)
			bottleTFlag = TRUE;
		else if(TRUE==bottleTFlag && TRUE==gameProcessFlag)
			bottleTFlag = FALSE;
		break;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CPlantsVsZombiesPlusDlg::initComboPlants()
{
	CString strTemp;
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->ResetContent();//����������������

	strTemp.Format(L"00 �㶹����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"01 ���տ�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"02 ӣ��ը��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"03 ���");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"04 ������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"05 ��������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"06 ���컨");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"07 ˫������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"08 С�繽");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"09 ���⹽");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"10 ���繽");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"11 Ĺ��������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"12 �Ȼ�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"13 ��С��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"14 ������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"15 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"16 ˯��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"17 �ѹ�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"18 ��������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"19 ���ƺ���");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"20 ������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"21 �ش�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"22 �����׮");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"23 �߼��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"24 ��Ģ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"25 ·�ƻ�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"26 ������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"27 ��Ҷ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"28 �Ѽ�����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"29 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"30 �Ϲ�ͷ");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"31 ������");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"32 ���Ĳ�Ͷ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"33 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"34 ����Ͷ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"35 ���ȶ�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"36 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"37 Ҷ�ӱ���ɡ");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"38 ��յ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"39 ����Ͷ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"40 ��ǹ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"41 ˫�����տ�");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"42 ����Ģ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"43 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"44 \"��\"��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"45 �����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"46 �ش���");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"47 ���׼�ũ��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"48 ģ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"49 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"50 �޼��");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"51 ����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);

	strTemp.Format(L"����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTSRL))->AddString(strTemp);
	strTemp.Format(L"����");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTSRL))->AddString(strTemp);

}

void CPlantsVsZombiesPlusDlg::OnBnClickedButtonMission()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nMission;
	CString sMission;

	GetDlgItem(IDC_EDITMISSION)->GetWindowTextW(sMission);
	nMission = _tstoi(LPCTSTR(sMission));	//������nMission��ֵΪ0

	if (0==nMission || nMission>50)
		MessageBox(L"�����Ǵ� 1 �� 50 ������", L"Error", MB_ICONWARNING);
	else
	{
		if (IDOK == MessageBox(L"��ȷ�ϵ�ǰ��Ϸ����ð��ģʽ�У�����ᵼ�����������\n�������ð��ģʽΪ֮ǰδ��ɵĹؿ�������������ؿ��������ɣ�", L"����ô", MB_OKCANCEL | MB_ICONWARNING))
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
			WriteProcessMemory(gameProcess,(int *)(nBase+0x24),&nMission,sizeof(int),0);
		}
	}
}

void CPlantsVsZombiesPlusDlg::OnBnClickedButtonRound()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nRound = 2;

	if (IDOK == MessageBox(L"��ȷ�ϵ�ǰ��Ϸ����ð��ģʽ�У�����ᵼ�����������", L"����ڶ���", MB_OKCANCEL | MB_ICONWARNING))
	{
		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x2C),&nRound,sizeof(int),0);
	}
}

void CPlantsVsZombiesPlusDlg::OnBnClickedButtonPlants()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nAddPlantName = 0;
	CString sAddPlantName;

	GetDlgItem(IDC_COMBOPLANTS)->GetWindowTextW(sAddPlantName);
	nAddPlantName = _tstoi(LPCTSTR(sAddPlantName));

	if (nAddPlantName<0 || nAddPlantName>510)
	{
		MessageBox(L"û������ֲ��", L"Error!", MB_ICONWARNING);
		return;
	}

	int nAddPlantRL = 0;
	CString sAddPlantRL;

	GetDlgItem(IDC_COMBOPLANTSRL)->GetWindowTextW(sAddPlantRL);
	if("����" == sAddPlantRL)
		nAddPlantRL = 0;
	if("����" == sAddPlantRL)
		nAddPlantRL = 1;

	int curPlantTotalNum = 0;
	BOOL curPlantPosition[4][8] = {{FALSE}};
	int addPlantPosition = 0;
	int addPlantLine = 0;
	int addPlantRow = 0;
	int addPlantColor = 0;
	int addPlantGrow = 3;


	ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x350),&curPlantTotalNum,sizeof(int),0);	//�������
	for (int i=0; i<curPlantTotalNum; i++)
	{
		//����λ��(0����,1����,2С��,3ˮ���)
		ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0xC+i*0x58),&addPlantPosition,sizeof(int),0);
		if (0 == addPlantPosition)
		{
			//����������,����������
			ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0x10+i*0x58),&addPlantLine,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0x14+i*0x58),&addPlantRow,sizeof(int),0);
			curPlantPosition[addPlantRow][addPlantLine] = TRUE;
		}
	}
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<8; j++)
		{
			if (FALSE == curPlantPosition[i][j])
			{
				addPlantRow = i;
				addPlantLine = j;

				i=5;
				break;
			}
			if (3==i && 7==j)
			{
				MessageBox(L"����ֲ������,���ƶ�������һЩֲ��!", L"ʧ��", MB_ICONWARNING);
				return;
			}
		}
	}
	int addPlantTotalNum = curPlantTotalNum + 1;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350),&addPlantTotalNum,sizeof(int),0);	//�������
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x08+curPlantTotalNum*0x58),&nAddPlantName,sizeof(int),0);	//��������
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x10+curPlantTotalNum*0x58),&addPlantLine,sizeof(int),0);	//����������
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x14+curPlantTotalNum*0x58),&addPlantRow,sizeof(int),0);		//����������
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x18+curPlantTotalNum*0x58),&nAddPlantRL,sizeof(int),0);		//���跽��(0��,1��)
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x2C+curPlantTotalNum*0x58),&addPlantGrow,sizeof(int),0);	//����״̬(0����,1С��,2�л�,3��)
	
	int tmp = 1324960763;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x20+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//δ֪
	tmp = 4;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x34+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//δ֪
	tmp = 1324974559;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x40+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//δ֪
	tmp = 1317389145;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x48+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//δ֪
	tmp = 1318828743;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x50+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//�ɿ���

	CString successAddPlant = CString("�ɹ����\"") + sAddPlantName + CString("\",ˢ�»�԰�󿴵�!");
	MessageBox(successAddPlant, L"�ɹ�");
}


void CPlantsVsZombiesPlusDlg::OnBnClickedButtonGameBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ڴ��� 0054EBA8 - 74 2E - je 0054EBD8
	// ����	   0054EBA8 - EB 00 - jmp 0054EBAA
	// ��ôƫ�ƾ���0054EBAA-0054EBA8-2 (ռ�����ֽ�)

	CString enableGameBack;
	GetDlgItem(IDC_BUTTONGAMEBACK)->GetWindowTextW(enableGameBack);

	byte changeA;
	byte changeB;

	if ("������̨����" == enableGameBack)
	{
		GetDlgItem(IDC_BUTTONGAMEBACK)->SetWindowTextW(L"���ú�̨����");

		changeA = 0xEB;
		changeB = 0x00;

		WriteProcessMemory(gameProcess,(int *)(0x0054EBA8),&changeA,sizeof(byte),0);
		WriteProcessMemory(gameProcess,(int *)(0x0054EBA9),&changeB,sizeof(byte),0);
	}
	if ("���ú�̨����" == enableGameBack)
	{
		GetDlgItem(IDC_BUTTONGAMEBACK)->SetWindowTextW(L"������̨����");

		changeA = 0x74;
		changeB = 0x2E;

		WriteProcessMemory(gameProcess,(int *)(0x0054EBA8),&changeA,sizeof(byte),0);
		WriteProcessMemory(gameProcess,(int *)(0x0054EBA9),&changeB,sizeof(byte),0);
	}
}


void CPlantsVsZombiesPlusDlg::OnBnClickedButtonGlassHouse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int curPlantTotalNum = 0;
	int plantPosition = 0;
	int glassHousePlantTotalNum = 0;

	ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x350),&curPlantTotalNum,sizeof(int),0);	//�������
	for (int i=0; i<curPlantTotalNum; i++)
	{
		//����λ��(0����,1����,2С��,3ˮ���)
		ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0xC+i*0x58),&plantPosition,sizeof(int),0);
		if (0 == plantPosition)
			glassHousePlantTotalNum++;

	}
	if (IDOK == MessageBox(L"�˲���������������е�ֲ��,ȷ��?", L"ɾ��ô", MB_OKCANCEL | MB_ICONWARNING))
	{
		curPlantTotalNum = curPlantTotalNum - glassHousePlantTotalNum;
		WriteProcessMemory(gameProcess,(int *)(nBase+0x350),&curPlantTotalNum,sizeof(int),0);
		MessageBox(L"ˢ�»�԰���������ֲ��!", L"��ճɹ�");
	}
}
