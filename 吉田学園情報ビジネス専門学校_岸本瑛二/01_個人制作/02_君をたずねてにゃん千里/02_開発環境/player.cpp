//============================================================================================================
//
//�N�������˂Ăɂ��痢[player.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "player.h"
#include "game.h"
#include "input.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "goal.h"
#include "fade.h"
#include "sound.h"

#include <assert.h>

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexturePlayer = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureStock	 = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStock	 = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static Player					s_player;					//�v���C���[�̏��
static STOCK					s_aStock[PLAYER_STOCK];		//�c�@�̏��

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void PlayerState(void);
void PlayerMove(void);
void LimitMove(void);
void TexAnimPlayer(void);
void SubStock(void);
STOCK *GetStock(void);

//============================================================================================================
//�v���C���[�̏���������
//============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̃|�C���^
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Cat_02_Copy.png",
								&s_pTexturePlayer);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Cat_Head_Copy.png",
								&s_pTextureStock);

	//�v���C���[���̏�����
	s_player.pos			= D3DXVECTOR3(PLAYERPOS_X, PLAYERPOS_Y, 0.0f);	//�ʒu(����)
	s_player.posOld		    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�ʒu(�O��)
	s_player.rot		    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//����
	s_player.move		    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�ړ���
	s_player.col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			//�F
	s_player.state		    = PLAYERSTATE_NORMAL;							//���
	s_player.nCounterState  = 0;											//�J�E���^�[
	s_player.nLife		    = PLAYER_LIFE;									//�̗�
	s_player.nStock			= PLAYER_STOCK;									//�c�@
	s_player.nCntAnim	    = 0;											//�J�E���^�[
	s_player.nPtnAnim	    = 0;											//�p�^�[���ԍ�
	s_player.nDirectionMove = PL_DIRECTION_RIGHT;							//�E�����ɂ���
	s_player.fWidth			= (PLAYER_WIDTH * 0.5f);						//��
	s_player.fHeight		= PLAYER_HEIGHT;								//����
	s_player.bJump			= false;										//�W�����v���Ă��Ȃ���Ԃɂ���
	s_player.bDisp			= true;											//�\�������Ԃɂ���

	for (int i = 0; i < PLAYER_STOCK; i++)
	{//�c�@�̏��̏�����
		STOCK *pStock = &s_aStock[i];		//�|�C���^�ɕϐg

		pStock->pos.x = STOCK_POS_X + (i * STOCK_SIZE) + (i *  +STOCK_INTERVAL);
		pStock->pos.y = STOCK_POS_Y;
		pStock->pos.z = 0.0f;						//�ʒu

		pStock->fWidth	= (STOCK_SIZE * 0.5f);		//��
		pStock->fHeight = (STOCK_SIZE * 0.5f);		//����
		pStock->bUse	= true;						//�g�p����Ă����Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPlayer,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PLAYER_STOCK,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffStock,
								NULL);

	/*���v���C���[��*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	{//���_���W�̐ݒ�
		float fLeft	 = (s_player.pos.x - s_player.fWidth);		//��
		float fRight = (s_player.pos.x + s_player.fWidth);		//�E
		float fUp	 = (s_player.pos.y - s_player.fHeight);		//��
		float fDown  =  s_player.pos.y;							//��

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
	}

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	//�e�N�X�`�����W�̐ݒ�
	SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPlayer->Unlock();

	/*���c�@��*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < PLAYER_STOCK; i++)
	{
		STOCK *pStock = &s_aStock[i];		//�|�C���^�ɕϐg

		//���_���W�̐ݒ�
		float fLeft	 = (pStock->pos.x - pStock->fWidth);	//��
		float fRight = (pStock->pos.x + pStock->fWidth);	//�E
		float fUp	 = (pStock->pos.y - pStock->fHeight);	//��
		float fDown  = (pStock->pos.y + pStock->fHeight);	//��

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//4���i�߂�
	}
	
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStock->Unlock();
}

//============================================================================================================
//�v���C���[�̏I������
//============================================================================================================
void UninitPlayer(void)
{
	//�T�E���h�̏I������
	StopSound();

	/*���e�N�X�`���̔j����*/

	if (s_pTexturePlayer != NULL)
	{//�v���C���[
		s_pTexturePlayer->Release();
		s_pTexturePlayer = NULL;
	}

	if (s_pTextureStock != NULL)
	{//�c�@
		s_pTextureStock->Release();
		s_pTextureStock = NULL;
	}

	/*�����_�o�b�t�@�̔j����*/

	if (s_pVtxBuffPlayer != NULL)
	{//�v���C���[
		s_pVtxBuffPlayer->Release();
		s_pVtxBuffPlayer = NULL;
	}

	if (s_pVtxBuffStock != NULL)
	{//�c�@
		s_pVtxBuffStock->Release();
		s_pVtxBuffStock = NULL;
	}
}

