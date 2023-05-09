//================================================
//
//3Dゲーム(仮)[player.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "player.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "model.h"
#include "gimmick.h"
#include "line.h"
#include "item.h"
#include "fade.h"
#include "door.h"
#include "score.h"
#include "panel.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//***************************
//namespace
//***************************
namespace
{
CStage::DIRECTION dirDoor = CStage::DIRECTION::DIR_NONE;	//触れたドアの方向
}

//***************************
//定数の定義
//***************************
const float CPlayer::MOVE_SPEED = 2.5f;		//移動速度
const float CPlayer::ROT_SMOOTHNESS = 0.5f;	//回転の滑らかさ
const float CPlayer::LIMIT_POS = 315.0f;	//移動制限位置

const int CPlayer::MAX_WORD = 256;	//最大文字数

const char* CPlayer::FILE_NAME = "data/TEXT/motion.txt";	//ファイル名

//***************************
//静的メンバ変数
//***************************
CPlayer::MOTION_SET CPlayer::m_aMotion[MOTION_TYPE::MAX] ={};

CLine* CPlayer::m_apLine[MAX_LINE] = {};	//ラインのポインタ

//================================================
//生成
//================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = nullptr;	//ポインタ

	if (pPlayer != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pPlayer = new CPlayer;	//メモリの動的確保

	pPlayer->Init();	//初期化

	return pPlayer;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CPlayer::CPlayer() :CObject::CObject(CObject::PRIORITY::PRIO_MODEL),
	m_pModel(nullptr),
	m_pItem(nullptr),
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_vtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_motionType(MOTION_TYPE::NONE),
	m_nCurrentKey(0),
	m_nCntMotion(0),
	m_nNumMotion(0),
	m_nNumKeySet(0),
	m_nNumKey(0),
	m_nGetItem(0),
	m_bPressKey(false),
	m_bCollObjectX(false),
	m_bCollGimmick(false),
	m_bCollDoor(false),
	m_bCollPanelStand(false),
	m_bGetItem(false),
	m_bCanMove(false)
{
	//メンバ変数のクリア
	for (int i = 0; i < CStage::MAX_COIN; i++)
	{
		m_aStageCoin[i] = CStage::STAGE::NONE;
	}

	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
	
	for (int i = 0; i < NUM_VTX_3D; i++)
	{
		m_aPosVtx[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//タイプの設定
	CObject::SetObjType(CObject::OBJ_TYPE::PLAYER);
}

//================================================
//デストラクタ
//================================================
CPlayer::~CPlayer()
{
}

//================================================
//初期化
//================================================
HRESULT CPlayer::Init()
{
	//モデルの生成
	m_pModel = CModel::Create();

	//頂点の最大値と最小値を設定
	SetVtxMaxAndMin();

	//読み込み
	Load();

	//メンバ変数の初期化
	m_pItem = nullptr;

	for (int i = 0; i < CStage::MAX_COIN; i++)
	{
		m_aStageCoin[i] = CStage::STAGE::NONE;
	}

	m_pos = D3DXVECTOR3(0.0f, 5.0f, -150.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/*
		頂点の最大値・最小値から、モデルの範囲を表す頂点の位置を設定する
		コメント ---> 正面から見た頂点の位置
	*/
	/* 左側 */
	m_aPosVtx[0] = m_vtxMin;										//手前の下側(最小値)
	m_aPosVtx[1] = D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z);	//奥の下側
	m_aPosVtx[2] = D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z);	//手前の上側
	m_aPosVtx[3] = D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z);	//奥の上側

	/* 右側 */
	m_aPosVtx[4] = D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z);	//手前の下側
	m_aPosVtx[5] = D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMax.z);	//奥の下側
	m_aPosVtx[6] = D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z);	//手前の上側
	m_aPosVtx[7] = m_vtxMax;										//奥の上側(最大値)

	m_motionType = MOTION_TYPE::NEUTRAL;

	m_nCurrentKey = 0;
	m_nCntMotion = 0;
	m_nNumMotion = 0;
	m_nNumKeySet = 0;
	m_nNumKey = 0;
	m_nGetItem = 0;
	m_bPressKey = false;
	m_bCollObjectX = false;
	m_bCollGimmick = false;
	m_bCollDoor = false;
	m_bCollPanelStand = false;
	m_bGetItem = false;
	m_bCanMove = false;

	for (int i = 0; i < MAX_LINE; i++)
	{
		m_apLine[i] = CLine::Create();
	}

	return S_OK;
}

