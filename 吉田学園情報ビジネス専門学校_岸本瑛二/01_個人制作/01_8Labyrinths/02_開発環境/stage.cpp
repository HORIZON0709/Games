//================================================
//
//3Dゲーム(仮)[stage.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "stage.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "object3D.h"
#include "gimmick.h"
#include "item.h"
#include "door.h"
#include "player.h"

#include "debug_proc.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const char* CStage::s_apFileName[] =
{//ステージのパス
	"data/TEXT/Stage_01.txt",	//ステージ01
	"data/TEXT/Stage_02.txt",	//ステージ02
	"data/TEXT/Stage_03.txt",	//ステージ03
	"data/TEXT/Stage_04.txt",	//ステージ04
	"data/TEXT/Stage_05.txt",	//ステージ05
	"data/TEXT/Stage_06.txt",	//ステージ06
	"data/TEXT/Stage_07.txt",	//ステージ07
	"data/TEXT/Stage_08.txt"	//ステージ08
};

static_assert(sizeof(CStage::s_apFileName) / sizeof(CStage::s_apFileName[0]) == CStage::STAGE::MAX, "aho");

const float CStage::ROT_DOOR = 1.57f;	//ドアの向き

const D3DXVECTOR3 CStage::POS_DOOR[CStage::DIRECTION::DIR_MAX] =
{//ドアの位置
	D3DXVECTOR3(-300.0f,0.0f,0.0f),	//左
	D3DXVECTOR3(0.0f,0.0f,300.0f),	//奥
	D3DXVECTOR3(300.0f,0.0f,0.0f),	//右
	D3DXVECTOR3(0.0f,0.0f,-300.0f)	//手前
};

const float CStage::FLOAR_SIZE = 700.0f;				//床のサイズ
const float CStage::WALL_WIDTH = FLOAR_SIZE;			//壁の幅
const float CStage::WALL_HEIGHT = FLOAR_SIZE * 0.5f;	//壁の高さ

const int CStage::MAX_WORD = 256;	//最大文字数

//***************************
//静的メンバ変数
//***************************
CObjectX* CStage::m_apModel[MAX_MODEL] = {};		//モデルのポインタ
CGimmick* CStage::m_apGimmick[MAX_GIMMICK] = {};	//ギミックのポインタ
CItem* CStage::m_pItem = nullptr;					//アイテムのポインタ
CDoor* CStage::m_apDoor[MAX_DOOR] = {};				//ドアのポインタ

CObject3D* CStage::m_pFloar = nullptr;					//3Dポリゴンの床のポインタ
CObject3D* CStage::m_apWall[DIRECTION::DIR_MAX] = {};	//3Dポリゴンの壁(四方)のポインタ

//================================================
//生成
//================================================
CStage* CStage::Create(const STAGE &stage)
{
	CStage* pStage = nullptr;	//ポインタ

	if (pStage != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pStage = new CStage;	//メモリの動的確保

	pStage->Init(stage, s_apFileName[stage]);	//初期化

	return pStage;	//動的確保したものを返す
}

//================================================
//Xモデル情報の取得
//================================================
CObjectX* CStage::GetObjectX(int nIdx)
{
	return m_apModel[nIdx];
}

//================================================
//ギミック情報の取得
//================================================
CGimmick* CStage::GetGimmick(int nIdx)
{
	return m_apGimmick[nIdx];
}

//================================================
//アイテム情報の取得
//================================================
CItem* CStage::GetItem()
{
	return m_pItem;
}

//================================================
//ドア情報の取得
//================================================
CDoor* CStage::GetDoor(int nIdx)
{
	return m_apDoor[nIdx];
}

//================================================
//コンストラクタ
//================================================
CStage::CStage() :
	m_stage(STAGE::NONE),
	m_stageNext(STAGE::NONE),
	m_nNumModel(0),
	m_nCntModelSet(0)
{
	//メンバ変数のクリア
	for (int i = 0; i < MAX_DOOR; i++)
	{
		m_aDir[i] = 0;
	}
}

//================================================
//デストラクタ
//================================================
CStage::~CStage()
{
}

//================================================
//初期化
//================================================
HRESULT CStage::Init(const STAGE &stage, const char* pStage)
{
	//メンバ変数の初期化
	m_stage = stage;
	m_stageNext = STAGE::NONE;
	m_nNumModel = 0;
	m_nCntModelSet = 0;

	for (int i = 0; i < MAX_DOOR; i++)
	{
		m_aDir[i] = -1;
	}

	//床と壁の生成
	CreateFloarAndWalls();

	//読み込み
	Load(pStage);

	return S_OK;
}

//================================================
//終了
//================================================
void CStage::Uninit()
{
	for (int i = 0; i < MAX_DOOR; i++)
	{
		if (m_apDoor[i] != nullptr)
		{//NULLチェック
			m_apDoor[i]->SetDeathFlag();	//死亡フラグの設定
			m_apDoor[i] = nullptr;			//nullptrにする
		}
	}

	if (m_pItem != nullptr)
	{//NULLチェック
		m_pItem->SetDeathFlag();	//死亡フラグの設定
		m_pItem = nullptr;			//nullptrにする
	}

	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (m_apGimmick[i] != nullptr)
		{//NULLチェック
			m_apGimmick[i]->SetDeathFlag();	//死亡フラグの設定
			m_apGimmick[i] = nullptr;		//nullptrにする
		}
	}

	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (m_apModel[i] != nullptr)
		{//NULLチェック
			m_apModel[i]->SetDeathFlag();	//死亡フラグの設定
			m_apModel[i] = nullptr;			//nullptrにする
		}
	}
}

