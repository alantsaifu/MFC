
// ChineseChessMFCDlg.h : ͷ�ļ�
//

#include "ChessManRules.h"
#pragma once

struct ChessBoard
{
	int ChessBoardX, ChessBoardY, ChessBoardWide, 
		ChessBoardFocusXFlag, ChessBoardFocusYFlag,
		ChessBoardFocusLong, ChessBoardFocusDistance;

	CImageList ChessMan;       //����ͼƬ����
	CBitmap ChessManBmp;   //����λͼ

	BYTE ShowChessMan[10][9];    //�ƶ����Ӻ�

	ChessManMove CMMove;

	ChessManRule CMRule;
	BYTE nFlag;
	BYTE nChessMan; //�����������

	ChessBoard()
	{
		ChessBoardX = 30;
		ChessBoardY = 50;
		ChessBoardWide = 50;
		ChessBoardFocusXFlag = 1;
		ChessBoardFocusYFlag = 1;
		ChessBoardFocusLong = 15;
		ChessBoardFocusDistance = 5;

		//����һ������14��24λɫ40x40ͼƬ��ImageList,ILC_MASK����˼��ͬʱ����һ�� mask,����������ָ���˱�����ɫ�Ժ�ImageList�Ϳ��Ի�͸��ͼ����  
		ChessMan.Create(40, 40, ILC_COLOR24|ILC_MASK, 14, 14);
		ChessManBmp.LoadBitmap(IDB_CHESSMAN);
		//��ͼƬ�ӵ�ImageList��
		ChessMan.Add(&ChessManBmp, RGB(0,255,0));

		memcpy(ShowChessMan, InitChessMan, 90);//��ʼ������

		CMMove.ChessManID = NOCHESS;
		CMRule.ChessManID = NOCHESS;
		nFlag = FALSE;
		nChessMan = 0;

	}