//================================================
//終了
//================================================
void CPlayer::Uninit()
{
	for (int i = 0; i < MAX_LINE; i++)
	{
		if (m_apLine[i] != nullptr)
		{
			m_apLine[i]->Uninit();
			delete m_apLine[i];
			m_apLine[i] = nullptr;
		}
	}
}

//================================================
//更新
//================================================
void CPlayer::Update()
{
	//移動
	Move();

	//モーション
	Motion();

	//当たり判定
	Collision();

	//ステージ切り替え
	StageChange();

	//スコア(アイテム取得数)をセット
	CGame::GetScore()->SetScore(m_nGetItem);

	if (!m_bFadeOut && m_bCollDoor && m_bCanMove)
	{//暗転していない & ドアに当たった & 移動できる
		//暗転
		CApplication::GetFade()->Set(CFade::STATE::FADE_OUT);

		//暗転した
		m_bFadeOut = true;
	}

	if (m_bFadeOut && (CApplication::GetFade()->GetState() == CFade::STATE::NONE))
	{//暗転した & 現在フェードしていない
		D3DXVECTOR3 posDoor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ドアの位置

		switch (dirDoor)
		{
		case CStage::DIRECTION::DIR_LEFT:	//左
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_RIGHT];
			posDoor.x -= 50.0f;
			break;

		case CStage::DIRECTION::DIR_BACK:	//奥
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_FRONT];
			posDoor.z += 50.0f;
			break;

		case CStage::DIRECTION::DIR_RIGHT:	//右
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_LEFT];
			posDoor.x += 50.0f;
			break;

		case CStage::DIRECTION::DIR_FRONT:	//手前
			posDoor = CStage::POS_DOOR[CStage::DIRECTION::DIR_BACK];
			posDoor.z -= 50.0f;
			break;
		}

		//位置を設定
		m_pos = posDoor;

		//移動した
		m_bCanMove = false;

		//明転した
		m_bFadeOut = false;

		//明転
		CApplication::GetFade()->Set(CFade::STATE::FADE_IN);
	}

#ifdef _DEBUG
	/* デバッグ表示 */

	//プレイヤー
	CDebugProc::Print("\n《 Player 》\n");
	CDebugProc::Print("m_pos:[%f,%f,%f]\n", m_pos.x, m_pos.y, m_pos.z);	//位置
	CDebugProc::Print("m_rot:[%f,%f,%f]\n", m_rot.x, m_rot.y, m_rot.z);	//向き
	CDebugProc::Print("m_vec:[%f,%f,%f]\n", m_vec.x, m_vec.y, m_vec.z);	//方向ベクトル

	D3DXVECTOR3 size = m_vtxMax - m_vtxMin;	//頂点の最大・最小からサイズを算出

	CDebugProc::Print("size:[%f,%f,%f]\n", size.x, size.y, size.z);	//サイズ

	if (m_bCollGimmick)
	{//対象のオブジェクトに当たっている場合
		CDebugProc::Print("Collision:[true]\n", m_bCollGimmick);
	}
	else
	{//当たっていない場合
		CDebugProc::Print("Collision:[false]\n", m_bCollGimmick);
	}

	//モデルパーツ
	CDebugProc::Print("\n《 Parts 》\n");

	CModel::MODEL_INFO aInfo[MAX_PARTS] = {};	//モデル情報

	for (int i = 0; i < MAX_PARTS; i++)
	{
		//モデル情報の取得
		aInfo[i] = m_pModel->GetModelInfo(i);

		//位置・向き
		CDebugProc::Print("pos:[%f,%f,%f]\n", aInfo[i].pos.x, aInfo[i].pos.y, aInfo[i].pos.z);
		CDebugProc::Print("rot:[%f,%f,%f]\n", aInfo[i].rot.x, aInfo[i].rot.y, aInfo[i].rot.z);
	}

	//ラインの設定まとめ
	SetLines();

