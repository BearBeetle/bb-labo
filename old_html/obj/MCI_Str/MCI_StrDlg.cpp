// MCI_StrDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MCI_Str.h"
#include "MCI_StrDlg.h"

#include <MMSYSTEM.H>	// 追加 by BB

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCI_StrDlg ダイアログ

CMCI_StrDlg::CMCI_StrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMCI_StrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMCI_StrDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMCI_StrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMCI_StrDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMCI_StrDlg, CDialog)
	//{{AFX_MSG_MAP(CMCI_StrDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_MCI_OPEN, OnMciOpen)
	ON_BN_CLICKED(ID_MCI_PLAY, OnMciPlay)
	ON_BN_CLICKED(ID_MCI_STOP, OnMciStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCI_StrDlg メッセージ ハンドラ

BOOL CMCI_StrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	m_IsFileOpen=FALSE;		// 追加 by BB
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CMCI_StrDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CMCI_StrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/*
 *	イベント処理(ファイルオープン）
 *	追加 by BB
 */
void CMCI_StrDlg::OnMciOpen() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// ファイル名取得
	CFileDialog	OFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_EXPLORER,
		"All Files (*.*)|*.*||",NULL);
	if(OFileDialog.DoModal()!=IDOK){
		AfxMessageBox("ファイルは指定されませんでした",MB_OK,0);
		return;
	}
	// 前のファイルがオープンされているときは、それをクローズする
	if(m_IsFileOpen){
		m_IsFileOpen=FALSE;
		::mciSendString("close mcitest",NULL,0,NULL);
	}

	// MCIコマンド作成
	char strShortName[100],strSendStr[MAX_PATH];
	::GetShortPathName((LPCTSTR)OFileDialog.GetPathName(),strShortName,sizeof(strShortName));

	wsprintf(strSendStr,"open %s alias mcitest",strShortName);
	if(::mciSendString(strSendStr,NULL,0,NULL)==0){
		// フラグ書き換え
		m_IsFileOpen=TRUE;
	}
}

/*
 *	イベント処理(再生）
 *	追加 by BB
 */
void CMCI_StrDlg::OnMciPlay() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::mciSendString("play mcitest ",NULL,0,NULL);	
}

/*
 *	イベント処理(終了）
 *	追加 by BB
 */
void CMCI_StrDlg::OnMciStop() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::mciSendString("stop mcitest",NULL,0,NULL);	
}

/*
 *	終了
 */
void CMCI_StrDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	if(m_IsFileOpen){
		::mciSendString("close mcitest",NULL,0,NULL);
	}
	CDialog::OnOK();
}
