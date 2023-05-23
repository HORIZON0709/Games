//============================================================================================================
//
//君をたずねてにゃん千里[enemy.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "block.h"
#include "goal.h"

#include <assert.h>

//============================================================================================================
//スタティック変数宣言
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture	= NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff	= NULL;		//頂点バッファへのポインタ
static Enemy					s_aEnemy[MAX_ENEMY];	//敵の情報
static float					s_fSaveMove;			//移動量保存用

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void EnemyMove(Enemy *pEnemy);
void EnemyState(Enemy *pEnemy);
void LimitMove(Enemy *pEnemy);
void TexAnimEnemy(Enemy *pEnemy);

//============================================================================================================
//敵の初期化処理
//============================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Cat_04_Copy.png",
								&s_pTexture);

	//敵の情報の初期化
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		s_aEnemy[i].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEnemy[i].posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEnemy[i].move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEnemy[i].state			= ENEMYSTATE_NORMAL;
		s_aEnemy[i].type			= MOVETYPE_GROUND;
		s_aEnemy[i].nCounterState	= 0;
		s_aEnemy[i].nLife			= 0;
		s_aEnemy[i].nCntAnim		= 0;
		s_aEnemy[i].nPtnAnim		= 0;
		s_aEnemy[i].nDirectionMove	= ENEMY_DIRECTION_LEFT;		//左向き
		s_aEnemy[i].fWidth			= 0.0f;
		s_aEnemy[i].fHeight			= 0.0f;
		s_aEnemy[i].bUse			= false;					//使用していない状態にする
		s_aEnemy[i].pBLK			= NULL;
	}

	s_fSaveMove = 0.0f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ENEMY; i++)
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
		SetTex(pVtx, 0.0f, 0.25f, 0.0f, 0.5f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//敵の終了処理
//============================================================================================================
void UninitEnemy(void)
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
//敵の更新処理
//============================================================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//ポインタに変身

		if (!pEnemy->bUse)
		{//使用されていない
			continue;
		}

		/*↓敵が使用されている↓*/

		//敵の状態
		EnemyState(pEnemy);

		//敵の移動制限
		LimitMove(pEnemy);

		pEnemy->pBLK = NULL;	//NULLを入れる

		//ブロックとの当たり判定
		CollisionBlockEnemy(&pEnemy->pos,		//位置
							&pEnemy->posOld,	//前回の位置
							&pEnemy->move,		//移動量
							 pEnemy->fWidth,	//幅
							 pEnemy->fHeight,	//高さ
							&pEnemy->pBLK);		//乗っているブロックの情報

		//敵の動き方
		EnemyMove(pEnemy);
		
		//移動方向の確認
		if (pEnemy->move.x >= 0.0f)
		{//右に進んでるとき
			pEnemy->nDirectionMove = ENEMY_DIRECTION_RIGHT;
		}
		else
		{//左に進んでるとき
			pEnemy->nDirectionMove = ENEMY_DIRECTION_LEFT;
		}

		//敵のテクスチャアニメーション
		TexAnimEnemy(pEnemy);

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//ポインタを4つ分進める

		//頂点座標の設定
		float fLeft		= (pEnemy->pos.x - pEnemy->fWidth);		//左
		float fRight	= (pEnemy->pos.x + pEnemy->fWidth);		//右
		float fUp		= (pEnemy->pos.y - pEnemy->fHeight);	//上
		float fDown		= (pEnemy->pos.y + pEnemy->fHeight);	//下

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//テクスチャ座標の設定
		SetTex(pVtx,
				(1.0f / PL_PATTERN_X) *	 pEnemy->nPtnAnim + 0.0005f,
				(1.0f / PL_PATTERN_X) * (pEnemy->nPtnAnim + 1),
				(1.0f / PL_PATTERN_Y) *	 pEnemy->nDirectionMove + 0.02f,
				(1.0f / PL_PATTERN_Y) * (pEnemy->nDirectionMove + 1));

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//============================================================================================================
//敵の描画処理
//============================================================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//ポインタに変身

		if (pEnemy->bUse)
		{//敵が使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexture);

			//敵の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
									i * 4,					//描画する最初の頂点インデックス
									2);						//描画するプリミティブ数
		}
	}
}

