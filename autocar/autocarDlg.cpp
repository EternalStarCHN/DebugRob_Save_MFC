// autocarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "autocar.h"
#include "autocarDlg.h"
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "baseapi.h"
#include "strngs.h"
#include "DebugRob.h"
#pragma comment(lib, "libtesseract302.lib")

#ifdef _DEBUG
#endif

CvCapture *capture;
CvCapture *capture1;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;

CRect rect1;
CDC *pDC1;
HDC hDC1;
CWnd *pwnd1;
CRect rect2;
CDC *pDC2;
HDC hDC2;
CWnd *pwnd2;

CRect rect3;
CDC *pDC3;
HDC hDC3;
CWnd *pwnd3;

#define PRATIO 18.6

//定义串口数据接收消息常量
#define WM_RECV_SERIAL_DATA WM_USER + 101
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_ABOUTBOX
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CautocarDlg 对话框

CautocarDlg::CautocarDlg(CWnd *pParent /*=NULL*/)
		: CDialog(CautocarDlg::IDD, pParent), m_sersend(_T("")), m_serrev(_T("")), m_djhead(_T("")), m_djlefthand(_T("")), m_djrighthand(_T("")), m_cgmagnetic(_T("")), m_cgdegree(_T("")), m_cgrgb1(_T("")), m_cgrgb2(_T("")), m_cgclockleft(_T("")), m_cgclockright(_T("")), m_pwmleft(_T("500")), m_pwmright(_T("500")), m_pwnlimit(_T("")), m_ceqing(_T("")), m_cggyro(_T("")), m_cgclockleftadd(_T("")), m_cgclockrightadd(_T("")), m_cggyrodegree(_T("180")), m_linedegree(_T("")), m_linecrossy(_T("")), m_centerx(_T("")), m_centery(_T("")), m_captionnumber(_T("")), m_locationstart(_T("")), m_locationnext(_T("")), m_locationgold(_T("")), m_PCtrl(_T("2")), m_ICtrl(_T("0")), m_DCtrl(_T("0")), m_UpStatus(_T("")), m_DownStatus(_T("")), m_linebuttonlocation(_T("")), m_mymode(_T("")), m_mymodeevery(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_tunerpwm = 0;
	m_speedpwm = 1000;
	m_tunerstart = 0;
	m_modeall = 0;
	SetMidLine = 320;
	SetSpeed = 55;
	LastMidLine = 320;
	DirCtrlFlag = 0;
	UpDownStatus = 0;
	RunFlag = 0;
	m_linecenterlast = 0;
	m_modeevery = 0;
	m_stopcar = 0;
	m_modeeveryold = 0;
	sign = 0;
	next = 2;
	step = 0;
}

void CautocarDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSEND, m_sersend);
	DDX_Text(pDX, IDC_EDITREV, m_serrev);
	DDX_Text(pDX, IDC_EDITHEAD, m_djhead);
	DDX_Text(pDX, IDC_EDITLEFTHAND, m_djlefthand);
	DDX_Text(pDX, IDC_EDITRIGHTHAND, m_djrighthand);
	DDX_Text(pDX, IDC_EDITEDGE, m_cgmagnetic);
	DDX_Text(pDX, IDC_EDITPAPO, m_cgdegree);
	DDX_Text(pDX, IDC_EDITCOLOR, m_cgrgb1);
	DDX_Text(pDX, IDC_EDITCOLOR2, m_cgrgb2);
	DDX_Text(pDX, IDC_EDITLEFTCLOCK, m_cgclockleft);
	DDX_Text(pDX, IDC_EDITRIGHTCLOCK, m_cgclockright);
	DDX_Text(pDX, IDC_EDITLEFTPWM, m_pwmleft);
	DDX_Text(pDX, IDC_EDITRIGHTPWM, m_pwmright);
	DDX_Text(pDX, IDC_EDITLIMITPWM2, m_pwnlimit);
	DDX_Text(pDX, IDC_EDITCEQING, m_ceqing);
	DDX_Text(pDX, IDC_EDITGYRO, m_cggyro);
	DDX_Text(pDX, IDC_EDITLEFTCLOCKADD, m_cgclockleftadd);
	DDX_Text(pDX, IDC_EDITRIGHTCLOCKADD, m_cgclockrightadd);
	DDX_Text(pDX, IDC_EDITGYRODEGREE, m_cggyrodegree);
	DDX_Text(pDX, IDC_EDITLINEDEGREE, m_linedegree);
	DDX_Text(pDX, IDC_EDITLINENUMBER, m_linecrossy);
	DDX_Text(pDX, IDC_EDITCENTERX, m_centerx);
	DDX_Text(pDX, IDC_EDITCENTERY, m_centery);
	DDX_Text(pDX, IDC_EDITSCANNUMBER, m_captionnumber);
	DDX_Text(pDX, IDC_EDITLOCATION, m_locationstart);
	DDX_Text(pDX, IDC_EDITNEXTLOCATION, m_locationnext);
	DDX_Text(pDX, IDC_EDITGOLDLOCATION, m_locationgold);
	DDX_Text(pDX, IDC_EDIT_P, m_PCtrl);
	DDX_Text(pDX, IDC_EDIT_I, m_ICtrl);
	DDX_Text(pDX, IDC_EDIT_D, m_DCtrl);
	DDX_Text(pDX, IDC_EDIT_UP, m_UpStatus);
	DDX_Text(pDX, IDC_EDIT_Down, m_DownStatus);
	DDX_Text(pDX, IDC_EDITLINEBOTTON, m_linebuttonlocation);
	DDX_Text(pDX, IDC_EDITMODE, m_mymode);
	DDX_Text(pDX, IDC_EDITMODEEVERY, m_mymodeevery);
}

BEGIN_MESSAGE_MAP(CautocarDlg, CDialog)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_RECV_SERIAL_DATA, &CautocarDlg::OnRecvSerialData)
ON_BN_CLICKED(IDC_BTOPEN, &CautocarDlg::OnBnClickedBtopen)
ON_BN_CLICKED(IDC_BTCLOSE, &CautocarDlg::OnBnClickedBtclose)
ON_BN_CLICKED(IDC_BTSEND, &CautocarDlg::OnBnClickedBtsend)

ON_BN_CLICKED(IDC_BTSPEEDUP, &CautocarDlg::OnBnClickedBtspeedup)
ON_BN_CLICKED(IDC_BTSPEEDDOWN, &CautocarDlg::OnBnClickedBtspeeddown)
ON_BN_CLICKED(IDC_BTLEFT, &CautocarDlg::OnBnClickedBtleft)
ON_BN_CLICKED(IDC_BTRIGHT, &CautocarDlg::OnBnClickedBtright)
ON_BN_CLICKED(IDC_BTCLEARCLOCK, &CautocarDlg::OnBnClickedBtclearclock)
ON_WM_TIMER()
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BTOPENVIDEO, &CautocarDlg::OnBnClickedBtopenvideo)
ON_BN_CLICKED(IDC_BTCLOSEVIDEO, &CautocarDlg::OnBnClickedBtclosevideo)
ON_BN_CLICKED(IDC_BTAUTODRIVE, &CautocarDlg::OnBnClickedBtautodrive)
ON_BN_CLICKED(IDC_BTSTOPDRIVE, &CautocarDlg::OnBnClickedBtstopdrive)
ON_BN_CLICKED(IDC_BTAUTO12, &CautocarDlg::OnBnClickedBtauto12)
ON_BN_CLICKED(IDC_BTAUTO23, &CautocarDlg::OnBnClickedBtauto23)
ON_BN_CLICKED(IDC_BTAUTO34, &CautocarDlg::OnBnClickedBtauto34)
ON_BN_CLICKED(IDC_BTAUTO35, &CautocarDlg::OnBnClickedBtauto35)
ON_BN_CLICKED(IDC_BTAUTO36, &CautocarDlg::OnBnClickedBtauto36)
ON_BN_CLICKED(IDC_BTAUTO45, &CautocarDlg::OnBnClickedBtauto45)
ON_BN_CLICKED(IDC_BTAUTO46, &CautocarDlg::OnBnClickedBtauto46)
ON_BN_CLICKED(IDC_BTAUTO57, &CautocarDlg::OnBnClickedBtauto57)
ON_BN_CLICKED(IDC_BTAUTO58, &CautocarDlg::OnBnClickedBtauto58)
ON_BN_CLICKED(IDC_BTAUTO67, &CautocarDlg::OnBnClickedBtauto67)
ON_BN_CLICKED(IDC_BTAUTO68, &CautocarDlg::OnBnClickedBtauto68)
ON_BN_CLICKED(IDC_BTAUTO71, &CautocarDlg::OnBnClickedBtauto71)
ON_BN_CLICKED(IDC_BTAUTO81, &CautocarDlg::OnBnClickedBtauto81)
ON_BN_CLICKED(IDC_BTSCANNUMBER, &CautocarDlg::OnBnClickedBtscannumber)

ON_EN_CHANGE(IDC_EDITGYRO, &CautocarDlg::OnEnChangeEditgyro)
ON_EN_CHANGE(IDC_EDITSCANNUMBER, &CautocarDlg::OnEnChangeEditscannumber)
ON_EN_CHANGE(IDC_EDITHEAD, &CautocarDlg::OnEnChangeEdithead)
ON_BN_CLICKED(IDC_Patern12, &CautocarDlg::OnBnClickedPatern12)
END_MESSAGE_MAP()

// CautocarDlg 消息处理程序
//初始化
BOOL CautocarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);	// 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码
	// TODO: Add extra initialization here
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);
	pDC = pwnd->GetDC();
	//pDC =GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	pwnd1 = GetDlgItem(IDC_ShowImage1);
	//pwnd->MoveWindow(35,30,352,288);
	pDC1 = pwnd1->GetDC();
	//pDC =GetDC();
	hDC1 = pDC1->GetSafeHdc();
	pwnd1->GetClientRect(&rect1);

	pwnd2 = GetDlgItem(IDC_ShowImage3);
	//pwnd->MoveWindow(35,30,352,288);
	pDC2 = pwnd2->GetDC();
	//pDC =GetDC();
	hDC2 = pDC2->GetSafeHdc();
	pwnd2->GetClientRect(&rect2);

	pwnd3 = GetDlgItem(IDC_ShowImage5);
	//pwnd->MoveWindow(35,30,352,288);
	pDC3 = pwnd3->GetDC();
	//pDC =GetDC();
	hDC3 = pDC3->GetSafeHdc();
	pwnd3->GetClientRect(&rect3);

	//启动串口
	OnBnClickedBtopen();
	//启动摄像头
	//OnBnClickedBtopenvideo();
	//启动自动发车
	SetTimer(3, 2000, NULL);

	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void CautocarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CautocarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CautocarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//打开串口
