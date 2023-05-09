//================================================
//
//3Dゲーム(仮)[model.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>
#include <stdio.h>

//***************************
//モデルクラスの定義
//***************************
class CModel
{/* 基本クラス */
private: /* 定数の定義 */
	static const int MAX_WORD;			//最大文字数
	static const int MAX_PARTS = 16;	//最大パーツ数
	static const int NUM_PARTS = 5;		//パーツ数

	static const char* FILE_NAME;	//ファイル名

private: /* 構造体の定義 */
	struct PARTS_SET	//パーツ情報
	{
		int nIndex;			//インデックス数
		int nParent;		//親パーツ番号
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
	};

	struct CHARACTER_SET	//キャラクター情報
	{
		int nNumParts;					//パーツ数
		PARTS_SET aPartsSet[NUM_PARTS];	//パーツ情報
	};
public:
	struct MODEL_INFO	//モデルの各情報
	{
		LPD3DXMESH pMesh;		//メッシュ情報のポインタ
		LPD3DXBUFFER pBuffMat;	//マテリアル情報のポインタ
		DWORD numMat;			//マテリアル情報の数
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//向き
		D3DXVECTOR3 vtxMax;		//最大値
		D3DXVECTOR3 vtxMin;		//最小値
	};

public: /* 静的メンバ関数 */
	static CModel* Create();	//生成

	/*
		モデルの各情報の取得
		int nNum ---> 番号
	*/
	static MODEL_INFO GetModelInfo(int nNum);

	/*
		モデルの位置情報を設定
		int nNum ---> 番号
		const D3DXVECTOR3 &pos ---> 位置
	*/
	static void SetModelInfo_Pos(int nNum, const D3DXVECTOR3 &pos);

	/*
		モデルの向き情報を設定
		int nNum ---> 番号
		const D3DXVECTOR3 &rot ---> 向き
	*/
	static void SetModelInfo_Rot(int nNum, const D3DXVECTOR3 &rot);

public: /* 静的メンバ変数 */
	static CHARACTER_SET m_characterSet;	//キャラクター情報

	static MODEL_INFO m_aModelInfo[NUM_PARTS];	//モデルの各情報

public: /* コンストラクタ・デストラクタ */
	CModel();
	~CModel();

public: /* メンバ関数 */
	HRESULT Init();	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	D3DXMATRIX GetMtxWorld();	//ワールドマトリックスの取得

private:
	void DrawShadow();	//影の描画
	void Load();		//読み込み

	/*
		キャラクター設定
		FILE* pFile ---> ファイルポインタ
	*/
	void Set_CharacterSet(FILE* pFile);

	/*
		パーツ設定
		FILE* pFile ---> ファイルポインタ
		PARTS_SET* pParts ---> パーツ情報のポインタ
	*/
	void Set_PartsSet(FILE* pFile, PARTS_SET* pParts);

private: /* メンバ変数 */
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	
	int m_nNumModel;	//モデル数
};

#endif