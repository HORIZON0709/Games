//================================================
//
//3Dゲーム(仮)[stage.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>
#include <stdio.h>

//***************************
//前方宣言
//***************************
class CObject3D;
class CObjectX;
class CGimmick;
class CItem;
class CDoor;

//***************************
//ステージクラスの定義
//***************************
class CStage
{/* 基本クラス */
public: /* 列挙型の定義 */
	enum STAGE	//ステージの種類
	{
		NONE = -1,
		Stage01 = 0,	//ステージ1
		Stage02,		//ステージ2
		Stage03,		//ステージ3
		Stage04,		//ステージ4
		Stage05,		//ステージ5
		Stage06,		//ステージ6
		Stage07,		//ステージ7
		Stage08,		//ステージ8
		MAX
	};

	enum DIRECTION	//方向
	{
		DIR_NONE = -1,
		DIR_LEFT = 0,	//左
		DIR_BACK,		//奥
		DIR_RIGHT,		//右
		DIR_FRONT,		//手前
		DIR_MAX
	};

private:
	enum MODEL_TYPE	//モデルタイプ
	{
		TYPE_NONE = -1,
		TYPE_OBJECT = 0,	//オブジェクト
		TYPE_GIMMICK,		//ギミック
		TYPE_ITEM,			//アイテム
		TYPE_DOOR,			//ドア
		TYPE_MAX
	};

public: /* 定数の定義 */
	static const char* s_apFileName[];	//ファイルパス

	static const int MAX_MODEL = 24;	//モデルの最大数
	static const int MAX_GIMMICK = 8;	//ギミックの最大数
	static const int MAX_DOOR = 2;		//ドアの最大数
	static const int MAX_COIN = 3;		//コインの最大数

	static const float ROT_DOOR;	//ドアの向き

	static const D3DXVECTOR3 POS_DOOR[DIRECTION::DIR_MAX];	//ドアの位置
private:
	static const float FLOAR_SIZE;	//床のサイズ
	static const float WALL_WIDTH;	//壁の幅
	static const float WALL_HEIGHT;	//壁の高さ

	static const int MAX_WORD;	//最大文字数

private: /* 構造体の定義 */
	struct MODELSET_INFO	//モデル配置に必要な情報
	{
		int nIndex;			//インデックス数
		MODEL_TYPE type;	//モデルタイプ
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
		DIRECTION dir;		//方向
	};

public: /* 静的メンバ関数 */
	/*
		生成
		const STAGE &stage ---> ステージ
	*/
	static CStage* Create(const STAGE &stage);

	/*
		Xモデル情報の取得
		int nIdx ---> インデックス数
	*/
	static CObjectX* GetObjectX(int nIdx);

	/*
		ギミック情報の取得
		int nIdx ---> インデックス数
	*/
	static CGimmick* GetGimmick(int nIdx);

	static CItem* GetItem();	//アイテム情報の取得

	/*
		ドア情報の取得
		int nIdx ---> インデックス数
	*/
	static CDoor* GetDoor(int nIdx);

public: /* 静的メンバ変数 */
	static CObjectX* m_apModel[MAX_MODEL];		//Xモデルのポインタ
	static CGimmick* m_apGimmick[MAX_GIMMICK];	//ギミックのポインタ
	static CItem* m_pItem;						//アイテムのポインタ
	static CDoor* m_apDoor[MAX_DOOR];			//ドアのポインタ

private:
	static CObject3D* m_pFloar;						//3Dポリゴンの床のポインタ
	static CObject3D* m_apWall[DIRECTION::DIR_MAX];	//3Dポリゴンの壁(四方)のポインタ

public: /* コンストラクタ・デストラクタ */
	CStage();
	~CStage();

public: /* メンバ関数 */
	/*
		初期化
		const STAGE &stage ---> ステージの種類
		const char* pStage ---> ステージのファイルパス
	*/
	HRESULT Init(const STAGE &stage, const char* pStage);

	void Uninit();		//終了
	void UninitAll();	//全ての終了
	void Update();		//更新
	CStage* Set();		//ステージの設定
	STAGE Get();		//ステージの取得

	/*
		ステージの変更
		const STAGE &stage ---> ステージの種類
	*/
	void Change(const STAGE &stage);

	/*
		ドアの方向の読み込み
		const STAGE &stage ---> ステージの種類
	*/
	void LoadDoorDir(const STAGE &stage);

	/*
		ドアの方向を取得
		int nIdx ---> 番号
	*/
	int GetDoorDir(int nIdx);

private:
	/*
		読み込み
		const char* pStage ---> ステージのファイルパス
	*/
	void Load(const char* pStage);

	/*
		モデルセット設定
		FILE* pFile ---> ファイルポインタ
	*/
	void Set_ModelSet(FILE* pFile);

	void CreateFloarAndWalls();	//床と壁の生成

	void SetWall();	//壁の設定

private: /* メンバ変数 */
	MODELSET_INFO m_aModelSetInfo[MAX_MODEL];	//モデル設置に必要な情報

	STAGE m_stage;		//現在のステージ
	STAGE m_stageNext;	//次のステージ

	int m_nNumModel;	//モデル数
	int m_nCntModelSet;	//セット済モデル数カウント用

	int m_aDir[MAX_DOOR];	//ドアの方向(番号)
};

#endif