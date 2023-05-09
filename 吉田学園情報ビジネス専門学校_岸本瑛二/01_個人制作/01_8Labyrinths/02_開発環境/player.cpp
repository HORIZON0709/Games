//================================================
//
//3D�Q�[��(��)[player.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "player.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "model.h"
#include "gimmick.h"
#include "line.h"
#include "item.h"
#include "fade.h"
#include "door.h"
#include "score.h"
#include "panel.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//***************************
//namespace
//***************************
namespace
{
CStage::DIRECTION dirDoor = CStage::DIRECTION::DIR_NONE;	//�G�ꂽ�h�A�̕���
}

//***************************
//�萔�̒�`
//***************************
const float CPlayer::MOVE_SPEED = 2.5f;		//�ړ����x
const float CPlayer::ROT_SMOOTHNESS = 0.5f;	//��]�̊��炩��
const float CPlayer::LIMIT_POS = 315.0f;	//�ړ������ʒu

const int CPlayer::MAX_WORD = 256;	//�ő啶����

const char* CPlayer::FILE_NAME = "data/TEXT/motion.txt";	//�t�@�C����

//***************************
//�ÓI�����o�ϐ�
//***************************
CPlayer::MOTION_SET CPlayer::m_aMotion[MOTION_TYPE::MAX] ={};

CLine* CPlayer::m_apLine[MAX_LINE] = {};	//���C���̃|�C���^

//================================================
//����
//================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = nullptr;	//�|�C���^

	if (pPlayer != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pPlayer = new CPlayer;	//�������̓��I�m��

	pPlayer->Init();	//������

	return pPlayer;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CPlayer::CPlayer() :CObject::CObject(CObject::PRIORITY::PRIO_MODEL),
	m_pModel(nullptr),
	m_pItem(nullptr),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_motionType(MOTION_TYPE::NONE),
	m_nCurrentKey(0),
	m_nCntMotion(0),
	m_nNumMotion(0),
	m_nNumKeySet(0),
	m_nNumKey(0),
	m_nGetItem(0),
	m_bPressKey(false),
	m_bCollObjectX(false),
	m_bCollGimmick(false),
	m_bCollDoor(false),
	m_bCollPanelStand(false),
	m_bGetItem(false),
	m_bCanMove(false)
{
	//�����o�ϐ��̃N���A
	for (int i = 0; i < CStage::MAX_COIN; i++)
	{
		m_aStageCoin[i] = CStage::STAGE::NONE;
	}

	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
	
	for (int i = 0; i < NUM_VTX_3D; i++)
	{
		m_aPosVtx[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::PLAYER);
}

//================================================
//�f�X�g���N�^
//================================================
CPlayer::~CPlayer()
{
}

//================================================
//������
//================================================
HRESULT CPlayer::Init()
{
	//���f���̐���
	m_pModel = CModel::Create();

	//���_�̍ő�l�ƍŏ��l��ݒ�
	SetVtxMaxAndMin();

	//�ǂݍ���
	Load();

	//�����o�ϐ��̏�����
	m_pItem = nullptr;

	for (int i = 0; i < CStage::MAX_COIN; i++)
	{
		m_aStageCoin[i] = CStage::STAGE::NONE;
	}

	m_pos = D3DXVECTOR3(0.0f, 5.0f, -150.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/*
		���_�̍ő�l�E�ŏ��l����A���f���͈̔͂�\�����_�̈ʒu��ݒ肷��
		�R�����g ---> ���ʂ��猩�����_�̈ʒu
	*/
	/* ���� */
	m_aPosVtx[0] = m_vtxMin;										//��O�̉���(�ŏ��l)
	m_aPosVtx[1] = D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z);	//���̉���
	m_aPosVtx[2] = D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z);	//��O�̏㑤
	m_aPosVtx[3] = D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z);	//���̏㑤

	/* �E�� */
	m_aPosVtx[4] = D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z);	//��O�̉���
	m_aPosVtx[5] = D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMax.z);	//���̉���
	m_aPosVtx[6] = D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z);	//��O�̏㑤
	m_aPosVtx[7] = m_vtxMax;										//���̏㑤(�ő�l)

	m_motionType = MOTION_TYPE::NEUTRAL;

	m_nCurrentKey = 0;
	m_nCntMotion = 0;
	m_nNumMotion = 0;
	m_nNumKeySet = 0;
	m_nNumKey = 0;
	m_nGetItem = 0;
	m_bPressKey = false;
	m_bCollObjectX = false;
	m_bCollGimmick = false;
	m_bCollDoor = false;
	m_bCollPanelStand = false;
	m_bGetItem = false;
	m_bCanMove = false;

	for (int i = 0; i < MAX_LINE; i++)
	{
		m_apLine[i] = CLine::Create();
	}

	return S_OK;
}