void CautocarDlg::OnBnClickedBtopen()
{
	//串口初始化
	DCB dcb;
	int nPort;
	nPort = 2;
	//如果打开过串口啥都不做
	if (!this->m_ComPort.IsOpen())
	{
		if (!this->m_ComPort.Open(nPort, LsComm::CComPort::AutoReceiveBySignal))
		{

			CString sMsg;
			sMsg.Format("提示:不能打开串口%d!", nPort);
			AfxMessageBox(sMsg, MB_ICONINFORMATION | MB_OK);
		}
		this->m_ComPort.GetSerialPort()->GetState(dcb);
		dcb.BaudRate = 115200;

		this->m_ComPort.GetSerialPort()->SetState(dcb);
		this->m_ComPort.SetReceiveFunc((FOnReceiveData)OnReceiveData, this);
		this->m_ComPort.SetBreakHandleFunc(OnComBreak);
	}
}

void CautocarDlg::OnBnClickedBtclose()
{
	m_ComPort.Close();
}

// 串口接收数据处理函数
//ffff ffff 左电机 右电机 坡度 颜色8 000d 000a
LONG CautocarDlg::OnRecvSerialData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);
	CString strOldRecv = _T("");
	CString strRecv = _T("");
	//串口接收到的BUF
	CHAR *pBuf = (CHAR *)wParam;
	//串口接收到的BUF长度
	DWORD dwBufLen = lParam;

	//长度判断，太长丢弃
	if (dwBufLen == 5)
	{
		CString c;
		byte revbuf[5] = "\0";
		memcpy(revbuf, pBuf, dwBufLen);
		int mybuff;
		if (revbuf[0] == 0xFF && revbuf[4] == 0xAA)
		{
			Debug::DebugRob::UpdateReceiveTag();
		}
	}
	else if(dwBufLen >= 10)
	{
		Reset();
		OnBnClickedBtclosevideo();

		if (!capture)
		{
			capture = cvCaptureFromCAM(0);
		}
		if (!capture)
		{
			AfxMessageBox("无法打开摄像头");
			return 0;
		}
		m_Frame = cvQueryFrame(capture);
		m_CvvImage.CopyOf(m_Frame, 1);
		m_CvvImage.DrawToHDC(hDC, &rect);
		SetTimer(1, 50, NULL);
	}

	//释放内存
	delete[] pBuf;
	pBuf = NULL;

	UpdateData(false);
	return 0;
}

//回调函数
void OnReceiveData(LPVOID pSender, void *pBuf, DWORD InBufferCount)
{
	BYTE *pRecvBuf = NULL; //接收缓冲区
	//得到父对象指针
	CautocarDlg *pThis = (CautocarDlg *)pSender;
	//将接收的缓冲区拷贝到pRecvBuf种
	pRecvBuf = new BYTE[InBufferCount];
	CopyMemory(pRecvBuf, pBuf, InBufferCount);

	//发送异步消息，表示收到串口数据，消息处理完，应释放内存
	pThis->PostMessage(WM_RECV_SERIAL_DATA, WPARAM(pRecvBuf), InBufferCount);
}

// CheatingfactoryDlg 对话框
void OnComBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat)
{
	//deal with the break of com here
	switch (dwMask)
	{
	case EV_BREAK:
	{

		break;
	}
	case EV_CTS:
	{
		break;
	}
	case EV_DSR:
	{
		break;
	}
	case EV_ERR:
	{
		break;
	}
	case EV_RING:
	{
		break;
	}
	case EV_RLSD:
	{
		break;
	}
	case EV_RXCHAR:
	{
		break;
	}
	case EV_RXFLAG:
	{
		break;
	}
	case EV_TXEMPTY:
	{
		break;
	}
	default:
	{
	}
	}
}
//发送数据
void CautocarDlg::OnBnClickedBtsend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	int mybuff;
	sendData[0] = 0x7e;
	sendData[1] = 0x7e;
	sendData[2] = 0x7e;
	sendData[3] = 0x7e;
	mybuff = _ttoi(m_pwmleft);
	sendData[4] = mybuff / 256;
	sendData[5] = mybuff % 256;
	mybuff = _ttoi(m_pwmright);
	sendData[6] = mybuff / 256;
	sendData[7] = mybuff % 256;
	mybuff = _ttoi(m_djhead);
	sendData[8] = mybuff / 256;
	sendData[9] = mybuff % 256;
	mybuff = _ttoi(m_djlefthand);
	sendData[10] = mybuff / 256;
	sendData[11] = mybuff % 256;
	mybuff = _ttoi(m_djrighthand);
	sendData[12] = mybuff / 256;
	sendData[13] = mybuff % 256;
	mybuff = _ttoi(m_cggyro);
	sendData[14] = mybuff / 256;
	sendData[15] = mybuff % 256;

	sendData[16] = 0x0d;
	sendData[17] = 0x0a;

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(sendData, sizeof(sendData));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x"), sendData[0], sendData[1], sendData[2], sendData[3], sendData[4], sendData[5], sendData[6], sendData[7], sendData[8], sendData[9], sendData[10], sendData[11], sendData[12], sendData[13], sendData[14], sendData[15], sendData[16], sendData[17]);
	}

	m_mymode.Format(_T("%d"), m_modeevery);
	UpdateData(false);
}

//按键处理上下左右
BOOL CautocarDlg::PreTranslateMessage(MSG *pMSG)
{
	if (pMSG->message == WM_KEYDOWN) // If a keydown message
	{

		UpdateData(true);
		switch (pMSG->wParam)
		{
		case VK_UP:
			m_speedpwm = _ttoi(m_pwnlimit) + 500;
			m_pwmleft.Format(_T("%d"), m_speedpwm - m_tunerpwm);
			m_pwmright.Format(_T("%d"), m_speedpwm + m_tunerpwm);
			UpdateData(false);
			// OnBnClickedBtsend();
			break;
		case VK_DOWN:
			m_speedpwm = 500 - _ttoi(m_pwnlimit);
			m_pwmleft.Format(_T("%d"), m_speedpwm - m_tunerpwm);
			m_pwmright.Format(_T("%d"), m_speedpwm + m_tunerpwm);
			UpdateData(false);
			// OnBnClickedBtsend();
			break;
		case VK_LEFT:
			m_tunerpwm += 30;
			m_speedpwm = 500;
			m_pwmleft.Format(_T("%d"), m_speedpwm - m_tunerpwm);
			m_pwmright.Format(_T("%d"), m_speedpwm + m_tunerpwm);
			UpdateData(false);
			//OnBnClickedBtsend();
			break;
		case VK_RIGHT:
			m_tunerpwm -= 30;
			m_speedpwm = 500;
			m_pwmleft.Format(_T("%d"), m_speedpwm - m_tunerpwm);
			m_pwmright.Format(_T("%d"), m_speedpwm + m_tunerpwm);
			UpdateData(false);
			//OnBnClickedBtsend();
			break;
		default:
			break;
		}
	}
	if (pMSG->message == WM_KEYUP) // If a keydown message
	{

		UpdateData(true);
		m_pwmleft = "500";
		m_pwmright = "500";
		m_tunerpwm = 0;
		m_speedpwm = 0;
		UpdateData(false);
	}

	return ::CDialog::PreTranslateMessage(pMSG);
}

//加速
void CautocarDlg::OnBnClickedBtspeedup()
{
	UpdateData(true);
	//每次加50
	int m_speedlimitup = _ttoi(m_pwnlimit) + 10;
	//限制最大值
	if (m_speedlimitup > 900)
		m_speedlimitup = 900;
	m_pwnlimit.Format(_T("%d"), m_speedlimitup);
	UpdateData(false);
}

//减速
void CautocarDlg::OnBnClickedBtspeeddown()
{
	UpdateData(true);
	//每次减50
	int m_speedlimitup = _ttoi(m_pwnlimit) - 10;
	//限制最下值
	if (m_speedlimitup < 0)
		m_speedlimitup = 0;
	m_pwnlimit.Format(_T("%d"), m_speedlimitup);
	UpdateData(false);
}
//左转
void CautocarDlg::OnBnClickedBtleft()
{
	// TODO: 在此添加控件通知处理程序代码
	m_speedpwm = 100;
	m_tunerstart = 1;
	m_cggyro = '1';
	m_tunerpwm = 250;
	m_pwmleft.Format(_T("%d"), m_speedpwm - m_tunerpwm);
	m_pwmright.Format(_T("%d"), m_speedpwm + m_tunerpwm);
	m_stopmagnetic = _ttoi(m_cgmagnetic) + _ttoi(m_cggyrodegree) - 18;
	if (m_stopmagnetic > 360)
		m_stopmagnetic -= 360;
	UpdateData(false);
}
//右转
void CautocarDlg::OnBnClickedBtright()
{
	m_speedpwm = 1000;
	m_tunerstart = 2;
	m_tunerpwm = -250;
	m_cggyro = '1';
	m_pwmleft.Format(_T("%d"), m_speedpwm - m_tunerpwm);
	m_pwmright.Format(_T("%d"), m_speedpwm + m_tunerpwm);
	m_stopmagnetic = _ttoi(m_cgmagnetic) - (_ttoi(m_cggyrodegree) - 18);
	if (m_stopmagnetic < 0)
		m_stopmagnetic += 360;
	UpdateData(false);
}
//旋转到达角度判断
void CautocarDlg::Stoptuner(void)
{
	if (m_tunerstart == 1)
	{
		if (_ttoi(m_cgmagnetic) > m_stopmagnetic || _ttoi(m_cgmagnetic) > 350)
		{
			m_pwmleft = "500";
			m_pwmright = "500";
			m_tunerpwm = 0;
			m_speedpwm = 100;
			m_cggyro = '0';
			UpdateData(false);
			m_tunerstart = 0;
		}
	}
	if (m_tunerstart == 2)
	{
		if (_ttoi(m_cgmagnetic) < m_stopmagnetic || _ttoi(m_cgmagnetic) < 10)
		{
			m_pwmleft = "500";
			m_pwmright = "500";
			m_tunerpwm = 0;
			m_speedpwm = 100;
			m_cggyro = '0';
			UpdateData(false);
			m_tunerstart = 0;
		}
	}
}

