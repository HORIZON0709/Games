//================================================
//
//3D�Q�[��(��)[door.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "door.h"
#include "application.h"
#include "renderer.h"
#include "line.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//================================================
//����
//================================================
CDoor* CDoor::Create(char* pFileName)
{
	CDoor* pDoor = nullptr;	//�|�C���^

	if (pDoor != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pDoor = new CDoor;	//�������̓��I�m��

	pDoor->SetFileName(pFileName);	//�t�@�C�����̐ݒ�

	pDoor->Init();	//������

	return pDoor;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CDoor::CDoor() :CObjectX::CObjectX(),
	m_dir(CStage::DIRECTION::DIR_NONE)
{
	//�����o�ϐ��̃N���A
	memset(m_apLine, 0, sizeof(m_apLine));
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::DOOR);
}

//================================================
//�f�X�g���N�^
//================================================
CDoor::~CDoor()
{
}

//================================================
//������
//================================================
HRESULT CDoor::Init()
{
	//�e�N���X�̏�����
	CObjectX::Init();

	//�����o�ϐ��̏�����
	m_dir = CStage::DIRECTION::DIR_NONE;

	for (int i = 0; i < MAX_LINE; i++)
	{
		//���C���̐���
		m_apLine[i] = CLine::Create();
	}

	return S_OK;
}

//================================================
//�I��
//================================================
void CDoor::Uninit()
{
	for (int i = 0; i < MAX_LINE; i++)
	{//���C��
		if (m_apLine[i] != nullptr)
		{
			m_apLine[i]->Uninit();	//�I��
			delete m_apLine[i];		//�������̉��
			m_apLine[i] = nullptr;	//nullptr�ɂ���
		}
	}

	//�e�N���X�̏I��
	CObjectX::Uninit();
}

//================================================
//�X�V
//================================================
void CDoor::Update()
{
	//�e�N���X�̍X�V
	CObjectX::Update();

	//���C���̐ݒ�܂Ƃ�
	SetLines();
}

//================================================
//�`��
//================================================
void CDoor::Draw()
{
	//�e�N���X�̕`��
	CObjectX::Draw();

	for (int i = 0; i < MAX_LINE; i++)
	{
		//���C���̕`��
		m_apLine[i]->Draw();
	}
}

//================================================
//�����̐ݒ�
//================================================
void CDoor::SetDir(CStage::DIRECTION dir)
{
	m_dir = dir;
}

//================================================
//�����̎擾
//================================================
CStage::DIRECTION CDoor::GetDir()
{
	return m_dir;
}

//================================================
//���C���̐ݒ�܂Ƃ�
//================================================
void CDoor::SetLines()
{
	//�e�����擾
	D3DXVECTOR3 pos = CObjectX::GetPos();		//�ʒu
	D3DXVECTOR3 rot = CObjectX::GetRot();		//����
	D3DXVECTOR3 vtxMax = CObjectX::GetVtxMax();	//���_�̍ő�l
	D3DXVECTOR3 vtxMin = CObjectX::GetVtxMin();	//���_�̍ŏ��l

	//�F(�S�Ă̐��œ����F)
	D3DXCOLOR col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	//���Ԗڂ�
	int nNum = 0;

	//********** �㕔���� **********//

	//�n�_�E�I�_
	D3DXVECTOR3 start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	D3DXVECTOR3 end = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �㕔��O�� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �㕔�E�� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �㕔���� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �������� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ������O�� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �����E�� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �������� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ��O���� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ��O�E�� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ������ **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ���E�� **********//

	//�n�_�E�I�_
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);

	//�ݒ�
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�
}

