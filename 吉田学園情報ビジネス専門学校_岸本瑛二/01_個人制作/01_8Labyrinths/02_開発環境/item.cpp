//================================================
//
//3Dゲーム(仮)[item.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "item.h"
#include "application.h"
#include "renderer.h"
#include "line.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const float CItem::ROTATION_SPEED = 0.05f;	//回転速度

//================================================
//生成
//================================================
CItem* CItem::Create(char* pFileName)
{
	CItem* pItem = nullptr;	//ポインタ

	if (pItem != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pItem = new CItem;	//メモリの動的確保

	pItem->SetFileName(pFileName);	//ファイル名の設定

	pItem->Init();	//初期化

	return pItem;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CItem::CItem() :CObjectX::CObjectX(),
	m_isStage(CStage::STAGE::NONE),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//メンバ変数のクリア
	memset(m_apLine, 0, sizeof(m_apLine));
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::ITEM);
}

//================================================
//デストラクタ
//================================================
CItem::~CItem()
{
}

//================================================
//初期化
//================================================
HRESULT CItem::Init()
{
	//親クラスの初期化
	CObjectX::Init();

	//メンバ変数の初期化
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < MAX_LINE; i++)
	{
		//ラインの生成
		m_apLine[i] = CLine::Create();
	}

	return S_OK;
}

//================================================
//終了
//================================================
void CItem::Uninit()
{
	for (int i = 0; i < MAX_LINE; i++)
	{//ライン
		if (m_apLine[i] != nullptr)
		{
			m_apLine[i]->Uninit();	//終了
			delete m_apLine[i];		//メモリの解放
			m_apLine[i] = nullptr;	//nullptrにする
		}
	}

	//親クラスの終了
	CObjectX::Uninit();
}

//================================================
//更新
//================================================
void CItem::Update()
{
	//親クラスの更新
	CObjectX::Update();

	//回転
	m_rot.y += ROTATION_SPEED;

	//角度の正規化
	NormalizeAngle(&m_rot.y);

	//向きを設定
	CObjectX::SetRot(m_rot);

#ifdef _DEBUG
	//各情報を表示
	CDebugProc::Print("\n《 Item 》\n");
	CDebugProc::Print("m_rot:[%f,%f,%f]\n", m_rot.x, m_rot.y, m_rot.z);

	//ラインの設定まとめ
	SetLines();

#endif // _DEBUG
}

//================================================
//描画
//================================================
void CItem::Draw()
{
	//親クラスの描画
	CObjectX::Draw();

	for (int i = 0; i < MAX_LINE; i++)
	{
		//ラインの描画
		m_apLine[i]->Draw();
	}
}

//================================================
//ラインの設定まとめ
//================================================
void CItem::SetLines()
{
	//各情報を取得
	D3DXVECTOR3 pos = CObjectX::GetPos();		//位置
	D3DXVECTOR3 rot = CObjectX::GetRot();		//向き
	D3DXVECTOR3 vtxMax = CObjectX::GetVtxMax();	//頂点の最大値
	D3DXVECTOR3 vtxMin = CObjectX::GetVtxMin();	//頂点の最小値

	//色(全ての線で同じ色)
	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//何番目か
	int nNum = 0;

	//********** 上部左側 **********//

	//始点・終点
	D3DXVECTOR3 start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	D3DXVECTOR3 end = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 上部手前側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 上部右側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 上部奥側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部左側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部手前側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部右側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部奥側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 手前左側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 手前右側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 奥左側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 奥右側 **********//

	//始点・終点
	start = D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z);
	end = D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z);

	//設定
	m_apLine[nNum]->Set(pos, rot, start, end, col);

	nNum++;	//次に進める
}

//================================================
//どのステージにあるかを設定
//================================================
void CItem::SetIsStage(CStage::STAGE isStage)
{
	m_isStage = isStage;
}