//================================================
//�I��
//================================================
void CPlayer::Uninit()
{
	for (int i = 0; i < MAX_LINE; i++)
	{
		if (m_apLine[i] != nullptr)
		{
			m_apLine[i]->Uninit();
			delete m_apLine[i];
			m_apLine[i] = nullptr;
		}
	}
}

//================================================
//�X�V
//================================================
void CPlayer::Update()
{
	//�ړ�
	Move();

	//���[�V����
	Motion();

	//�����蔻��
	Collision();

	//�X�e�[�W�؂�ւ�
	StageChange();

	//�X�R�A(�A�C�e���擾��)���Z�b�g
	CGame::GetScore()->SetScore(m_nGetItem);

	if (!m_bFadeOut && m_bCollDoor && m_bCanMove)
	{//�Ó]���Ă��Ȃ� & �h�A�ɓ������� & �ړ��ł���
		//�Ó]
		CApplication::GetFade()->Set(CFade::STATE::FADE_OUT);

		//�Ó]����
		m_bFadeOut = true;
	}

	if (m_bFadeOut && (CApplication::GetFade()->GetState() == CFade::STATE::NONE))
	{//�Ó]���� & ���݃t�F�[�h���Ă��Ȃ�
		D3DXVECTOR3 posDoor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�h�A�̈ʒu

		switch (dirDoor)
		{
		case CStage::DIRECTION::DIR_LEFT:	//��
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_RIGHT];
			posDoor.x -= 50.0f;
			break;

		case CStage::DIRECTION::DIR_BACK:	//��
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_FRONT];
			posDoor.z += 50.0f;
			break;

		case CStage::DIRECTION::DIR_RIGHT:	//�E
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_LEFT];
			posDoor.x += 50.0f;
			break;

		case CStage::DIRECTION::DIR_FRONT:	//��O
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_BACK];
			posDoor.z -= 50.0f;
			break;
		}

		//�ʒu��ݒ�
		m_pos = posDoor;

		//�ړ�����
		m_bCanMove = false;

		//���]����
		m_bFadeOut = false;

		//���]
		CApplication::GetFade()->Set(CFade::STATE::FADE_IN);
	}

#ifdef _DEBUG
	/* �f�o�b�O�\�� */

	//�v���C���[
	CDebugProc::Print("\n�s Player �t\n");
	CDebugProc::Print("m_pos:[%f,%f,%f]\n", m_pos.x, m_pos.y, m_pos.z);	//�ʒu
	CDebugProc::Print("m_rot:[%f,%f,%f]\n", m_rot.x, m_rot.y, m_rot.z);	//����
	CDebugProc::Print("m_vec:[%f,%f,%f]\n", m_vec.x, m_vec.y, m_vec.z);	//�����x�N�g��

	D3DXVECTOR3 size = m_vtxMax - m_vtxMin;	//���_�̍ő�E�ŏ�����T�C�Y���Z�o

	CDebugProc::Print("size:[%f,%f,%f]\n", size.x, size.y, size.z);	//�T�C�Y

	if (m_bCollGimmick)
	{//�Ώۂ̃I�u�W�F�N�g�ɓ������Ă���ꍇ
		CDebugProc::Print("Collision:[true]\n", m_bCollGimmick);
	}
	else
	{//�������Ă��Ȃ��ꍇ
		CDebugProc::Print("Collision:[false]\n", m_bCollGimmick);
	}

	//���f���p�[�c
	CDebugProc::Print("\n�s Parts �t\n");

	CModel::MODEL_INFO aInfo[MAX_PARTS] = {};	//���f�����

	for (int i = 0; i < MAX_PARTS; i++)
	{
		//���f�����̎擾
		aInfo[i] = m_pModel->GetModelInfo(i);

		//�ʒu�E����
		CDebugProc::Print("pos:[%f,%f,%f]\n", aInfo[i].pos.x, aInfo[i].pos.y, aInfo[i].pos.z);
		CDebugProc::Print("rot:[%f,%f,%f]\n", aInfo[i].rot.x, aInfo[i].rot.y, aInfo[i].rot.z);
	}

	//���C���̐ݒ�܂Ƃ�
	SetLines();

