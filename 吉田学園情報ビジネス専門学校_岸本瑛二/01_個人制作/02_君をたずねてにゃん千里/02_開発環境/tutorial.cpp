//============================================================================================================
//
//君をたずねてにゃん千里[tutorial.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "tutorial.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_apTexture[NUM_TUTORIAL] = { NULL,NULL };	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff				  = NULL;			//頂点バッファへのポインタ
static int						s_nCountTex;								//テクスチャのカウント

//============================================================================================================
//チュートリアルの初期化処理
//============================================================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Tutorial_01.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Tutorial_02.png",
								&s_apTexture[1]);

	s_nCountTex = 0;		//テクスチャのカウントを初期化する

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
}

//============================================================================================================
//チュートリアルの終了処理
//============================================================================================================
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	for (int i = 0; i < NUM_TUTORIAL; i++)
	{
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
//チュートリアルの更新処理
//============================================================================================================
void UpdateTutorial(void)
{
	if (GetFade() == FADE_NONE)
	{//フェードしていないとき
		if (GetKeyboardTrigger(DIK_D))
		{//Dキーが押された
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_SELECT);

			s_nCountTex = 1;
		}
		else if (GetKeyboardTrigger(DIK_A))
		{//Aキーが押された
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_SELECT);

			s_nCountTex = 0;
		}
		else if (GetKeyboardTrigger(DIK_RETURN))
		{//Enterキーが押された
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_DECIDE);

			//ゲーム画面に移行
			SetFade(MODE_GAME);
		}
	}
}

//============================================================================================================
//チュートリアルの描画処理
//============================================================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_apTexture[s_nCountTex]);

	//チュートリアルの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数
}

//============================================================================================================
//チュートリアルの取得
//============================================================================================================
int GetTutorial(void)
{
	return s_nCountTex;
}