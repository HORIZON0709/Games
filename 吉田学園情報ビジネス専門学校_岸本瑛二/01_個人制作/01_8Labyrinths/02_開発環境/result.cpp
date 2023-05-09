//================================================
//
//3D�Q�[��(��)[result.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "result.h"
#include "application.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

#include "bg.h"
#include "object2D.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const float CResult::GAMECLEAR_WIDTH = 800.0f;	//�Q�[���N���A�̕�
const float CResult::GAMECLEAR_HEIGHT = 300.0f;	//�Q�[���N���A�̍���

//***************************
//�ÓI�����o�ϐ�
//***************************
CBg* CResult::m_pBg = nullptr;				//�w�i
CObject2D* CResult::m_pGameClear = nullptr;	//�Q�[���N���A

//================================================
//�R���X�g���N�^
//================================================
CResult::CResult() : CMode(MODE::RESULT),
m_bFadeOut(false)
{
}

//================================================
//�f�X�g���N�^
//================================================
CResult::~CResult()
{
}

//================================================
//������
//================================================
HRESULT CResult::Init()
{
	//�����o�ϐ��̏�����
	m_bFadeOut = false;

	/* �w�i */

	//����
	m_pBg = CBg::Create();

	//�e�N�X�`���̐ݒ�
	m_pBg->SetTexture(CTexture::Bg_Result);

	//�e�N�X�`�����W�̐ݒ�
	m_pBg->SetTexUV(1, 0);

	/* �Q�[���N���A */

	//����
	m_pGameClear = CObject2D::Create();

	D3DXVECTOR3 pos = D3DXVECTOR3(	//�ʒu�ݒ�p
		CRenderer::SCREEN_WIDTH * 0.5f,
		CRenderer::SCREEN_HEIGHT * 0.5f,
		0.0f);

	//�ʒu��ݒ�
	m_pGameClear->SetPos(pos);

	//�T�C�Y��ݒ�
	D3DXVECTOR2 size = D3DXVECTOR2(GAMECLEAR_WIDTH, GAMECLEAR_HEIGHT);
	m_pGameClear->SetSize(size);

	//�e�N�X�`���̐ݒ�
	m_pGameClear->SetTexture(CTexture::GameClear);

	//�e�N�X�`�����W�̐ݒ�
	m_pGameClear->SetTexUV(1, 0);

	//BGM�J�n
	CApplication::GetSound()->Play(CSound::LABEL_BGM_Result);

	//���]
	CApplication::GetFade()->Set(CFade::STATE::FADE_IN);

	return S_OK;
}

//================================================
//�I��
//================================================
void CResult::Uninit()
{
	/* �T�E���h */

	CApplication::GetSound()->Stop(CSound::LABEL_BGM_Result);

	/* �I�u�W�F�N�g */

	CObject2D::ReleaseAll();	//�S�Ẳ��(2D)

	/* �w�i */

	if (m_pBg != nullptr)
	{//NULL�`�F�b�N
		m_pBg = nullptr;	//nullptr�ɂ���
	}
}

//================================================
//�X�V
//================================================
void CResult::Update()
{
	CObject::UpdateAll();	//�I�u�W�F�N�g

	CInput* pInput = CInput::GetKey();	//�L�[�{�[�h�����擾

	if (pInput->Trigger(CInput::STANDARD_KEY::DECISION))
	{//����L�\
		CApplication::GetFade()->Set(CFade::STATE::FADE_OUT);	//�Ó]
		m_bFadeOut = true;	//�Ó]����
	}

	if (m_bFadeOut && (CApplication::GetFade()->GetState() == CFade::STATE::NONE))
	{//�t�F�[�h���I����ĈÓ]���̏ꍇ
		Change(MODE::TITLE);	//���[�h�̐ݒ�
	}
}

//================================================
//�`��
//================================================
void CResult::Draw()
{
	CObject::DrawAll();	//�I�u�W�F�N�g
}