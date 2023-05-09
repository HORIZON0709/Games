//================================================
//
//3D�Q�[��(��)[stage.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "stage.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "object3D.h"
#include "gimmick.h"
#include "item.h"
#include "door.h"
#include "player.h"

#include "debug_proc.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const char* CStage::s_apFileName[] =
{//�X�e�[�W�̃p�X
	"data/TEXT/Stage_01.txt",	//�X�e�[�W01
	"data/TEXT/Stage_02.txt",	//�X�e�[�W02
	"data/TEXT/Stage_03.txt",	//�X�e�[�W03
	"data/TEXT/Stage_04.txt",	//�X�e�[�W04
	"data/TEXT/Stage_05.txt",	//�X�e�[�W05
	"data/TEXT/Stage_06.txt",	//�X�e�[�W06
	"data/TEXT/Stage_07.txt",	//�X�e�[�W07
	"data/TEXT/Stage_08.txt"	//�X�e�[�W08
};

static_assert(sizeof(CStage::s_apFileName) / sizeof(CStage::s_apFileName[0]) == CStage::STAGE::MAX, "aho");

const float CStage::ROT_DOOR = 1.57f;	//�h�A�̌���

const D3DXVECTOR3 CStage::POS_DOOR[CStage::DIRECTION::DIR_MAX] =
{//�h�A�̈ʒu
	D3DXVECTOR3(-300.0f,0.0f,0.0f),	//��
	D3DXVECTOR3(0.0f,0.0f,300.0f),	//��
	D3DXVECTOR3(300.0f,0.0f,0.0f),	//�E
	D3DXVECTOR3(0.0f,0.0f,-300.0f)	//��O
};

const float CStage::FLOAR_SIZE = 700.0f;				//���̃T�C�Y
const float CStage::WALL_WIDTH = FLOAR_SIZE;			//�ǂ̕�
const float CStage::WALL_HEIGHT = FLOAR_SIZE * 0.5f;	//�ǂ̍���

const int CStage::MAX_WORD = 256;	//�ő啶����

//***************************
//�ÓI�����o�ϐ�
//***************************
CObjectX* CStage::m_apModel[MAX_MODEL] = {};		//���f���̃|�C���^
CGimmick* CStage::m_apGimmick[MAX_GIMMICK] = {};	//�M�~�b�N�̃|�C���^
CItem* CStage::m_pItem = nullptr;					//�A�C�e���̃|�C���^
CDoor* CStage::m_apDoor[MAX_DOOR] = {};				//�h�A�̃|�C���^

CObject3D* CStage::m_pFloar = nullptr;					//3D�|���S���̏��̃|�C���^
CObject3D* CStage::m_apWall[DIRECTION::DIR_MAX] = {};	//3D�|���S���̕�(�l��)�̃|�C���^

