//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// Author : KIshimoto Eiji
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "application.h"
#include "renderer.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	/* 仮背景 */
	"data/TEXTURE/Bg_Title_Tentative.png",	//仮背景：タイトル
	"data/TEXTURE/Bg_Result_Tentative.png",	//仮背景：リザルト

	/* Number_Single */
	"data/TEXTURE/Number_Single/Number_Single_1.png",	//ナンバー画像(単体)[1]
	"data/TEXTURE/Number_Single/Number_Single_2.png",	//ナンバー画像(単体)[2]
	"data/TEXTURE/Number_Single/Number_Single_3.png",	//ナンバー画像(単体)[3]
	"data/TEXTURE/Number_Single/Number_Single_4.png",	//ナンバー画像(単体)[4]
	"data/TEXTURE/Number_Single/Number_Single_5.png",	//ナンバー画像(単体)[5]
	"data/TEXTURE/Number_Single/Number_Single_6.png",	//ナンバー画像(単体)[6]
	"data/TEXTURE/Number_Single/Number_Single_7.png",	//ナンバー画像(単体)[7]
	"data/TEXTURE/Number_Single/Number_Single_8.png",	//ナンバー画像(単体)[8]
	"data/TEXTURE/Number_Single/Cross.png",				//バツ

	/* Number */
	"data/TEXTURE/Number00.png",	//ナンバー画像

	/* ゲーム中用素材 */
	"data/TEXTURE/WallAndFloar.jpg",	//壁と床用
	"data/TEXTURE/GameUI_Purpose.png",	//目的
	"data/TEXTURE/GameUI_Coin.png",		//獲得コイン数

	/* 背景 */
	"data/TEXTURE/Bg_Title.jpg",	//タイトル背景
	"data/TEXTURE/Bg_Result.jpg",	//リザルト背景


	/* タイトルロゴ */
	"data/TEXTURE/TitleLogo.png",	//タイトルロゴ

	/* PressEnter */
	"data/TEXTURE/PressEnter.png",	//PressEnter

	/* ゲームクリア */
	"data/TEXTURE/GameClear.png",	//ゲームクリア
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::MAX, "aho");

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	
	for (int i = 0; i < MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < MAX);

	if (s_pTexture[inTexture] != NULL)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
