//================================================
//
//3D�Q�[��(��)[object3D.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "object3D.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//================================================
//����
//================================================
CObject3D* CObject3D::Create()
{
	CObject3D* pObject3D = nullptr;	//�|�C���^

	if (pObject3D != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pObject3D = new CObject3D;	//�������̓��I�m��

	pObject3D->Init();	//������

	return pObject3D;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CObject3D::CObject3D() : CObject::CObject(CObject::PRIORITY::PRIO_POLYGON_3D),
	m_pTexture(nullptr),
	m_pVtxBuff(nullptr),
	m_texture(CTexture::NONE),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//�����o�ϐ��̃N���A
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::POLYGON_3D);
}

//================================================
//�f�X�g���N�^
//================================================
CObject3D::~CObject3D()
{
	/* ����R��̊m�F */
	assert(m_pTexture == nullptr);
	assert(m_pVtxBuff == nullptr);
}

//================================================
//������
//================================================
HRESULT CObject3D::Init()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�����o�ϐ��̏����ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::NONE;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
void CObject3D::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//================================================
//�X�V
//================================================
void CObject3D::Update()
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	D3DXCOLOR col = pVtx[0].col;
	col = pVtx[1].col;
	col = pVtx[2].col;
	col = pVtx[3].col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================================
//�`��
//================================================
void CObject3D::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���C�g��؂�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CApplication::GetTexture();	//�e�N�X�`�����擾

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
							0,					//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);					//�`�悷��v���~�e�B�u��

	//���C�g��t����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, nullptr);
}

//================================================
//�ʒu��ݒ�
//================================================
void CObject3D::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//================================================
//�ʒu���擾
//================================================
D3DXVECTOR3 CObject3D::GetPos()
{
	return m_pos;
}

//================================================
//������ݒ�
//================================================
void CObject3D::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//================================================
//�������擾
//================================================
D3DXVECTOR3 CObject3D::GetRot()
{
	return m_rot;
}

//================================================
//�T�C�Y�̐ݒ�
//================================================
void CObject3D::SetSize(const D3DXVECTOR3 &size)
{
	m_size = size;	//�T�C�Y��ݒ�

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�T�C�Y�𔼕��ɂ���
	D3DXVECTOR3 sizeHalf = D3DXVECTOR3((size.x * 0.5f), (size.y * 0.5f), (size.z * 0.5f));

	//���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-sizeHalf.x, +sizeHalf.y, +sizeHalf.z);
	pVtx[1].pos = D3DXVECTOR3(+sizeHalf.x, +sizeHalf.y, +sizeHalf.z);
	pVtx[2].pos = D3DXVECTOR3(-sizeHalf.x, -sizeHalf.y, -sizeHalf.z);
	pVtx[3].pos = D3DXVECTOR3(+sizeHalf.x, -sizeHalf.y, -sizeHalf.z);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================================
//�T�C�Y�̎擾
//================================================
D3DXVECTOR3 CObject3D::GetSize()
{
	return m_size;
}

//================================================
//�ړ��ʂ̐ݒ�
//================================================
void CObject3D::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//================================================
//�ړ��ʂ̎擾
//================================================
D3DXVECTOR3 CObject3D::GetMove()
{
	return m_move;
}

//================================================
//�e�N�X�`���̐ݒ�
//================================================
void CObject3D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//================================================
//�e�N�X�`�����W�̐ݒ�(�e�N�X�`���A�j���[�V�����p)
//================================================
void CObject3D::SetTexUV(int nDivNumU, int nDivNumV, int nPtnAnim)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//��Z�p��float�ɕϊ�
 	float fDivideU = (1.0f / nDivNumU);	//U����
	float fDivideV = (1.0f / nDivNumV);	//V����

	//V���W�����i�ڂ�
	int nNumV = (nPtnAnim / nDivNumU);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(fDivideU * nPtnAnim,		 fDivideV * nNumV);
	pVtx[1].tex = D3DXVECTOR2(fDivideU * (nPtnAnim + 1), fDivideV * nNumV);
 	pVtx[2].tex = D3DXVECTOR2(fDivideU * nPtnAnim,		 fDivideV * (nNumV + 1));
	pVtx[3].tex = D3DXVECTOR2(fDivideU * (nPtnAnim + 1), fDivideV * (nNumV + 1));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================================
//�e�N�X�`�����W�̐ݒ�(�X�N���[���p)
//================================================
void CObject3D::SetTexUV(bool bDirection, float fMove)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!bDirection)
	{//X���̃X�N���[��������ꍇ
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex.x += fMove;
		pVtx[1].tex.x += fMove;
		pVtx[2].tex.x += fMove;
		pVtx[3].tex.x += fMove;

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
		return;
	}
	
	/* Y���̃X�N���[��������ꍇ */

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex.y += fMove;
	pVtx[1].tex.y += fMove;
	pVtx[2].tex.y += fMove;
	pVtx[3].tex.y += fMove;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================================
