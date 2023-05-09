//================================================
//
//3D�Q�[��(��)[light.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "light.h"
#include "application.h"
#include "renderer.h"

//================================================
//�R���X�g���N�^
//================================================
CLight::CLight()
{
	//�����o�ϐ����N���A
	memset(m_aLight, 0, sizeof(m_aLight));
}

//================================================
//�f�X�g���N�^
//================================================
CLight::~CLight()
{
}

//================================================
//������
//================================================
void CLight::Init()
{
	/* ���C�g1 */

	//�g�U���ƕ����̐ݒ�p
	D3DXCOLOR diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 vecDir = D3DXVECTOR3(1.0f, -0.8f, 0.0f);

	//�ݒ�
	Set(0, diffuse, vecDir);

	/* ���C�g2 */

	vecDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�����̐ݒ�

	//�ݒ�
	Set(1, diffuse, vecDir);
}

//================================================
//�I��
//================================================
void CLight::Uninit()
{
}

//================================================
//�X�V
//================================================
void CLight::Update()
{
}

//================================================
//�ݒ�
//================================================
void CLight::Set(const int &nNum, const D3DXCOLOR &Diffuse, const D3DXVECTOR3 &vecDirection)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;	//���C�g�̕����x�N�g��

	//���C�g�̎�ނ�ݒ�
	m_aLight[nNum].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	m_aLight[nNum].Diffuse = Diffuse;

	//���C�g�̕�����ݒ�
	vecDir = vecDirection;

	//���K������(�傫��1�̃x�N�g���ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);

	//���C�g�̕���������
	m_aLight[nNum].Direction = vecDir;

	//���C�g��ݒ肷��
	pDevice->SetLight(nNum, &m_aLight[nNum]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(nNum, TRUE);
}