//============================================================================================================
//�v���C���[�̍X�V����
//============================================================================================================
void UpdatePlayer(void)
{
	//�v���C���[�̏��
	PlayerState();

	//�v���C���[�̈ړ��E�W�����v
	PlayerMove();

	//�v���C���[�̃e�N�X�`���A�j���[�V��������
	TexAnimPlayer();

	//�u���b�N�Ƃ̓����蔻��(�W�����v�̉ۂ����f)
	s_player.bJump = !CollisionBlockPlayer(&s_player.pos,		//���݂̈ʒu
										   &s_player.posOld,	//�O��̈ʒu
										   &s_player.move,		//�ړ���
											s_player.fWidth,	//��
											s_player.fHeight);	//����

	//�v���C���[�̈ړ�����
	LimitMove();

	//�G�Ƃ̓����蔻��
	CollisionEnemyPlayer(&s_player.pos, &s_player.posOld, &s_player.move, s_player.fWidth, s_player.fHeight);

	//�A�C�e���Ƃ̓����蔻��
	CollisionItem(&s_player.pos, s_player.fWidth, s_player.fHeight);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	float fLeft  = (s_player.pos.x - s_player.fWidth);		//��
	float fRight = (s_player.pos.x + s_player.fWidth);		//�E
	float fUp	 = (s_player.pos.y - s_player.fHeight);		//��
	float fDown  =  s_player.pos.y;							//��

	pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPlayer->Unlock();
}

