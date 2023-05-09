//================================================
//
//3Dゲーム(仮)[result.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//***************************
//インクルード
//***************************
#include "mode.h"

#include <d3dx9.h>

//***************************
//前方宣言
//***************************
class CBg;
class CObject2D;

//***************************
//リザルトクラスの定義
//***************************
class CResult : public CMode
{/* CModeの派生クラス */
private: /* 定数の定義 */
	static const float GAMECLEAR_WIDTH;		//ゲームクリアの幅
	static const float GAMECLEAR_HEIGHT;	//ゲームクリアの高さ

public: /* コンストラクタ・デストラクタ */
	CResult();
	~CResult() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

private: /* 静的メンバ変数 */
	static CBg* m_pBg;				//背景
	static CObject2D* m_pGameClear;	//ゲームクリア

private: /* メンバ変数 */
	bool m_bFadeOut;	//暗転したかどうか
};
#endif