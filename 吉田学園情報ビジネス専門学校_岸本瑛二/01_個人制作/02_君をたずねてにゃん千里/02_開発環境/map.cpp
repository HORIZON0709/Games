//============================================================================================================
//
//君をたずねてにゃん千里[map.cpp]
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
//スタティック変数
//============================================================================================================
static float s_fMapSizeX, s_fMapSizeY;		//マップのサイズX,Y
static int s_aMap[MAX_Y][MAX_X];			//マップ

//============================================================================================================
//マップの初期化処理
//============================================================================================================
void SetMap(char FileName[MAX_FILENAME])
{
	FILE *pFile;			//ファイルポインタ
	int nCntX, nCntY;		//カウント(X,Y)
	int nCntSize = 0;		//カウント(サイズ)

	//マップのサイズを計算
	s_fMapSizeX = (float)(SCREEN_WIDTH * SCREEN_NUMBER) / MAX_X;
	s_fMapSizeY = (float)(SCREEN_HEIGHT / MAX_Y);

	//ファイルを開く
	pFile = fopen(&FileName[0],"r");

	if (pFile != NULL)
	{//ファイルが開けたら
		for (nCntY = 0; nCntY < MAX_Y; nCntY++)
		{
			for (nCntX = 0; nCntX < MAX_X; nCntX++)
			{//マップの読み込み
				fscanf(pFile, "%d\n", &s_aMap[nCntY][nCntX]);
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	for (nCntY = 0; nCntY < MAX_Y; nCntY++)
	{
		for (nCntX = 0; nCntX < MAX_X; nCntX++)
		{//マップの設定
			switch (s_aMap[nCntY][nCntX])
			{
			case OBJECT_NONE:	//何も配置しない
				break;

			case OBJECT_BLOCK_HORIZONTAL:	//ブロック(横)

				if (nCntX < MAX_X)
				{//Xが最大値未満のとき
					if (s_aMap[nCntY][nCntX + 1] == OBJECT_BLOCK_HORIZONTAL)
					{
						nCntSize++;		//大きさを測る
					}
					else
					{
						//ちょっと短くする
						float fWidth  = (BLOCK_WIDTH / 2);
						float fHeight = (BLOCK_HEIGHT / 2);

						//ブロックの設置
						SetBlock(D3DXVECTOR3((((s_fMapSizeX * nCntX) + fWidth) - (nCntSize * fWidth)),
											((s_fMapSizeY * nCntY) + fHeight),
											0.0f),						//位置
								 D3DXVECTOR3(BLOCK_MOVE, 0.0f, 0.0f),	//移動量
								 fWidth + (nCntSize * fWidth),			//幅
								 fHeight);								//高さ

						nCntSize = 0;	//サイズをリセッツ
					}
				}
				break;

			case OBJECT_ITEM:	//アイテム
			{
				//ちょっと短くする
				float fWidth  = (ITEM_WIDTH / 2);
				float fHeight = (ITEM_HEIGHT / 2);

				//アイテムの設置
				SetItem(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth),
									(s_fMapSizeY * nCntY + fHeight), 
									0.0f),						//位置
						D3DXVECTOR3(ITEM_MOVE, 0.0f, 0.0f),		//移動量
						ITEM_TYPE_COIN);						//種類

			}
				break;

			case OBJECT_ENEMY_GROUND:	//敵(地上)
			{
				//ちょっと短くする
				float fWidth = (ENEMY_WIDTH / 2);
				float fHeight = (ENEMY_HEIGHT / 2);

				SetEnemy(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth),
									 (s_fMapSizeY * nCntY + fHeight) - fHeight,
									 0.0f),							//位置
						D3DXVECTOR3(ENEMY_GROUND_MOVE, 0.0f, 0.0f),	//移動量
						MOVETYPE_GROUND);							//動き方
			}
				break;

			case OBJECT_ENEMY_ONBLOCK:	//敵(ブロックの上)
			{
				//ちょっと短くする
				float fWidth = (ENEMY_WIDTH / 2);
				float fHeight = (ENEMY_HEIGHT / 2);

				SetEnemy(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth),
									 (s_fMapSizeY * nCntY + fHeight) - fHeight,
									 0.0f),								//位置
						D3DXVECTOR3(ENEMY_ONBLOCK_MOVE, 0.0f, 0.0f),	//移動量
						MOVETYPE_ONBLOCK);								//動き方
			}
				break;

			case OBJECT_GOAL:	//ゴール
			{
				//ちょっと短くする
				float fWidth = (GOAL_WIDTH / 2);
				float fHeight = (GOAL_HEIGHT / 2);

				SetGoal(D3DXVECTOR3((s_fMapSizeX * nCntX + fWidth) - 6.0f,	//ズレ調整してる
									(s_fMapSizeY * nCntY + fHeight) - (fHeight * 0.6f),
									0.0f));		//位置
			}
				break;

			case OBJECT_BLOCK_VERTICAL:		//ブロック(縦)
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
			case OBJECT_NONE:				//何も配置しない
			case OBJECT_BLOCK_HORIZONTAL:	//ブロック(横)
			case OBJECT_ITEM:				//アイテム
			case OBJECT_ENEMY_GROUND:		//敵(地上)
			case OBJECT_ENEMY_ONBLOCK:		//敵(ブロックの上)
			case OBJECT_GOAL:				//ゴール
				break;

			case OBJECT_BLOCK_VERTICAL:		//ブロック(縦)

				if (nCntY < MAX_Y)
				{//Yが最大値未満のとき
					if (s_aMap[nCntY + 1][nCntX] == OBJECT_BLOCK_VERTICAL)
					{
						nCntSize++;		//大きさを測る
					}
					else
					{
						//ちょっと短くする
						float fWidth = (BLOCK_WIDTH / 2);
						float fHeight = (BLOCK_HEIGHT / 2);

						//ブロックの設置
						SetBlock(D3DXVECTOR3(((s_fMapSizeX * nCntX) + fWidth),
											(((s_fMapSizeY * nCntY) + fHeight) - (nCntSize * fHeight)),
											0.0f),									//位置
											D3DXVECTOR3(BLOCK_MOVE, 0.0f, 0.0f),	//移動量
											fWidth,									//幅
											fHeight + (nCntSize * fHeight));		//高さ

						nCntSize = 0;	//サイズをリセッツ
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