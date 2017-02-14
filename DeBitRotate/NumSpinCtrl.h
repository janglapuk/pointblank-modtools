#if !defined(AFX_NUMSPINCTRL_H__C2BD95A2_A482_11D2_A6A7_00600844997A__INCLUDED_)
#define AFX_NUMSPINCTRL_H__C2BD95A2_A482_11D2_A6A7_00600844997A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

////////////////////////////////////////////////////////////
// NumSpinCtrl.h : header file
//
// Damir Valiulin, Rocscience Inc., 2002-07-03
//
// This simple class allows you to work with doubles for spin control.
//
// 1. Use SetRangeAndDelta to specify your value range and the delta change
//
// 2. Set current position with a call to SetPos
//
// 3. Specify output precision in 3 ways:
//    a) Default is "%g" value formatting. You can achieve the same by calling
//       SetDecimalPlaces (-1); and SetFormatString(NULL);
//    b) You can provide your own formatting string by calling
//       SetFormatString (). Ex: SetFormatString ("%.5f");
//    c) You can specify a number of decimal places after decimal point with a call to
//       SetDecimalPlaces (). If you supply -1 as a parameter, control will
//       use default formatting string: "%g". You can also specify whether you want
//       to trim trailing zeros with a call to SetTrimTrailingZeros()
//
//
// Revisions:
//
//   2002-07-03 - Commented out ModifyStyle in InitControl() - it didn't work,
//                didn't remove UDS_SETBUDDYINT style. Put ASSERT instead. So now
//                program will ASSERT if you forget to remove UDS_SETBUDDYINT 
//                style in resources.
//                Fixed wrapping. In some cases due to poor machine precision
//                the wrapping condition didn't evaluate properly.
//
//   2002-06-05 - Warren Stevens added wrapping ability. If the style of the spin
//                control is set to "wrap", the value will wrap around if increased
//                or decreased past the range limits.
//
//   2002-04-17 - Fixed bug with trimming zeros. If number of decimal places was set
//                to zero, it was still trimming zeros (e.g. 100 would become 1).
//
//   2001-08-24 - Changed ON_NOTIFY_REFLECT to ON_NOTIFY_REFLECT_EX so that parent
//                can also handle notification message from spin control.
//                Thanks to Emmanuil Tsagarakis for this one.
//
//   2001-07-06 - Original version.
//
/////////////////////////////////////////////////////////////////////////////
// CNumSpinCtrl window

class CNumSpinCtrl : public CSpinButtonCtrl
{
	// Construction
public:
	CNumSpinCtrl();
	virtual ~CNumSpinCtrl();

	// Operations
public:
	void GetRangeAndDelta(double &lower, double& upper, double& delta);
	void SetRangeAndDelta(double lower, double upper, double delta);

	double GetPos();
	void   SetPos(double val);

	int  GetDecimalPlaces ();
	void SetDecimalPlaces (int num_places);
	void SetFormatString (LPCTSTR lpszFormatString = NULL);

	void SetTrimTrailingZeros (BOOL bTrim)		{ m_bTrimTrailingZeros = bTrim; }
	BOOL GetTrimTrailingZeros ()					{ return m_bTrimTrailingZeros; }

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumSpinCtrl)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitSpinCtrl();
	void SetIntPos (double pos);
	void SetValueForBuddy (double val);
	CString FormatValue (double val);

	// Generated message map functions
protected:
	//{{AFX_MSG(CNumSpinCtrl)
	afx_msg BOOL OnDeltapos (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	/// Attributes
protected:
	double m_MinVal;
	double m_MaxVal;
	double m_Delta;
	UINT m_IntRange;
	int m_NumDecPlaces;
	CString m_strFormat;
	BOOL m_bTrimTrailingZeros;
	char m_DecSymbol;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMSPINCTRL_H__C2BD95A2_A482_11D2_A6A7_00600844997A__INCLUDED_)
