//================================================
//
//3Dゲーム(仮)[number.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//***************************
//インクルード
//***************************
#include "object2D.h"

//***************************
//ナンバークラスの定義
//***************************
class CNumber : public CObject2D
{/* CObject2Dの派生クラス */
public: /* 定数の定義 */
	static const int DIVIDE_TEX_U = 10;	//テクスチャの分割数( U方向 )

	static const float POS_X;			//位置( X )
	static const float POS_Y;			//位置( Y )
	static const float NUMBER_WIDTH;	//横幅
	static const float NUMBER_HEIGHT;	//縦幅

public: /* 静的メンバ関数 */
	static CNumber* Create();	//生成

public: /* コンストラクタ・デストラクタ */
	CNumber();
	~CNumber()override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画
};
#endif