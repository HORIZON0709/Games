//============================================================================================================
//
//君をたずねてにゃん千里[game.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "game.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "item.h"
#include "map.h"
#include "goal.h"
#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "time.h"
#include "sound.h"
#include "fade.h"
//#include "effect.h"
#include "pause.h"
#include "ranking.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static GAMESTATE s_gameState			= GAMESTATE_NONE;	//ゲームの状態
static int		 s_nCounterGameState	= 0;				//状態管理カウンター
static bool		 s_pPause				= false;			//ポーズ中かどうか

//============================================================================================================
//ゲームの初期化処理
//============================================================================================================
void InitGame(void)
{
	//背景の初期化処理
	InitBG();

	//ブロックの初期化処理
	InitBlock();

	//アイテムの初期化処理
	InitItem();

	//敵の初期化処理
	InitEnemy();

	//ゴールの初期化処理
	InitGoal();

	////エフェクトの初期化処理
	//InitEffect();

	//プレイヤーの初期化処理
	InitPlayer();

	////ポップの初期化処理
	//InitPop();

	//スコアの初期化処理
	InitScore();

	//スコアの設定処理
	SetScore(0);

	//タイムの初期化処理
	InitTime();
	SetTime(TIME_LIMIT);

	//ポーズの初期化処理
	InitPause();

	//マップの設定
	SetMap(FILE_MAP);

	//サウンドの再生
	PlaySound(SOUND_LABEL_GAME);

	s_gameState			= GAMESTATE_NORMAL;		//通常状態
	s_nCounterGameState = 0;					//カウンターの初期化
	s_pPause			= false;				//ポーズ解除
}

//============================================================================================================
//ゲームの終了処理
//============================================================================================================
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//背景の終了処理
	UninitBG();

	//ブロックの終了処理
	UninitBlock();
	
	//アイテムの終了処理
	UninitItem();

	//ゴールの終了処理
	UninitGoal();

	////エフェクトの終了処理
	//UninitEffect();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//スコアの終了処理
	UninitScore();

	//タイムの終了処理
	UninitTime();

	//ポーズの終了処理
	UninitPause();
}

//============================================================================================================
//ゲームの更新処理
//============================================================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{//Pキーが押された
		s_pPause = s_pPause ? false : true;

		if (s_pPause)
		{//ポーズ中
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_PAUSE);
		}
		else if (!s_pPause)
		{//ポーズ中ではない
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}
	}

	if (s_pPause)
	{//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}
	else if (!s_pPause)
	{//ポーズ中でなければ
		
		switch (s_gameState)
		{
		case GAMESTATE_NORMAL:		//通常状態
			//背景の更新処理
			UpdateBG();

			//ブロックの更新処理
			UpdateBlock();

			//アイテムの更新処理
			UpdateItem();

			//ゴールの更新処理
			UpdateGoal();

			////エフェクトの更新処理
			//UpdateEffect();

			//敵の更新処理
			UpdateEnemy();

			//スコアの更新処理
			UpdateScore();

			//プレイヤーの更新処理
			UpdatePlayer();

			//タイムの更新処理
			UpdateTime();
			break;

		case GAMESTATE_END:			//終了状態

			s_nCounterGameState++;

			if (s_nCounterGameState >= 60)
			{
				s_gameState = GAMESTATE_NONE;		//何もしていない状態

				//画面(モード)の設定
				SetFade(MODE_RESULT);
			}
			break;
		}
	}
}

//============================================================================================================
//ゲームの描画処理
//============================================================================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//敵の描画処理
	DrawEnemy();

	////エフェクトの描画処理
	//DrawEffect();

	//ブロックの描画処理
	DrawBlock();

	//アイテムの描画処理
	DrawItem();

	//ゴールの描画処理
	DrawGoal();

	//プレイヤーの描画処理
	DrawPlayer();

	//スコアの描画処理
	DrawScore();

	//タイムの描画処理
	DrawTime();

	if (s_pPause)
	{//ポーズ中
		//ポーズの描画処理
		DrawPause();
	}
}

//============================================================================================================
//ゲームの設定処理
//============================================================================================================
void SetGameState(GAMESTATE state)
{
	s_gameState	= state;
	s_nCounterGameState = 0;
}

//============================================================================================================
//ゲームの取得処理
//============================================================================================================
GAMESTATE GetGamaState(void)
{
	return s_gameState;
}

//============================================================================================================
//ポーズの有効無効設定
//============================================================================================================
void SetEnablePause(bool bPause)
{
	s_pPause = bPause;
}