//============================================================================================================
//�v���C���[�̕`�揈��
//============================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	/*���v���C���[��*/

	if (s_player.bDisp)
	{//�v���C���[���\������Ă���
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexturePlayer);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}

	/*���c�@��*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffStock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PLAYER_STOCK; i++)
	{
		STOCK *pStock = &s_aStock[i];	//�|�C���^�ɕϐg

		if (pStock->bUse)
		{//�g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTextureStock);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
									i * 4,					//�`�悷��ŏ��̒��_�C���f�b�N�X
									2);						//�`�悷��v���~�e�B�u��
		}
	}
}

//============================================================================================================
//�v���C���[�̈ړ�����
//============================================================================================================
void LimitMove(void)
{
	//�ړ�����(��)
	if (s_player.pos.y > SCREEN_HEIGHT)
	{//�v���C���[�����[�𒴂�����
		s_player.pos.y = SCREEN_HEIGHT;
		s_player.move.y = 0.0f;

		//�W�����v���Ă��Ȃ���Ԃɂ���
		s_player.bJump = false;
	}

	//�ړ�����(���E)
	if (s_player.pos.x - s_player.fWidth < 0)
	{//�v���C���[�����[�𒴂�����
		s_player.pos.x = s_player.fWidth;
	}
	else if (s_player.pos.x + s_player.fWidth > SCREEN_WIDTH)
	{//�v���C���[���E�[�𒴂�����
		s_player.pos.x = (SCREEN_WIDTH - s_player.fWidth);
	}
	else if (s_player.pos.x + s_player.fWidth > (SCREEN_WIDTH / 2))
	{//�v���C���[����ʒ����𒴂�����
		if (!GetPosGoal())
		{//�S�[�����w��̈ʒu�ɂ��Ȃ�
			s_player.pos.x = ((SCREEN_WIDTH / 2) - s_player.fWidth);
		}
	}
}

//============================================================================================================
//�v���C���[�̏��
//============================================================================================================
void PlayerState(void)
{
	switch (s_player.state)
	{
	case PLAYERSTATE_NORMAL:	//�ʏ���
		break;

	case PLAYERSTATE_DAMAGE:	//�_���[�W���

		s_player.nCounterState--;	//�J�E���g�_�E��

		if (s_player.nCounterState <= 0)
		{//�J�E���^�[��0�ɂȂ�����
			s_player.nCounterState = 120;				//�J�E���^�[����
			s_player.state = PLAYERSTATE_INVINCIBLE;	//���G��Ԃɂ���

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[�̐ݒ�(�ʏ�(��))
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_DEATH:		//���S���

		s_player.nCounterState--;	//�J�E���g�_�E��

		if (GetFade() == FADE_NONE)
		{//�t�F�[�h���Ă��Ȃ��Ƃ�
			if (s_player.nCounterState <= 0 && GetGamaState() != GAMESTATE_END)
			{//�J�E���^�[��0�ɂȂ��� & �I����Ԃł͂Ȃ��Ƃ�
				//�I����Ԃɐݒ�
				SetGameState(GAMESTATE_END);
				return;
			}
		}
		break;

	case PLAYERSTATE_INVINCIBLE:	//���G���

		s_player.nCounterState--;	//�J�E���g�_�E��

		//�_�ł�����(���l��sin�J�[�u�ŕϓ�)
		s_player.col.a = sinf((s_player.nCounterState * BLINKING_SPEED) * (D3DX_PI * 2.0f)) * BLINKING_SIZE + BLINKING_DEGREE;

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�(�ʏ�(��))
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, s_player.col.a);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffPlayer->Unlock();

		if (s_player.nCounterState <= 0)
		{//�J�E���^�[��0�ɂȂ�����
			s_player.state = PLAYERSTATE_NORMAL;	//�ʏ��Ԃɂ���

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[�̐ݒ�(�ʏ�(��))
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuffPlayer->Unlock();
		}
		break;
	}
}

//============================================================================================================
//�v���C���[�̈ړ��E�W�����v
//============================================================================================================
void PlayerMove(void)
{
	/*���v���C���[�̈ړ���*/

	if (GetKeyboardPress(DIK_A))
	{//A�L�[��������Ă���(��)
		//�������ɂ���
		s_player.nDirectionMove = PL_DIRECTION_LEFT;

		//�ʒu���X�V
		s_player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_SPEED;
	}
	else if (GetKeyboardPress(DIK_D))
	{//D�L�[��������Ă���(�E)
		//�E�����ɂ���
		s_player.nDirectionMove = PL_DIRECTION_RIGHT;

		//�ʒu���X�V
		s_player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;
	}
	else
	{//���������Ă��Ȃ��Ƃ�
		//�Î~������
		if (s_player.nDirectionMove == PL_DIRECTION_RIGHT)
		{//�E�����Ă�Ƃ�
			//���Ε����ɗ͂�������
			s_player.move.x -= sinf(D3DX_PI * 0.5f) * 0.01f;

			if (s_player.move.x < 0.0f)
			{//0���߂�����
				s_player.move.x = 0.0f;
			}
		}
		else if (s_player.nDirectionMove == PL_DIRECTION_LEFT)
		{//�������Ă�Ƃ�
			//���Ε����ɗ͂�������
 			s_player.move.x += sinf(D3DX_PI * 0.5f) * 0.01f;

			if (s_player.move.x > 0.0f)
			{//0���߂�����
				s_player.move.x = 0.0f;
			}
		}
	}

	//�ړ��ʂ��X�V(����������)
	s_player.move.x += (0.0f - s_player.move.x) * SPEED_DOWN;

	/*���v���C���[�̃W�����v��*/

	if (!s_player.bJump)
	{//�W�����v���Ă��Ȃ��Ƃ�
		if (GetKeyboardTrigger(DIK_SPACE))
		{//�X�y�[�X�L�[�������ꂽ(�W�����v)
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_JUMP);

			//�ړ��ʂ�ύX
			s_player.move.y = PLAYER_JUMP;

			//�W�����v���Ă����Ԃɂ���
			s_player.bJump = true;
		}
	}

	//�d�͂����Z����
	s_player.move.y += PLAYER_GRAVITY;	

	//�O��̈ʒu��ۑ�
	s_player.posOld = s_player.pos;

	//�ʒu���X�V
	s_player.pos += s_player.move;
}

