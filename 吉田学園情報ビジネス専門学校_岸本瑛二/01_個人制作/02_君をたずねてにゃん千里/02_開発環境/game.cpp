//============================================================================================================
//
//�N�������˂Ăɂ��痢[game.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "game.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "item.h"
#include "map.h"
#include "goal.h"
#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "time.h"
#include "sound.h"
#include "fade.h"
//#include "effect.h"
#include "pause.h"
#include "ranking.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static GAMESTATE s_gameState			= GAMESTATE_NONE;	//�Q�[���̏��
static int		 s_nCounterGameState	= 0;				//��ԊǗ��J�E���^�[
static bool		 s_pPause				= false;			//�|�[�Y�����ǂ���

//============================================================================================================
//�Q�[���̏���������
//============================================================================================================
void InitGame(void)
{
	//�w�i�̏���������
	InitBG();

	//�u���b�N�̏���������
	InitBlock();

	//�A�C�e���̏���������
	InitItem();

	//�G�̏���������
	InitEnemy();

	//�S�[���̏���������
	InitGoal();

	////�G�t�F�N�g�̏���������
	//InitEffect();

	//�v���C���[�̏���������
	InitPlayer();

	////�|�b�v�̏���������
	//InitPop();

	//�X�R�A�̏���������
	InitScore();

	//�X�R�A�̐ݒ菈��
	SetScore(0);

	//�^�C���̏���������
	InitTime();
	SetTime(TIME_LIMIT);

	//�|�[�Y�̏���������
	InitPause();

	//�}�b�v�̐ݒ�
	SetMap(FILE_MAP);

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_GAME);

	s_gameState			= GAMESTATE_NORMAL;		//�ʏ���
	s_nCounterGameState = 0;					//�J�E���^�[�̏�����
	s_pPause			= false;				//�|�[�Y����
}

//============================================================================================================
//�Q�[���̏I������
//============================================================================================================
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�w�i�̏I������
	UninitBG();

	//�u���b�N�̏I������
	UninitBlock();
	
	//�A�C�e���̏I������
	UninitItem();

	//�S�[���̏I������
	UninitGoal();

	////�G�t�F�N�g�̏I������
	//UninitEffect();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();

	//�X�R�A�̏I������
	UninitScore();

	//�^�C���̏I������
	UninitTime();

	//�|�[�Y�̏I������
	UninitPause();
}

//============================================================================================================
//�Q�[���̍X�V����
//============================================================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{//P�L�[�������ꂽ
		s_pPause = s_pPause ? false : true;

		if (s_pPause)
		{//�|�[�Y��
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_PAUSE);
		}
		else if (!s_pPause)
		{//�|�[�Y���ł͂Ȃ�
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}
	}

	if (s_pPause)
	{//�|�[�Y��
		//�|�[�Y�̍X�V����
		UpdatePause();
	}
	else if (!s_pPause)
	{//�|�[�Y���łȂ����
		
		switch (s_gameState)
		{
		case GAMESTATE_NORMAL:		//�ʏ���
			//�w�i�̍X�V����
			UpdateBG();

			//�u���b�N�̍X�V����
			UpdateBlock();

			//�A�C�e���̍X�V����
			UpdateItem();

			//�S�[���̍X�V����
			UpdateGoal();

			////�G�t�F�N�g�̍X�V����
			//UpdateEffect();

			//�G�̍X�V����
			UpdateEnemy();

			//�X�R�A�̍X�V����
			UpdateScore();

			//�v���C���[�̍X�V����
			UpdatePlayer();

			//�^�C���̍X�V����
			UpdateTime();
			break;

		case GAMESTATE_END:			//�I�����

			s_nCounterGameState++;

			if (s_nCounterGameState >= 60)
			{
				s_gameState = GAMESTATE_NONE;		//�������Ă��Ȃ����

				//���(���[�h)�̐ݒ�
				SetFade(MODE_RESULT);
			}
			break;
		}
	}
}

//============================================================================================================
//�Q�[���̕`�揈��
//============================================================================================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBG();

	//�G�̕`�揈��
	DrawEnemy();

	////�G�t�F�N�g�̕`�揈��
	//DrawEffect();

	//�u���b�N�̕`�揈��
	DrawBlock();

	//�A�C�e���̕`�揈��
	DrawItem();

	//�S�[���̕`�揈��
	DrawGoal();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�^�C���̕`�揈��
	DrawTime();

	if (s_pPause)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
		DrawPause();
	}
}

//============================================================================================================
//�Q�[���̐ݒ菈��
//============================================================================================================
void SetGameState(GAMESTATE state)
{
	s_gameState	= state;
	s_nCounterGameState = 0;
}

//============================================================================================================
//�Q�[���̎擾����
//============================================================================================================
GAMESTATE GetGamaState(void)
{
	return s_gameState;
}

//============================================================================================================
//�|�[�Y�̗L�������ݒ�
//============================================================================================================
void SetEnablePause(bool bPause)
{
	s_pPause = bPause;
}