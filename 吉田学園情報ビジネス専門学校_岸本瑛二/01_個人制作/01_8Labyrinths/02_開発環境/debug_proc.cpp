//================================================
//
//デバッグ表示[debug_proc.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "debug_proc.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>

//***************************
//静的メンバ変数
//***************************
LPD3DXFONT CDebugProc::m_pFont = nullptr;	//フォント情報
char CDebugProc::m_str[MAX_TEXT] = {};		//登録文字列

//================================================
//文字列の登録
//================================================
void CDebugProc::Print(const char *pFormat, ...)
{
#ifdef _DEBUG
	// 変数
	char str[MAX_TEXT] = {};

	// リストの作成
	va_list args;
	va_start(args, pFormat);
	vsprintf(&str[0], pFormat, args);
	va_end(args);

	// 文字列の連結
	strcat(&m_str[0], &str[0]);
#endif // _DEBUG
}

//================================================
//描画
//================================================
void CDebugProc::Draw()
{
#ifdef _DEBUG
	RECT rect = { 0, 0, CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText(NULL, m_str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	m_str[0] = '\0';
#endif // _DEBUG
}

//================================================
//コンストラクタ
//================================================
CDebugProc::CDebugProc()
{
#ifdef _DEBUG
#endif // _DEBUG
}

//================================================
//デストラクタ
//================================================
CDebugProc::~CDebugProc()
{
#ifdef _DEBUG
#endif // _DEBUG
}

//================================================
//初期化
//================================================
void CDebugProc::Init()
{
#ifdef _DEBUG
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif // _DEBUG
}

//================================================
//終了
//================================================
void CDebugProc::Uninit()
{
#ifdef _DEBUG
	if (m_pFont != nullptr)
	{// フォントの破棄
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG
}
