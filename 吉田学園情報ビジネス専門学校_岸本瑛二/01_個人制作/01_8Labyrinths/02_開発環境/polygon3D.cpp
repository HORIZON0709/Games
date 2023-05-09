//================================================
//
//3Dゲーム(仮)[polygon3D.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "polygon3D.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const float CPolygon3D::POLYGON_WIDTH = 300.0f;		//横幅
const float CPolygon3D::POLYGON_HEIGHT = 300.0f;	//縦幅

//================================================
//生成
//================================================
CPolygon3D* CPolygon3D::Create()
{
	CPolygon3D* pPolygon3D = nullptr;	//ポインタ

	if (pPolygon3D != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pPolygon3D = new CPolygon3D;	//メモリの動的確保

	pPolygon3D->Init();	//初期化

	return pPolygon3D;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CPolygon3D::CPolygon3D()
{
	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::BG);
}

//================================================
//デストラクタ
//================================================
CPolygon3D::~CPolygon3D()
{
}

//================================================
//初期化
//================================================
HRESULT CPolygon3D::Init()
{
	CObject3D::Init();	//親クラス

	//位置を設定
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CObject3D::SetPos(pos);

	//サイズを設定
	D3DXVECTOR3 size = D3DXVECTOR3(POLYGON_WIDTH, 0.0f, POLYGON_HEIGHT);
	CObject3D::SetSize(size);

	D3DXCOLOR col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	CObject3D::SetCol(col);

	return S_OK;
}

//================================================
//終了
//================================================
void CPolygon3D::Uninit()
{
	CObject3D::Uninit();	//親クラス
}

//================================================
//更新
//================================================
void CPolygon3D::Update()
{
	CObject3D::Update();	//親クラス
}

//================================================
//描画
//================================================
void CPolygon3D::Draw()
{
	CObject3D::Draw();	//親クラス
}