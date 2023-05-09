//================================================
//
//3Dゲーム(仮)[object2D.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "object2D.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//================================================
//生成
//================================================
CObject2D* CObject2D::Create()
{
	CObject2D* pObject2D = nullptr;	//ポインタ

	if (pObject2D != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pObject2D = new CObject2D;	//メモリの動的確保

	pObject2D->Init();	//初期化

	return pObject2D;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CObject2D::CObject2D() : CObject::CObject(CObject::PRIORITY::PRIO_POLYGON_2D),
	m_pTexture(nullptr),
	m_pVtxBuff(nullptr),
	m_texture(CTexture::NONE),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR2(0.0f,0.0f)),
	m_col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
	m_bDraw(false)
{
	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::POLYGON_2D);
}

//================================================
//コンストラクタ(デフォルト引数ver.)
//================================================
//CObject2D::CObject2D(const int &nPriority /* = 3 */) : CObject(nPriority)
//{
//
//}

//================================================
//デストラクタ
//================================================
CObject2D::~CObject2D()
{
	/* 解放漏れの確認 */
	assert(m_pTexture == nullptr);
	assert(m_pVtxBuff == nullptr);
}

//================================================
//初期化
//================================================
HRESULT CObject2D::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//メンバ変数の初期設定
	m_texture = CTexture::NONE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDraw = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================
//終了
//================================================
void CObject2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//================================================
//更新
//================================================
void CObject2D::Update()
{
}

//================================================
//描画
//================================================
void CObject2D::Draw()
{
	if (!m_bDraw)
	{//描画しない場合
		return;
	}

	/* 描画する場合 */

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	CTexture* pTexture = CApplication::GetTexture();	//テクスチャを取得

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,					//描画する最初の頂点インデックス
							2);					//描画するプリミティブ数
}

//================================================
//位置を設定
//================================================
void CObject2D::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;	//位置を設定

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthHalf = (m_size.x * 0.5f);	//横幅の半分
	float fHeightHalf = (m_size.y * 0.5f);	//縦幅の半分

	//頂点情報を設定
	pVtx[0].pos = m_pos + D3DXVECTOR3(-fWidthHalf, -fHeightHalf, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(+fWidthHalf, -fHeightHalf, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(-fWidthHalf, +fHeightHalf, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(+fWidthHalf, +fHeightHalf, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//位置を取得
//================================================
D3DXVECTOR3 CObject2D::GetPos()
{
	return m_pos;
}

//================================================
//サイズの設定
//================================================
void CObject2D::SetSize(const D3DXVECTOR2 &size)
{
	m_size = size;	//サイズの設定

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthHalf = (m_size.x * 0.5f);	//横幅の半分
	float fHeightHalf = (m_size.y * 0.5f);	//縦幅の半分

	//頂点情報を設定
	pVtx[0].pos = m_pos + D3DXVECTOR3(-fWidthHalf, -fHeightHalf, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(+fWidthHalf, -fHeightHalf, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(-fWidthHalf, +fHeightHalf, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(+fWidthHalf, +fHeightHalf, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//サイズの取得
//================================================
D3DXVECTOR2 CObject2D::GetSize()
{
	return m_size;
}

//================================================
//移動量の設定
//================================================
void CObject2D::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//================================================
//移動量の取得
//================================================
D3DXVECTOR3 CObject2D::GetMove()
{
	return m_move;
}

//================================================
//色の設定
//================================================
void CObject2D::SetCol(const D3DXCOLOR &col)
{
	m_col = col;	//色を設定

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報を設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//色の設定
//================================================
D3DXCOLOR CObject2D::GetCol()
{
	return m_col;
}

//================================================
//テクスチャの設定
//================================================
void CObject2D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//================================================
//テクスチャ座標の設定(アニメーションに対応)
//================================================
void CObject2D::SetTexUV(const int nDivNum, const int nPtnAnim)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

 	float fDivide = (1.0f / nDivNum);	//乗算用にfloatに変換

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(fDivide * nPtnAnim,		0.0f);
	pVtx[1].tex = D3DXVECTOR2(fDivide * (nPtnAnim + 1), 0.0f);
 	pVtx[2].tex = D3DXVECTOR2(fDivide * nPtnAnim,		1.0f);
	pVtx[3].tex = D3DXVECTOR2(fDivide * (nPtnAnim + 1), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//テクスチャの取得
//================================================
CTexture::TEXTURE CObject2D::GetTexture()
{
	return m_texture;
}

//================================================
//描画するかどうかの設定
//================================================
void CObject2D::SetIsDraw(bool bDraw)
{
	m_bDraw = bDraw;
}