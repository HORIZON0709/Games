//============================================================================================================
//
//君をたずねてにゃん千里[goal.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "goal.h"
#include "game.h"
#include "player.h"
#include "fade.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static GOAL						s_goal;					//ゴールの情報
static bool						bGoal;					//ゴールしたかどうか
static bool						bPosGoal;				//ゴールがどこか

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void TexAnimGoal(void);

//============================================================================================================
//ゴールの初期化処理
//============================================================================================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Goal_Copy.png",
								&s_pTexture);

	//情報の初期化
	s_goal.pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_goal.move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_goal.fWidth	= 0.0f;
	s_goal.fHeight	= 0.0f;
	s_goal.nCntAnim = 0;
	s_goal.nPtnAnim = 0;
	s_goal.bUse		= false;	//使用されていない状態にする

	bGoal = false;		//ゴールしていない状態にする
	bPosGoal = false;	//指定の位置にいない状態にする

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
	SetTex(pVtx, 0.0f, 0.25f, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//ゴールの終了処理
//============================================================================================================
void UninitGoal(void)
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
//ゴールの更新処理
//============================================================================================================
void UpdateGoal(void)
{
	Player *pPlayer = GetPlayer();

	if (!bPosGoal)
	{//指定の位置にいないとき
		//プレイヤーの移動量を(ちょっといじくって)入れる
		s_goal.move.x = -(pPlayer->move.x * 0.7f);

		if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
		{//プレイヤーの右端が画面中央 & 動いている
			//位置を更新
			s_goal.pos.x += s_goal.move.x;
		}

		if (s_goal.pos.x <= (SCREEN_WIDTH - s_goal.fWidth))
		{//指定の位置に着いたら
			bPosGoal = true;	//指定の位置に着いた
		}
	}

	//ゴールのテクスチャアニメーション
	TexAnimGoal();

	if (pPlayer->pos.x > s_goal.pos.x - s_goal.fWidth &&
		pPlayer->pos.x < s_goal.pos.x + s_goal.fWidth &&
		pPlayer->pos.y - pPlayer->fHeight < s_goal.pos.y + s_goal.fHeight &&
		pPlayer->pos.y					  > s_goal.pos.y - s_goal.fHeight)
	{//プレイヤーがゴールに触れたら
		if (GetFade() == FADE_NONE && GetGamaState() != GAMESTATE_END)
		{//フェードしていないとき & 終了状態ではないとき
			bGoal = true;	//ゴールした

			//終了状態に設定
			SetGameState(GAMESTATE_END);
		}
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	float fLeft		= (s_goal.pos.x - s_goal.fWidth);	//左
	float fRight	= (s_goal.pos.x + s_goal.fWidth);	//右
	float fUp		= (s_goal.pos.y - s_goal.fHeight);	//上
	float fDown		= (s_goal.pos.y + s_goal.fHeight);	//下

	pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

	//テクスチャ座標の設定
	SetTex(pVtx,
			(1.0f / GOAL_MAX_PATTERN) *	 s_goal.nPtnAnim,
			(1.0f / GOAL_MAX_PATTERN) * (s_goal.nPtnAnim + 1),
			0.0f,1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//ゴールの描画処理
//============================================================================================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	if (s_goal.bUse)
	{//ゴールが使用されている
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		//ゴールの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								0,						//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}
}

//============================================================================================================
//ゴールの設定処理
//============================================================================================================
void SetGoal(D3DXVECTOR3 pos)
{
	if (!s_goal.bUse)
	{//ゴールが使用されていないとき
		s_goal.pos		= pos;
		s_goal.fWidth	= (GOAL_WIDTH * 0.5f);
		s_goal.fHeight	= (GOAL_HEIGHT * 0.5f);
		s_goal.bUse		= true;			//使用している状態にする

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		float fLeft		= (s_goal.pos.x - s_goal.fWidth);	//左
		float fRight	= (s_goal.pos.x + s_goal.fWidth);	//右
		float fUp		= (s_goal.pos.y - s_goal.fHeight);	//上
		float fDown		= (s_goal.pos.y + s_goal.fHeight);	//下

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//============================================================================================================
//ゴールのテクスチャアニメーション
//============================================================================================================
void TexAnimGoal(void)
{
	s_goal.nCntAnim++;		//カウントアップ

	if (s_goal.nCntAnim % GOAL_ANIM_SPEED == 0)
	{//一定時間経過したら
		s_goal.nPtnAnim = (s_goal.nPtnAnim + 1) % GOAL_MAX_PATTERN;	//パターン番号を更新する
	}
}

//============================================================================================================
//ゴールしたかどうかを取得
//============================================================================================================
bool GetGoal(void)
{
	return bGoal;
}

//============================================================================================================
//ゴールが指定の位置にいるかどうかを取得
//============================================================================================================
bool GetPosGoal(void)
{
	return bPosGoal;
}