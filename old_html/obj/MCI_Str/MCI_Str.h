// MCI_Str.h : MCI_STR �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_MCI_STR_H__6A664165_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
#define AFX_MCI_STR_H__6A664165_0E29_11D2_8BE5_D13C6C669812__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CMCI_StrApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� MCI_Str.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CMCI_StrApp : public CWinApp
{
public:
	CMCI_StrApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMCI_StrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CMCI_StrApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MCI_STR_H__6A664165_0E29_11D2_8BE5_D13C6C669812__INCLUDED_)
