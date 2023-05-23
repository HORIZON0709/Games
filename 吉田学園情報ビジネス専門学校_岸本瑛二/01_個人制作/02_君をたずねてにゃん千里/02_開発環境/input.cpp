//============================================================================================================
//
//�N�������˂Ăɂ��痢[input,cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "input.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECTINPUT8		s_pInput	   = NULL;				//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8 s_pDevKeyboard = NULL;				//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
static BYTE					s_aKeyState[NUM_KEY_MAX];			//�L�[�{�[�h�̃v���X���
static BYTE					s_aKeyStateTrigger[NUM_KEY_MAX];	//�L�[�{�[�h�̃g���K�[���
static BYTE					s_aKeyStateRelease[NUM_KEY_MAX];	//�L�[�{�[�h�̃����[�X���
static XINPUT_STATE			s_joykeyState;						//�W���C�p�b�h�̃v���X���

//============================================================================================================
//�L�[�{�[�h�̏���������
//============================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&s_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard,&s_pDevKeyboard,NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd,
													(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//============================================================================================================
//�L�[�{�[�h�̏I������
//============================================================================================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�������
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}
}

//============================================================================================================
//�L�[�{�[�h�̍X�V����
//============================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//�L�[�{�[�h�̃g���K�[����ۑ�
			s_aKeyStateTrigger[nCntKey] = (~s_aKeyState[nCntKey]) & (aKeyState[nCntKey]);

			//�L�[�{�[�h�̃����[�X����ۑ�
			s_aKeyStateRelease[nCntKey] = (s_aKeyState[nCntKey]) & (~aKeyState[nCntKey]);

			//�L�[�{�[�h�̃v���X����ۑ�
			s_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();		//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//============================================================================================================
//�L�[�{�[�h�̃v���X�����擾
//============================================================================================================
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//============================================================================================================
//�L�[�{�[�h�̃g���K�[�����擾
//============================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//============================================================================================================
//�L�[�{�[�h�̃����[�X�����擾
//============================================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//============================================================================================================
//�W���C�p�b�h�̏���������
//============================================================================================================
HRESULT InitJoypad(void)
{
	//�������̃N���A
	memset(&s_joykeyState, 0, sizeof(XINPUT_STATE));

	//Xinput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

//============================================================================================================
//�W���C�p�b�h�̏I������
//============================================================================================================
void UninitJoypad(void)
{
	//Xinput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//============================================================================================================
//�W���C�p�b�h�̍X�V����
//============================================================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		//�W���C�p�b�h�̓��͏��

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		s_joykeyState = joykeyState;	//�W���C�p�b�h�̃v���X����ۑ�
	}
}

//============================================================================================================
//�W���C�p�b�h�̃v���X�����擾
//============================================================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (s_joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}