//================================================
//
//3Dゲーム(仮)[title.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//***************************
//インクルード
//***************************
#include "mode.h"

#include <d3dx9.h>

//***************************
//前方宣言
//***************************
class CObject2D;
class CBg;

//***************************
//タイトルクラスの定義
//***************************
class CTitle : public CMode
{/* CModeの派生クラス */
private: /* 定数の定義 */
	static const float LOGO_WIDTH;	//ロゴの幅
	static const float LOGO_HEIGHT;	//ロゴの高さ

	static const float PRESSENTER_WIDTH;	//PressEnterの幅
	static const float PRESSENTER_HEIGHT;	//PressEnterの高さ

public: /* コンストラクタ・デストラクタ */
	CTitle();
	~CTitle() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

private: /* 静的メンバ変数 */
	static CBg* m_pBg;					//背景
	static CObject2D* m_pLogo;			//タイトルロゴ
	static CObject2D* m_pPressEnter;	//PressEnter

private: /* メンバ変数 */
	bool m_bFadeOut;	//暗転したかどうか
};
#endif