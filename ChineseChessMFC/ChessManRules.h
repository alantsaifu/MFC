//define.h 
#ifndef CHESSMANRULES_H
#define CHESSMANRULES_H

#define START      1//��ʼ
#define OVER       0//����

#define BLACKCHESS 1//�ڷ�
#define REDCHESS   2//�췽

//--------����--------
#define NOCHESS    0 //û������

#define B_GENERAL  1 //�ڽ�
#define B_CHARIOT  2 //�ڳ�
#define B_HORSE	   3 //����
#define B_CANON	   4 //����
#define B_ADVISOR  5 //��ʿ
#define B_ELEPHANT 6 //����
#define B_SOLDIER  7 //�ڱ�

#define R_GENERAL  8 //��˧
#define R_CHARIOT  9 //�쳵
#define R_HORSE    10//����
#define R_CANON    11//����
#define R_ADVISOR  12//��ʿ
#define R_ELEPHANT 13//����
#define R_SOLDIER  14//����

//��ʼ������λ��
const BYTE InitChessMan[10][9] =
{
	{B_CHARIOT,B_HORSE,B_ELEPHANT,B_ADVISOR,B_GENERAL,B_ADVISOR,B_ELEPHANT,B_HORSE,B_CHARIOT},
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	{NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS},
	{B_SOLDIER,NOCHESS,B_SOLDIER,NOCHESS,B_SOLDIER,NOCHESS,B_SOLDIER,NOCHESS,B_SOLDIER},
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	//����                                            ����//
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	{R_SOLDIER,NOCHESS,R_SOLDIER,NOCHESS,R_SOLDIER,NOCHESS,R_SOLDIER,NOCHESS,R_SOLDIER},
	{NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS},
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	{R_CHARIOT,R_HORSE,R_ELEPHANT,R_ADVISOR,R_GENERAL,R_ADVISOR,R_ELEPHANT,R_HORSE,R_CHARIOT}
};
//������ɫ

//�ж�������ɫ
#define IsBlack(x) (x>=B_GENERAL && x<=B_SOLDIER)
#define IsRed(x)   (x>=R_GENERAL && x<=R_SOLDIER)

//�ж����������ǲ���ͬɫ
#define IsSameSide(x,y) ((IsBlack(x) && IsBlack(y)) || (IsRed(x) && IsRed(y)))

//����λ��
struct ChessManPos
{
	BYTE x;
	BYTE y;
};

//�����ƶ�
struct ChessManMove
{
	BYTE ChessManID;
	POINT MovePoint;
};

//�����߷�
struct ChessManRule
{
	BYTE ChessManID;  //������ʲô����
	ChessManPos From;//��ʼλ��
	ChessManPos To;  //�ߵ�ʲôλ��
};

//����ʱ��Ҫ�����ݽṹ
struct ChessManUndoMove
{
	ChessManMove cmChessMove;
	BYTE ChessManID;//���Ե�������
};

#endif
