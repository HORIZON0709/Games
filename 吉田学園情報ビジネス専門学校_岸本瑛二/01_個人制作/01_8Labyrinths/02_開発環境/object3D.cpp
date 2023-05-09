//================================================
//
//3Dゲーム(仮)[object3D.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "object3D.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//================================================
//生成
//================================================
CObject3D* CObject3D::Create()
{
	CObject3D* pObject3D = nullptr;	//ポインタ

	if (pObject3D != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pObject3D = new CObject3D;	//メモリの動的確保

	pObject3D->Init();	//初期化

	return pObject3D;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CObject3D::CObject3D() : CObject::CObject(CObject::PRIORITY::PRIO_POLYGON_3D),
	m_pTexture(nullptr),
	m_pVtxBuff(nullptr),
	m_texture(CTexture::NONE),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//メンバ変数のクリア
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::POLYGON_3D);
}

//================================================
//デストラクタ
//================================================
CObject3D::~CObject3D()
{
	/* 解放漏れの確認 */
	assert(m_pTexture == nullptr);
	assert(m_pVtxBuff == nullptr);
}

//================================================
//初期化
//================================================
HRESULT CObject3D::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//メンバ変数の初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::NONE;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
void CObject3D::Uninit()
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
void CObject3D::Update()
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	D3DXCOLOR col = pVtx[0].col;
	col = pVtx[1].col;
	col = pVtx[2].col;
	col = pVtx[3].col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//描画
//================================================
void CObject3D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ライトを切る
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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CApplication::GetTexture();	//テクスチャを取得

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,					//描画する最初の頂点インデックス
							2);					//描画するプリミティブ数

	//ライトを付ける
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//テクスチャの設定
	pDevice->SetTexture(0, nullptr);
}

//================================================
//位置を設定
//================================================
void CObject3D::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//================================================
//位置を取得
//================================================
D3DXVECTOR3 CObject3D::GetPos()
{
	return m_pos;
}

//================================================
//向きを設定
//================================================
void CObject3D::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//================================================
//向きを取得
//================================================
D3DXVECTOR3 CObject3D::GetRot()
{
	return m_rot;
}

