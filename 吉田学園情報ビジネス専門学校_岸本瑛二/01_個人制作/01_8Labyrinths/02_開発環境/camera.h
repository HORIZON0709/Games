//================================================
//
//3Dゲーム(仮)[camera.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//カメラクラスの定義
//***************************
class CCamera
{/* 基本クラス */
public: /* 定数の定義 */
	static const float MOVE_SPEED;	//移動速度

public: /* コンストラクタ・デストラクタ */
	CCamera();
	~CCamera();

public: /* メンバ関数 */
	void Init();	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Set();		//設定

	D3DXMATRIX GetMatrixView();	//ビューマトリクスの取得
	D3DXVECTOR3 GetPosV();		//視点の位置の取得
private:
	void Move();	//移動

private: /* メンバ変数 */
	D3DXVECTOR3 m_posV;	//視点
	D3DXVECTOR3 m_posR;	//注視点
	D3DXVECTOR3 m_vecU;	//上方向ベクトル
	D3DXVECTOR3 m_move;	//移動量

	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
};

#endif