//================================================
//����
//================================================
CStage* CStage::Create(const STAGE &stage)
{
	CStage* pStage = nullptr;	//�|�C���^

	if (pStage != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pStage = new CStage;	//�������̓��I�m��

	pStage->Init(stage, s_apFileName[stage]);	//������

	return pStage;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//X���f�����̎擾
//================================================
CObjectX* CStage::GetObjectX(int nIdx)
{
	return m_apModel[nIdx];
}

//================================================
//�M�~�b�N���̎擾
//================================================
CGimmick* CStage::GetGimmick(int nIdx)
{
	return m_apGimmick[nIdx];
}

//================================================
//�A�C�e�����̎擾
//================================================
CItem* CStage::GetItem()
{
	return m_pItem;
}

//================================================
//�h�A���̎擾
//================================================
CDoor* CStage::GetDoor(int nIdx)
{
	return m_apDoor[nIdx];
}

//================================================
//�R���X�g���N�^
//================================================
CStage::CStage() :
	m_stage(STAGE::NONE),
	m_stageNext(STAGE::NONE),
	m_nNumModel(0),
	m_nCntModelSet(0)
{
	//�����o�ϐ��̃N���A
	for (int i = 0; i < MAX_DOOR; i++)
	{
		m_aDir[i] = 0;
	}
}

//================================================
//�f�X�g���N�^
//================================================
CStage::~CStage()
{
}

//================================================
//������
//================================================
HRESULT CStage::Init(const STAGE &stage, const char* pStage)
{
	//�����o�ϐ��̏�����
	m_stage = stage;
	m_stageNext = STAGE::NONE;
	m_nNumModel = 0;
	m_nCntModelSet = 0;

	for (int i = 0; i < MAX_DOOR; i++)
	{
		m_aDir[i] = -1;
	}

	//���ƕǂ̐���
	CreateFloarAndWalls();

	//�ǂݍ���
	Load(pStage);

	return S_OK;
}

//================================================
//�I��
//================================================
void CStage::Uninit()
{
	for (int i = 0; i < MAX_DOOR; i++)
	{
		if (m_apDoor[i] != nullptr)
		{//NULL�`�F�b�N
			m_apDoor[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apDoor[i] = nullptr;			//nullptr�ɂ���
		}
	}

	if (m_pItem != nullptr)
	{//NULL�`�F�b�N
		m_pItem->SetDeathFlag();	//���S�t���O�̐ݒ�
		m_pItem = nullptr;			//nullptr�ɂ���
	}

	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (m_apGimmick[i] != nullptr)
		{//NULL�`�F�b�N
			m_apGimmick[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apGimmick[i] = nullptr;		//nullptr�ɂ���
		}
	}

	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (m_apModel[i] != nullptr)
		{//NULL�`�F�b�N
			m_apModel[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apModel[i] = nullptr;			//nullptr�ɂ���
		}
	}
}

//================================================
//�S�Ă̏I��
//================================================
void CStage::UninitAll()
{
	for (int i = 0; i < MAX_DOOR; i++)
	{
		if (m_apDoor[i] != nullptr)
		{//NULL�`�F�b�N
			m_apDoor[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apDoor[i] = nullptr;			//nullptr�ɂ���
		}
	}

	if (m_pItem != nullptr)
	{//NULL�`�F�b�N
		m_pItem->SetDeathFlag();	//���S�t���O�̐ݒ�
		m_pItem = nullptr;			//nullptr�ɂ���
	}

	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (m_apGimmick[i] != nullptr)
		{//NULL�`�F�b�N
			m_apGimmick[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apGimmick[i] = nullptr;		//nullptr�ɂ���
		}
	}

	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (m_apModel[i] != nullptr)
		{//NULL�`�F�b�N
			m_apModel[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apModel[i] = nullptr;			//nullptr�ɂ���
		}
	}

	for (int i = 0; i < DIRECTION::DIR_MAX; i++)
	{
		if (m_apWall[i] != nullptr)
		{//NULL�`�F�b�N
			m_apWall[i]->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_apWall[i] = nullptr;			//nullptr�ɂ���
		}
	}

	if (m_pFloar != nullptr)
	{//NULL�`�F�b�N
		m_pFloar->SetDeathFlag();	//���S�t���O�̐ݒ�
		m_pFloar = nullptr;			//nullptr�ɂ���
	}
}

//================================================
//�X�V
//================================================
void CStage::Update()
{
	if (CGame::GetPlayer()->GetIsGetItem())
	{//�v���C���[���A�C�e�����擾������
		if (m_pItem != nullptr)
		{//NULL�`�F�b�N
			m_pItem->SetDeathFlag();	//���S�t���O�̐ݒ�
			m_pItem = nullptr;			//nullptr�ɂ���
		}
	}

#ifdef _DEBUG
	//�f�o�b�O�\��
	CDebugProc::Print("\n�s Stage �t\n");
	CDebugProc::Print("���݂̃X�e�[�W:[%d]\n", m_stage);	//���݂̃X�e�[�W
#endif // _DEBUG
}

//================================================
//�X�e�[�W�̐ݒ�
//================================================
CStage* CStage::Set()
{
	if (m_stageNext == STAGE::NONE)
	{//���̃X�e�[�W�����܂��Ă��Ȃ�
		return this;
	}

	//���݂̃X�e�[�W���I��
	Uninit();

	m_stage = m_stageNext;		//�X�e�[�W��ύX
	m_stageNext = STAGE::NONE;	//�����Ȃ���Ԃɂ���

	return Create(m_stage);	//�ύX��̃X�e�[�W�𐶐����ĕԂ�
}

//================================================
//�X�e�[�W�̎擾
//================================================
CStage::STAGE CStage::Get()
{
	return m_stage;
}

//================================================
//�X�e�[�W�̕ύX
//================================================
void CStage::Change(const STAGE &stage)
{
	assert(stage > STAGE::NONE && stage < STAGE::MAX);
	
	m_stageNext = stage;
}

//================================================
//�h�A�̕����̓ǂݍ���
//================================================
void CStage::LoadDoorDir(const STAGE &stage)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(s_apFileName[stage], "r");

	if (pFile == nullptr)
	{//�t�@�C�����J���Ȃ������ꍇ
		assert(false);
	}

	/* �t�@�C�����J�����ꍇ */

	for (int i = 0; i < MAX_DOOR; i++)
	{
		m_aDir[i] = -1;
	}

	char aText[MAX_WORD] = {};	//�e�L�X�g�i�[�p

	int nNumDoor = 0;	//�h�A�̐�

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

		if (strcmp(&aText[0], "MODELSET") == 0)
		{//���f���Z�b�g
			while (strcmp(&aText[0], "END_MODELSET") != 0)
			{//���f���Z�b�g���I���܂œǂݍ��ݑ�����
				//������ǂݍ���
				fscanf(pFile, "%s", &aText[0]);

				if (strcmp(&aText[0], "DIR") == 0)
				{//����
					//�u���v��ǂݍ���
					fscanf(pFile, "%s", &aText[0]);

					//������ǂݍ���
					fscanf(pFile, "%d", &m_aDir[nNumDoor]);

					//�h�A�̐����J�E���g����
					nNumDoor++;
				}
			}
		}
	}
}

//================================================
//�h�A�̕������擾
//================================================
int CStage::GetDoorDir(int nIdx)
{
	return m_aDir[nIdx];
}

//================================================
//�ǂݍ���
//================================================
void CStage::Load(const char* pStage)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pStage, "r");
	
	if (pFile == nullptr)
	{//�t�@�C�����J���Ȃ������ꍇ
		assert(false);
	}

	/* �t�@�C�����J�����ꍇ */

	char aText[MAX_WORD] = {};	//�e�L�X�g�i�[�p

	while (strncmp(&aText[0], "SCRIPT", 6) != 0)
	{//�e�L�X�g�̍ŏ��̍s��ǂݍ��ނ܂ŌJ��Ԃ�
		fgets(aText, MAX_WORD, pFile);	//1�s�ۂ��Ɠǂݍ���
	}

	char aFileName[MAX_MODEL][MAX_WORD] = {};	//�t�@�C���p�X�ǂݍ��ݗp

	int nCntFile = 0;	//�t�@�C�����J�E���g

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
			fscanf(pFile, "%s", &aText[0]);

			//X�t�@�C���̃p�X��ǂݍ���
			fscanf(pFile, "%s", &aFileName[nCntFile][0]);

			//�J�E���g�𑝉�
			nCntFile++;
		}
		else if (strcmp(&aText[0], "MODELSET") == 0)
		{//���f���Z�b�g
			//���f���ݒ�
			Set_ModelSet(pFile);

			//�J�E���g�A�b�v
			m_nCntModelSet++;
		}
	}

	//�t�@�C�������
	fclose(pFile);

	for (int i = 0, nNumModel = 0, nNumGimmick = 0, nNumDoor = 0; i < m_nCntModelSet; i++)
	{
		//�C���f�b�N�X��
		int nIndex = m_aModelSetInfo[i].nIndex;

		switch (m_aModelSetInfo[i].type)
		{
		case MODEL_TYPE::TYPE_OBJECT:	//�I�u�W�F�N�g
			//����
			m_apModel[nNumModel] = CObjectX::Create(&aFileName[nIndex][0]);

			//�ʒu�E�����̐ݒ�
			m_apModel[nNumModel]->SetPos(m_aModelSetInfo[i].pos);
			m_apModel[nNumModel]->SetRot(m_aModelSetInfo[i].rot);

			//�J�E���g�A�b�v
			nNumModel++;
			break;

		case MODEL_TYPE::TYPE_GIMMICK:	//�M�~�b�N
			//����
			m_apGimmick[nNumGimmick] = CGimmick::Create(&aFileName[nIndex][0]);

			//�ʒu�E�����̐ݒ�
			m_apGimmick[nNumGimmick]->SetPos(m_aModelSetInfo[i].pos);
			m_apGimmick[nNumGimmick]->SetRot(m_aModelSetInfo[i].rot);

			//�J�E���g�A�b�v
			nNumGimmick++;
			break;

		case MODEL_TYPE::TYPE_ITEM:	//�A�C�e��
			for (int i = 0; i < MAX_COIN; i++)
			{
				//�R�C���̊l���󋵂��擾
				STAGE stageCoin = CGame::GetPlayer()->GetStageCoin(i);

				if (stageCoin == m_stage)
				{//���݂̃X�e�[�W�̃R�C�����l���ς̏ꍇ
					break;
				}
			}

			/* �l�����Ă��Ȃ��ꍇ */

			//����
			m_pItem = CItem::Create(&aFileName[nIndex][0]);
			
			//�ʒu�E�����̐ݒ�
			m_pItem->SetPos(m_aModelSetInfo[i].pos);
			m_pItem->SetRot(m_aModelSetInfo[i].rot);

			//�z�u���ꂽ�X�e�[�W��ݒ�
			m_pItem->SetIsStage(m_stage);
			break;

		case MODEL_TYPE::TYPE_DOOR:	//�h�A
			//����
			m_apDoor[nNumDoor] = CDoor::Create(&aFileName[nIndex][0]);

			//�����̐ݒ�
			m_apDoor[nNumDoor]->SetDir(m_aModelSetInfo[i].dir);

			//�ʒu�̐ݒ�
			m_apDoor[nNumDoor]->SetPos(POS_DOOR[m_aModelSetInfo[i].dir]);

			if (m_aModelSetInfo[i].dir == DIRECTION::DIR_LEFT ||
				m_aModelSetInfo[i].dir == DIRECTION::DIR_RIGHT)
			{//�h�A�̈ʒu�� [���� or �E��] �ɂ���ꍇ
				//�����̐ݒ�
				m_apDoor[nNumDoor]->SetRot(D3DXVECTOR3(0.0f, ROT_DOOR, 0.0f));
			}

			//�J�E���g�A�b�v
			nNumDoor++;
			break;

		case MODEL_TYPE::TYPE_NONE:	//���̑�
		case MODEL_TYPE::TYPE_MAX:
		default:
			assert(false);
			break;
		}
	}
}

