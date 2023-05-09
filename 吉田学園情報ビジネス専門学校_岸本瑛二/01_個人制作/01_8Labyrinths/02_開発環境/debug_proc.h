//================================================
//
//�f�o�b�O�\��[debug_proc.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _DEBUG_PROC_H_
#define _DEBUG_PROC_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//�f�o�b�O�\���N���X�̒�`
//***************************
class CDebugProc
{/* ��{�N���X */
private: /* �萔�̒�` */
	static const int MAX_TEXT = 1024;	//�������̍ő�l

public: /* �ÓI�����o�֐� */
	static void Print(const char *pFormat, ...);	//������̓o�^
	static void Draw();	//�`��

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CDebugProc();
	~CDebugProc();

public: /* �����o�֐� */
	void Init();	//������
	void Uninit();	//�I��

private: /* �ÓI�����o�ϐ� */
	static LPD3DXFONT m_pFont;		//�t�H���g���
	static char m_str[MAX_TEXT];	//�o�^������
};

#endif