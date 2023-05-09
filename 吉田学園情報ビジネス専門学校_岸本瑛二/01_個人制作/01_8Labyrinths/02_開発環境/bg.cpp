//================================================
//
//3Dゲーム(仮)[bg.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "bg.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const float CBg::BG_WIDTH = (float)CRenderer::SCREEN_WIDTH;		//横幅
const float CBg::BG_HEIGHT = (float)CRenderer::SCREEN_HEIGHT;	//縦幅

//================================================
//生成
//================================================
CBg* CBg::Create()
{
	CBg* pBg = nullptr;	//ポインタ

	if (pBg != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pBg = new CBg;	//メモリの動的確保

	pBg->Init();	//初期化

	return pBg;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CBg::CBg()
{
	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::BG);
}

//================================================
//デストラクタ
//================================================
CBg::~CBg()
{
}

//================================================
//初期化
//================================================
HRESULT CBg::Init()
{
	CObject2D::Init();	//親クラス

	//位置を設定
	D3DXVECTOR3 pos = D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f,
								  CRenderer::SCREEN_HEIGHT * 0.5f,
								  0.0f);
	CObject2D::SetPos(pos);

	//サイズを設定
	D3DXVECTOR2 size = D3DXVECTOR2(BG_WIDTH, BG_HEIGHT);
	CObject2D::SetSize(size);

	return S_OK;
}

//================================================
//終了
//================================================
void CBg::Uninit()
{
	CObject2D::Uninit();	//親クラス
}

//================================================
//更新
//================================================
void CBg::Update()
{
	CObject2D::Update();	//親クラス
}

//================================================
//描画
//================================================
void CBg::Draw()
{
	CObject2D::Draw();	//親クラス
}