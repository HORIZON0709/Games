//================================================
//
//3Dゲーム(仮)[application.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//前方宣言
//***************************
class CInput;
class CInputKeyboard;
class CRenderer;
class CTexture;
class CMode;
class CFade;
class CSound;

//***************************
//アプリケーションクラスの定義
//***************************
class CApplication
{/* 基本クラス */
public: /* 静的メンバ関数 */

	//********** 情報の取得 **********//

	static CInput* GetInput();					//インプット
	static CInputKeyboard* GetInputKeyboard();	//キーボード

	static CTexture* GetTexture();		//テクスチャ
	static CRenderer* GetRenderer();	//レンダラー

	static CMode* GetMode();	//モード
	static CFade* GetFade();	//フェード
	static CSound* GetSound();	//サウンド

public: /* コンストラクタ・デストラクタ */
	CApplication();
	~CApplication();

public: /* メンバ関数 */
	HRESULT Init(HWND hWnd, BOOL bWindow,HINSTANCE hInstance);	//初期化

	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

private: /* 静的メンバ変数 */
	
	//********** ポインタ **********//

	static CInput* m_pInput;					//インプット
	static CInputKeyboard* m_pInputKeyboard;	//キーボード

	static CTexture* m_pTexture;	//テクスチャ
	static CRenderer* m_pRenderer;	//レンダラー

	static CMode* m_pMode;		//モード
	static CFade* m_pFade;		//フェード
	static CSound* m_pSound;	//サウンド
};
#endif