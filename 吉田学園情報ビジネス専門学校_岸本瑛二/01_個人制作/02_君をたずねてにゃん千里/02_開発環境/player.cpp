//============================================================================================================
//
//君をたずねてにゃん千里[player.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "player.h"
#include "game.h"
#include "input.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "goal.h"
#include "fade.h"
#include "sound.h"

#include <assert.h>

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexturePlayer = NULL;	//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureStock	 = NULL;	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStock	 = NULL;	//頂点バッファへのポインタ
static Player					s_player;					//プレイヤーの情報
static STOCK					s_aStock[PLAYER_STOCK];		//残機の情報

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void PlayerState(void);
void PlayerMove(void);
void LimitMove(void);
void TexAnimPlayer(void);
void SubStock(void);
STOCK *GetStock(void);

//============================================================================================================
//プレイヤーの初期化処理
//============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスのポインタ
	VERTEX_2D *pVtx;							//頂点情報へのポインタ
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Cat_02_Copy.png",
								&s_pTexturePlayer);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Cat_Head_Copy.png",
								&s_pTextureStock);

	//プレイヤー情報の初期化
	s_player.pos			= D3DXVECTOR3(PLAYERPOS_X, PLAYERPOS_Y, 0.0f);	//位置(現在)
	s_player.posOld		    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//位置(前回)
	s_player.rot		    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//向き
	s_player.move		    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//移動量
	s_player.col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			//色
	s_player.state		    = PLAYERSTATE_NORMAL;							//状態
	s_player.nCounterState  = 0;											//カウンター
	s_player.nLife		    = PLAYER_LIFE;									//体力
	s_player.nStock			= PLAYER_STOCK;									//残機
	s_player.nCntAnim	    = 0;											//カウンター
	s_player.nPtnAnim	    = 0;											//パターン番号
	s_player.nDirectionMove = PL_DIRECTION_RIGHT;							//右向きにする
	s_player.fWidth			= (PLAYER_WIDTH * 0.5f);						//幅
	s_player.fHeight		= PLAYER_HEIGHT;								//高さ
	s_player.bJump			= false;										//ジャンプしていない状態にする
	s_player.bDisp			= true;											//表示する状態にする

	for (int i = 0; i < PLAYER_STOCK; i++)
	{//残機の情報の初期化
		STOCK *pStock = &s_aStock[i];		//ポインタに変身

		pStock->pos.x = STOCK_POS_X + (i * STOCK_SIZE) + (i *  +STOCK_INTERVAL);
		pStock->pos.y = STOCK_POS_Y;
		pStock->pos.z = 0.0f;						//位置

		pStock->fWidth	= (STOCK_SIZE * 0.5f);		//幅
		pStock->fHeight = (STOCK_SIZE * 0.5f);		//高さ
		pStock->bUse	= true;						//使用されている状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPlayer,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PLAYER_STOCK,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffStock,
								NULL);

	/*↓プレイヤー↓*/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	{//頂点座標の設定
		float fLeft	 = (s_player.pos.x - s_player.fWidth);		//左
		float fRight = (s_player.pos.x + s_player.fWidth);		//右
		float fUp	 = (s_player.pos.y - s_player.fHeight);		//上
		float fDown  =  s_player.pos.y;							//下

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
	}

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
	s_pVtxBuffPlayer->Unlock();

	/*↓残機↓*/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < PLAYER_STOCK; i++)
	{
		STOCK *pStock = &s_aStock[i];		//ポインタに変身

		//頂点座標の設定
		float fLeft	 = (pStock->pos.x - pStock->fWidth);	//左
		float fRight = (pStock->pos.x + pStock->fWidth);	//右
		float fUp	 = (pStock->pos.y - pStock->fHeight);	//上
		float fDown  = (pStock->pos.y + pStock->fHeight);	//下

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

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

		pVtx += 4;		//4つ分進める
	}
	
	//頂点バッファをアンロックする
	s_pVtxBuffStock->Unlock();
}

//============================================================================================================
//プレイヤーの終了処理
//============================================================================================================
void UninitPlayer(void)
{
	//サウンドの終了処理
	StopSound();

	/*↓テクスチャの破棄↓*/

	if (s_pTexturePlayer != NULL)
	{//プレイヤー
		s_pTexturePlayer->Release();
		s_pTexturePlayer = NULL;
	}

	if (s_pTextureStock != NULL)
	{//残機
		s_pTextureStock->Release();
		s_pTextureStock = NULL;
	}

	/*↓頂点バッファの破棄↓*/

	if (s_pVtxBuffPlayer != NULL)
	{//プレイヤー
		s_pVtxBuffPlayer->Release();
		s_pVtxBuffPlayer = NULL;
	}

	if (s_pVtxBuffStock != NULL)
	{//残機
		s_pVtxBuffStock->Release();
		s_pVtxBuffStock = NULL;
	}
}