#endif // _DEBUG
}

//================================================
//�`��
//================================================
void CPlayer::Draw()
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

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

	//���f���̕`��
	m_pModel->Draw();

	for (int i = 0; i < MAX_LINE; i++)
	{
		//���C���̕`��
		m_apLine[i]->Draw();
	}
}

//================================================
//�ʒu���擾
//================================================
D3DXVECTOR3 CPlayer::GetPos()
{
	return m_pos;
}

//================================================
//���_�̍ő�l���擾
//================================================
D3DXVECTOR3 CPlayer::GetVtxMax()
{
	return m_vtxMax;
}

//================================================
//���_�̍ŏ��l���擾
//================================================
D3DXVECTOR3 CPlayer::GetVtxMin()
{
	return m_vtxMin;
}

//================================================
//�A�C�e�����擾�������ǂ������擾
//================================================
bool CPlayer::GetIsGetItem()
{
	return m_bGetItem;
}

//================================================
//�A�C�e���̊l���󋵂��擾
//================================================
CStage::STAGE CPlayer::GetStageCoin(int nIdx)
{
	return m_aStageCoin[nIdx];
}

//================================================
//�ړ�
//================================================
void CPlayer::Move()
{
	//�L�[�{�[�h�����擾
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	if (pKeyboard->GetPress(DIK_D))
	{//D�L�[������
		if (pKeyboard->GetPress(DIK_W))
		{//�E�O
			m_vec = D3DXVECTOR3(1.0f, 0.0f, 1.0f);	//�ړ�������ݒ�
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//�E���
			m_vec = D3DXVECTOR3(1.0f, 0.0f, -1.0f);	//�ړ�������ݒ�
		}
		else
		{//�E
			m_vec = D3DXVECTOR3(1.0f, 0.0f, 0.0f);	//�ړ�������ݒ�
		}

		//�L�[������
		m_bPressKey = true;
	}
	else if (pKeyboard->GetPress(DIK_A))
	{//A�L�[������
		if (pKeyboard->GetPress(DIK_W))
		{//���O
			m_vec = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);	//�ړ�������ݒ�
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//�����
			m_vec = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);	//�ړ�������ݒ�
		}
		else
		{//��
			m_vec = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);	//�ړ�������ݒ�
		}

		//�L�[������
		m_bPressKey = true;
	}
	else if (pKeyboard->GetPress(DIK_W))
	{//W�L�[������
		m_vec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);	//�ړ�������ݒ�

		//�L�[������
		m_bPressKey = true;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{//S�L�[������
		m_vec = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//�ړ�������ݒ�

		//�L�[������
		m_bPressKey = true;
	}

	//********** �� �ړ����� �� **********//

	//�x�N�g���̑傫����1�ɂ���
	D3DXVec3Normalize(&m_vec, &m_vec);

	//2�����̒P�ʃx�N�g������p�x�����߂�
	m_rotDest.y = atan2f(-m_vec.x, -m_vec.z);

	//���݂̌����ƖړI�̌����̍������v�Z
	float fDif = m_rotDest.y - m_rot.y;

	//�p�x�̐��K��
	NormalizeAngle(&fDif);

	//���݂̌������X�V
	m_rot.y += fDif * ROT_SMOOTHNESS;

	//�p�x�̐��K��
	NormalizeAngle(&m_rot.y);

	//********** �� �ړ��� �� **********//

	if (!m_bPressKey)
	{//�ړ��L�[��������Ă��Ȃ��ꍇ
		return;
	}

	/* �ړ��L�[��������Ă���ꍇ */

	//�ړ��ʂɑ��(�ړ��x�N�g�� * �ړ����x)
	m_move = m_vec * MOVE_SPEED;

	//���݂̈ʒu���A�O��̈ʒu�Ƃ��ĕۑ�
	m_posOld = m_pos;

	//���݂̈ʒu�Ɉړ��ʂ����Z
	m_pos += m_move;

	if (m_pos.x < -LIMIT_POS)
	{//�ړ������ʒu(����)�𒴂�����
		m_pos.x = -LIMIT_POS;	//�ʒu��߂�
	}
	else if (m_pos.x > LIMIT_POS)
	{//�ړ������ʒu(�E��)�𒴂�����
		m_pos.x = LIMIT_POS;	//�ʒu��߂�
	}

	if (m_pos.z < -LIMIT_POS)
	{//�ړ������ʒu(����)�𒴂�����
		m_pos.z = -LIMIT_POS;	//�ʒu��߂�
	}
	else if (m_pos.z > LIMIT_POS)
	{//�ړ������ʒu(��O��)�𒴂�����
		m_pos.z = LIMIT_POS;	//�ʒu��߂�
	}

	//�ړ��ʂ�0�ɂ���
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�u�L�[��������Ă��Ȃ��v�ɂ���
	m_bPressKey = false;
}