void CautocarDlg::OnBnClickedBtclearclock()
{
	UpdateData(true);
	m_cgclockleftadd = (_T(""));

	m_cgclockrightadd = (_T(""));
	UpdateData(false);
}
//定时获取摄像头信息，同时对图像做处理
void CautocarDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int count = 5;
	if (nIDEvent == 1)
	{
		UpdateData(true);
		// TODO: Add your message handler code here and/or call default
		/************************************************************************/
		/* 显示摄像头                                                           */
		/************************************************************************/
		//显示到第2个图像
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);
		::Sleep(20);
		m_Frame = cvQueryFrame(capture);

		m_Frame = cvQueryFrame(capture);
		//显示到第一个图像
		m_CvvImage.CopyOf(m_Frame, 1);
		m_CvvImage.DrawToHDC(hDC, &rect);
		// 获取当前帧的灰度图

		IplImage *frame_1;
		IplImage *frame_2;
		IplImage *frame_3;
		IplImage *frame_4;
		IplImage *frame_color;

		frame_1 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
		frame_2 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
		frame_3 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
		frame_4 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
		frame_color = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 3);
		//cvSplit(m_Frame, frame_1, frame_2, frame_4, 0);//分离RGB通道，分别得到BGR的单通道图像

		cvCvtColor(m_Frame, frame_color, CV_BGR2HSV);				//得到HSV图
		cvSplit(frame_color, frame_3, frame_1, frame_2, 0); //分离hsv通道，分别得到hsv的单通道图像
		// Start 开始部分
		// 开机sign=0，如果平均色度大于200，,红色，sign=1，开始跑
		if (sign == 0 || sign == 1000)
		//if(1)
		{ //判断开机
			CvScalar myaverage = cvAvg(frame_color);
			m_linedegree.Format("%f:%f:%f", myaverage.val[0], myaverage.val[1], myaverage.val[2]);
			if ( myaverage.val[1] < 20)
			{
				sign = 1000; //准备状态
			}
			else if (sign == 1000 && myaverage.val[1] > 40)
			{
				sign = 1;
				Debug::DebugRob::UpdateReceiveTag();
				KillTimer(1);
				OnBnClickedBtclosevideo();
				OnBnClickedBtautodrive();
			}
		}
		UpdateData(false);
	}
	if (nIDEvent == 2)
	{
		if (ReceiveCommand())
		{
			OnBnClickedPatern12();
		}
	}

	if (nIDEvent == 3)
	{
		OnBnClickedBtautodrive();
		KillTimer(3);
	}

	CDialog::OnTimer(nIDEvent);
}

void CautocarDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnBnClickedBtclose();
	OnBnClickedBtclosevideo();
	CDialog::OnClose();
}
//打开摄像头
void CautocarDlg::OnBnClickedBtopenvideo()
{
	//摄像头1
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
	}
	if (!capture)
	{
		AfxMessageBox("无法打开摄像头");
		return;
	}
	m_Frame = cvQueryFrame(capture);
	m_CvvImage.CopyOf(m_Frame, 1);
	m_CvvImage.DrawToHDC(hDC, &rect);
	SetTimer(1, 50, NULL);
	/*
	//摄像头2
	if(!capture1)
	{
		capture1 = cvCaptureFromCAM(1);
	}
	if (!capture1)
	{
		AfxMessageBox("无法打开摄像头");
		return;
	}
	m_Frame=cvQueryFrame(capture1);
	m_CvvImage.CopyOf(m_Frame,1);	
	m_CvvImage.DrawToHDC(hDC3, &rect3);
*/
	// 设置计时器,每10ms触发一次事件
	
}
//关闭摄像头
void CautocarDlg::OnBnClickedBtclosevideo()
{

	cvReleaseCapture(&capture);
	cvReleaseCapture(&capture1);
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	pDC1->StretchBlt(rect1.left, rect1.top, rect1.Width(), rect1.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	pDC2->StretchBlt(rect2.left, rect2.top, rect2.Width(), rect2.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	pDC3->StretchBlt(rect3.left, rect3.top, rect3.Width(), rect3.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	KillTimer(1);
}

void CautocarDlg::OnBnClickedBtautodrive()
{
	UpdateData(true);
	m_modeall = 10; //大模式自动开机
	SetTimer(2, 50, NULL);
	UpdateData(false);
}
void CautocarDlg::Btauto10()
{
	UpdateData(true);
	if (m_modeall == 11)
	{
		//启动成功转换开机项
		OnBnClickedBtauto12();
	}
	m_mymodeevery.Format(_T("%d-%d"), m_modeall, m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtstopdrive()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pwmleft = "500";
	m_pwmright = "500";
	m_cggyro = "0";
	m_cggyrodegree = "180";
	UpdateData(false);
	KillTimer(2);
}
void CautocarDlg::OnBnClickedBtauto12()
{
	UpdateData(true);

	unsigned char serialSendDataArray[7] = {0xF0, 0x05, 0x07, 0x00, 0x00,0x0A, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}

void CautocarDlg::Btauto12()
{
	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, LeftEncoder = 0, RightEncoder = 0;
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation); //底部位置
	if (::abs(NewMidLine - LastMidLine) > 30)
		NewMidLine = LastMidLine;

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	LeftEncoder = _ttoi(m_cgclockleft);
	RightEncoder = _ttoi(m_cgclockright);
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 400; //跑的距离

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = NewMidLine * 0.5 + topx * 0.5;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = 0;

	LastMidLine = NewMidLine;

	if (Length > 12 && m_modeevery == 0)
	{

		//DirCtrlFlag=1;// wo gaile yige 0
		m_modeevery = 1;
	}

	if (Length > 15 && m_modeevery == 1)
	{
		m_pwnlimit = "10";
		m_modeevery = 2;
	}
	if (Length > 35 && m_modeevery == 2) //35
	{

		DirCtrlFlag = 1;
		m_pwnlimit = "30";
		m_modeevery = 3;
	}

	if (Length > 200 && m_modeevery == 3)
	{
		m_pwnlimit = "30";
		m_modeevery = 4;
	}
	if (Length > 250)
	{
		if (_ttoi(m_cgdegree) == 1 && m_modeevery == 4)
		{
			m_modeevery = 5;
			DirCtrlFlag = 0;
			m_pwnlimit = "45";
		}
		if (_ttoi(m_cgdegree) == 0 && m_modeevery == 5)
		{
			m_modeevery = 6;
			m_pwnlimit = "25";
		}

		if ((_ttoi(m_cgrgb1) + _ttoi(m_cgrgb2)) < 16300 && m_modeevery == 6) // 2 hao pingtai  huangse jiance  tingzhi   9000
		{
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
			m_modeevery = 7;
		}
	}

	if (m_modeevery == 10)
	{
		m_modeeverytime--;

		if (m_modeeverytime <= 0)
		{
			OnBnClickedBtauto23();
		} // lianjie  23  luxian

		else if (m_modeeverytime <= 10)
		{
			m_djhead = "300";
		}
		else if (m_modeeverytime <= 20)
		{
			m_djhead = "410";
		}
		else if (m_modeeverytime <= 30)
		{
			m_djhead = "190";
		}
		else
		{
			m_djhead = "410";
		}
	}
	if (m_modeevery == 9)
	{
		//转换23
		m_pwnlimit = "0";
		DirCtrlFlag = 0;
		m_modeevery = 10;
		m_modeeverytime = 40;
		m_djhead = "410";
	}
	if (m_modeevery == 8)
	{
		//转身
		m_cggyro = "0";
		m_modeevery = 9;
	}
	if (m_modeevery == 7)
	{
		//转身
		m_cggyro = "3";
		m_modeeverytime++;
		if (m_modeeverytime >= 5)
			m_modeevery = 8;
	}

	//速度环控制
	if (m_modeevery == 2)
	{
		int m_speederror = _ttoi(m_pwnlimit) + PID_Speed_Controlleft((_ttoi(m_pwnlimit) - (LeftEncoder + RightEncoder) / 10));
		if (m_speederror > _ttoi(m_pwnlimit) * 2)
			m_speedpwm = _ttoi(m_pwnlimit) * 2;
		if (m_speederror < 0)
			m_speedpwm = 1;
		m_speedpwm += 500;
	}
	else
		m_speedpwm = _ttoi(m_pwnlimit) + 500;

	//m_speedpwm=_ttoi(m_pwnlimit)+500;
	//输出PWM
	if (DirControlPWM >= 200)
		DirControlPWM = 200;
	if (DirControlPWM <= -200)
		DirControlPWM = -200;

	if (DirControlPWM < 0)
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
		m_pwmright.Format(_T("%d"), m_speedpwm);
	}
	else
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm);
		m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);
	}

	m_mymodeevery.Format(_T("%d-%d"), m_modeall, m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto23()
{
	UpdateData(true);

	unsigned char serialSendDataArray[6] = {0xF0, 0x04, 0x06, 0x00, 0x5A, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}

void CautocarDlg::Btauto23()
{
	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, topxy = 480, LeftEncoder = 0, RightEncoder = 0; //jia le xy shizi
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation); //底部位置
	if (::abs(NewMidLine - LastMidLine) > 30)
		NewMidLine = LastMidLine;

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	topxy = _ttoi(m_linecrossy);

	LeftEncoder = _ttoi(m_cgclockleft);
	RightEncoder = _ttoi(m_cgclockright);
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 400; //跑的距离

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = NewMidLine * 0.5 + topx * 0.5;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = 0;

	LastMidLine = NewMidLine;

	static int x = 0;

	if (Length > 15 && m_modeevery == 0)
	{

		//DirCtrlFlag=1;// wo gaile yige 0
		m_modeevery = 1;
	}

	if (Length > 18 && m_modeevery == 1)
	{
		m_pwnlimit = "15";
		m_modeevery = 2;
	}
	if (m_modeevery == 3)
	{
		m_pwnlimit = "28";
		m_modeevery = 4;
	}

	if (Length > 35 && m_modeevery == 2) //35
	{

		DirCtrlFlag = 1;
		m_pwnlimit = "25";
		m_modeevery = 3;
	}

	if (Length > 70)
	{

		if (topxy < 470 && m_modeevery == 4) // shi zi san ci jian ce
		{
			m_pwnlimit = "8";
			DirCtrlFlag = 1;
			if (topxy < 200)
			{
				m_pwnlimit = "6";
				DirCtrlFlag = 1;
				if (topxy < 100) //110
				{
					m_pwnlimit = "4";
					x = Length;
					m_modeevery = 5;
				}
			}
		}
		if ((Length - x) > 15 && m_modeevery == 5) //15
		{
			m_modeevery = 6;
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
		}
	}

	if (m_modeevery == 19)
	{
		m_pwnlimit = "0";
		DirCtrlFlag = 0;
		m_modeeverytime--;

		if (m_modeeverytime > 8)
		{
			OnBnClickedBtscannumber();
		}

		if (_ttoi(m_locationgold) == 5)
		{
			m_djrighthand = "420";
			if (m_modeeverytime <= 0)
			{
				OnBnClickedBtauto35();
			}
		}
		else if (_ttoi(m_locationgold) == 6)
		{
			m_djrighthand = "420";
			if (m_modeeverytime <= 0)
			{
				OnBnClickedBtauto35();
			}
		}
		else
			OnBnClickedBtauto35();

		//OnBnClickedBtauto34();
	}

	if ((Length - x) > 200)
	{
		if (_ttoi(m_cgdegree) == 1 && m_modeevery == 16)
		{
			m_modeevery = 17;
			DirCtrlFlag = 0;
			m_pwnlimit = "50";
		}
		if (_ttoi(m_cgdegree) == 0 && m_modeevery == 17)
		{
			m_modeevery = 18;
			m_pwnlimit = "35";
		}

		if ((_ttoi(m_cgrgb1) + _ttoi(m_cgrgb2)) < 6000 && m_modeevery == 18)
		{
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
			m_modeeverytime = 10; //次数
			m_modeevery = 19;
		}
	}

	if (m_modeevery == 15)
	{
		DirCtrlFlag = 1;
		m_pwnlimit = "20";
		m_modeevery = 16;
		x = Length;
	}
	if (m_modeevery == 14)
	{
		//qiaoqiaoban转23
		m_cggyro = "0";
		DirCtrlFlag = 1;
		m_pwnlimit = "0";
		m_modeevery = 15;
	}
	if (m_modeevery == 13 && _ttoi(m_cgdegree) == 0)
	{
		//转身90du
		m_cggyro = "2";
		m_modeeverytime++;
		if (m_modeeverytime >= 4)
			m_modeevery = 14;
		DirCtrlFlag = 1;
	}
	if (m_modeevery == 12 && (_ttoi(m_cgdegree) == 2 || _ttoi(m_cgdegree) == 0))
	{
		//转身

		m_modeeverytime++;
		if (m_modeeverytime >= 3)
		{
			m_modeevery = 13;
			m_pwnlimit = "0";
			x = Length;
		}
	}

	if (m_modeevery == 11 && (Length - x) > 40) // shang po  xingshi juli
	{

		DirCtrlFlag = 0;
		m_pwnlimit = "0";
		m_modeevery = 12;
	}

	if (m_modeevery == 10 && _ttoi(m_cgdegree) == 0) //  chong po   liqi   shidi  yinggai  buyong na me da
	{
		//qiaoqiaoban转23
		DirCtrlFlag = 1;
		m_pwnlimit = "38";
		m_modeevery = 11;
		x = Length;
	}
	if (m_modeevery == 9 && _ttoi(m_cgdegree) == 1)
	{
		//qiaoqiaoban转23
		m_modeeverytime++;
		if (m_modeeverytime >= 3)
		{
			DirCtrlFlag = 1;
			m_pwnlimit = "28";
			m_modeevery = 10;
		}
	}

	if (m_modeevery == 8 && (Length - x) > 15)
	{
		//qiaoqiaoban转23
		DirCtrlFlag = 1;
		m_pwnlimit = "45";
		m_modeevery = 9;
	}
	if (m_modeevery == 7)
	{
		//转身90du
		x = Length;
		m_cggyro = "0";
		DirCtrlFlag = 1;
		m_modeevery = 8;
		m_pwnlimit = "5";
	}
	if (m_modeevery == 6)
	{
		//转身
		m_cggyro = "1";
		m_modeeverytime++;
		if (m_modeeverytime >= 3) //  zhuan wan  ci shu  fa song
			m_modeevery = 7;
		m_pwnlimit = "0";
	}

	//速度环控制
	if (m_modeevery == 2)
	{
		int m_speederror = _ttoi(m_pwnlimit) + PID_Speed_Controlleft((_ttoi(m_pwnlimit) - (LeftEncoder + RightEncoder) / 10));
		if (m_speederror > _ttoi(m_pwnlimit) * 2)
			m_speedpwm = _ttoi(m_pwnlimit) * 1; //  yuan shi zhi  2
		if (m_speederror < 0)
			m_speedpwm = 1;
		m_speedpwm += 500;
	}
	else
		m_speedpwm = _ttoi(m_pwnlimit) + 500;

	//m_speedpwm=_ttoi(m_pwnlimit)+500;
	//输出PWM
	if (DirControlPWM >= 200)
		DirControlPWM = 100; //  yuanshizhi  200
	if (DirControlPWM <= -200)
		DirControlPWM = -100; // yuan shizhi 200

	if (DirControlPWM < 0)
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
		m_pwmright.Format(_T("%d"), m_speedpwm);
	}
	else
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm);
		m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);
	}

	m_mymodeevery.Format(_T("%d"), m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto34()
{
	UpdateData(true);

	unsigned char serialSendDataArray[6] = {0xF0, 0x04, 0x06, 0xFF, 0xA6, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto34()
{

	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, LeftEncoder = 0, RightEncoder = 0;
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation); //底部位置
	if (::abs(NewMidLine - LastMidLine) > 30)
		NewMidLine = LastMidLine;

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	LeftEncoder = _ttoi(m_cgclockleft);
	RightEncoder = _ttoi(m_cgclockright);
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 400; //跑的距离

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = NewMidLine * 0.5 + topx * 0.5;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = 0;

	LastMidLine = NewMidLine;

	if (m_modeevery == 2)
	{
		//转换23
		m_pwnlimit = "20";
		DirCtrlFlag = 0;
		m_modeevery = 3;
	}
	if (m_modeevery == 1)
	{
		//转身
		m_cggyro = "0";
		m_modeevery = 2;
	}
	if (m_modeevery == 0)
	{
		//转身
		// m_pwnlimit="0";
		m_cggyro = "3";
		m_modeeverytime++;
		if (m_modeeverytime >= 6)
			m_modeevery = 1;
	}

	if (Length > 12 && m_modeevery == 3)
	{

		//DirCtrlFlag=1;// wo gaile yige 0
		m_modeevery = 4;
	}

	if (Length > 15 && m_modeevery == 4)
	{
		m_pwnlimit = "10";
		m_modeevery = 5;
	}
	if (Length > 35 && m_modeevery == 5) //35
	{

		DirCtrlFlag = 1;
		m_pwnlimit = "30";
		m_modeevery = 6;
	}

	if (Length > 200 && m_modeevery == 6)
	{
		m_pwnlimit = "70";
		m_modeevery = 7;
	}
	if (Length > 280)
	{
		if (_ttoi(m_cgdegree) == 1 && m_modeevery == 7)
		{
			m_modeevery = 8;
			DirCtrlFlag = 0;
			m_pwnlimit = "40";
		}
		if (_ttoi(m_cgdegree) == 0 && m_modeevery == 8)
		{
			m_modeevery = 9;
			m_pwnlimit = "30";
		}

		if ((_ttoi(m_cgrgb1) + _ttoi(m_cgrgb2)) < 20000 && m_modeevery == 9)
		{
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
			m_modeevery = 10;
		}
	}

	if (m_modeevery == 10)
	{
	}

	//速度环控制
	if (m_modeevery == 2)
	{
		int m_speederror = _ttoi(m_pwnlimit) + PID_Speed_Controlleft((_ttoi(m_pwnlimit) - (LeftEncoder + RightEncoder) / 10));
		if (m_speederror > _ttoi(m_pwnlimit) * 2)
			m_speedpwm = _ttoi(m_pwnlimit) * 2;
		if (m_speederror < 0)
			m_speedpwm = 1;
		m_speedpwm += 500;
	}
	else
		m_speedpwm = _ttoi(m_pwnlimit) + 500;

	//m_speedpwm=_ttoi(m_pwnlimit)+500;
	//输出PWM
	if (DirControlPWM >= 200)
		DirControlPWM = 200;
	if (DirControlPWM <= -200)
		DirControlPWM = -200;

	if (DirControlPWM < 0)
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
		m_pwmright.Format(_T("%d"), m_speedpwm);
	}
	else
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm);
		m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);
	}

	m_mymodeevery.Format(_T("%d"), m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto35()
{
	UpdateData(true);

	unsigned char serialSendDataArray[7] = {0xF0, 0x05, 0x06, 0x00, 0x9D, 0x00, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto35()
{

	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, LeftEncoder = 0, RightEncoder = 0;
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation); //底部位置
	if (::abs(NewMidLine - LastMidLine) > 30)
		NewMidLine = LastMidLine;

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	LeftEncoder = _ttoi(m_cgclockleft);
	RightEncoder = _ttoi(m_cgclockright);
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 400; //跑的距离

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = NewMidLine * 0.5 + topx * 0.5;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = 0;

	LastMidLine = NewMidLine;

	if (m_modeevery == 2)
	{
		//转换23
		m_pwnlimit = "20";
		DirCtrlFlag = 0;
		m_modeevery = 3;
	}
	if (m_modeevery == 1)
	{
		//转身
		m_cggyro = "0";
		m_modeevery = 2;
	}
	if (m_modeevery == 0)
	{
		//转身
		// m_pwnlimit="0";
		m_cggyro = "3";
		m_modeeverytime++;
		if (m_modeeverytime >= 6)
			m_modeevery = 1;
		m_djrighthand = "190";
	}

	if (Length > 12 && m_modeevery == 3)
	{

		//DirCtrlFlag=1;// wo gaile yige 0
		m_modeevery = 4;
	}

	if (Length > 15 && m_modeevery == 4)
	{
		m_pwnlimit = "10";
		m_modeevery = 5;
	}
	if (Length > 35 && m_modeevery == 5) //35
	{

		DirCtrlFlag = 1;
		m_pwnlimit = "30";
		m_modeevery = 6;
	}

	if (Length > 200 && m_modeevery == 6)
	{
		m_pwnlimit = "70";
		m_modeevery = 7;
	}
	if (Length > 280)
	{
		if (_ttoi(m_cgdegree) == 1 && m_modeevery == 7)
		{
			m_modeevery = 8;
			DirCtrlFlag = 0;
			m_pwnlimit = "40";
		}
		if (_ttoi(m_cgdegree) == 0 && m_modeevery == 8)
		{
			m_modeevery = 9;
			m_pwnlimit = "30";
		}

		if ((_ttoi(m_cgrgb1) + _ttoi(m_cgrgb2)) < 15000 && m_modeevery == 9)
		{
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
			m_modeevery = 10;
			m_modeeverytime = 10;
		}
	}

	if (m_modeevery == 10)
	{
		m_pwnlimit = "0";
		DirCtrlFlag = 0;
		m_modeeverytime--;

		if (m_modeeverytime > 8)
		{
			OnBnClickedBtscannumber();
		}

		if (_ttoi(m_locationgold) == 5)
		{
			m_djrighthand = "420";
			if (m_modeeverytime <= 0)
			{
				OnBnClickedBtauto71();
			}
		}
		else if (_ttoi(m_locationgold) == 6)
		{
			m_djrighthand = "420";
			if (m_modeeverytime <= 0)
			{
				OnBnClickedBtauto71();
			}
		}
		else
			OnBnClickedBtauto35();

		//OnBnClickedBtauto34();
	}

	//速度环控制
	if (m_modeevery == 2)
	{
		int m_speederror = _ttoi(m_pwnlimit) + PID_Speed_Controlleft((_ttoi(m_pwnlimit) - (LeftEncoder + RightEncoder) / 10));
		if (m_speederror > _ttoi(m_pwnlimit) * 2)
			m_speedpwm = _ttoi(m_pwnlimit) * 2;
		if (m_speederror < 0)
			m_speedpwm = 1;
		m_speedpwm += 500;
	}
	else
		m_speedpwm = _ttoi(m_pwnlimit) + 500;

	//m_speedpwm=_ttoi(m_pwnlimit)+500;
	//输出PWM
	if (DirControlPWM >= 200)
		DirControlPWM = 200;
	if (DirControlPWM <= -200)
		DirControlPWM = -200;

	if (DirControlPWM < 0)
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
		m_pwmright.Format(_T("%d"), m_speedpwm);
	}
	else
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm);
		m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);
	}

	m_mymodeevery.Format(_T("%d"), m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto36()
{
	UpdateData(true);

	unsigned char serialSendDataArray[6] = {0xF0, 0x04, 0x06, 0x00, 0x1E, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}

void CautocarDlg::Btauto36()
{
	static int x;
	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, topxy = 480, LeftEncoder = 0, RightEncoder = 0;
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation); //底部位置
	if (::abs(NewMidLine - LastMidLine) > 30)
		NewMidLine = LastMidLine;

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	topxy = _ttoi(m_linecrossy);

	LeftEncoder = _ttoi(m_cgclockleft);
	RightEncoder = _ttoi(m_cgclockright);
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 400; //跑的距离

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = NewMidLine * 0.5 + topx * 0.5;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = 0;

	LastMidLine = NewMidLine;

	if (m_modeevery == 2)
	{
		//转换23
		m_pwnlimit = "20";
		DirCtrlFlag = 0;
		m_modeevery = 3;
	}
	if (m_modeevery == 1)
	{
		//转身
		m_cggyro = "0";
		m_modeevery = 2;
	}
	if (m_modeevery == 0)
	{
		//转身
		// m_pwnlimit="0";
		m_cggyro = "3";
		m_modeeverytime++;
		if (m_modeeverytime >= 6)
			m_modeevery = 1;
	}

	if (Length > 12 && m_modeevery == 3)
	{

		//DirCtrlFlag=1;// wo gaile yige 0
		m_modeevery = 4;
	}

	if (Length > 15 && m_modeevery == 4)
	{
		m_pwnlimit = "10";
		m_modeevery = 5;
	}
	if (Length > 35 && m_modeevery == 5) //35
	{

		DirCtrlFlag = 1;
		m_pwnlimit = "30";
		m_modeevery = 6;
	}

	if (Length > 200 && m_modeevery == 6)
	{
		m_pwnlimit = "70";
		m_modeevery = 7;
	}
	if (Length > 500 && m_modeevery == 7)
	{
		m_pwnlimit = "30";
		m_modeevery = 8;
	}
	if (Length > 700 && m_modeevery == 8 && topxy < 470)
	{
		m_pwnlimit = "20";
		m_modeevery = 9;
		x = Length;
	}

	if ((Length - x) > 10 && m_modeevery == 9)
	{
		m_modeevery = 10;
		m_pwnlimit = "0";
		DirCtrlFlag = 0;
	}

	if ((Length - x) > 120 && m_modeevery == 16)
	{
		if (_ttoi(m_cgdegree) == 1 && m_modeevery == 16)
		{
			m_modeevery = 17;
			DirCtrlFlag = 0;
			m_pwnlimit = "40";
		}
		/*	if(_ttoi(m_cgdegree)==0&&m_modeevery==17)
		{
			m_modeevery=18;
			m_pwnlimit="30";
			

		}*/

		if ((_ttoi(m_cgrgb1) + _ttoi(m_cgrgb2)) < 15000 && m_modeevery == 17)
		{
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
			m_modeevery = 19;
		}
	}

	if (m_modeevery == 15)
	{
		//转身90du

		m_cggyro = "0";
		DirCtrlFlag = 1;
		m_modeevery = 16;
		x = Length;
		m_pwnlimit = "20";
	}
	if (m_modeevery == 14 && (Length - x) > 50)
	{
		//转身
		m_cggyro = "1";
		m_modeeverytime++;
		if (m_modeeverytime >= 6)
		{
			m_modeevery = 15;
			m_pwnlimit = "0";
		}
	}

	if (m_modeevery == 13 && (Length - x) > 200 && topxy < 470)

	{
		DirCtrlFlag = 1;
		m_pwnlimit = "88";
		m_modeevery = 14;
		x = Length;
	}

	if (m_modeevery == 12)

	{
		DirCtrlFlag = 1;
		m_pwnlimit = "30";
		m_modeevery = 13;
		x = Length;
	}

	if (m_modeevery == 11)
	{
		//转身90du

		m_cggyro = "0";
		DirCtrlFlag = 1;
		m_modeevery = 12;
		m_pwnlimit = "0";
	}
	if (m_modeevery == 10)
	{
		//转身
		m_cggyro = "2";
		m_modeeverytime++;
		if (m_modeeverytime >= 6)
		{
			m_modeevery = 11;
			m_pwnlimit = "0";
		}
	}

	/*
	 if(Length > 280)
    {
		if(_ttoi(m_cgdegree)==1&&m_modeevery==7)
		{	m_modeevery=8;
			DirCtrlFlag=0;
			m_pwnlimit="40";
			
		}
		if(_ttoi(m_cgdegree)==0&&m_modeevery==8)
		{
			m_modeevery=9;
			m_pwnlimit="30";
			

		}
		
        if((_ttoi(m_cgrgb1)+_ttoi(m_cgrgb2))<20000&&m_modeevery==9)
		{
			m_pwnlimit="0";
			DirCtrlFlag=0;
			m_modeevery=10;
		}
    } 

	 if(	m_modeevery==10)
	 {
	 
	 }
*/

	//速度环控制
	if (m_modeevery == 2)
	{
		int m_speederror = _ttoi(m_pwnlimit) + PID_Speed_Controlleft((_ttoi(m_pwnlimit) - (LeftEncoder + RightEncoder) / 10));
		if (m_speederror > _ttoi(m_pwnlimit) * 2)
			m_speedpwm = _ttoi(m_pwnlimit) * 2;
		if (m_speederror < 0)
			m_speedpwm = 1;
		m_speedpwm += 500;
	}
	else
		m_speedpwm = _ttoi(m_pwnlimit) + 500;

	//m_speedpwm=_ttoi(m_pwnlimit)+500;
	//输出PWM
	if (DirControlPWM >= 200)
		DirControlPWM = 200;
	if (DirControlPWM <= -200)
		DirControlPWM = -200;

	if (DirControlPWM < 0)
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
		m_pwmright.Format(_T("%d"), m_speedpwm);
	}
	else
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm);
		m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);
	}

	m_mymodeevery.Format(_T("%d"), m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto45()
{
	UpdateData(true);

	unsigned char serialSendDataArray[6] = {0xF0, 0x04, 0x06, 0xFF, 0xE0, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto45()
{
}

void CautocarDlg::OnBnClickedBtauto46()
{
	UpdateData(true);

	unsigned char serialSendDataArray[6] = {0xF0, 0x04, 0x01, 0x00, 0x00, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto46()
{
}

void CautocarDlg::OnBnClickedBtauto57()
{
	UpdateData(true);

	unsigned char serialSendDataArray[6] = {0xF0, 0x04, 0x02, 0x00, 0x00, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4], serialSendDataArray[5]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto57()
{
}

void CautocarDlg::OnBnClickedBtauto58()
{
	UpdateData(true);

	unsigned char serialSendDataArray[5] = {0xF0, 0x03, 0x00, 0x00, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto58()
{
}

void CautocarDlg::OnBnClickedBtauto67()
{
	UpdateData(true);

	unsigned char serialSendDataArray[5] = {0xF0, 0x03, 0x00, 0x01, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto67()
{
}
void CautocarDlg::OnBnClickedBtauto68()
{
	UpdateData(true);

	unsigned char serialSendDataArray[5] = {0xF0, 0x03, 0x01, 0x00, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto68()
{
	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, LeftEncoder = 0, RightEncoder = 0;
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation);

	FILE *fp;
	fp = fopen("D:\\autocarcv\\MidLineData.txt", "a");
	if (fp == NULL)
	{
	}
	fprintf(fp, "%d\n", NewMidLine);
	fclose(fp);

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	LeftEncoder = _ttoi(m_cgclockleft) - 30000;
	RightEncoder = _ttoi(m_cgclockright) - 30000;
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 2;

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = LastMidLine;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = DirControlPWMbuff;

	LastMidLine = NewMidLine;

	m_speedpwm = _ttoi(m_pwnlimit);

	/*if(_ttoi(m_UpStatus)==0)
    {    
        m_pwnlimit="500";
        if(m_modeevery==0)
        { 
            m_modeevery=1;
            m_clockbuffleft=_ttoi(m_cgclockleftadd);
        }
        if(m_modeevery==1)
        {
            if((Length>m_clockbuffleft+60*PRATIO)&&(m_clockbuffleft+80*PRATIO)>Length)
            m_pwnlimit="0";
        }
    }

    if(_ttoi(m_DownStatus)==0)
    {    
        m_pwnlimit="50";
        if(m_modeevery==1)
        { 
            m_modeevery=2;
            m_clockbuffleft=_ttoi(m_cgclockleftadd);
        }
        if(m_modeevery==2)
        {
            if((Length>m_clockbuffleft+20*PRATIO)&&(m_clockbuffleft+50*PRATIO)>Length)
            m_pwnlimit="100";
        }
    }*/

	m_speedpwm = _ttoi(m_pwnlimit) + 1000;
	//输出PWM
	if (DirControlPWM >= 900)
		DirControlPWM = 900;
	if (DirControlPWM <= -900)
		DirControlPWM = -900;

	m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
	m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);

	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto71()
{

	UpdateData(true);

	unsigned char serialSendDataArray[5] = {0xF0, 0x03, 0x00, 0x03, 0x0A};

	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(serialSendDataArray, sizeof(serialSendDataArray));
		m_sersend.Format(_T("%2x %2x %2x %2x %2x"),
										 serialSendDataArray[0], serialSendDataArray[1], serialSendDataArray[2],
										 serialSendDataArray[3], serialSendDataArray[4]);
	}

	UpdateData(false);
}
void CautocarDlg::Btauto71()
{
	UpdateData(true);

	int NewMidLine = 0, DirControlPWM = 0;
	int topx = 0, topy = 0, topxy = 480, LeftEncoder = 0, RightEncoder = 0; //jia le xy shizi
	int Length = 0;

	NewMidLine = _ttoi(m_linebuttonlocation); //底部位置
	if (::abs(NewMidLine - LastMidLine) > 30)
		NewMidLine = LastMidLine;

	topx = _ttoi(m_centerx);
	topy = _ttoi(m_centery);
	topxy = _ttoi(m_linecrossy);

	LeftEncoder = _ttoi(m_cgclockleft);
	RightEncoder = _ttoi(m_cgclockright);
	Length = (_ttoi(m_cgclockleftadd) + _ttoi(m_cgclockrightadd)) / 400; //跑的距离

	if (topy > 400)
	{
		NewMidLine = NewMidLine * 0.7 + topx * 0.3;
	}
	//if(topy<=300){NewMidLine=320;}
	if (topy < 50)
	{
		NewMidLine = NewMidLine * 0.5 + topx * 0.5;
	}

	if (NewMidLine <= 0)
		NewMidLine = 0;
	if (NewMidLine >= 640)
		NewMidLine = 640;

	if (DirCtrlFlag)
	{
		DirControlPWM = PID_Dir_Control(SetMidLine - NewMidLine);
		DirControlPWMbuff = DirControlPWM;
	}
	else
		DirControlPWM = 0;

	LastMidLine = NewMidLine;

	static int x = 0;

	if (Length > 15 && m_modeevery == 0)
	{

		//DirCtrlFlag=1;// wo gaile yige 0
		m_modeevery = 1;
	}

	if (Length > 18 && m_modeevery == 1)
	{
		m_pwnlimit = "15";
		m_modeevery = 2;
	}
	if (m_modeevery == 3)
	{
		m_pwnlimit = "28";
		m_modeevery = 4;
	}

	if (Length > 35 && m_modeevery == 2) //35
	{

		DirCtrlFlag = 1;
		m_pwnlimit = "25";
		m_modeevery = 3;
	}

	if (Length > 250)
	{

		if (topxy < 470 && m_modeevery == 4) // shi zi san ci jian ce
		{
			m_pwnlimit = "8";
			DirCtrlFlag = 1;
			if (topxy < 200)
			{
				m_pwnlimit = "6";
				DirCtrlFlag = 1;
				if (topxy < 110)
				{
					m_pwnlimit = "4";
					x = Length;
					m_modeevery = 5;
				}
			}
		}
		if ((Length - x) > 15 && m_modeevery == 5)
		{
			m_modeevery = 6;
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
		}
	}

	if (m_modeevery == 19)
	{
		m_pwnlimit = "0";
		m_cggyro = "0";
		DirCtrlFlag = 0;
		//m_modeeverytime--;

		//if(m_modeeverytime>8){ OnBnClickedBtscannumber();}

		//if(_ttoi(m_locationgold)==5)
		//{ m_djrighthand = "420"; if(m_modeeverytime<=0){OnBnClickedBtauto35();} }
		//else if(_ttoi(m_locationgold)==6)
		//{ m_djrighthand = "420"; if(m_modeeverytime<=0){OnBnClickedBtauto35();} }
		//else
		//OnBnClickedBtauto35();

		//OnBnClickedBtauto34();
	}

	if ((Length - x) > 200)
	{
		if (_ttoi(m_cgdegree) == 1 && m_modeevery == 16)
		{
			m_modeevery = 17;
			DirCtrlFlag = 0;
			m_pwnlimit = "50";
		}
		if (_ttoi(m_cgdegree) == 0 && m_modeevery == 17)
		{
			m_modeevery = 18;
			m_pwnlimit = "35";
		}

		if ((_ttoi(m_cgrgb1) + _ttoi(m_cgrgb2)) < 17000 && m_modeevery == 18)
		{
			m_pwnlimit = "0";
			DirCtrlFlag = 0;
			m_modeeverytime = 10; //次数
			m_modeevery = 19;
		}
	}

	if (m_modeevery == 15)
	{
		DirCtrlFlag = 1;
		m_pwnlimit = "20";
		m_modeevery = 16;
		x = Length;
	}
	if (m_modeevery == 14)
	{
		//qiaoqiaoban转23
		m_cggyro = "0";
		DirCtrlFlag = 1;
		m_pwnlimit = "0";
		m_modeevery = 15;
	}
	if (m_modeevery == 13 && _ttoi(m_cgdegree) == 0)
	{
		//转身90du
		m_cggyro = "1"; //返回左转
		m_modeeverytime++;
		if (m_modeeverytime >= 4) //怕信号受不到,给了四次
			m_modeevery = 14;
		DirCtrlFlag = 1;
	}
	if (m_modeevery == 12 && (_ttoi(m_cgdegree) == 2 || _ttoi(m_cgdegree) == 0))
	{
		//转身

		m_modeeverytime++;
		if (m_modeeverytime >= 3)
		{
			m_modeevery = 13;
			m_pwnlimit = "0";
			x = Length;
		}
	}

	if (m_modeevery == 11 && (Length - x) > 45) // shang po  xingshi juli
	{

		DirCtrlFlag = 0;
		m_pwnlimit = "0";
		m_modeevery = 12;
	}

	if (m_modeevery == 10 && _ttoi(m_cgdegree) == 0) //  chong po   liqi   shidi  yinggai  buyong na me da
	{
		//qiaoqiaoban转23
		DirCtrlFlag = 1;
		m_pwnlimit = "35";
		m_modeevery = 11;
		x = Length;
	}
	if (m_modeevery == 9 && _ttoi(m_cgdegree) == 1) //cgdegree变量为下位机发上来的坡度变量
	{
		//qiaoqiaoban转23
		m_modeeverytime++;
		if (m_modeeverytime >= 3)
		{
			DirCtrlFlag = 1;
			m_pwnlimit = "28";
			m_modeevery = 10;
		}
	}

	if (m_modeevery == 8 && (Length - x) > 10) //51返回时十字交叉点到开始上坡点的距离判断 这个地方的转弯一定要做的很准
	{
		//qiaoqiaoban转23
		DirCtrlFlag = 1;
		m_pwnlimit = "45";
		m_modeevery = 9;
	}
	if (m_modeevery == 7)
	{
		//转身90du
		x = Length;
		m_cggyro = "0";
		DirCtrlFlag = 1;
		m_modeevery = 8;
		m_pwnlimit = "5";
	}
	if (m_modeevery == 6)
	{
		//转身
		m_cggyro = "2"; //返回右转
		m_modeeverytime++;
		if (m_modeeverytime >= 3) //  zhuan wan  ci shu  fa song
			m_modeevery = 7;
		m_pwnlimit = "0";
	}

	//速度环控制
	if (m_modeevery == 2)
	{
		int m_speederror = _ttoi(m_pwnlimit) + PID_Speed_Controlleft((_ttoi(m_pwnlimit) - (LeftEncoder + RightEncoder) / 10));
		if (m_speederror > _ttoi(m_pwnlimit) * 2)
			m_speedpwm = _ttoi(m_pwnlimit) * 1; //  yuan shi zhi  2
		if (m_speederror < 0)
			m_speedpwm = 1;
		m_speedpwm += 500;
	}
	else
		m_speedpwm = _ttoi(m_pwnlimit) + 500;

	//m_speedpwm=_ttoi(m_pwnlimit)+500;
	//输出PWM
	if (DirControlPWM >= 200)
		DirControlPWM = 100; //  yuanshizhi  200
	if (DirControlPWM <= -200)
		DirControlPWM = -100; // yuan shizhi 200

	if (DirControlPWM < 0)
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm - DirControlPWM);
		m_pwmright.Format(_T("%d"), m_speedpwm);
	}
	else
	{
		m_pwmleft.Format(_T("%d"), m_speedpwm);
		m_pwmright.Format(_T("%d"), m_speedpwm + DirControlPWM);
	}

	m_mymodeevery.Format(_T("%d"), m_modeevery);
	UpdateData(false);
}

void CautocarDlg::OnBnClickedBtauto81()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_modeall = 81;					 //大模式
	m_locationstart = "1";	 //起点站台序号
	m_locationnext = "2";		 //目标站台序号
	m_locationgold = "0";		 //宝藏
	m_pwmleft = "500";			 //初始化速度
	m_pwmright = "500";			 //初始化速度
	m_cgclockleftadd = "0";	//距离累加值
	m_cgclockrightadd = "0"; //距离累加值
	m_clockbuffleft = 0;
	m_pwnlimit = "20"; //速度
	DirCtrlFlag = 1;	 //允许控制方向
	// m_modeevery=0;//小模式
	m_stopcar = 0;
	UpdateData(false);

	SetTimer(2, 20, NULL);
}

void CautocarDlg::Btauto81()
{
}

void CautocarDlg::OnBnClickedBtscannumber()
{
	//关闭摄像头
	if (!capture1)
	{
		capture1 = cvCaptureFromCAM(0);
	}
	if (!capture1)
	{
		AfxMessageBox("无法打开摄像头");
		return;
	}

	//显示到第2个图像
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);
	::Sleep(20);
	m_Frame = cvQueryFrame(capture1);

	m_CvvImage.CopyOf(m_Frame, 1);
	m_CvvImage.DrawToHDC(hDC3, &rect3);

	cv::Mat src = m_Frame;
	// resize(src,src,Size(800,600));//标准大小
	cv::Mat src_gray;
	cv::Mat src_all = src.clone();
	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point>> contours, contours2, contours_out, contours_all;
	std::vector<cv::Vec4i> hierarchy, hierarchy_out, hierarchy_all;
	//预处理
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, cv::Size(3, 3)); //模糊，去除毛刺
	cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	//第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	//腐蚀操作
	dilate(src_gray, src, element);
	threshold(src, threshold_output, 100, 255, cv::THRESH_OTSU);

	m_Frame = &IplImage(threshold_output);
	m_CvvImage.CopyOf(m_Frame, 1);
	m_CvvImage.DrawToHDC(hDC2, &rect2);

	//寻找轮廓
	//第一个参数是输入图像 2值化的
	//第二个参数是内存存储器，FindContours找到的轮廓放到内存里面。
	//第三个参数是层级，**[Next, Previous, First_Child, Parent]** 的vector
	//第四个参数是类型，采用树结构
	//第五个参数是节点拟合模式，这里是全部寻找 findContours(image_contour_outside, contours_out, hierarchy_out, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	findContours(threshold_output, contours_out, hierarchy_out, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	findContours(threshold_output, contours_all, hierarchy_all, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
	cv::Mat Drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);

	if (contours_all.size() == contours_out.size())
		return; //没有内轮廓，则提前返回

	for (int i = 0; i < contours_out.size(); i++)
	{
		int conloursize = contours_out[i].size();
		for (int j = 0; j < contours_all.size(); j++)
		{
			int tem_size = contours_all[j].size();
			if (conloursize == tem_size)
			{
				swap(contours_all[j], contours_all[contours_all.size() - 1]);
				contours_all.pop_back();
				break;
			}
		}
	}

	//contours_all中只剩下内轮廓
	//查找最大轮廓
	double maxarea = 0;
	int maxAreaIdx = 0;
	for (int index = contours_all.size() - 1; index >= 0; index--)
	{
		double tmparea = fabs(contourArea(contours_all[index]));
		if (tmparea > maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = index; //记录最大轮廓的索引号
		}
	}
	cv::Scalar color = cv::Scalar(0, 0, 255);
	//绘制最大内轮廓
	drawContours(Drawing, contours_all, maxAreaIdx, color, 2, 8, hierarchy_all, 0, cv::Point());
	//轮廓长度
	double g_dConLength = arcLength(contours_all[maxAreaIdx], true);
	//轮廓面积
	double g_dConArea = contourArea(contours_all[maxAreaIdx], true);

	//m_locationgold.Format("长度%f 面积%f 比例%f", g_dConLength, g_dConArea, g_dConArea / g_dConLength);
	m_locationgold.Format("%f", g_dConArea / g_dConLength);

	m_Frame = &IplImage(Drawing);
	m_CvvImage.CopyOf(m_Frame, 1);
	m_CvvImage.DrawToHDC(hDC1, &rect1);

	//关闭摄像头
	cvReleaseCapture(&capture1);

	// OnBnClickedBtopenvideo();

	UpdateData(false);
}
Point CautocarDlg::Center_cal(vector<vector<Point>> contours, int i) //找到所提取轮廓的中心点
{
	int centerx = 0, centery = 0, n = contours[i].size();
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}

int CautocarDlg::PID_Dir_Control(int err)
{
	static float Pd = 0.2, Id = 0, Dd = 0.4;
	//Pd=0.4+((_ttoi(m_pwnlimit))/300);

	Pd = 0.5; //0.6
	float temp = 0;
	static int LastErr = 0, IntErr = 0;
	int NewErr = 0;

	NewErr = err;
	IntErr += NewErr;
	if (IntErr > 100)
		IntErr = 100;
	if (IntErr < -100)
		IntErr = -100;
	temp = NewErr * Pd + IntErr * Id + (NewErr - LastErr) * Dd;
	LastErr = NewErr;
	return (int)temp;
}

int CautocarDlg::PID_Speed_Controlleft(int err)
{
	static float P = 0.5, I = 0.5, D = 0.2;
	float temp = 0;
	static int LastErrl = 0, IntErrl = 0;
	int NewErrl = 0;

	NewErrl = err;
	IntErrl += NewErrl;
	if (IntErrl > 200)
		IntErrl = 200;
	if (IntErrl < -200)
		IntErrl = -200;
	temp = NewErrl * P + IntErrl * I + (NewErrl - LastErrl) * D;
	LastErrl = NewErrl;

	return (int)temp;
}

void CautocarDlg::stopcar(void)
{
	UpdateData(true);
	if (_ttoi(m_cgrgb1) == 1 || _ttoi(m_cgrgb2) == 1)
	{
		m_stopcar = 2;
		m_modeevery = 1000;
	}
	if (m_stopcar == 2)
	{
		m_modeeverynumber--;
		if (m_modeeverynumber == 0)
		{
			m_stopcar = 0;
			m_modeevery = m_modeeveryold;
		}
		m_pwnlimit = "0";
		m_pwmleft = "500";
		m_pwmright = "500";
		m_tunerpwm = 0;
		m_speedpwm = 1000;
		int speedstop = (_ttoi(m_cgclockleft) - 30000) * 20;
		if (speedstop > 70)
			speedstop = 70;
		if (speedstop < -70)
			speedstop = -70;
		m_pwmleft.Format(_T("%d"), m_speedpwm - speedstop);
		m_pwmright.Format(_T("%d"), m_speedpwm - speedstop);
	}
	UpdateData(false);
}

void CautocarDlg::OnEnChangeEditgyro()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CautocarDlg::OnEnChangeEditscannumber()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CautocarDlg::SendData(unsigned char arrays[], int lenth)
{
	if (this->m_ComPort.IsOpen())
	{
		this->m_ComPort.Output(arrays, lenth);
	}
}

void CautocarDlg::PointMode(PointMode_t pointMode, int16_t angle, uint8_t lineSpeed)
{
	UpdateData(true);
	uint8_t angleH = (angle & 0xFF00) >> 8, angleL = (angle & 0x00FF);
	uint8_t serialSendDataArray[7] = {0xF0, 0x05, pointMode, angleH, angleL, lineSpeed, 0x0A};

	SendData(serialSendDataArray, 7);
	UpdateData(false);
}

void CautocarDlg::LineMode(uint8_t point, uint8_t delay, uint8_t transSpeed)
{
	unsigned char serialSendDataArray[6] = {0xF0, 0x04, point, delay, transSpeed, 0x0A};

	SendData(serialSendDataArray, 6);
}

void CautocarDlg::OnEnChangeEdithead()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CautocarDlg::Reset(void)
{
	next = 2;
	step = 0;
	sign = 0;
}

void CautocarDlg::OnBnClickedPatern12()
{
	if (sign == -1 || sign == 1000)
	{
		return;
	}
	else if (sign == 1)
	{
		_1To2();
	}
	else if (sign == 2)
	{
		if (next == 3)
		{
			_2To3();
		}
		else if (next == 4)
		{
			_2To4();
		}
	}
	else if (sign == 3)
	{
		if (next == 5)
		{
			_3To5();
		}
		else if (next == 6)
		{
			_3To6();
		}
	}
	else if (sign == 4)
	{
		if (next == 5)
		{
			_4To5();
		}
		else if (next == 6)
		{
			_4To6();
		}
	}
	else if (sign == 5)
	{
		if (next == 7)
		{
			_5To7();
		}
		else if (next == 8)
		{
			_5To8();
		}
	}
	else if (sign == 6)
	{
		if (next == 7)
		{
			_6To7();
		}
		else if (next == 8)
		{
			_6To8();
		}
	}
}

int CautocarDlg::_1To2(void)
{
	switch (step++)
	{
	/* 开始 */
	case 0:
		PointMode(QUIT_, 0);
		break;

	/* 识别图像, 举手转头 */
	case 1:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 20)
		{
			next = 3;
		}
		else if (res > 20)
		{
			next = 4;
		}
		sign = 2;
		step = 0;
		PointMode(TURN_ANYANGLE, 162);
		break;
	}

	return next;
}

int CautocarDlg::_2To3(void)
{
	switch (step++)
	{
	case 0:
		PointMode(HEAD_MOVE, 0);
		break;

	/* 转180 */
	case 1:
		PointMode(HAND_UP, 0);
		break;

	/* 下一个点停，走 */
	case 2:
		LineMode(0, 0);
		PointMode(QUIT_, 0);
		break;

	/* 转角，下个点停，延后0.1s，走 */
	case 3:
		PointMode(TURN_ANYANGLE, -30);
		break;

	case 4:
		LineMode(0, 13);
		PointMode(QUIT_, 0);
		break;

	/* 转角去3，过一个点后停，走 */
	case 5:
		PointMode(TURN_ANYANGLE, -140, 12);
		break;

	case 6:
		LineMode(1, 0, 50);
		PointMode(QUIT_, 0);
		break;	

	/* 识别图像 */
	case 7:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 11)
		{
			next = 5;
		}
		else if (res > 11)
		{
			next = 6;
		}
		sign = 3;
		step = 0;

		/* 举手转头 */
		PointMode(TURN_ANYANGLE, 162);
		break;
	}

	return next;
}

int CautocarDlg::_2To4(void)
{
	switch (step++)
	{
	case 0:
		PointMode(HEAD_MOVE, 0);
		break;

	/* 转180 */
	case 1:
		PointMode(HAND_UP, 0);
		break;

	/* 下一个点停，走 */
	case 2:
		LineMode(0, 0);
		PointMode(QUIT_, 0);
		break;

	/* 转角，下个点停，延后0.1s，走 */
	case 3:
		PointMode(TURN_ANYANGLE, -30);
		break;

	case 4:
		LineMode(0, 17, 40);
		PointMode(QUIT_, 0);
		break;

	/* 转角去4，过一个点后停，走 */
	case 5:
		PointMode(TURN_ANYANGLE, 28);
		break;

	case 6:
		LineMode(1, 0, 50);
		PointMode(QUIT_, 0);
		break;

	/* 识别图像 */
	case 7:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 11)
		{
			next = 5;
		}
		else if (res > 11)
		{
			next = 6;
		}
		sign = 4;
		step = 0;

		/* 举手转头 */
		PointMode(TURN_ANYANGLE, 162);
		break;
	}

	return next;
}

int CautocarDlg::_3To5(void)
{
	switch (step++)
	{
	/* 转180 */
	case 0:
		PointMode(HAND_UP, 0);
		break;

	/* 下一个点停，走 */
	case 1:
		LineMode(0, 20);
		PointMode(QUIT_, 0);
		break;

	/* 转角，过一个点停，延后0.1s，走 */
	case 2:
		PointMode(TURN_ANYANGLE, -40);
		break;

	case 3:
		LineMode(1, 10);
		PointMode(QUIT_, 0);
		break;

	/* 转角去5，走 */
	case 4:
		PointMode(TURN_ANYANGLE, 40);
		break;

	case 5:
		LineMode(0, 0);
		PointMode(QUIT_, 0);
		break;

	/* 识别图像 */
	case 6:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 16)
		{
			next = 8;
		}
		else if (res > 16)
		{
			next = 7;
		}
		sign = 5;
		step = 0;

		/* 举手 */
		PointMode(TURN_ANYANGLE, 162);
		break;
	}

	return next;
}

