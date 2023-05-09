//================================================
//
//3Dゲーム(仮)[score.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "score.h"
#include "number.h"

#include "application.h"
#include "input.h"

#include <math.h>
#include <assert.h>

//================================================
//生成
//================================================
CScore* CScore::Create()
{
	CScore* pScore = nullptr;	//ポインタ

	if (pScore != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pScore = new CScore;	//メモリの動的確保

	pScore->Init();	//初期化

	return pScore;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CScore::CScore():CObject::CObject(CObject::PRIORITY::PRIO_POLYGON_2D),
	m_pNumber(nullptr),
	m_nScore(0)
{
	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::SCORE);
}

//================================================
//デストラクタ
//================================================
CScore::~CScore()
{
}

//================================================
//初期化
//================================================
HRESULT CScore::Init()
{
	if (m_pNumber != nullptr)
	{//NULLチェック
		m_pNumber = nullptr;
	}

	/* nullptrの場合 */

	m_pNumber = CNumber::Create();	//メモリの動的確保

	//位置の設定
	m_pNumber->SetPos(D3DXVECTOR3(CNumber::POS_X, CNumber::POS_Y, 0.0f));

	//サイズの設定
	m_pNumber->SetSize(D3DXVECTOR2(CNumber::NUMBER_WIDTH, CNumber::NUMBER_HEIGHT));

	//テクスチャの設定
	m_pNumber->SetTexture(CTexture::TEXTURE::Number00);

	//テクスチャ座標の設定
	m_pNumber->SetTexUV(CNumber::DIVIDE_TEX_U, 0);

	return S_OK;
}

//================================================
//終了
//================================================
void CScore::Uninit()
{
}

//================================================
//更新
//================================================
void CScore::Update()
{
	//テクスチャ座標の設定
	m_pNumber->SetTexUV(CNumber::DIVIDE_TEX_U, m_nScore);
}

//================================================
//描画
//================================================
void CScore::Draw()
{
}

//================================================
//スコアのセット
//================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//================================================
//スコア加算
//================================================
void CScore::AddScore(const int nValue)
{
	m_nScore += nValue;
}

//================================================
//現在のスコアの取得
//================================================
int CScore::GetCurrentScore()
{
	return m_nScore;
}