//============================================================================================================
//プレイヤーの更新処理
//============================================================================================================
void UpdatePlayer(void)
{
	//プレイヤーの状態
	PlayerState();

	//プレイヤーの移動・ジャンプ
	PlayerMove();

	//プレイヤーのテクスチャアニメーション処理
	TexAnimPlayer();

	//ブロックとの当たり判定(ジャンプの可否も判断)
	s_player.bJump = !CollisionBlockPlayer(&s_player.pos,		//現在の位置
										   &s_player.posOld,	//前回の位置
										   &s_player.move,		//移動量
											s_player.fWidth,	//幅
											s_player.fHeight);	//高さ

	//プレイヤーの移動制限
	LimitMove();

	//敵との当たり判定
	CollisionEnemyPlayer(&s_player.pos, &s_player.posOld, &s_player.move, s_player.fWidth, s_player.fHeight);

	//アイテムとの当たり判定
	CollisionItem(&s_player.pos, s_player.fWidth, s_player.fHeight);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	float fLeft  = (s_player.pos.x - s_player.fWidth);		//左
	float fRight = (s_player.pos.x + s_player.fWidth);		//右
	float fUp	 = (s_player.pos.y - s_player.fHeight);		//上
	float fDown  =  s_player.pos.y;							//下

	pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

	//頂点バッファをアンロックする
	s_pVtxBuffPlayer->Unlock();
}

//============================================================================================================
//プレイヤーの描画処理
//============================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	/*↓プレイヤー↓*/

	if (s_player.bDisp)
	{//プレイヤーが表示されている
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexturePlayer);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								0,						//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}

	/*↓残機↓*/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffStock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PLAYER_STOCK; i++)
	{
		STOCK *pStock = &s_aStock[i];	//ポインタに変身

		if (pStock->bUse)
		{//使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTextureStock);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
									i * 4,					//描画する最初の頂点インデックス
									2);						//描画するプリミティブ数
		}
	}
}

//============================================================================================================
//プレイヤーの移動制限
//============================================================================================================
void LimitMove(void)
{
	//移動制限(下)
	if (s_player.pos.y > SCREEN_HEIGHT)
	{//プレイヤーが下端を超えたら
		s_player.pos.y = SCREEN_HEIGHT;
		s_player.move.y = 0.0f;

		//ジャンプしていない状態にする
		s_player.bJump = false;
	}

	//移動制限(左右)
	if (s_player.pos.x - s_player.fWidth < 0)
	{//プレイヤーが左端を超えたら
		s_player.pos.x = s_player.fWidth;
	}
	else if (s_player.pos.x + s_player.fWidth > SCREEN_WIDTH)
	{//プレイヤーが右端を超えたら
		s_player.pos.x = (SCREEN_WIDTH - s_player.fWidth);
	}
	else if (s_player.pos.x + s_player.fWidth > (SCREEN_WIDTH / 2))
	{//プレイヤーが画面中央を超えたら
		if (!GetPosGoal())
		{//ゴールが指定の位置にいない
			s_player.pos.x = ((SCREEN_WIDTH / 2) - s_player.fWidth);
		}
	}
}

//============================================================================================================
//プレイヤーの状態
//============================================================================================================
void PlayerState(void)
{
	switch (s_player.state)
	{
	case PLAYERSTATE_NORMAL:	//通常状態
		break;

	case PLAYERSTATE_DAMAGE:	//ダメージ状態

		s_player.nCounterState--;	//カウントダウン

		if (s_player.nCounterState <= 0)
		{//カウンターが0になったら
			s_player.nCounterState = 120;				//カウンター準備
			s_player.state = PLAYERSTATE_INVINCIBLE;	//無敵状態にする

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラーの設定(通常(白))
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//頂点バッファをアンロックする
			s_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_DEATH:		//死亡状態

		s_player.nCounterState--;	//カウントダウン

		if (GetFade() == FADE_NONE)
		{//フェードしていないとき
			if (s_player.nCounterState <= 0 && GetGamaState() != GAMESTATE_END)
			{//カウンターが0になった & 終了状態ではないとき
				//終了状態に設定
				SetGameState(GAMESTATE_END);
				return;
			}
		}
		break;

	case PLAYERSTATE_INVINCIBLE:	//無敵状態

		s_player.nCounterState--;	//カウントダウン

		//点滅させる(α値をsinカーブで変動)
		s_player.col.a = sinf((s_player.nCounterState * BLINKING_SPEED) * (D3DX_PI * 2.0f)) * BLINKING_SIZE + BLINKING_DEGREE;

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定(通常(白))
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);

		//頂点バッファをアンロックする
		s_pVtxBuffPlayer->Unlock();

		if (s_player.nCounterState <= 0)
		{//カウンターが0になったら
			s_player.state = PLAYERSTATE_NORMAL;	//通常状態にする

			//頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラーの設定(通常(白))
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//頂点バッファをアンロックする
			s_pVtxBuffPlayer->Unlock();
		}
		break;
	}
}

