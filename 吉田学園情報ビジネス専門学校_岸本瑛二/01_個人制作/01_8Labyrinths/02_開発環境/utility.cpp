//================================================
//
//3Dゲーム(仮)[utility.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//インクルード
//***************************
#include "utility.h"

//================================================
//角度の正規化
//================================================
void NormalizeAngle(float* pAngle)
{
	if (*pAngle >= D3DX_PI)
	{//角度が[3.14]以上の場合
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle <= -D3DX_PI)
	{//角度が[-3.14]以下の場合
		*pAngle += D3DX_PI * 2.0f;
	}
}

//================================================
//当たり判定(モデル同士)
//================================================
bool CollisionModel(
	D3DXVECTOR3* pPosOwn,
	const D3DXVECTOR3 &posOldOwn,
	const D3DXVECTOR3 &posTarget,
	const D3DXVECTOR3 &sizeOwn,
	const D3DXVECTOR3 &sizeTarget)
{
	//********** 説明用変数 **********//

	//自身のサイズの半分
	D3DXVECTOR3 sizeHalfOwn = D3DXVECTOR3(
		sizeOwn.x * 0.5f,
		sizeOwn.y * 0.5f,
		sizeOwn.z * 0.5f
	);

	//自身の判定の最大値・最小値(現在の位置)
	D3DXVECTOR3 ownMax = D3DXVECTOR3(
		pPosOwn->x + sizeHalfOwn.x,
		pPosOwn->y + sizeOwn.y,
		pPosOwn->z + sizeHalfOwn.z
		);

	D3DXVECTOR3 ownMin = D3DXVECTOR3(
		pPosOwn->x - sizeHalfOwn.x,
		pPosOwn->y,
		pPosOwn->z - sizeHalfOwn.z
	);;

	//自身の判定の最大値・最小値(前回の位置)
	D3DXVECTOR3 oldOwnMax = D3DXVECTOR3(
		posOldOwn.x + sizeHalfOwn.x,
		posOldOwn.y + sizeOwn.y,
		posOldOwn.z + sizeHalfOwn.z
	);

	D3DXVECTOR3 oldOwnMin = D3DXVECTOR3(
		posOldOwn.x - sizeHalfOwn.x,
		posOldOwn.y,
		posOldOwn.z - sizeHalfOwn.z
	);

	//対象の判定の最大値・最小値
	D3DXVECTOR3 targetMax = D3DXVECTOR3(
		posTarget.x + (sizeTarget.x * 0.5f),
		posTarget.y + sizeTarget.y,
		posTarget.z + (sizeTarget.z * 0.5f)
	);

	D3DXVECTOR3 targetMin = D3DXVECTOR3(
		posTarget.x - (sizeTarget.x * 0.5f),
		posTarget.y,
		posTarget.z - (sizeTarget.z * 0.5f)
	);;

	//X軸の判定式
	bool bRightToLeft = (ownMin.x < targetMax.x);	//右から左へ
	bool bLeftToRight = (ownMax.x > targetMin.x);	//左から右へ

	//Y軸の判定式
	bool bTopToBottom = (ownMin.y < targetMax.y);	//上から下へ
	bool bBottomToTop = (ownMax.y > targetMin.y);	//下から上へ

	//Z軸の判定式
	bool bFrontToBack = (ownMax.z > targetMin.z);	//手前から奥へ
	bool bBackToFront = (ownMin.z < targetMax.z);	//奥から手前へ

	bool bCollision = false;	//当たったかどうか

	//********** 当たり判定 **********//

	if (bTopToBottom && bBottomToTop)
	{//Y軸で当たっている場合
		if (bFrontToBack && bBackToFront)
		{//Z軸で当たっている場合
			if ((oldOwnMax.x <= targetMin.x) && bLeftToRight)
			{//自身が対象に「左から右へ向かって当たった」場合
				//押し出す
				pPosOwn->x = (targetMin.x - sizeHalfOwn.x);

				//「当たった」判定を返す
				bCollision =  true;
			}
			else if ((oldOwnMin.x >= targetMax.x) && bRightToLeft)
			{//自身が対象に「右から左へ向かって当たった」場合
				//押し出す
				pPosOwn->x = (targetMax.x + sizeHalfOwn.x);

				//「当たった」判定を返す
				bCollision = true;
			}
		}
		
		if (bLeftToRight && bRightToLeft)
		{//X軸で当たっている場合
			if ((oldOwnMax.z <= targetMin.z) && bFrontToBack)
			{//自身が対象に「手前から奥へ向かって当たった」場合
				//押し出す
				pPosOwn->z = (targetMin.z - sizeHalfOwn.z);

				//「当たった」判定を返す
				bCollision = true;
			}
			else if ((oldOwnMin.z >= targetMax.z) && bBackToFront)
			{//自身が対象に「奥から手前へ向かって当たった」場合
				//押し出す
				pPosOwn->z = (targetMax.z + sizeHalfOwn.z);

				//「当たった」判定を返す
				bCollision = true;
			}
		}
	}

	if ((bLeftToRight && bRightToLeft) && (bFrontToBack && bBackToFront))
	{//X軸、Z軸で当たっている場合
		if ((oldOwnMax.y <= targetMin.y) && bBottomToTop)
		{//自身が対象に「下から上へ向かって当たった」場合
			//押し出す
			pPosOwn->y = (targetMin.y - sizeOwn.y);

			//「当たった」判定を返す
			bCollision = true;
		}
		else if ((oldOwnMin.y >= targetMax.y) && bTopToBottom)
		{//自身が対象に「上から下へ向かって当たった」場合
			//押し出す
			pPosOwn->y = targetMax.y;

			//「当たった」判定を返す
			bCollision = true;
		}
	}

	return bCollision;	//「当たっていない」判定を返す
}