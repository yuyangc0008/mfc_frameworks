#include "stdafx.h"
#include "MfcDialogBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMfcDialogBase::CMfcDialogBase(UINT nIDIcon, UINT nIDTemplate, CWnd* pParent/* = NULL*/)
	: CDialog(nIDTemplate, pParent)
	, m_DlgRect(0, 0, 400, 100)
{
	m_hIcon = AfxGetApp()->LoadIcon(nIDIcon);
}

CMfcDialogBase::~CMfcDialogBase()
{
	RemoveAllCtrls();
}

void CMfcDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMfcDialogBase, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


BOOL CMfcDialogBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitDlgFont();
	InitCtrlsProp();
	CreateCtrls();
	SetCtrlsLayout();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMfcDialogBase::OnPaint()
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
HCURSOR CMfcDialogBase::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMfcDialogBase::InitDlgFont()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -16;
#ifdef _UNICODE
	swprintf_s(lf.lfFaceName, 32, L"Arial");
#else
	sprintf_s(lf.lfFaceName, 32, "Arial");
#endif
	m_DlgFont.CreateFontIndirect(&lf);
}

void CMfcDialogBase::InitCtrlsProp()
{
#ifdef _DEBUG
	m_DlgCtrlsProp.RemoveAll();
	AddCtrlProp(CTRL_LABEL, CRect(0, 0, m_DlgRect.Width(), m_DlgRect.Height()),
		0, _T("Static"), _T("CMfcDialogBase::InitCtrlsProp() must be overwritten"),
		WS_CHILD|WS_VISIBLE|SS_LEFT, this, 0xffffff, NULL, &m_DlgFont);
#endif
}

void CMfcDialogBase::CreateCtrls()
{
	RemoveAllCtrls();
	for (INT_PTR i = 0; i < m_DlgCtrlsProp.GetCount(); i++)
	{
		sCtrlProp &prop = m_DlgCtrlsProp[i];
		CWnd *wnd = new CWnd();
		wnd->CreateEx(prop.dwExStyle, prop.lpszClassName, prop.lpszWindowName,
			prop.dwStyle, prop.rcPos, prop.pParent, prop.nID, prop.lpParam);
		wnd->SetFont(prop.pFont);
		m_DlgCtrls.Add(wnd);
	}
	ASSERT(m_DlgCtrlsProp.GetCount() == m_DlgCtrls.GetCount());
}

void CMfcDialogBase::SetCtrlsLayout()
{
	SetWindowPos(NULL, 0, 0, m_DlgRect.Width(), m_DlgRect.Height(),
		SWP_NOMOVE);
	ASSERT(m_DlgCtrlsProp.GetCount() == m_DlgCtrls.GetCount());
	for (INT_PTR i = 0; i < m_DlgCtrlsProp.GetCount(); i++)
		m_DlgCtrls[i]->MoveWindow(m_DlgCtrlsProp[i].rcPos);
}

void CMfcDialogBase::AddCtrlProp(const sCtrlProp& prop)
{
	m_DlgCtrlsProp.Add(prop);
}

void CMfcDialogBase::AddCtrlProp(eCtrlType nType, const CRect& rcPos, DWORD dwExStyle,
	LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
	CWnd* pParent, UINT nID, LPVOID lpParam, CFont* pFont)
{
	sCtrlProp prop;
	prop.nType = nType;
	prop.rcPos = rcPos;
	prop.dwExStyle = dwExStyle;
	prop.lpszClassName = lpszClassName;
	prop.lpszWindowName = lpszWindowName;
	prop.dwStyle = WS_VISIBLE | WS_CHILD | dwStyle;
	prop.pParent = pParent;
	prop.nID = nID;
	prop.lpParam = lpParam;
	prop.pFont = pFont;
	AddCtrlProp(prop);
}

void CMfcDialogBase::AddCtrlPropCustom(const CRect& rcPos, CWnd* pCustomWnd)
{
	if (pCustomWnd)
	{
		sCtrlProp prop;
		prop.nType = CTRL_CUSTOM;
		prop.rcPos = rcPos;
		prop.pCustomWnd = pCustomWnd;
		AddCtrlProp(prop);
	}
}

void CMfcDialogBase::RemoveAllCtrls()
{
	for (INT_PTR i = 0; i < m_DlgCtrls.GetCount(); i++)
	{
		m_DlgCtrls[i]->DestroyWindow();
		delete m_DlgCtrls[i];
	}
	m_DlgCtrls.RemoveAll();
}

