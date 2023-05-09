//================================================
//
//3D�Q�[��(��)[title.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "title.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

#include "object2D.h"
#include "bg.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const float CTitle::LOGO_WIDTH = 900.0f;	//���S�̕�
const float CTitle::LOGO_HEIGHT = 300.0f;	//���S�̍���

const float CTitle::PRESSENTER_WIDTH = 640.0f;	//PressEnter�̕�
const float CTitle::PRESSENTER_HEIGHT = 120.0f;	//PressEnter�̍���

//***************************
//�ÓI�����o�ϐ�
//***************************
CBg* CTitle::m_pBg = nullptr;					//�w�i
CObject2D* CTitle::m_pLogo = nullptr;			//�^�C�g�����S
CObject2D* CTitle::m_pPressEnter = nullptr;		//PressEnter

//================================================
//�R���X�g���N�^
//================================================
CTitle::CTitle() : CMode(MODE::TITLE),
m_bFadeOut(false)
{
}

//================================================
//�f�X�g���N�^
//================================================
CTitle::~CTitle()
{
}

//================================================
//������
//================================================
HRESULT CTitle::Init()
{
	//�����o�ϐ��̏�����
	m_bFadeOut = false;

	{/* �w�i */

		//����
		m_pBg = CBg::Create();

		//�e�N�X�`���̐ݒ�
		m_pBg->SetTexture(CTexture::Bg_Title);

		//�e�N�X�`�����W�̐ݒ�
		m_pBg->SetTexUV(1, 0);
	}

	{/* �^�C�g�����S */

		//����
		m_pLogo = CObject2D::Create();

		D3DXVECTOR3 pos = D3DXVECTOR3(	//�ʒu�ݒ�p
			CRenderer::SCREEN_WIDTH * 0.5f,
			CRenderer::SCREEN_HEIGHT * 0.3f,
			0.0f);

		//�ʒu��ݒ�
		m_pLogo->SetPos(pos);

		//�T�C�Y��ݒ�
		D3DXVECTOR2 size = D3DXVECTOR2(LOGO_WIDTH, LOGO_HEIGHT);
		m_pLogo->SetSize(size);

		//�e�N�X�`���̐ݒ�
		m_pLogo->SetTexture(CTexture::TitleLogo);

		//�e�N�X�`�����W�̐ݒ�
		m_pLogo->SetTexUV(1, 0);
	}

	{/* PressEnter */

		//����
		m_pPressEnter = CObject2D::Create();

		D3DXVECTOR3 pos = D3DXVECTOR3(	//�ʒu�ݒ�p
			CRenderer::SCREEN_WIDTH * 0.5f,
			CRenderer::SCREEN_HEIGHT * 0.8f,
			0.0f);

		//�ʒu��ݒ�
		m_pPressEnter->SetPos(pos);

		//�T�C�Y��ݒ�
		D3DXVECTOR2 size = D3DXVECTOR2(PRESSENTER_WIDTH, PRESSENTER_HEIGHT);
		m_pPressEnter->SetSize(size);

		//�e�N�X�`���̐ݒ�
		m_pPressEnter->SetTexture(CTexture::PressEnter);

		//�e�N�X�`�����W�̐ݒ�
		m_pPressEnter->SetTexUV(1, 0);
	}

	//BGM�J�n
	CApplication::GetSound()->Play(CSound::LABEL_BGM_Title);

	//���]
	CApplication::GetFade()->Set(CFade::STATE::FADE_IN);

	return S_OK;
}

//================================================
//�I��
//================================================
void CTitle::Uninit()
{
	/* �T�E���h */

	CApplication::GetSound()->Stop(CSound::LABEL_BGM_Title);

	/* �I�u�W�F�N�g */

	CObject2D::ReleaseAll();	//�S�Ẳ��(2D)

	/* �^�C�g�����S */

	if (m_pLogo != nullptr)
	{//NULL�`�F�b�N
		m_pLogo = nullptr;	//nullptr�ɂ���
	}

	/* �w�i */

	if (m_pBg != nullptr)
	{//NULL�`�F�b�N
		m_pBg = nullptr;	//nullptr�ɂ���
	}
}

//================================================
//�X�V
//================================================
void CTitle::Update()
{
	CObject::UpdateAll();	//�I�u�W�F�N�g

	CInput* pInput = CInput::GetKey();	//�L�[�{�[�h�����擾

	if (pInput->Trigger(CInput::STANDARD_KEY::DECISION))
	{//����L�\
		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Enter);

		CApplication::GetFade()->Set(CFade::STATE::FADE_OUT);	//�Ó]
		m_bFadeOut = true;	//�Ó]����
	}

	if (m_bFadeOut && (CApplication::GetFade()->GetState() == CFade::STATE::NONE))
	{//�t�F�[�h���I����ĈÓ]���̏ꍇ
		Change(MODE::GAME);	//���[�h�̐ݒ�
	}
}

//================================================
//�`��
//================================================
void CTitle::Draw()
{
	CObject::DrawAll();	//�I�u�W�F�N�g
}