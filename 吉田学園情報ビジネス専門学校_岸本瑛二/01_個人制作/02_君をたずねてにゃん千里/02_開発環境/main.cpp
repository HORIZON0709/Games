//============================================================================================================
//
//君をたずねてにゃん千里[main.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "score.h"
#include "fade.h"
#include "sound.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define CLASS_NAME		"WindowsClass"				//ウインドウクラスの名前
#define WINDOW_NAME		"君をたずねてにゃん千里"	//ウインドウの名前(キャプションに表示)

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3D9			s_pD3D = NULL;				//Direct3Dオブジェクトへのポインタ
static LPDIRECT3DDEVICE9	s_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
static MODE					s_mode = MODE_TITLE;		//現在のモード
static LPD3DXFONT			s_pFont = NULL;				//フォントへのポインタ
static int					s_nCountFPS = 0;			//FPSカウンター

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//============================================================================================================
//メイン関数
//============================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };		//画面サイズの構造体
	HWND hWnd;											//ウインドウハンドル(識別子)
	MSG msg;											//メッセージを格納する変数
	DWORD dwCurrentTime;								//現在時刻
	DWORD dwExecLastTime;								//最後に処理した時刻
	DWORD dwFrameCount;									//フレームカウント
	DWORD dwFPSLastTime;								//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),		//ファイルのアイコン
	};

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,							//拡張ウインドウスタイル
							CLASS_NAME,					//ウインドウクラスの名前
							WINDOW_NAME,				//ウインドウの名前
							WS_OVERLAPPEDWINDOW,		//ウインドウスタイル
							CW_USEDEFAULT,				//ウインドウの左上X座標
							CW_USEDEFAULT,				//ウインドウの左上Y座標
							(rect.right - rect.left),	//ウインドウの幅
							(rect.bottom - rect.top),	//ウインドウの高さ
							NULL,						//親ウインドウのハンドル
							NULL,						//メニューハンドルまたは子ウインドウID
							hInstance,					//インスタンスハンドル
							NULL);						//ウインドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得(保存)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd,nCmdShow);		//ウインドウ表示状態を設定
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);		//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				s_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPSを計測した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;		//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME,wcex.hInstance);

	return (int)msg.wParam;
}

//============================================================================================================
//ウインドウプロシージャ
//============================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	VERTEX_2D *pVtx = 0;											//頂点情報へのポインタ
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			//ウインドウの領域(短形)

	switch (uMsg)
	{
	case WM_DESTROY:        //ウインドウ破棄のメッセージ
							//WM_QOITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:        //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:        //[ESC]キーが押された
							   //ウインドウを破棄する (WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);        //規定の処理を返す
}

//============================================================================================================
//初期化処理
//============================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&s_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp,
										&s_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&s_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダースタートの設定
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバック表示用のフォントの生成
	D3DXCreateFont(s_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
					"Terminal", &s_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	////ジョイパッドの初期化処理
	//InitJoypad();

	//サウンドの初期化処理
	InitSound(hWnd);

	//フェードの設定
	InitFade(s_mode);

	//ランキングのリセット
	ResetRanking();

	return S_OK;
}

//============================================================================================================
//終了処理
//============================================================================================================
void Uninit(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	////ジョイパッドの終了処理
	//UninitJoypad();

	//サウンドの終了処理
	UninitSound();

	//フェードの終了処理
	UninitFade();

	//タイトル画面の終了処理
	UninitTitle();

	//チュートリアルの終了処理
	UninitTutorial();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//ランキング画面の終了処理
	UninitRanking();

	//デバック表示用フォントの破棄
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (s_pD3DDevice != NULL)
	{
		s_pD3DDevice->Release();
		s_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (s_pD3D != NULL)
	{
		s_pD3D->Release();
		s_pD3D = NULL;
	}
}

//============================================================================================================
//更新処理
//============================================================================================================
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	////ジョイパッドの更新処理
	//UpdateJoypad();

	switch (s_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT:		//リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING:		//ランキング画面
		UpdateRanking();
		break;
	}

	//フェードの更新処理
	UpdateFade();
}

//============================================================================================================
//描画処理
//============================================================================================================
void Draw(void)
{
	//画面クリア(バックバッファ＆Zバッファのクリア)
	s_pD3DDevice->Clear(0, NULL,
						(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
						D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//描画処理
	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		switch (s_mode)
		{
		case MODE_TITLE:		//タイトル画面
			DrawTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			DrawTutorial();
			break;

		case MODE_GAME:			//ゲーム画面
			DrawGame();
			break;

		case MODE_RESULT:		//リザルト画面
			DrawResult();
			break;

		case MODE_RANKING:		//ランキング画面
			DrawRanking();
			break;
		}

		//フェードの描画処理
		DrawFade();

#ifdef _DEBUG
		//FPSの表示
		DrawFPS();
#endif

		//描画終了
		s_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//============================================================================================================
//モードの設定
//============================================================================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (s_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UninitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:		//リザルト画面
		UninitResult();
		break;

	case MODE_RANKING:		//ランキング画面
		UninitRanking();
		break;
	}

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:		//リザルト画面
		InitResult();
		break;

	case MODE_RANKING:		//ランキング画面
		InitRanking();

		//ランキングの設定
		SetRanking(GetScore());
		break;
	}

	s_mode = mode;		//現在の画面(モード)を切り替える
}

//============================================================================================================
//デバイスの取得
//============================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return s_pD3DDevice;
}

//============================================================================================================
//モードの取得
//============================================================================================================
MODE GetMode(void)
{
	return s_mode;
}

//============================================================================================================
//FPSの表示
//============================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	int pTutorial = GetTutorial();

	//文字列に代入
	wsprintf(&aStr[0], "FPS : %2d\n", s_nCountFPS);				//FPS
	wsprintf(&aStr[9], "GameMode : %2d\n", s_mode);				//ゲームモード
	wsprintf(&aStr[23], "TutorialTex : %2d\n", pTutorial);		//チュートリアルのテクスチャ

	//テキストの描画
	s_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 255, 255));
}

//============================================================================================================
//テクスチャ座標の設定
//============================================================================================================
void SetTex(VERTEX_2D *pVtx, float fLeft, float fRight, float fUp, float fDown)
{
	pVtx[0].tex = D3DXVECTOR2(fLeft, fUp);
	pVtx[1].tex = D3DXVECTOR2(fRight, fUp);
	pVtx[2].tex = D3DXVECTOR2(fLeft, fDown);
	pVtx[3].tex = D3DXVECTOR2(fRight, fDown);
}