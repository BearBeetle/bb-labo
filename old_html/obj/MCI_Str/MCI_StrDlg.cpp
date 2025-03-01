// MCI_StrDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "MCI_Str.h"
#include "MCI_StrDlg.h"

#include <MMSYSTEM.H>	// �ǉ� by BB

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCI_StrDlg �_�C�A���O

CMCI_StrDlg::CMCI_StrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMCI_StrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMCI_StrDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMCI_StrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMCI_StrDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
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
// CMCI_StrDlg ���b�Z�[�W �n���h��

BOOL CMCI_StrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	m_IsFileOpen=FALSE;		// �ǉ� by BB
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CMCI_StrDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CMCI_StrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/*
 *	�C�x���g����(�t�@�C���I�[�v���j
 *	�ǉ� by BB
 */
void CMCI_StrDlg::OnMciOpen() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// �t�@�C�����擾
	CFileDialog	OFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_EXPLORER,
		"All Files (*.*)|*.*||",NULL);
	if(OFileDialog.DoModal()!=IDOK){
		AfxMessageBox("�t�@�C���͎w�肳��܂���ł���",MB_OK,0);
		return;
	}
	// �O�̃t�@�C�����I�[�v������Ă���Ƃ��́A������N���[�Y����
	if(m_IsFileOpen){
		m_IsFileOpen=FALSE;
		::mciSendString("close mcitest",NULL,0,NULL);
	}

	// MCI�R�}���h�쐬
	char strShortName[100],strSendStr[MAX_PATH];
	::GetShortPathName((LPCTSTR)OFileDialog.GetPathName(),strShortName,sizeof(strShortName));

	wsprintf(strSendStr,"open %s alias mcitest",strShortName);
	if(::mciSendString(strSendStr,NULL,0,NULL)==0){
		// �t���O��������
		m_IsFileOpen=TRUE;
	}
}

/*
 *	�C�x���g����(�Đ��j
 *	�ǉ� by BB
 */
void CMCI_StrDlg::OnMciPlay() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::mciSendString("play mcitest ",NULL,0,NULL);	
}

/*
 *	�C�x���g����(�I���j
 *	�ǉ� by BB
 */
void CMCI_StrDlg::OnMciStop() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::mciSendString("stop mcitest",NULL,0,NULL);	
}

/*
 *	�I��
 */
void CMCI_StrDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	if(m_IsFileOpen){
		::mciSendString("close mcitest",NULL,0,NULL);
	}
	CDialog::OnOK();
}