int CautocarDlg::_3To6(void)
{
//	switch (step++)
//	{
//	/* 转180 */
//	case 0:
//		PointMode(TURN_ANYANGLE, 163);
//		break;
//
//	/* 下一个点停，走 */
//	case 1:
//		LineMode(0, 20);
//		PointMode(QUIT_, 0);
//		break;
//
//	/* 转角，过一个点停，延后0.1s，走 */
//	case 2:
//		PointMode(TURN_ANYANGLE, -90);
//		break;
//
//	case 3:
//		LineMode(0, 8);
//		PointMode(QUIT_, 0);
//		break;
//
//	/* 转角去6，走 */
//	case 4:
//		PointMode(TURN_ANYANGLE, -90);
//		break;
//
//	case 5:
//		LineMode(0, 0);
//		PointMode(QUIT_, 0);
//		break;
//
//	case 6:
//		PointMode(TURN_ANYANGLE, -140);
//		break;
//
//	case 7:
//		LineMode(0, 0, 1);
//		PointMode(QUIT_, 0);
//		break;
//
//	case 8:
//		PointMode(TURN_ANYANGLE, 88);
//		break;
//
//	case 9:
//		LineMode(0, 0);
//		PointMode(QUIT_, 0);
//		break;
//
//	/* 识别图像 */
//	case 10:
//		this->OnBnClickedBtscannumber();
//		float res = atof(m_locationgold);
//		if (res < 16)
//		{
//			next = 7;
//		}
//		else if (res > 16)
//		{
//			next = 8;
//		}
//		sign = 6;
//		step = 0;
//
//		/* 举手 */
//		PointMode(HAND_UP, 0);
//		break;
	/*}

	return next;*/
	//
	//switch (step++)
	//{
	///* 转180 */
	//case 0:
	//	PointMode(TURN_ANYANGLE, 163);
	//	break;

	///* 去4 */
	//case 1:
	//	LineMode(0, 20);
	//	PointMode(QUIT_, 0);
	//	break;

	///* 转角，过一个点停，延后0.1s，走 */
	//case 2:
	//	PointMode(TURN_ANYANGLE, -40);
	//	break;

	//case 3:
	//	LineMode(1, 10);
	//	PointMode(QUIT_, 0);
	//	break;

	///* 转角去5，走 */
	//case 4:
	//	PointMode(TURN_ANYANGLE, 40);
	//	break;

	//case 5:
	//	LineMode(0, 0);
	//	PointMode(QUIT_, 0);
	//	break;
	//}

	//return next;


	switch (step++)
	{
	/* 转180 */
	case 0:
		PointMode(HAND_UP, 0);
		break;

	/* 去4 */
	case 1:
		LineMode(3, 0, 50);
		PointMode(QUIT_, 0);
		break;

	/* 转角，过一个点停，延后0.1s，走 */
	case 2:
		PointMode(TURN_ANYANGLE, 162);
		break;

	/* 下一个点停，走 */
	case 3:
		LineMode(0, 25);
		PointMode(QUIT_, 0);
		break;

	/* 转角，过一个点停，延后0.1s，走 */
	case 4:
		PointMode(TURN_ANYANGLE, 88);
		break;

	case 5:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	/* 转角去5，走 */
	case 6:
		PointMode(TURN_ANYANGLE, 88);
		break;

	case 7:
		LineMode(0, 0, 50);
		PointMode(QUIT_, 0);
		break;
	}
	return next;
}

