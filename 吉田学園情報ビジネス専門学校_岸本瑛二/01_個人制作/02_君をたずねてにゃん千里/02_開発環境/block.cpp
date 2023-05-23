//============================================================================================================
//
//君をたずねてにゃん千里[block,cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "block.h"
#include "player.h"
#include "enemy.h"
#include "goal.h"
#include "time.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9	   s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static BLOCK				   s_aBlock[MAX_BLOCK];		//ブロックの情報

//============================================================================================================
//ブロックの初期化処理
//============================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ
	int nCntBlock;								//ブロックのカウント

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/block004.jpg",
								&s_pTexture);

	//ブロックの情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		s_aBlock[nCntBlock].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置を初期化
		s_aBlock[nCntBlock].posOld	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置を初期化
		s_aBlock[nCntBlock].move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量を初期化
		s_aBlock[nCntBlock].fWidth	= 0.0f;								//幅を初期化
		s_aBlock[nCntBlock].fHeight = 0.0f;								//高さを初期化
		s_aBlock[nCntBlock].bUse	= false;							//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		pVtx += 4;		//ポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//ブロックの終了処理
//============================================================================================================
void UninitBlock(void)
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
//ブロックの更新処理
//============================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		BLOCK *pBlock = &s_aBlock[nCntBlock];	//ポインタに変身

		if (!pBlock->bUse)
		{//使用されていない
			continue;
		}

		/*↓ブロックが使用されている↓*/

		Player *pPlayer = GetPlayer();			//プレイヤー情報

		//前回の位置を保存
		pBlock->posOld = pBlock->pos;

		if (!GetPosGoal())
		{//ゴールが指定の位置にいない
			//プレイヤーの移動量を(ちょっといじくって)入れる
			pBlock->move.x = -(pPlayer->move.x * 0.7f);

			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//プレイヤーの右端が画面中央 & 動いている
				//位置を更新
				pBlock->pos.x += pBlock->move.x;
			}
		}

		//頂点座標の設定
		float fLeft		= (pBlock->pos.x - pBlock->fWidth);		//左
		float fRight	= (pBlock->pos.x + pBlock->fWidth);		//右
		float fUp		= (pBlock->pos.y - pBlock->fHeight);	//上
		float fDown		= (pBlock->pos.y + pBlock->fHeight);	//下

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		pVtx += 4;		//ポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//ブロックの描画処理
//============================================================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	int nCntBlock;								//ブロックのカウント

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (s_aBlock[nCntBlock].bUse)
		{//ブロックが使用されている
			 //テクスチャの設定
			pDevice->SetTexture(0, s_pTexture);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
									nCntBlock * 4,			//描画を開始する頂点インデックス
									2);						//描画するプリミティブ数
		}
	}
}

//============================================================================================================
//ブロックの設定処理
//============================================================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntBlock;		//ブロックのカウント

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		BLOCK *pBlock = &s_aBlock[nCntBlock];	//ポインタに変身

		if (!pBlock->bUse)
		{//ブロックが使用されていない
			pBlock->pos		= pos;
			pBlock->move	= move;
			pBlock->fWidth	= fWidth;
			pBlock->fHeight = fHeight;
			pBlock->bUse	= true;		//使用している状態にする

			//頂点座標の設定
			pVtx[0].pos.x = pos.x - fWidth;
			pVtx[0].pos.y = pos.y - fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pos.x + fWidth;
			pVtx[1].pos.y = pos.y - fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pos.x - fWidth;
			pVtx[2].pos.y = pos.y + fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pos.x + fWidth;
			pVtx[3].pos.y = pos.y + fHeight;
			pVtx[3].pos.z = 0.0f;

			//テクスチャ座標の設定
			float fTexU = fWidth  / (BLOCK_WIDTH / 2);
			float fTexV = fHeight / (BLOCK_HEIGHT / 2);

			SetTex(pVtx, 0.0f, fTexU, 0.0f, fTexV);
			break;
		}

		pVtx += 4;		//ポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//ブロックとプレイヤーの当たり判定
