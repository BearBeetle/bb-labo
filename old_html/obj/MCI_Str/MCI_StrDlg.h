// MCI_StrDlg.h : �w�b�_�[ �t�@�C��
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
// �\�z
public:
	CMCI_StrDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// Dialog Data
	//{{AFX_DATA(CMCI_StrDlg)
	enum { IDD = IDD_MCI_STR_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMCI_StrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;
	BOOL  m_IsFileOpen;		// �t�@�C���I�[�v������ �ǉ� by BB

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MCI_STRDLG_H__6A664167_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
