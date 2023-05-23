//============================================================================================================
//
//�N�������˂Ăɂ��痢[block,cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "block.h"
#include "player.h"
#include "enemy.h"
#include "goal.h"
#include "time.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9	   s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static BLOCK				   s_aBlock[MAX_BLOCK];		//�u���b�N�̏��

//============================================================================================================
//�u���b�N�̏���������
//============================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^
	int nCntBlock;								//�u���b�N�̃J�E���g

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/block004.jpg",
								&s_pTexture);

	//�u���b�N�̏��̏�����
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		s_aBlock[nCntBlock].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu��������
		s_aBlock[nCntBlock].posOld	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu��������
		s_aBlock[nCntBlock].move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ��ʂ�������
		s_aBlock[nCntBlock].fWidth	= 0.0f;								//����������
		s_aBlock[nCntBlock].fHeight = 0.0f;								//������������
		s_aBlock[nCntBlock].bUse	= false;							//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
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
		SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//�|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�u���b�N�̏I������
//============================================================================================================
void UninitBlock(void)
{
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
//�u���b�N�̍X�V����
//============================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		BLOCK *pBlock = &s_aBlock[nCntBlock];	//�|�C���^�ɕϐg

		if (!pBlock->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*���u���b�N���g�p����Ă��遫*/

		Player *pPlayer = GetPlayer();			//�v���C���[���

		//�O��̈ʒu��ۑ�
		pBlock->posOld = pBlock->pos;

		if (!GetPosGoal())
		{//�S�[�����w��̈ʒu�ɂ��Ȃ�
			//�v���C���[�̈ړ��ʂ�(������Ƃ���������)�����
			pBlock->move.x = -(pPlayer->move.x * 0.7f);

			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//�v���C���[�̉E�[����ʒ��� & �����Ă���
				//�ʒu���X�V
				pBlock->pos.x += pBlock->move.x;
			}
		}

		//���_���W�̐ݒ�
		float fLeft		= (pBlock->pos.x - pBlock->fWidth);		//��
		float fRight	= (pBlock->pos.x + pBlock->fWidth);		//�E
		float fUp		= (pBlock->pos.y - pBlock->fHeight);	//��
		float fDown		= (pBlock->pos.y + pBlock->fHeight);	//��

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		pVtx += 4;		//�|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�u���b�N�̕`�揈��
//============================================================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	int nCntBlock;								//�u���b�N�̃J�E���g

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (s_aBlock[nCntBlock].bUse)
		{//�u���b�N���g�p����Ă���
			 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
									nCntBlock * 4,			//�`����J�n���钸�_�C���f�b�N�X
									2);						//�`�悷��v���~�e�B�u��
		}
	}
}

//============================================================================================================
//�u���b�N�̐ݒ菈��
//============================================================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntBlock;		//�u���b�N�̃J�E���g

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		BLOCK *pBlock = &s_aBlock[nCntBlock];	//�|�C���^�ɕϐg

		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			pBlock->pos		= pos;
			pBlock->move	= move;
			pBlock->fWidth	= fWidth;
			pBlock->fHeight = fHeight;
			pBlock->bUse	= true;		//�g�p���Ă����Ԃɂ���

			//���_���W�̐ݒ�
			pVtx[0].pos.x = pos.x - fWidth;
			pVtx[0].pos.y = pos.y - fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pos.x + fWidth;
			pVtx[1].pos.y = pos.y - fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pos.x - fWidth;
			pVtx[2].pos.y = pos.y + fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pos.x + fWidth;
			pVtx[3].pos.y = pos.y + fHeight;
			pVtx[3].pos.z = 0.0f;

			//�e�N�X�`�����W�̐ݒ�
			float fTexU = fWidth  / (BLOCK_WIDTH / 2);
			float fTexV = fHeight / (BLOCK_HEIGHT / 2);

			SetTex(pVtx, 0.0f, fTexU, 0.0f, fTexV);
			break;
		}

		pVtx += 4;		//�|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�u���b�N�ƃv���C���[�̓����蔻��
