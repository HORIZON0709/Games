//================================================
//
//3Dゲーム(仮)[line.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _LINE_H_
#define _LINE_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//定数の定義
//***************************
const DWORD FVF_VERTEX_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);	//頂点フォーマット

//***************************
//ラインクラスの定義
//***************************
class CLine
{/* 基本クラス */
public: /* 構造体の定義 */
	struct VERTEX_LINE
	{//頂点データ
		D3DXVECTOR3 pos;	//頂点座標
		D3DCOLOR	col;	//頂点カラー
	};

public: /* 静的メンバ関数 */
	static CLine* Create();	//生成

public: /* コンストラクタ・デストラクタ */
	CLine();
	~CLine();

public: /* オーバーライド関数 */
	HRESULT Init();	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

public: /* メンバ関数 */
	/*
		設定
		const D3DXVECTOR3 &pos ---> 位置
		const D3DXVECTOR3 &rot ---> 向き
		const D3DXVECTOR3 &start ---> 始点
		const D3DXVECTOR3 &end ---> 終点
		const D3DXCOLOR &col ---> 色
	*/
	void Set(
		const D3DXVECTOR3 &pos,
		const D3DXVECTOR3 &rot,
		const D3DXVECTOR3 &start,
		const D3DXVECTOR3 &end,
		const D3DXCOLOR &col);

private: /* メンバ変数 */
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_start;	//始点
	D3DXVECTOR3 m_end;		//終点

	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス

	D3DXCOLOR m_col;	//色
};
#endif