//================================================
//���f���Z�b�g�ݒ�
//================================================
void CStage::Set_ModelSet(FILE* pFile)
{
	char aText[MAX_WORD] = {};	//�e�L�X�g�i�[�p

	//�|�C���^�ɑ��
	MODELSET_INFO* pInfo = &m_aModelSetInfo[m_nCntModelSet];

	while (strcmp(&aText[0], "END_MODELSET") != 0)
	{//���f���Z�b�g���I���܂ŌJ��Ԃ�
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
			fscanf(pFile, "%d", &pInfo->nIndex);
		}
		else if (strcmp(&aText[0], "MODEL_TYPE") == 0)
		{//���f���^�C�v
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			int nType = 0;	//�^�C�v�ǂݍ��ݗp

			//���f���^�C�v��ǂݍ���
			fscanf(pFile, "%d", &nType);

			switch (nType)
			{
			case 0:	//�I�u�W�F�N�g
				pInfo->type = MODEL_TYPE::TYPE_OBJECT;
				break;

			case 1:	//�M�~�b�N
				pInfo->type = MODEL_TYPE::TYPE_GIMMICK;
				break;

			case 2:	//�A�C�e��
				pInfo->type = MODEL_TYPE::TYPE_ITEM;
				break;

			case 3:	//�h�A
				pInfo->type = MODEL_TYPE::TYPE_DOOR;
				break;

			default:	//���̑�
				assert(false);
				break;
			}
		}
		else if (strcmp(&aText[0], "POS") == 0)
		{//�ʒu
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�ʒu��ǂݍ���
			fscanf(pFile, "%f", &pInfo->pos.x);
			fscanf(pFile, "%f", &pInfo->pos.y);
			fscanf(pFile, "%f", &pInfo->pos.z);
		}
		else if (strcmp(&aText[0], "ROT") == 0)
		{//����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//������ǂݍ���
			fscanf(pFile, "%f", &pInfo->rot.x);
			fscanf(pFile, "%f", &pInfo->rot.y);
			fscanf(pFile, "%f", &pInfo->rot.z);
		}
		else if (strcmp(&aText[0], "DIR") == 0)
		{//����
			//�u���v��ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			int nDir = 0;	//�����ǂݍ��ݗp

			//������ǂݍ���
			fscanf(pFile, "%d", &nDir);

			switch (nDir)
			{
			case 0:	//��
				pInfo->dir = DIRECTION::DIR_LEFT;
				break;

			case 1:	//��
				pInfo->dir = DIRECTION::DIR_BACK;
				break;

			case 2:	//�E
				pInfo->dir = DIRECTION::DIR_RIGHT;
				break;

			case 3:	//��O
				pInfo->dir = DIRECTION::DIR_FRONT;
				break;

			default:	//���̑�
				assert(false);
				break;
			}
		}
	}
}

