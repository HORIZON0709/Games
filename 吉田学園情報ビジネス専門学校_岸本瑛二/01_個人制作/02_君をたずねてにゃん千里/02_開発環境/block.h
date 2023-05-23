//============================================================================================================
//
//�N�������˂Ăɂ��痢[block.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define MAX_BLOCK		(512)		//�u���b�N�̍ő吔
#define BLOCK_WIDTH		(40.0f)		//�u���b�N�̕�
#define BLOCK_HEIGHT	(40.0f)		//�u���b�N�̍���
#define BLOCK_MOVE		(-3.0f)		//�u���b�N�̈ړ���(MapSet�p)

//============================================================================================================
//�u���b�N�̍\���̂̒�`
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}BLOCK;

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight);
bool CollisionBlockPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void CollisionBlockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight,BLOCK **pBLK);
BLOCK *GetBlock(void);

#endif