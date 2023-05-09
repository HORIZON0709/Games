//================================================
//
//デバッグ表示[debug_proc.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _DEBUG_PROC_H_
#define _DEBUG_PROC_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//デバッグ表示クラスの定義
//***************************
class CDebugProc
{/* 基本クラス */
private: /* 定数の定義 */
	static const int MAX_TEXT = 1024;	//文字数の最大値

public: /* 静的メンバ関数 */
	static void Print(const char *pFormat, ...);	//文字列の登録
	static void Draw();	//描画

public: /* コンストラクタ・デストラクタ */
	CDebugProc();
	~CDebugProc();

public: /* メンバ関数 */
	void Init();	//初期化
	void Uninit();	//終了

private: /* 静的メンバ変数 */
	static LPD3DXFONT m_pFont;		//フォント情報
	static char m_str[MAX_TEXT];	//登録文字列
};

#endif