//================================================
//全ての終了
//================================================
void CStage::UninitAll()
{
	for (int i = 0; i < MAX_DOOR; i++)
	{
		if (m_apDoor[i] != nullptr)
		{//NULLチェック
			m_apDoor[i]->SetDeathFlag();	//死亡フラグの設定
			m_apDoor[i] = nullptr;			//nullptrにする
		}
	}

	if (m_pItem != nullptr)
	{//NULLチェック
		m_pItem->SetDeathFlag();	//死亡フラグの設定
		m_pItem = nullptr;			//nullptrにする
	}

	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (m_apGimmick[i] != nullptr)
		{//NULLチェック
			m_apGimmick[i]->SetDeathFlag();	//死亡フラグの設定
			m_apGimmick[i] = nullptr;		//nullptrにする
		}
	}

	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (m_apModel[i] != nullptr)
		{//NULLチェック
			m_apModel[i]->SetDeathFlag();	//死亡フラグの設定
			m_apModel[i] = nullptr;			//nullptrにする
		}
	}

	for (int i = 0; i < DIRECTION::DIR_MAX; i++)
	{
		if (m_apWall[i] != nullptr)
		{//NULLチェック
			m_apWall[i]->SetDeathFlag();	//死亡フラグの設定
			m_apWall[i] = nullptr;			//nullptrにする
		}
	}

	if (m_pFloar != nullptr)
	{//NULLチェック
		m_pFloar->SetDeathFlag();	//死亡フラグの設定
		m_pFloar = nullptr;			//nullptrにする
	}
}

//================================================
//更新
//================================================
void CStage::Update()
{
	if (CGame::GetPlayer()->GetIsGetItem())
	{//プレイヤーがアイテムを取得したら
		if (m_pItem != nullptr)
		{//NULLチェック
			m_pItem->SetDeathFlag();	//死亡フラグの設定
			m_pItem = nullptr;			//nullptrにする
		}
	}

#ifdef _DEBUG
	//デバッグ表示
	CDebugProc::Print("\n《 Stage 》\n");
	CDebugProc::Print("現在のステージ:[%d]\n", m_stage);	//現在のステージ
#endif // _DEBUG
}

//================================================
//ステージの設定
//================================================
CStage* CStage::Set()
{
	if (m_stageNext == STAGE::NONE)
	{//次のステージが決まっていない
		return this;
	}

	//現在のステージを終了
	Uninit();

	m_stage = m_stageNext;		//ステージを変更
	m_stageNext = STAGE::NONE;	//何もない状態にする

	return Create(m_stage);	//変更後のステージを生成して返す
}

//================================================
//ステージの取得
//================================================
CStage::STAGE CStage::Get()
{
	return m_stage;
}

//================================================
//ステージの変更
//================================================
void CStage::Change(const STAGE &stage)
{
	assert(stage > STAGE::NONE && stage < STAGE::MAX);
	
	m_stageNext = stage;
}

