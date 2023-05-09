//================================================
//
//3Dゲーム(仮)[objectMesh.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECTMESH_H_
#define _OBJECTMESH_H_

//***************************
//インクルード
//***************************
#include "object3D.h"
#include "texture.h"

//***************************
//オブジェクトメッシュクラスの定義
//***************************
class CObjectMesh : public CObject
{/* CObjectの派生クラス */
private: /* 定数の定義 */
	static const int NUM_VERTEX;	//1ポリゴンの頂点数

	static const int NUM_BLK_X;	//ブロック数(X軸)
	static const int NUM_BLK_Z;	//ブロック数(Z軸)

	static const int NUM_VTX_X;	//頂点数(X軸)
	static const int NUM_VTX_Z;	//頂点数(Z軸)

	static const float MESH_SIZE;	//メッシュのサイズ

public: /* 静的メンバ関数 */
	static CObjectMesh* Create();	//生成

public: /* コンストラクタ・デストラクタ */
	CObjectMesh();
	~CObjectMesh() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

public: /* Pos */
	/*
		位置を設定
		const D3DXVECTOR3 &pos ---> 任意の位置
	*/
	void SetPos(const D3DXVECTOR3 &pos);

	//位置を取得
	D3DXVECTOR3 GetPos();

public: /* Rot */
	/*
		向きを設定
		const D3DXVECTOR3 &pos ---> 任意の向き
	*/
	void SetRot(const D3DXVECTOR3 &rot);

	//向きを取得
	D3DXVECTOR3 GetRot();

public: /* メンバ関数 */
	/*
		当たり判定
		D3DXVECTOR3* pPos ---> 位置のポインタ
	*/
	bool Collision(D3DXVECTOR3* pPos);

	/*
		2Dベクトルの内積
		D3DXVECTOR3* vec1 ---> 任意のベクトル1
		D3DXVECTOR3* vec2 ---> 任意のベクトル2
	*/
	float Vec2Dot(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

	/*
		2Dベクトルの外積
		D3DXVECTOR3* vec1 ---> 任意のベクトル1
		D3DXVECTOR3* vec2 ---> 任意のベクトル2
	*/
	float Vec2Cross(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);
private:
	int AskVtx();	//頂点数を求める
	int AskIdx();	//インデックス数を求める
	int AskPol();	//ポリゴン数を求める

private: /* メンバ変数 */
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ

	CTexture::TEXTURE m_texture;	//テクスチャの列挙型

	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き

	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス

	float m_fWidth;		//幅
	float m_fHeight;	//高さ

	int m_nNumVtx;	//頂点数
	int m_nNumIdx;	//インデックス数
	int m_nNumPol;	//ポリゴン数
};

#endif