#endif // _DEBUG
}

//================================================
//描画
//================================================
void CPlayer::Draw()
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

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

	//モデルの描画
	m_pModel->Draw();

	for (int i = 0; i < MAX_LINE; i++)
	{
		//ラインの描画
		m_apLine[i]->Draw();
	}
}

//================================================
//位置を取得
//================================================
D3DXVECTOR3 CPlayer::GetPos()
{
	return m_pos;
}

//================================================
//頂点の最大値を取得
//================================================
D3DXVECTOR3 CPlayer::GetVtxMax()
{
	return m_vtxMax;
}

//================================================
//頂点の最小値を取得
//================================================
D3DXVECTOR3 CPlayer::GetVtxMin()
{
	return m_vtxMin;
}

//================================================
//アイテムを取得したかどうかを取得
//================================================
bool CPlayer::GetIsGetItem()
{
	return m_bGetItem;
}

//================================================
//アイテムの獲得状況を取得
//================================================
CStage::STAGE CPlayer::GetStageCoin(int nIdx)
{
	return m_aStageCoin[nIdx];
}

//================================================
//移動
//================================================
void CPlayer::Move()
{
	//キーボード情報を取得
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	if (pKeyboard->GetPress(DIK_D))
	{//Dキー押下中
		if (pKeyboard->GetPress(DIK_W))
		{//右前
			m_vec = D3DXVECTOR3(1.0f, 0.0f, 1.0f);	//移動方向を設定
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//右後ろ
			m_vec = D3DXVECTOR3(1.0f, 0.0f, -1.0f);	//移動方向を設定
		}
		else
		{//右
			m_vec = D3DXVECTOR3(1.0f, 0.0f, 0.0f);	//移動方向を設定
		}

		//キー押下中
		m_bPressKey = true;
	}
	else if (pKeyboard->GetPress(DIK_A))
	{//Aキー押下中
		if (pKeyboard->GetPress(DIK_W))
		{//左前
			m_vec = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);	//移動方向を設定
		}
		else if (pKeyboard->GetPress(DIK_S))
		{//左後ろ
			m_vec = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);	//移動方向を設定
		}
		else
		{//左
			m_vec = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);	//移動方向を設定
		}

		//キー押下中
		m_bPressKey = true;
	}
	else if (pKeyboard->GetPress(DIK_W))
	{//Wキー押下中
		m_vec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);	//移動方向を設定

		//キー押下中
		m_bPressKey = true;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{//Sキー押下中
		m_vec = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//移動方向を設定

		//キー押下中
		m_bPressKey = true;
	}

	//********** ↓ 移動方向 ↓ **********//

	//ベクトルの大きさを1にする
	D3DXVec3Normalize(&m_vec, &m_vec);

	//2方向の単位ベクトルから角度を求める
	m_rotDest.y = atan2f(-m_vec.x, -m_vec.z);

	//現在の向きと目的の向きの差分を計算
	float fDif = m_rotDest.y - m_rot.y;

	//角度の正規化
	NormalizeAngle(&fDif);

	//現在の向きを更新
	m_rot.y += fDif * ROT_SMOOTHNESS;

	//角度の正規化
	NormalizeAngle(&m_rot.y);

	//********** ↓ 移動量 ↓ **********//

	if (!m_bPressKey)
	{//移動キーが押されていない場合
		return;
	}

	/* 移動キーが押されている場合 */

	//移動量に代入(移動ベクトル * 移動速度)
	m_move = m_vec * MOVE_SPEED;

	//現在の位置を、前回の位置として保存
	m_posOld = m_pos;

	//現在の位置に移動量を加算
	m_pos += m_move;

	if (m_pos.x < -LIMIT_POS)
	{//移動制限位置(左側)を超えたら
		m_pos.x = -LIMIT_POS;	//位置を戻す
	}
	else if (m_pos.x > LIMIT_POS)
	{//移動制限位置(右側)を超えたら
		m_pos.x = LIMIT_POS;	//位置を戻す
	}

	if (m_pos.z < -LIMIT_POS)
	{//移動制限位置(奥側)を超えたら
		m_pos.z = -LIMIT_POS;	//位置を戻す
	}
	else if (m_pos.z > LIMIT_POS)
	{//移動制限位置(手前側)を超えたら
		m_pos.z = LIMIT_POS;	//位置を戻す
	}

	//移動量を0にする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//「キー押下されていない」にする
	m_bPressKey = false;
}

