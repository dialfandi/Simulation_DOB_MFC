
// GraphDlg.h : header file
//

#pragma once
#include "tchart1.h"


// CGraphDlg dialog
class CGraphDlg : public CDialogEx
{
// Construction
public:
	CGraphDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTchart1 m_chart;
	afx_msg void OnBnClickedOk();
	CTchart1 m_chart1;
};
