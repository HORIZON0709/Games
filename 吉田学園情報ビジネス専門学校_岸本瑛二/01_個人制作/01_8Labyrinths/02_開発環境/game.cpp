//================================================
//
//3D�Q�[��(��)[game.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "game.h"
#include "application.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

#include "camera.h"
#include "light.h"
#include "player.h"
#include "item.h"
#include "stage.h"
#include "panel.h"
#include "score.h"
#include "object2D.h"

#include <assert.h>
#include <time.h>

//***************************
//�萔�̒�`
//***************************
const int CGame::INTERVAL_STRAIGHT = 120;		//�����G�̐����Ԋu
const int CGame::FADE_INTERVAL_GAMEOVER = 60;	//�t�F�[�h�܂ł̊Ԋu(�Q�[���I�[�o�[��)
const int CGame::FADE_INTERVAL_GAMECLEAR = 180;	//�t�F�[�h�܂ł̊Ԋu(�Q�[���N���A��)

const float CGame::UI_PURPOSE_WIDTH = 400.0f;	//UI(�ړI)�̕�
const float CGame::UI_PURPOSE_HEIGHT = 200.0f;	//UI(�ړI)�̍���
const float CGame::UI_COIN_WIDTH = 600.0f;		//UI(�l���R�C����)�̕�
const float CGame::UI_COIN_HEIGHT = 100.0f;		//UI(�l���R�C����)�̍���

//***************************
//�ÓI�����o�ϐ�
//***************************
CCamera* CGame::m_pCamera = nullptr;	//�J����
CLight* CGame::m_pLight = nullptr;		//���C�g
CPlayer* CGame::m_pPlayer = nullptr;	//�v���C���[
CStage* CGame::m_pStage = nullptr;		//�X�e�[�W
CPanel* CGame::m_pPanel = nullptr;		//�p�l��
CScore* CGame::m_pScore = nullptr;		//�X�R�A

CObject2D* CGame::m_apUI[UI_TYPE::MAX] = {};	//UI

//================================================
//�J���������擾
//================================================
CCamera* CGame::GetCamera()
{
	return m_pCamera;
}

//================================================
//���C�g�����擾
//================================================
CLight* CGame::GetLight()
{
	return m_pLight;
}

//================================================
//�v���C���[�����擾
//================================================
CPlayer* CGame::GetPlayer()
{
	return m_pPlayer;
}

//================================================
//�X�e�[�W�����擾
//================================================
CStage* CGame::GetStage()
{
	return m_pStage;
}

//================================================
//�p�l�������擾
//================================================
CPanel* CGame::GetPanel()
{
	return m_pPanel;
}

//================================================
//�X�R�A�����擾
//================================================
CScore* CGame::GetScore()
{
	return m_pScore;
}

//================================================
//�R���X�g���N�^
//================================================
CGame::CGame() : 
	CMode(MODE::GAME),
	m_nCntIntervalFade(0),
	m_bFadeOut(false),
	m_bWireFrame(false)
{
}

//================================================
//�f�X�g���N�^
//================================================
CGame::~CGame()
{
}

//================================================
//������
//================================================
HRESULT CGame::Init()
{
	srand((unsigned)time(NULL));	//�����_����q�̏�����

	//�����o�ϐ��̏�����
	m_nCntIntervalFade = 0;
	m_bFadeOut = false;
	m_bWireFrame = false;

	//***** ���� *****//

	/* �J���� */

	if (m_pCamera == nullptr)
	{//NULL�`�F�b�N
		m_pCamera = new CCamera;	//���I�m��
		m_pCamera->Init();			//������
	}

	/* ���C�g */

	if (m_pLight == nullptr)
	{//NULL�`�F�b�N
		m_pLight = new CLight;	//���I�m��
		m_pLight->Init();		//������
	}

	/* �v���C���[ */

	if (m_pPlayer == nullptr)
	{//NULL�`�F�b�N
		m_pPlayer = CPlayer::Create();	//����
	}

	/* �X�e�[�W */

	if (m_pStage == nullptr)
	{//NULL�`�F�b�N
		m_pStage = CStage::Create(CStage::STAGE::Stage08);	//����
	}

	/* �p�l�� */

	if (m_pPanel == nullptr)
	{//NULL�`�F�b�N
		m_pPanel = CPanel::Create();	//����
	}

	/* �X�R�A */

	if (m_pScore == nullptr)
	{//NULL�`�F�b�N
		m_pScore = CScore::Create();	//����
	}

	/* UI */

	for (int i = 0; i < UI_TYPE::MAX; i++)
	{
		if (m_apUI[i] != nullptr)
		{//NULL�`�F�b�N
			m_apUI[i] = nullptr;
		}

		/* nullptr�̏ꍇ */

		//����
		m_apUI[i] = CObject2D::Create();

		//�e���ݒ�p
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);			//�T�C�Y
		CTexture::TEXTURE tex = CTexture::TEXTURE::NONE;	//�e�N�X�`��

		switch ((UI_TYPE)i)
		{//UI�^�C�v�ʂ̐ݒ�
		case UI_TYPE::PURPOSE:	//�ړI
			//�ʒu
			pos = D3DXVECTOR3(
				CRenderer::SCREEN_WIDTH * 0.17f,
				CRenderer::SCREEN_HEIGHT * 0.15f,
				0.0f
			);
			
			//�T�C�Y
			size = D3DXVECTOR2(UI_PURPOSE_WIDTH, UI_PURPOSE_HEIGHT);

			//�e�N�X�`��
			tex = CTexture::TEXTURE::GameUI_Purpose;
			break;

		case UI_TYPE::COIN:	//�l���R�C����
			//�ʒu
			pos = D3DXVECTOR3(
				CRenderer::SCREEN_WIDTH * 0.75f,
				CRenderer::SCREEN_HEIGHT * 0.9f,
				0.0f
			);
			
			//�T�C�Y
			size = D3DXVECTOR2(UI_COIN_WIDTH, UI_COIN_HEIGHT);

			//�e�N�X�`��
			tex = CTexture::TEXTURE::GameUI_Coin;
			break;

		default:
			assert(false);
			break;
		}

		//�e���̐ݒ�
		m_apUI[i]->SetPos(pos);		//�ʒu
		m_apUI[i]->SetSize(size);	//�T�C�Y
		m_apUI[i]->SetTexture(tex);	//�e�N�X�`��
	}

	//BGM�J�n
	CApplication::GetSound()->Play(CSound::LABEL_BGM_Game);

	//���]����
	m_bFadeOut = false;

	//���]
	CApplication::GetFade()->Set(CFade::STATE::FADE_IN);

	return S_OK;
}

