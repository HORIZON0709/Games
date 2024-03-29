//============================================================================================================
//
//君をたずねてにゃん千里[ranking.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define MAX_RANK			(5)						//ランク数
#define RANK_POS_X			(150.0f)				//順位の初期位置(X)
#define RANK_POS_Y			(470.0f)				//順位の初期位置(Y)
#define RANK_WIDTH			(75)					//順位の幅
#define RANK_HEIGHT			(210)					//順位の高さ
#define RANKINGSCORE_POS_X	(RANK_POS_X + 150.0f)	//ランキングスコアの初期位置(X)
#define RANKINGSCORE_POS_Y	(RANK_POS_Y - 190.0f)	//ランキングスコアの初期位置(Y)
#define RANKINGSCORE_WIDTH	(30)					//ランキングスコアの幅
#define RANKINGSCORE_HEIGHT	(45)					//ランキングスコアの高さ
#define RANKINGSCORE_SPACE	(40)					//ランキングスコアの間隔
#define RANKING_TIME		(10)					//ランキングの表示時間
#define RANKING_FADE_SPEED	(0.02f)					//ランキングの点滅スピード
#define RANKING_LOGO_POS_X	(315.0f)				//ロゴの位置(X)
#define RANKING_LOGO_POS_Y	(190.0f)				//ロゴの位置(Y)
#define RANKING_LOGO_WIDTH	(200.0f)				//ロゴの幅
#define RANKING_LOGO_HEIGHT	(50.0f)					//ロゴの高さ

//============================================================================================================
//フェードの状態
//============================================================================================================
typedef enum
{
	RANK_IN = 0,		//透明にする
	RANK_OUT,			//不透明にする
	RANK_MAX
}RANK;

//============================================================================================================
//ランキングスコアの構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	int nScore;			//スコア
}RankScore;

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);
void SaveRanking(void);
void LoadRanking(void);

#endif