//================================================
//
//3Dゲーム(仮)[camera.cpp]
//Author:Kishimoto Eiji
//
//================================================
#include "camera.h"
#include "application.h"
#include "inputKeyboard.h"
#include "renderer.h"
#include "game.h"
#include "fade.h"
#include "player.h"

#include "debug_proc.h"

//***************************
//定数の定義
//***************************
const float CCamera::MOVE_SPEED = 1.5f;		//移動速度

//================================================
//コンストラクタ
//================================================
CCamera::CCamera():
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vecU(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	//メンバ変数のクリア
	memset(m_mtxProjection,0,sizeof(m_mtxProjection));
	memset(m_mtxView, 0, sizeof(m_mtxView));
}

//================================================
//デストラクタ
//================================================
CCamera::~CCamera()
{}

//================================================
//初期化
//================================================
void CCamera::Init()
{
	//メンバ変数の初期設定
	m_posV = D3DXVECTOR3(-50.0f, 100.0f, -300.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Set();	//カメラをセット
}

//================================================
//終了
//================================================
void CCamera::Uninit()
{
}

//================================================
//更新
//================================================
void CCamera::Update()
{
	if (CApplication::GetFade()->GetState() == CFade::STATE::FADE_OUT)
	{//暗転中
		return;
	}

#ifdef _DEBUG	//デバッグ時のみ

	//移動
	Move();

#endif

	//注視点の位置をプレイヤーの位置に合わせる
	m_posR = CGame::GetPlayer()->GetPos();

	//視点を指定の位置に固定する
	m_posV = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, 110.0f, -200.0f);

	CDebugProc::Print("\n《 Camera 》\n");
	CDebugProc::Print("m_posV:[%f,%f,%f]\n", m_posV.x, m_posV.y, m_posV.z);

	if ((m_move.x == 0.0f) && (m_move.y == 0.0f) && (m_move.z == 0.0f))
	{//移動していなかったら
		return;
	}

	/* 移動していたら */

	//移動量に応じて位置を更新(視点)
	//m_posV.x += m_move.x * MOVE_SPEED;	//X軸
	//m_posV.y += m_move.y * MOVE_SPEED;	//Y軸
	//m_posV.z += m_move.z * MOVE_SPEED;	//Z軸

	//移動量に応じて位置を更新(注視点)
	//m_posR.x += m_move.x * MOVE_SPEED;	//X軸
	//m_posR.y += m_move.y * MOVE_SPEED;	//Y軸
	//m_posR.z += m_move.z * MOVE_SPEED;	//Z軸

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量をリセット
}

//================================================
//設定
//================================================
void CCamera::Set()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);  

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),
								(float)CRenderer::SCREEN_WIDTH / (float)CRenderer::SCREEN_HEIGHT,
								10.0f,
								3000.0f);

	// プロジェクションマトリックスの作成(平行投影)
	/*D3DXMatrixOrthoLH(&m_mtxProjection,						//プロジェクションマトリックス
						(float)CRenderer::SCREEN_WIDTH,		//幅
						(float)CRenderer::SCREEN_HEIGHT,	//高さ
						10.0f,								//ニア
						3000.0f);							//ファー*/

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//================================================
//ビューマトリクスの取得
//================================================
D3DXMATRIX CCamera::GetMatrixView()
{
	return m_mtxView;
}

//================================================
//視点の位置の取得
//================================================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//================================================
//移動
//================================================
void CCamera::Move()
{
	//キーボード情報の取得
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	/* 前後・左右・上下 */

	if (pKeyboard->GetPress(DIK_A))
	{//←キー
		/* 移動方向に応じて移動量を加算 */

		if (pKeyboard->GetPress(DIK_W))
		{//左上
			m_move.x = -1.0f;	//X軸
			m_move.z = +1.0f;	//Z軸
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//左下
			m_move.x = -1.0f;	//X軸
			m_move.z = -1.0f;	//Z軸
		}
		else
		{//左
			m_move.x = -1.0f;	//X軸
		}
	}
	else if (pKeyboard->GetPress(DIK_D))
	{//→キー
		/* 方向に応じて移動量を加算 */
		
		if (pKeyboard->GetPress(DIK_W))
		{//右上
			m_move.x = +1.0f;	//X軸
			m_move.z = +1.0f;	//Z軸
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//右下
			m_move.x = +1.0f;	//X軸
			m_move.z = -1.0f;	//Z軸
		}
		else
		{//右
			m_move.x = +1.0f;	//X軸
		}
	}
	else if (pKeyboard->GetPress(DIK_W))
	{//↑キー(上)
		m_move.z = +1.0f;	//Z軸
	}
	else if (pKeyboard->GetPress(DIK_S))
	{//↓キー(下)
		m_move.z = -1.0f;	//Z軸
	}
	else if (pKeyboard->GetPress(DIK_O))
	{//Oキー
		m_move.y = +1.0f;	//Y軸
	}
	else if (pKeyboard->GetPress(DIK_L))
	{//Lキー
		m_move.y = -1.0f;	//Y軸
	}
}