//================================================
//�I��
//================================================
void CGame::Uninit()
{
	/* �T�E���h */

	CApplication::GetSound()->Stop(CSound::LABEL_BGM_Game);

	/* �I�u�W�F�N�g */

	CObject2D::ReleaseAll();	//�S�Ẳ��(2D)
	CObjectX::ReleaseAll();		//�S�Ẳ��(X���f��)

	/* �X�R�A */

	m_pScore = nullptr;	//nullptr�ɂ���

	/* �p�l�� */

	if (m_pPanel != nullptr)
	{
		m_pPanel->Uninit();	//�I��
		delete m_pPanel;	//�������̉��
		m_pPanel = nullptr;	//nullptr�ɂ���
	}

	/* �X�e�[�W */

	if (m_pStage != nullptr)
	{//NULL�`�F�b�N
		m_pStage->UninitAll();	//�S�Ă̏I��
		delete m_pStage;		//�������̉��
		m_pStage = nullptr;		//nullptr�ɂ���
	}

	/* �v���C���[ */

	m_pPlayer = nullptr;	//nullptr�ɂ���

	/* ���C�g */

	if (m_pLight != nullptr)
	{//NULL�`�F�b�N
		m_pLight->Uninit();	//�I��
		delete m_pLight;	//�������̉��
		m_pLight = nullptr;	//nullptr�ɂ���
	}

	/* �J���� */

	if (m_pCamera != nullptr)
	{//NULL�`�F�b�N
		m_pCamera->Uninit();	//�I��
		delete m_pCamera;		//�������̉��
		m_pCamera = nullptr;	//nullptr�ɂ���
	}
}

//================================================
//�X�V
//================================================
void CGame::Update()
{
	//���C���[�t���[���̐؂�ւ�
	SwitchWireFrame();

	if (m_pPanel != nullptr)
	{//NULL�`�F�b�N
		m_pPanel->Update();	//�p�l��
	}

	if (m_pPanel->GetIsPanel())
	{//�p�l�����쒆�̏ꍇ
		return;
	}

	/* �p�l����������Ă��Ȃ��ꍇ */

	CObject::UpdateAll();	//�I�u�W�F�N�g

	if (m_pCamera != nullptr)
	{//NULL�`�F�b�N
		m_pCamera->Update();	//�J����
	}

	if (m_pLight != nullptr)
	{//NULL�`�F�b�N
		m_pLight->Update();	//���C�g
	}

	if (m_pStage != nullptr)
	{//NULL�`�F�b�N
		m_pStage->Update();
		m_pStage = m_pStage->Set();	//�X�e�[�W
	}

	if (m_pScore->GetCurrentScore() == CStage::MAX_COIN)
	{//�A�C�e����S�Ċl������
		m_nCntIntervalFade++;	//�J�E���g�A�b�v
	}

	if (!m_bFadeOut && (m_nCntIntervalFade > FADE_INTERVAL_GAMEOVER))
	{//�Ó]���Ă��Ȃ� & �J�E���g����萔�𒴂���
		//�Ó]
		CApplication::GetFade()->Set(CFade::STATE::FADE_OUT);

		//�Ó]����
		m_bFadeOut = true;
	}

	if (m_bFadeOut && (CApplication::GetFade()->GetState() == CFade::STATE::NONE))
	{//�Ó]���� & ���݃t�F�[�h���Ă��Ȃ�
		//���U���g�ֈڍs
		Change(MODE::RESULT);
		
		//�J�E���g���Z�b�g
		m_nCntIntervalFade = 0;

		//���]����
		m_bFadeOut = false;

		//���]
		CApplication::GetFade()->Set(CFade::STATE::FADE_IN);
	}
}

//================================================
//�`��
//================================================
void CGame::Draw()
{
	//�J�����̐ݒ�
	m_pCamera->Set();

	CObject::DrawAll();	//�I�u�W�F�N�g
}

//================================================
//���C���[�t���[���̐؂�ւ�
//================================================
void CGame::SwitchWireFrame()
{
	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_F2))
	{//F2�L�[
		//�\��:��\���̐؂�ւ�
		m_bWireFrame = m_bWireFrame ? false : true;

		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		if (m_bWireFrame)
		{//�\����
			//���C���[�t���[�����[�h�̐ݒ�
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{//��\����
			//���ɖ߂�
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}