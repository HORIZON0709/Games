//============================================================================================================
//
//�N�������˂Ăɂ��痢[enemy.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "block.h"
#include "goal.h"

#include <assert.h>

//============================================================================================================
//�X�^�e�B�b�N�ϐ��錾
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture	= NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff	= NULL;		//���_�o�b�t�@�ւ̃|�C���^
static Enemy					s_aEnemy[MAX_ENEMY];	//�G�̏��
static float					s_fSaveMove;			//�ړ��ʕۑ��p

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void EnemyMove(Enemy *pEnemy);
void EnemyState(Enemy *pEnemy);
void LimitMove(Enemy *pEnemy);
void TexAnimEnemy(Enemy *pEnemy);

//============================================================================================================
//�G�̏���������
//============================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Cat_04_Copy.png",
								&s_pTexture);

	//�G�̏��̏�����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		s_aEnemy[i].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEnemy[i].posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEnemy[i].move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEnemy[i].state			= ENEMYSTATE_NORMAL;
		s_aEnemy[i].type			= MOVETYPE_GROUND;
		s_aEnemy[i].nCounterState	= 0;
		s_aEnemy[i].nLife			= 0;
		s_aEnemy[i].nCntAnim		= 0;
		s_aEnemy[i].nPtnAnim		= 0;
		s_aEnemy[i].nDirectionMove	= ENEMY_DIRECTION_LEFT;		//������
		s_aEnemy[i].fWidth			= 0.0f;
		s_aEnemy[i].fHeight			= 0.0f;
		s_aEnemy[i].bUse			= false;					//�g�p���Ă��Ȃ���Ԃɂ���
		s_aEnemy[i].pBLK			= NULL;
	}

	s_fSaveMove = 0.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
		SetTex(pVtx, 0.0f, 0.25f, 0.0f, 0.5f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�G�̏I������
//============================================================================================================
void UninitEnemy(void)
{
	//�T�E���h�̏I������
	StopSound();

	//�e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//�G�̍X�V����
//============================================================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//�|�C���^�ɕϐg

		if (!pEnemy->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*���G���g�p����Ă��遫*/

		//�G�̏��
		EnemyState(pEnemy);

		//�G�̈ړ�����
		LimitMove(pEnemy);

		pEnemy->pBLK = NULL;	//NULL������

		//�u���b�N�Ƃ̓����蔻��
		CollisionBlockEnemy(&pEnemy->pos,		//�ʒu
							&pEnemy->posOld,	//�O��̈ʒu
							&pEnemy->move,		//�ړ���
							 pEnemy->fWidth,	//��
							 pEnemy->fHeight,	//����
							&pEnemy->pBLK);		//����Ă���u���b�N�̏��

		//�G�̓�����
		EnemyMove(pEnemy);
		
		//�ړ������̊m�F
		if (pEnemy->move.x >= 0.0f)
		{//�E�ɐi��ł�Ƃ�
			pEnemy->nDirectionMove = ENEMY_DIRECTION_RIGHT;
		}
		else
		{//���ɐi��ł�Ƃ�
			pEnemy->nDirectionMove = ENEMY_DIRECTION_LEFT;
		}

		//�G�̃e�N�X�`���A�j���[�V����
		TexAnimEnemy(pEnemy);

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�|�C���^��4���i�߂�

		//���_���W�̐ݒ�
		float fLeft		= (pEnemy->pos.x - pEnemy->fWidth);		//��
		float fRight	= (pEnemy->pos.x + pEnemy->fWidth);		//�E
		float fUp		= (pEnemy->pos.y - pEnemy->fHeight);	//��
		float fDown		= (pEnemy->pos.y + pEnemy->fHeight);	//��

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		SetTex(pVtx,
				(1.0f / PL_PATTERN_X) *	 pEnemy->nPtnAnim + 0.0005f,
				(1.0f / PL_PATTERN_X) * (pEnemy->nPtnAnim + 1),
				(1.0f / PL_PATTERN_Y) *	 pEnemy->nDirectionMove + 0.02f,
				(1.0f / PL_PATTERN_Y) * (pEnemy->nDirectionMove + 1));

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//============================================================================================================
//�G�̕`�揈��
//============================================================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//�|�C���^�ɕϐg

		if (pEnemy->bUse)
		{//�G���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture);

			//�G�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
									i * 4,					//�`�悷��ŏ��̒��_�C���f�b�N�X
									2);						//�`�悷��v���~�e�B�u��
		}
	}
}