//============================================================================================================
//敵の設定処理
//============================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, MOVETYPE type)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//ポインタに変身

		if (!pEnemy->bUse)
		{//敵が使用されていない
			pEnemy->pos				= pos;
			pEnemy->posOld			= pos;
			pEnemy->move			= move;
			pEnemy->state			= ENEMYSTATE_NORMAL;
			pEnemy->type			= type;
			pEnemy->nCounterState	= COUNTER_ENEMY;
			pEnemy->nLife			= ENEMY_LIFE;
			pEnemy->fWidth			= (ENEMY_WIDTH / 2);
			pEnemy->fHeight			= (ENEMY_HEIGHT / 2);
			pEnemy->bUse			= true;			//使用している状態にする

			s_fSaveMove = move.x;	//移動量を保存する

			//頂点座標の設定
			float fLeft		= (pEnemy->pos.x - pEnemy->fWidth);		//左
			float fRight	= (pEnemy->pos.x + pEnemy->fWidth);		//右
			float fUp		= (pEnemy->pos.y - pEnemy->fHeight);	//上
			float fDown		= (pEnemy->pos.y + pEnemy->fHeight);	//下

			pVtx[0].pos = D3DXVECTOR3(fLeft, fUp, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRight, fUp, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fLeft, fDown, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
			break;
		}

		pVtx += 4;		//ポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//敵の動き方
//============================================================================================================
void EnemyMove(Enemy *pEnemy)
{
	Player *pPlayer = GetPlayer();		//プレイヤー情報

	switch (pEnemy->type)
	{
	case MOVETYPE_GROUND:	//地上
	
		if (!GetPosGoal())
		{//ゴールが指定の位置にいない
			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//プレイヤーの右端が画面中央 & 動いている
				//プレイヤーの移動量を(ちょっといじくって)入れる
				pEnemy->pos.x += -(pPlayer->move.x * 0.7f);
			}
		}
		break;

	case MOVETYPE_ONBLOCK:	//ブロックの上

		if (!GetPosGoal())
		{//ゴールが指定の位置にいない
			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//プレイヤーの右端が画面中央 & 動いている
				//プレイヤーの移動量を(ちょっといじくって)入れる
				pEnemy->pos.x -= (pPlayer->move.x * 0.7f);
			}
		}

		if (pEnemy->pBLK != NULL)
		{
			if (pEnemy->pos.x - pEnemy->fWidth < pEnemy->pBLK->pos.x - pEnemy->pBLK->fWidth)
			{//左から落ちそうになったら
				//位置を戻す
				pEnemy->pos.x = pEnemy->pBLK->pos.x - pEnemy->pBLK->fWidth + pEnemy->fWidth;

				//向きを反転
				pEnemy->nDirectionMove = (pEnemy->nDirectionMove + 1) % 2;

				//移動方向を反転
				pEnemy->move.x *= -1.0f;
			}
			else if (pEnemy->pos.x + pEnemy->fWidth > pEnemy->pBLK->pos.x + pEnemy->pBLK->fWidth)
			{//右から落ちそうになったら
				//位置を戻す
				pEnemy->pos.x = pEnemy->pBLK->pos.x + pEnemy->pBLK->fWidth - pEnemy->fWidth;

				//向きを反転
				pEnemy->nDirectionMove = (pEnemy->nDirectionMove + 1) % 2;

				//移動方向を反転
				pEnemy->move.x *= -1.0f;
			}
		}
		break;

	default:
		assert(false);
		break;
	}

	//重力を加算する
	pEnemy->move.y += ENEMY_GRAVITY;

	//前回の位置を保存
	pEnemy->posOld = pEnemy->pos;

	//位置を更新
	pEnemy->pos += pEnemy->move;
}