//============================================================================================================
bool CollisionBlockPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bLand = false;		//着地しているかどうか

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		BLOCK *pBlock = &s_aBlock[i];	//ポインタに変身
		
		if (!pBlock->bUse)
		{//ブロックが使用されていない
			continue;
		}

		/*↓ブロックが使用されている↓*/

		if (pPos->x + fWidth > pBlock->pos.x - pBlock->fWidth &&
			pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth)
		{//プレイヤーの幅がブロックの範囲内にある
			if (pPosOld->y <= pBlock->pos.y - pBlock->fHeight &&
				pPos->y		> pBlock->pos.y - pBlock->fHeight)
			{//上側
				bLand = true;
				pPos->y	 = pBlock->pos.y - pBlock->fHeight;
				pMove->y = 0.0f;

				if (pBlock->move.x != SPEED_STOP)
				{//ブロックが横に移動しているとき
					pPos->x += (pBlock->move.x * 0.1f);
				}
				else if (pBlock->move.y != SPEED_STOP)
				{//ブロックが縦に移動しているとき
					pPos->y += pBlock->move.y;
				}
			}
			else if (pPosOld->y - fHeight >= pBlock->pos.y + pBlock->fHeight &&
					 pPos->y - fHeight	   < pBlock->pos.y + pBlock->fHeight)
			{//下側
				pPos->y  = pBlock->pos.y + pBlock->fHeight + fHeight;
				pMove->y = 0.0f;
			}
		}
		
		if (pPosOld->y - fHeight < pBlock->pos.y + pBlock->fHeight &&
			pPosOld->y			 > pBlock->pos.y - pBlock->fHeight)
		{//プレイヤーの前回の高さがブロックの範囲内にある
			if (pPosOld->x + fWidth <= pBlock->pos.x - pBlock->fWidth &&
				pPos->x	   + fWidth  > pBlock->pos.x - pBlock->fWidth)
			{//左側
				pPos->x  = pBlock->pos.x - pBlock->fWidth - fWidth;
				pMove->x = 0.0f;
			}
			else if (pPosOld->x - fWidth >= pBlock->pos.x + pBlock->fWidth &&
					 pPos->x    - fWidth  < pBlock->pos.x + pBlock->fWidth)
			{//右側
				pPos->x  = pBlock->pos.x + pBlock->fWidth + fWidth;
				pMove->x = 0.0f;
			}
		}
	}

	return bLand;
}

//============================================================================================================
//ブロックと敵の当たり判定
//============================================================================================================
void CollisionBlockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, BLOCK **pBLK)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		BLOCK *pBlock = &s_aBlock[i];	//ポインタに変身

		if (!pBlock->bUse)
		{//ブロックが使用されていない
			continue;
		}

		/*↓ブロックが使用されている↓*/

		if (pPos->x + fWidth > pBlock->pos.x - pBlock->fWidth &&
			pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth)
		{//敵の幅がブロックの範囲内にある
			if (pPosOld->y + fHeight <= pBlock->pos.y - pBlock->fHeight &&
				pPos->y	   + fHeight  > pBlock->pos.y - pBlock->fHeight)
			{//上側
				pPos->y = pBlock->pos.y - pBlock->fHeight - fHeight;
				pMove->y = 0.0f;

				if (pBlock != NULL)
				{
					//敵の構造体のpBLKにs_aBlock(pBlock)のアドレスを設定
					*pBLK = pBlock;
				}
			}
			else if (pPosOld->y - fHeight >= pBlock->pos.y + pBlock->fHeight &&
					 pPos->y    - fHeight  < pBlock->pos.y + pBlock->fHeight)
			{//下側
				pPos->y = pBlock->pos.y + pBlock->fHeight + fHeight;
				pMove->y = 0.0f;
			}
		}

		if (pPos->y + fHeight > pBlock->pos.y - pBlock->fHeight &&
			pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight)
		{//敵の高さがブロックの範囲内にある
			if (pPosOld->x - fWidth >= pBlock->pos.x + pBlock->fWidth &&
				pPos->x - fWidth	 < pBlock->pos.x + pBlock->fWidth)
			{//敵の左側がブロックの右側にめり込んだ
				pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth;
				pMove->x *= -1.0f;
			}
			else if (pPosOld->x + fWidth <= pBlock->pos.x - pBlock->fWidth &&
				pPos->x + fWidth  > pBlock->pos.x - pBlock->fWidth)
			{//敵の右側がブロックの左側にめり込んだ
				pPos->x = pBlock->pos.x - pBlock->fWidth - fWidth;
				pMove->x *= -1.0f;
			}
		}
	}
}

//============================================================================================================
//ブロックの取得
//============================================================================================================
BLOCK *GetBlock(void)
{
	return &s_aBlock[0];
}