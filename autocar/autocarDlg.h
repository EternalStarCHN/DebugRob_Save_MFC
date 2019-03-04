// autocarDlg.h : ͷ�ļ�
//
#include "cv.h"
#include "highgui.h"
#include "resource.h"
#include "CvvImage.h"
#include "DebugRob.h"
#include "zbar.h"            
#include <iostream>  

#pragma once

#include "./serial/ComPort.h"


using namespace std;      
using namespace zbar;  //���zbar���ƿռ�    
using namespace cv;  
using namespace Debug;

// CautocarDlg �Ի���
class CautocarDlg : public CDialog, public DebugRob
{
// ����
public:
	CautocarDlg(CWnd* pParent = NULL);	//��׼���캯��

	IplImage* image;
	IplImage* m_Frame;
	CvvImage m_CvvImage;

	int m_modeall;
    int m_modeevery;
	int m_modeeverytime;
	int m_modeeveryold;
	int m_modeeverynumber;

	LsComm::CComPort m_ComPort;	
	void SendData(unsigned char arrays[], int lenth);
	/* 0xF0 0x04 0xhh 0xhh 0xhh 0x0A */
	void PointMode(PointMode_t, int16_t angle = 0, uint8_t lineSpeed = 10);
	/* 0xF0 0x03 0xhh 0xhh 0x0A */
	void LineMode(uint8_t point, uint8_t delay = 0, uint8_t transSpeed = 40);
	void Reset(void);
	int _1To2(void);
	int _2To3(void);
	int _2To4(void);
	int _3To5(void);
	int _3To6(void);
	int _4To5(void);
	int _4To6(void);
	int _5To7(void);
	int _5To8(void);
	int _6To7(void);
	int _6To8(void);

	// ���ڽ������ݴ�����
	afx_msg LONG OnRecvSerialData(WPARAM wParam,LPARAM lParam);
	unsigned char sendData[18];

	BOOL PreTranslateMessage(MSG* pMSG);
	int m_tunerpwm;//pwmת���ֵ����ת����ơ�
	int m_speedpwm;//ȫ��PWM�ٶȣ���1000�Ӽ�
	int m_tunerstart;//��ʼԭ����ת
	int m_stopmagnetic;//ֹͣ�Ĵų��Ƕ�
  int m_linecenterlast;

// �Ի�������
	enum { IDD = IDD_AUTOCAR_DIALOG };

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
public:
	afx_msg void OnBnClickedBtopen();
	afx_msg void OnBnClickedBtclose();
	CString m_sersend;
	CString m_serrev;
	afx_msg void OnBnClickedBtsend();
	CString m_djhead;//���ͷ��
	CString m_djlefthand;
	CString m_djrighthand;
	CString m_cgmagnetic;//���дų�
	CString m_cgdegree;
	CString m_cgrgb1;
	CString m_cgrgb2;
	CString m_cgclockleft;
	CString m_cgclockright;
	CString m_pwmleft;
	CString m_pwmright;
	CString m_pwnlimit;

    int m_clockbuffleft;
    int m_clockbufffright;
	/*******�ҵı�������**********/
	int   SetMidLine;
	int   SetSpeed;
	int   LastMidLine;
	int   DirCtrlFlag;
    int   UpDownStatus;
    int   RunFlag;
    int   DirControlPWMbuff;

	afx_msg void OnBnClickedBtspeedup();
	afx_msg void OnBnClickedBtspeeddown();
	afx_msg void OnBnClickedBtleft();
	afx_msg void OnBnClickedBtright();
	void Stoptuner(void);
	CString m_ceqing;
	CString m_cggyro;
	CString m_cgclockleftadd;
	CString m_cgclockrightadd;
	afx_msg void OnBnClickedBtclearclock();
	CString m_cggyrodegree;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtopenvideo();
	afx_msg void OnBnClickedBtclosevideo();
	CString m_linedegree;

	CString m_linecrossy;
	CString m_centerx;
	CString m_centery;
	afx_msg void OnBnClickedBtautodrive();
	afx_msg void OnBnClickedBtstopdrive();
	afx_msg void OnBnClickedBtauto12();
	afx_msg void OnBnClickedBtauto23();
	afx_msg void OnBnClickedBtauto34();
	afx_msg void OnBnClickedBtauto35();
	afx_msg void OnBnClickedBtauto36();
	afx_msg void OnBnClickedBtauto45();
	afx_msg void OnBnClickedBtauto46();
	afx_msg void OnBnClickedBtauto57();
	afx_msg void OnBnClickedBtauto58();
	afx_msg void OnBnClickedBtauto67();
	afx_msg void OnBnClickedBtauto68();
	afx_msg void OnBnClickedBtauto71();
	afx_msg void OnBnClickedBtauto81();
	void Btauto12();
	void Btauto23();
	void Btauto34();
	void Btauto35();
	void Btauto36();
	void Btauto45();
	void Btauto46();
	void Btauto57();
	void Btauto58();
	void Btauto67();
	void Btauto68();
	void Btauto71();
	void Btauto81();
	void Btauto10();
	CString m_captionnumber;
	afx_msg void OnBnClickedBtscannumber();
	CString m_locationstart;
	CString m_locationnext;
	CString m_locationgold;
	int PID_Dir_Control(int err);	
    int PID_Speed_Controlleft(int err);
    CString m_PCtrl;
    CString m_ICtrl;
    CString m_DCtrl;
    CString m_UpStatus;
    CString m_DownStatus;

	CString m_linebuttonlocation;
	void stopcar(void);
	int m_stopcar;//�ĵ�ֹͣС��
	CString m_mymode;

	CString m_mymodeevery;
	Point Center_cal(vector<vector<Point> > contours,int i);

	afx_msg void OnEnChangeEditgyro();
	afx_msg void OnEnChangeEditscannumber();
	afx_msg void OnEnChangeEdithead();
	afx_msg void OnBnClickedPatern12();

    //模式设置
	int sign;
	int next;
	int step;
};
/*Deal Receive Data Function / �������ݴ�����
*/
void OnReceiveData(LPVOID pSender,void* pBuf,DWORD InBufferCount);
/*Deal with the break of com /�������ж� 
*/
void OnComBreak(LPVOID pSender,DWORD dwMask,COMSTAT stat);