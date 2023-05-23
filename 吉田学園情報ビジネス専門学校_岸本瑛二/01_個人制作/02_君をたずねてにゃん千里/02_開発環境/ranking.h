//============================================================================================================
//
//�N�������˂Ăɂ��痢[ranking.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define MAX_RANK			(5)						//�����N��
#define RANK_POS_X			(150.0f)				//���ʂ̏����ʒu(X)
#define RANK_POS_Y			(470.0f)				//���ʂ̏����ʒu(Y)
#define RANK_WIDTH			(75)					//���ʂ̕�
#define RANK_HEIGHT			(210)					//���ʂ̍���
#define RANKINGSCORE_POS_X	(RANK_POS_X + 150.0f)	//�����L���O�X�R�A�̏����ʒu(X)
#define RANKINGSCORE_POS_Y	(RANK_POS_Y - 190.0f)	//�����L���O�X�R�A�̏����ʒu(Y)
#define RANKINGSCORE_WIDTH	(30)					//�����L���O�X�R�A�̕�
#define RANKINGSCORE_HEIGHT	(45)					//�����L���O�X�R�A�̍���
#define RANKINGSCORE_SPACE	(40)					//�����L���O�X�R�A�̊Ԋu
#define RANKING_TIME		(10)					//�����L���O�̕\������
#define RANKING_FADE_SPEED	(0.02f)					//�����L���O�̓_�ŃX�s�[�h
#define RANKING_LOGO_POS_X	(315.0f)				//���S�̈ʒu(X)
#define RANKING_LOGO_POS_Y	(190.0f)				//���S�̈ʒu(Y)
#define RANKING_LOGO_WIDTH	(200.0f)				//���S�̕�
#define RANKING_LOGO_HEIGHT	(50.0f)					//���S�̍���

//============================================================================================================
//�t�F�[�h�̏��
//============================================================================================================
typedef enum
{
	RANK_IN = 0,		//�����ɂ���
	RANK_OUT,			//�s�����ɂ���
	RANK_MAX
}RANK;

//============================================================================================================
//�����L���O�X�R�A�̍\���̂̒�`
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	int nScore;			//�X�R�A
}RankScore;

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);
void SaveRanking(void);
void LoadRanking(void);

#endif