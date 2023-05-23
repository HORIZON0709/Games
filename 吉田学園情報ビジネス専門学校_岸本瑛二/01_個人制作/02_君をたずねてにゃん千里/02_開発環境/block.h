//============================================================================================================
//
//君をたずねてにゃん千里[block.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define MAX_BLOCK		(512)		//ブロックの最大数
#define BLOCK_WIDTH		(40.0f)		//ブロックの幅
#define BLOCK_HEIGHT	(40.0f)		//ブロックの高さ
#define BLOCK_MOVE		(-3.0f)		//ブロックの移動量(MapSet用)

//============================================================================================================
//ブロックの構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用しているかどうか
}BLOCK;

//============================================================================================================
//プロトタイプ宣言
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