int CautocarDlg::_4To5(void)
{
	switch (step++)
	{
	/* 转180 */
	case 0:
		PointMode(HAND_UP, 0);
		break;

	/* 下一个点停，走 */
	case 1:
		LineMode(0, 25);
		PointMode(QUIT_, 0);
		break;

	/* 转角，过一个点停，延后0.1s，走 */
	case 2:
		PointMode(TURN_ANYANGLE, 88);
		break;

	case 3:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 4:
		PointMode(TURN_ANYANGLE, 88);
		break;

	case 5:
		LineMode(0, 0, 50);
		PointMode(QUIT_, 0);
		break;

	case 6:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 16)
		{
			next = 7;
		}
		else if (res > 16)
		{
			next = 8;
		}
		sign = 5;
		step = 0;
		PointMode(TURN_ANYANGLE, 162);
		break;
	}

	return next;
}

int CautocarDlg::_4To6(void)
{
	switch (step++)
	{
	case 0:
		PointMode(HAND_UP, 0);
		break;

	case 1:
		LineMode(3, 0, 50);
		PointMode(QUIT_, 0);
		break;

	case 2:
		PointMode(TURN_ANYANGLE, 162);
		break;

	case 3:
		LineMode(0, 20);
		PointMode(QUIT_, 0);
		break;

	case 4:
		PointMode(TURN_ANYANGLE, -40);
		break;

	case 5:
		LineMode(1, 10);
		PointMode(QUIT_, 0);
		break;

	case 6:
		PointMode(TURN_ANYANGLE, 40);
		break;

	case 7:
		LineMode(0, 0);
		PointMode(QUIT_, 0);
		break;
	}

	return next;
}