	//��������
	void DrawChessBoard(CPaintDC* dc)
	{
		CFont RiverFont;
		RiverFont.CreatePointFont(250,L"����");  

		CFont *OldFont;  
		OldFont = dc->SelectObject(&RiverFont);
		
		int n, i,j;
		//�����ϰ벿��
		for (i=0; i<9; i++)
		{
			dc->MoveTo(ChessBoardX+i*ChessBoardWide, ChessBoardY);
			dc->LineTo(ChessBoardX+i*ChessBoardWide, ChessBoardY+4*ChessBoardWide);
		}
		for (j=0; j<5; j++)
		{
			dc->MoveTo(ChessBoardX, ChessBoardY+j*ChessBoardWide);
			dc->LineTo(ChessBoardX+8*ChessBoardWide, ChessBoardY+j*ChessBoardWide);
		}
		//���Ӻ��粿��
		dc->SelectObject(RiverFont);
		dc->TextOut(ChessBoardX+5,ChessBoardY+4*ChessBoardWide+10,L"�� ��             �� ��");
		dc->SelectObject(OldFont);
		for (i=0; i<2; i++)
		{
			dc->MoveTo(ChessBoardX+i*8*ChessBoardWide, ChessBoardY+4*ChessBoardWide);
			dc->LineTo(ChessBoardX+i*8*ChessBoardWide, ChessBoardY+5*ChessBoardWide);
		}
		//�����°벿��
		for (i=0; i<9; i++)
		{
			dc->MoveTo(ChessBoardX+i*ChessBoardWide, ChessBoardY+5*ChessBoardWide);
			dc->LineTo(ChessBoardX+i*ChessBoardWide, ChessBoardY+9*ChessBoardWide);
		}
		for (j=0; j<5; j++)
		{
			dc->MoveTo(ChessBoardX, ChessBoardY+(j+5)*ChessBoardWide);
			dc->LineTo(ChessBoardX+8*ChessBoardWide, ChessBoardY+(j+5)*ChessBoardWide);
		}
		//��˧���ָ�
		for (i=0; i<2; i++)
		{
			for (j=0; j<2; j++)
			{
				dc->MoveTo(ChessBoardX+(3+j*2)*ChessBoardWide, ChessBoardY+i*7*ChessBoardWide);
				dc->LineTo(ChessBoardX+(5-j*2)*ChessBoardWide, ChessBoardY+(i*7+2)*ChessBoardWide);
			}
		}
		//��̨����
		for (i=0; i<2; i++)
		{
			for (n=0; n<2; n++)
			{
				for (j=0; j<4; j++)
				{
					switch (j)
					{
					case 0:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = -1;
						break;
					case 1:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = 1;
						break;
					case 2:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = -1;
						break;
					case 3:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = 1;
						break;
					}
					dc->MoveTo(ChessBoardX+(i*6+1)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*5+2)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance+ChessBoardFocusYFlag*ChessBoardFocusLong);
					dc->LineTo(ChessBoardX+(i*6+1)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*5+2)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
					dc->LineTo(ChessBoardX+(i*6+1)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance+ChessBoardFocusXFlag*ChessBoardFocusLong, ChessBoardY+(n*5+2)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
				}
			}
		}
		//���佹��
		for (i=0; i<4; i++)
		{
			for (n=0; n<2; n++)
			{
				for (j=0; j<2; j++)
				{
					switch (j)
					{
					case 0:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = -1;
						break;
					case 1:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = 1;
						break;
					}
					dc->MoveTo(ChessBoardX+(i*2+2)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance+ChessBoardFocusYFlag*ChessBoardFocusLong);
					dc->LineTo(ChessBoardX+(i*2+2)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
					dc->LineTo(ChessBoardX+(i*2+2)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance+ChessBoardFocusXFlag*ChessBoardFocusLong, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
				}
				for (j=0; j<2; j++)
				{
					switch (j)
					{
					case 0:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = -1;
						break;
					case 1:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = 1;
						break;
					}
					dc->MoveTo(ChessBoardX+i*2*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance+ChessBoardFocusYFlag*ChessBoardFocusLong);
					dc->LineTo(ChessBoardX+i*2*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
					dc->LineTo(ChessBoardX+i*2*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance+ChessBoardFocusXFlag*ChessBoardFocusLong, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
				}
			}
		}
	}
	//��������
	void DrawChessMan(CPaintDC* dc)
	{
		int i, j;
		POINT pt;

		for (i=0; i<10; i++)
		{
			for (j=0; j<9; j++)
			{
				if(ShowChessMan[i][j]==NOCHESS)
					continue;

				pt.x = 10+j*ChessBoardWide;
				pt.y = 30+i*ChessBoardWide;
				ChessMan.Draw(dc, ShowChessMan[i][j]-1, pt, ILD_TRANSPARENT);
			}
		}

		//�����û������϶�������
		if(CMMove.ChessManID != NOCHESS)
			ChessMan.Draw(dc, CMMove.ChessManID-1, CMMove.MovePoint, ILD_TRANSPARENT);
	}

	//��֤�����ƶ��ĺϷ���
	BYTE ValidMove()
	{
		int x, y;
		switch (CMRule.ChessManID)
		{			      
		case B_GENERAL:     //�ڽ�
			//���������ϵ�����
			for (y=0; y<10; y++)
			{
				//û������,��������
				if (ShowChessMan[y][CMRule.From.x] == NOCHESS)
					continue;
				//������,��Ϊ��˧����������
				else
					//����Ǻ�˧
					if (ShowChessMan[y][CMRule.From.x] == R_GENERAL)
						if ((CMRule.To.x==CMRule.From.x && CMRule.To.y==y) //���ԳԵ���˧,�����ںڽ����ָ����ƶ�
							|| (CMRule.To.x>2 && CMRule.To.x<6 && CMRule.To.y > -1 && CMRule.To.y<3
							&& ((CMRule.To.x==CMRule.From.x && abs(CMRule.To.y-CMRule.From.y)==1)
							|| (CMRule.To.y==CMRule.From.y && abs(CMRule.To.x-CMRule.From.x)==1))))
							nFlag = TRUE;
						else
							nFlag = FALSE;
					else
						//����������������,ֻ���ںڽ����ָ����ƶ�
						if (CMRule.To.x>2 && CMRule.To.x<6 && CMRule.To.y > -1 && CMRule.To.y<3
							&& ((CMRule.To.x==CMRule.From.x && abs(CMRule.To.y-CMRule.From.y)==1)
							|| (CMRule.To.y==CMRule.From.y && abs(CMRule.To.x-CMRule.From.x)==1)))
							nFlag = TRUE;
						else
							nFlag = FALSE;
			}
			break;
 		case B_CHARIOT:     //�ڳ�
			if (CMRule.To.x==CMRule.From.x || CMRule.To.y==CMRule.From.y)
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
 		case B_HORSE:       //����
			if ((abs(CMRule.To.x-CMRule.From.x)==1 && abs(CMRule.To.y-CMRule.From.y)==2)
				|| (abs(CMRule.To.x-CMRule.From.x)==2 && abs(CMRule.To.y-CMRule.From.y)==1))
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
 		case B_CANON:       //����
			if (CMRule.To.x==CMRule.From.x || CMRule.To.y==CMRule.From.y)
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
		case B_ADVISOR:     //��ʿ
			if (CMRule.To.x>2 && CMRule.To.x<6 && CMRule.To.y > -1 && CMRule.To.y<3
				&& (abs(CMRule.To.x-CMRule.From.x)==1 && abs(CMRule.To.y-CMRule.From.y)==1))
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
		case B_ELEPHANT:    //����
			if (CMRule.To.x>-1 && CMRule.To.x<9 && CMRule.To.y > -1 && CMRule.To.y<5
				&& (abs(CMRule.To.x-CMRule.From.x)==2 && abs(CMRule.To.y-CMRule.From.y)==2))
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
 		case B_SOLDIER:     //�ڱ�
			if (CMRule.From.y>2 && CMRule.From.y<5)
				if (CMRule.To.x==CMRule.From.x && CMRule.To.y-CMRule.From.y==1)
					nFlag = TRUE;
				else
					nFlag = FALSE;
			else
				if ((CMRule.To.x==CMRule.From.x && CMRule.To.y-CMRule.From.y==1)
					|| (CMRule.To.y==CMRule.From.y && abs(CMRule.To.x-CMRule.From.x)==1))
					nFlag = TRUE;
				else
					nFlag = FALSE;
			break;


// 		case R_GENERAL:     //��˧
// 		case R_CHARIOT:     //�쳵
// 		case R_HORSE:       //����
// 		case R_CANON:       //����
// 		case R_ADVISOR:     //��ʿ
// 		case R_ELEPHANT:    //����
// 		case R_SOLDIER:     //����
			break;
		}

		return nFlag;
	}
};

// CChineseChessMFCDlg �Ի���
class CChineseChessMFCDlg : public CDialogEx
{
	// ����
public:
	CChineseChessMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_CHINESECHESSMFC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	ChessBoard DrawCB;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};