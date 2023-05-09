//================================================
//
//3D�Q�[��(��)[model.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "model.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const int CModel::MAX_WORD = 256;	//�ő啶����

const char* CModel::FILE_NAME = "data/TEXT/motion.txt";	//�t�@�C����

//***************************
//�ÓI�����o�ϐ�
//***************************
CModel::CHARACTER_SET CModel::m_characterSet = {};	//�L�����N�^�[���

CModel::MODEL_INFO CModel::m_aModelInfo[NUM_PARTS] = {};	//���f�����

//================================================
//����
//================================================
CModel* CModel::Create()
{
	CModel* pModel = nullptr;	//�|�C���^

	if (pModel != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pModel = new CModel;	//�������̓��I�m��

	pModel->Init();	//������

	return pModel;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CModel::CModel():
	m_nNumModel(0)
{
	//�����o�ϐ��̃N���A
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
}

//================================================
//�f�X�g���N�^
//================================================
CModel::~CModel()
{
}

//================================================
//������
//================================================
HRESULT CModel::Init()
{
	//�����o�ϐ��̏����ݒ�
	m_nNumModel = 0;

	//�t�@�C���ǂݍ���
	Load();

	for (int i = 0; i < m_nNumModel; i++)
	{
		//���f�����̈ʒu�ƌ����ɁA�L�����N�^�[�ݒ�̈ʒu�ƌ�������
		m_aModelInfo[i].pos = m_characterSet.aPartsSet[i].pos;
		m_aModelInfo[i].rot = m_characterSet.aPartsSet[i].rot;

		//���_���̎擾
		int nNumVtx = m_aModelInfo[i].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		DWORD sizeFVF = D3DXGetFVFVertexSize(m_aModelInfo[i].pMesh->GetFVF());

		BYTE *pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

		//���_�o�b�t�@�̃��b�N
		m_aModelInfo[i].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int j = 0; j < nNumVtx; j++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			/* ���_���W���r���A���f���̍ő�l�E�ŏ��l���擾 */

			//***** �ő� *****//

			if (vtx.x > m_aModelInfo[i].vtxMax.x)
			{//X
				m_aModelInfo[i].vtxMax.x = vtx.x;
			}

			if (vtx.y > m_aModelInfo[i].vtxMax.y)
			{//Y
				m_aModelInfo[i].vtxMax.y = vtx.y;
			}

			if (vtx.z > m_aModelInfo[i].vtxMax.z)
			{//Z
				m_aModelInfo[i].vtxMax.z = vtx.z;
			}

			//***** �ŏ� *****//

			if (vtx.x < m_aModelInfo[i].vtxMin.x)
			{//X
				m_aModelInfo[i].vtxMin.x = vtx.x;
			}

			if (vtx.y < m_aModelInfo[i].vtxMin.y)
			{//Y
				m_aModelInfo[i].vtxMin.y = vtx.y;
			}

			if (vtx.z < m_aModelInfo[i].vtxMin.z)
			{//Z
				m_aModelInfo[i].vtxMin.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		m_aModelInfo[i].pMesh->UnlockVertexBuffer();
	}

	return S_OK;
}

//================================================
//�I��
//================================================
void CModel::Uninit()
{
	for (int i = 0; i < m_nNumModel; i++)
	{
		//���b�V���̉��
		if (m_aModelInfo[i].pMesh != nullptr)
		{
			m_aModelInfo[i].pMesh = nullptr;
		}

		//�}�e���A���̉��
		if (m_aModelInfo[i].pBuffMat != nullptr)
		{
			m_aModelInfo[i].pBuffMat = nullptr;
		}
	}
}

//================================================
//�X�V
//================================================
void CModel::Update()
{
}

//================================================
//�`��
//================================================
void CModel::Draw()
{
	for (int i = 0; i < m_nNumModel; i++)
	{
		//�L�����N�^�[�ݒ�̏��
		D3DXVECTOR3 pos = m_characterSet.aPartsSet[i].pos + m_aModelInfo[i].pos;	//�ʒu
		D3DXVECTOR3 rot = m_characterSet.aPartsSet[i].rot + m_aModelInfo[i].rot;	//����

		D3DXMATRIX mtxRot, mtxTrans, mtxParent;	//�v�Z�p�}�g���b�N�X

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//���f���̉e��`��
		//DrawShadow();

		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		if(m_characterSet.aPartsSet[i].nParent == -1)
		{//���g���e���f���̏ꍇ
			//���݂̃}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		//���[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
		D3DXMATERIAL* pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

		//���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^��ێ�
		pMat = (D3DXMATERIAL*)m_aModelInfo[i].pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)m_aModelInfo[i].numMat; j++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[j].MatD3D);

			//���f���p�[�c�̕`��
			m_aModelInfo[i].pMesh->DrawSubset(j);
		}

		//�ێ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//================================================
//���[���h�}�g���b�N�X�̎擾
//================================================
D3DXMATRIX CModel::GetMtxWorld()
{
	return m_mtxWorld;
}

//================================================
//���f���̊e���̎擾
//================================================
CModel::MODEL_INFO CModel::GetModelInfo(int nNum)
{
	return m_aModelInfo[nNum];
}

//================================================
//���f���̈ʒu����ݒ�
//================================================
void CModel::SetModelInfo_Pos(int nNum, const D3DXVECTOR3 &pos)
{
	m_aModelInfo[nNum].pos = pos;
}

//================================================
//���f���̌�������ݒ�
//================================================
void CModel::SetModelInfo_Rot(int nNum, const D3DXVECTOR3 &rot)
{
	m_aModelInfo[nNum].rot = rot;
}

//================================================
//�e�̕`��
//================================================
void CModel::DrawShadow()
{
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, nor;

	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̕�����ݒ�
	vecLight = D3DXVECTOR4(-1.0f, 0.8f, 0.0f, 0.0f);

	//�ʒu��ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�@����ݒ�
	nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�@���ƕ��ʂ�1�_���畽�ʂ��쐬
	D3DXPlaneFromPointNormal(&planeField, &pos, &nor);

	//���C�g�ƕ��ʂ���e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//���[���h�}�g���b�N�X�ƃV���h�E�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &m_mtxWorld);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < m_nNumModel; i++)
	{
		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
		D3DXMATERIAL* pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

		//���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^��ێ�
		pMat = (D3DXMATERIAL*)m_aModelInfo[i].pBuffMat->GetBufferPointer();
		D3DMATERIAL9 mat = pMat->MatD3D;

		//�F�����ɐݒ�
		mat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		mat.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&mat);

		for (int i = 0; i < (int)m_aModelInfo[i].numMat; i++)
		{
			//���f���p�[�c�̕`��
			m_aModelInfo[i].pMesh->DrawSubset(i);
		}

		//�ێ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//================================================
//�ǂݍ���
//================================================
void CModel::Load()
{
	//�t�@�C�����J��
	FILE* pFile = fopen(FILE_NAME, "r");
	
	if (pFile == nullptr)
	{//�t�@�C�����J���Ȃ������ꍇ
		assert(false);
	}

	/* �t�@�C�����J�����ꍇ */

	char aText[MAX_WORD];	//�e�L�X�g�i�[�p

	while (strncmp(&aText[0], "SCRIPT", 6) != 0)
	{//�e�L�X�g�̍ŏ��̍s��ǂݍ��ނ܂ŌJ��Ԃ�
		fgets(aText, MAX_WORD, pFile);	//1�s�ۂ��Ɠǂݍ���
	}

	char aFileName[NUM_PARTS][MAX_WORD] = {};
	int nCnt = 0;

	while (strcmp(&aText[0], "END_SCRIPT") != 0)
	{//�e�L�X�g�̍ŏI�s��ǂݍ��ނ܂ŌJ��Ԃ�
		//������ǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//�u���b�N�R�����g
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//�R�����g
			//1�s�S�ēǂݍ���
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "NUM_MODEL") == 0)
		{//���f����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//���f������ǂݍ���
			fscanf(pFile, "%d", &m_nNumModel);
		}
		else if (strcmp(&aText[0], "MODEL_FILENAME") == 0)
		{//�t�@�C����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", aText);

			//X�t�@�C���̃p�X��ǂݍ���
			fscanf(pFile, "%s", &aFileName[nCnt][0]);

			//�J�E���g�A�b�v
			nCnt++;
		}
		else if (strcmp(&aText[0], "CHARACTERSET") == 0)
		{//�L�����N�^�[�Z�b�g
			//�L�����N�^�[�ݒ�
			Set_CharacterSet(pFile);
		}
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < m_nNumModel; i++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			&aFileName[i][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_aModelInfo[i].pBuffMat,
			NULL,
			&m_aModelInfo[i].numMat,
			&m_aModelInfo[i].pMesh);
	}

	//�t�@�C�������
	fclose(pFile);
}

//================================================
//�L�����N�^�[�ݒ�
//================================================
void CModel::Set_CharacterSet(FILE* pFile)
{
	int nCnt = 0;
	char aText[MAX_WORD];	//�e�L�X�g�i�[�p

	while (strcmp(&aText[0], "END_CHARACTERSET") != 0)
	{//�L�����N�^�[�Z�b�g���I���܂ŌJ��Ԃ�
		//������ǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//�u���b�N�R�����g
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//�R�����g
			//1�s�S�ēǂݍ���
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "NUM_PARTS") == 0)
		{//�p�[�c��
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�L�[����ǂݍ���
			fscanf(pFile, "%d", &m_characterSet.nNumParts);
		}
		else if (strcmp(&aText[0], "PARTSSET") == 0)
		{//�p�[�c�Z�b�g
			//�p�[�c�Z�b�g�ݒ�
			Set_PartsSet(pFile, &m_characterSet.aPartsSet[nCnt]);

			//�p�[�c���J�E���g�𑝉�
			nCnt++;
		}
	}
}

//================================================
//�p�[�c�ݒ�
//================================================
void CModel::Set_PartsSet(FILE* pFile, PARTS_SET* pParts)
{
	char aText[MAX_WORD];	//�e�L�X�g�i�[�p

	while (strcmp(&aText[0], "END_PARTSSET") != 0)
	{//�L�����N�^�[�Z�b�g���I���܂ŌJ��Ԃ�
		//������ǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//�u���b�N�R�����g
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//�R�����g
			//1�s�S�ēǂݍ���
			fgets(aText, MAX_WORD, pFile);
			continue;
		}
		
		if (strcmp(&aText[0], "INDEX") == 0)
		{//�C���f�b�N�X��
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�C���f�b�N�X����ǂݍ���
			fscanf(pFile, "%d", &pParts->nIndex);
		}
		else if (strcmp(&aText[0], "PARENT") == 0)
		{//�e�p�[�c�ԍ�
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�e�p�[�c�ԍ���ǂݍ���
			fscanf(pFile, "%d", &pParts->nParent);
		}
		else if (strcmp(&aText[0], "POS") == 0)
		{//�ʒu
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�ʒu��ǂݍ���
			fscanf(pFile, "%f", &pParts->pos.x);
			fscanf(pFile, "%f", &pParts->pos.y);
			fscanf(pFile, "%f", &pParts->pos.z);
		}
		else if (strcmp(&aText[0], "ROT") == 0)
		{//����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//������ǂݍ���
			fscanf(pFile, "%f", &pParts->rot.x);
			fscanf(pFile, "%f", &pParts->rot.y);
			fscanf(pFile, "%f", &pParts->rot.z);
		}
	}
}