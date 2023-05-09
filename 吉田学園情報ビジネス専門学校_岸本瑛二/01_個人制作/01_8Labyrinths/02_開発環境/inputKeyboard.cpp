//================================================
//
//3D�Q�[��(��)[inputKeyboard.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "inputKeyboard.h"

//================================================
//�R���X�g���N�^
//================================================
CInputKeyboard::CInputKeyboard():
	m_pInput(nullptr),
	m_pDevKeyboard(nullptr)
{
	//�����o�ϐ��̃N���A
	memset(m_aKeyStatePress, 0, sizeof(m_aKeyStatePress));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
}

//================================================
//�f�X�g���N�^
//================================================
CInputKeyboard::~CInputKeyboard()
{
}

//================================================
//������
//================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevKeyboard->Acquire();

	return S_OK;
}

//================================================
//�I��
//================================================
void CInputKeyboard::Uninit()
{
	//���̓f�o�C�X�i�L�[�{�[�h�j�̕���
	if (m_pDevKeyboard != nullptr)
	{
		m_pDevKeyboard->Unacquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevKeyboard->Release();
		m_pDevKeyboard = nullptr;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//================================================
//�X�V
//================================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY] = {};	//�L�[�{�[�h�̓��͏��
	
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			//�g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = ~m_aKeyStatePress[nCntKey] & aKeyState[nCntKey];

			//�����[�X����ۑ�
			m_aKeyStateRelease[nCntKey] = m_aKeyStatePress[nCntKey] & ~aKeyState[nCntKey];

			//�v���X����ۑ�
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
		}

		return;
	}
	
	m_pDevKeyboard->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
}

//================================================
//�v���X���̎擾
//================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//================================================
//�����[�X���̎擾
//================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//================================================
//�g���K�[���̎擾
//================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}