//�F�̐ݒ�
//================================================
void CObject3D::SetCol(const D3DXCOLOR &col)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================================
//�����蔻��
//================================================
bool CObject3D::Collision(OBJ_TYPE myType, OBJ_TYPE targetType)
{
	//for (int i = 0; i < MAX_OBJECT; i++)
	//{
	//	CObject* pObjectTarget = GetObjects(i);	//�Ώۂ̏��̎擾

	//	if (pObjectTarget == nullptr || pObjectTarget == this)
	//	{//NULL�`�F�b�N
	//		continue;
	//	}

	//	/* �unullptr�ł͂Ȃ��v���u�ΏۂƎ��g�������ł͂Ȃ��v�ꍇ */

	//	CObject::OBJ_TYPE typeTarget = pObjectTarget->GetObjType();	//�^�C�v�̎擾

	//	if (!(((this->GetObjType() == myType) && (typeTarget == targetType))))
	//	{//�u���g�̃^�C�v�v�Ɓu�Ώۂ̃^�C�v�v�������ƈ�v���Ȃ��ꍇ
	//		continue;
	//	}

	//	/* �u���g�̃^�C�v�v�Ɓu�Ώۂ̃^�C�v�v�������ƈ�v�����ꍇ */

	//	//���g�̏������ꂼ��擾
	//	D3DXVECTOR3 posMyself = this->GetPos();		//�ʒu
	//	D3DXVECTOR2 sizeMyself = this->GetSize();	//�T�C�Y

	//	/* ���g�̔���p */
	//	float fLeft		= (posMyself.x - (sizeMyself.x * 0.5f));	//���[
	//	float fRight	= (posMyself.x + (sizeMyself.x * 0.5f));	//�E�[
	//	float fTop		= (posMyself.y + (sizeMyself.y * 0.5f));	//��[
	//	float fBottom	= (posMyself.y - (sizeMyself.y * 0.5f));	//���[

	//	CObject3D* pObjTarget3D = (CObject3D*)pObjectTarget;	//Object3D�^�ɃL���X�g

	//	//�Ώۂ̏������ꂼ��擾
	//	D3DXVECTOR3 posTarget = pObjTarget3D->GetPos();	//�ʒu
	//	D3DXVECTOR2 sizeTarget = pObjTarget3D->GetSize();	//�T�C�Y

	//	/* �Ώۂ̔���p */
	//	float fLeftTarget	= (posTarget.x - (sizeTarget.x * 0.5f));	//���[
	//	float fRightTarget	= (posTarget.x + (sizeTarget.x * 0.5f));	//�E�[
	//	float fTopTarget	= (posTarget.y + (sizeTarget.y * 0.5f));	//��[
	//	float fBottomTarget = (posTarget.y - (sizeTarget.y * 0.5f));	//���[

	//	if (fLeft < fRightTarget
	//		&& fRight > fLeftTarget
	//		&& fTop > fBottomTarget
	//		&& fBottom < fTopTarget)
	//	{//�u���g�v���u�Ώۂ͈͓̔��v�ɗ����ꍇ
	//		return true;	//�u���������v��Ԃ�
	//	}
	//}

	return false;	//�u�������Ă��Ȃ��v��Ԃ�
}