//================================================
//サイズの設定
//================================================
void CObject3D::SetSize(const D3DXVECTOR3 &size)
{
	m_size = size;	//サイズを設定

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//サイズを半分にする
	D3DXVECTOR3 sizeHalf = D3DXVECTOR3((size.x * 0.5f), (size.y * 0.5f), (size.z * 0.5f));

	//頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-sizeHalf.x, +sizeHalf.y, +sizeHalf.z);
	pVtx[1].pos = D3DXVECTOR3(+sizeHalf.x, +sizeHalf.y, +sizeHalf.z);
	pVtx[2].pos = D3DXVECTOR3(-sizeHalf.x, -sizeHalf.y, -sizeHalf.z);
	pVtx[3].pos = D3DXVECTOR3(+sizeHalf.x, -sizeHalf.y, -sizeHalf.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//サイズの取得
//================================================
D3DXVECTOR3 CObject3D::GetSize()
{
	return m_size;
}

//================================================
//移動量の設定
//================================================
void CObject3D::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//================================================
//移動量の取得
//================================================
D3DXVECTOR3 CObject3D::GetMove()
{
	return m_move;
}

//================================================
//テクスチャの設定
//================================================
void CObject3D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//================================================
//テクスチャ座標の設定(テクスチャアニメーション用)
//================================================
void CObject3D::SetTexUV(int nDivNumU, int nDivNumV, int nPtnAnim)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//乗算用にfloatに変換
 	float fDivideU = (1.0f / nDivNumU);	//U方向
	float fDivideV = (1.0f / nDivNumV);	//V方向

	//V座標が何段目か
	int nNumV = (nPtnAnim / nDivNumU);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(fDivideU * nPtnAnim,		 fDivideV * nNumV);
	pVtx[1].tex = D3DXVECTOR2(fDivideU * (nPtnAnim + 1), fDivideV * nNumV);
 	pVtx[2].tex = D3DXVECTOR2(fDivideU * nPtnAnim,		 fDivideV * (nNumV + 1));
	pVtx[3].tex = D3DXVECTOR2(fDivideU * (nPtnAnim + 1), fDivideV * (nNumV + 1));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//テクスチャ座標の設定(スクロール用)
//================================================
void CObject3D::SetTexUV(bool bDirection, float fMove)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!bDirection)
	{//X軸のスクロールをする場合
		//テクスチャ座標の設定
		pVtx[0].tex.x += fMove;
		pVtx[1].tex.x += fMove;
		pVtx[2].tex.x += fMove;
		pVtx[3].tex.x += fMove;

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
		return;
	}
	
	/* Y軸のスクロールをする場合 */

	//テクスチャ座標の設定
	pVtx[0].tex.y += fMove;
	pVtx[1].tex.y += fMove;
	pVtx[2].tex.y += fMove;
	pVtx[3].tex.y += fMove;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//色の設定
//================================================
void CObject3D::SetCol(const D3DXCOLOR &col)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//当たり判定
//================================================
bool CObject3D::Collision(OBJ_TYPE myType, OBJ_TYPE targetType)
{
	//for (int i = 0; i < MAX_OBJECT; i++)
	//{
	//	CObject* pObjectTarget = GetObjects(i);	//対象の情報の取得

	//	if (pObjectTarget == nullptr || pObjectTarget == this)
	//	{//NULLチェック
	//		continue;
	//	}

	//	/* 「nullptrではない」かつ「対象と自身が同じではない」場合 */

	//	CObject::OBJ_TYPE typeTarget = pObjectTarget->GetObjType();	//タイプの取得

	//	if (!(((this->GetObjType() == myType) && (typeTarget == targetType))))
	//	{//「自身のタイプ」と「対象のタイプ」が引数と一致しない場合
	//		continue;
	//	}

	//	/* 「自身のタイプ」と「対象のタイプ」が引数と一致した場合 */

	//	//自身の情報をそれぞれ取得
	//	D3DXVECTOR3 posMyself = this->GetPos();		//位置
	//	D3DXVECTOR2 sizeMyself = this->GetSize();	//サイズ

	//	/* 自身の判定用 */
	//	float fLeft		= (posMyself.x - (sizeMyself.x * 0.5f));	//左端
	//	float fRight	= (posMyself.x + (sizeMyself.x * 0.5f));	//右端
	//	float fTop		= (posMyself.y + (sizeMyself.y * 0.5f));	//上端
	//	float fBottom	= (posMyself.y - (sizeMyself.y * 0.5f));	//下端

	//	CObject3D* pObjTarget3D = (CObject3D*)pObjectTarget;	//Object3D型にキャスト

	//	//対象の情報をそれぞれ取得
	//	D3DXVECTOR3 posTarget = pObjTarget3D->GetPos();	//位置
	//	D3DXVECTOR2 sizeTarget = pObjTarget3D->GetSize();	//サイズ

	//	/* 対象の判定用 */
	//	float fLeftTarget	= (posTarget.x - (sizeTarget.x * 0.5f));	//左端
	//	float fRightTarget	= (posTarget.x + (sizeTarget.x * 0.5f));	//右端
	//	float fTopTarget	= (posTarget.y + (sizeTarget.y * 0.5f));	//上端
	//	float fBottomTarget = (posTarget.y - (sizeTarget.y * 0.5f));	//下端

	//	if (fLeft < fRightTarget
	//		&& fRight > fLeftTarget
	//		&& fTop > fBottomTarget
	//		&& fBottom < fTopTarget)
	//	{//「自身」が「対象の範囲内」に来た場合
	//		return true;	//「当たった」を返す
	//	}
	//}

	return false;	//「当たっていない」を返す
}