//================================================
//ドアの方向の読み込み
//================================================
void CStage::LoadDoorDir(const STAGE &stage)
{
	//ファイルを開く
	FILE* pFile = fopen(s_apFileName[stage], "r");

	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		assert(false);
	}

	/* ファイルが開けた場合 */

	for (int i = 0; i < MAX_DOOR; i++)
	{
		m_aDir[i] = -1;
	}

	char aText[MAX_WORD] = {};	//テキスト格納用

	int nNumDoor = 0;	//ドアの数

	while (strncmp(&aText[0], "SCRIPT", 6) != 0)
	{//テキストの最初の行を読み込むまで繰り返す
		fgets(aText, MAX_WORD, pFile);	//1行丸ごと読み込む
	}

	while (strcmp(&aText[0], "END_SCRIPT") != 0)
	{//テキストの最終行を読み込むまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "MODELSET") == 0)
		{//モデルセット
			while (strcmp(&aText[0], "END_MODELSET") != 0)
			{//モデルセットが終わるまで読み込み続ける
				//文字を読み込む
				fscanf(pFile, "%s", &aText[0]);

				if (strcmp(&aText[0], "DIR") == 0)
				{//方向
					//「＝」を読み込む
					fscanf(pFile, "%s", &aText[0]);

					//方向を読み込む
					fscanf(pFile, "%d", &m_aDir[nNumDoor]);

					//ドアの数をカウントする
					nNumDoor++;
				}
			}
		}
	}
}

//================================================
//ドアの方向を取得
//================================================
int CStage::GetDoorDir(int nIdx)
{
	return m_aDir[nIdx];
}

//================================================
//読み込み
//================================================
void CStage::Load(const char* pStage)
{
	//ファイルを開く
	FILE* pFile = fopen(pStage, "r");
	
	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		assert(false);
	}

	/* ファイルが開けた場合 */

	char aText[MAX_WORD] = {};	//テキスト格納用

	while (strncmp(&aText[0], "SCRIPT", 6) != 0)
	{//テキストの最初の行を読み込むまで繰り返す
		fgets(aText, MAX_WORD, pFile);	//1行丸ごと読み込む
	}

	char aFileName[MAX_MODEL][MAX_WORD] = {};	//ファイルパス読み込み用

	int nCntFile = 0;	//ファイル数カウント

	while (strcmp(&aText[0], "END_SCRIPT") != 0)
	{//テキストの最終行を読み込むまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "NUM_MODEL") == 0)
		{//モデル数
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//モデル数を読み込む
			fscanf(pFile, "%d", &m_nNumModel);
		}
		else if (strcmp(&aText[0], "MODEL_FILENAME") == 0)
		{//ファイル名
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//Xファイルのパスを読み込む
			fscanf(pFile, "%s", &aFileName[nCntFile][0]);

			//カウントを増加
			nCntFile++;
		}
		else if (strcmp(&aText[0], "MODELSET") == 0)
		{//モデルセット
			//モデル設定
			Set_ModelSet(pFile);

			//カウントアップ
			m_nCntModelSet++;
		}
	}

	//ファイルを閉じる
	fclose(pFile);

	for (int i = 0, nNumModel = 0, nNumGimmick = 0, nNumDoor = 0; i < m_nCntModelSet; i++)
	{
		//インデックス数
		int nIndex = m_aModelSetInfo[i].nIndex;

		switch (m_aModelSetInfo[i].type)
		{
		case MODEL_TYPE::TYPE_OBJECT:	//オブジェクト
			//生成
			m_apModel[nNumModel] = CObjectX::Create(&aFileName[nIndex][0]);

			//位置・向きの設定
			m_apModel[nNumModel]->SetPos(m_aModelSetInfo[i].pos);
			m_apModel[nNumModel]->SetRot(m_aModelSetInfo[i].rot);

			//カウントアップ
			nNumModel++;
			break;

		case MODEL_TYPE::TYPE_GIMMICK:	//ギミック
			//生成
			m_apGimmick[nNumGimmick] = CGimmick::Create(&aFileName[nIndex][0]);

			//位置・向きの設定
			m_apGimmick[nNumGimmick]->SetPos(m_aModelSetInfo[i].pos);
			m_apGimmick[nNumGimmick]->SetRot(m_aModelSetInfo[i].rot);

			//カウントアップ
			nNumGimmick++;
			break;

		case MODEL_TYPE::TYPE_ITEM:	//アイテム
			for (int i = 0; i < MAX_COIN; i++)
			{
				//コインの獲得状況を取得
				STAGE stageCoin = CGame::GetPlayer()->GetStageCoin(i);

				if (stageCoin == m_stage)
				{//現在のステージのコインを獲得済の場合
					break;
				}
			}

			/* 獲得していない場合 */

			//生成
			m_pItem = CItem::Create(&aFileName[nIndex][0]);
			
			//位置・向きの設定
			m_pItem->SetPos(m_aModelSetInfo[i].pos);
			m_pItem->SetRot(m_aModelSetInfo[i].rot);

			//配置されたステージを設定
			m_pItem->SetIsStage(m_stage);
			break;

		case MODEL_TYPE::TYPE_DOOR:	//ドア
			//生成
			m_apDoor[nNumDoor] = CDoor::Create(&aFileName[nIndex][0]);

			//方向の設定
			m_apDoor[nNumDoor]->SetDir(m_aModelSetInfo[i].dir);

			//位置の設定
			m_apDoor[nNumDoor]->SetPos(POS_DOOR[m_aModelSetInfo[i].dir]);

			if (m_aModelSetInfo[i].dir == DIRECTION::DIR_LEFT ||
				m_aModelSetInfo[i].dir == DIRECTION::DIR_RIGHT)
			{//ドアの位置が [左側 or 右側] にある場合
				//向きの設定
				m_apDoor[nNumDoor]->SetRot(D3DXVECTOR3(0.0f, ROT_DOOR, 0.0f));
			}

			//カウントアップ
			nNumDoor++;
			break;

		case MODEL_TYPE::TYPE_NONE:	//その他
		case MODEL_TYPE::TYPE_MAX:
		default:
			assert(false);
			break;
		}
	}
}

