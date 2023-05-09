//================================================
//
//3Dゲーム(仮)[bg.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _BG_H_
#define _BG_H_

//***************************
//インクルード
//***************************
#include "object2D.h"

//***************************
//背景クラスの定義
//***************************
class CBg : public CObject2D
{/* CObject2Dの派生クラス */
public: /* 定数の定義 */
	static const float BG_WIDTH;	//横幅
	static const float BG_HEIGHT;	//縦幅

public: /* 静的メンバ関数 */
	static CBg* Create();	//生成

public: /* コンストラクタ・デストラクタ */
	CBg();
	~CBg()override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画
};
#endif