//================================================
//モーション
//================================================
void CPlayer::Motion()
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (m_pModel == nullptr)
		{//NULLチェック
			continue;
		}

		/* nullptrではない場合 */

		//モーション情報
		MOTION_SET motion = m_aMotion[m_motionType];

		//相対値を計算(モーションカウンター / 再生フレーム数)
		float fRelativeValue = (float)m_nCntMotion / (float)motion.aKeySet[m_nCurrentKey].nFrame;

		//次のキー番号(計算用)
		int nNextKey = (m_nCurrentKey + 1) % motion.nNumKey;

		//差分(終了値 - 開始値)
		D3DXVECTOR3 posDif = (motion.aKeySet[nNextKey].aKey[i].pos - motion.aKeySet[m_nCurrentKey].aKey[i].pos);
		D3DXVECTOR3 rotDif = (motion.aKeySet[nNextKey].aKey[i].rot - motion.aKeySet[m_nCurrentKey].aKey[i].rot);

		//差分 * 相対値
		D3DXVECTOR3 pos = D3DXVECTOR3(	//位置
			posDif.x * fRelativeValue,
			posDif.y * fRelativeValue,
			posDif.z * fRelativeValue);

		D3DXVECTOR3 rot = D3DXVECTOR3(	//向き
			rotDif.x * fRelativeValue,
			rotDif.y * fRelativeValue,
			rotDif.z * fRelativeValue);

		//位置・向きの現在値を取得
		D3DXVECTOR3 posPre = m_pModel->GetModelInfo(i).pos;
		D3DXVECTOR3 rotPre = m_pModel->GetModelInfo(i).rot;

		//現在値に代入(開始値 + (差分 * 相対値))
		posPre = motion.aKeySet[m_nCurrentKey].aKey[i].pos + pos;
		rotPre = motion.aKeySet[m_nCurrentKey].aKey[i].rot + rot;

		//角度の正規化
		NormalizeAngle(&rotPre.x);	
		NormalizeAngle(&rotPre.y);
		NormalizeAngle(&rotPre.z);

		//位置・向きを反映
		m_pModel->SetModelInfo_Pos(i, posPre);
		m_pModel->SetModelInfo_Rot(i, rotPre);
	}

	m_nCntMotion++;	//モーションカウンターを進める

	if (m_nCntMotion < m_aMotion[m_motionType].aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレーム数に達していない場合
		return;
	}

	/* 再生フレーム数に達した場合 */

	m_nCurrentKey++;	//現在のキー番号を一つ進める
	m_nCntMotion = 0;	//モーションカウンターを初期化

	if (m_nCurrentKey == m_aMotion[m_motionType].nNumKey)
	{//現在のキー番号が、キーの総数に達したら
		m_nCurrentKey = 0;	//現在のキー番号を0に戻す
	}
}

