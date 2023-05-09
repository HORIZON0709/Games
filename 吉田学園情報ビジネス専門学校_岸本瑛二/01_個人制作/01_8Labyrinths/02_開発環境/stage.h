//================================================
//
//3D�Q�[��(��)[stage.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>
#include <stdio.h>

//***************************
//�O���錾
//***************************
class CObject3D;
class CObjectX;
class CGimmick;
class CItem;
class CDoor;

//***************************
//�X�e�[�W�N���X�̒�`
//***************************
class CStage
{/* ��{�N���X */
public: /* �񋓌^�̒�` */
	enum STAGE	//�X�e�[�W�̎��
	{
		NONE = -1,
		Stage01 = 0,	//�X�e�[�W1
		Stage02,		//�X�e�[�W2
		Stage03,		//�X�e�[�W3
		Stage04,		//�X�e�[�W4
		Stage05,		//�X�e�[�W5
		Stage06,		//�X�e�[�W6
		Stage07,		//�X�e�[�W7
		Stage08,		//�X�e�[�W8
		MAX
	};

	enum DIRECTION	//����
	{
		DIR_NONE = -1,
		DIR_LEFT = 0,	//��
		DIR_BACK,		//��
		DIR_RIGHT,		//�E
		DIR_FRONT,		//��O
		DIR_MAX
	};

private:
	enum MODEL_TYPE	//���f���^�C�v
	{
		TYPE_NONE = -1,
		TYPE_OBJECT = 0,	//�I�u�W�F�N�g
		TYPE_GIMMICK,		//�M�~�b�N
		TYPE_ITEM,			//�A�C�e��
		TYPE_DOOR,			//�h�A
		TYPE_MAX
	};

public: /* �萔�̒�` */
	static const char* s_apFileName[];	//�t�@�C���p�X

	static const int MAX_MODEL = 24;	//���f���̍ő吔
	static const int MAX_GIMMICK = 8;	//�M�~�b�N�̍ő吔
	static const int MAX_DOOR = 2;		//�h�A�̍ő吔
	static const int MAX_COIN = 3;		//�R�C���̍ő吔

	static const float ROT_DOOR;	//�h�A�̌���

	static const D3DXVECTOR3 POS_DOOR[DIRECTION::DIR_MAX];	//�h�A�̈ʒu
private:
	static const float FLOAR_SIZE;	//���̃T�C�Y
	static const float WALL_WIDTH;	//�ǂ̕�
	static const float WALL_HEIGHT;	//�ǂ̍���

	static const int MAX_WORD;	//�ő啶����

private: /* �\���̂̒�` */
	struct MODELSET_INFO	//���f���z�u�ɕK�v�ȏ��
	{
		int nIndex;			//�C���f�b�N�X��
		MODEL_TYPE type;	//���f���^�C�v
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 rot;	//����
		DIRECTION dir;		//����
	};

public: /* �ÓI�����o�֐� */
	/*
		����
		const STAGE &stage ---> �X�e�[�W
	*/
	static CStage* Create(const STAGE &stage);

	/*
		X���f�����̎擾
		int nIdx ---> �C���f�b�N�X��
	*/
	static CObjectX* GetObjectX(int nIdx);

	/*
		�M�~�b�N���̎擾
		int nIdx ---> �C���f�b�N�X��
	*/
	static CGimmick* GetGimmick(int nIdx);

	static CItem* GetItem();	//�A�C�e�����̎擾

	/*
		�h�A���̎擾
		int nIdx ---> �C���f�b�N�X��
	*/
	static CDoor* GetDoor(int nIdx);

public: /* �ÓI�����o�ϐ� */
	static CObjectX* m_apModel[MAX_MODEL];		//X���f���̃|�C���^
	static CGimmick* m_apGimmick[MAX_GIMMICK];	//�M�~�b�N�̃|�C���^
	static CItem* m_pItem;						//�A�C�e���̃|�C���^
	static CDoor* m_apDoor[MAX_DOOR];			//�h�A�̃|�C���^

private:
	static CObject3D* m_pFloar;						//3D�|���S���̏��̃|�C���^
	static CObject3D* m_apWall[DIRECTION::DIR_MAX];	//3D�|���S���̕�(�l��)�̃|�C���^

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CStage();
	~CStage();

public: /* �����o�֐� */
	/*
		������
		const STAGE &stage ---> �X�e�[�W�̎��
		const char* pStage ---> �X�e�[�W�̃t�@�C���p�X
	*/
	HRESULT Init(const STAGE &stage, const char* pStage);

	void Uninit();		//�I��
	void UninitAll();	//�S�Ă̏I��
	void Update();		//�X�V
	CStage* Set();		//�X�e�[�W�̐ݒ�
	STAGE Get();		//�X�e�[�W�̎擾

	/*
		�X�e�[�W�̕ύX
		const STAGE &stage ---> �X�e�[�W�̎��
	*/
	void Change(const STAGE &stage);

	/*
		�h�A�̕����̓ǂݍ���
		const STAGE &stage ---> �X�e�[�W�̎��
	*/
	void LoadDoorDir(const STAGE &stage);

	/*
		�h�A�̕������擾
		int nIdx ---> �ԍ�
	*/
	int GetDoorDir(int nIdx);

private:
	/*
		�ǂݍ���
		const char* pStage ---> �X�e�[�W�̃t�@�C���p�X
	*/
	void Load(const char* pStage);

	/*
		���f���Z�b�g�ݒ�
		FILE* pFile ---> �t�@�C���|�C���^
	*/
	void Set_ModelSet(FILE* pFile);

	void CreateFloarAndWalls();	//���ƕǂ̐���

	void SetWall();	//�ǂ̐ݒ�

private: /* �����o�ϐ� */
	MODELSET_INFO m_aModelSetInfo[MAX_MODEL];	//���f���ݒu�ɕK�v�ȏ��

	STAGE m_stage;		//���݂̃X�e�[�W
	STAGE m_stageNext;	//���̃X�e�[�W

	int m_nNumModel;	//���f����
	int m_nCntModelSet;	//�Z�b�g�σ��f�����J�E���g�p

	int m_aDir[MAX_DOOR];	//�h�A�̕���(�ԍ�)
};

#endif