//================================================
//���[�V����
//================================================
void CPlayer::Motion()
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (m_pModel == nullptr)
		{//NULL�`�F�b�N
			continue;
		}

		/* nullptr�ł͂Ȃ��ꍇ */

		//���[�V�������
		MOTION_SET motion = m_aMotion[m_motionType];

		//���Βl���v�Z(���[�V�����J�E���^�[ / �Đ��t���[����)
		float fRelativeValue = (float)m_nCntMotion / (float)motion.aKeySet[m_nCurrentKey].nFrame;

		//���̃L�[�ԍ�(�v�Z�p)
		int nNextKey = (m_nCurrentKey + 1) % motion.nNumKey;

		//����(�I���l - �J�n�l)
		D3DXVECTOR3 posDif = (motion.aKeySet[nNextKey].aKey[i].pos - motion.aKeySet[m_nCurrentKey].aKey[i].pos);
		D3DXVECTOR3 rotDif = (motion.aKeySet[nNextKey].aKey[i].rot - motion.aKeySet[m_nCurrentKey].aKey[i].rot);

		//���� * ���Βl
		D3DXVECTOR3 pos = D3DXVECTOR3(	//�ʒu
			posDif.x * fRelativeValue,
			posDif.y * fRelativeValue,
			posDif.z * fRelativeValue);

		D3DXVECTOR3 rot = D3DXVECTOR3(	//����
			rotDif.x * fRelativeValue,
			rotDif.y * fRelativeValue,
			rotDif.z * fRelativeValue);

		//�ʒu�E�����̌��ݒl���擾
		D3DXVECTOR3 posPre = m_pModel->GetModelInfo(i).pos;
		D3DXVECTOR3 rotPre = m_pModel->GetModelInfo(i).rot;

		//���ݒl�ɑ��(�J�n�l + (���� * ���Βl))
		posPre = motion.aKeySet[m_nCurrentKey].aKey[i].pos + pos;
		rotPre = motion.aKeySet[m_nCurrentKey].aKey[i].rot + rot;

		//�p�x�̐��K��
		NormalizeAngle(&rotPre.x);	
		NormalizeAngle(&rotPre.y);
		NormalizeAngle(&rotPre.z);

		//�ʒu�E�����𔽉f
		m_pModel->SetModelInfo_Pos(i, posPre);
		m_pModel->SetModelInfo_Rot(i, rotPre);
	}

	m_nCntMotion++;	//���[�V�����J�E���^�[��i�߂�

	if (m_nCntMotion < m_aMotion[m_motionType].aKeySet[m_nCurrentKey].nFrame)
	{//���[�V�����J�E���^�[���Đ��t���[�����ɒB���Ă��Ȃ��ꍇ
		return;
	}

	/* �Đ��t���[�����ɒB�����ꍇ */

	m_nCurrentKey++;	//���݂̃L�[�ԍ�����i�߂�
	m_nCntMotion = 0;	//���[�V�����J�E���^�[��������

	if (m_nCurrentKey == m_aMotion[m_motionType].nNumKey)
	{//���݂̃L�[�ԍ����A�L�[�̑����ɒB������
		m_nCurrentKey = 0;	//���݂̃L�[�ԍ���0�ɖ߂�
	}
}