//============================================================================================================
//�G�̐ݒ菈��
//============================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, MOVETYPE type)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//�|�C���^�ɕϐg

		if (!pEnemy->bUse)
		{//�G���g�p����Ă��Ȃ�
			pEnemy->pos				= pos;
			pEnemy->posOld			= pos;
			pEnemy->move			= move;
			pEnemy->state			= ENEMYSTATE_NORMAL;
			pEnemy->type			= type;
			pEnemy->nCounterState	= COUNTER_ENEMY;
			pEnemy->nLife			= ENEMY_LIFE;
			pEnemy->fWidth			= (ENEMY_WIDTH / 2);
			pEnemy->fHeight			= (ENEMY_HEIGHT / 2);
			pEnemy->bUse			= true;			//�g�p���Ă����Ԃɂ���

			s_fSaveMove = move.x;	//�ړ��ʂ�ۑ�����

			//���_���W�̐ݒ�
			float fLeft		= (pEnemy->pos.x - pEnemy->fWidth);		//��
			float fRight	= (pEnemy->pos.x + pEnemy->fWidth);		//�E
			float fUp		= (pEnemy->pos.y - pEnemy->fHeight);	//��
			float fDown		= (pEnemy->pos.y + pEnemy->fHeight);	//��

			pVtx[0].pos = D3DXVECTOR3(fLeft, fUp, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRight, fUp, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fLeft, fDown, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
			break;
		}

		pVtx += 4;		//�|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�G�̓�����
//============================================================================================================
void EnemyMove(Enemy *pEnemy)
{
	Player *pPlayer = GetPlayer();		//�v���C���[���

	switch (pEnemy->type)
	{
	case MOVETYPE_GROUND:	//�n��
	
		if (!GetPosGoal())
		{//�S�[�����w��̈ʒu�ɂ��Ȃ�
			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//�v���C���[�̉E�[����ʒ��� & �����Ă���
				//�v���C���[�̈ړ��ʂ�(������Ƃ���������)�����
				pEnemy->pos.x += -(pPlayer->move.x * 0.7f);
			}
		}
		break;

	case MOVETYPE_ONBLOCK:	//�u���b�N�̏�

		if (!GetPosGoal())
		{//�S�[�����w��̈ʒu�ɂ��Ȃ�
			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//�v���C���[�̉E�[����ʒ��� & �����Ă���
				//�v���C���[�̈ړ��ʂ�(������Ƃ���������)�����
				pEnemy->pos.x -= (pPlayer->move.x * 0.7f);
			}
		}

		if (pEnemy->pBLK != NULL)
		{
			if (pEnemy->pos.x - pEnemy->fWidth < pEnemy->pBLK->pos.x - pEnemy->pBLK->fWidth)
			{//�����痎�������ɂȂ�����
				//�ʒu��߂�
				pEnemy->pos.x = pEnemy->pBLK->pos.x - pEnemy->pBLK->fWidth + pEnemy->fWidth;

				//�����𔽓]
				pEnemy->nDirectionMove = (pEnemy->nDirectionMove + 1) % 2;

				//�ړ������𔽓]
				pEnemy->move.x *= -1.0f;
			}
			else if (pEnemy->pos.x + pEnemy->fWidth > pEnemy->pBLK->pos.x + pEnemy->pBLK->fWidth)
			{//�E���痎�������ɂȂ�����
				//�ʒu��߂�
				pEnemy->pos.x = pEnemy->pBLK->pos.x + pEnemy->pBLK->fWidth - pEnemy->fWidth;

				//�����𔽓]
				pEnemy->nDirectionMove = (pEnemy->nDirectionMove + 1) % 2;

				//�ړ������𔽓]
				pEnemy->move.x *= -1.0f;
			}
		}
		break;

	default:
		assert(false);
		break;
	}

	//�d�͂����Z����
	pEnemy->move.y += ENEMY_GRAVITY;

	//�O��̈ʒu��ۑ�
	pEnemy->posOld = pEnemy->pos;

	//�ʒu���X�V
	pEnemy->pos += pEnemy->move;
}

