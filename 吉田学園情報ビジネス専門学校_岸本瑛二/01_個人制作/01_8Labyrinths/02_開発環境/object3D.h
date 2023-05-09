//================================================
//
//3Dゲーム(仮)[object3D.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//***************************
//インクルード
//***************************
#include "object.h"
#include "texture.h"

//***************************
//定数の定義
//***************************

/* 頂点フォーマット */
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//***************************
//オブジェクトクラスの定義
//***************************
class CObject3D : public CObject
{/* CObjectの派生クラス */
public: /* 構造体の定義 */
	struct VERTEX_3D
	{//頂点データ
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 nor;	//法線ベクトル 
		D3DCOLOR col;		//色
		D3DXVECTOR2 tex;	//テクスチャ座標
	};

public: /* 静的メンバ関数 */
	static CObject3D* Create();	//生成

public: /* コンストラクタ・デストラクタ */
	CObject3D();
	~CObject3D() override;

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
		const D3DXVECTOR3 &rot ---> 任意の向き
	*/
	void SetRot(const D3DXVECTOR3 &rot);

	//向きを取得
	D3DXVECTOR3 GetRot();

public: /* Size */
	/*
		サイズを設定
		const D3DXVECTOR3 &size ---> 任意のサイズ
	*/
	void SetSize(const D3DXVECTOR3 &size);

	//サイズを取得
	D3DXVECTOR3 GetSize();

public: /* Move */
	/*
		移動量を設定
		const D3DXVECTOR3 &move ---> 任意の移動量
	*/
	void SetMove(const D3DXVECTOR3 &move);

	//移動量を取得
	D3DXVECTOR3 GetMove();

public: /* Texture */
	/*
		テクスチャの設定
		CTexture::TEXTURE texture ---> 任意のテクスチャ
	*/
	void SetTexture(CTexture::TEXTURE texture);

	/*
		テクスチャ座標の設定(テクスチャアニメーション用)
		int nDivNumU ---> テクスチャの分割数( U )
		int nDivNumV ---> テクスチャの分割数( V )
		int nPtnAnim ---> アニメーションパターン数
		単体画像(背景など)の場合は「1,1,0」
	*/
	void SetTexUV(int nDivNumU, int nDivNumV, int nPtnAnim);

	/*
		テクスチャ座標の設定(スクロール用)
		bool bDirection ---> スクロール方向[ false : X軸 / true : Y軸 ]
		float fMove ---> 移動量
	*/
	void SetTexUV(bool bDirection, float fMove);

public: /* Color */
	/*
		色の設定
		const D3DXCOLOR &col ---> 任意の色
	*/
	void SetCol(const D3DXCOLOR &col);

public: /* Collision */
	/*
		当たり判定
		OBJ_TYPE myType ---> 自身のタイプ
		OBJ_TYPE targetType ---> 判定を取る対象のタイプ
	*/
	bool Collision(OBJ_TYPE myType, OBJ_TYPE targetType);

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXMATRIX	m_mtxWorld;				//ワールドマトリックス
	CTexture::TEXTURE m_texture;		//テクスチャの列挙型

	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_size;	//サイズ
};

#endif