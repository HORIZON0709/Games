//================================================
//
//3Dゲーム(仮)[player.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************
//インクルード
//***************************
#include "object.h"
#include "stage.h"

#include <stdio.h>

//***************************
//前方宣言
//***************************
class CModel;
class CItem;
class CLine;

//***************************
//プレイヤークラスの定義
//***************************
class CPlayer : public CObject
{/* CObjectの派生クラス */
private: /* 列挙型の定義 */
	enum MOTION_TYPE	//モーションの種類
	{
		NONE = -1,
		NEUTRAL = 0,	//ニュートラル
		MOVE,			//移動
		ATTACK,			//攻撃
		MAX
	};

private: /* 定数の定義 */
	static const float MOVE_SPEED;		//移動速度
	static const float ROT_SMOOTHNESS;	//回転の滑らかさ
	static const float LIMIT_POS;		//移動制限位置
	
	static const int MAX_PARTS = 5;		//最大パーツ数
	static const int NUM_KEYSET = 2;	//キーセット数
	static const int MAX_LINE = 12;		//ラインの最大数
	static const int NUM_VTX_3D = 8;	//立体の頂点数
	static const int MAX_WORD;			//最大文字数

	static const char* FILE_NAME;	//ファイル名

private: /* 構造体の定義 */
	struct KEY //キー要素
	{
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
	};

	struct KEY_SET //キー情報
	{
		int nFrame;				//フレーム数
		KEY aKey[MAX_PARTS];	//キー要素
	};

	struct MOTION_SET	//モーション情報
	{
		bool bLoop;						//ループをするかしないか
		int nNumKey;					//キー数
		KEY_SET aKeySet[NUM_KEYSET];	//キー情報
	};

public: /* 静的メンバ関数 */
	static CPlayer* Create();	//生成

private: /* 静的メンバ変数 */
	static MOTION_SET m_aMotion[MOTION_TYPE::MAX];	//モーション情報

	static CLine* m_apLine[MAX_LINE];	//ラインのポインタ

public: /* コンストラクタ・デストラクタ */
	CPlayer();
	~CPlayer() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

public: /* メンバ関数 */
	D3DXVECTOR3 GetPos();			//位置を取得
	D3DXVECTOR3 GetVtxMax();		//頂点の最大値を取得
	D3DXVECTOR3 GetVtxMin();		//頂点の最小値を取得
	bool GetIsGetItem();			//アイテムを取得したかどうかを取得

	/*
		アイテムの獲得状況を取得
		int nIdx ---> 番号
	*/
	CStage::STAGE GetStageCoin(int nIdx);

private:
	void Move();		//移動
	void Motion();		//モーション
	void Collision();	//当たり判定
	void StageChange();	//ステージ切り替え

	void SetVtxMaxAndMin();	//頂点の最大値と最小値の設定
	void SetLines();		//ラインの設定まとめ

	void Load();	//読み込み

	void Set_Motion(FILE* pFile, char aText[]);	//モーション設定
	void Set_KeySet(FILE* pFile, char aText[]);	//キーセット設定
	void Set_Key(FILE* pFile, char aText[]);	//キー設定

private: /* メンバ変数 */
	CModel* m_pModel;	//モデルのポインタ
	CItem* m_pItem;		//アイテムのポインタ

	CStage::STAGE m_aStageCoin[CStage::MAX_COIN];	//獲得したアイテムがあったステージ

	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス

	D3DXVECTOR3 m_pos;		//現在の位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_vec;		//方向ベクトル
	D3DXVECTOR3 m_rot;		//現在の向き
	D3DXVECTOR3 m_rotDest;	//目的の向き
	D3DXVECTOR3 m_vtxMax;	//頂点の最大値
	D3DXVECTOR3 m_vtxMin;	//頂点の最小値

	D3DXVECTOR3 m_aPosVtx[NUM_VTX_3D];	//頂点の位置

	MOTION_TYPE m_motionType;	//モーションの種類

	int m_nCurrentKey;	//現在のキー番号
	int m_nCntMotion;	//モーションカウンター
	int m_nNumMotion;	//モーション数
	int m_nNumKeySet;	//キーセット数
	int m_nNumKey;		//キー数
	int m_nGetItem;		//アイテム取得数

	bool m_bPressKey;		//キー押下中かどうか
	bool m_bCollObjectX;	//Xモデルとの当たり判定
	bool m_bCollGimmick;	//ギミックとの当たり判定
	bool m_bCollDoor;		//ドアとの当たり判定
	bool m_bCollPanelStand;	//パネルスタンドとの当たり判定
	bool m_bGetItem;		//アイテムを取得したかどうか
	bool m_bFadeOut;		//フェードしたかどうか
	bool m_bCanMove;		//移動できるかどうか
};

#endif