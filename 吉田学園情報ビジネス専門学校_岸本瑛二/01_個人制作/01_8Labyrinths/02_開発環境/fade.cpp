//================================================
//
//3Dゲーム(仮)[fade.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "fade.h"
#include "application.h"
#include "renderer.h"
#include "mode.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const DWORD CFade::FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//頂点フォーマット

const float CFade::FADE_SPEED = 0.02f;	//フェードの速度

//================================================
//生成
//================================================
CFade* CFade::Create()
{
	CFade* pFade = nullptr;	//ポインタ

	if (pFade != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pFade = new CFade;	//メモリの動的確保

	pFade->Init();	//初期化

	return pFade;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CFade::CFade() :
	m_pVtxBuff(nullptr),
	m_state(STATE::NONE),
	m_col(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
{
}

//================================================
//デストラクタ
//================================================
CFade::~CFade()
{
	/* 解放漏れの確認 */
	assert(m_pVtxBuff == nullptr);
}

//================================================
//初期化
//================================================
HRESULT CFade::Init()
{
	//メンバ変数の初期化
	m_state = STATE::NONE;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

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

	float fWidth = (float)(CRenderer::SCREEN_WIDTH);	//横幅
	float fHeight = (float)(CRenderer::SCREEN_HEIGHT);	//縦幅

	//頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
void CFade::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//================================================
//更新
//================================================
void CFade::Update()
{
	if (m_state == STATE::NONE)
	{//フェードしていない場合
		return;
	}

	/* フェードしているとき */

	if (m_state == STATE::FADE_IN)
	{//フェードイン( 明転 )
		m_col.a -= FADE_SPEED;	//透明にしていく

		if (m_col.a <= 0.0f)
		{//完全に透明になったら
			m_col.a = 0.0f;			//0.0にする
			m_state = STATE::NONE;	//フェードしていない状態にする
		}
	}
	else if (m_state == STATE::FADE_OUT)
	{//フェードアウト( 暗転 )
		m_col.a += FADE_SPEED;	//不透明にしていく

		if (m_col.a >= 1.0f)
		{//完全に不透明になったら
			m_col.a = 1.0f;			//1.0にする
			m_state = STATE::NONE;	//フェードしていない状態にする
		}
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//描画
//================================================
void CFade::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, nullptr);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,					//描画する最初の頂点インデックス
							2);					//描画するプリミティブ数
}

//================================================
//設定
//================================================
void CFade::Set(const STATE &state)
{
	m_state = state;	//フェード状態を設定
}

//================================================
//フェード状態の取得
//================================================
CFade::STATE CFade::GetState()
{
	return m_state;
}