//================================================
//
//3Dゲーム(仮)[mode.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _MODE_H_
#define _MODE_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//モードクラスの定義
//***************************
class CMode
{/* 基本クラス */
public: /* 列挙型の定義 */
	enum MODE	//モードの種類
	{
		NONE = -1,
		TITLE = 0,	//タイトル
		GAME,		//ゲーム
		RESULT,		//リザルト
		MAX
	};

public: /* 静的メンバ関数 */
	/*
		生成
		const MODE &mode ---> 生成するモード
	*/
	static CMode* Create(const MODE &mode);

public: /* コンストラクタ・デストラクタ */
	CMode() = delete;	//デフォルト(使わない)
	CMode(MODE mode);
	virtual ~CMode();

public: /* 純粋仮想関数 */
	virtual HRESULT Init() = 0;	//初期化
	virtual void Uninit() = 0;	//終了
	virtual void Update() = 0;	//更新
	virtual void Draw() = 0;	//描画

public: /* メンバ関数 */
	CMode* Set();					//モードの設定
	MODE Get();						//モードの取得
	void Change(const MODE &mode);	//モードの変更

private: /* メンバ変数 */
	MODE m_mode;		//現在のモード
	MODE m_modeNext;	//次のモード
};
#endif