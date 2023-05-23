//============================================================================================================
//
//君をたずねてにゃん千里[bg,cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "bg.h"
#include "input.h"
#include "player.h"
#include "goal.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_apTexture[NUM_BG] = { NULL,NULL, NULL };	//テクスチャ(3枚分)へのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;							//頂点バッファへのポインタ
static D3DXVECTOR2				s_aPosTexUV[NUM_BG];						//テクスチャ座標の開始位置(UV値)
static D3DXVECTOR2				s_aMoveBG;									//背景の移動量

//============================================================================================================
//背景の初期化処理
//============================================================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ
	int nCntBG;									//背景のカウント

	//テクスチャの読み込み(３枚分)

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/bg101.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/空と雲.png",
								&s_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/住宅街_家々_2_Copy.png",
								&s_apTexture[2]);

	//初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		s_aPosTexUV[nCntBG] = D3DXVECTOR2(0.0f, 0.0f);
	}

	s_aMoveBG = D3DXVECTOR2(BG_SPEED, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
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

		pVtx += 4;		//ポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//背景の終了処理
//============================================================================================================
void UninitBG(void)
{
	//テクスチャの破棄
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (s_apTexture[nCntBG] != NULL)
		{
			s_apTexture[nCntBG]->Release();
			s_apTexture[nCntBG] = NULL;
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
//背景の更新処理
//============================================================================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ
	Player *pPlayer = GetPlayer();	//プレイヤーの取得
	int nCntBG;						//背景のカウント

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!GetPosGoal())
	{//ゴールが指定の位置にいない
		if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
		{//プレイヤーの右端が画面中央 & 動いている

			//プレイヤーのmoveを背景のmoveに代入(進行方向と逆)
			s_aMoveBG.x = -(pPlayer->move.x * 0.001f);

			//位置を更新
			for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
			{
				s_aPosTexUV[nCntBG] = s_aMoveBG;
			}

			for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
			{
				//テクスチャ座標の更新
				pVtx[0].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);
				pVtx[1].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);
				pVtx[2].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);
				pVtx[3].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);

				pVtx += 4;		//ポインタを4つ分進める
			}
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//背景の描画処理
//============================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	int nCntBG;									//背景のカウント

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTexture[nCntBG]);

		//背景の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCntBG * 4,				//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}
}