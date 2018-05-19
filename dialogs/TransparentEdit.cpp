#include "stdafx.h"
#include "TransparentEdit.h"

IMPLEMENT_DYNAMIC(CTransparentEdit, CEdit)

CTransparentEdit::CTransparentEdit()
	: m_TextColor(0)
{
}

CTransparentEdit::~CTransparentEdit()
{ }

BEGIN_MESSAGE_MAP(CTransparentEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_UPDATE, &OnUpdate)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &OnKillFocus)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

HBRUSH CTransparentEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_TextColor);
	return (HBRUSH)GetStockObject(/*NULL_BRUSH*/HOLLOW_BRUSH);
}

void CTransparentEdit::OnKillFocus()
{
	UpdateCtrl();
}

void CTransparentEdit::OnUpdate()
{
	UpdateCtrl();
}

void CTransparentEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateCtrl();
	CEdit::OnLButtonDown(nFlags, point);
}

void CTransparentEdit::UpdateCtrl()
{
	CWnd* pParent = GetParent();
	CRect rcEdit;
	GetWindowRect(rcEdit);
	pParent->ScreenToClient(rcEdit);
	rcEdit.DeflateRect(2, 2);
	pParent->InvalidateRect(rcEdit, FALSE);
}
