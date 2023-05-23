//============================================================================================================
//
//君をたずねてにゃん千里[enemy.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "block.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define MAX_ENEMY				(64)		//敵の最大数
#define ENEMY_PATTERN_X			(4)			//アニメーション画像の数(X)
#define ENEMY_PATTERN_Y			(2)			//アニメーション画像の数(Y)
#define ENEMY_ANIM_SPEED		(10)		//アニメーションスピード
#define ENEMY_WIDTH				(64.0f)		//敵の幅
#define ENEMY_HEIGHT			(72.0f)		//敵の高さ
#define ENEMY_LIFE				(30)		//敵の体力
#define ENEMY_GROUND_MOVE		(-1.5f)		//敵(地上)の移動量(MaSet用)
#define ENEMY_ONBLOCK_MOVE		(-1.0f)		//敵(ブロックの上)の移動量(MaSet用)
#define ENEMY_DIRECTION_RIGHT	(0)			//右
#define ENEMY_DIRECTION_LEFT	(1)			//左
#define ENEMY_GRAVITY			(1.2f)		//敵にかかる重力
#define DAMAGE_FROM_PL			(30)		//プレイヤーからのダメージ
#define COUNTER_ENEMY			(10)		//敵のダメージ状態の継続時間
#define ENEMY_POINT				(50)		//敵のポイント

//============================================================================================================
//敵の状態
//============================================================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常状態
	ENEMYSTATE_DAMAGE,			//ダメージ状態	
	ENEMYSTATE_MAX
}ENEMYSTATE;

//============================================================================================================
//敵の動き方
//============================================================================================================
typedef enum
{
	MOVETYPE_GROUND = 0,	//地上
	MOVETYPE_ONBLOCK,		//ブロックの上
	MOVETYPE_MAX
}MOVETYPE;

//============================================================================================================
//敵構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	ENEMYSTATE state;		//状態
	MOVETYPE type;			//動き方
	int nCounterState;		//状態管理カウンター
	int nLife;				//体力
	int nCntAnim;			//カウンター
	int nPtnAnim;			//パターン番号
	int nDirectionMove;		//向き
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用しているかどうか
	BLOCK *pBLK;			//着地しているブロックの情報
}Enemy;

//============================================================================================================
//プロトタイプ宣言
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