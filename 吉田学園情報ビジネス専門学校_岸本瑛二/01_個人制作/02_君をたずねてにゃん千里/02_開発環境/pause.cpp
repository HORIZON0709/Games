//============================================================================================================
//
//君をたずねてにゃん千里[pause.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "fade.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9	   s_apTexture[NUM_PAUSE] = { NULL, NULL, NULL, NULL };		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPause		  = NULL;							//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseMenu	  = NULL;							//頂点バッファへのポインタ
static PAUSE_MENU			   s_pauseMenu;												//ポーズメニュー
static PauseMenu			   s_aPauseMenu[PAUSE_MENU_MAX];							//ポーズメニューの情報
static D3DXVECTOR3			   s_posPause;												//ポーズの位置

//============================================================================================================
//ポーズの初期化処理
//============================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ
	int nCntPause;								//カウント

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/ゲームに戻る_Copy.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/やりなおす_Copy.png",
								&s_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/タイトルへ_Copy.png",
								&s_apTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/pause100.png",
								&s_apTexture[3]);

	//情報の初期化
	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		s_aPauseMenu[nCntPause].pos = D3DXVECTOR3(640.0f, 260.0f + (nCntPause * PAUSEMENU_SPACE), 0.0f);
		s_aPauseMenu[nCntPause].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}

	s_pauseMenu = PAUSE_MENU_CONTINUE;					//ポーズメニューを初期化
	s_posPause	= D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//ポーズの位置を初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPause,
								NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPauseMenu,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//ポーズの半透明ポリゴン
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		else if (nCntPause == 1)
		{//ポーズ
			//頂点座標の設定
			pVtx[0].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[0].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[1].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[2].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[3].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[3].pos.z = 0.0f;
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntPause == 0)
		{//ポーズの半透明ポリゴン
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		}
		else if (nCntPause == 1)
		{//ポーズ
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
		}

		//テクスチャ座標の設定
		SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPause->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{//ポーズメニューの方
		//頂点座標の設定
		pVtx[0].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[0].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[1].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[2].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[3].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
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
		SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//ポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPauseMenu->Unlock();
}

//============================================================================================================
//ポーズの終了処理
//============================================================================================================
void UninitPause(void)
{
	int nCntPause;		//カウント

	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (s_apTexture[nCntPause] != NULL)
		{
			s_apTexture[nCntPause]->Release();
			s_apTexture[nCntPause] = NULL;
		}
	}

	//頂点バッファの破棄
	if (s_pVtxBuffPause != NULL)
	{
		s_pVtxBuffPause->Release();
		s_pVtxBuffPause = NULL;
	}

	if (s_pVtxBuffPauseMenu != NULL)
	{
		s_pVtxBuffPauseMenu->Release();
		s_pVtxBuffPauseMenu = NULL;
	}
}

//============================================================================================================
//ポーズの更新処理
//============================================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	if (GetFade() == FADE_NONE)
	{//フェードしていないとき
		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

		if (GetKeyboardTrigger(DIK_W))
		{//Wキーが押された
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_SELECT);

			//白にする
			s_aPauseMenu[s_pauseMenu].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += (s_pauseMenu * 4);		//ポインタを該当の位置まで進める

			 //頂点カラーの設定
			pVtx[0].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[1].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[2].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[3].col = s_aPauseMenu[s_pauseMenu].col;

			 //1つ上を選ぶ
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu - 1) + 3) % 3);
		}

		if (GetKeyboardTrigger(DIK_S))
		{//Sキーが押された
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_SELECT);

			//白にする
			s_aPauseMenu[s_pauseMenu].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += (s_pauseMenu * 4);		//ポインタを該当の位置まで進める

			//頂点カラーの設定
			pVtx[0].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[1].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[2].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[3].col = s_aPauseMenu[s_pauseMenu].col;

			 //1つ下を選ぶ
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu + 1) + 3) % 3);
		}

		//頂点バッファをアンロックする
		s_pVtxBuffPauseMenu->Unlock();

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (s_pauseMenu * 4);		//ポインタを該当の位置まで進める

		//選択部分の色を変える
		s_aPauseMenu[s_pauseMenu].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = s_aPauseMenu[s_pauseMenu].col;
		pVtx[1].col = s_aPauseMenu[s_pauseMenu].col;
		pVtx[2].col = s_aPauseMenu[s_pauseMenu].col;
		pVtx[3].col = s_aPauseMenu[s_pauseMenu].col;

		//頂点バッファをアンロックする
		s_pVtxBuffPauseMenu->Unlock();

		if (GetKeyboardTrigger(DIK_RETURN))
		{//Enterキーが押された
			switch (s_pauseMenu)
			{
			case PAUSE_MENU_CONTINUE:	//ゲームに戻る
				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_CANCEL);

				//ポーズ解除
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:		//ゲームをやり直す
				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//ゲーム画面に移行
				SetFade(MODE_GAME);
				break;

			case PAUSE_MENU_QUIT:		//タイトル画面に戻る
				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//タイトル画面に移行
				SetFade(MODE_TITLE);
				break;
			}
		}
	}
}

//============================================================================================================
//ポーズの描画処理
//============================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	int nCntPause;								//カウント
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//ポーズの半透明ポリゴン
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		else if (nCntPause == 1)
		{//ポーズ
			//テクスチャの設定
			pDevice->SetTexture(0, s_apTexture[3]);
		}

		//ポーズの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCntPause * 4,			//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPauseMenu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTexture[nCntPause]);

		//ポーズの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCntPause * 4,			//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}
}