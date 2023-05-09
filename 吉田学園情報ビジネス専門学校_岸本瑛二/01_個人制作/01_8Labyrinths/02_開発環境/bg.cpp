//================================================
//
//3D�Q�[��(��)[bg.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "bg.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const float CBg::BG_WIDTH = (float)CRenderer::SCREEN_WIDTH;		//����
const float CBg::BG_HEIGHT = (float)CRenderer::SCREEN_HEIGHT;	//�c��

//================================================
//����
//================================================
CBg* CBg::Create()
{
	CBg* pBg = nullptr;	//�|�C���^

	if (pBg != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pBg = new CBg;	//�������̓��I�m��

	pBg->Init();	//������

	return pBg;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CBg::CBg()
{
	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::BG);
}

//================================================
//�f�X�g���N�^
//================================================
CBg::~CBg()
{
}

//================================================
//������
//================================================
HRESULT CBg::Init()
{
	CObject2D::Init();	//�e�N���X

	//�ʒu��ݒ�
	D3DXVECTOR3 pos = D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f,
								  CRenderer::SCREEN_HEIGHT * 0.5f,
								  0.0f);
	CObject2D::SetPos(pos);

	//�T�C�Y��ݒ�
	D3DXVECTOR2 size = D3DXVECTOR2(BG_WIDTH, BG_HEIGHT);
	CObject2D::SetSize(size);

	return S_OK;
}

//================================================
//�I��
//================================================
void CBg::Uninit()
{
	CObject2D::Uninit();	//�e�N���X
}

//================================================
//�X�V
//================================================
void CBg::Update()
{
	CObject2D::Update();	//�e�N���X
}

//================================================
//�`��
//================================================
void CBg::Draw()
{
	CObject2D::Draw();	//�e�N���X
}