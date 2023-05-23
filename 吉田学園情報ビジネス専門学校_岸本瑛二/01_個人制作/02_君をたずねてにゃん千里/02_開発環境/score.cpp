//============================================================================================================
//
//君をたずねてにゃん千里[score.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "score.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static D3DXVECTOR3				s_posScore;					//スコアの位置
static int						s_nScore;					//スコアの値

//============================================================================================================
//スコアの初期化処理
//============================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/number000.png",
								&s_pTexture);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//スコアの情報の初期化
		s_posScore = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);	//スコアの位置を初期化
		s_nScore   = 0;												//スコアの値を初期化
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH),				 s_posScore.y,		  		  0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH, s_posScore.y,				  0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH),				 s_posScore.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH, s_posScore.y + SCORE_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//スコアの終了処理
//============================================================================================================
void UninitScore(void)
{
	//テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//スコアの更新処理
//============================================================================================================
void UpdateScore(void)
{

}

//============================================================================================================
//スコアの描画処理
//============================================================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		//スコアの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCnt * 4,				//描画する最初の頂点インデックス
								2 * MAX_SCORE);			//描画するプリミティブ数
	}
}

//============================================================================================================
//スコアの設定処理
//============================================================================================================
void SetScore(int nScore)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nScore	= nScore;
	aPosTexU[0] = s_nScore % 100000000 / 10000000;
	aPosTexU[1] = s_nScore % 10000000 / 1000000;
	aPosTexU[2] = s_nScore % 1000000 / 100000;
	aPosTexU[3] = s_nScore % 100000 / 10000;
	aPosTexU[4] = s_nScore % 10000 / 1000;
	aPosTexU[5] = s_nScore % 1000 / 100;
	aPosTexU[6] = s_nScore % 100 / 10;
	aPosTexU[7] = s_nScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//スコアの加算処理
//============================================================================================================
void AddScore(int nValue)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nScore   += nValue;
	aPosTexU[0]	= s_nScore % 100000000 / 10000000;
	aPosTexU[1] = s_nScore % 10000000 / 1000000;
	aPosTexU[2] = s_nScore % 1000000 / 100000;
	aPosTexU[3] = s_nScore % 100000 / 10000;
	aPosTexU[4] = s_nScore % 10000 / 1000;
	aPosTexU[5] = s_nScore % 1000 / 100;
	aPosTexU[6] = s_nScore % 100 / 10;
	aPosTexU[7] = s_nScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//スコアの減算処理
//============================================================================================================
void SubScore(int nValue)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	//減算する
	s_nScore -= nValue;

	if (s_nScore <= 0)
	{//0以下だったら
		s_nScore = 0;
	}

	aPosTexU[0] = s_nScore % 100000000 / 10000000;
	aPosTexU[1] = s_nScore % 10000000 / 1000000;
	aPosTexU[2] = s_nScore % 1000000 / 100000;
	aPosTexU[3] = s_nScore % 100000 / 10000;
	aPosTexU[4] = s_nScore % 10000 / 1000;
	aPosTexU[5] = s_nScore % 1000 / 100;
	aPosTexU[6] = s_nScore % 100 / 10;
	aPosTexU[7] = s_nScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//スコアの取得
//============================================================================================================
int GetScore(void)
{
	return s_nScore;
}