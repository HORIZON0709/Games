//============================================================================================================
//
//�N�������˂Ăɂ��痢[map.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "map.h"
#include "block.h"
#include "item.h"
#include "enemy.h"
#include "goal.h"

#include <assert.h>

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static float s_fMapSizeX, s_fMapSizeY;		//�}�b�v�̃T�C�YX,Y
static int s_aMap[MAX_Y][MAX_X];			//�}�b�v

//============================================================================================================
//�}�b�v�̏���������
//============================================================================================================
void SetMap(char FileName[MAX_FILENAME])
{
	FILE *pFile;			//�t�@�C���|�C���^
	int nCntX, nCntY;		//�J�E���g(X,Y)
	int nCntSize = 0;		//�J�E���g(�T�C�Y)

	//�}�b�v�̃T�C�Y���v�Z
	s_fMapSizeX = (float)(SCREEN_WIDTH * SCREEN_NUMBER) / MAX_X;
	s_fMapSizeY = (float)(SCREEN_HEIGHT / MAX_Y);

	//�t�@�C�����J��
	pFile = fopen(&FileName[0],"r");

	if (pFile != NULL)
	{//�t�@�C�����J������
		for (nCntY = 0; nCntY < MAX_Y; nCntY++)
		{
			for (nCntX = 0; nCntX < MAX_X; nCntX++)
			{//�}�b�v�̓ǂݍ���
				fscanf(pFile, "%d\n", &s_aMap[nCntY][nCntX]);
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	for (nCntY = 0; nCntY < MAX_Y; nCntY++)
	{
		for (nCntX = 0; nCntX < MAX_X; nCntX++)
		{//�}�b�v�̐ݒ�
			switch (s_aMap[nCntY][nCntX])
			{
			case OBJECT_NONE:	//�����z�u���Ȃ�
				break;

			case OBJECT_BLOCK_HORIZONTAL:	//�u���b�N(��)

				if (nCntX < MAX_X)
				{//X���ő�l�����̂Ƃ�
					if (s_aMap[nCntY][nCntX + 1] == OBJECT_BLOCK_HORIZONTAL)
					{
						nCntSize++;		//�傫���𑪂�
					}
					else
					{
						//������ƒZ������
						float fWidth  = (BLOCK_WIDTH / 2);
						float fHeight = (BLOCK_HEIGHT / 2);

						//�u���b�N�̐ݒu
						SetBlock(D3DXVECTOR3((((s_fMapSizeX * nCntX) + fWidth) - (nCntSize * fWidth)),
											((s_fMapSizeY * nCntY) + fHeight),
											0.0f),						//�ʒu
								 D3DXVECTOR3(BLOCK_MOVE, 0.0f, 0.0f),	//�ړ���
								 fWidth + (nCntSize * fWidth),			//��
								 fHeight);								//����

						nCntSize = 0;	//�T�C�Y�����Z�b�c
					}
				}
				break;

			case OBJECT_ITEM:	//�A�C�e��
			{
				//������ƒZ������
				float fWidth  = (ITEM_WIDTH / 2);
				float fHeight = (ITEM_HEIGHT / 2);

				//�A�C�e���̐ݒu
				SetItem(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth),
									(s_fMapSizeY * nCntY + fHeight), 
									0.0f),						//�ʒu
						D3DXVECTOR3(ITEM_MOVE, 0.0f, 0.0f),		//�ړ���
						ITEM_TYPE_COIN);						//���

			}
				break;

			case OBJECT_ENEMY_GROUND:	//�G(�n��)
			{
				//������ƒZ������
				float fWidth = (ENEMY_WIDTH / 2);
				float fHeight = (ENEMY_HEIGHT / 2);

				SetEnemy(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth),
									 (s_fMapSizeY * nCntY + fHeight) - fHeight,
									 0.0f),							//�ʒu
						D3DXVECTOR3(ENEMY_GROUND_MOVE, 0.0f, 0.0f),	//�ړ���
						MOVETYPE_GROUND);							//������
			}
				break;

			case OBJECT_ENEMY_ONBLOCK:	//�G(�u���b�N�̏�)
			{
				//������ƒZ������
				float fWidth = (ENEMY_WIDTH / 2);
				float fHeight = (ENEMY_HEIGHT / 2);

				SetEnemy(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth),
									 (s_fMapSizeY * nCntY + fHeight) - fHeight,
									 0.0f),								//�ʒu
						D3DXVECTOR3(ENEMY_ONBLOCK_MOVE, 0.0f, 0.0f),	//�ړ���
						MOVETYPE_ONBLOCK);								//������
			}
				break;

			case OBJECT_GOAL:	//�S�[��
			{
				//������ƒZ������
				float fWidth = (GOAL_WIDTH / 2);
				float fHeight = (GOAL_HEIGHT / 2);

				SetGoal(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth) - 6.0f,	//�Y���������Ă�
									(s_fMapSizeY * nCntY + fHeight) - (fHeight * 0.6f),
									0.0f));		//�ʒu
			}
				break;

			case OBJECT_BLOCK_VERTICAL:		//�u���b�N(�c)
				break;

			default:
				assert(false);
				break;
			}
		}
	}

	for (nCntX = 0; nCntX < MAX_X; nCntX++)
	{
		for (nCntY = 0; nCntY < MAX_Y; nCntY++)
		{
			switch (s_aMap[nCntY][nCntX])
			{
			case OBJECT_NONE:				//�����z�u���Ȃ�
			case OBJECT_BLOCK_HORIZONTAL:	//�u���b�N(��)
			case OBJECT_ITEM:				//�A�C�e��
			case OBJECT_ENEMY_GROUND:		//�G(�n��)
			case OBJECT_ENEMY_ONBLOCK:		//�G(�u���b�N�̏�)
			case OBJECT_GOAL:				//�S�[��
				break;

			case OBJECT_BLOCK_VERTICAL:		//�u���b�N(�c)

				if (nCntY < MAX_Y)
				{//Y���ő�l�����̂Ƃ�
					if (s_aMap[nCntY + 1][nCntX] == OBJECT_BLOCK_VERTICAL)
					{
						nCntSize++;		//�傫���𑪂�
					}
					else
					{
						//������ƒZ������
						float fWidth = (BLOCK_WIDTH / 2);
						float fHeight = (BLOCK_HEIGHT / 2);

						//�u���b�N�̐ݒu
						SetBlock(D3DXVECTOR3(((s_fMapSizeX * nCntX) + fWidth),
											(((s_fMapSizeY * nCntY) + fHeight) - (nCntSize * fHeight)),
											0.0f),									//�ʒu
											D3DXVECTOR3(BLOCK_MOVE, 0.0f, 0.0f),	//�ړ���
											fWidth,									//��
											fHeight + (nCntSize * fHeight));		//����

						nCntSize = 0;	//�T�C�Y�����Z�b�c
					}
				}
				break;

			default:
				assert(false);
				break;
			}
		}
	}
}