//================================================
//当たり判定
//================================================
void CPlayer::Collision()
{
	if (CApplication::GetFade()->GetState() != CFade::STATE::NONE)
	{//フェード中の場合
		return;
	}

	/* フェードしていない場合 */

	/* Xモデル */
	for (int i = 0; i < CStage::MAX_MODEL; i++)
	{
		//対象のXモデル情報を取得
		CObjectX* pObjX = CStage::GetObjectX(i);

		if (pObjX == nullptr)
		{//NULLチェック
			continue;
		}

		/* nullptrではない場合 */

		//自身のサイズを算出
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//対象のサイズを算出
		D3DXVECTOR3 sizeTarget = (pObjX->GetVtxMax() - pObjX->GetVtxMin());

		//当たり判定
		m_bCollObjectX = CollisionModel(
			&m_pos,				//自身の現在の位置
			m_posOld,			//自身の前回の位置
			pObjX->GetPos(),	//対象の位置
			sizeOwn,			//自身のサイズ
			sizeTarget			//対象のサイズ
		);
	}

	/* ギミック */
	for (int i = 0; i < CStage::MAX_GIMMICK; i++)
	{
		//対象のギミック情報を取得
		CGimmick* pGimmick = CStage::GetGimmick(i);

		if (pGimmick == nullptr)
		{//NULLチェック
			continue;
		}

		/* nullptrではない場合 */

		//自身のサイズを算出
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//対象のサイズを算出
		D3DXVECTOR3 sizeTarget = (pGimmick->GetVtxMax() - pGimmick->GetVtxMin());

		//当たり判定
		m_bCollGimmick = CollisionModel(
			&m_pos,				//自身の現在の位置
			m_posOld,			//自身の前回の位置
			pGimmick->GetPos(),	//対象の位置
			sizeOwn,			//自身のサイズ
			sizeTarget			//対象のサイズ
		);
	}

	/* パネルスタンド */

	//パネル情報の取得
	CPanel* pPanel = CGame::GetPanel();

	if (pPanel != nullptr)
	{//nullptrではない場合
		//自身のサイズを算出
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//対象のサイズを算出
		D3DXVECTOR3 sizeTarget = (pPanel->GetPanelStand()->GetVtxMax() - pPanel->GetPanelStand()->GetVtxMin());

		//当たり判定
		m_bCollPanelStand = CollisionModel(
			&m_pos,								//自身の現在の位置
			m_posOld,							//自身の前回の位置
			pPanel->GetPanelStand()->GetPos(),	//対象の位置
			sizeOwn,							//自身のサイズ
			sizeTarget							//対象のサイズ
		);

		//当たったかどうかを送る
		pPanel->SetCollPlayer(m_bCollPanelStand);
	}

	/* ドア */
	for (int i = 0; i < CStage::MAX_DOOR; i++)
	{
		//ドア情報を取得
		CDoor* pDoor = CStage::GetDoor(i);

		if (pDoor == nullptr)
		{//NULLチェック
			continue;
		}

		/* nullptrではない場合 */

		//自身のサイズを算出
		D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

		//対象のサイズを算出
		D3DXVECTOR3 sizeTarget = (pDoor->GetVtxMax() - pDoor->GetVtxMin());

		//当たり判定
		m_bCollDoor = CollisionModel(
			&m_pos,				//自身の現在の位置
			m_posOld,			//自身の前回の位置
			pDoor->GetPos(),	//対象の位置
			sizeOwn,			//自身のサイズ
			sizeTarget			//対象のサイズ
		);

		if (m_bCollDoor)
		{//当たっていたら
			//当たったドアの方向を取得
			dirDoor = pDoor->GetDir();
			return;
		}
	}

	//アイテム情報を取得
	m_pItem = CStage::GetItem();

	if (m_pItem == nullptr)
	{//NULLチェック
		return;
	}

	//自身のサイズを算出
	D3DXVECTOR3 sizeOwn = (m_vtxMax - m_vtxMin);

	//対象のサイズを算出
	D3DXVECTOR3 sizeTarget = (m_pItem->GetVtxMax() - m_pItem->GetVtxMin());

	//当たり判定
	m_bGetItem = CollisionModel(
		&m_pos,				//自身の現在の位置
		m_posOld,			//自身の前回の位置
		m_pItem->GetPos(),	//対象の位置
		sizeOwn,			//自身のサイズ
		sizeTarget			//対象のサイズ
	);

	if (m_bGetItem)
	{//アイテムを獲得した場合
		//獲得したステージを記録
		m_aStageCoin[m_nGetItem] = CGame::GetStage()->Get();

		//アイテム獲得数を増やす
		m_nGetItem++;
	}
}