//================================================
//���ƕǂ̐ݒ�
//================================================
void CStage::CreateFloarAndWalls()
{
	if (m_pFloar == nullptr)
	{//nullptr�̏ꍇ
		//���̐���
		m_pFloar = CObject3D::Create();

		//���̐ݒ�
		m_pFloar->SetSize(D3DXVECTOR3(FLOAR_SIZE, 0.0f, FLOAR_SIZE));

		//�e�N�X�`���̐ݒ�
		m_pFloar->SetTexture(CTexture::TEXTURE::WallAndFloar);
	}

	int nCntCreate = 0;	//�������J�E���g�p

	for (int i = 0; i < DIRECTION::DIR_MAX; i++)
	{
		if (m_apWall[i] != nullptr)
		{//nullptr�ł͂Ȃ��ꍇ
			continue;
		}

		/* nullptr�̏ꍇ */

		//�ǂ̐���
		m_apWall[i] = CObject3D::Create();

		//�e�N�X�`���̐ݒ�
		m_apWall[i]->SetTexture(CTexture::TEXTURE::WallAndFloar);

		//�J�E���g�A�b�v
		nCntCreate++;
	}

	if (nCntCreate >= DIRECTION::DIR_MAX)
	{//�ǂ��S�Đ����o���Ă�����
		//�ǂ̐ݒ�
		SetWall();

		//�J�E���g���Z�b�g
		nCntCreate = 0;
	}
}

