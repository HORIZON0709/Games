//================================================
//
//3Dゲーム(仮)[result.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "result.h"
#include "application.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

#include "bg.h"
#include "object2D.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const float CResult::GAMECLEAR_WIDTH = 800.0f;	//ゲームクリアの幅
const float CResult::GAMECLEAR_HEIGHT = 300.0f;	//ゲームクリアの高さ

//***************************
//静的メンバ変数
//***************************
CBg* CResult::m_pBg = nullptr;				//背景
CObject2D* CResult::m_pGameClear = nullptr;	//ゲームクリア

//================================================
//コンストラクタ
//================================================
CResult::CResult() : CMode(MODE::RESULT),
m_bFadeOut(false)
{
}

//================================================
//デストラクタ
//================================================
CResult::~CResult()
{
}

//================================================
//初期化
//================================================
HRESULT CResult::Init()
{
	//メンバ変数の初期化
	m_bFadeOut = false;

	/* 背景 */

	//生成
	m_pBg = CBg::Create();

	//テクスチャの設定
	m_pBg->SetTexture(CTexture::Bg_Result);

	//テクスチャ座標の設定
	m_pBg->SetTexUV(1, 0);

	/* ゲームクリア */

	//生成
	m_pGameClear = CObject2D::Create();

	D3DXVECTOR3 pos = D3DXVECTOR3(	//位置設定用
		CRenderer::SCREEN_WIDTH * 0.5f,
		CRenderer::SCREEN_HEIGHT * 0.5f,
		0.0f);

	//位置を設定
	m_pGameClear->SetPos(pos);

	//サイズを設定
	D3DXVECTOR2 size = D3DXVECTOR2(GAMECLEAR_WIDTH, GAMECLEAR_HEIGHT);
	m_pGameClear->SetSize(size);

	//テクスチャの設定
	m_pGameClear->SetTexture(CTexture::GameClear);

	//テクスチャ座標の設定
	m_pGameClear->SetTexUV(1, 0);

	//BGM開始
	CApplication::GetSound()->Play(CSound::LABEL_BGM_Result);

	//明転
	CApplication::GetFade()->Set(CFade::STATE::FADE_IN);

	return S_OK;
}

//================================================
//終了
//================================================
void CResult::Uninit()
{
	/* サウンド */

	CApplication::GetSound()->Stop(CSound::LABEL_BGM_Result);

	/* オブジェクト */

	CObject2D::ReleaseAll();	//全ての解放(2D)

	/* 背景 */

	if (m_pBg != nullptr)
	{//NULLチェック
		m_pBg = nullptr;	//nullptrにする
	}
}

//================================================
//更新
//================================================
void CResult::Update()
{
	CObject::UpdateAll();	//オブジェクト

	CInput* pInput = CInput::GetKey();	//キーボード情報を取得

	if (pInput->Trigger(CInput::STANDARD_KEY::DECISION))
	{//決定キ―
		CApplication::GetFade()->Set(CFade::STATE::FADE_OUT);	//暗転
		m_bFadeOut = true;	//暗転した
	}

	if (m_bFadeOut && (CApplication::GetFade()->GetState() == CFade::STATE::NONE))
	{//フェードが終わって暗転中の場合
		Change(MODE::TITLE);	//モードの設定
	}
}

//================================================
//描画
//================================================
void CResult::Draw()
{
	CObject::DrawAll();	//オブジェクト
}