int CautocarDlg::_5To7(void)
{
	switch (step++)
	{
	case 0:
		PointMode(HAND_UP, 0, 14);
		break;

	case 1:
		LineMode(1, 0);
		PointMode(QUIT_, 0);
		break;

	case 2:
		PointMode(TURN_ANYANGLE, 88);
		break;

	case 3:
		LineMode(0, 35, 2);
		PointMode(QUIT_, 0);
		break;

	case 4:
		PointMode(TURN_ANYANGLE, 73, 4);
		break;

	case 5:
		LineMode(1, 0, 80);
		PointMode(QUIT_, 0);
		break;

	case 6:
		PointMode(HAND_UP, 0);
		break;
	}

	return next;
}

int CautocarDlg::_5To8(void)
{
	switch (step++)
	{
	case 0:
		PointMode(HAND_UP, 0, 13);
		break;

	case 1:
		LineMode(1, 0);
		PointMode(QUIT_, 0);
		break;

	case 2:
		PointMode(TURN_ANYANGLE, 80);
		break;

	case 3:
		LineMode(0, 4, 2);
		PointMode(QUIT_, 0);
		break;

	case 4:
		PointMode(TURN_ANYANGLE, -79);
		break;

	case 5:
		LineMode(0, 0, 3);
		PointMode(QUIT_, 0);
		break;


	case 10:
		PointMode(HAND_UP, 0);
		break;
	}

	return next;
}

