//================================================
//
//3Dゲーム(仮)[model.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "model.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>

//***************************
//定数の定義
//***************************
const int CModel::MAX_WORD = 256;	//最大文字数

const char* CModel::FILE_NAME = "data/TEXT/motion.txt";	//ファイル名

//***************************
//静的メンバ変数
//***************************
CModel::CHARACTER_SET CModel::m_characterSet = {};	//キャラクター情報

CModel::MODEL_INFO CModel::m_aModelInfo[NUM_PARTS] = {};	//モデル情報

//================================================
//生成
//================================================
CModel* CModel::Create()
{
	CModel* pModel = nullptr;	//ポインタ

	if (pModel != nullptr)
	{//NULLチェック
		assert(false);
	}

	/* nullptrの場合 */

	pModel = new CModel;	//メモリの動的確保

	pModel->Init();	//初期化

	return pModel;	//動的確保したものを返す
}

//================================================
//コンストラクタ
//================================================
CModel::CModel():
	m_nNumModel(0)
{
	//メンバ変数のクリア
	memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
}

//================================================
//デストラクタ
//================================================
CModel::~CModel()
{
}

//================================================
//初期化
//================================================
HRESULT CModel::Init()
{
	//メンバ変数の初期設定
	m_nNumModel = 0;

	//ファイル読み込み
	Load();

	for (int i = 0; i < m_nNumModel; i++)
	{
		//モデル情報の位置と向きに、キャラクター設定の位置と向きを代入
		m_aModelInfo[i].pos = m_characterSet.aPartsSet[i].pos;
		m_aModelInfo[i].rot = m_characterSet.aPartsSet[i].rot;

		//頂点数の取得
		int nNumVtx = m_aModelInfo[i].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		DWORD sizeFVF = D3DXGetFVFVertexSize(m_aModelInfo[i].pMesh->GetFVF());

		BYTE *pVtxBuff;	//頂点バッファへのポインタ

		//頂点バッファのロック
		m_aModelInfo[i].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int j = 0; j < nNumVtx; j++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			/* 頂点座標を比較し、モデルの最大値・最小値を取得 */

			//***** 最大 *****//

			if (vtx.x > m_aModelInfo[i].vtxMax.x)
			{//X
				m_aModelInfo[i].vtxMax.x = vtx.x;
			}

			if (vtx.y > m_aModelInfo[i].vtxMax.y)
			{//Y
				m_aModelInfo[i].vtxMax.y = vtx.y;
			}

			if (vtx.z > m_aModelInfo[i].vtxMax.z)
			{//Z
				m_aModelInfo[i].vtxMax.z = vtx.z;
			}

			//***** 最小 *****//

			if (vtx.x < m_aModelInfo[i].vtxMin.x)
			{//X
				m_aModelInfo[i].vtxMin.x = vtx.x;
			}

			if (vtx.y < m_aModelInfo[i].vtxMin.y)
			{//Y
				m_aModelInfo[i].vtxMin.y = vtx.y;
			}

			if (vtx.z < m_aModelInfo[i].vtxMin.z)
			{//Z
				m_aModelInfo[i].vtxMin.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		m_aModelInfo[i].pMesh->UnlockVertexBuffer();
	}

	return S_OK;
}

//================================================
//終了
//================================================
void CModel::Uninit()
{
	for (int i = 0; i < m_nNumModel; i++)
	{
		//メッシュの解放
		if (m_aModelInfo[i].pMesh != nullptr)
		{
			m_aModelInfo[i].pMesh = nullptr;
		}

		//マテリアルの解放
		if (m_aModelInfo[i].pBuffMat != nullptr)
		{
			m_aModelInfo[i].pBuffMat = nullptr;
		}
	}
}

//================================================
//更新
//================================================
void CModel::Update()
{
}

//================================================
//描画
//================================================
void CModel::Draw()
{
	for (int i = 0; i < m_nNumModel; i++)
	{
		//キャラクター設定の情報
		D3DXVECTOR3 pos = m_characterSet.aPartsSet[i].pos + m_aModelInfo[i].pos;	//位置
		D3DXVECTOR3 rot = m_characterSet.aPartsSet[i].rot + m_aModelInfo[i].rot;	//向き

		D3DXMATRIX mtxRot, mtxTrans, mtxParent;	//計算用マトリックス

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//モデルの影を描画
		//DrawShadow();

		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		if(m_characterSet.aPartsSet[i].nParent == -1)
		{//自身が親モデルの場合
			//現在のマトリックスを取得
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		//ワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		D3DMATERIAL9 matDef;	//現在のマテリアル保存用
		D3DXMATERIAL* pMat;		//マテリアルデータへのポインタ

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを保持
		pMat = (D3DXMATERIAL*)m_aModelInfo[i].pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)m_aModelInfo[i].numMat; j++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			//モデルパーツの描画
			m_aModelInfo[i].pMesh->DrawSubset(j);
		}

		//保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//================================================
//ワールドマトリックスの取得
//================================================
D3DXMATRIX CModel::GetMtxWorld()
{
	return m_mtxWorld;
}

//================================================
//モデルの各情報の取得
//================================================
CModel::MODEL_INFO CModel::GetModelInfo(int nNum)
{
	return m_aModelInfo[nNum];
}

//================================================
//モデルの位置情報を設定
//================================================
void CModel::SetModelInfo_Pos(int nNum, const D3DXVECTOR3 &pos)
{
	m_aModelInfo[nNum].pos = pos;
}

//================================================
//モデルの向き情報を設定
//================================================
void CModel::SetModelInfo_Rot(int nNum, const D3DXVECTOR3 &rot)
{
	m_aModelInfo[nNum].rot = rot;
}

//================================================
//影の描画
//================================================
void CModel::DrawShadow()
{
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, nor;

	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトの方向を設定
	vecLight = D3DXVECTOR4(-1.0f, 0.8f, 0.0f, 0.0f);

	//位置を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線を設定
	nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線と平面の1点から平面を作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &nor);

	//ライトと平面から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//ワールドマトリックスとシャドウマトリックスを掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &m_mtxWorld);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < m_nNumModel; i++)
	{
		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		D3DMATERIAL9 matDef;	//現在のマテリアル保存用
		D3DXMATERIAL* pMat;		//マテリアルデータへのポインタ

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを保持
		pMat = (D3DXMATERIAL*)m_aModelInfo[i].pBuffMat->GetBufferPointer();
		D3DMATERIAL9 mat = pMat->MatD3D;

		//色を黒に設定
		mat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		mat.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//マテリアルの設定
		pDevice->SetMaterial(&mat);

		for (int i = 0; i < (int)m_aModelInfo[i].numMat; i++)
		{
			//モデルパーツの描画
			m_aModelInfo[i].pMesh->DrawSubset(i);
		}

		//保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//================================================
//読み込み
//================================================
void CModel::Load()
{
	//ファイルを開く
	FILE* pFile = fopen(FILE_NAME, "r");
	
	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		assert(false);
	}

	/* ファイルが開けた場合 */

	char aText[MAX_WORD];	//テキスト格納用

	while (strncmp(&aText[0], "SCRIPT", 6) != 0)
	{//テキストの最初の行を読み込むまで繰り返す
		fgets(aText, MAX_WORD, pFile);	//1行丸ごと読み込む
	}

	char aFileName[NUM_PARTS][MAX_WORD] = {};
	int nCnt = 0;

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
			fscanf(pFile, "%s", aText);

			//Xファイルのパスを読み込む
			fscanf(pFile, "%s", &aFileName[nCnt][0]);

			//カウントアップ
			nCnt++;
		}
		else if (strcmp(&aText[0], "CHARACTERSET") == 0)
		{//キャラクターセット
			//キャラクター設定
			Set_CharacterSet(pFile);
		}
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < m_nNumModel; i++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(
			&aFileName[i][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_aModelInfo[i].pBuffMat,
			NULL,
			&m_aModelInfo[i].numMat,
			&m_aModelInfo[i].pMesh);
	}

	//ファイルを閉じる
	fclose(pFile);
}

//================================================
//キャラクター設定
//================================================
void CModel::Set_CharacterSet(FILE* pFile)
{
	int nCnt = 0;
	char aText[MAX_WORD];	//テキスト格納用

	while (strcmp(&aText[0], "END_CHARACTERSET") != 0)
	{//キャラクターセットが終わるまで繰り返す
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

		if (strcmp(&aText[0], "NUM_PARTS") == 0)
		{//パーツ数
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//キー数を読み込む
			fscanf(pFile, "%d", &m_characterSet.nNumParts);
		}
		else if (strcmp(&aText[0], "PARTSSET") == 0)
		{//パーツセット
			//パーツセット設定
			Set_PartsSet(pFile, &m_characterSet.aPartsSet[nCnt]);

			//パーツ数カウントを増加
			nCnt++;
		}
	}
}

//================================================
//パーツ設定
//================================================
void CModel::Set_PartsSet(FILE* pFile, PARTS_SET* pParts)
{
	char aText[MAX_WORD];	//テキスト格納用

	while (strcmp(&aText[0], "END_PARTSSET") != 0)
	{//キャラクターセットが終わるまで繰り返す
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
			fscanf(pFile, "%d", &pParts->nIndex);
		}
		else if (strcmp(&aText[0], "PARENT") == 0)
		{//親パーツ番号
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//親パーツ番号を読み込む
			fscanf(pFile, "%d", &pParts->nParent);
		}
		else if (strcmp(&aText[0], "POS") == 0)
		{//位置
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//位置を読み込む
			fscanf(pFile, "%f", &pParts->pos.x);
			fscanf(pFile, "%f", &pParts->pos.y);
			fscanf(pFile, "%f", &pParts->pos.z);
		}
		else if (strcmp(&aText[0], "ROT") == 0)
		{//向き
			//「＝」を読み込む
			fscanf(pFile, "%s", &aText[0]);

			//向きを読み込む
			fscanf(pFile, "%f", &pParts->rot.x);
			fscanf(pFile, "%f", &pParts->rot.y);
			fscanf(pFile, "%f", &pParts->rot.z);
		}
	}
}