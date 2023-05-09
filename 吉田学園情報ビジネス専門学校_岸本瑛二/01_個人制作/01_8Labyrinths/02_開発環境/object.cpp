//================================================
//
//3Dゲーム(仮)[object.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "object.h"
#include "renderer.h"

//***************************
//静的メンバ変数
//***************************
CObject* CObject::m_apTop[PRIORITY::PRIO_MAX] = {};		//先頭のオブジェクトのポインタ
CObject* CObject::m_apCurrent[PRIORITY::PRIO_MAX] = {};	//現在(末尾)のオブジェクトのポインタ

int CObject::m_nNumAll = 0;	//最大数

//================================================
//全ての解放
//================================================
void CObject::ReleaseAll()
{
	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		//先頭のオブジェクトを保存
		CObject* pObject = m_apTop[i];

		while (pObject)
		{//pObjがnullptrになるまで
			//「保存したオブジェクト」の「次のオブジェクト」を保存
			CObject* pNext = pObject->m_pNext;

			//解放
			pObject->Release();

			//「次のオブジェクト」に変更
			pObject = pNext;
		}

		//先頭のオブジェクトを保存
		pObject = m_apTop[i];

		while (pObject)
		{
			//「保存したオブジェクト」の「次のオブジェクト」を保存
			CObject* pNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{//死亡フラグが立っている場合
				//終了
				pObject->Uninit();
			}

			//「次のオブジェクト」に変更
			pObject = pNext;
		}
	}
}

//================================================
//全ての更新
//================================================
void CObject::UpdateAll()
{
	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		//先頭のオブジェクトを保存
		CObject* pObject = m_apTop[i];

		while (pObject)
		{//pObjがnullptrになるまで
			//「保存したオブジェクト」の「次のオブジェクト」を保存
			CObject* pNext = pObject->m_pNext;

			//更新
			pObject->Update();

			//「次のオブジェクト」に変更
			pObject = pNext;
		}

		//先頭のオブジェクトを保存
		pObject = m_apTop[i];

		while (pObject)
		{
			//「保存したオブジェクト」の「次のオブジェクト」を保存
			CObject* pNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{//死亡フラグが立っている場合
				//終了
				pObject->Uninit();

				//解放
				pObject->Release();
			}

			//「次のオブジェクト」に変更
			pObject = pNext;
		}
	}
}

//================================================
//全ての描画
//================================================
void CObject::DrawAll()
{
	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		//先頭のオブジェクトを保存
		CObject* pObject = m_apTop[i];

		while (pObject)
		{//pObjがnullptrになるまで
			//「保存したオブジェクト」の「次のオブジェクト」を保存
			CObject* pNext = pObject->m_pNext;

			//描画
			pObject->Draw();

			//「次のオブジェクト」に変更
			pObject = pNext;
		}
	}
}
//================================================
//デフォルトコンストラクタ
//================================================
CObject::CObject()
{
}

//================================================
//コンストラクタ
//================================================
CObject::CObject(const PRIORITY &priority)
{
	if (m_apTop[priority] == nullptr)
	{//オブジェクトが一つも無い場合
		//自身を先頭として登録
		m_apTop[priority] = this;

		//末尾に自身(先頭)を入れる
		m_apCurrent[priority] = m_apTop[priority];

		//総数を一つ増やす
		m_nNumAll++;
		return;
	}

	/* オブジェクトが1つ以上ある場合 */

	//末尾の次に自身を入れる
	m_apCurrent[priority]->m_pNext = this;

	//自身が一番後ろになる
	this->m_pPrev = m_apCurrent[priority];

	//末尾に自身を入れる
	m_apCurrent[priority] = this;

	//総数を一つ増やす
	m_nNumAll++;
	return;
}

//================================================
//デストラクタ
//================================================
CObject::~CObject()
{
}

//================================================
//解放
//================================================
void CObject::Release()
{
	if (this == nullptr)
	{//NULLチェック
		return;
	}

	/* nullptrではない場合 */

	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		if (this->m_pPrev != nullptr)
		{//自身の前が存在している場合
			//「自身の前が持つ、次の情報」に、「自身の次の情報」を接続
			this->m_pPrev->m_pNext = this->m_pNext;
		}

		if (this->m_pNext != nullptr)
		{//自身の次が存在している場合
			//「自身の次が持つ、前の情報」に、「自身の前の情報」を接続
			this->m_pNext->m_pPrev = this->m_pPrev;
		}

		if (this->m_pPrev == nullptr)
		{//自身の前に何も無い場合(自身が先頭)
			//先頭に、自身の次の情報を設定
			m_apTop[i] = this->m_pNext;
		}

		if (this->m_pNext == nullptr)
		{//自身の次に何も無い場合(自身が末尾)
			//末尾に、自身の前の情報を設定
			m_apCurrent[i] = this->m_pPrev;
		}
	}

	//総数を一つ減らす
	m_nNumAll--;
}

//================================================
//死亡フラグの設定
//================================================
void CObject::SetDeathFlag()
{
	//死亡フラグを立てる
	this->m_bDeath = true;
}

//================================================
//タイプの設定
//================================================
void CObject::SetObjType(const CObject::OBJ_TYPE &type)
{
	m_objType = type;
}

//================================================
//タイプの取得
//================================================
CObject::OBJ_TYPE CObject::GetObjType()
{
	return m_objType;
}