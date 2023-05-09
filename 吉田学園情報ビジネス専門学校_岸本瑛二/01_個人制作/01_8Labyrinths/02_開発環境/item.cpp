//================================================
//
//3D�Q�[��(��)[item.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "item.h"
#include "application.h"
#include "renderer.h"
#include "line.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const float CItem::ROTATION_SPEED = 0.05f;	//��]���x

//================================================
//����
//================================================
CItem* CItem::Create(char* pFileName)
{
	CItem* pItem = nullptr;	//�|�C���^

	if (pItem != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pItem = new CItem;	//�������̓��I�m��

	pItem->SetFileName(pFileName);	//�t�@�C�����̐ݒ�

	pItem->Init();	//������

	return pItem;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CItem::CItem() :CObjectX::CObjectX(),
	m_isStage(CStage::STAGE::NONE),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//�����o�ϐ��̃N���A
	memset(m_apLine, 0, sizeof(m_apLine));
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::ITEM);
}

//================================================
//�f�X�g���N�^
//================================================
CItem::~CItem()
{
}

//================================================
//������
//================================================
HRESULT CItem::Init()
{
	//�e�N���X�̏�����
	CObjectX::Init();

	//�����o�ϐ��̏�����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
void CItem::Uninit()
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
void CItem::Update()
{
	//�e�N���X�̍X�V
	CObjectX::Update();

	//��]
	m_rot.y += ROTATION_SPEED;

	//�p�x�̐��K��
	NormalizeAngle(&m_rot.y);

	//������ݒ�
	CObjectX::SetRot(m_rot);

#ifdef _DEBUG
	//�e����\��
	CDebugProc::Print("\n�s Item �t\n");
	CDebugProc::Print("m_rot:[%f,%f,%f]\n", m_rot.x, m_rot.y, m_rot.z);

	//���C���̐ݒ�܂Ƃ�
	SetLines();

#endif // _DEBUG
}

//================================================
//�`��
//================================================
void CItem::Draw()
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
//���C���̐ݒ�܂Ƃ�
//================================================
void CItem::SetLines()
{
	//�e�����擾
	D3DXVECTOR3 pos = CObjectX::GetPos();		//�ʒu
	D3DXVECTOR3 rot = CObjectX::GetRot();		//����
	D3DXVECTOR3 vtxMax = CObjectX::GetVtxMax();	//���_�̍ő�l
	D3DXVECTOR3 vtxMin = CObjectX::GetVtxMin();	//���_�̍ŏ��l

	//�F(�S�Ă̐��œ����F)
	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

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

//================================================
//�ǂ̃X�e�[�W�ɂ��邩��ݒ�
//================================================
void CItem::SetIsStage(CStage::STAGE isStage)
{
	m_isStage = isStage;
}