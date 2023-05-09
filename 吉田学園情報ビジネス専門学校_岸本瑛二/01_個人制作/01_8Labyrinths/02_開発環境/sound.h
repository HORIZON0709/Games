//================================================
//
//3Dゲーム(仮)[sound.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//***************************
//インクルード
//***************************
#include <d3dx9.h>
#include <xaudio2.h>

//***************************
//サウンドクラス定義
//***************************
class CSound
{/* 基本クラス */
public: /* 列挙型の定義 */
	enum ELabel
	{
		LABEL_NONE = -1,
		LABEL_BGM_Title = 0,	//タイトル
		LABEL_BGM_Game,			//ゲーム
		LABEL_BGM_Result,		//リザルト
		LABEL_SE_Enter,			//決定
		LABEL_SE_Select,		//選択
		LABEL_MAX,
	};

public: /* 構造体の定義 */
	struct SParam
	{
		char *pFileName;	//ファイル名
		int loop;			//ループ
	};

public: /* 定数の定義 */
	static const SParam PARAM[];	//パラメータの情報

public: /* コンストラクタ・デストラクタ */
	CSound();
	~CSound();

public: /* メンバ関数 */
	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT Play(ELabel sound);
	void Stop(ELabel label);
	void Stop();

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	//チャンクのチェック
	HRESULT LoadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	//チャンクデータの読み込み

private: /* メンバ変数 */
	IXAudio2* m_pXAudio2;							//XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;		//マスターボイス
	IXAudio2SourceVoice* m_pSourceVoice[LABEL_MAX];	//ソースボイス
	BYTE* m_pDataAudio[LABEL_MAX];					//オーディオデータ
	DWORD m_sizeAudio[LABEL_MAX];					//オーディオデータサイズ
};

#endif