//============================================================================================================
//
//君をたずねてにゃん千里[fade.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "fade.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static FADE					   s_fade;					//フェードの状態
static MODE					   s_modeNext;				//次の画面(モード)
static D3DXCOLOR			   s_colorFade;				//ポリゴン(フェード)の色

//============================================================================================================
//フェードの初期化処理
//============================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	VERTEX_2D *pVtx;								//頂点情報へのポインタ

	s_fade		= FADE_IN;								//フェードイン状態
	s_modeNext	= modeNext;								//次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//モードの設定
	SetMode(s_modeNext);
}

//============================================================================================================
//フェードの終了処理
//============================================================================================================
void UninitFade(void)
{
	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//フェードの更新処理
//============================================================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (s_fade != FADE_NONE)
	{//フェードしているとき
		if (s_fade == FADE_IN)
		{//フェードイン状態
			s_colorFade.a -= FADE_SPEED;	//ポリゴンを透明にしていく

			if (s_colorFade.a <= 0.0f)
			{
				s_colorFade.a = 0.0f;
				s_fade		  = FADE_NONE;		//何もしていない状態に
			}
		}
		else if (s_fade == FADE_OUT)
		{//フェードアウト状態
			s_colorFade.a += FADE_SPEED;	//ポリゴンを不透明にしていく

			if (s_colorFade.a >= 1.0f)
			{
				s_colorFade.a = 1.0f;
				s_fade		  = FADE_IN;	//フェードイン状態

				//モード設定(次の画面に移行)
				SetMode(s_modeNext);
			}
		}

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//フェードの描画処理
//============================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイス

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0,NULL);

	//敵の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数
}

//============================================================================================================
//フェードの設定処理
//============================================================================================================
void SetFade(MODE modeNext)
{
	s_fade		= FADE_OUT;								//フェードアウト状態に
	s_modeNext	= modeNext;								//次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒いポリゴン(透明)にしておく
}

//============================================================================================================
//フェードの取得処理
//============================================================================================================
FADE GetFade(void)
{
	return s_fade;
}