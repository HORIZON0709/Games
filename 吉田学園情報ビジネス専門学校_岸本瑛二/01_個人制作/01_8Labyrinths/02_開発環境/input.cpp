//================================================
//
//3D�Q�[��(��)[input.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "input.h"

#include <assert.h>

//***************************
//�ÓI�����o�ϐ�
//***************************
CInput* CInput::m_pInput = nullptr;

//================================================
//����
//================================================
CInput* CInput::Create()
{
	if (m_pInput != nullptr)
	{//NULL�`�F�b�N
		return nullptr;
	}

	/* nullptr�̏ꍇ */

	m_pInput = new CInput;	//�������̓��I�m��

	return m_pInput;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�擾
//================================================
CInput* CInput::GetKey()
{
	return m_pInput;
}

//================================================
//�R���X�g���N�^
//================================================
CInput::CInput():
	m_pInputKeyboard(nullptr)
{
}

//================================================
//�f�X�g���N�^
//================================================
CInput::~CInput()
{
}

//================================================
//������
//================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInputKeyboard != nullptr)
	{//NULL�`�F�b�N
		m_pInputKeyboard = nullptr;
	}

	/* nullptr�̏ꍇ */

	m_pInputKeyboard = new CInputKeyboard;	//�������̓��I�m��

	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//�I��
//================================================
void CInput::Uninit()
{
	if (m_pInputKeyboard != nullptr)
	{//�L�[�{�[�h
		m_pInputKeyboard->Uninit();	//�I��
		delete m_pInputKeyboard;	//�������̉��
		m_pInputKeyboard = nullptr;	//nullptr�ɂ���
	}

	if (m_pInput != nullptr)
	{//�C���v�b�g
		delete m_pInput;	//�������̉��
		m_pInput = nullptr;	//nullptr�ɂ���
	}
}

//================================================
//�X�V����
//================================================
void CInput::Update()
{
	m_pInputKeyboard->Update();
}

//================================================
//�v���X
//================================================
bool CInput::Press(STANDARD_KEY key)
{
	switch (key)
	{
	case STANDARD_KEY::UP: /* �� */
		if (m_pInputKeyboard->GetPress(DIK_W) ||
			m_pInputKeyboard->GetPress(DIK_UP))
		{
			return true;
		}
		break;

	case STANDARD_KEY::DOWN: /* �� */
		if (m_pInputKeyboard->GetPress(DIK_S) ||
			m_pInputKeyboard->GetPress(DIK_DOWN))
		{
			return true;
		}
		break;

	case STANDARD_KEY::LEFT: /* �� */
		if (m_pInputKeyboard->GetPress(DIK_A) ||
			m_pInputKeyboard->GetPress(DIK_LEFT))
		{
			return true;
		}
		break;

	case STANDARD_KEY::RIGHT: /* �E */
		if (m_pInputKeyboard->GetPress(DIK_D) ||
			m_pInputKeyboard->GetPress(DIK_RIGHT))
		{
			return true;
		}
		break;

	case STANDARD_KEY::DECISION: /* ���� */
		if (m_pInputKeyboard->GetPress(DIK_RETURN))
		{
			return true;
		}
		break;

	case STANDARD_KEY::SHOT: /* ���� */
		if (m_pInputKeyboard->GetPress(DIK_SPACE))
		{
			return true;
		}
		break;

	default: /* ��L�ȊO */
		assert(false);
		break;
	}

	return false;
}

//================================================
//�����[�X
//================================================
bool CInput::Release(STANDARD_KEY key)
{
	switch (key)
	{
	case STANDARD_KEY::UP: /* �� */
		if (m_pInputKeyboard->GetRelease(DIK_W) ||
			m_pInputKeyboard->GetRelease(DIK_UP))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DOWN: /* �� */
		if (m_pInputKeyboard->GetRelease(DIK_S) ||
			m_pInputKeyboard->GetRelease(DIK_DOWN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::LEFT: /* �� */
		if (m_pInputKeyboard->GetRelease(DIK_A) ||
			m_pInputKeyboard->GetRelease(DIK_LEFT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::RIGHT: /* �E */
		if (m_pInputKeyboard->GetRelease(DIK_D) ||
			m_pInputKeyboard->GetRelease(DIK_RIGHT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DECISION: /* ���� */
		if (m_pInputKeyboard->GetRelease(DIK_RETURN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::SHOT: /* ���� */
		if (m_pInputKeyboard->GetRelease(DIK_SPACE))
		{
			return true;
		}
		break;
	default: /* ����ȊO */
		assert(false);
		break;
	}

	return false;
}

//================================================
//�g���K�[
//================================================
bool CInput::Trigger(STANDARD_KEY key)
{
	switch (key)
	{
	case STANDARD_KEY::UP: /* �� */
		if (m_pInputKeyboard->GetTrigger(DIK_W) ||
			m_pInputKeyboard->GetTrigger(DIK_UP))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DOWN: /* �� */
		if (m_pInputKeyboard->GetTrigger(DIK_S) ||
			m_pInputKeyboard->GetTrigger(DIK_DOWN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::LEFT: /* �� */
		if (m_pInputKeyboard->GetTrigger(DIK_A) ||
			m_pInputKeyboard->GetTrigger(DIK_LEFT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::RIGHT: /* �E */
		if (m_pInputKeyboard->GetTrigger(DIK_D) ||
			m_pInputKeyboard->GetTrigger(DIK_RIGHT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DECISION: /* ���� */
		if (m_pInputKeyboard->GetTrigger(DIK_RETURN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::SHOT: /* ���� */
		if (m_pInputKeyboard->GetTrigger(DIK_SPACE))
		{
			return true;
		}
		break;
	default: /* ����ȊO */
		assert(false);
		break;
	}

	return false;
}