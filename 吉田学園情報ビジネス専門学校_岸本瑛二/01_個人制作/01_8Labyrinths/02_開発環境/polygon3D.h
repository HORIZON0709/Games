//================================================
//
//3Dゲーム(仮)[polygon3D.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

//***************************
//インクルード
//***************************
#include "object3D.h"

//***************************
//3Dポリゴンクラスの定義
//***************************
class CPolygon3D : public CObject3D
{/* CObject3Dの派生クラス */
public: /* 定数の定義 */
	static const float POLYGON_WIDTH;	//横幅
	static const float POLYGON_HEIGHT;	//縦幅

public: /* 静的メンバ関数 */
	static CPolygon3D* Create();	//生成

public: /* コンストラクタ・デストラクタ */
	CPolygon3D();
	~CPolygon3D()override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画
};
#endif