//================================================
//�����蔻��
//================================================
void CPlayer::Collision()
{
	if (CApplication::GetFade()->GetState() != CFade::STATE::NONE)
	{//�t�F�[�h���̏ꍇ
		return;
	}

	/* �t�F�[�h���Ă��Ȃ��ꍇ */

	/* X���f�� */
	for (int i = 0; i < CStage::MAX_MODEL; i++)
	{
		//�Ώۂ�X���f�������擾
		CObjectX* pObjX = CStage::GetObjectX(i);

		if (pObjX == nullptr)
		{//NULL�`�F�b�N
			continue;
		}

		/* nullptr�ł͂Ȃ��ꍇ */

		//���g�̃T�C�Y���Z�o
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//�Ώۂ̃T�C�Y���Z�o
		D3DXVECTOR3 sizeTarget = (pObjX->GetVtxMax() - pObjX->GetVtxMin());

		//�����蔻��
		m_bCollObjectX = CollisionModel(
			&m_pos,				//���g�̌��݂̈ʒu
			m_posOld,			//���g�̑O��̈ʒu
			pObjX->GetPos(),	//�Ώۂ̈ʒu
			sizeOwn,			//���g�̃T�C�Y
			sizeTarget			//�Ώۂ̃T�C�Y
		);
	}

	/* �M�~�b�N */
	for (int i = 0; i < CStage::MAX_GIMMICK; i++)
	{
		//�Ώۂ̃M�~�b�N�����擾
		CGimmick* pGimmick = CStage::GetGimmick(i);

		if (pGimmick == nullptr)
		{//NULL�`�F�b�N
			continue;
		}

		/* nullptr�ł͂Ȃ��ꍇ */

		//���g�̃T�C�Y���Z�o
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//�Ώۂ̃T�C�Y���Z�o
		D3DXVECTOR3 sizeTarget = (pGimmick->GetVtxMax() - pGimmick->GetVtxMin());

		//�����蔻��
		m_bCollGimmick = CollisionModel(
			&m_pos,				//���g�̌��݂̈ʒu
			m_posOld,			//���g�̑O��̈ʒu
			pGimmick->GetPos(),	//�Ώۂ̈ʒu
			sizeOwn,			//���g�̃T�C�Y
			sizeTarget			//�Ώۂ̃T�C�Y
		);
	}

	/* �p�l���X�^���h */

	//�p�l�����̎擾
	CPanel* pPanel = CGame::GetPanel();

	if (pPanel != nullptr)
	{//nullptr�ł͂Ȃ��ꍇ
		//���g�̃T�C�Y���Z�o
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//�Ώۂ̃T�C�Y���Z�o
		D3DXVECTOR3 sizeTarget = (pPanel->GetPanelStand()->GetVtxMax() - pPanel->GetPanelStand()->GetVtxMin());

		//�����蔻��
		m_bCollPanelStand = CollisionModel(
			&m_pos,								//���g�̌��݂̈ʒu
			m_posOld,							//���g�̑O��̈ʒu
			pPanel->GetPanelStand()->GetPos(),	//�Ώۂ̈ʒu
			sizeOwn,							//���g�̃T�C�Y
			sizeTarget							//�Ώۂ̃T�C�Y
		);

		//�����������ǂ����𑗂�
		pPanel->SetCollPlayer(m_bCollPanelStand);
	}

	/* �h�A */
	for (int i = 0; i < CStage::MAX_DOOR; i++)
	{
		//�h�A�����擾
		CDoor* pDoor = CStage::GetDoor(i);

		if (pDoor == nullptr)
		{//NULL�`�F�b�N
			continue;
		}

		/* nullptr�ł͂Ȃ��ꍇ */

		//���g�̃T�C�Y���Z�o
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//�Ώۂ̃T�C�Y���Z�o
		D3DXVECTOR3 sizeTarget = (pDoor->GetVtxMax() - pDoor->GetVtxMin());

		//�����蔻��
		m_bCollDoor = CollisionModel(
			&m_pos,				//���g�̌��݂̈ʒu
			m_posOld,			//���g�̑O��̈ʒu
			pDoor->GetPos(),	//�Ώۂ̈ʒu
			sizeOwn,			//���g�̃T�C�Y
			sizeTarget			//�Ώۂ̃T�C�Y
		);

		if (m_bCollDoor)
		{//�������Ă�����
			//���������h�A�̕������擾
			dirDoor = pDoor->GetDir();
			return;
		}
	}

	//�A�C�e�������擾
	m_pItem = CStage::GetItem();

	if (m_pItem == nullptr)
	{//NULL�`�F�b�N
		return;
	}

	//���g�̃T�C�Y���Z�o
	D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

	//�Ώۂ̃T�C�Y���Z�o
	D3DXVECTOR3 sizeTarget = (m_pItem->GetVtxMax() - m_pItem->GetVtxMin());

	//�����蔻��
	m_bGetItem = CollisionModel(
		&m_pos,				//���g�̌��݂̈ʒu
		m_posOld,			//���g�̑O��̈ʒu
		m_pItem->GetPos(),	//�Ώۂ̈ʒu
		sizeOwn,			//���g�̃T�C�Y
		sizeTarget			//�Ώۂ̃T�C�Y
	);

	if (m_bGetItem)
	{//�A�C�e�����l�������ꍇ
		//�l�������X�e�[�W���L�^
		m_aStageCoin[m_nGetItem] = CGame::GetStage()->Get();

		//�A�C�e���l�����𑝂₷
		m_nGetItem++;
	}
}

