//================================================
//
//3D�Q�[��(��)[camera.cpp]
//Author:Kishimoto Eiji
//
//================================================
#include "camera.h"
#include "application.h"
#include "inputKeyboard.h"
#include "renderer.h"
#include "game.h"
#include "fade.h"
#include "player.h"

#include "debug_proc.h"

//***************************
//�萔�̒�`
//***************************
const float CCamera::MOVE_SPEED = 1.5f;		//�ړ����x

//================================================
//�R���X�g���N�^
//================================================
CCamera::CCamera():
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vecU(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//�����o�ϐ��̃N���A
	memset(m_mtxProjection,0,sizeof(m_mtxProjection));
	memset(m_mtxView, 0, sizeof(m_mtxView));
}

//================================================
//�f�X�g���N�^
//================================================
CCamera::~CCamera()
{}

//================================================
//������
//================================================
void CCamera::Init()
{
	//�����o�ϐ��̏����ݒ�
	m_posV = D3DXVECTOR3(-50.0f, 100.0f, -300.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Set();	//�J�������Z�b�g
}

//================================================
//�I��
//================================================
void CCamera::Uninit()
{
}

//================================================
//�X�V
//================================================
void CCamera::Update()
{
	if (CApplication::GetFade()->GetState() == CFade::STATE::FADE_OUT)
	{//�Ó]��
		return;
	}

#ifdef _DEBUG	//�f�o�b�O���̂�

	//�ړ�
	Move();

#endif

	//�����_�̈ʒu���v���C���[�̈ʒu�ɍ��킹��
	m_posR = CGame::GetPlayer()->GetPos();

	//���_���w��̈ʒu�ɌŒ肷��
	m_posV = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, 110.0f, -200.0f);

	CDebugProc::Print("\n�s Camera �t\n");
	CDebugProc::Print("m_posV:[%f,%f,%f]\n", m_posV.x, m_posV.y, m_posV.z);

	if ((m_move.x == 0.0f) && (m_move.y == 0.0f) && (m_move.z == 0.0f))
	{//�ړ����Ă��Ȃ�������
		return;
	}

	/* �ړ����Ă����� */

	//�ړ��ʂɉ����Ĉʒu���X�V(���_)
	//m_posV.x += m_move.x * MOVE_SPEED;	//X��
	//m_posV.y += m_move.y * MOVE_SPEED;	//Y��
	//m_posV.z += m_move.z * MOVE_SPEED;	//Z��

	//�ړ��ʂɉ����Ĉʒu���X�V(�����_)
	//m_posR.x += m_move.x * MOVE_SPEED;	//X��
	//m_posR.y += m_move.y * MOVE_SPEED;	//Y��
	//m_posR.z += m_move.z * MOVE_SPEED;	//Z��

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ��ʂ����Z�b�g
}

//================================================
//�ݒ�
//================================================
void CCamera::Set()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);  

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),
								(float)CRenderer::SCREEN_WIDTH / (float)CRenderer::SCREEN_HEIGHT,
								10.0f,
								3000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
	/*D3DXMatrixOrthoLH(&m_mtxProjection,						//�v���W�F�N�V�����}�g���b�N�X
						(float)CRenderer::SCREEN_WIDTH,		//��
						(float)CRenderer::SCREEN_HEIGHT,	//����
						10.0f,								//�j�A
						3000.0f);							//�t�@�[*/

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//================================================
//�r���[�}�g���N�X�̎擾
//================================================
D3DXMATRIX CCamera::GetMatrixView()
{
	return m_mtxView;
}

//================================================
//���_�̈ʒu�̎擾
//================================================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//================================================
//�ړ�
//================================================
void CCamera::Move()
{
	//�L�[�{�[�h���̎擾
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	/* �O��E���E�E�㉺ */

	if (pKeyboard->GetPress(DIK_A))
	{//���L�[
		/* �ړ������ɉ����Ĉړ��ʂ����Z */

		if (pKeyboard->GetPress(DIK_W))
		{//����
			m_move.x = -1.0f;	//X��
			m_move.z = +1.0f;	//Z��
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//����
			m_move.x = -1.0f;	//X��
			m_move.z = -1.0f;	//Z��
		}
		else
		{//��
			m_move.x = -1.0f;	//X��
		}
	}
	else if (pKeyboard->GetPress(DIK_D))
	{//���L�[
		/* �����ɉ����Ĉړ��ʂ����Z */
		
		if (pKeyboard->GetPress(DIK_W))
		{//�E��
			m_move.x = +1.0f;	//X��
			m_move.z = +1.0f;	//Z��
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//�E��
			m_move.x = +1.0f;	//X��
			m_move.z = -1.0f;	//Z��
		}
		else
		{//�E
			m_move.x = +1.0f;	//X��
		}
	}
	else if (pKeyboard->GetPress(DIK_W))
	{//���L�[(��)
		m_move.z = +1.0f;	//Z��
	}
	else if (pKeyboard->GetPress(DIK_S))
	{//���L�[(��)
		m_move.z = -1.0f;	//Z��
	}
	else if (pKeyboard->GetPress(DIK_O))
	{//O�L�[
		m_move.y = +1.0f;	//Y��
	}
	else if (pKeyboard->GetPress(DIK_L))
	{//L�L�[
		m_move.y = -1.0f;	//Y��
	}
}