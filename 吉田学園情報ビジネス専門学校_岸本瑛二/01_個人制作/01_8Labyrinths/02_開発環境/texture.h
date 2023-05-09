//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// Author : KIshimoto Eiji
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		NONE = -1,	//使用しない

		/* 仮背景 */
		Bg_Title_Tentative = 0,	//仮背景：タイトル
		Bg_Result_Tentative,	//仮背景：リザルト

		/* Number_Single */
		Number_Single_1,	//ナンバー画像(単体)[1]
		Number_Single_2,	//ナンバー画像(単体)[2]
		Number_Single_3,	//ナンバー画像(単体)[3]
		Number_Single_4,	//ナンバー画像(単体)[4]
		Number_Single_5,	//ナンバー画像(単体)[5]
		Number_Single_6,	//ナンバー画像(単体)[6]
		Number_Single_7,	//ナンバー画像(単体)[7]
		Number_Single_8,	//ナンバー画像(単体)[8]
		Cross,				//バツ

		/* Number */
		Number00,	//ナンバー画像

		/* ゲーム中用素材 */
		WallAndFloar,	//壁と床用
		GameUI_Purpose,	//目的
		GameUI_Coin,	//獲得コイン数

		/* 背景 */
		Bg_Title,	//タイトル背景
		Bg_Result,	//リザルト背景

		/* タイトルロゴ */
		TitleLogo,	//タイトルロゴ

		/* PressEnter */
		PressEnter,	//PressEnter

		/* ゲームクリア */
		GameClear,	//ゲームクリア

		MAX
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
