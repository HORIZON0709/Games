//================================================
//
//3D�Q�[��(��)[application.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//�O���錾
//***************************
class CInput;
class CInputKeyboard;
class CRenderer;
class CTexture;
class CMode;
class CFade;
class CSound;

//***************************
//�A�v���P�[�V�����N���X�̒�`
//***************************
class CApplication
{/* ��{�N���X */
public: /* �ÓI�����o�֐� */

	//********** ���̎擾 **********//

	static CInput* GetInput();					//�C���v�b�g
	static CInputKeyboard* GetInputKeyboard();	//�L�[�{�[�h

	static CTexture* GetTexture();		//�e�N�X�`��
	static CRenderer* GetRenderer();	//�����_���[

	static CMode* GetMode();	//���[�h
	static CFade* GetFade();	//�t�F�[�h
	static CSound* GetSound();	//�T�E���h

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CApplication();
	~CApplication();

public: /* �����o�֐� */
	HRESULT Init(HWND hWnd, BOOL bWindow,HINSTANCE hInstance);	//������

	void Uninit();	//�I��
	void Update();	//�X�V
	void Draw();	//�`��

private: /* �ÓI�����o�ϐ� */
	
	//********** �|�C���^ **********//

	static CInput* m_pInput;					//�C���v�b�g
	static CInputKeyboard* m_pInputKeyboard;	//�L�[�{�[�h

	static CTexture* m_pTexture;	//�e�N�X�`��
	static CRenderer* m_pRenderer;	//�����_���[

	static CMode* m_pMode;		//���[�h
	static CFade* m_pFade;		//�t�F�[�h
	static CSound* m_pSound;	//�T�E���h
};
#endif