//================================================
//
//3D�Q�[��(��)[inputKeyboard.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

//***************************
//�}�N����`
//***************************
#define DIRECTINPUT_VERSION		(0x0800)	//�r���h���̌x���΍��p�}�N��(���̈ʒu���瓮�����Ȃ�)

//***************************
//���C�u�����[�����N
//***************************
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v(�L�[�{�[�h)
//#pragma comment(lib,"xinput.lib")	//���͏����ɕK�v�i�W���C�p�b�g�j
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g�Ɏg�p

//***************************
//�C���N���[�h
//***************************
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>	//���͏����ɕK�v

//***************************
//�L�[�{�[�h���̓N���X�̒�`
//***************************
class CInputKeyboard
{/* ��{�N���X */
private: /* �萔�̒�` */
	static const int MAX_KEY = 256;	//�L�[�̍ő吔

public: /* �ÓI�����o�֐� */

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CInputKeyboard();
	~CInputKeyboard();

public: /* �����o�֐� */
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//������
	void Uninit();									//�I��
	void Update();									//�X�V
public: /* Get�n */
	bool GetPress(int nKey);	//�v���X
	bool GetRelease(int nKey);	//�����[�X
	bool GetTrigger(int nKey);	//�g���K�[

private: /* �����o�ϐ� */
	LPDIRECTINPUT8 m_pInput;				//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;	//���̓f�o�C�X�i�L�[�{�[�h�i�R���g���[���[�p�͕ʂɍ��j�j�ւ̃|�C���^
	BYTE m_aKeyStatePress[MAX_KEY];			//�v���X���
	BYTE m_aKeyStateRelease[MAX_KEY];		//�����[�X���
	BYTE m_aKeyStateTrigger[MAX_KEY];		//�g���K�[���
};

#endif