//================================================
//�X�e�[�W�؂�ւ�
//================================================
void CPlayer::StageChange()
{
	if (!m_bCollDoor)
	{//�h�A�ɐG��Ă��Ȃ�
		return;
	}

	/* �h�A�ɓ��������ꍇ */

	//�p�l�����擾�p
	CPanel::PANEL_INFO aInfo[CPanel::GRID_Y][CPanel::GRID_X] = {};

	//���݂̃X�e�[�W���擾
	CStage* pStage = CGame::GetStage();

	int nPosX = 0, nPosY = 0;	//�p�l���̈ʒu�ԍ�

	for (int Y = 0; Y < CPanel::GRID_Y; Y++)
	{
		for (int X = 0; X < CPanel::GRID_X; X++)
		{
			//�p�l�������擾
			aInfo[Y][X] = CGame::GetPanel()->GetPanelInfo(Y, X);

			if (aInfo[Y][X].stage == pStage->Get())
			{//���݂̃X�e�[�W�Ɠ����X�e�[�W�������p�l���̏ꍇ
				//�ʒu�ԍ���ۑ�
				nPosX = X;	//X��
				nPosY = Y;	//Y��
			}
		}
	}

	//�ړ���̔ԍ��p
	int nDestX = nPosX;	//X��
	int nDestY = nPosY;	//Y��

	switch (dirDoor)
	{//���������h�A�̕���
	case CStage::DIRECTION::DIR_LEFT:	//����
		nDestX--;	//������X�e�[�W�̍����̃X�e�[�W�𒲂ׂ�

		if (nDestX < 0)
		{//0����(-1�ȉ�)�ɂȂ����ꍇ
			nDestX = 0;	//0�ɌŒ�
		}
		break;

	case CStage::DIRECTION::DIR_BACK:	//����
		nDestY--;	//������X�e�[�W�̉����̃X�e�[�W�𒲂ׂ�

		if (nDestY < 0)
		{//0����(-1�ȉ�)�ɂȂ����ꍇ
			nDestY = 0;	//0�ɌŒ�
		}
		break;

	case CStage::DIRECTION::DIR_RIGHT:	//�E��
		nDestX++;	//������X�e�[�W�̉E���̃X�e�[�W�𒲂ׂ�

		if (nDestX >= CPanel::GRID_X)
		{//2���傫��(3�ȏ�)�ɂȂ����ꍇ
			nDestX = (CPanel::GRID_X - 1);	//2�ɌŒ�
		}
		break;

	case CStage::DIRECTION::DIR_FRONT:	//��O��
		nDestY++;	//������X�e�[�W�̎�O���̃X�e�[�W�𒲂ׂ�

		if (nDestY >= CPanel::GRID_Y)
		{//2���傫��(3�ȏ�)�ɂȂ����ꍇ
			nDestY = (CPanel::GRID_Y - 1);	//2�ɌŒ�
		}
		break;

	case CStage::DIRECTION::DIR_NONE:
	case CStage::DIRECTION::DIR_MAX:
	default:	//���̑�
		assert(false);
		break;
	}

	if (aInfo[nDestY][nDestX].stage == CStage::STAGE::NONE)
	{//�h�A�̐�ɃX�e�[�W�������ꍇ
		return;
	}

	/* �h�A�̐�ɃX�e�[�W������ꍇ */

	//�h�A��̃X�e�[�W�́A�h�A�̕����݂̂�ǂݍ���
	pStage->LoadDoorDir(aInfo[nDestY][nDestX].stage);

	CStage::DIRECTION aDir[CStage::MAX_DOOR] = {};	//�h�A�����擾�p

	for (int i = 0; i < CStage::MAX_DOOR; i++)
	{
		//�ǂݍ��񂾃h�A�̕������擾
		aDir[i] = (CStage::DIRECTION)pStage->GetDoorDir(i);
	}

	for (int i = 0; i < CStage::MAX_DOOR; i++)
	{
		if (aDir[i] == CStage::DIRECTION::DIR_NONE)
		{//�h�A�������ꍇ
			continue;
		}

		/* �h�A������ꍇ */

		switch (dirDoor)
		{//�G�ꂽ�h�A�̕���
		case CStage::DIRECTION::DIR_LEFT:	//����
			if (aDir[i] == CStage::DIRECTION::DIR_RIGHT)
			{//�h�A��̃X�e�[�W�ɁA�Ή�����u�E���v�̃h�A������ꍇ
				m_bCanMove = true;	//�ړ��ł���
			}
			break;

		case CStage::DIRECTION::DIR_BACK:	//����
			if (aDir[i] == CStage::DIRECTION::DIR_FRONT)
			{//�h�A��̃X�e�[�W�ɁA�Ή�����u��O���v�̃h�A������ꍇ
				m_bCanMove = true;	//�ړ��ł���
			}
			break;

		case CStage::DIRECTION::DIR_RIGHT:	//�E��
			if (aDir[i] == CStage::DIRECTION::DIR_LEFT)
			{//�h�A��̃X�e�[�W�ɁA�Ή�����u�����v�̃h�A������ꍇ
				m_bCanMove = true;	//�ړ��ł���
			}
			break;

		case CStage::DIRECTION::DIR_FRONT:	//��O��
			if (aDir[i] == CStage::DIRECTION::DIR_BACK)
			{//�h�A��̃X�e�[�W�ɁA�Ή�����u�����v�̃h�A������ꍇ
				m_bCanMove = true;	//�ړ��ł���
			}
			break;

		default:
			break;
		}

		if (m_bCanMove)
		{//�h�A��ʉ߂��ăX�e�[�W�ړ��o����ꍇ
			//�X�e�[�W��ύX
			pStage->Change(aInfo[nDestY][nDestX].stage);
			return;
		}
	}
}

