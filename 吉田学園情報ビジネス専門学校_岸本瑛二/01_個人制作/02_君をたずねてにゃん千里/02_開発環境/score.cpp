//============================================================================================================
//
//�N�������˂Ăɂ��痢[score.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "score.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static D3DXVECTOR3				s_posScore;					//�X�R�A�̈ʒu
static int						s_nScore;					//�X�R�A�̒l

//============================================================================================================
//�X�R�A�̏���������
//============================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/number000.png",
								&s_pTexture);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//�X�R�A�̏��̏�����
		s_posScore = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);	//�X�R�A�̈ʒu��������
		s_nScore   = 0;												//�X�R�A�̒l��������
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH),				 s_posScore.y,		  		  0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH, s_posScore.y,				  0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH),				 s_posScore.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_posScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH, s_posScore.y + SCORE_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�X�R�A�̏I������
//============================================================================================================
void UninitScore(void)
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
//�X�R�A�̍X�V����
//============================================================================================================
void UpdateScore(void)
{

}

//============================================================================================================
//�X�R�A�̕`�揈��
//============================================================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�X�R�A�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
								2 * MAX_SCORE);			//�`�悷��v���~�e�B�u��
	}
}

//============================================================================================================
//�X�R�A�̐ݒ菈��
//============================================================================================================
void SetScore(int nScore)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nScore	= nScore;
	aPosTexU[0] = s_nScore % 100000000 / 10000000;
	aPosTexU[1] = s_nScore % 10000000 / 1000000;
	aPosTexU[2] = s_nScore % 1000000 / 100000;
	aPosTexU[3] = s_nScore % 100000 / 10000;
	aPosTexU[4] = s_nScore % 10000 / 1000;
	aPosTexU[5] = s_nScore % 1000 / 100;
	aPosTexU[6] = s_nScore % 100 / 10;
	aPosTexU[7] = s_nScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
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
//�X�R�A�̉��Z����
//============================================================================================================
void AddScore(int nValue)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nScore   += nValue;
	aPosTexU[0]	= s_nScore % 100000000 / 10000000;
	aPosTexU[1] = s_nScore % 10000000 / 1000000;
	aPosTexU[2] = s_nScore % 1000000 / 100000;
	aPosTexU[3] = s_nScore % 100000 / 10000;
	aPosTexU[4] = s_nScore % 10000 / 1000;
	aPosTexU[5] = s_nScore % 1000 / 100;
	aPosTexU[6] = s_nScore % 100 / 10;
	aPosTexU[7] = s_nScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
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
//�X�R�A�̌��Z����
//============================================================================================================
void SubScore(int nValue)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	//���Z����
	s_nScore -= nValue;

	if (s_nScore <= 0)
	{//0�ȉ���������
		s_nScore = 0;
	}

	aPosTexU[0] = s_nScore % 100000000 / 10000000;
	aPosTexU[1] = s_nScore % 10000000 / 1000000;
	aPosTexU[2] = s_nScore % 1000000 / 100000;
	aPosTexU[3] = s_nScore % 100000 / 10000;
	aPosTexU[4] = s_nScore % 10000 / 1000;
	aPosTexU[5] = s_nScore % 1000 / 100;
	aPosTexU[6] = s_nScore % 100 / 10;
	aPosTexU[7] = s_nScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
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
//�X�R�A�̎擾
//============================================================================================================
int GetScore(void)
{
	return s_nScore;
}