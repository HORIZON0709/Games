//================================================
//
//3Dゲーム(仮)[light.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//ライトクラスの定義
//***************************
class CLight
{/* 基本クラス */
private: /* 定数の定義 */
	static const int MAX_LIGHT = 3;	//ライトの最大数

public: /* コンストラクタ・デストラクタ */
	CLight();
	~CLight();

public: /* メンバ関数 */
	void Init();	//初期化
	void Uninit();	//終了
	void Update();	//更新
private:
	/* 設定 */
	void Set(const int &nNum, const D3DXCOLOR &Diffuse, const D3DXVECTOR3 &vecDirection);

private: /* メンバ変数 */
	D3DLIGHT9 m_aLight[MAX_LIGHT];	//ライト情報
};

#endif