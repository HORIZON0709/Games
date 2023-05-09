//================================================
//
//3Dゲーム(仮)[inputKeyboard.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "inputKeyboard.h"

//================================================
//コンストラクタ
//================================================
CInputKeyboard::CInputKeyboard():
	m_pInput(nullptr),
	m_pDevKeyboard(nullptr)
{
	//メンバ変数のクリア
	memset(m_aKeyStatePress, 0, sizeof(m_aKeyStatePress));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
}

//================================================
//デストラクタ
//================================================
CInputKeyboard::~CInputKeyboard()
{
}

//================================================
//初期化
//================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevKeyboard->Acquire();

	return S_OK;
}

//================================================
//終了
//================================================
void CInputKeyboard::Uninit()
{
	//入力デバイス（キーボード）の放棄
	if (m_pDevKeyboard != nullptr)
	{
		m_pDevKeyboard->Unacquire();	//キーボードへのアクセス権を放棄
		m_pDevKeyboard->Release();
		m_pDevKeyboard = nullptr;
	}

	//DirectInputオブジェクトの破壊
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//================================================
//更新
//================================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY] = {};	//キーボードの入力情報
	
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			//トリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = ~m_aKeyStatePress[nCntKey] & aKeyState[nCntKey];

			//リリース情報を保存
			m_aKeyStateRelease[nCntKey] = m_aKeyStatePress[nCntKey] & ~aKeyState[nCntKey];

			//プレス情報を保存
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
		}

		return;
	}
	
	m_pDevKeyboard->Acquire();	//キーボードへのアクセス権を獲得
}

//================================================
//プレス情報の取得
//================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//================================================
//リリース情報の取得
//================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//================================================
//トリガー情報の取得
//================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}