//================================================
//モデルセット設定
//================================================
void CStage::Set_ModelSet(FILE* pFile)
{
	char aText[MAX_WORD] = {};	//テキスト格納用

	//ポインタに代入
	MODELSET_INFO* pInfo = &m_aModelSetInfo[m_nCntModelSet];

	while (strcmp(&aText[0], "END_MODELSET") != 0)
	{//モデルセットが終わるまで繰り返す
		//文字を読み込む
		fscanf(pFile, "%s", &aText[0]);

		if (strncmp(&aText[0], "#-", 2) == 0)
		{//ブロックコメント
			continue;
		}
		else if (strncmp(&aText[0], "#", 1) == 0)
		{//コメント
			//1行全て読み込む
			fgets(aText, MAX_WORD, pFile);
			continue;
		}

		if (strcmp(&aText[0], "INDEX") == 0)
		{//インデックス数
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//インデックス数を読み込む
			fscanf(pFile, "%d", &pInfo->nIndex);
		}
		else if (strcmp(&aText[0], "MODEL_TYPE") == 0)
		{//モデルタイプ
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			int nType = 0;	//タイプ読み込み用

			//モデルタイプを読み込む
			fscanf(pFile, "%d", &nType);

			switch (nType)
			{
			case 0:	//オブジェクト
				pInfo->type = MODEL_TYPE::TYPE_OBJECT;
				break;

			case 1:	//ギミック
				pInfo->type = MODEL_TYPE::TYPE_GIMMICK;
				break;

			case 2:	//アイテム
				pInfo->type = MODEL_TYPE::TYPE_ITEM;
				break;

			case 3:	//ドア
				pInfo->type = MODEL_TYPE::TYPE_DOOR;
				break;

			default:	//その他
				assert(false);
				break;
			}
		}
		else if (strcmp(&aText[0], "POS") == 0)
		{//位置
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//位置を読み込む
			fscanf(pFile, "%f", &pInfo->pos.x);
			fscanf(pFile, "%f", &pInfo->pos.y);
			fscanf(pFile, "%f", &pInfo->pos.z);
		}
		else if (strcmp(&aText[0], "ROT") == 0)
		{//向き
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//向きを読み込む
			fscanf(pFile, "%f", &pInfo->rot.x);
			fscanf(pFile, "%f", &pInfo->rot.y);
			fscanf(pFile, "%f", &pInfo->rot.z);
		}
		else if (strcmp(&aText[0], "DIR") == 0)
		{//方向
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			int nDir = 0;	//方向読み込み用

			//方向を読み込む
			fscanf(pFile, "%d", &nDir);

			switch (nDir)
			{
			case 0:	//左
				pInfo->dir = DIRECTION::DIR_LEFT;
				break;

			case 1:	//奥
				pInfo->dir = DIRECTION::DIR_BACK;
				break;

			case 2:	//右
				pInfo->dir = DIRECTION::DIR_RIGHT;
				break;

			case 3:	//手前
				pInfo->dir = DIRECTION::DIR_FRONT;
				break;

			default:	//その他
				assert(false);
				break;
			}
		}
	}
}

