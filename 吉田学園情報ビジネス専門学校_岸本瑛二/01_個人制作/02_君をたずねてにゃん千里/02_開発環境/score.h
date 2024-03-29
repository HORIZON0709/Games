//============================================================================================================
//
//君をたずねてにゃん千里[score.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define MAX_SCORE			(8)			//スコアの最大桁数
#define SCORE_POS_X			(1035.0f)	//スコアの位置(X)
#define SCORE_POS_Y			(10.0f)		//スコアの位置(Y)
#define SCORE_WIDTH			(30)		//スコアの幅
#define SCORE_HEIGHT		(45)		//スコアの高さ
#define ENEMY_ADDSCORE		(200)		//敵のスコア(+)
#define ENEMY_SUBSCORE		(50)		//敵のスコア(-)
#define ENEMY_SPECIALSCORE	(400)		//敵のスコア(スペシャル)

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
void SubScore(int nValue);
int GetScore(void);

#endif