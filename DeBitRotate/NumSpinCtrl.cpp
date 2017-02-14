// NumSpinCtrl.cpp : implementation file
//
// Damir Valiulin, Rocscience Inc.
//
// See header file for usage.
//

#include "stdafx.h"
#include "NumSpinCtrl.h"
#include <math.h>
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPS 1.0e-9

/////////////////////////////////////////////////////////////////////////////
// CNumSpinCtrl

CNumSpinCtrl::CNumSpinCtrl()
{
	m_Delta = 1;
	m_MinVal = 0;
	m_MaxVal = 100;
	m_IntRange = 100;

	lconv* pLconv = localeconv ();
	m_DecSymbol = *pLconv->decimal_point;
	m_bTrimTrailingZeros = TRUE;
	SetDecimalPlaces (-1); // simple formatting through "%g"
	SetFormatString (NULL);
}

CNumSpinCtrl::~CNumSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CNumSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CNumSpinCtrl)
	ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltapos)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumSpinCtrl message handlers

void CNumSpinCtrl::SetValueForBuddy (double val)
{
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		CString str;
		if (m_strFormat.IsEmpty ())
			str = FormatValue (val);
		else
			str.Format (m_strFormat, val);

		pEdit->SetWindowText (str);
	}
}

void CNumSpinCtrl::SetPos(double val)
{
	SetValueForBuddy (val);
	SetIntPos (val);
}

double CNumSpinCtrl::GetPos()
{
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		CString str;
		pEdit->GetWindowText (str);
		double val = atof (str);
		return val;
	}
	else
	{
		//ASSERT (FALSE); // you didn't set buddy
		return 0.0;
	}
}

void CNumSpinCtrl::SetRangeAndDelta(double lower, double upper, double delta)
{
	m_MinVal = lower;
	m_MaxVal = upper;
	m_Delta = delta;

	ASSERT (m_MaxVal > m_MinVal); // reversed min/max is not implemented, although it's probably easy

	//// avoid division by zero
	if (m_Delta == 0.0)
		return;

	/// Figure out the integer range to use, so that acceleration can work properly
	double range = fabs ((m_MaxVal - m_MinVal) / m_Delta);
	if (range > (double)UD_MAXVAL)
		m_IntRange = UD_MAXVAL;
	else
		m_IntRange = (int) range;
	CSpinButtonCtrl::SetRange32 (0, m_IntRange);

	/// Set integer position
	SetIntPos (GetPos());
}

void CNumSpinCtrl::SetIntPos (double pos)
{
	//// avoid division by zero
	if (m_MaxVal == m_MinVal)
		return;

	int int_pos;

	if (pos < m_MinVal)
		int_pos = 0;
	else if (pos > m_MaxVal)
		int_pos = m_IntRange;
	else
	{
		// where in the range is current position?
		double pos_in_range = (pos - m_MinVal) / (m_MaxVal - m_MinVal);
		int_pos = (int)(m_IntRange * pos_in_range + 0.5);
	}
	CSpinButtonCtrl::SetPos (int_pos);
}

void CNumSpinCtrl::GetRangeAndDelta(double& lower, double& upper, double& delta)
{
	lower = m_MinVal;
	upper = m_MaxVal;
	delta = m_Delta;
}

BOOL CNumSpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pUD = (NM_UPDOWN*)pNMHDR;

	double val = GetPos () + m_Delta * pUD->iDelta;

	const bool can_wrap = (UDS_WRAP & GetStyle());


	if (pUD->iDelta < 0) // spin down
	{
		double abs_eps = fabs(EPS * max (val, m_MinVal));
		if (abs_eps < EPS) abs_eps = EPS;

		if (m_MinVal - val > abs_eps){  //if (val < m_MinVal){		
			if(can_wrap){
				val = m_MaxVal;
			}
			else{
				val = m_MinVal;
			}
		}
	}
	else  // spin up
	{
		double abs_eps = fabs(EPS * max (val, m_MaxVal));
		if (abs_eps < EPS) abs_eps = EPS;

		if (val - m_MaxVal > abs_eps){   //if (val > m_MaxVal){		
			if(can_wrap){
				val = m_MinVal;
			}
			else{
				val = m_MaxVal;
			}
		}
	}

	SetValueForBuddy (val);

	*pResult = 0;

	return FALSE; // let parent process this notification too.
}

int CNumSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitSpinCtrl();

	return 0;
}

void CNumSpinCtrl::PreSubclassWindow() 
{
	CSpinButtonCtrl::PreSubclassWindow();
	InitSpinCtrl();
}

void CNumSpinCtrl::InitSpinCtrl()
{
	ASSERT ((GetStyle () & UDS_SETBUDDYINT) != UDS_SETBUDDYINT); // Otherwise control won't work properly!
	//ModifyStyle (UDS_SETBUDDYINT, UDS_ARROWKEYS);
	SetRangeAndDelta (m_MinVal, m_MaxVal, m_Delta); // set default values
}

CString CNumSpinCtrl::FormatValue (double val)
{
	CString str;

	if (m_NumDecPlaces == -1) // no precision specified
	{
		str.Format (_T("%g"), val);
		return str;
	}

	CString fstr;

	/// Check which format to use ('e' or 'f')
	bool bExponential;
	fstr.Format (_T("%g"), val);
	if (fstr.Find (_T('e')) != -1)
	{
		fstr.Format (_T("%%.%de"), m_NumDecPlaces);
		bExponential = true;
	}
	else
	{
		fstr.Format (_T("%%.%df"), m_NumDecPlaces);
		bExponential = false;
	}

	/// Output the number with given format
	str.Format (fstr, val);

	/// Check for trailing zeros and remove them
	if (m_bTrimTrailingZeros)
	{
		if (bExponential)
		{
			// separate base and exponent
			CString strBase, strExponent;
			int e_pos = str.Find (_T('e'));
			strBase = str.Left (e_pos);
			strExponent = str.Right (str.GetLength () - e_pos);

			if (str.Find (m_DecSymbol) != -1){
				strBase.TrimRight (_T('0'));
				strBase.TrimRight (m_DecSymbol); // if dot is all that's left
			}

			str = strBase + strExponent; //reconstruct
		}
		else
		{
			if (str.Find (m_DecSymbol) != -1){
				str.TrimRight (_T('0'));
				str.TrimRight (m_DecSymbol); // if dot is all that's left
			}
		}
	}

	return str;
}

void CNumSpinCtrl::SetDecimalPlaces(int number)
{
	m_NumDecPlaces = number;
}

int CNumSpinCtrl::GetDecimalPlaces()
{
	return m_NumDecPlaces;
}

void CNumSpinCtrl::SetFormatString (LPCTSTR lpszFormatString /*= NULL*/)
{
	if (lpszFormatString == NULL)
		m_strFormat.Empty ();
	else
		m_strFormat = lpszFormatString;
}