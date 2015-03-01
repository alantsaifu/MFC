
// ChineseChessMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChineseChessMFC.h"
#include "ChineseChessMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�жϿ�ʼ
BYTE StartFlag = START;
BYTE WhichSide = REDCHESS;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChineseChessMFCDlg �Ի���




CChineseChessMFCDlg::CChineseChessMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChineseChessMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChineseChessMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChineseChessMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CChineseChessMFCDlg ��Ϣ�������

BOOL CChineseChessMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, 
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	HINSTANCE hInstUser32 = LoadLibrary(L"user32.dll"); 
	if(hInstUser32) 
	{ 
		typedef BOOL (WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
		SLWA SLWAFunc= NULL;
		//ȡ��SetLayeredWindowAttributes����ָ�� 
		SLWAFunc=(SLWA)GetProcAddress(hInstUser32, "SetLayeredWindowAttributes");
		if(SLWAFunc)SLWAFunc(this->GetSafeHwnd(), 0xABCDEF, 255, LWA_ALPHA | LWA_COLORKEY);
		FreeLibrary(hInstUser32);
	}

	HINSTANCE hInstDwmapi = LoadLibrary(L"dwmapi.dll");
	MARGINS mar={-1};
	if(hInstDwmapi) 
	{ 
		typedef BOOL (WINAPI *DEFICA)(HWND,MARGINS*);
		DEFICA DEFICAFunc= NULL;
		//ȡ��DwmExtendFrameIntoClientArea����ָ�� 
		DEFICAFunc=(DEFICA)GetProcAddress(hInstDwmapi, "DwmExtendFrameIntoClientArea");
		if(DEFICAFunc)DEFICAFunc(this->GetSafeHwnd(),&mar);
		FreeLibrary(hInstDwmapi);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChineseChessMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChineseChessMFCDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	CRect rcGlassArea;
	GetClientRect ( rcGlassArea );
	dc.FillSolidRect(rcGlassArea, 0xABCDEF);  //���ڱ���ɫ

	DrawCB.DrawChessBoard(&dc);
	DrawCB.DrawChessMan(&dc);

	CDialogEx::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChineseChessMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChineseChessMFCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	StartFlag = START;

	int x, y;
	//�����껻��������ϵĸ���
	x = (point.x-10)/DrawCB.ChessBoardWide;
	y = (point.y-30)/DrawCB.ChessBoardWide;
	//�ж�����Ƿ��������ڣ����ҵ������û�����
	if(x>=0 && x<9 && y>=0 && y<10 )
	{
		//����ǰ���ӵ���Ϣװ�룬��¼�ƶ����ӵĽṹ��
		DrawCB.CMMove.ChessManID = DrawCB.ShowChessMan[y][x];

		//����ǰ���ӵ���Ϣװ��,��¼�Ϸ��ƶ����ӵĽṹ��
		DrawCB.CMRule.From.x = x;
		DrawCB.CMRule.From.y = y;
		DrawCB.CMRule.ChessManID = DrawCB.ShowChessMan[y][x];

		//��������ԭλ������ȥ��
		DrawCB.ShowChessMan[y][x] = NOCHESS;

		//�������е�����λ��������ڵ�
		point.x -= 20;
		point.y -= 20;
		DrawCB.CMMove.MovePoint = point;
		//�ػ���Ļ
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		SetCapture();//��ռ��꽹��
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CChineseChessMFCDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int x, y;
	//�����껻��������ϵĸ���
	x = (point.x-10)/DrawCB.ChessBoardWide;
	y = (point.y-30)/DrawCB.ChessBoardWide;

	//���ƶ����λ��װ��,��¼�Ϸ��ƶ����ӵĽṹ��
	DrawCB.CMRule.To.x = x;
	DrawCB.CMRule.To.y = y;

	//�ж��Ƿ����ƶ����ӣ����Ҹ����ӵ��ƶ���һ���Ϸ��߷�
	if(DrawCB.CMMove.ChessManID && DrawCB.ValidMove())
	{
		DrawCB.ShowChessMan[y][x] = DrawCB.CMMove.ChessManID;
	}
	else
	{
		DrawCB.ShowChessMan[DrawCB.CMRule.From.y][DrawCB.CMRule.From.x] = DrawCB.CMMove.ChessManID;
	}

	DrawCB.CMMove.ChessManID = NOCHESS;//���ƶ����������

	//�ػ���Ļ  
	InvalidateRect(NULL,FALSE);
	UpdateWindow();	
	ReleaseCapture();//�ͷ���꽹��

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CChineseChessMFCDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(DrawCB.CMMove.ChessManID)
	{
		//��ֹ�������ϳ�����
		if(point.x < 30)//���
			point.x = 30;
		if(point.y < 50)//�ϱ�
			point.y = 50;
		if(point.x > 430)//�ұ�
			point.x = 430;
		if(point.y > 500)//�±�
			point.y = 500;

		//����������λ��������ڴ�
		point.x -= 20;
		point.y -= 20;

		DrawCB.CMMove.MovePoint = point;//�����ƶ����ӵ�����	

		InvalidateRect(NULL,FALSE);//ˢ�´���
		UpdateWindow();//����ִ��ˢ��
	}
	CDialogEx::OnMouseMove(nFlags, point);
}
