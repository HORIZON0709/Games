//================================================
//
//3Dゲーム(仮)[object.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>

//***************************
//オブジェクトクラスの定義
//***************************
class CObject
{/* 基本クラス */
public: /* 列挙型の定義 */
	//オブジェクトの種類
	enum OBJ_TYPE
	{
		NONE = -1,
		PLAYER = 0,	//プレイヤー
		ENEMY,		//敵
		BG,			//背景
		MODEL,		//モデル
		ITEM,		//アイテム
		GIMMICK,	//ギミック
		DOOR,		//ドア
		POLYGON_2D,	//2Dポリゴン
		POLYGON_3D,	//3Dポリゴン
		SCORE,		//スコア
		NUMBER,		//ナンバー
		MAX
	};

	//描画の優先順位
	enum PRIORITY
	{
		PRIO_NONE = -1,
		PRIO_BG = 0,		//背景
		PRIO_MESH,			//メッシュ
		PRIO_MODEL,			//モデル
		PRIO_POLYGON_3D,	//3Dポリゴン
		PRIO_POLYGON_2D,	//2Dポリゴン
		PRIO_MAX
	};

public: /* 静的メンバ関数 */
	static void ReleaseAll();	//全ての解放
	static void UpdateAll();	//全ての更新
	static void DrawAll();		//全ての描画

public: /* コンストラクタ・デストラクタ */
	CObject();
	CObject(const PRIORITY &priority);
	virtual ~CObject();

public: /* 純粋仮想関数 */
	virtual HRESULT Init() = 0;	//初期化
	virtual void Uninit() = 0;	//終了
	virtual void Update() = 0;	//更新
	virtual void Draw() = 0;	//描画

public: /* メンバ関数 */
	void SetDeathFlag();	//死亡フラグの設定

public:	/* ObjType */
	/*
		タイプの設定
		const OBJ_TYPE &type ---> 任意のタイプ
	*/
	void SetObjType(const OBJ_TYPE &type);

	//タイプの取得
	OBJ_TYPE GetObjType();

private:
	void Release();	//解放

private: /* 静的メンバ変数 */
	static CObject* m_apTop[PRIORITY::PRIO_MAX];		//先頭のオブジェクトのポインタ
	static CObject* m_apCurrent[PRIORITY::PRIO_MAX];	//現在(末尾)のオブジェクトのポインタ

	static int m_nNumAll;	//オブジェクトの総数
	
private: /* メンバ変数 */
	CObject* m_pPrev;	//前のオブジェクトへのポインタ
	CObject* m_pNext;	//次のオブジェクトへのポインタ

	CObject::OBJ_TYPE m_objType;	//種類

	bool m_bDeath;	//死亡フラグ
};

#endif