//============================================================================================================
//�G�̏��
//============================================================================================================
void EnemyState(Enemy *pEnemy)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:		//�ʏ���
		break;

	case ENEMYSTATE_DAMAGE:		//�_���[�W���

		pEnemy->nCounterState--;	//�J�E���g�_�E��

		if (pEnemy->nCounterState <= 0)
		{//�_���[�W��Ԃ̌p�����I�������
			pEnemy->state = ENEMYSTATE_NORMAL;		//�ʏ��Ԃɂ���

			//���_�J���[�̍X�V
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�G�̈ړ�����
//============================================================================================================
void LimitMove(Enemy *pEnemy)
{
	//�ړ�����(��)
	if (pEnemy->pos.y + pEnemy->fHeight > SCREEN_HEIGHT)
	{//�v���C���[�����[�𒴂�����
		pEnemy->pos.y = SCREEN_HEIGHT - pEnemy->fHeight;
		pEnemy->move.y = 0.0f;
	}

}

//============================================================================================================
//�G�̃e�N�X�`���A�j���[�V����
//============================================================================================================
void TexAnimEnemy(Enemy *pEnemy)
{
	pEnemy->nCntAnim++;		//�J�E���g�A�b�v

	if (pEnemy->nCntAnim % ENEMY_ANIM_SPEED == 0)
	{//��莞�Ԍo�߂�����
		pEnemy->nPtnAnim = (pEnemy->nPtnAnim + 1) % ENEMY_PATTERN_X;	//�p�^�[���ԍ����X�V����
	}
}

//============================================================================================================
//�G�ƃv���C���[�̓����蔻��
//============================================================================================================
void CollisionEnemyPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];	//�|�C���^�ɕϐg

		if (!pEnemy->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*���G���g�p����Ă��遫*/

		if (pPos->x - (fWidth / 10) < pEnemy->pos.x + pEnemy->fWidth &&
			pPos->x + (fWidth / 10) > pEnemy->pos.x - pEnemy->fWidth)
		{//�v���C���[�̕����G�͈͓̔��̂Ƃ�
			if (pPosOld->y <= pEnemy->pos.y - pEnemy->fHeight &&
				pPos->y		> pEnemy->pos.y - pEnemy->fHeight)
			{//�v���C���[���ォ�瓥��Â�����
				pPos->y = pEnemy->pos.y - pEnemy->fHeight;

				pMove->y *= -0.5f;

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_TRAMP);

				//�G�̃q�b�g����
				HitEnemy(i, DAMAGE_FROM_PL);
				continue;
			}

			if (pPos->x - fWidth  < pEnemy->pos.x + pEnemy->fWidth &&
				pPos->x + fWidth  > pEnemy->pos.x - pEnemy->fWidth &&
				pPos->y - fHeight < pEnemy->pos.y + pEnemy->fHeight &&
				pPos->y			  > pEnemy->pos.y - pEnemy->fHeight)
			{//�v���C���[���G�ɐG�ꂽ��
				if (GetPlayerState() == PLAYERSTATE_NORMAL)
				{//�ʏ��Ԃ̂Ƃ�
					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					//�v���C���[�̃q�b�g����
					HitPlayer(10);
				}
			}
		}
	}
}

//============================================================================================================
//�G�̃q�b�g����
//============================================================================================================
void HitEnemy(int nCnt, int nDamage)
{
	Enemy *pEnemy = &s_aEnemy[nCnt];	//�|�C���^�ɕϐg

	if (pEnemy->state == ENEMYSTATE_NORMAL)
	{//�ʏ��Ԃ̎�
		pEnemy->nLife -= nDamage;	//���C�t�����炷

		if (pEnemy->nLife <= 0)
		{//�G�̗̑͂��O�ȉ�
			//�X�R�A�̉��Z
			AddScore(ENEMY_POINT);

			//�g�p����Ă��Ȃ���Ԃ�
			pEnemy->bUse = false;
		}
		else
		{
			//�_���[�W��Ԃ�
			pEnemy->state = ENEMYSTATE_DAMAGE;

			//�J�E���^�[�̏���
			pEnemy->nCounterState = COUNTER_ENEMY;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCnt * 4);	//�Y���̈ʒu�܂Ői�߂�

			//���_�J���[�̐ݒ�(��)
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}
	}
}

//============================================================================================================
//�G�̎擾
//============================================================================================================
Enemy *GetEnemy(void)
{
	return &s_aEnemy[0];
}