//================================================
//���_�̍ő�l�ƍŏ��l��ݒ�
//================================================
void CPlayer::SetVtxMaxAndMin()
{
	//���_�̍ő吔�E�ŏ��l��������
	m_vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//�e�p�[�c�̒��_�̍ő�l�E�ŏ��l���i�[����z��
	D3DXVECTOR3 aVtxMax[MAX_PARTS] = {};
	D3DXVECTOR3 aVtxMin[MAX_PARTS] = {};

	for (int i = 0; i < MAX_PARTS; i++)
	{
		//���f���̊e���̎擾
		CModel::MODEL_INFO modelInfo = CModel::GetModelInfo(i);

		//�e�p�[�c���璸�_�̍ő�l�E�ŏ��l���擾
		aVtxMax[i] = modelInfo.vtxMax;
		aVtxMin[i] = modelInfo.vtxMin;
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		//********** �ő�l **********//

		/* �e�p�[�c�̍ő�l���A���݂̍ő�l���傫���ꍇ�A�ő�l��ݒ肷�� */

		if (aVtxMax[i].x > m_vtxMax.x)
		{//X���W
			m_vtxMax.x = aVtxMax[i].x;	//�ő�l��ݒ�
		}

		if (aVtxMax[i].y > m_vtxMax.y)
		{//Y���W
			m_vtxMax.y = aVtxMax[i].y;	//�ő�l��ݒ�
		}

		if (aVtxMax[i].z> m_vtxMax.z)
		{//Z���W
			m_vtxMax.z = aVtxMax[i].z;	//�ő�l��ݒ�
		}
		
		//********** �ŏ��l **********//

		/* �e�p�[�c�̍ŏ��l���A���݂̍ŏ��l��菬�����ꍇ�A�ŏ��l��ݒ肷�� */

		if (aVtxMin[i].x < m_vtxMin.x)
		{//X���W
			m_vtxMin.x = aVtxMin[i].x;	//�ŏ��l��ݒ�
		}

		if (aVtxMin[i].y < m_vtxMin.y)
		{//Y���W
			m_vtxMin.y = aVtxMin[i].y;	//�ŏ��l��ݒ�
		}

		if (aVtxMin[i].z < m_vtxMin.z)
		{//Z���W
			m_vtxMin.z = aVtxMin[i].z;	//�ŏ��l��ݒ�
		}
	}
}

