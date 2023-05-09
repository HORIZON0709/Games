//================================================
//
//3Dゲーム(仮)[gimmick.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "gimmick.h"
#include "application.h"
#include "renderer.h"
#include "line.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const float CGimmick::MOVE_SPEED = 1.5f;		//移動速度
const float CGimmick::ROT_SMOOTHNESS = 0.5f;	//回転の滑らかさ
const float CGimmick::ROTATION_SPEED = 0.25f;	//回転速度

//================================================
//生成
//================================================
CGimmick* CGimmick::Create(char* pFileName)
{
	CGimmick* pGimmick = nullptr;	//ポインタ

	if (pGimmick != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pGimmick = new CGimmick;	//メモリの動的確保

	pGimmick->SetFileName(pFileName);	//ファイル名の設定

	pGimmick->Init();	//初期化

	return pGimmick;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CGimmick::CGimmick() :CObjectX::CObjectX(),
	m_vec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_quaternion(D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f))
{
	//メンバ変数のクリア
	memset(m_apLine, 0, sizeof(m_apLine));
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));

	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::GIMMICK);
}

//================================================
//デストラクタ
//================================================
CGimmick::~CGimmick()
{
}

//================================================
//初期化
//================================================
HRESULT CGimmick::Init()
{
	//親クラスの初期化
	CObjectX::Init();

	//メンバ変数の初期化
	m_vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

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
void CGimmick::Uninit()
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
void CGimmick::Update()
{
	//親クラスの更新
	CObjectX::Update();

	//ラインの設定まとめ
	SetLines();
}

//================================================
//描画
//================================================
void CGimmick::Draw()
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
void CGimmick::SetLines()
{
	//各情報を取得
	D3DXVECTOR3 pos = CObjectX::GetPos();		//位置
	D3DXVECTOR3 rot = CObjectX::GetRot();		//向き
	D3DXVECTOR3 vtxMax = CObjectX::GetVtxMax();	//頂点の最大値
	D3DXVECTOR3 vtxMin = CObjectX::GetVtxMin();	//頂点の最小値

	//色(全ての線で同じ色)
	D3DXCOLOR col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

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