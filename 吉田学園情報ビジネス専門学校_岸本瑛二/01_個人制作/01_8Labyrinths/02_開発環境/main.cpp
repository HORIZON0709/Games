//=============================================================================
//
//メイン
//Author : 
//
//=============================================================================
//***************************
//ライブラリーリンク
//***************************
#pragma comment(lib,"winmm.lib")

//***************************
//インクルード
//***************************
#include "application.h"
#include "renderer.h"
#include "debug_proc.h"

#include <Windows.h>
#include <tchar.h> //_T
#include <d3dx9.h>
#include <assert.h>

//***************************
//定数定義
//***************************
namespace
{
//ウインドウのクラス名
LPCTSTR CLASS_NAME = _T("AppClass");
//ウインドウのキャプション名
LPCTSTR WINDOW_NAME = _T("8Labyrinths");
}

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//***************************
//スタティック変数
//***************************
namespace
{
int s_nCountFPS;	//FPSカウンタ
}//namespaceはここまで

 //=============================================================================
//メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	RECT rect = { 0, 0, CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT };
	//指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウの作成
	HWND hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	/* アプリケーション */

	CApplication* pApplication = nullptr;	//ポインタ

	if (pApplication == nullptr)
	{//NULLチェック
		pApplication = new CApplication;	//メモリの動的確保
	}

	if (FAILED(pApplication->Init(hWnd, TRUE, hInstance)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	//分解能を設定
	timeBeginPeriod(1);

	//フレームカウント初期化
	DWORD dwCurrentTime = 0;
	DWORD dwExecLastTime = timeGetTime();
#ifdef _DEBUG
	DWORD dwFrameCount = 0;
	DWORD dwFPSLastTime = dwExecLastTime;
#endif //_DEBUG

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	//メッセージループ
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{ //PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				//メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();	//現在の時間を取得
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒ごとに実行
				//FPSを算出
				s_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//現在の時間を保存
				dwFrameCount = 0;
			}
#endif //_DEBUG

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{ //1/60秒経過
				//現在の時間を保存
				dwExecLastTime = dwCurrentTime;

				//更新処理
				pApplication->Update();

				//描画処理
				pApplication->Draw();

#ifdef _DEBUG
				dwFrameCount++;

				//文字列の登録
				CDebugProc::Print("FPS : %d\n",s_nCountFPS);
#endif //_DEBUG
			}
		}
	}

	if (pApplication != nullptr)
	{//NULLチェック
		pApplication->Uninit();	//終了処理
		delete pApplication;	//メモリの解放
		pApplication = nullptr;	//nullptrにする
	}

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
//ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: //[ESC]キーが押された
			//ウィンドウを破棄
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//================================================
//FPSの取得
//================================================
int GetFPS()
{
	return s_nCountFPS;
}