//================================================
//���C���̐ݒ�܂Ƃ�
//================================================
void CPlayer::SetLines()
{
	//����(�Œ�)
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�F(�S�Ă̐��œ����F)
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//���Ԗڂ�
	int nNum = 0;

	//********** �㕔���� **********//

	//�n�_�E�I�_
	D3DXVECTOR3 start = m_aPosVtx[3];
	D3DXVECTOR3 end = m_aPosVtx[2];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �㕔��O�� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[2];
	end = m_aPosVtx[6];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �㕔�E�� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[6];
	end = m_aPosVtx[7];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �㕔���� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[7];
	end = m_aPosVtx[3];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �������� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[1];
	end = m_aPosVtx[0];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ������O�� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[0];
	end = m_aPosVtx[4];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �����E�� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[4];
	end = m_aPosVtx[5];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** �������� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[5];
	end = m_aPosVtx[1];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ��O���� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[2];
	end = m_aPosVtx[0];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ��O�E�� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[6];
	end = m_aPosVtx[4];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ������ **********//

	//�n�_�E�I�_
	start = m_aPosVtx[3];
	end = m_aPosVtx[1];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�

	//********** ���E�� **********//

	//�n�_�E�I�_
	start = m_aPosVtx[7];
	end = m_aPosVtx[5];

	//�ݒ�
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//���ɐi�߂�
}

//================================================
//�ǂݍ���
//================================================
void CPlayer::Load()
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

		if (strcmp(&aText[0], "MOTIONSET") == 0)
		{//���[�V�����Z�b�g
			//���[�V�����ݒ�
			Set_Motion(pFile, &aText[0]);

			//���[�V�������𑝂₷
			m_nNumMotion++;
		}
	}

	//�t�@�C�������
	fclose(pFile);
}

//================================================
//���[�V�����ݒ�
//================================================
void CPlayer::Set_Motion(FILE* pFile, char aText[])
{
	while (strcmp(&aText[0], "END_MOTIONSET") != 0)
	{//���[�V�����Z�b�g���I���܂ŌJ��Ԃ�
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

		if (strcmp(&aText[0], "LOOP") == 0)
		{//���[�v���邩�ǂ���
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			int nLoop = 0;	//�ǂݍ��ݗp

			//���[�v�̗L����ǂݍ���
			fscanf(pFile, "%d", &nLoop);

			if (nLoop == 0)
			{
				//���[�v����
				m_aMotion[m_nNumMotion].bLoop = false;
			}
			else
			{
				//���[�v�L��
				m_aMotion[m_nNumMotion].bLoop = true;
			}
		}
		else if (strcmp(&aText[0], "NUM_KEY") == 0)
		{//�L�[��
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�L�[����ǂݍ���
			fscanf(pFile, "%d", &m_aMotion[m_nNumMotion].nNumKey);
		}
		else if (strcmp(&aText[0], "KEYSET") == 0)
		{//�L�[�Z�b�g
			//�L�[�Z�b�g�ݒ�
			Set_KeySet(pFile, &aText[0]);

			//�L�[�Z�b�g���𑝂₷
			m_nNumKeySet++;
		}
	}

	//�L�[�Z�b�g�������Z�b�g
	m_nNumKeySet = 0;
}

//================================================
//�L�[�Z�b�g�ݒ�
//================================================
void CPlayer::Set_KeySet(FILE* pFile, char aText[])
{
	while (strcmp(&aText[0], "END_KEYSET") != 0)
	{//�L�[�Z�b�g���I���܂ŌJ��Ԃ�
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

		if (strcmp(&aText[0], "FRAME") == 0)
		{//�t���[����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�t���[������ǂݍ���
			fscanf(pFile, "%d", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].nFrame);
		}
		else if (strcmp(&aText[0], "KEY") == 0)
		{//�L�[
			//�L�[�ݒ�
			Set_Key(pFile, &aText[0]);

			//�L�[���𑝂₷
			m_nNumKey++;
		}
	}

	//�L�[�������Z�b�g
	m_nNumKey = 0;
}

//================================================
//�L�[�ݒ�
//================================================
void CPlayer::Set_Key(FILE* pFile, char aText[])
{
	while (strcmp(&aText[0], "END_KEY") != 0)
	{//�L�[���I���܂ŌJ��Ԃ�
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

		if (strcmp(&aText[0], "POS") == 0)
		{//�ʒu
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�ʒu��ǂݍ���
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].pos.x);	//X��
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].pos.y);	//Y��
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].pos.z);	//Z��
		}
		else if (strcmp(&aText[0], "ROT") == 0)
		{//����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//������ǂݍ���
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].rot.x);	//X��
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].rot.y);	//Y��
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].rot.z);	//Z��
		}
	}
}