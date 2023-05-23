//============================================================================================================
//
//君をたずねてにゃん千里[sound.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//============================================================================================================
// サウンドファイル
//============================================================================================================
typedef enum
{
	SOUND_LABEL_TITLE,			//タイトルBGM
	SOUND_LABEL_GAME,			//ゲームBGM
	SOUND_LABEL_GAMECLEAR,		//ゲームクリアBGM
	SOUND_LABEL_RANKING,		//ランキングBGM
	SOUND_LABEL_SE_GAMEOVER,	//ゲームオーバー音
	SOUND_LABEL_SE_PAUSE,		//ポーズ音
	SOUND_LABEL_SE_SELECT,		//セレクト音
	SOUND_LABEL_SE_DECIDE,		//決定音
	SOUND_LABEL_SE_CANCEL,		//キャンセル音
	SOUND_LABEL_SE_JUMP,		//ジャンプ音
	SOUND_LABEL_SE_TRAMP,		//踏みつけ音
	SOUND_LABEL_SE_ITEMGET,		//アイテム取得音
	SOUND_LABEL_SE_DAMAGE,		//ダメージ音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//============================================================================================================
// プロトタイプ宣言
//============================================================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif