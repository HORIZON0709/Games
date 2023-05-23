//============================================================================================================
//
//�N�������˂Ăɂ��痢[bg,cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "bg.h"
#include "input.h"
#include "player.h"
#include "goal.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_apTexture[NUM_BG] = { NULL,NULL, NULL };	//�e�N�X�`��(3����)�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;							//���_�o�b�t�@�ւ̃|�C���^
static D3DXVECTOR2				s_aPosTexUV[NUM_BG];						//�e�N�X�`�����W�̊J�n�ʒu(UV�l)
static D3DXVECTOR2				s_aMoveBG;									//�w�i�̈ړ���

//============================================================================================================
//�w�i�̏���������
//============================================================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^
	int nCntBG;									//�w�i�̃J�E���g

	//�e�N�X�`���̓ǂݍ���(�R����)

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/bg101.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/��Ɖ_.png",
								&s_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�Z��X_�ƁX_2_Copy.png",
								&s_apTexture[2]);

	//������
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		s_aPosTexUV[nCntBG] = D3DXVECTOR2(0.0f, 0.0f);
	}

	s_aMoveBG = D3DXVECTOR2(BG_SPEED, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

		pVtx += 4;		//�|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�w�i�̏I������
//============================================================================================================
void UninitBG(void)
{
	//�e�N�X�`���̔j��
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (s_apTexture[nCntBG] != NULL)
		{
			s_apTexture[nCntBG]->Release();
			s_apTexture[nCntBG] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//�w�i�̍X�V����
//============================================================================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^
	Player *pPlayer = GetPlayer();	//�v���C���[�̎擾
	int nCntBG;						//�w�i�̃J�E���g

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!GetPosGoal())
	{//�S�[�����w��̈ʒu�ɂ��Ȃ�
		if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
		{//�v���C���[�̉E�[����ʒ��� & �����Ă���

			//�v���C���[��move��w�i��move�ɑ��(�i�s�����Ƌt)
			s_aMoveBG.x = -(pPlayer->move.x * 0.001f);

			//�ʒu���X�V
			for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
			{
				s_aPosTexUV[nCntBG] = s_aMoveBG;
			}

			for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
			{
				//�e�N�X�`�����W�̍X�V
				pVtx[0].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);
				pVtx[1].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);
				pVtx[2].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);
				pVtx[3].tex.x -= (s_aPosTexUV[nCntBG].x) / (nCntBG + 2);

				pVtx += 4;		//�|�C���^��4���i�߂�
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�w�i�̕`�揈��
//============================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	int nCntBG;									//�w�i�̃J�E���g

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_apTexture[nCntBG]);

		//�w�i�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCntBG * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}
}