int CautocarDlg::_6To7(void)
{
	switch (step++)
	{

	case 0:
		PointMode(TURN_ANYANGLE, 155);
		break;


	case 1:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;


	case 2:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 3:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 4:
		PointMode(TURN_ANYANGLE, -163);
		break;

	case 5:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 6:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 7:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 8:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 9:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 10:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 11:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 12:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 13:
		LineMode(0, 0);
		PointMode(QUIT_, 0);
		break;


	case 14:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 16)
		{
			next = 1;
		}
		else if (res > 16)
		{
			next = 8;
		}
		sign = 7;
		step = 0;

		PointMode(HAND_UP, 0);
		break;
	}

	return next;
}

int CautocarDlg::_6To8(void)
{
	switch (step++)
	{

	case 0:
		PointMode(TURN_ANYANGLE, 155);
		break;


	case 1:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 2:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 3:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 4:
		PointMode(TURN_ANYANGLE, -163);
		break;

	case 5:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 6:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 7:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 8:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 9:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 10:
		PointMode(TURN_ANYANGLE, 90);
		break;

	case 11:
		LineMode(0, 10);
		PointMode(QUIT_, 0);
		break;

	case 12:
		PointMode(TURN_ANYANGLE, -90);
		break;

	case 13:
		LineMode(0, 0);
		PointMode(QUIT_, 0);
		break;

	case 14:
		this->OnBnClickedBtscannumber();
		float res = atof(m_locationgold);
		if (res < 16)
		{
			next = 1;
		}
		else if (res > 16)
		{
			next = 8;
		}
		sign = 8;
		step = 0;

		PointMode(HAND_UP, 0);
		break;
	}

	return next;
}
