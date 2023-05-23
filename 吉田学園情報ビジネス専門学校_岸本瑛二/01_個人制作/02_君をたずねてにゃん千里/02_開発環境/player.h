//============================================================================================================
//
//�N�������˂Ăɂ��痢[player.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _PLAYER_H_			//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_			//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define PLAYER_ANIMSPEED	(7)			//�v���C���[�̃A�j���[�V�����X�s�[�h
#define PL_PATTERN_X		(4)			//�A�j���[�V�����摜�̉��̐�
#define PL_PATTERN_Y		(2)			//�A�j���[�V�����摜�̏c�̐�
#define PL_DIRECTION_RIGHT	(0)			//�E����
#define PL_DIRECTION_LEFT	(1)			//������
#define PLAYERPOS_X			(100.0f)	//�v���C���[�̏����ʒu(X)
#define PLAYERPOS_Y			(720.0f)	//�v���C���[�̏����ʒu(Y)
#define PLAYER_WIDTH		(64.0f)		//�v���C���[�̕�
#define PLAYER_HEIGHT		(72.0f)		//�v���C���[�̍���
#define PLAYER_SPEED		(1.0f)		//�v���C���[�̃X�s�[�h(���{��)
#define SPEED_DOWN			(0.1f)		//�����W��
#define SPEED_STOP			(0.0f)		//�~�܂��Ă���
#define PLAYER_LIFE			(30)		//�v���C���[�̃��C�t
#define PLAYER_JUMP			(-23.0f)	//�v���C���[�̃W�����v��
#define PLAYER_GRAVITY		(1.2f)		//�v���C���[�ɂ�����d��
#define BLINKING_SPEED		(0.2f)		//�v���C���[�̓_�ł̑���
#define BLINKING_DEGREE		(0.2f)		//�v���C���[�̓_�ŉ���( 0��1 / �������Z��)
#define BLINKING_SIZE		(0.5f)		//�v���C���[�̓_�ł̐U�ꕝ

#define PLAYER_STOCK		(3)			//�v���C���[�̎c�@
#define STOCK_POS_X			(50.0f)		//�c�@�̏����ʒu(X)
#define STOCK_POS_Y			(50.0f)		//�c�@�̏����ʒu(Y)
#define STOCK_SIZE			(50.0f)		//�c�@�̃T�C�Y(���A����)
#define STOCK_INTERVAL		(15.0f)		//�c�@���m�̊Ԋu

//============================================================================================================
//�v���C���[�̏��
//============================================================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//�o�����(�_��)
	PLAYERSTATE_NORMAL,			//�ʏ���
	PLAYERSTATE_DAMAGE,			//�_���[�W���
	PLAYERSTATE_DEATH,			//���S���
	PLAYERSTATE_INVINCIBLE,		//���G���
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//============================================================================================================
//�v���C���[�\���̂̒�`
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(����)
	D3DXVECTOR3 posOld;		//�ʒu(�O��)
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	PLAYERSTATE state;		//�v���C���[�̏��
	int nCounterState;		//��ԊǗ��J�E���^�[
	int nLife;				//�̗�
	int nStock;				//�c�@
	int nCntAnim;			//�J�E���^�[
	int nPtnAnim;			//�p�^�[���ԍ�
	int nDirectionMove;		//����
	float fWidth;			//�v���C���[�̕�
	float fHeight;			//�v���C���[�̍���
	bool bJump;				//�W�����v���Ă��邩�ǂ���
	bool bDisp;				//�\�����邩���Ȃ���
}Player;

//============================================================================================================
//�c�@�̍\���̂̒�`
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p����Ă��邩�ǂ���
}STOCK;

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
void CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight);
Player *GetPlayer(void);
PLAYERSTATE GetPlayerState(void);

#endif // !_PLAYER_H_