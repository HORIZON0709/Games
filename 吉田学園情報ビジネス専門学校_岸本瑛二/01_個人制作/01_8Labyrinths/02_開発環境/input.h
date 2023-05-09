//================================================
//
//3Dゲーム(仮)[input.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//***************************
//インクルード
//***************************
#include "inputKeyboard.h"

//***************************
//インプットクラスの定義
//***************************
class CInput
{/* 基本クラス */
public: /* 列挙型の定義 */
	enum STANDARD_KEY
	{/* 通常操作で使用するキー */
		UP = 0,		//上
		DOWN,		//下
		LEFT,		//左
		RIGHT,		//右
		DECISION,	//決定
		SHOT		//発射
	};

public: /* 静的メンバ関数 */
	static CInput* Create();	//生成
	static CInput* GetKey();	//取得

public: /* コンストラクタ・デストラクタ */
	CInput();
	~CInput();

public: /* メンバ関数 */
	HRESULT Init(HINSTANCE hInstance,HWND hWnd);	//初期化
	void Uninit();									//終了
	void Update();									//更新

	bool Press(STANDARD_KEY key);	//プレス
	bool Release(STANDARD_KEY key);	//リリース
	bool Trigger(STANDARD_KEY key);	//トリガー

private: /* 静的メンバ変数 */
	static CInput* m_pInput;

private: /* メンバ変数 */
	CInputKeyboard* m_pInputKeyboard;	//キーボードのポインタ
};

#endif