
// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Graph.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGraphDlg dialog



CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
	//, m_chart(0)
	//, m_chart1(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Control(pDX, IDC_TCHART2, m_chart1);
}

BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGraphDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGraphDlg message handlers

BOOL CGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGraphDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGraphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGraphDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();

	double sinyalSinus;
	
	double xstep[500];
	double *step = &xstep[100]; //Hasil dari step + plan
	
	double xy[500]; 
	double *y = &xy[5];//K Aksen output sinyal sinus + Pn

	double xkA[500]; 
	double *kA = &xkA[5];//kAksen keluaran dari tf2

	double xdA[500]; 
	double *dA = &xdA[5];//d aksen untuk menghilangkan loop aljabar

	double xQ[500]; //Low pass Filter
	double *Q = &xQ[5];

	double xvA[500]; 
	double *vA = &xvA[5];//v aksen output PID + (tf2 - low pass)

	double xomega[500];
	double *omega = &xomega[5];

	double xPn[500];
	double *Pn = &xPn[2]; //Variabel plan

	double xv[500]; 
	double *v = &xv[5]; //PID output


	omega[0] = omega[-1] = omega[-2] = 0;
	v[0] = v[-1] = v[-2] = 0;
	Pn[0] = Pn[-1] = Pn[-2] = 0;
	vA[0] = vA[-1] = vA[-2] = 0;
	y[0] = y[-1] = y[-2] = 0;
	step[0] = step[-1] = step[-2] = 0;
	kA[0] = kA[-1] = kA[-2] = 0;
	dA[0] = dA[-1] = dA[-2] = 0;
	Q[0] = Q[-1] = Q[-2] = 0;
	sinyalSinus = 0;

	double u, SP, Kp, Ki, Kd, Error, Error_I, Error_D, Error_prev;

	u = Error = Error_I = Error_D = Error_prev = 0;
	SP = 10;
	Kp = 0.4;
	Ki = 2;
	Kd = 0;

	for (int n = 0; n < 200; n++) {
		//PID berasal dari output Y setelah penjumlahan plant (Plant = PID + sinyalStep);
		//PID Controller
		Error = SP - y[n-1];
		Error_I += Error * 0.01;
		Error_D = (Error - Error_prev) / 0.01;
		Error_prev = Error;

		u = (Error * Kp) + (Error_I * Ki) + (Error_D * Kd);
		v[n] = omega[n] = u;

		if (omega[n-1] = 0) { omega[n-1] = 0; }
		else { omega[n] -= dA[n]; }
		//omega[n] = 0.0000961*v[n-1] + 0.00009233*v[n-2] + 1.885*omega[n - 1] - 0.8869*omega[n - 2];

		//Step
		if (n > 10/0.1) {
			step[n] = v[n] + 0.5;
		}
		else {
			step[n] = v[n]+0;
		}

		
		//Plan Sistem
		Pn[n] = 0.995*step[n - 1] + 0.995*step[n] + 0.99*Pn[n-1];

		//Membangkitkan sinyal sinus
		sinyalSinus = 0.01 * sin(2 * 3.14 * 1 * n / 10);

		//Diskrit transfer function Plan
		y[n] = Pn[n] + sinyalSinus;
		
		//Diskrit transfer function Q.Pn-1
		kA[n] = 0.01167*y[n] + 0.001111*y[n - 1] + 0.1111*y[n - 2] + 0.6667*kA[n - 1] - 0.1111*kA[n - 2];
		
		//Diskrit transfer function Low Pass Filter
		Q[n] = 0.1111*omega[n] + 0.2222*omega[n - 1] + 0.1111*omega[n - 2] + 0.6667*Q[n - 1] - 0.1111*Q[n - 2];

		//Pengurangan (Filter)
		dA[n] = kA[n] + Q[n]; //tf2 - tf1
		
		((CSeries)(m_chart.get_Series(0))).AddXY(n*0.01, y[n], NULL, clTeeColor);
		((CSeries)(m_chart1.get_Series(0))).AddXY(n*0.01, Error, NULL, clTeeColor);
		//((CSeries)(m_chart.get_Series(1))).AddXY(i, graph2, NULL, clTeeColor);
	}
}
