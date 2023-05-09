//================================================
//
//3Dゲーム(仮)[light.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "light.h"
#include "application.h"
#include "renderer.h"

//================================================
//コンストラクタ
//================================================
CLight::CLight()
{
	//メンバ変数をクリア
	memset(m_aLight, 0, sizeof(m_aLight));
}

//================================================
//デストラクタ
//================================================
CLight::~CLight()
{
}

//================================================
//初期化
//================================================
void CLight::Init()
{
	/* ライト1 */

	//拡散光と方向の設定用
	D3DXCOLOR diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 vecDir = D3DXVECTOR3(1.0f, -0.8f, 0.0f);

	//設定
	Set(0, diffuse, vecDir);

	/* ライト2 */

	vecDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//方向の設定

	//設定
	Set(1, diffuse, vecDir);
}

//================================================
//終了
//================================================
void CLight::Uninit()
{
}

//================================================
//更新
//================================================
void CLight::Update()
{
}

//================================================
//設定
//================================================
void CLight::Set(const int &nNum, const D3DXCOLOR &Diffuse, const D3DXVECTOR3 &vecDirection)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;	//ライトの方向ベクトル

	//ライトの種類を設定
	m_aLight[nNum].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_aLight[nNum].Diffuse = Diffuse;

	//ライトの方向を設定
	vecDir = vecDirection;

	//正規化する(大きさ1のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);

	//ライトの方向を決定
	m_aLight[nNum].Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(nNum, &m_aLight[nNum]);

	//ライトを有効にする
	pDevice->LightEnable(nNum, TRUE);
}