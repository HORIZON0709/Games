//============================================================================================================
//
//君をたずねてにゃん千里[input,cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "input.h"

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECTINPUT8		s_pInput	   = NULL;				//DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8 s_pDevKeyboard = NULL;				//入力デバイス(キーボード)へのポインタ
static BYTE					s_aKeyState[NUM_KEY_MAX];			//キーボードのプレス情報
static BYTE					s_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードのトリガー情報
static BYTE					s_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報
static XINPUT_STATE			s_joykeyState;						//ジョイパッドのプレス情報

//============================================================================================================
//キーボードの初期化処理
//============================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&s_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard,&s_pDevKeyboard,NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd,
													(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//============================================================================================================
//キーボードの終了処理
//============================================================================================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();	//キーボードへのアクセス権を放棄
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}
}

//============================================================================================================
//キーボードの更新処理
//============================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//キーボードのトリガー情報を保存
			s_aKeyStateTrigger[nCntKey] = (~s_aKeyState[nCntKey]) & (aKeyState[nCntKey]);

			//キーボードのリリース情報を保存
			s_aKeyStateRelease[nCntKey] = (s_aKeyState[nCntKey]) & (~aKeyState[nCntKey]);

			//キーボードのプレス情報を保存
			s_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();		//キーボードへのアクセス権を獲得
	}
}

//============================================================================================================
//キーボードのプレス情報を取得
//============================================================================================================
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//============================================================================================================
//キーボードのトリガー情報を取得
//============================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//============================================================================================================
//キーボードのリリース情報を取得
//============================================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//============================================================================================================
//ジョイパッドの初期化処理
//============================================================================================================
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&s_joykeyState, 0, sizeof(XINPUT_STATE));

	//Xinputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//============================================================================================================
//ジョイパッドの終了処理
//============================================================================================================
void UninitJoypad(void)
{
	//Xinputのステートを設定(無効にする)
	XInputEnable(false);
}

//============================================================================================================
//ジョイパッドの更新処理
//============================================================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		//ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		s_joykeyState = joykeyState;	//ジョイパッドのプレス情報を保存
	}
}

//============================================================================================================
//ジョイパッドのプレス情報を取得
//============================================================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (s_joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}