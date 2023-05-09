//================================================
//
//3Dゲーム(仮)[game.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _GAME_H_
#define _GAME_H_

//***************************
//インクルード
//***************************
#include "mode.h"

#include <d3dx9.h>

//***************************
//前方宣言
//***************************
class CCamera;
class CLight;
class CPlayer;
class CItem;
class CStage;
class CPanel;
class CScore;
class CObject2D;

//***************************
//ゲームクラスの定義
//***************************
class CGame : public CMode
{/* CModeの派生クラス */
private: /* 列挙型の定義 */
	enum UI_TYPE	//UIタイプ
	{
		NONE = -1,
		PURPOSE = 0,	//目的
		COIN,			//獲得コイン数
		MAX
	};

private: /* 定数の定義 */
	static const int INTERVAL_STRAIGHT;			//直線敵の生成間隔
	static const int FADE_INTERVAL_GAMEOVER;	//フェードまでの間隔(ゲームオーバー時)
	static const int FADE_INTERVAL_GAMECLEAR;	//フェードまでの間隔(ゲームクリア時)

	static const float UI_PURPOSE_WIDTH;	//UI(目的)の幅
	static const float UI_PURPOSE_HEIGHT;	//UI(目的)の高さ
	static const float UI_COIN_WIDTH;		//UI(獲得コイン数)の幅
	static const float UI_COIN_HEIGHT;		//UI(獲得コイン数)の高さ

public: /* 静的メンバ関数 */
	static CCamera* GetCamera();	//カメラの情報を取得
	static CLight* GetLight();		//ライトの情報を取得
	static CPlayer* GetPlayer();	//プレイヤーの情報を取得
	static CStage* GetStage();		//ステージの情報を取得
	static CPanel* GetPanel();		//パネルの情報を取得
	static CScore* GetScore();		//スコアの情報を取得

public: /* コンストラクタ・デストラクタ */
	CGame();
	~CGame() override;

public: /* オーバーライド関数 */
	HRESULT Init() override;	//初期化
	void Uninit() override;		//終了
	void Update() override;		//更新
	void Draw() override;		//描画

private: /* 静的メンバ変数 */
	static CCamera* m_pCamera;		//カメラ
	static CLight* m_pLight;		//ライト
	static CPlayer* m_pPlayer;		//プレイヤー
	static CStage* m_pStage;		//ステージ
	static CPanel* m_pPanel;		//パネル
	static CScore* m_pScore;		//スコア

	static CObject2D* m_apUI[UI_TYPE::MAX];	//UI

private: /* メンバ関数 */
	void SwitchWireFrame();	//ワイヤーフレームの切り替え

private: /* メンバ変数 */
	int m_nCntIntervalFade;	//フェードするまでの間隔カウント用

	bool m_bFadeOut;	//暗転したかどうか
	bool m_bWireFrame;	//ワイヤーフレーム表示切り替え
};
#endif