//============================================================================================================
//敵の状態
//============================================================================================================
void EnemyState(Enemy *pEnemy)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:		//通常状態
		break;

	case ENEMYSTATE_DAMAGE:		//ダメージ状態

		pEnemy->nCounterState--;	//カウントダウン

		if (pEnemy->nCounterState <= 0)
		{//ダメージ状態の継続が終わったら
			pEnemy->state = ENEMYSTATE_NORMAL;		//通常状態にする

			//頂点カラーの更新
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//敵の移動制限
//============================================================================================================
void LimitMove(Enemy *pEnemy)
{
	//移動制限(下)
	if (pEnemy->pos.y + pEnemy->fHeight > SCREEN_HEIGHT)
	{//プレイヤーが下端を超えたら
		pEnemy->pos.y = SCREEN_HEIGHT - pEnemy->fHeight;
		pEnemy->move.y = 0.0f;
	}

}

//============================================================================================================
//敵のテクスチャアニメーション
//============================================================================================================
void TexAnimEnemy(Enemy *pEnemy)
{
	pEnemy->nCntAnim++;		//カウントアップ

	if (pEnemy->nCntAnim % ENEMY_ANIM_SPEED == 0)
	{//一定時間経過したら
		pEnemy->nPtnAnim = (pEnemy->nPtnAnim + 1) % ENEMY_PATTERN_X;	//パターン番号を更新する
	}
}

//============================================================================================================
//敵とプレイヤーの当たり判定
//============================================================================================================
void CollisionEnemyPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//ポインタに変身

		if (!pEnemy->bUse)
		{//使用されていない
			continue;
		}

		/*↓敵が使用されている↓*/

		if (pPos->x - (fWidth / 10) < pEnemy->pos.x + pEnemy->fWidth &&
			pPos->x + (fWidth / 10) > pEnemy->pos.x - pEnemy->fWidth)
		{//プレイヤーの幅が敵の範囲内のとき
			if (pPosOld->y <= pEnemy->pos.y - pEnemy->fHeight &&
				pPos->y		> pEnemy->pos.y - pEnemy->fHeight)
			{//プレイヤーが上から踏んづけたら
				pPos->y = pEnemy->pos.y - pEnemy->fHeight;

				pMove->y *= -0.5f;

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_TRAMP);

				//敵のヒット処理
				HitEnemy(i, DAMAGE_FROM_PL);
				continue;
			}

			if (pPos->x - fWidth  < pEnemy->pos.x + pEnemy->fWidth &&
				pPos->x + fWidth  > pEnemy->pos.x - pEnemy->fWidth &&
				pPos->y - fHeight < pEnemy->pos.y + pEnemy->fHeight &&
				pPos->y			  > pEnemy->pos.y - pEnemy->fHeight)
			{//プレイヤーが敵に触れたら
				if (GetPlayerState() == PLAYERSTATE_NORMAL)
				{//通常状態のとき
					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					//プレイヤーのヒット処理
					HitPlayer(10);
				}
			}
		}
	}
}

//============================================================================================================
//敵のヒット処理
//============================================================================================================
void HitEnemy(int nCnt, int nDamage)
{
	Enemy *pEnemy = &s_aEnemy[nCnt];	//ポインタに変身

	if (pEnemy->state == ENEMYSTATE_NORMAL)
	{//通常状態の時
		pEnemy->nLife -= nDamage;	//ライフを減らす

		if (pEnemy->nLife <= 0)
		{//敵の体力が０以下
			//スコアの加算
			AddScore(ENEMY_POINT);

			//使用されていない状態に
			pEnemy->bUse = false;
		}
		else
		{
			//ダメージ状態に
			pEnemy->state = ENEMYSTATE_DAMAGE;

			//カウンターの準備
			pEnemy->nCounterState = COUNTER_ENEMY;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCnt * 4);	//該当の位置まで進める

			//頂点カラーの設定(赤)
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			//頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}
	}
}

//============================================================================================================
//敵の取得
//============================================================================================================
Enemy *GetEnemy(void)
{
	return &s_aEnemy[0];
}