
// PlantsVsZombiesPlusDlg.h : ͷ�ļ�
//

#pragma once


// CPlantsVsZombiesPlusDlg �Ի���
class CPlantsVsZombiesPlusDlg : public CDialogEx
{
// ����
public:
	CPlantsVsZombiesPlusDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLANTSVSZOMBIESPLUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	HWND GameHwnd;      //ȡ����Ϸ���
	DWORD GameProcessId;  //ȡ����Ϸ����ID
	HANDLE gameProcess;   //��ȡ��Ϸ����
	BOOL gameProcessFlag;  //�ж��Ƿ����ɹ�

	CString GameRunFlagText;
	CRect GameRunFlagTextRect;
	bool GameRunFlagTextRectFlag;

	int *ZW_BASE;       //��ַ
	int nBase;		  //�洢��ַ
	int nSun;			//����ֵ
	int nMoney;			//���
	int nRecharge;		//��ȴ
	int nFertilizer;	//����
	int nSpray;			//ɱ���
	int nChocolate;		//�ɿ���
	int nTree;			//����ʳ��
	int nBottleT;		//����͸��

	BOOL unlimitedSunFlag;    //�ж��Ƿ�ʼ��������
	BOOL noRechargeFlag;      //�ж��Ƿ�ʼû����ȴ
	BOOL bottleTFlag;         //�ж��Ƿ�ʼ����͸��


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	void initComboPlants();
	afx_msg void OnBnClickedButtonMission();
	afx_msg void OnBnClickedButtonPlants();
	afx_msg void OnBnClickedButtonGameBack();
	afx_msg void OnBnClickedButtonGlassHouse();
	afx_msg void OnBnClickedButtonRound();
};