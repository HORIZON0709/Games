//============================================================================================================
//
//君をたずねてにゃん千里[title.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTextureBG		 = NULL;	//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureLogo		 = NULL;	//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTexturePressEnter = NULL;	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBG		 = NULL;	//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPressEnter = NULL;	//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffLogo		 = NULL;	//頂点バッファへのポインタ
static PressEnter				s_PressEnter;					//PressEnterの情報
static FADEENTER				s_FadeEnter;					//PressEnterの点滅状態
static D3DXVECTOR3				s_posTitleLogo;					//タイトルロゴの位置
static TITLELOGO				s_TitleLogo;					//タイトルロゴの状態

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void TitleLogoMotion(void);

//============================================================================================================
//タイトルの初期化処理
//============================================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/ACG_Title.png",
								&s_pTextureBG);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/君をたずねてにゃん千里_Copy.png",
								&s_pTextureLogo);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/PressEnter_Copy.png",
								&s_pTexturePressEnter);

	//情報の初期化
	s_PressEnter.pos = D3DXVECTOR3(PRESSENTER_POS_X, PRESSENTER_POS_Y, 0.0f);
	s_PressEnter.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	s_FadeEnter		 = FADEENTER_IN;
	s_posTitleLogo	 = D3DXVECTOR3((SCREEN_WIDTH / 2), -170.0f, 0.0f);
	s_TitleLogo		 = TITLELOGO_POP;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffBG,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffLogo,
								NULL);
	
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPressEnter,
								NULL);

	/*↓背景画像↓*/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

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
	s_pVtxBuffBG->Unlock();

	/*↓タイトルロゴ↓*/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
	pVtx[0].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
	pVtx[1].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
	pVtx[2].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
	pVtx[3].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuffLogo->Unlock();

	/*↓Press Enterの画像↓*/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = s_PressEnter.pos.x - PRESSENTER_WIDTH;
	pVtx[0].pos.y = s_PressEnter.pos.y - PRESSENTER_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_PressEnter.pos.x + PRESSENTER_WIDTH;
	pVtx[1].pos.y = s_PressEnter.pos.y - PRESSENTER_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_PressEnter.pos.x - PRESSENTER_WIDTH;
	pVtx[2].pos.y = s_PressEnter.pos.y + PRESSENTER_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_PressEnter.pos.x + PRESSENTER_WIDTH;
	pVtx[3].pos.y = s_PressEnter.pos.y + PRESSENTER_HEIGHT;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuffPressEnter->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_TITLE);
}

//============================================================================================================
//タイトルの終了処理
//============================================================================================================
void UninitTitle(void)
{
	//サウンドの停止
	StopSound();

	/*↓テクスチャの破棄↓*/

	if (s_pTextureBG != NULL)
	{//背景画像の方
		s_pTextureBG->Release();
		s_pTextureBG = NULL;
	}

	if (s_pTextureLogo != NULL)
	{//タイトルロゴの方
		s_pTextureLogo->Release();
		s_pTextureLogo = NULL;
	}

	if (s_pTextureBG != NULL)
	{//Press Enterの方
		s_pTexturePressEnter->Release();
		s_pTexturePressEnter = NULL;
	}

	/*↓頂点バッファの破棄↓*/

	if (s_pVtxBuffBG != NULL)
	{//背景画像の方
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	if (s_pVtxBuffLogo != NULL)
	{//タイトルロゴの方
		s_pVtxBuffLogo->Release();
		s_pVtxBuffLogo = NULL;
	}

	if (s_pVtxBuffPressEnter != NULL)
	{//Press Enterの方
		s_pVtxBuffPressEnter->Release();
		s_pVtxBuffPressEnter = NULL;
	}
}

//============================================================================================================
//タイトルの更新処理
//============================================================================================================
void UpdateTitle(void)
{
	//タイトルロゴの動き
	TitleLogoMotion();

	/*↓Press Enterの点滅↓*/

	if (GetFade() == FADE_NONE || GetFade() == FADE_IN)
	{//フェードしていないorフェードイン状態のとき
		if (s_FadeEnter == FADEENTER_IN)
		{//透明にするとき
			s_PressEnter.col.a -= PRESSENTER_SPEED;	//透明にしていく

			if (s_PressEnter.col.a <= 0.0f)
			{//0.0fを過ぎたら
				s_PressEnter.col.a = 0.0f;			//0.0fに戻す
				s_FadeEnter		   = FADEENTER_OUT;	//不透明にする状態に
			}
		}
		else if (s_FadeEnter == FADEENTER_OUT)
		{//不透明にするとき
			s_PressEnter.col.a += PRESSENTER_SPEED;	//不透明にしていく

			if (s_PressEnter.col.a >= 1.0f)
			{//1.0fを過ぎたら
				s_PressEnter.col.a = 1.0f;			//1.0fに戻す
				s_FadeEnter		   = FADEENTER_IN;	//透明にする状態に
			}
		}
	}
	else if (GetFade() == FADE_OUT)
	{//フェードアウト状態のとき
		if (s_FadeEnter == FADEENTER_IN)
		{//透明にするとき
			s_PressEnter.col.a -= PRESSENTER_HIGHSPEED;	//透明にしていく

			if (s_PressEnter.col.a <= 0.0f)
			{//0.0fを過ぎたら
				s_PressEnter.col.a = 0.0f;			//0.0fに戻す
				s_FadeEnter		   = FADEENTER_OUT;	//不透明にする状態に
			}
		}
		else if (s_FadeEnter == FADEENTER_OUT)
		{//不透明にするとき
			s_PressEnter.col.a += PRESSENTER_HIGHSPEED;	//不透明にしていく

			if (s_PressEnter.col.a >= 1.0f)
			{//1.0fを過ぎたら
				s_PressEnter.col.a = 1.0f;			//1.0fに戻す
				s_FadeEnter		   = FADEENTER_IN;	//透明にする状態に
			}
		}
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);

	//頂点バッファをアンロックする
	s_pVtxBuffPressEnter->Unlock();
}

//============================================================================================================
//タイトルの描画処理
//============================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	/*↓背景画像の方↓*/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureBG);

	//タイトルの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数

	/*↓タイトルロゴの方↓*/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureLogo);

	//タイトルの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数

	/*↓Press Enterの方↓*/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexturePressEnter);

	//タイトルの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数
}

//============================================================================================================
//タイトルロゴの動き
//============================================================================================================
void TitleLogoMotion(void)
{
	if (GetFade() == FADE_NONE)
	{//フェードしていないとき

		switch (s_TitleLogo)
		{
		case TITLELOGO_NORMAL:		//通常状態

			if (GetKeyboardTrigger(DIK_RETURN))
			{//Enterキーが押されたら
				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//チュートリアル画面に移行
				SetFade(MODE_TUTORIAL);
			}
			break;

		case TITLELOGO_POP:		//出現状態

								//タイトルロゴを徐々に降ろす
			s_posTitleLogo.y += LOGO_SPEED;

			if (s_posTitleLogo.y >= (SCREEN_HEIGHT / 5 * 2) || GetKeyboardTrigger(DIK_RETURN))
			{//定位置を越したら or Enterキーが押されたら
				//定位置にする
				s_posTitleLogo.y = (SCREEN_HEIGHT / 5 * 2);

				//通常状態にする
				s_TitleLogo = TITLELOGO_NORMAL;
			}

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の更新
			pVtx[0].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
			pVtx[0].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
			pVtx[1].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
			pVtx[2].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
			pVtx[3].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			//頂点バッファをアンロックする
			s_pVtxBuffLogo->Unlock();
			break;
		}
	}
}