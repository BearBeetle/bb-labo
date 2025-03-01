// MCI_StrDlg.h : ヘッダー ファイル
//

#if !defined(AFX_MCI_STRDLG_H__6A664167_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
#define AFX_MCI_STRDLG_H__6A664167_0E29_11D2_8BE5_D13C6C669812__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMCI_StrDlg dialog

class CMCI_StrDlg : public CDialog
{
// 構築
public:
	CMCI_StrDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// Dialog Data
	//{{AFX_DATA(CMCI_StrDlg)
	enum { IDD = IDD_MCI_STR_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMCI_StrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;
	BOOL  m_IsFileOpen;		// ファイルオープン中か 追加 by BB

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMCI_StrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMciOpen();
	afx_msg void OnMciPlay();
	afx_msg void OnMciStop();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MCI_STRDLG_H__6A664167_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
