//================================================
//
//3Dゲーム(仮)[inputKeyboard.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

//***************************
//マクロ定義
//***************************
#define DIRECTINPUT_VERSION		(0x0800)	//ビルド時の警告対策用マクロ(この位置から動かさない)

//***************************
//ライブラリーリンク
//***************************
#pragma comment(lib,"dinput8.lib")	//入力処理に必要(キーボード)
//#pragma comment(lib,"xinput.lib")	//入力処理に必要（ジョイパット）
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネントに使用

//***************************
//インクルード
//***************************
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>	//入力処理に必要

//***************************
//キーボード入力クラスの定義
//***************************
class CInputKeyboard
{/* 基本クラス */
private: /* 定数の定義 */
	static const int MAX_KEY = 256;	//キーの最大数

public: /* 静的メンバ関数 */

public: /* コンストラクタ・デストラクタ */
	CInputKeyboard();
	~CInputKeyboard();

public: /* メンバ関数 */
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit();									//終了
	void Update();									//更新
public: /* Get系 */
	bool GetPress(int nKey);	//プレス
	bool GetRelease(int nKey);	//リリース
	bool GetTrigger(int nKey);	//トリガー

private: /* メンバ変数 */
	LPDIRECTINPUT8 m_pInput;				//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;	//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	BYTE m_aKeyStatePress[MAX_KEY];			//プレス情報
	BYTE m_aKeyStateRelease[MAX_KEY];		//リリース情報
	BYTE m_aKeyStateTrigger[MAX_KEY];		//トリガー情報
};

#endif