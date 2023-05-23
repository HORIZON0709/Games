//============================================================================================================
//
//君をたずねてにゃん千里[result.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "goal.h"
#include "sound.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_apTexture[GAME_MAX]	= { NULL,NULL };		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff				= NULL;					//頂点バッファへのポインタ

//============================================================================================================
//リザルトの初期化処理
//============================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/GAME_OVER.png",
								&s_apTexture[GAME_OVER]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/GAME_CLEAR.png",
								&s_apTexture[GAME_CLEAR]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	if (GetGoal())
	{//ゲームクリア
		//サウンドの再生
		PlaySound(SOUND_LABEL_GAMECLEAR);
	}
	else
	{//ゲームオーバー
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_GAMEOVER);
	}
}

//============================================================================================================
//リザルトの終了処理
//============================================================================================================
void UninitResult(void)
{
	//サウンドの停止
	StopSound();

	for (int i = 0; i < GAME_MAX; i++)
	{
		//テクスチャの破棄
		if (s_apTexture[i] != NULL)
		{
			s_apTexture[i]->Release();
			s_apTexture[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//リザルトの更新処理
//============================================================================================================
void UpdateResult(void)
{
	if (GetFade() == FADE_NONE)
	{//フェードしていないとき
		if (GetKeyboardTrigger(DIK_RETURN))
		{//Enterキーが押されたら
			//ランキング画面に移行
			SetFade(MODE_RANKING);
		}
	}
}

//============================================================================================================
//リザルトの描画処理
//============================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (GetGoal())
	{//ゲームクリア
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTexture[GAME_CLEAR]);

		//リザルトの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								0,						//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}
	else
	{//ゲームオーバー
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTexture[GAME_OVER]);

		//リザルトの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								0,						//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}
}