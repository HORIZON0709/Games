//============================================================================================================
//
//君をたずねてにゃん千里[player.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _PLAYER_H_			//このマクロ定義がされていなかったら
#define _PLAYER_H_			//２重インクルード防止のマクロ定義

#include "main.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define PLAYER_ANIMSPEED	(7)			//プレイヤーのアニメーションスピード
#define PL_PATTERN_X		(4)			//アニメーション画像の横の数
#define PL_PATTERN_Y		(2)			//アニメーション画像の縦の数
#define PL_DIRECTION_RIGHT	(0)			//右向き
#define PL_DIRECTION_LEFT	(1)			//左向き
#define PLAYERPOS_X			(100.0f)	//プレイヤーの初期位置(X)
#define PLAYERPOS_Y			(720.0f)	//プレイヤーの初期位置(Y)
#define PLAYER_WIDTH		(64.0f)		//プレイヤーの幅
#define PLAYER_HEIGHT		(72.0f)		//プレイヤーの高さ
#define PLAYER_SPEED		(1.0f)		//プレイヤーのスピード(何倍か)
#define SPEED_DOWN			(0.1f)		//減衰係数
#define SPEED_STOP			(0.0f)		//止まっている
#define PLAYER_LIFE			(30)		//プレイヤーのライフ
#define PLAYER_JUMP			(-23.0f)	//プレイヤーのジャンプ力
#define PLAYER_GRAVITY		(1.2f)		//プレイヤーにかかる重力
#define BLINKING_SPEED		(0.2f)		//プレイヤーの点滅の速さ
#define BLINKING_DEGREE		(0.2f)		//プレイヤーの点滅加減( 0→1 / 薄い→濃い)
#define BLINKING_SIZE		(0.5f)		//プレイヤーの点滅の振れ幅

#define PLAYER_STOCK		(3)			//プレイヤーの残機
#define STOCK_POS_X			(50.0f)		//残機の初期位置(X)
#define STOCK_POS_Y			(50.0f)		//残機の初期位置(Y)
#define STOCK_SIZE			(50.0f)		//残機のサイズ(幅、高さ)
#define STOCK_INTERVAL		(15.0f)		//残機同士の間隔

//============================================================================================================
//プレイヤーの状態
//============================================================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//出現状態(点滅)
	PLAYERSTATE_NORMAL,			//通常状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_DEATH,			//死亡状態
	PLAYERSTATE_INVINCIBLE,		//無敵状態
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//============================================================================================================
//プレイヤー構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置(現在)
	D3DXVECTOR3 posOld;		//位置(前回)
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	PLAYERSTATE state;		//プレイヤーの状態
	int nCounterState;		//状態管理カウンター
	int nLife;				//体力
	int nStock;				//残機
	int nCntAnim;			//カウンター
	int nPtnAnim;			//パターン番号
	int nDirectionMove;		//向き
	float fWidth;			//プレイヤーの幅
	float fHeight;			//プレイヤーの高さ
	bool bJump;				//ジャンプしているかどうか
	bool bDisp;				//表示するかしないか
}Player;

//============================================================================================================
//残機の構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用されているかどうか
}STOCK;

//============================================================================================================
//プロトタイプ宣言
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