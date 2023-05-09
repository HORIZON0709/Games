//================================================
//
//3D�Q�[��(��)[polygon3D.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "polygon3D.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const float CPolygon3D::POLYGON_WIDTH = 300.0f;		//����
const float CPolygon3D::POLYGON_HEIGHT = 300.0f;	//�c��

//================================================
//����
//================================================
CPolygon3D* CPolygon3D::Create()
{
	CPolygon3D* pPolygon3D = nullptr;	//�|�C���^

	if (pPolygon3D != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pPolygon3D = new CPolygon3D;	//�������̓��I�m��

	pPolygon3D->Init();	//������

	return pPolygon3D;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CPolygon3D::CPolygon3D()
{
	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::BG);
}

//================================================
//�f�X�g���N�^
//================================================
CPolygon3D::~CPolygon3D()
{
}

//================================================
//������
//================================================
HRESULT CPolygon3D::Init()
{
	CObject3D::Init();	//�e�N���X

	//�ʒu��ݒ�
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CObject3D::SetPos(pos);

	//�T�C�Y��ݒ�
	D3DXVECTOR3 size = D3DXVECTOR3(POLYGON_WIDTH, 0.0f, POLYGON_HEIGHT);
	CObject3D::SetSize(size);

	D3DXCOLOR col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	CObject3D::SetCol(col);

	return S_OK;
}

//================================================
//�I��
//================================================
void CPolygon3D::Uninit()
{
	CObject3D::Uninit();	//�e�N���X
}

//================================================
//�X�V
//================================================
void CPolygon3D::Update()
{
	CObject3D::Update();	//�e�N���X
}

//================================================
//�`��
//================================================
void CPolygon3D::Draw()
{
	CObject3D::Draw();	//�e�N���X
}