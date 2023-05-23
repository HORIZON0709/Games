//============================================================================================================
//
//�N�������˂Ăɂ��痢[enemy.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "block.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define MAX_ENEMY				(64)		//�G�̍ő吔
#define ENEMY_PATTERN_X			(4)			//�A�j���[�V�����摜�̐�(X)
#define ENEMY_PATTERN_Y			(2)			//�A�j���[�V�����摜�̐�(Y)
#define ENEMY_ANIM_SPEED		(10)		//�A�j���[�V�����X�s�[�h
#define ENEMY_WIDTH				(64.0f)		//�G�̕�
#define ENEMY_HEIGHT			(72.0f)		//�G�̍���
#define ENEMY_LIFE				(30)		//�G�̗̑�
#define ENEMY_GROUND_MOVE		(-1.5f)		//�G(�n��)�̈ړ���(MaSet�p)
#define ENEMY_ONBLOCK_MOVE		(-1.0f)		//�G(�u���b�N�̏�)�̈ړ���(MaSet�p)
#define ENEMY_DIRECTION_RIGHT	(0)			//�E
#define ENEMY_DIRECTION_LEFT	(1)			//��
#define ENEMY_GRAVITY			(1.2f)		//�G�ɂ�����d��
#define DAMAGE_FROM_PL			(30)		//�v���C���[����̃_���[�W
#define COUNTER_ENEMY			(10)		//�G�̃_���[�W��Ԃ̌p������
#define ENEMY_POINT				(50)		//�G�̃|�C���g

//============================================================================================================
//�G�̏��
//============================================================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//�ʏ���
	ENEMYSTATE_DAMAGE,			//�_���[�W���	
	ENEMYSTATE_MAX
}ENEMYSTATE;

//============================================================================================================
//�G�̓�����
//============================================================================================================
typedef enum
{
	MOVETYPE_GROUND = 0,	//�n��
	MOVETYPE_ONBLOCK,		//�u���b�N�̏�
	MOVETYPE_MAX
}MOVETYPE;

//============================================================================================================
//�G�\���̂̒�`
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	ENEMYSTATE state;		//���
	MOVETYPE type;			//������
	int nCounterState;		//��ԊǗ��J�E���^�[
	int nLife;				//�̗�
	int nCntAnim;			//�J�E���^�[
	int nPtnAnim;			//�p�^�[���ԍ�
	int nDirectionMove;		//����
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
	BLOCK *pBLK;			//���n���Ă���u���b�N�̏��
}Enemy;

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, MOVETYPE type);
void HitEnemy(int nCnt, int nDamage);
void CollisionEnemyPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
Enemy *GetEnemy(void);

#endif