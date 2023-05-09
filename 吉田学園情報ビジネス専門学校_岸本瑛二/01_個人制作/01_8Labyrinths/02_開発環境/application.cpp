//================================================
//
//3D�Q�[��(��)[application.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "application.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "fade.h"
#include "mode.h"
#include "sound.h"

//***************************
//�ÓI�����o�ϐ�
//***************************
CInput* CApplication::m_pInput = nullptr;					//�C���v�b�g
CInputKeyboard* CApplication::m_pInputKeyboard = nullptr;	//�L�[�{�[�h

CTexture* CApplication::m_pTexture = nullptr;	//�e�N�X�`��
CRenderer* CApplication::m_pRenderer = nullptr;	//�����_���[

CMode* CApplication::m_pMode = nullptr;		//���[�h
CFade* CApplication::m_pFade = nullptr;		//�t�F�[�h
CSound* CApplication::m_pSound = nullptr;	//�T�E���h

//================================================
//�C���v�b�g�����擾
//================================================
CInput* CApplication::GetInput()
{
	return m_pInput;
}

//================================================
//�L�[�{�[�h�����擾
//================================================
CInputKeyboard* CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//================================================
//�e�N�X�`�������擾
//================================================
CTexture* CApplication::GetTexture()
{
	return m_pTexture;
}

//================================================
//�����_���[�����擾
//================================================
CRenderer* CApplication::GetRenderer()
{
	return m_pRenderer;
}

//================================================
//���[�h�����擾
//================================================
CMode* CApplication::GetMode()
{
	return m_pMode;
}

//================================================
//�t�F�[�h�����擾
//================================================
CFade* CApplication::GetFade()
{
	return m_pFade;
}

//================================================
//�T�E���h�����擾
//================================================
CSound* CApplication::GetSound()
{
	return m_pSound;
}

//================================================
//�R���X�g���N�^
//================================================
CApplication::CApplication()
{
}

//================================================
//�f�X�g���N�^
//================================================
CApplication::~CApplication()
{
}

//================================================
//������
//================================================
HRESULT CApplication::Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance)
{
	/* �C���v�b�g */

	if (m_pInput == nullptr)
	{//NULL�`�F�b�N
		m_pInput = CInput::Create();	//����
	}

	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	/* �L�[�{�[�h */

	if (m_pInputKeyboard == nullptr)
	{//NULL�`�F�b�N
		m_pInputKeyboard = new CInputKeyboard;	//�������̓��I�m��
	}

	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	/* �e�N�X�`�� */

	if (m_pTexture == nullptr)
	{//NULL�`�F�b�N
		m_pTexture = new CTexture;	//�������̓��I�m��
	}

	/* �����_���[ */

	if (m_pRenderer == nullptr)
	{//NULL�`�F�b�N
		m_pRenderer = new CRenderer;	//�������̓��I�m��
	}

	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	/* �t�F�[�h */

	if (m_pFade == nullptr)
	{//NULL�`�F�b�N
		m_pFade = CFade::Create();	//����
	}

	/* �T�E���h */

	if (m_pSound == nullptr)
	{//NULL�`�F�b�N
		m_pSound = new CSound;	//�������̓��I�m��
	}

	if (FAILED(m_pSound->Init(hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	/* ���[�h */

	if (m_pMode == nullptr)
	{//NULL�`�F�b�N
		m_pMode = CMode::Create(CMode::MODE::TITLE);	//����
	}

	return S_OK;
}

//================================================
//�I��
//================================================
void CApplication::Uninit()
{
	/* ���[�h */

	if (m_pMode != nullptr)
	{//NULL�`�F�b�N
		m_pMode->Uninit();	//�I��
		delete m_pMode;		//�������̉��
		m_pMode = nullptr;	//nullptr�ɂ���
	}

	/* �T�E���h */

	if (m_pSound != nullptr)
	{//NULL�`�F�b�N
		m_pSound->Uninit();	//�I��
		delete m_pSound;	//�������̉��
		m_pSound = nullptr;	//nullptr�ɂ���
	}

	/* �t�F�[�h */

	if (m_pFade != nullptr)
	{//NULL�`�F�b�N
		m_pFade->Uninit();	//�I��
		delete m_pFade;		//�������̉��
		m_pFade = nullptr;	//nullptr�ɂ���
	}

	/* �����_���[ */

	if (m_pRenderer != nullptr)
	{//NULL�`�F�b�N
		m_pRenderer->Uninit();	//�I������
		delete m_pRenderer;		//�������̉��
		m_pRenderer = nullptr;	//nullptr�ɂ���
	}

	/* �e�N�X�`�� */

	if (m_pTexture != nullptr)
	{//NULL�`�F�b�N
		m_pTexture->ReleaseAll();	//�I������
		delete m_pTexture;			//�������̉��
		m_pTexture = nullptr;		//nullptr�ɂ���
	}

	/* �L�[�{�[�h */

	if (m_pInputKeyboard != nullptr)
	{//NULL�`�F�b�N
		m_pInputKeyboard->Uninit();	//�I������
		delete m_pInputKeyboard;	//�������̉��
		m_pInputKeyboard = nullptr;	//nullptr�ɂ���
	}

	/* �C���v�b�g */

	m_pInput->Uninit();	//�I������

}

//================================================
//�X�V
//================================================
void CApplication::Update()
{
	if (m_pInput != nullptr)
	{//NULL�`�F�b�N
		m_pInput->Update();	//�C���v�b�g
	}

	if (m_pInputKeyboard != nullptr)
	{//NULL�`�F�b�N
		m_pInputKeyboard->Update();	//�L�[�{�[�h
	}

	if (m_pRenderer != nullptr)
	{//NULL�`�F�b�N
		m_pRenderer->Update();	//�����_���[
	}

	if (m_pMode != nullptr)
	{//NULL�`�F�b�N
		m_pMode = m_pMode->Set();	//���[�h�̐ݒ�
	}

	if (m_pFade != nullptr)
	{//NULL�`�F�b�N
		m_pFade->Update();	//�t�F�[�h
	}
}

//================================================
//�`��
//================================================
void CApplication::Draw()
{
	if (m_pRenderer != nullptr)
	{//NULL�`�F�b�N
		m_pRenderer->Draw();	//�����_���[
	}
}