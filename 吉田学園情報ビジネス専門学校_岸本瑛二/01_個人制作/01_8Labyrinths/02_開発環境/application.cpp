//================================================
//
//3Dゲーム(仮)[application.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "application.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "fade.h"
#include "mode.h"
#include "sound.h"

//***************************
//静的メンバ変数
//***************************
CInput* CApplication::m_pInput = nullptr;					//インプット
CInputKeyboard* CApplication::m_pInputKeyboard = nullptr;	//キーボード

CTexture* CApplication::m_pTexture = nullptr;	//テクスチャ
CRenderer* CApplication::m_pRenderer = nullptr;	//レンダラー

CMode* CApplication::m_pMode = nullptr;		//モード
CFade* CApplication::m_pFade = nullptr;		//フェード
CSound* CApplication::m_pSound = nullptr;	//サウンド

//================================================
//インプット情報を取得
//================================================
CInput* CApplication::GetInput()
{
	return m_pInput;
}

//================================================
//キーボード情報を取得
//================================================
CInputKeyboard* CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//================================================
//テクスチャ情報を取得
//================================================
CTexture* CApplication::GetTexture()
{
	return m_pTexture;
}

//================================================
//レンダラー情報を取得
//================================================
CRenderer* CApplication::GetRenderer()
{
	return m_pRenderer;
}

//================================================
//モード情報を取得
//================================================
CMode* CApplication::GetMode()
{
	return m_pMode;
}

//================================================
//フェード情報を取得
//================================================
CFade* CApplication::GetFade()
{
	return m_pFade;
}

//================================================
//サウンド情報を取得
//================================================
CSound* CApplication::GetSound()
{
	return m_pSound;
}

//================================================
//コンストラクタ
//================================================
CApplication::CApplication()
{
}

//================================================
//デストラクタ
//================================================
CApplication::~CApplication()
{
}

//================================================
//初期化
//================================================
HRESULT CApplication::Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance)
{
	/* インプット */

	if (m_pInput == nullptr)
	{//NULLチェック
		m_pInput = CInput::Create();	//生成
	}

	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	/* キーボード */

	if (m_pInputKeyboard == nullptr)
	{//NULLチェック
		m_pInputKeyboard = new CInputKeyboard;	//メモリの動的確保
	}

	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	/* テクスチャ */

	if (m_pTexture == nullptr)
	{//NULLチェック
		m_pTexture = new CTexture;	//メモリの動的確保
	}

	/* レンダラー */

	if (m_pRenderer == nullptr)
	{//NULLチェック
		m_pRenderer = new CRenderer;	//メモリの動的確保
	}

	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	/* フェード */

	if (m_pFade == nullptr)
	{//NULLチェック
		m_pFade = CFade::Create();	//生成
	}

	/* サウンド */

	if (m_pSound == nullptr)
	{//NULLチェック
		m_pSound = new CSound;	//メモリの動的確保
	}

	if (FAILED(m_pSound->Init(hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	/* モード */

	if (m_pMode == nullptr)
	{//NULLチェック
		m_pMode = CMode::Create(CMode::MODE::TITLE);	//生成
	}

	return S_OK;
}

//================================================
//終了
//================================================
void CApplication::Uninit()
{
	/* モード */

	if (m_pMode != nullptr)
	{//NULLチェック
		m_pMode->Uninit();	//終了
		delete m_pMode;		//メモリの解放
		m_pMode = nullptr;	//nullptrにする
	}

	/* サウンド */

	if (m_pSound != nullptr)
	{//NULLチェック
		m_pSound->Uninit();	//終了
		delete m_pSound;	//メモリの解放
		m_pSound = nullptr;	//nullptrにする
	}

	/* フェード */

	if (m_pFade != nullptr)
	{//NULLチェック
		m_pFade->Uninit();	//終了
		delete m_pFade;		//メモリの解放
		m_pFade = nullptr;	//nullptrにする
	}

	/* レンダラー */

	if (m_pRenderer != nullptr)
	{//NULLチェック
		m_pRenderer->Uninit();	//終了処理
		delete m_pRenderer;		//メモリの解放
		m_pRenderer = nullptr;	//nullptrにする
	}

	/* テクスチャ */

	if (m_pTexture != nullptr)
	{//NULLチェック
		m_pTexture->ReleaseAll();	//終了処理
		delete m_pTexture;			//メモリの解放
		m_pTexture = nullptr;		//nullptrにする
	}

	/* キーボード */

	if (m_pInputKeyboard != nullptr)
	{//NULLチェック
		m_pInputKeyboard->Uninit();	//終了処理
		delete m_pInputKeyboard;	//メモリの解放
		m_pInputKeyboard = nullptr;	//nullptrにする
	}

	/* インプット */

	m_pInput->Uninit();	//終了処理

}

//================================================
//更新
//================================================
void CApplication::Update()
{
	if (m_pInput != nullptr)
	{//NULLチェック
		m_pInput->Update();	//インプット
	}

	if (m_pInputKeyboard != nullptr)
	{//NULLチェック
		m_pInputKeyboard->Update();	//キーボード
	}

	if (m_pRenderer != nullptr)
	{//NULLチェック
		m_pRenderer->Update();	//レンダラー
	}

	if (m_pMode != nullptr)
	{//NULLチェック
		m_pMode = m_pMode->Set();	//モードの設定
	}

	if (m_pFade != nullptr)
	{//NULLチェック
		m_pFade->Update();	//フェード
	}
}

//================================================
//描画
//================================================
void CApplication::Draw()
{
	if (m_pRenderer != nullptr)
	{//NULLチェック
		m_pRenderer->Draw();	//レンダラー
	}
}