//================================================
//ステージ切り替え
//================================================
void CPlayer::StageChange()
{
	if (!m_bCollDoor)
	{//ドアに触れていない
		return;
	}

	/* ドアに当たった場合 */

	//パネル情報取得用
	CPanel::PANEL_INFO aInfo[CPanel::GRID_Y][CPanel::GRID_X] = {};

	//現在のステージを取得
	CStage* pStage = CGame::GetStage();

	int nPosX = 0, nPosY = 0;	//パネルの位置番号

	for (int Y = 0; Y < CPanel::GRID_Y; Y++)
	{
		for (int X = 0; X < CPanel::GRID_X; X++)
		{
			//パネル情報を取得
			aInfo[Y][X] = CGame::GetPanel()->GetPanelInfo(Y, X);

			if (aInfo[Y][X].stage == pStage->Get())
			{//現在のステージと同じステージ情報を持つパネルの場合
				//位置番号を保存
				nPosX = X;	//X軸
				nPosY = Y;	//Y軸
			}
		}
	}

	//移動先の番号用
	int nDestX = nPosX;	//X軸
	int nDestY = nPosY;	//Y軸

	switch (dirDoor)
	{//当たったドアの方向
	case CStage::DIRECTION::DIR_LEFT:	//左側
		nDestX--;	//今居るステージの左側のステージを調べる

		if (nDestX < 0)
		{//0未満(-1以下)になった場合
			nDestX = 0;	//0に固定
		}
		break;

	case CStage::DIRECTION::DIR_BACK:	//奥側
		nDestY--;	//今居るステージの奥側のステージを調べる

		if (nDestY < 0)
		{//0未満(-1以下)になった場合
			nDestY = 0;	//0に固定
		}
		break;

	case CStage::DIRECTION::DIR_RIGHT:	//右側
		nDestX++;	//今居るステージの右側のステージを調べる

		if (nDestX >= CPanel::GRID_X)
		{//2より大きく(3以上)になった場合
			nDestX = (CPanel::GRID_X - 1);	//2に固定
		}
		break;

	case CStage::DIRECTION::DIR_FRONT:	//手前側
		nDestY++;	//今居るステージの手前側のステージを調べる

		if (nDestY >= CPanel::GRID_Y)
		{//2より大きく(3以上)になった場合
			nDestY = (CPanel::GRID_Y - 1);	//2に固定
		}
		break;

	case CStage::DIRECTION::DIR_NONE:
	case CStage::DIRECTION::DIR_MAX:
	default:	//その他
		assert(false);
		break;
	}

	if (aInfo[nDestY][nDestX].stage == CStage::STAGE::NONE)
	{//ドアの先にステージが無い場合
		return;
	}

	/* ドアの先にステージがある場合 */

	//ドア先のステージの、ドアの方向のみを読み込む
	pStage->LoadDoorDir(aInfo[nDestY][nDestX].stage);

	CStage::DIRECTION aDir[CStage::MAX_DOOR] = {};	//ドア方向取得用

	for (int i = 0; i < CStage::MAX_DOOR; i++)
	{
		//読み込んだドアの方向を取得
		aDir[i] = (CStage::DIRECTION)pStage->GetDoorDir(i);
	}

	for (int i = 0; i < CStage::MAX_DOOR; i++)
	{
		if (aDir[i] == CStage::DIRECTION::DIR_NONE)
		{//ドアが無い場合
			continue;
		}

		/* ドアがある場合 */

		switch (dirDoor)
		{//触れたドアの方向
		case CStage::DIRECTION::DIR_LEFT:	//左側
			if (aDir[i] == CStage::DIRECTION::DIR_RIGHT)
			{//ドア先のステージに、対応する「右側」のドアがある場合
				m_bCanMove = true;	//移動できる
			}
			break;

		case CStage::DIRECTION::DIR_BACK:	//奥側
			if (aDir[i] == CStage::DIRECTION::DIR_FRONT)
			{//ドア先のステージに、対応する「手前側」のドアがある場合
				m_bCanMove = true;	//移動できる
			}
			break;

		case CStage::DIRECTION::DIR_RIGHT:	//右側
			if (aDir[i] == CStage::DIRECTION::DIR_LEFT)
			{//ドア先のステージに、対応する「左側」のドアがある場合
				m_bCanMove = true;	//移動できる
			}
			break;

		case CStage::DIRECTION::DIR_FRONT:	//手前側
			if (aDir[i] == CStage::DIRECTION::DIR_BACK)
			{//ドア先のステージに、対応する「奥側」のドアがある場合
				m_bCanMove = true;	//移動できる
			}
			break;

		default:
			break;
		}

		if (m_bCanMove)
		{//ドアを通過してステージ移動出来る場合
			//ステージを変更
			pStage->Change(aInfo[nDestY][nDestX].stage);
			return;
		}
	}
}

