//============================================================================================================
//
//�N�������˂Ăɂ��痢[item.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "item.h"
#include "player.h"
#include "score.h"
#include "goal.h"
#include "sound.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static Item						s_aItem[MAX_ITEM];		//�A�C�e���̏��

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void ItemMotion(void);

//============================================================================================================
//�A�C�e���̏���������
//============================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̃|�C���^
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�n�[�g_02.png",
								&s_pTexture);

	for (int i = 0; i < MAX_ITEM; i++)
	{//���̏�����
		s_aItem[i].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu������������
		s_aItem[i].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ��ʂ�����������
		s_aItem[i].fWidth	= 0;								//��������������
		s_aItem[i].fHeight	= 0;								//����������������
		s_aItem[i].nCounter = 0;								//�J�E���^�[������������
		s_aItem[i].type		= ITEM_TYPE_COIN;					//�R�C���ɂ���
		s_aItem[i].bUse		= false;							//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
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
//�A�C�e���̏I������
//============================================================================================================
void UninitItem(void)
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
//�A�C�e���̍X�V����
//============================================================================================================
void UpdateItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];	//�|�C���^�ɕϐg		

		if (!pItem->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*���A�C�e�����g�p����Ă��遫*/

		Player *pPlayer = GetPlayer();		//�v���C���[���

		if (!GetPosGoal())
		{//�S�[�����w��̈ʒu�ɂ��Ȃ�
			//�v���C���[�̈ړ��ʂ�(������Ƃ���������)�����
			pItem->move.x = -(pPlayer->move.x * 0.7f);

			if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
			{//�v���C���[�̉E�[����ʒ��� & �����Ă���
				//�ʒu���X�V
				pItem->pos.x += pItem->move.x;
			}
		}

		//�A�C�e���̃��[�V����
		ItemMotion();

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�|�C���^��4���i�߂�

		//���_���W�̍X�V
		float fLeft		= (pItem->pos.x - pItem->fWidth);	//��
		float fRight	= (pItem->pos.x + pItem->fWidth);	//�E
		float fUp		= (pItem->pos.y - pItem->fHeight);	//��
		float fDown		= (pItem->pos.y + pItem->fHeight);	//��

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//============================================================================================================
//�A�C�e���̕`�揈��
//============================================================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (s_aItem[i].bUse)
		{//�A�C�e�����g�p����Ă���
			 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
									i * 4,					//�`����J�n���钸�_�C���f�b�N�X
									2);						//�`�悷��v���~�e�B�u��
		}
	}
}

//============================================================================================================
//�u���b�N�̐ݒ菈��
//============================================================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEM_TYPE itemType)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];	//�|�C���^�ɕϐg

		if (!pItem->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			pItem->pos		= pos;
			pItem->move		= move;
			pItem->fWidth	= (ITEM_WIDTH / 2);
			pItem->fHeight	= (ITEM_HEIGHT / 2);
			pItem->type		= itemType;
			pItem->bUse		= true;					//�g�p���Ă����Ԃɂ���

			//���_���W�̐ݒ�
			pVtx[0].pos.x = pos.x - pItem->fWidth;
			pVtx[0].pos.y = pos.y - pItem->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pos.x + pItem->fWidth;
			pVtx[1].pos.y = pos.y - pItem->fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pos.x - pItem->fWidth;
			pVtx[2].pos.y = pos.y + pItem->fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pos.x + pItem->fWidth;
			pVtx[3].pos.y = pos.y + pItem->fHeight;
			pVtx[3].pos.z = 0.0f;

			//�e�N�X�`�����W�̐ݒ�
			float fTexU = pItem->fWidth	 / (ITEM_WIDTH / 2);
			float fTexV = pItem->fHeight / (ITEM_HEIGHT / 2);

			SetTex(pVtx, 0.0f, fTexU, 0.0f, fTexV);
			break;
		}

		pVtx += 4;		//�|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�A�C�e���̃��[�V����
//============================================================================================================
void ItemMotion(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];	//�|�C���^�ɕϐg

		if (!pItem->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*���A�C�e�����g�p����Ă��遫*/

		//�A�C�e�����㉺������
		pItem->pos.y += sinf((D3DX_PI * 2) * (pItem->nCounter * 0.001f)) * 0.07f;

		//�J�E���g�A�b�v
		pItem->nCounter++;

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�A�C�e���̓����蔻��
//============================================================================================================
void CollisionItem(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];

		if (!pItem->bUse)
		{//�A�C�e�����g�p����Ă��Ȃ�
			continue;
		}

		/*���A�C�e�����g�p����Ă��遫*/

		if (pPos->x + (fWidth / 5)  > pItem->pos.x - pItem->fWidth &&
			pPos->x - (fWidth / 5)  < pItem->pos.x + pItem->fWidth &&
			pPos->y - fHeight		< pItem->pos.y + pItem->fHeight &&
			pPos->y					> pItem->pos.y - pItem->fHeight)
		{//�Ώۂ��A�C�e���͈͓̔��ɂ���
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_ITEMGET);

			//�X�R�A�̉��Z
			AddScore(100);

			pItem->bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
		}
	}
}

//============================================================================================================
//�A�C�e���̎擾
//============================================================================================================
Item *GetItem(void)
{
	return &s_aItem[0];
}