//================================================
//床と壁の設定
//================================================
void CStage::CreateFloarAndWalls()
{
	if (m_pFloar == nullptr)
	{//nullptrの場合
		//床の生成
		m_pFloar = CObject3D::Create();

		//床の設定
		m_pFloar->SetSize(D3DXVECTOR3(FLOAR_SIZE, 0.0f, FLOAR_SIZE));

		//テクスチャの設定
		m_pFloar->SetTexture(CTexture::TEXTURE::WallAndFloar);
	}

	int nCntCreate = 0;	//生成数カウント用

	for (int i = 0; i < DIRECTION::DIR_MAX; i++)
	{
		if (m_apWall[i] != nullptr)
		{//nullptrではない場合
			continue;
		}

		/* nullptrの場合 */

		//壁の生成
		m_apWall[i] = CObject3D::Create();

		//テクスチャの設定
		m_apWall[i]->SetTexture(CTexture::TEXTURE::WallAndFloar);

		//カウントアップ
		nCntCreate++;
	}

	if (nCntCreate >= DIRECTION::DIR_MAX)
	{//壁が全て生成出来ていたら
		//壁の設定
		SetWall();

		//カウントリセット
		nCntCreate = 0;
	}
}

//================================================
//壁の設定
//================================================
void CStage::SetWall()
{
	//説明用変数
	float fFloarSizeHalf = (FLOAR_SIZE * 0.5f);		//床サイズの半分
	float fWallHeightHalf = (WALL_HEIGHT * 0.5f);	//壁の高さの半分

	//********** 左 **********//

	//生成する方向
	DIRECTION dir = DIRECTION::DIR_LEFT;

	//サイズ
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_HEIGHT, 0.0f, WALL_WIDTH));

	//位置
	D3DXVECTOR3 pos = D3DXVECTOR3(
		m_pFloar->GetPos().x - fFloarSizeHalf,
		m_pFloar->GetPos().y + fWallHeightHalf,
		0.0f);
	m_apWall[dir]->SetPos(pos);

	//向き
	m_apWall[dir]->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f));

	//********** 奥 **********//

	//生成する方向
	dir = DIRECTION::DIR_BACK;

	//サイズ
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_WIDTH, 0.0f, WALL_HEIGHT));

	//位置
	pos = D3DXVECTOR3(
		0.0f,
		m_pFloar->GetPos().y + fWallHeightHalf,
		m_pFloar->GetPos().z + fFloarSizeHalf);
	m_apWall[dir]->SetPos(pos);

	//向き
	m_apWall[dir]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));

	//********** 右 **********//

	//生成する方向
	dir = DIRECTION::DIR_RIGHT;

	//サイズ
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_HEIGHT, 0.0f, WALL_WIDTH));

	//位置
	pos = D3DXVECTOR3(
		m_pFloar->GetPos().x + fFloarSizeHalf,
		m_pFloar->GetPos().y + fWallHeightHalf,
		0.0f);
	m_apWall[dir]->SetPos(pos);

	//向き
	m_apWall[dir]->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));

	//********** 手前 **********//

	//生成する方向
	dir = DIRECTION::DIR_FRONT;

	//サイズ
	m_apWall[dir]->SetSize(D3DXVECTOR3(WALL_WIDTH, 0.0f, WALL_HEIGHT));

	//位置
	pos = D3DXVECTOR3(
		0.0f,
		m_pFloar->GetPos().y + fWallHeightHalf,
		m_pFloar->GetPos().z - fFloarSizeHalf);
	m_apWall[dir]->SetPos(pos);

	//向き
	m_apWall[dir]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));

	//色
	m_apWall[dir]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
}