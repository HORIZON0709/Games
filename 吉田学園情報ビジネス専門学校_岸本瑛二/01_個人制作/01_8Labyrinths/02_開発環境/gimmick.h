//================================================
//
//3Dゲーム(仮)[gimmick.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

//***************************
//インクルード
//***************************
#include "objectX.h"

//***************************
//前方宣言
//***************************
class CLine;

//***************************
//プレイヤークラスの定義
//***************************
class CGimmick : public CObjectX
{/* CObjectXの派生クラス */
private: /* 定数の定義 */
	static const float MOVE_SPEED;		//移動速度
	static const float ROT_SMOOTHNESS;	//回転の滑らかさ
	static const float ROTATION_SPEED;	//回転速度

	static const int MAX_LINE = 12;	//ラインの最大数

public: /* 静的メンバ関数 */
	/*
		生成
		char* pFileName ---> ファイル名
	*/
	static CGimmick* Create(char* pFileName);

public: /* コンストラクタ・デストラクタ */
	CGimmick();
	~CGimmick() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

private: /* メンバ関数 */
	void SetLines();	//ラインの設定まとめ

private: /* メンバ変数 */
	CLine* m_apLine[MAX_LINE];	//ラインのポインタ

	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス

	D3DXVECTOR3 m_vec;		//方向ベクトル
	D3DXVECTOR3 m_rotDest;	//目的の向き

	D3DXQUATERNION m_quaternion;	//クォータニオン
};

#endif