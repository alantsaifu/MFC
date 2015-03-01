
// SoGameDlg.h : ͷ�ļ�
//

#pragma once


// CSoGameDlg �Ի���
class CSoGameDlg : public CDialogEx
{
// ����
public:
	CSoGameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	int Player1StateOld;
	int Player1StateCur;
	int Player2StateOld;
	int Player2StateCur;
	int PlayerHP[2];
	double ButtleSpeedH;  //�ӵ�ˮƽ�ٶ�
	double ButtleSpeedV;  //�ӵ���ֱ�ٶ�
	double ButtleX;  //�ӵ���ʼX����
	double ButtleY;  //�ӵ���ʼY����
	BOOL ButtleFlag; //�ӵ���ұ�־
	BOOL ButtleSpeedFlag; //�ӵ��ٶȱ�־
	BOOL ButtleFireFlag; //�ӵ������־
	int ButtleFireTime;  //�ӵ�����ʱ��
	int WindSpeed; //����
	int WindSpeedTmp; //�������
	int GameGravity;  //����
	

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void BulletPathWay();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void ButtleEdge(int x, int y);
};