//================================================
//�ǂ̐ݒ�
//================================================
void CStage::SetWall()
{
	//�����p�ϐ�
	float fFloarSizeHalf = (FLOAR_SIZE * 0.5f);		//���T�C�Y�̔���
	float fWallHeightHalf = (WALL_HEIGHT * 0.5f);	//�ǂ̍����̔���

	//********** �� **********//

	//�����������
	DIRECTION dir = DIRECTION::DIR_LEFT;

	//�T�C�Y
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_HEIGHT, 0.0f, WALL_WIDTH));

	//�ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(
		m_pFloar->GetPos().x - fFloarSizeHalf,
		m_pFloar->GetPos().y + fWallHeightHalf,
		0.0f);
	m_apWall[dir]->SetPos(pos);

	//����
	m_apWall[dir]->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f));

	//********** �� **********//

	//�����������
	dir = DIRECTION::DIR_BACK;

	//�T�C�Y
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_WIDTH, 0.0f, WALL_HEIGHT));

	//�ʒu
	pos = D3DXVECTOR3(
		0.0f,
		m_pFloar->GetPos().y + fWallHeightHalf,
		m_pFloar->GetPos().z + fFloarSizeHalf);
	m_apWall[dir]->SetPos(pos);

	//����
	m_apWall[dir]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));

	//********** �E **********//

	//�����������
	dir = DIRECTION::DIR_RIGHT;

	//�T�C�Y
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_HEIGHT, 0.0f, WALL_WIDTH));

	//�ʒu
	pos = D3DXVECTOR3(
		m_pFloar->GetPos().x + fFloarSizeHalf,
		m_pFloar->GetPos().y + fWallHeightHalf,
		0.0f);
	m_apWall[dir]->SetPos(pos);

	//����
	m_apWall[dir]->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));

	//********** ��O **********//

	//�����������
	dir = DIRECTION::DIR_FRONT;

	//�T�C�Y
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_WIDTH, 0.0f, WALL_HEIGHT));

	//�ʒu
	pos = D3DXVECTOR3(
		0.0f,
		m_pFloar->GetPos().y + fWallHeightHalf,
		m_pFloar->GetPos().z - fFloarSizeHalf);
	m_apWall[dir]->SetPos(pos);

	//����
	m_apWall[dir]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));

	//�F
	m_apWall[dir]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
}