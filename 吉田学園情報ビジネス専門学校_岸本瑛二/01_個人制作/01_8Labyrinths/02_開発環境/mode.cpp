//================================================
//
//3Dゲーム(仮)[mode.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "mode.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include <assert.h>

//--------------------------------------------------
// 生成
//--------------------------------------------------
CMode* CMode::Create(const MODE &mode)
{
	CMode* pMode = nullptr;	//ポインタ

	switch (mode)
	{
	case MODE::TITLE: /* タイトル */
		pMode = new CTitle;	//メモリの動的確保
		break;

	case MODE::GAME: /* ゲーム */
		pMode = new CGame;	//メモリの動的確保
		break;

	case MODE::RESULT: /* リザルト */
		pMode = new CResult;	//メモリの動的確保
		break;

	case MODE::MAX:
	default: /* それ以外 */
		assert(false);
		break;
	}

	if (pMode == nullptr)
	{// NULLチェック
		assert(false);
	}

	/* nullptrではない場合 */

	pMode->Init();	//初期化

	return pMode;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CMode::CMode(MODE mode) :
	m_modeNext(MODE::NONE)
{
	m_mode = mode;
}

//================================================
//デストラクタ
//================================================
CMode::~CMode()
{
}

//================================================
//モードの設定
//================================================
CMode* CMode::Set()
{
	if (m_modeNext == MODE::NONE)
	{//次のモードが決まっていない
		return this;
	}

	//現在のモードを終了
	Uninit();

	m_mode = m_modeNext;		//モードを変更
	m_modeNext = MODE::NONE;	//何もない状態にする
	
	return Create(m_mode);	//変更後のモードを生成して返す
}

//================================================
//モードの取得
//================================================
CMode::MODE CMode::Get()
{
	return m_mode;
}

//================================================
//モードの変更
//================================================
void CMode::Change(const MODE &mode)
{
	assert(mode > MODE::NONE && mode < MODE::MAX);

	m_modeNext = mode;
}