//============================================================================================================
//�v���C���[�̃e�N�X�`���A�j���[�V����
//============================================================================================================
void TexAnimPlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (!s_player.bJump)
	{//�W�����v���Ă��Ȃ��Ƃ�
		if (s_player.nDirectionMove == PL_DIRECTION_RIGHT)
		{//�E�����̎�
			if (s_player.move.x != SPEED_STOP || 
				(s_player.pos.x + s_player.fWidth >= (SCREEN_WIDTH / 2) && s_player.move.x != SPEED_STOP))
			{//�ړ����Ă��� or �ړ����̃v���C���[�̉E�[����ʒ����ɂ���
				s_player.nCntAnim++;	//�J�E���^�[�����Z

				if (s_player.nCntAnim % PLAYER_ANIMSPEED == 0)
				{//��莞�Ԍo�߂�����
					s_player.nPtnAnim = (s_player.nPtnAnim + 1) % PL_PATTERN_X;		//�p�^�[���ԍ����X�V����

					//�e�N�X�`�����W�̐ݒ�
					SetTex(pVtx,
							(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
							(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
							(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
							(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
				}
			}
			else
			{//�ړ����Ă��Ȃ�
				if (s_player.nPtnAnim == 1)
				{//�p�^�[��1�̂Ƃ�
					s_player.nPtnAnim = 2;		//�p�^�[���ԍ���ݒ�
				}
				else if (s_player.nPtnAnim == 3)
				{//�p�^�[��3�̂Ƃ�
					s_player.nPtnAnim = 0;		//�p�^�[���ԍ���ݒ�
				}

				//�e�N�X�`�����W�̐ݒ�
				SetTex(pVtx,
						(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
						(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
						(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
						(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
			}
		}
		else if (s_player.nDirectionMove == PL_DIRECTION_LEFT)
		{//�������̎�
			if (s_player.move.x != SPEED_STOP)
			{//�ړ����Ă���
				s_player.nCntAnim++;	//�J�E���^�[�����Z

				if (s_player.nCntAnim % PLAYER_ANIMSPEED == 0)
				{//��莞�Ԍo�߂�����
					s_player.nPtnAnim = (s_player.nPtnAnim + 1) % PL_PATTERN_X;	//�p�^�[���ԍ����X�V����

					//�e�N�X�`�����W�̐ݒ�
					SetTex(pVtx,
							(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
							(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
							(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
							(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
				}
			}
			else
			{//�ړ����Ă��Ȃ�
				if (s_player.nPtnAnim == 1)
				{//�p�^�[��1�̂Ƃ�
					s_player.nPtnAnim = 2;		//�p�^�[���ԍ���ݒ�
				}
				else if (s_player.nPtnAnim == 3)
				{//�p�^�[��3�̂Ƃ�
					s_player.nPtnAnim = 0;		//�p�^�[���ԍ���ݒ�
				}

				//�e�N�X�`�����W�̐ݒ�
				SetTex(pVtx,
						(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
						(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
						(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
						(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
			}
		}
	}
	else if (s_player.bJump)
	{//�W�����v���Ă���Ƃ�
		if (s_player.nPtnAnim == 0 || s_player.nPtnAnim == 2)
		{
			s_player.nPtnAnim = (s_player.nPtnAnim + 1) % PL_PATTERN_X;	//�p�^�[���ԍ����X�V����
		}
		
		//�e�N�X�`�����W�̐ݒ�
		SetTex(pVtx,
				(1.0f / PL_PATTERN_X) * s_player.nPtnAnim,
				(1.0f / PL_PATTERN_X) * (s_player.nPtnAnim + 1),
				(1.0f / PL_PATTERN_Y) * s_player.nDirectionMove + 0.02f,
				(1.0f / PL_PATTERN_Y) * (s_player.nDirectionMove + 1));
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPlayer->Unlock();
}

//============================================================================================================
//�v���C���[�̃q�b�g����
//============================================================================================================
void HitPlayer(int nDamage)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (s_player.bDisp && s_player.state == PLAYERSTATE_NORMAL)
	{//�v���C���[���\������Ă��� & �ʏ��Ԃ̂Ƃ�
		//�̗͂����炷
		s_player.nLife -= nDamage;

		if (s_player.nLife <= 0)
		{//�v���C���[�̗̑͂��Ȃ��Ȃ���
			//�T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_EXPLOSION);

			s_player.bDisp = false;					//��\���ɂ���
			s_player.state = PLAYERSTATE_DEATH;		//���S��Ԃɂ���
			s_player.nCounterState = 10;			//�J�E���^�[������
		}
		else
		{
			s_player.state = PLAYERSTATE_DAMAGE;	//�_���[�W��Ԃɂ���
			s_player.nCounterState = 15;			//�J�E���^�[������

			//���_�J���[�̐ݒ�(��)
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		}

		//�c�@�̌���
		SubStock();
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPlayer->Unlock();
}

//============================================================================================================
//�v���C���[�̓����蔻��
//============================================================================================================
void CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight)
{
	Player *pPL = GetPlayer();		//�|�C���^�ɕϐg

	if (pPos->y - fHeight < pPL->pos.y &&
		pPos->y + fHeight > pPL->pos.y - pPL->fHeight)
	{//�Ώۂ̑O��̍������v���C���[�͈͓̔��ɂ���
		if (pPosOld->x + fWidth <= pPL->pos.x - pPL->fWidth &&
			pPos->x + fWidth > pPL->pos.x - pPL->fWidth)
		{//�E��
			pPL->pos.x = pPos->x + pPL->fWidth + fWidth;
		}
		else if (pPosOld->x - fWidth >= pPL->pos.x + pPL->fWidth &&
				 pPos->x - fWidth < pPL->pos.x + pPL->fWidth)
		{//����
			pPL->pos.x = pPos->x - pPL->fWidth - fWidth;
		}
	}
}

//============================================================================================================
//�c�@�̌�������
//============================================================================================================
void SubStock(void)
{
	switch (s_player.nLife)
	{
	case PLAYER_LIFE:	//�m�[�_���[�W
		break;

	case (PLAYER_LIFE - 10):	//�_���[�W��1��󂯂�
		s_aStock[2].bUse = false;	//�E����c�@�����炷
		break;

	case (PLAYER_LIFE - 20):	//�_���[�W��2��󂯂�
		s_aStock[1].bUse = false;	//�E����c�@�����炷
		break;

	case 0:						//�_���[�W��3��󂯂�(�̗͂�0)
		s_aStock[0].bUse = false;	//�E����c�@�����炷
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================================================================
//�v���C���[���̎擾
//============================================================================================================
Player *GetPlayer(void)
{
	return &s_player;
}

//============================================================================================================
//�v���C���[�̏�Ԃ̎擾
//============================================================================================================
PLAYERSTATE GetPlayerState(void)
{
	return s_player.state;
}

//============================================================================================================
//�c�@�̏��̎擾
//============================================================================================================
STOCK *GetStock(void)
{
	return &s_aStock[0];
}