//============================================================================================================
//
//君をたずねてにゃん千里[bg.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define NUM_BG		(3)			//背景の数
#define BG_SPEED	(0.1f)		//背景のスピード

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif