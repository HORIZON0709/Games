//============================================================================================================
//
//君をたずねてにゃん千里[item.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "item.h"
#include "player.h"
#include "score.h"
#include "goal.h"
#include "sound.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static Item						s_aItem[MAX_ITEM];		//アイテムの情報

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void ItemMotion(void);

//============================================================================================================
//アイテムの初期化処理
//============================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスのポインタ
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/ハート_02.png",
								&s_pTexture);

	for (int i = 0; i < MAX_ITEM; i++)
	{//情報の初期化
		s_aItem[i].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置を初期化する
		s_aItem[i].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量を初期化する
		s_aItem[i].fWidth	= 0;								//幅を初期化する
		s_aItem[i].fHeight	= 0;								//高さを初期化する
		s_aItem[i].nCounter = 0;								//カウンターを初期化する
		s_aItem[i].type		= ITEM_TYPE_COIN;					//コインにする
		s_aItem[i].bUse		= false;							//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
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
//アイテムの終了処理
//============================================================================================================
void UninitItem(void)
{
	//サウンドの終了処理
	StopSound();

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
//アイテムの更新処理
//============================================================================================================
void UpdateItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];	//ポインタに変身		

		if (!pItem->bUse)
		{//使用されていない
			continue;
		}

		/*↓アイテムが使用されている↓*/

		Player *pPlayer = GetPlayer();		//プレイヤー情報

		if (!GetPosGoal())
		{//ゴールが指定の位置にいない
			//プレイヤーの移動量を(ちょっといじくって)入れる
			pItem->move.x = -(pPlayer->move.x * 0.7f);

			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//プレイヤーの右端が画面中央 & 動いている
				//位置を更新
				pItem->pos.x += pItem->move.x;
			}
		}

		//アイテムのモーション
		ItemMotion();

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//ポインタを4つ分進める

		//頂点座標の更新
		float fLeft		= (pItem->pos.x - pItem->fWidth);	//左
		float fRight	= (pItem->pos.x + pItem->fWidth);	//右
		float fUp		= (pItem->pos.y - pItem->fHeight);	//上
		float fDown		= (pItem->pos.y + pItem->fHeight);	//下

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//============================================================================================================
//アイテムの描画処理
//============================================================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (s_aItem[i].bUse)
		{//アイテムが使用されている
			 //テクスチャの設定
			pDevice->SetTexture(0, s_pTexture);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
									i * 4,					//描画を開始する頂点インデックス
									2);						//描画するプリミティブ数
		}
	}
}

//============================================================================================================
//ブロックの設定処理
//============================================================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEM_TYPE itemType)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];	//ポインタに変身

		if (!pItem->bUse)
		{//ブロックが使用されていない
			pItem->pos		= pos;
			pItem->move		= move;
			pItem->fWidth	= (ITEM_WIDTH / 2);
			pItem->fHeight	= (ITEM_HEIGHT / 2);
			pItem->type		= itemType;
			pItem->bUse		= true;					//使用している状態にする

			//頂点座標の設定
			pVtx[0].pos.x = pos.x - pItem->fWidth;
			pVtx[0].pos.y = pos.y - pItem->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pos.x + pItem->fWidth;
			pVtx[1].pos.y = pos.y - pItem->fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pos.x - pItem->fWidth;
			pVtx[2].pos.y = pos.y + pItem->fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pos.x + pItem->fWidth;
			pVtx[3].pos.y = pos.y + pItem->fHeight;
			pVtx[3].pos.z = 0.0f;

			//テクスチャ座標の設定
			float fTexU = pItem->fWidth	 / (ITEM_WIDTH / 2);
			float fTexV = pItem->fHeight / (ITEM_HEIGHT / 2);

			SetTex(pVtx, 0.0f, fTexU, 0.0f, fTexV);
			break;
		}

		pVtx += 4;		//ポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//アイテムのモーション
//============================================================================================================
void ItemMotion(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];	//ポインタに変身

		if (!pItem->bUse)
		{//使用されていない
			continue;
		}

		/*↓アイテムが使用されている↓*/

		//アイテムを上下させる
		pItem->pos.y += sinf((D3DX_PI * 2) * (pItem->nCounter * 0.001f)) * 0.07f;

		//カウントアップ
		pItem->nCounter++;

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//アイテムの当たり判定
//============================================================================================================
void CollisionItem(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];

		if (!pItem->bUse)
		{//アイテムが使用されていない
			continue;
		}

		/*↓アイテムが使用されている↓*/

		if (pPos->x + (fWidth / 5)  > pItem->pos.x - pItem->fWidth &&
			pPos->x - (fWidth / 5)  < pItem->pos.x + pItem->fWidth &&
			pPos->y - fHeight		< pItem->pos.y + pItem->fHeight &&
			pPos->y					> pItem->pos.y - pItem->fHeight)
		{//対象がアイテムの範囲内にいる
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_ITEMGET);

			//スコアの加算
			AddScore(100);

			pItem->bUse = false;	//使用していない状態にする
		}
	}
}

//============================================================================================================
//アイテムの取得
//============================================================================================================
Item *GetItem(void)
{
	return &s_aItem[0];
}