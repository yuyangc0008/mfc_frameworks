#ifndef _TRANSPARENT_EDIT_H_V100_
#define _TRANSPARENT_EDIT_H_V100_

class CTransparentEdit : public CEdit
{
	DECLARE_DYNAMIC(CTransparentEdit)

public:
	CTransparentEdit();
	virtual ~CTransparentEdit();

	void SetTextColor(COLORREF color)
	{
		m_TextColor = color;
		UpdateCtrl();
	}

protected:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnUpdate();
	afx_msg void OnKillFocus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	void UpdateCtrl();

private:
	COLORREF			m_TextColor;

};


#endif /* _TRANSPARENT_EDIT_H_V100_ */
