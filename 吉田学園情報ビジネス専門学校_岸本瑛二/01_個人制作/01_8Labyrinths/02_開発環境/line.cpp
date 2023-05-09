//================================================
//
//3Dゲーム(仮)[line.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "line.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//================================================
//生成
//================================================
CLine* CLine::Create()
{
	CLine* pLine = nullptr;	//ポインタ

	if (pLine != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pLine = new CLine;	//メモリの動的確保

	pLine->Init();	//初期化

	return pLine;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CLine::CLine() :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_start(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_end(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_col(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
{
	//メンバ変数のクリア
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
}

//================================================
//デストラクタ
//================================================
CLine::~CLine()
{
}

//================================================
//初期化
//================================================
HRESULT CLine::Init()
{
	//メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_LINE) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_LINE* pVtx = nullptr;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//================================================
//終了
//================================================
void CLine::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{//頂点バッファの解放(破棄)
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//================================================
//更新
//================================================
void CLine::Update()
{
}

//================================================
//描画
//================================================
void CLine::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_LINE));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_LINE);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_LINELIST,	//プリミティブの種類
		0,				//描画する最初の頂点インデックス
		1);				//描画するプリミティブ数

	//ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================================
//設定
//================================================
void CLine::Set(
	const D3DXVECTOR3 &pos,
	const D3DXVECTOR3 &rot,
	const D3DXVECTOR3 &start,
	const D3DXVECTOR3 &end,
	const D3DXCOLOR &col)
{
	/* 各情報の設定 */
	m_pos = pos;		//位置
	m_rot = rot;		//向き
	m_start = start;	//始点
	m_end = end;		//終点
	m_col = col;		//色

	VERTEX_LINE* pVtx = nullptr;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}