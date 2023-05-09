//================================================
//
//3D�Q�[��(��)[objectMesh.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "objectMesh.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const int CObjectMesh::NUM_VERTEX = 3;	//1�|���S���̒��_��

const int CObjectMesh::NUM_BLK_X = 20;	//�u���b�N��(X��)
const int CObjectMesh::NUM_BLK_Z = 20;	//�u���b�N��(Z��)

const int CObjectMesh::NUM_VTX_X = (NUM_BLK_X + 1);	//���_��(X��)
const int CObjectMesh::NUM_VTX_Z = (NUM_BLK_Z + 1);	//���_��(Z��)

const float CObjectMesh::MESH_SIZE = 80.0f;	//���b�V���̃T�C�Y

//================================================
//����
//================================================
CObjectMesh* CObjectMesh::Create()
{
	CObjectMesh* pObjectMesh = nullptr;	//�|�C���^

	if (pObjectMesh != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pObjectMesh = new CObjectMesh;	//�������̓��I�m��

	pObjectMesh->Init();	//������

	return pObjectMesh;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CObjectMesh::CObjectMesh() : CObject::CObject(CObject::PRIORITY::PRIO_MESH),
	m_pVtxBuff(nullptr),
	m_pIdxBuff(nullptr),
	m_texture(CTexture::TEXTURE::NONE),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_fWidth(0.0f),
	m_fHeight(0.0f),
	m_nNumVtx(0),
	m_nNumIdx(0),
	m_nNumPol(0)
{
	//�����o�ϐ��̃N���A
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
}

//================================================
//�f�X�g���N�^
//================================================
CObjectMesh::~CObjectMesh()
{
	/* ����R��̊m�F */
	assert(m_pVtxBuff == nullptr);
	assert(m_pIdxBuff == nullptr);
}

//================================================
//������
//================================================
HRESULT CObjectMesh::Init()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�����o�ϐ��̏����ݒ�
	m_texture = CTexture::TEXTURE::NONE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = MESH_SIZE;
	m_fHeight = MESH_SIZE;

	m_nNumVtx = AskVtx();	//���_��
	m_nNumIdx = AskIdx();	//�C���f�b�N�X��
	m_nNumPol = AskPol();	//�|���S����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(CObject3D::VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(CObject3D::VERTEX_3D) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//***** ���_���̏����� *****//

	CObject3D::VERTEX_3D* pVtx = nullptr;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < m_nNumVtx; i++)
	{
		float fX = (float)(((i % NUM_VTX_X) - (NUM_BLK_X * 0.5f)) * MESH_SIZE);			//X���W
		float fZ = (float)(((i / NUM_VTX_X) - (NUM_BLK_X * 0.5f)) * MESH_SIZE * -1.0f);	//Z���W

		float fY = sinf(D3DX_PI / 3 * (i % NUM_VTX_X)) * 30.0f;

		//���_���W�̐ݒ�
		pVtx[i].pos = D3DXVECTOR3(fX, fY, fZ);

		//�e���_�̖@���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fU = (float)(i % NUM_VTX_X);	//U���W
		float fV = (float)(i / NUM_VTX_X);	//V���W

		//�e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(fU, fV);
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//***** �C���f�b�N�X���̐ݒ� *****//

	WORD* pIdx = nullptr;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int z = 0, n = 0; z < NUM_BLK_Z; z++)
	{
		for (int x = 0; x < NUM_VTX_X; x++)
		{
			n = (x * 2) + ((z * 2) * NUM_VTX_X) + (z * 2);	//�Z������

			pIdx[n] = (WORD)(x + (z * NUM_VTX_X) + NUM_VTX_X);
			pIdx[n + 1] = (WORD)(x + (z * NUM_VTX_X));
		}

		if ((n + 1) < m_nNumIdx)
		{//���ɂ܂��i�������Ă��鎞
			n = (NUM_VTX_X * 2) + (((NUM_BLK_X + 2) * 2) * z);	//�Z������

			pIdx[n] = pIdx[n - 1];
			pIdx[n + 1] = (WORD)((NUM_VTX_X * 2) + (z * NUM_VTX_X));
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	//***** �@���̐ݒ� *****//

	D3DXMATRIX mtxTrans, mtxWorld;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	pVtx = nullptr;	//���_���ւ̃|�C���^��nullptr��

	pIdx = nullptr;	//�C���f�b�N�X���ւ̃|�C���^��nullptr��

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0); 

	//�C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < m_nNumPol; i++)
	{
		if (pIdx[i] == pIdx[i + 1] ||
			pIdx[i] == pIdx[i + 2] ||
			pIdx[i + 1] == pIdx[i + 2])
		{//�k�ރ|���S�����`�����Ă��钸�_�������ꍇ
			continue;
		}

		/* ��L�ȊO�̒��_�̏ꍇ */

		D3DXVECTOR3 vecPos[NUM_VERTEX];	//�x�N�g���̈ʒu

		memset(vecPos, 0, sizeof(vecPos));

		for (int nVtx = 0; nVtx < NUM_VERTEX; nVtx++)
		{
			int nIdx = pIdx[i + nVtx];	//�C���f�b�N�X�ԍ�

			//�C���f�b�N�X�ԍ��ɉ������|���S���̒��_�̈ʒu����
			vecPos[nVtx] = pVtx[nIdx].pos;
		}

		D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʖ@���x�N�g��

		//�x�N�g�������߂�
		D3DXVECTOR3 vec1 = vecPos[1] - vecPos[0];
		D3DXVECTOR3 vec2 = vecPos[2] - vecPos[0];

		//�@�������߂�
		D3DXVec3Cross(&nor, &vec1, &vec2);

		if (i % 2 != 0)
		{
			nor *= -1;	//�x�N�g�������𔽓]
		}

		//���K��
		D3DXVec3Normalize(&nor, &nor);

		//���_���̖ʖ@���x�N�g�������Z
		pVtx[pIdx[i + 0]].nor += nor;
		pVtx[pIdx[i + 1]].nor += nor;
		pVtx[pIdx[i + 2]].nor += nor;

		//���K��
		D3DXVec3Normalize(&pVtx[pIdx[i + 0]].nor, &pVtx[pIdx[i + 0]].nor);
		D3DXVec3Normalize(&pVtx[pIdx[i + 1]].nor, &pVtx[pIdx[i + 1]].nor);
		D3DXVec3Normalize(&pVtx[pIdx[i + 2]].nor, &pVtx[pIdx[i + 2]].nor);
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================
//�I��
//================================================
void CObjectMesh::Uninit()
{
	//���_�o�b�t�@�̉��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//================================================
//�X�V
//================================================
void CObjectMesh::Update()
{
}

//================================================
//�`��
//================================================
void CObjectMesh::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	
	//���C�g��؂�
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CObject3D::VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CApplication::GetTexture();	//�e�N�X�`�����擾

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0, 0,
		m_nNumVtx,	//���_��
		0,			//�`�悷��ŏ��̒��_�C���f�b�N�X
		m_nNumPol);	//�`�悷��v���~�e�B�u��
	
	//���C�g��t����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, nullptr);
}

//================================================
//�ʒu��ݒ�
//================================================
void CObjectMesh::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//================================================
//�ʒu���擾
//================================================
D3DXVECTOR3 CObjectMesh::GetPos()
{
	return m_pos;
}

//================================================
//������ݒ�
//================================================
void CObjectMesh::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//================================================
//�������擾
//================================================
D3DXVECTOR3 CObjectMesh::GetRot()
{
	return m_rot;
}

//================================================
//�����蔻��
//================================================
bool CObjectMesh::Collision(D3DXVECTOR3* pPos)
{
	D3DXMATRIX mtxTrans, mtxWorld;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	CObject3D::VERTEX_3D* pVtx = nullptr;	//���_���ւ̃|�C���^

	WORD* pIdx = nullptr;	//�C���f�b�N�X���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	bool bCollsion = false;	//�����������ǂ���

	for (int i = 0; i < m_nNumPol; i++)
	{
		if (pIdx[i] == pIdx[i + 1] ||
			pIdx[i] == pIdx[i + 2] ||
			pIdx[i + 1] == pIdx[i + 2])
		{//�k�ރ|���S�����`�����Ă��钸�_�������ꍇ
			continue;
		}

		/* ��L�ȊO�̒��_�̏ꍇ */

		D3DXVECTOR3 vecPos[NUM_VERTEX] = {};	//�x�N�g���̈ʒu

		for (int nVtx = 0; nVtx < NUM_VERTEX; nVtx++)
		{
			int nIdx = pIdx[i + nVtx];	//�C���f�b�N�X�ԍ�

			//�C���f�b�N�X�ԍ��ɉ��������_�̈ʒu����
			vecPos[nVtx] = pVtx[nIdx].pos;
		}

		for (int nVtx = 0; nVtx < NUM_VERTEX; nVtx++)
		{
			//�ʒu�̔��f
			D3DXVec3TransformCoord(&vecPos[nVtx], &vecPos[nVtx], &mtxWorld);
		}

		int nInLine = 0;	//�|���S���̓����ɂ��邩�ǂ���

		for (int nVtx = 0; nVtx < NUM_VERTEX; nVtx++)
		{
			//���_�Ԃ̃x�N�g�������߂�
			D3DXVECTOR3 vec = vecPos[(nVtx + 1) % NUM_VERTEX] - vecPos[nVtx];

			//���݂̈ʒu�Ǝn�_�܂ł̃x�N�g��
			D3DXVECTOR3 diffVec = *pPos - vecPos[nVtx];

			//�O�όv�Z
			float fVecLine = Vec2Cross(&diffVec, &vec);

			if ((i % 2 == 0 && fVecLine > 0.0f) ||
				(i % 2 != 0 && fVecLine < 0.0f))
			{//�����ɂ���
				nInLine++;	//���肵����ӂ��J�E���g����
			}
			else
			{//�O���ɂ���
				break;
			}
		}

		if (nInLine == NUM_VERTEX)
		{//�O�p�`�̒��ɂ���
			D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�@���x�N�g��

			//�x�N�g�������߂�
			D3DXVECTOR3 vec1 = vecPos[1] - vecPos[0];
			D3DXVECTOR3 vec2 = vecPos[2] - vecPos[0];

			//�@�������߂�
			D3DXVec3Cross(&nor, &vec1, &vec2);

			//���K��
			D3DXVec3Normalize(&nor, &nor);

			//�ʒu�̐ݒ�
			float fPosX = nor.x * (pPos->x - vecPos[0].x);
			float fPosZ = nor.z * (pPos->z - vecPos[0].z);

			//Y���W�����߂�
			pPos->y = (vecPos[0].y - ((fPosX + fPosZ) / nor.y));
			
			//��������
			bCollsion = true;

			if (bCollsion)
			{//�������Ă�����
				//���������|���S���̐F��ς���
				pVtx[pIdx[i + 0]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[pIdx[i + 1]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[pIdx[i + 2]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return bCollsion;	//�����Ԃ�
}

//================================================
// 2D�x�N�g���̓���
//================================================
float CObjectMesh::Vec2Dot(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2)
{
	return (vec1->x * vec2->x) + (vec1->z * vec2->z);
}

//================================================
// 2D�x�N�g���̊O��
//================================================
float CObjectMesh::Vec2Cross(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2)
{
	return (vec1->x * vec2->z) - (vec1->z * vec2->x);
}

//================================================
//���_�������߂�
//================================================
int CObjectMesh::AskVtx()
{
	// (X���̒��_��) * (Z���̒��_��) ---> [���_��] = [�u���b�N��] + 1
	int nVtx = NUM_VTX_X * NUM_VTX_Z;

	return nVtx;	//���߂����_����Ԃ�
}

//================================================
//�C���f�b�N�X�������߂�
//================================================
int CObjectMesh::AskIdx(void)
{
	// (X����1�i�ڂ̒��_��) * (Z���̃u���b�N����) + (�k�ރ|���S���p�̒��_��)
	int nIdx = (NUM_VTX_X * 2) * NUM_BLK_Z + ((NUM_BLK_Z - 1) * 2);

	return nIdx;	//���߂��C���f�b�N�X����Ԃ�
}

//================================================
//�|���S���������߂�
//================================================
int CObjectMesh::AskPol(void)
{
	// [(X���̃u���b�N��) * (Z���̃u���b�N��) * (�u���b�N��ɂ��|���S��2��)] + (�k�ރ|���S��)
	int nPol = (NUM_BLK_X * NUM_BLK_Z * 2) + ((NUM_BLK_Z - 1) * 4);

	return nPol;	//���߂��|���S������Ԃ�
}