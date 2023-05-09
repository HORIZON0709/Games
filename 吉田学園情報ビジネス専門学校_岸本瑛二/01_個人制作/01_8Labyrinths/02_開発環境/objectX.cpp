//================================================
//
//3Dゲーム(仮)[objectX.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "objectX.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//静的メンバ変数
//***************************
char* CObjectX::m_pFileName = nullptr;	//ファイル名

//================================================
//生成
//================================================
CObjectX* CObjectX::Create(char* pFileName)
{
	CObjectX* pObjectX = nullptr;	//ポインタ

	if (pObjectX != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pObjectX = new CObjectX;	//メモリの動的確保

	SetFileName(pFileName);	//ファイル名の設定

	pObjectX->Init();	//初期化

	return pObjectX;	//動的確保したものを返す
}

//================================================
//ファイル名の設定
//================================================
void CObjectX::SetFileName(char* pFileName)
{
	m_pFileName = pFileName;
}

//================================================
//コンストラクタ
//================================================
CObjectX::CObjectX() :CObject::CObject(CObject::PRIORITY::PRIO_MODEL),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//メンバ変数のクリア
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::MODEL);
}

//================================================
//デストラクタ
//================================================
CObjectX::~CObjectX()
{
}

//================================================
//初期化
//================================================
HRESULT CObjectX::Init()
{
	//メンバ変数の初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(
		m_pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_numMat,
		&m_pMesh);

	//頂点数の取得
	int nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	DWORD sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	BYTE *pVtxBuff;	//頂点バッファへのポインタ

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int j = 0; j < nNumVtx; j++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		/* 頂点座標を比較し、モデルの最大値・最小値を取得 */

		//***** 最大 *****//

		if (vtx.x > m_vtxMax.x)
		{//X
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y > m_vtxMax.y)
		{//Y
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z > m_vtxMax.z)
		{//Z
			m_vtxMax.z = vtx.z;
		}

		//***** 最小 *****//

		if (vtx.x < m_vtxMin.x)
		{//X
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y < m_vtxMin.y)
		{//Y
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z < m_vtxMin.z)
		{//Z
			m_vtxMin.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//================================================
//終了
//================================================
void CObjectX::Uninit()
{
	//メッシュの解放
	if (m_pMesh != nullptr)
	{
		m_pMesh = nullptr;
	}

	//マテリアルの解放
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat = nullptr;
	}
}

//================================================
//更新
//================================================
void CObjectX::Update()
{
}

//================================================
//描画
//================================================
void CObjectX::Draw()
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL* pMat;		//マテリアルデータへのポインタ

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを保持
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)m_numMat; i++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//モデルパーツの描画
		m_pMesh->DrawSubset(i);
	}

	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//================================================
//位置を設定
//================================================
void CObjectX::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//================================================
//位置を取得
//================================================
D3DXVECTOR3 CObjectX::GetPos()
{
	return m_pos;
}

//================================================
//移動量を設定
//================================================
void CObjectX::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//================================================
//移動量を取得
//================================================
D3DXVECTOR3 CObjectX::GetMove()
{
	return m_move;
}

//================================================
//向きを設定
//================================================
void CObjectX::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//================================================
//向きを取得
//================================================
D3DXVECTOR3 CObjectX::GetRot()
{
	return m_rot;
}

//================================================
//頂点の最大値を取得
//================================================
D3DXVECTOR3 CObjectX::GetVtxMax()
{
	return m_vtxMax;
}

//================================================
//頂点の最小値を取得
//================================================
D3DXVECTOR3 CObjectX::GetVtxMin()
{
	return m_vtxMin;
}
