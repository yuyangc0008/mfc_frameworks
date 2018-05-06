#ifndef _MFC_DIALOG_BASE_H_V100_YY_
#define _MFC_DIALOG_BASE_H_V100_YY_

/*
 * MFC Dialog basic class
 *@author: clarkyy
 *@date: 20180414
 *@usage:
 *  child class must overwrite InitCtrlsProp(), others are not necessary.
 *  /// test.h
 *  class CMFCDlgTest : public CMfcDialogBase
 *  {
 *  // constructor
 *  public:
 *  	CMFCDlgTest(CWnd* pParent = NULL);
 *  // windows message/event handle
 *  protected:
 *  	afx_msg void OnBtnClkTest();
 *  	DECLARE_MESSAGE_MAP()
 *  // functions to be overwritten 
 *  protected:
 *  	virtual void InitCtrlsProp();
 *  };
 *
 *  /// test.cpp
 *  CMFCDlgTest::CMFCDlgTest(CWnd* pParent)
 *  : CMfcDialogBase(IDR_MAINFRAME, IDD_MFC000_DIALOG, pParent)
 *  {
 *  }
 *  
 *  BEGIN_MESSAGE_MAP(CMFCDlgTest, CMfcDialogBase)
 *  	ON_BN_CLICKED(IDC_BTN_TEST, &OnBtnClkTest)
 *  END_MESSAGE_MAP()
 *  
 *  void CMFCDlgTest::InitCtrlsProp()
 *  {
 *  	m_DlgRect.SetRect(0, 0, 400, 400);
 *  	m_DlgCtrlsProp.RemoveAll();
 *  	AddCtrlProp(CTRL_LABEL, CRect(0, 0, m_DlgRect.Width(), 20),
 *  		0, _T("STATIC"), _T("Label Test"),
 *  		WS_CHILD | WS_VISIBLE | SS_LEFT, this, 0xffffff, NULL, &m_DlgFont);
 *  	AddCtrlProp(CTRL_BUTTON, CRect(0, 70, m_DlgRect.Width(), 100),
 *  		0, _T("BUTTON"), _T("Button Test"),
 *  		WS_CHILD | WS_VISIBLE | BS_CENTER, this, IDC_BTN_TEST, NULL, &m_DlgFont);
 *..	// more...
 *  }
 *  
 *  void CMFCDlgTest::OnBtnClkTest()
 *  {
 *  	MessageBox(_T("test done"), _T("Button Test"), MB_OK);
 *  }
 *
 *
 */

typedef enum eCtrlType
{
	CTRL_LABEL = 0,          // CStatic
	CTRL_BUTTON,             // CButton
	CTRL_EDIT,               // CEdit
	CTRL_COMBOBOX,           // CComboBox
	CTRL_LISTBOX,            // CListBox
	CTRL_CUSTOM,             // custom
	CTRL_COUNT
} eCtrlType;

typedef struct sCtrlProp
{
	eCtrlType      nType;
	CRect          rcPos;
	DWORD          dwExStyle;
	LPCTSTR        lpszClassName;
	LPCTSTR        lpszWindowName;
	DWORD          dwStyle;
	CWnd*          pParent;
	UINT           nID;
	LPVOID         lpParam;
	CFont*         pFont;
	CWnd*          pCustomWnd;
	sCtrlProp()
	{
		memset(this, 0, sizeof(sCtrlProp));
	}
} sCtrlProp;

class CMfcDialogBase : public CDialog
{
public:
	CMfcDialogBase(UINT nIDIcon, UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CMfcDialogBase();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	void AddCtrlProp(const sCtrlProp& prop);
	void AddCtrlProp(eCtrlType nType, const CRect& rcPos, DWORD dwExStyle,
		LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		CWnd* pParent, UINT nID, LPVOID lpParam, CFont* pFont);
	void AddCtrlPropCustom(const CRect& rcPos, CWnd* pCustomWnd);
	void RemoveAllCtrls();

	// 子类按需重写
	virtual void InitDlgFont();
	virtual void InitCtrlsProp();
	virtual void CreateCtrls();
	virtual void SetCtrlsLayout();
	virtual void DrawUI(CPaintDC &dc, int width, int height);  // 双缓冲，绘制到memDC

protected:
	HICON                    m_hIcon;
	CFont                    m_DlgFont;
	CArray<sCtrlProp>        m_DlgCtrlsProp;
	CArray<CWnd*>            m_DlgCtrls;
	CRect                    m_DlgRect;
};

#endif /* _MFC_DIALOG_BASE_H_V100_YY_ */