//============================================================================================================
bool CollisionBlockPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bLand = false;		//���n���Ă��邩�ǂ���

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		BLOCK *pBlock = &s_aBlock[i];	//�|�C���^�ɕϐg
		
		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			continue;
		}

		/*���u���b�N���g�p����Ă��遫*/

		if (pPos->x + fWidth > pBlock->pos.x - pBlock->fWidth &&
			pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth)
		{//�v���C���[�̕����u���b�N�͈͓̔��ɂ���
			if (pPosOld->y <= pBlock->pos.y - pBlock->fHeight &&
				pPos->y		> pBlock->pos.y - pBlock->fHeight)
			{//�㑤
				bLand = true;
				pPos->y	 = pBlock->pos.y - pBlock->fHeight;
				pMove->y = 0.0f;

				if (pBlock->move.x != SPEED_STOP)
				{//�u���b�N�����Ɉړ����Ă���Ƃ�
					pPos->x += (pBlock->move.x * 0.1f);
				}
				else if (pBlock->move.y != SPEED_STOP)
				{//�u���b�N���c�Ɉړ����Ă���Ƃ�
					pPos->y += pBlock->move.y;
				}
			}
			else if (pPosOld->y - fHeight >= pBlock->pos.y + pBlock->fHeight &&
					 pPos->y - fHeight	   < pBlock->pos.y + pBlock->fHeight)
			{//����
				pPos->y  = pBlock->pos.y + pBlock->fHeight + fHeight;
				pMove->y = 0.0f;
			}
		}
		
		if (pPosOld->y - fHeight < pBlock->pos.y + pBlock->fHeight &&
			pPosOld->y			 > pBlock->pos.y - pBlock->fHeight)
		{//�v���C���[�̑O��̍������u���b�N�͈͓̔��ɂ���
			if (pPosOld->x + fWidth <= pBlock->pos.x - pBlock->fWidth &&
				pPos->x	   + fWidth  > pBlock->pos.x - pBlock->fWidth)
			{//����
				pPos->x  = pBlock->pos.x - pBlock->fWidth - fWidth;
				pMove->x = 0.0f;
			}
			else if (pPosOld->x - fWidth >= pBlock->pos.x + pBlock->fWidth &&
					 pPos->x    - fWidth  < pBlock->pos.x + pBlock->fWidth)
			{//�E��
				pPos->x  = pBlock->pos.x + pBlock->fWidth + fWidth;
				pMove->x = 0.0f;
			}
		}
	}

	return bLand;
}

//============================================================================================================
//�u���b�N�ƓG�̓����蔻��
//============================================================================================================
void CollisionBlockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, BLOCK **pBLK)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		BLOCK *pBlock = &s_aBlock[i];	//�|�C���^�ɕϐg

		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			continue;
		}

		/*���u���b�N���g�p����Ă��遫*/

		if (pPos->x + fWidth > pBlock->pos.x - pBlock->fWidth &&
			pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth)
		{//�G�̕����u���b�N�͈͓̔��ɂ���
			if (pPosOld->y + fHeight <= pBlock->pos.y - pBlock->fHeight &&
				pPos->y	   + fHeight  > pBlock->pos.y - pBlock->fHeight)
			{//�㑤
				pPos->y = pBlock->pos.y - pBlock->fHeight - fHeight;
				pMove->y = 0.0f;

				if (pBlock != NULL)
				{
					//�G�̍\���̂�pBLK��s_aBlock(pBlock)�̃A�h���X��ݒ�
					*pBLK = pBlock;
				}
			}
			else if (pPosOld->y - fHeight >= pBlock->pos.y + pBlock->fHeight &&
					 pPos->y    - fHeight  < pBlock->pos.y + pBlock->fHeight)
			{//����
				pPos->y = pBlock->pos.y + pBlock->fHeight + fHeight;
				pMove->y = 0.0f;
			}
		}

		if (pPos->y + fHeight > pBlock->pos.y - pBlock->fHeight &&
			pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight)
		{//�G�̍������u���b�N�͈͓̔��ɂ���
			if (pPosOld->x - fWidth >= pBlock->pos.x + pBlock->fWidth &&
				pPos->x - fWidth	 < pBlock->pos.x + pBlock->fWidth)
			{//�G�̍������u���b�N�̉E���ɂ߂荞��
				pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth;
				pMove->x *= -1.0f;
			}
			else if (pPosOld->x + fWidth <= pBlock->pos.x - pBlock->fWidth &&
				pPos->x + fWidth  > pBlock->pos.x - pBlock->fWidth)
			{//�G�̉E�����u���b�N�̍����ɂ߂荞��
				pPos->x = pBlock->pos.x - pBlock->fWidth - fWidth;
				pMove->x *= -1.0f;
			}
		}
	}
}

//============================================================================================================
//�u���b�N�̎擾
//============================================================================================================
BLOCK *GetBlock(void)
{
	return &s_aBlock[0];
}