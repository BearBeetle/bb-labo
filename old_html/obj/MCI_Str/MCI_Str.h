// MCI_Str.h : MCI_STR アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_MCI_STR_H__6A664165_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
#define AFX_MCI_STR_H__6A664165_0E29_11D2_8BE5_D13C6C669812__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMCI_StrApp:
// このクラスの動作の定義に関しては MCI_Str.cpp ファイルを参照してください。
//

class CMCI_StrApp : public CWinApp
{
public:
	CMCI_StrApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMCI_StrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMCI_StrApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MCI_STR_H__6A664165_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