//============================================================================================================
//プレイヤーの移動・ジャンプ
//============================================================================================================
void PlayerMove(void)
{
	/*↓プレイヤーの移動↓*/

	if (GetKeyboardPress(DIK_A))
	{//Aキーが押されている(左)
		//左向きにする
		s_player.nDirectionMove = PL_DIRECTION_LEFT;

		//位置を更新
		s_player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_SPEED;
	}
	else if (GetKeyboardPress(DIK_D))
	{//Dキーが押されている(右)
		//右向きにする
		s_player.nDirectionMove = PL_DIRECTION_RIGHT;

		//位置を更新
		s_player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;
	}
	else
	{//何も押していないとき
		//静止させる
		if (s_player.nDirectionMove == PL_DIRECTION_RIGHT)
		{//右向いてるとき
			//反対方向に力を加える
			s_player.move.x -= sinf(D3DX_PI * 0.5f) * 0.01f;

			if (s_player.move.x < 0.0f)
			{//0を過ぎたら
				s_player.move.x = 0.0f;
			}
		}
		else if (s_player.nDirectionMove == PL_DIRECTION_LEFT)
		{//左向いてるとき
			//反対方向に力を加える
 			s_player.move.x += sinf(D3DX_PI * 0.5f) * 0.01f;

			if (s_player.move.x > 0.0f)
			{//0を過ぎたら
				s_player.move.x = 0.0f;
			}
		}
	}

	//移動量を更新(減衰させる)
	s_player.move.x += (0.0f - s_player.move.x) * SPEED_DOWN;

	/*↓プレイヤーのジャンプ↓*/

	if (!s_player.bJump)
	{//ジャンプしていないとき
		if (GetKeyboardTrigger(DIK_SPACE))
		{//スペースキーが押された(ジャンプ)
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_JUMP);

			//移動量を変更
			s_player.move.y = PLAYER_JUMP;

			//ジャンプしている状態にする
			s_player.bJump = true;
		}
	}

	//重力を加算する
	s_player.move.y += PLAYER_GRAVITY;	

	//前回の位置を保存
	s_player.posOld = s_player.pos;

	//位置を更新
	s_player.pos += s_player.move;
}

