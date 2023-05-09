//================================================
//
//3D�Q�[��(��)[utility.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "utility.h"

//================================================
//�p�x�̐��K��
//================================================
void NormalizeAngle(float* pAngle)
{
	if (*pAngle >= D3DX_PI)
	{//�p�x��[3.14]�ȏ�̏ꍇ
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle <= -D3DX_PI)
	{//�p�x��[-3.14]�ȉ��̏ꍇ
		*pAngle += D3DX_PI * 2.0f;
	}
}

//================================================
//�����蔻��(���f�����m)
//================================================
bool CollisionModel(
	D3DXVECTOR3* pPosOwn,
	const D3DXVECTOR3 &posOldOwn,
	const D3DXVECTOR3 &posTarget,
	const D3DXVECTOR3 &sizeOwn,
	const D3DXVECTOR3 &sizeTarget)
{
	//********** �����p�ϐ� **********//

	//���g�̃T�C�Y�̔���
	D3DXVECTOR3 sizeHalfOwn = D3DXVECTOR3(
		sizeOwn.x * 0.5f,
		sizeOwn.y * 0.5f,
		sizeOwn.z * 0.5f
	);

	//���g�̔���̍ő�l�E�ŏ��l(���݂̈ʒu)
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

	//���g�̔���̍ő�l�E�ŏ��l(�O��̈ʒu)
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

	//�Ώۂ̔���̍ő�l�E�ŏ��l
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

	//X���̔��莮
	bool bRightToLeft = (ownMin.x < targetMax.x);	//�E���獶��
	bool bLeftToRight = (ownMax.x > targetMin.x);	//������E��

	//Y���̔��莮
	bool bTopToBottom = (ownMin.y < targetMax.y);	//�ォ�牺��
	bool bBottomToTop = (ownMax.y > targetMin.y);	//��������

	//Z���̔��莮
	bool bFrontToBack = (ownMax.z > targetMin.z);	//��O���牜��
	bool bBackToFront = (ownMin.z < targetMax.z);	//�������O��

	bool bCollision = false;	//�����������ǂ���

	//********** �����蔻�� **********//

	if (bTopToBottom && bBottomToTop)
	{//Y���œ������Ă���ꍇ
		if (bFrontToBack && bBackToFront)
		{//Z���œ������Ă���ꍇ
			if ((oldOwnMax.x <= targetMin.x) && bLeftToRight)
			{//���g���ΏۂɁu������E�֌������ē��������v�ꍇ
				//�����o��
				pPosOwn->x = (targetMin.x - sizeHalfOwn.x);

				//�u���������v�����Ԃ�
				bCollision =  true;
			}
			else if ((oldOwnMin.x >= targetMax.x) && bRightToLeft)
			{//���g���ΏۂɁu�E���獶�֌������ē��������v�ꍇ
				//�����o��
				pPosOwn->x = (targetMax.x + sizeHalfOwn.x);

				//�u���������v�����Ԃ�
				bCollision = true;
			}
		}
		
		if (bLeftToRight && bRightToLeft)
		{//X���œ������Ă���ꍇ
			if ((oldOwnMax.z <= targetMin.z) && bFrontToBack)
			{//���g���ΏۂɁu��O���牜�֌������ē��������v�ꍇ
				//�����o��
				pPosOwn->z = (targetMin.z - sizeHalfOwn.z);

				//�u���������v�����Ԃ�
				bCollision = true;
			}
			else if ((oldOwnMin.z >= targetMax.z) && bBackToFront)
			{//���g���ΏۂɁu�������O�֌������ē��������v�ꍇ
				//�����o��
				pPosOwn->z = (targetMax.z + sizeHalfOwn.z);

				//�u���������v�����Ԃ�
				bCollision = true;
			}
		}
	}

	if ((bLeftToRight && bRightToLeft) && (bFrontToBack && bBackToFront))
	{//X���AZ���œ������Ă���ꍇ
		if ((oldOwnMax.y <= targetMin.y) && bBottomToTop)
		{//���g���ΏۂɁu�������֌������ē��������v�ꍇ
			//�����o��
			pPosOwn->y = (targetMin.y - sizeOwn.y);

			//�u���������v�����Ԃ�
			bCollision = true;
		}
		else if ((oldOwnMin.y >= targetMax.y) && bTopToBottom)
		{//���g���ΏۂɁu�ォ�牺�֌������ē��������v�ꍇ
			//�����o��
			pPosOwn->y = targetMax.y;

			//�u���������v�����Ԃ�
			bCollision = true;
		}
	}

	return bCollision;	//�u�������Ă��Ȃ��v�����Ԃ�
}