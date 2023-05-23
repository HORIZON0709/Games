//============================================================================================================
//
//�N�������˂Ăɂ��痢[time.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "time.h"
#include "game.h"
#include "fade.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static D3DXVECTOR3				s_posTime;				//�^�C���̈ʒu
static int						s_nTime;				//�^�C���̒l
static int						s_nCntTime;				//�^�C���̃J�E���g

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void SubTime(int nValue);

//============================================================================================================
//�^�C���̏���������
//============================================================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/number000.png",
								&s_pTexture);

	for (int nCnt = 0; nCnt < TIME_MAX; nCnt++)
	{
		//�X�R�A�̏��̏�����
		s_posTime	= D3DXVECTOR3(600.0f, 10.0f, 0.0f);		//�ʒu��������
		s_nTime		= 0;									//�l��������
		s_nCntTime	= 0;									//�J�E���g��������
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TIME_MAX; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(s_posTime.x + (nCnt * TIME_WIDTH),			  s_posTime.y,				 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_posTime.x + (nCnt * TIME_WIDTH) + TIME_WIDTH, s_posTime.y,				 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_posTime.x + (nCnt * TIME_WIDTH),			  s_posTime.y + TIME_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_posTime.x + (nCnt * TIME_WIDTH) + TIME_WIDTH, s_posTime.y + TIME_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		SetTex(pVtx, 0.0f, 0.1f, 0.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�^�C���̏I������
//============================================================================================================
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TIME_MAX; nCnt++)
	{
		if (s_pTexture != NULL)
		{
			s_pTexture->Release();
			s_pTexture = NULL;
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
//�^�C���̍X�V����
//============================================================================================================
void UpdateTime(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (s_nTime <= 10 && s_nTime >= 0)
	{//�c�莞��10�b�̎�
		for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
		{
			//���_�J���[�̍X�V
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			pVtx += 4;		//�|�C���^��4���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	s_nCntTime++;		//�J�E���g�A�b�v

	if (s_nCntTime % 60 == 0)
	{//���Ԍo��
		SubTime(1);
	}

	//�^�C���I�[�o�[���̉�ʑJ��
	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�
		if (s_nTime <= 0)
		{//�^�C���I�[�o�[�ɂȂ�����
			//�I����Ԃɐݒ�
			SetGameState(GAMESTATE_END);
		}
	}
}

//============================================================================================================
//�^�C���̕`�揈��
//============================================================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TIME_MAX; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�^�C���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
								2 * TIME_MAX);			//�`�悷��v���~�e�B�u��
	}
}

//============================================================================================================
//�^�C���̐ݒ菈��
//============================================================================================================
void SetTime(int nTime)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[TIME_MAX];		//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime		= nTime;
	aPosTexU[0] = s_nTime % 1000 / 100;
	aPosTexU[1] = s_nTime % 100 / 10;
	aPosTexU[2] = s_nTime % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < TIME_MAX; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�^�C���̌��Z����
//============================================================================================================
void SubTime(int nValue)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[TIME_MAX];		//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	if (s_nTime > 0)
	{//�^�C����0���傫��
		s_nTime -= nValue;
	}

	aPosTexU[0] = s_nTime % 1000 / 100;
	aPosTexU[1] = s_nTime % 100 / 10;
	aPosTexU[2] = s_nTime % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < TIME_MAX; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�f�o�C�X�̎擾
//============================================================================================================
int GetTime(void)
{
	return s_nTime;
}