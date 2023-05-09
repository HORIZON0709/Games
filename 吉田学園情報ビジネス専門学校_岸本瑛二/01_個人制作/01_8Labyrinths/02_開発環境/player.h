//================================================
//
//3D�Q�[��(��)[player.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************
//�C���N���[�h
//***************************
#include "object.h"
#include "stage.h"

#include <stdio.h>

//***************************
//�O���錾
//***************************
class CModel;
class CItem;
class CLine;

//***************************
//�v���C���[�N���X�̒�`
//***************************
class CPlayer : public CObject
{/* CObject�̔h���N���X */
private: /* �񋓌^�̒�` */
	enum MOTION_TYPE	//���[�V�����̎��
	{
		NONE = -1,
		NEUTRAL = 0,	//�j���[�g����
		MOVE,			//�ړ�
		ATTACK,			//�U��
		MAX
	};

private: /* �萔�̒�` */
	static const float MOVE_SPEED;		//�ړ����x
	static const float ROT_SMOOTHNESS;	//��]�̊��炩��
	static const float LIMIT_POS;		//�ړ������ʒu
	
	static const int MAX_PARTS = 5;		//�ő�p�[�c��
	static const int NUM_KEYSET = 2;	//�L�[�Z�b�g��
	static const int MAX_LINE = 12;		//���C���̍ő吔
	static const int NUM_VTX_3D = 8;	//���̂̒��_��
	static const int MAX_WORD;			//�ő啶����

	static const char* FILE_NAME;	//�t�@�C����

private: /* �\���̂̒�` */
	struct KEY //�L�[�v�f
	{
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 rot;	//����
	};

	struct KEY_SET //�L�[���
	{
		int nFrame;				//�t���[����
		KEY aKey[MAX_PARTS];	//�L�[�v�f
	};

	struct MOTION_SET	//���[�V�������
	{
		bool bLoop;						//���[�v�����邩���Ȃ���
		int nNumKey;					//�L�[��
		KEY_SET aKeySet[NUM_KEYSET];	//�L�[���
	};

public: /* �ÓI�����o�֐� */
	static CPlayer* Create();	//����

private: /* �ÓI�����o�ϐ� */
	static MOTION_SET m_aMotion[MOTION_TYPE::MAX];	//���[�V�������

	static CLine* m_apLine[MAX_LINE];	//���C���̃|�C���^

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CPlayer();
	~CPlayer() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

public: /* �����o�֐� */
	D3DXVECTOR3 GetPos();			//�ʒu���擾
	D3DXVECTOR3 GetVtxMax();		//���_�̍ő�l���擾
	D3DXVECTOR3 GetVtxMin();		//���_�̍ŏ��l���擾
	bool GetIsGetItem();			//�A�C�e�����擾�������ǂ������擾

	/*
		�A�C�e���̊l���󋵂��擾
		int nIdx ---> �ԍ�
	*/
	CStage::STAGE GetStageCoin(int nIdx);

private:
	void Move();		//�ړ�
	void Motion();		//���[�V����
	void Collision();	//�����蔻��
	void StageChange();	//�X�e�[�W�؂�ւ�

	void SetVtxMaxAndMin();	//���_�̍ő�l�ƍŏ��l�̐ݒ�
	void SetLines();		//���C���̐ݒ�܂Ƃ�

	void Load();	//�ǂݍ���

	void Set_Motion(FILE* pFile, char aText[]);	//���[�V�����ݒ�
	void Set_KeySet(FILE* pFile, char aText[]);	//�L�[�Z�b�g�ݒ�
	void Set_Key(FILE* pFile, char aText[]);	//�L�[�ݒ�

private: /* �����o�ϐ� */
	CModel* m_pModel;	//���f���̃|�C���^
	CItem* m_pItem;		//�A�C�e���̃|�C���^

	CStage::STAGE m_aStageCoin[CStage::MAX_COIN];	//�l�������A�C�e�����������X�e�[�W

	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X

	D3DXVECTOR3 m_pos;		//���݂̈ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_vec;		//�����x�N�g��
	D3DXVECTOR3 m_rot;		//���݂̌���
	D3DXVECTOR3 m_rotDest;	//�ړI�̌���
	D3DXVECTOR3 m_vtxMax;	//���_�̍ő�l
	D3DXVECTOR3 m_vtxMin;	//���_�̍ŏ��l

	D3DXVECTOR3 m_aPosVtx[NUM_VTX_3D];	//���_�̈ʒu

	MOTION_TYPE m_motionType;	//���[�V�����̎��

	int m_nCurrentKey;	//���݂̃L�[�ԍ�
	int m_nCntMotion;	//���[�V�����J�E���^�[
	int m_nNumMotion;	//���[�V������
	int m_nNumKeySet;	//�L�[�Z�b�g��
	int m_nNumKey;		//�L�[��
	int m_nGetItem;		//�A�C�e���擾��

	bool m_bPressKey;		//�L�[���������ǂ���
	bool m_bCollObjectX;	//X���f���Ƃ̓����蔻��
	bool m_bCollGimmick;	//�M�~�b�N�Ƃ̓����蔻��
	bool m_bCollDoor;		//�h�A�Ƃ̓����蔻��
	bool m_bCollPanelStand;	//�p�l���X�^���h�Ƃ̓����蔻��
	bool m_bGetItem;		//�A�C�e�����擾�������ǂ���
	bool m_bFadeOut;		//�t�F�[�h�������ǂ���
	bool m_bCanMove;		//�ړ��ł��邩�ǂ���
};

#endif