//================================================
//頂点の最大値と最小値を設定
//================================================
void CPlayer::SetVtxMaxAndMin()
{
	//頂点の最大数・最小値を初期化
	m_vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//各パーツの頂点の最大値・最小値を格納する配列
	D3DXVECTOR3 aVtxMax[MAX_PARTS] = {};
	D3DXVECTOR3 aVtxMin[MAX_PARTS] = {};

	for (int i = 0; i < MAX_PARTS; i++)
	{
		//モデルの各情報の取得
		CModel::MODEL_INFO modelInfo = CModel::GetModelInfo(i);

		//各パーツから頂点の最大値・最小値を取得
		aVtxMax[i] = modelInfo.vtxMax;
		aVtxMin[i] = modelInfo.vtxMin;
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		//********** 最大値 **********//

		/* 各パーツの最大値が、現在の最大値より大きい場合、最大値を設定する */

		if (aVtxMax[i].x > m_vtxMax.x)
		{//X座標
			m_vtxMax.x = aVtxMax[i].x;	//最大値を設定
		}

		if (aVtxMax[i].y > m_vtxMax.y)
		{//Y座標
			m_vtxMax.y = aVtxMax[i].y;	//最大値を設定
		}

		if (aVtxMax[i].z> m_vtxMax.z)
		{//Z座標
			m_vtxMax.z = aVtxMax[i].z;	//最大値を設定
		}
		
		//********** 最小値 **********//

		/* 各パーツの最小値が、現在の最小値より小さい場合、最小値を設定する */

		if (aVtxMin[i].x < m_vtxMin.x)
		{//X座標
			m_vtxMin.x = aVtxMin[i].x;	//最小値を設定
		}

		if (aVtxMin[i].y < m_vtxMin.y)
		{//Y座標
			m_vtxMin.y = aVtxMin[i].y;	//最小値を設定
		}

		if (aVtxMin[i].z < m_vtxMin.z)
		{//Z座標
			m_vtxMin.z = aVtxMin[i].z;	//最小値を設定
		}
	}
}

//================================================
//ラインの設定まとめ
//================================================
void CPlayer::SetLines()
{
	//向き(固定)
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//色(全ての線で同じ色)
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//何番目か
	int nNum = 0;

	//********** 上部左側 **********//

	//始点・終点
	D3DXVECTOR3 start = m_aPosVtx[3];
	D3DXVECTOR3 end = m_aPosVtx[2];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 上部手前側 **********//

	//始点・終点
	start = m_aPosVtx[2];
	end = m_aPosVtx[6];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 上部右側 **********//

	//始点・終点
	start = m_aPosVtx[6];
	end = m_aPosVtx[7];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 上部奥側 **********//

	//始点・終点
	start = m_aPosVtx[7];
	end = m_aPosVtx[3];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部左側 **********//

	//始点・終点
	start = m_aPosVtx[1];
	end = m_aPosVtx[0];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部手前側 **********//

	//始点・終点
	start = m_aPosVtx[0];
	end = m_aPosVtx[4];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部右側 **********//

	//始点・終点
	start = m_aPosVtx[4];
	end = m_aPosVtx[5];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 下部奥側 **********//

	//始点・終点
	start = m_aPosVtx[5];
	end = m_aPosVtx[1];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 手前左側 **********//

	//始点・終点
	start = m_aPosVtx[2];
	end = m_aPosVtx[0];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 手前右側 **********//

	//始点・終点
	start = m_aPosVtx[6];
	end = m_aPosVtx[4];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 奥左側 **********//

	//始点・終点
	start = m_aPosVtx[3];
	end = m_aPosVtx[1];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める

	//********** 奥右側 **********//

	//始点・終点
	start = m_aPosVtx[7];
	end = m_aPosVtx[5];

	//設定
	m_apLine[nNum]->Set(m_pos, rot, start, end, col);

	nNum++;	//次に進める
}

