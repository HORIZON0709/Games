//================================================
//
//3D�Q�[��(��)[fade.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "fade.h"
#include "application.h"
#include "renderer.h"
#include "mode.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const DWORD CFade::FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//���_�t�H�[�}�b�g

const float CFade::FADE_SPEED = 0.02f;	//�t�F�[�h�̑��x

//================================================
//����
//================================================
CFade* CFade::Create()
{
	CFade* pFade = nullptr;	//�|�C���^

	if (pFade != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pFade = new CFade;	//�������̓��I�m��

	pFade->Init();	//������

	return pFade;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CFade::CFade() :
	m_pVtxBuff(nullptr),
	m_state(STATE::NONE),
	m_col(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
{
}

//================================================
//�f�X�g���N�^
//================================================
CFade::~CFade()
{
	/* ����R��̊m�F */
	assert(m_pVtxBuff == nullptr);
}

//================================================
//������
//================================================
HRESULT CFade::Init()
{
	//�����o�ϐ��̏�����
	m_state = STATE::NONE;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = (float)(CRenderer::SCREEN_WIDTH);	//����
	float fHeight = (float)(CRenderer::SCREEN_HEIGHT);	//�c��

	//���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================
//�I��
//================================================
void CFade::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//================================================
//�X�V
//================================================
void CFade::Update()
{
	if (m_state == STATE::NONE)
	{//�t�F�[�h���Ă��Ȃ��ꍇ
		return;
	}

	/* �t�F�[�h���Ă���Ƃ� */

	if (m_state == STATE::FADE_IN)
	{//�t�F�[�h�C��( ���] )
		m_col.a -= FADE_SPEED;	//�����ɂ��Ă���

		if (m_col.a <= 0.0f)
		{//���S�ɓ����ɂȂ�����
			m_col.a = 0.0f;			//0.0�ɂ���
			m_state = STATE::NONE;	//�t�F�[�h���Ă��Ȃ���Ԃɂ���
		}
	}
	else if (m_state == STATE::FADE_OUT)
	{//�t�F�[�h�A�E�g( �Ó] )
		m_col.a += FADE_SPEED;	//�s�����ɂ��Ă���

		if (m_col.a >= 1.0f)
		{//���S�ɕs�����ɂȂ�����
			m_col.a = 1.0f;			//1.0�ɂ���
			m_state = STATE::NONE;	//�t�F�[�h���Ă��Ȃ���Ԃɂ���
		}
	}

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================================
//�`��
//================================================
void CFade::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, nullptr);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
							0,					//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);					//�`�悷��v���~�e�B�u��
}

//================================================
//�ݒ�
//================================================
void CFade::Set(const STATE &state)
{
	m_state = state;	//�t�F�[�h��Ԃ�ݒ�
}

//================================================
//�t�F�[�h��Ԃ̎擾
//================================================
CFade::STATE CFade::GetState()
{
	return m_state;
}