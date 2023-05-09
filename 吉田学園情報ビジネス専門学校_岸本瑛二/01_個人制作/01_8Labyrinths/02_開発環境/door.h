//================================================
//
//3Dゲーム(仮)[door.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _DOOR_H_
#define _DOOR_H_

//***************************
//インクルード
//***************************
#include "objectX.h"
#include "stage.h"

//***************************
//前方宣言
//***************************
class CLine;

//***************************
//プレイヤークラスの定義
//***************************
class CDoor : public CObjectX
{/* CObjectXの派生クラス */
private: /* 定数の定義 */
	static const int MAX_LINE = 12;	//ラインの最大数

public: /* 静的メンバ関数 */
	/*
		生成
		char* pFileName ---> ファイル名
	*/
	static CDoor* Create(char* pFileName);

public: /* コンストラクタ・デストラクタ */
	CDoor();
	~CDoor() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

public: /* メンバ関数 */
	/*
		方向の設定
		CStage::DIRECTION dir ---> 方向
	*/
	void SetDir(CStage::DIRECTION dir);

	CStage::DIRECTION GetDir();	//方向の取得

private:
	void SetLines();	//ラインの設定まとめ

private: /* メンバ変数 */
	CLine* m_apLine[MAX_LINE];	//ラインのポインタ

	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス

	CStage::DIRECTION m_dir;	//方向
};

#endif