//================================================
//読み込み
//================================================
void CPlayer::Load()
{
	//ファイルを開く
	FILE* pFile = fopen(FILE_NAME, "r");
	
	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		assert(false);
	}

	/* ファイルが開けた場合 */

	char aText[MAX_WORD];	//テキスト格納用

	while (strncmp(&aText[0], "SCRIPT", 6) != 0)
	{//テキストの最初の行を読み込むまで繰り返す
		fgets(aText, MAX_WORD, pFile);	//1行丸ごと読み込む
	}

	while (strcmp(&aText[0], "END_SCRIPT") != 0)
	{//テキストの最終行を読み込むまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "MOTIONSET") == 0)
		{//モーションセット
			//モーション設定
			Set_Motion(pFile, &aText[0]);

			//モーション数を増やす
			m_nNumMotion++;
		}
	}

	//ファイルを閉じる
	fclose(pFile);
}

//================================================
//モーション設定
//================================================
void CPlayer::Set_Motion(FILE* pFile, char aText[])
{
	while (strcmp(&aText[0], "END_MOTIONSET") != 0)
	{//モーションセットが終わるまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "LOOP") == 0)
		{//ループするかどうか
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			int nLoop = 0;	//読み込み用

			//ループの有無を読み込む
			fscanf(pFile, "%d", &nLoop);

			if (nLoop == 0)
			{
				//ループ無し
				m_aMotion[m_nNumMotion].bLoop = false;
			}
			else
			{
				//ループ有り
				m_aMotion[m_nNumMotion].bLoop = true;
			}
		}
		else if (strcmp(&aText[0], "NUM_KEY") == 0)
		{//キー数
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//キー数を読み込む
			fscanf(pFile, "%d", &m_aMotion[m_nNumMotion].nNumKey);
		}
		else if (strcmp(&aText[0], "KEYSET") == 0)
		{//キーセット
			//キーセット設定
			Set_KeySet(pFile, &aText[0]);

			//キーセット数を増やす
			m_nNumKeySet++;
		}
	}

	//キーセット数をリセット
	m_nNumKeySet = 0;
}

//================================================
//キーセット設定
//================================================
void CPlayer::Set_KeySet(FILE* pFile, char aText[])
{
	while (strcmp(&aText[0], "END_KEYSET") != 0)
	{//キーセットが終わるまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "FRAME") == 0)
		{//フレーム数
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//フレーム数を読み込む
			fscanf(pFile, "%d", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].nFrame);
		}
		else if (strcmp(&aText[0], "KEY") == 0)
		{//キー
			//キー設定
			Set_Key(pFile, &aText[0]);

			//キー数を増やす
			m_nNumKey++;
		}
	}

	//キー数をリセット
	m_nNumKey = 0;
}

//================================================
//キー設定
//================================================
void CPlayer::Set_Key(FILE* pFile, char aText[])
{
	while (strcmp(&aText[0], "END_KEY") != 0)
	{//キーが終わるまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "POS") == 0)
		{//位置
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//位置を読み込む
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].pos.x);	//X軸
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].pos.y);	//Y軸
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].pos.z);	//Z軸
		}
		else if (strcmp(&aText[0], "ROT") == 0)
		{//向き
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//向きを読み込む
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].rot.x);	//X軸
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].rot.y);	//Y軸
			fscanf(pFile, "%f", &m_aMotion[m_nNumMotion].aKeySet[m_nNumKeySet].aKey[m_nNumKey].rot.z);	//Z軸
		}
	}
}