//============================================================================================================
//プレイヤーのテクスチャアニメーション
//============================================================================================================
void TexAnimPlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (!s_player.bJump)
	{//ジャンプしていないとき
		if (s_player.nDirectionMove == PL_DIRECTION_RIGHT)
		{//右向きの時
			if (s_player.move.x != SPEED_STOP || 
				(s_player.pos.x + s_player.fWidth >= (SCREEN_WIDTH / 2) && s_player.move.x != SPEED_STOP))
			{//移動している or 移動中のプレイヤーの右端が画面中央にある
				s_player.nCntAnim++;	//カウンターを加算

				if (s_player.nCntAnim % PLAYER_ANIMSPEED == 0)
				{//一定時間経過したら
					s_player.nPtnAnim = (s_player.nPtnAnim + 1) % PL_PATTERN_X;		//パターン番号を更新する

					//テクスチャ座標の設定
					SetTex(pVtx,
							(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
							(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
							(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
							(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
				}
			}
			else
			{//移動していない
				if (s_player.nPtnAnim == 1)
				{//パターン1のとき
					s_player.nPtnAnim = 2;		//パターン番号を設定
				}
				else if (s_player.nPtnAnim == 3)
				{//パターン3のとき
					s_player.nPtnAnim = 0;		//パターン番号を設定
				}

				//テクスチャ座標の設定
				SetTex(pVtx,
						(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
						(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
						(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
						(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
			}
		}
		else if (s_player.nDirectionMove == PL_DIRECTION_LEFT)
		{//左向きの時
			if (s_player.move.x != SPEED_STOP)
			{//移動している
				s_player.nCntAnim++;	//カウンターを加算

				if (s_player.nCntAnim % PLAYER_ANIMSPEED == 0)
				{//一定時間経過したら
					s_player.nPtnAnim = (s_player.nPtnAnim + 1) % PL_PATTERN_X;	//パターン番号を更新する

					//テクスチャ座標の設定
					SetTex(pVtx,
							(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
							(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
							(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
							(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
				}
			}
			else
			{//移動していない
				if (s_player.nPtnAnim == 1)
				{//パターン1のとき
					s_player.nPtnAnim = 2;		//パターン番号を設定
				}
				else if (s_player.nPtnAnim == 3)
				{//パターン3のとき
					s_player.nPtnAnim = 0;		//パターン番号を設定
				}

				//テクスチャ座標の設定
				SetTex(pVtx,
						(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
						(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
						(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
						(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
			}
		}
	}
	else if (s_player.bJump)
	{//ジャンプしているとき
		if (s_player.nPtnAnim == 0 || s_player.nPtnAnim == 2)
		{
			s_player.nPtnAnim = (s_player.nPtnAnim + 1) % PL_PATTERN_X;	//パターン番号を更新する
		}
		
		//テクスチャ座標の設定
		SetTex(pVtx,
				(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
				(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
				(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
				(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPlayer->Unlock();
}

//============================================================================================================
//プレイヤーのヒット処理
//============================================================================================================
void HitPlayer(int nDamage)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (s_player.bDisp && s_player.state == PLAYERSTATE_NORMAL)
	{//プレイヤーが表示されている & 通常状態のとき
		//体力を減らす
		s_player.nLife -= nDamage;

		if (s_player.nLife <= 0)
		{//プレイヤーの体力がないなった
			//サウンドの再生
			//PlaySound(SOUND_LABEL_SE_EXPLOSION);

			s_player.bDisp = false;					//非表示にする
			s_player.state = PLAYERSTATE_DEATH;		//死亡状態にする
			s_player.nCounterState = 10;			//カウンターを準備
		}
		else
		{
			s_player.state = PLAYERSTATE_DAMAGE;	//ダメージ状態にする
			s_player.nCounterState = 15;			//カウンターを準備

			//頂点カラーの設定(赤)
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		}

		//残機の減少
		SubStock();
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPlayer->Unlock();
}

//============================================================================================================
//プレイヤーの当たり判定
//============================================================================================================
void CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight)
{
	Player *pPL = GetPlayer();		//ポインタに変身

	if (pPos->y - fHeight < pPL->pos.y &&
		pPos->y + fHeight > pPL->pos.y - pPL->fHeight)
	{//対象の前回の高さがプレイヤーの範囲内にある
		if (pPosOld->x + fWidth <= pPL->pos.x - pPL->fWidth &&
			pPos->x + fWidth > pPL->pos.x - pPL->fWidth)
		{//右側
			pPL->pos.x = pPos->x + pPL->fWidth + fWidth;
		}
		else if (pPosOld->x - fWidth >= pPL->pos.x + pPL->fWidth &&
				 pPos->x - fWidth < pPL->pos.x + pPL->fWidth)
		{//左側
			pPL->pos.x = pPos->x - pPL->fWidth - fWidth;
		}
	}
}

//============================================================================================================
//残機の減少処理
//============================================================================================================
void SubStock(void)
{
	switch (s_player.nLife)
	{
	case PLAYER_LIFE:	//ノーダメージ
		break;

	case (PLAYER_LIFE - 10):	//ダメージを1回受けた
		s_aStock[2].bUse = false;	//右から残機を減らす
		break;

	case (PLAYER_LIFE - 20):	//ダメージを2回受けた
		s_aStock[1].bUse = false;	//右から残機を減らす
		break;

	case 0:						//ダメージを3回受けた(体力が0)
		s_aStock[0].bUse = false;	//右から残機を減らす
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================================================================
//プレイヤー情報の取得
//============================================================================================================
Player *GetPlayer(void)
{
	return &s_player;
}

//============================================================================================================
//プレイヤーの状態の取得
//============================================================================================================
PLAYERSTATE GetPlayerState(void)
{
	return s_player.state;
}

//============================================================================================================
//残機の情報の取得
//============================================================================================================
STOCK *GetStock(void)
{
	return &s_aStock[0];
}