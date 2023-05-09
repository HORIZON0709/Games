//================================================
//
//3Dゲーム(仮)[input.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "input.h"

#include <assert.h>

//***************************
//静的メンバ変数
//***************************
CInput* CInput::m_pInput = nullptr;

//================================================
//生成
//================================================
CInput* CInput::Create()
{
	if (m_pInput != nullptr)
	{//NULLチェック
		return nullptr;
	}

	/* nullptrの場合 */

	m_pInput = new CInput;	//メモリの動的確保

	return m_pInput;	//動的確保したものを返す
}

//================================================
//取得
//================================================
CInput* CInput::GetKey()
{
	return m_pInput;
}

//================================================
//コンストラクタ
//================================================
CInput::CInput():
	m_pInputKeyboard(nullptr)
{
}

//================================================
//デストラクタ
//================================================
CInput::~CInput()
{
}

//================================================
//初期化
//================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInputKeyboard != nullptr)
	{//NULLチェック
		m_pInputKeyboard = nullptr;
	}

	/* nullptrの場合 */

	m_pInputKeyboard = new CInputKeyboard;	//メモリの動的確保

	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//終了
//================================================
void CInput::Uninit()
{
	if (m_pInputKeyboard != nullptr)
	{//キーボード
		m_pInputKeyboard->Uninit();	//終了
		delete m_pInputKeyboard;	//メモリの解放
		m_pInputKeyboard = nullptr;	//nullptrにする
	}

	if (m_pInput != nullptr)
	{//インプット
		delete m_pInput;	//メモリの解放
		m_pInput = nullptr;	//nullptrにする
	}
}

//================================================
//更新処理
//================================================
void CInput::Update()
{
	m_pInputKeyboard->Update();
}

//================================================
//プレス
//================================================
bool CInput::Press(STANDARD_KEY key)
{
	switch (key)
	{
	case STANDARD_KEY::UP: /* 上 */
		if (m_pInputKeyboard->GetPress(DIK_W) ||
			m_pInputKeyboard->GetPress(DIK_UP))
		{
			return true;
		}
		break;

	case STANDARD_KEY::DOWN: /* 下 */
		if (m_pInputKeyboard->GetPress(DIK_S) ||
			m_pInputKeyboard->GetPress(DIK_DOWN))
		{
			return true;
		}
		break;

	case STANDARD_KEY::LEFT: /* 左 */
		if (m_pInputKeyboard->GetPress(DIK_A) ||
			m_pInputKeyboard->GetPress(DIK_LEFT))
		{
			return true;
		}
		break;

	case STANDARD_KEY::RIGHT: /* 右 */
		if (m_pInputKeyboard->GetPress(DIK_D) ||
			m_pInputKeyboard->GetPress(DIK_RIGHT))
		{
			return true;
		}
		break;

	case STANDARD_KEY::DECISION: /* 決定 */
		if (m_pInputKeyboard->GetPress(DIK_RETURN))
		{
			return true;
		}
		break;

	case STANDARD_KEY::SHOT: /* 発射 */
		if (m_pInputKeyboard->GetPress(DIK_SPACE))
		{
			return true;
		}
		break;

	default: /* 上記以外 */
		assert(false);
		break;
	}

	return false;
}

//================================================
//リリース
//================================================
bool CInput::Release(STANDARD_KEY key)
{
	switch (key)
	{
	case STANDARD_KEY::UP: /* 上 */
		if (m_pInputKeyboard->GetRelease(DIK_W) ||
			m_pInputKeyboard->GetRelease(DIK_UP))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DOWN: /* 下 */
		if (m_pInputKeyboard->GetRelease(DIK_S) ||
			m_pInputKeyboard->GetRelease(DIK_DOWN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::LEFT: /* 左 */
		if (m_pInputKeyboard->GetRelease(DIK_A) ||
			m_pInputKeyboard->GetRelease(DIK_LEFT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::RIGHT: /* 右 */
		if (m_pInputKeyboard->GetRelease(DIK_D) ||
			m_pInputKeyboard->GetRelease(DIK_RIGHT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DECISION: /* 決定 */
		if (m_pInputKeyboard->GetRelease(DIK_RETURN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::SHOT: /* 発射 */
		if (m_pInputKeyboard->GetRelease(DIK_SPACE))
		{
			return true;
		}
		break;
	default: /* それ以外 */
		assert(false);
		break;
	}

	return false;
}

//================================================
//トリガー
//================================================
bool CInput::Trigger(STANDARD_KEY key)
{
	switch (key)
	{
	case STANDARD_KEY::UP: /* 上 */
		if (m_pInputKeyboard->GetTrigger(DIK_W) ||
			m_pInputKeyboard->GetTrigger(DIK_UP))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DOWN: /* 下 */
		if (m_pInputKeyboard->GetTrigger(DIK_S) ||
			m_pInputKeyboard->GetTrigger(DIK_DOWN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::LEFT: /* 左 */
		if (m_pInputKeyboard->GetTrigger(DIK_A) ||
			m_pInputKeyboard->GetTrigger(DIK_LEFT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::RIGHT: /* 右 */
		if (m_pInputKeyboard->GetTrigger(DIK_D) ||
			m_pInputKeyboard->GetTrigger(DIK_RIGHT))
		{
			return true;
		}
		break;
	case STANDARD_KEY::DECISION: /* 決定 */
		if (m_pInputKeyboard->GetTrigger(DIK_RETURN))
		{
			return true;
		}
		break;
	case STANDARD_KEY::SHOT: /* 発射 */
		if (m_pInputKeyboard->GetTrigger(DIK_SPACE))
		{
			return true;
		}
		break;
	default: /* それ以外 */
		assert(false);
		break;
	}

	return false;
}