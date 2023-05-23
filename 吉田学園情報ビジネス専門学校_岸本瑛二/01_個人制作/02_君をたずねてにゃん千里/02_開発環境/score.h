//============================================================================================================
//
//�N�������˂Ăɂ��痢[score.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define MAX_SCORE			(8)			//�X�R�A�̍ő包��
#define SCORE_POS_X			(1035.0f)	//�X�R�A�̈ʒu(X)
#define SCORE_POS_Y			(10.0f)		//�X�R�A�̈ʒu(Y)
#define SCORE_WIDTH			(30)		//�X�R�A�̕�
#define SCORE_HEIGHT		(45)		//�X�R�A�̍���
#define ENEMY_ADDSCORE		(200)		//�G�̃X�R�A(+)
#define ENEMY_SUBSCORE		(50)		//�G�̃X�R�A(-)
#define ENEMY_SPECIALSCORE	(400)		//�G�̃X�R�A(�X�y�V����)

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
void SubScore(int nValue);
int GetScore(void);

#endif