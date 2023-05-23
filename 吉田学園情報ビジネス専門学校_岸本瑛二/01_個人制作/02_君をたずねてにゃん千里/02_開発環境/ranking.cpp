//============================================================================================================
//
//�N�������˂Ăɂ��痢[ranking.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "time.h"
#include "sound.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTextureRank				= NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureRankScore			= NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureRankingBG			= NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureRankingLogo		= NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRank				= NULL;		//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRankScore			= NULL;		//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRankingBG			= NULL;		//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRankingLogo		= NULL;		//���_�o�b�t�@�ւ̃|�C���^
static RankScore				s_aRankScore[MAX_RANK + 1];				//�����L���O�X�R�A���
static int						s_nRankUpdate;							//�X�V�����NNo.
static RANK						s_Rank;									//�����N�̓_�ŏ��
static D3DXVECTOR2				s_posRank;								//���ʂ̈ʒu
static D3DXVECTOR2				s_PosLogo;								//���S�̈ʒu

//============================================================================================================
//�����L���O�̏���������
//============================================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Rank_1to5_Copy.png",
								&s_pTextureRank);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Number_White_Copy.png",
								&s_pTextureRankScore);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/ACG_Ranking.png",
								&s_pTextureRankingBG);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/RANKING_Copy.png",
								&s_pTextureRankingLogo);

	//�����L���O�̏��̏�����
	for (int i = 0; i < (MAX_RANK + 1); i++)
	{
		s_aRankScore[i].pos.x = RANKINGSCORE_POS_X;
		s_aRankScore[i].pos.y = RANKINGSCORE_POS_Y + (i * RANKINGSCORE_SPACE);
		s_aRankScore[i].pos.z = 0.0f;
		s_aRankScore[i].col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	s_nRankUpdate = -1;													//�X�V�����NNo.
	s_posRank = D3DXVECTOR2(RANK_POS_X, RANK_POS_Y);					//���ʂ̈ʒu
	s_PosLogo = D3DXVECTOR2(RANKING_LOGO_POS_X, RANKING_LOGO_POS_Y);	//���S�̈ʒu

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRank,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE * MAX_RANK,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRankScore,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRankingBG,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRankingLogo,
								NULL);

	/*�����ʂ̕���*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	{//���_���W�̐ݒ�
		float fLeft		= (s_posRank.x - RANK_WIDTH);	//��
		float fRight	= (s_posRank.x + RANK_WIDTH);	//�E
		float fUp		= (s_posRank.y - RANK_HEIGHT);	//��
		float fDown		= (s_posRank.y + RANK_HEIGHT);	//��

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
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRank->Unlock();

	/*�������L���O�X�R�A�̕���*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_RANK; i++)
	{
		for (int j = 0; j < MAX_SCORE; j++)
		{
			{//���_���W�̐ݒ�
				float fLeft	  = (s_aRankScore[i].pos.x + (j * RANKINGSCORE_WIDTH));							//��
				float fRight  = (s_aRankScore[i].pos.x + (j * RANKINGSCORE_WIDTH) + RANKINGSCORE_WIDTH);	//�E
				float fUp	  = (s_aRankScore[i].pos.y + (i * RANKINGSCORE_HEIGHT));						//��
				float fDown	  = (s_aRankScore[i].pos.y + (i * RANKINGSCORE_HEIGHT) + RANKINGSCORE_HEIGHT);	//��

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
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			SetTex(pVtx, 0.0f, 0.1f, 0.0f, 1.0f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankScore->Unlock();

	/*���w�i�摜�̕���*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankingBG->Lock(0, 0, (void**)&pVtx, 0);

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

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankingBG->Unlock();

	/*�������L���O���S��*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);

	{//���_���W�̐ݒ�
		float fLeft		= (s_PosLogo.x - RANKING_LOGO_WIDTH);
		float fRight	= (s_PosLogo.x + RANKING_LOGO_WIDTH);
		float fUp		= (s_PosLogo.y - RANKING_LOGO_HEIGHT);
		float fDown		= (s_PosLogo.y + RANKING_LOGO_HEIGHT);

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
	s_pVtxBuffRankingLogo->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_RANKING);

	//�^�C���̏���������
	InitTime();

	//�^�C���̐ݒ菈��
	SetTime(RANKING_TIME);
}

//============================================================================================================
//�����L���O�̏I������
//============================================================================================================
void UninitRanking(void)
{
	//�T�E���h�̒�~
	StopSound();

	/*���e�N�X�`���̔j����*/

	if (s_pTextureRank != NULL)
	{//����
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	if (s_pTextureRankScore != NULL)
	{//�X�R�A�\��
		s_pTextureRankScore->Release();
		s_pTextureRankScore = NULL;
	}

	if (s_pTextureRankingBG != NULL)
	{//�w�i�摜
		s_pTextureRankingBG->Release();
		s_pTextureRankingBG = NULL;
	}

	if (s_pTextureRankingLogo != NULL)
	{//���S
		s_pTextureRankingLogo->Release();
		s_pTextureRankingLogo = NULL;
	}

	/*�����_�o�b�t�@�̔j����*/

	if (s_pVtxBuffRank != NULL)
	{//����
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	if (s_pVtxBuffRankScore != NULL)
	{//�X�R�A�\��
		s_pVtxBuffRankScore->Release();
		s_pVtxBuffRankScore = NULL;
	}

	if (s_pVtxBuffRankingBG != NULL)
	{//�w�i�摜
		s_pVtxBuffRankingBG->Release();
		s_pVtxBuffRankingBG = NULL;
	}

	if (s_pVtxBuffRankingLogo != NULL)
	{//���S
		s_pVtxBuffRankingLogo->Release();
		s_pVtxBuffRankingLogo = NULL;
	}

	//�^�C���̏I������
	UninitTime();
}

//============================================================================================================
//�����L���O�̍X�V����
//============================================================================================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	/*�������L���O�X�R�A�̓_�Ł�*/

	if (s_nRankUpdate != -1)
	{//�X�V�����NNo.��0�ȏ�̎�
		if (s_Rank == RANK_IN)
		{//�����ɂ���Ƃ�
			s_aRankScore[s_nRankUpdate].col.a -= RANKING_FADE_SPEED;	//�����ɂ��Ă���

			if (s_aRankScore[s_nRankUpdate].col.a <= 0.0f)
			{//0.0f���߂�����
				s_aRankScore[s_nRankUpdate].col.a = 0.0f;	//0.0f�ɖ߂�
				s_Rank = RANK_OUT;							//�s�����ɂ����Ԃ�
			}
		}
		else if (s_Rank == RANK_OUT)
		{//�s�����ɂ���Ƃ�
			s_aRankScore[s_nRankUpdate].col.a += RANKING_FADE_SPEED;	//�|���S����s�����ɂ��Ă���

			if (s_aRankScore[s_nRankUpdate].col.a >= 1.0f)
			{//1.0f���߂�����
				s_aRankScore[s_nRankUpdate].col.a = 1.0f;	//1.0f�ɖ߂�
				s_Rank = RANK_IN;							//�����ɂ����Ԃ�
			}
		}

		pVtx += (s_nRankUpdate * 4) * MAX_SCORE;

		//���_�J���[�̐ݒ�
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, s_aRankScore[s_nRankUpdate].col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, s_aRankScore[s_nRankUpdate].col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, s_aRankScore[s_nRankUpdate].col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, s_aRankScore[s_nRankUpdate].col.a);

			pVtx += 4;		//�|�C���^��4���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankScore->Unlock();

	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ���
		if (GetKeyboardTrigger(DIK_RETURN))
		{//Enter�L�[�������ꂽ
			//�^�C�g����ʂɑJ��
			SetFade(MODE_TITLE);
		}
	}

	//�^�C���̍X�V����
	UpdateTime();
}

//============================================================================================================
//�����L���O�̕`�揈��
//============================================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	/*���w�i�摜��*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRankingBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureRankingBG);
		
	//�����L���O�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��

	/*�����ʁ�*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureRank);
		
	//�����L���O�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��

	/*�������L���O�X�R�A��*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureRankScore);
		
	for (int i = 0; i < MAX_SCORE * MAX_RANK; i++)
	{
		//�����L���O�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								i * 4,					//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}

	/*�����S��*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRankingLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureRankingLogo);
		
	//�����L���O�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��
}

//============================================================================================================
//�����L���O�̃��Z�b�g
//============================================================================================================
void ResetRanking(void)
{
	//�����L���O�X�R�A�̃��[�h
	LoadRanking();
}

//============================================================================================================
//�����L���O�̐ݒ菈��
//============================================================================================================
void SetRanking(int nScore)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int nChange;				//����ւ��p
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//�X�R�A�����ւ���
	s_aRankScore[5].nScore = nScore;

	//�I���\�[�g�ɂ����ѕς�
	for (int i = 0; i < MAX_RANK; i++)
	{
		for (int nCnt = i + 1; nCnt < (MAX_RANK + 1); nCnt++)
		{
			nChange = 0;
			nChange = s_aRankScore[i].nScore;

			if (s_aRankScore[i].nScore < s_aRankScore[nCnt].nScore)
			{//�傫�����ɕ��ѕς���
				s_aRankScore[i].nScore = s_aRankScore[nCnt].nScore;
				s_aRankScore[nCnt].nScore = nChange;
			}
		}
	}

	//�����L���O�X�R�A�̃Z�[�u
	SaveRanking();

	//�X�V�����NNo.���X�V
	for (int i = 0; i < MAX_RANK; i++)
	{
		if (s_aRankScore[i].nScore == nScore)
		{//�V�����X�R�A�ƈ�v�����ꍇ
			s_nRankUpdate = i;		//�J�E���g����������
			break;
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	//�e���ɃX�R�A�̐���������
	for (int i = 0; i < MAX_RANK; i++)
	{
		aPosTexU[0] = s_aRankScore[i].nScore % 100000000 / 10000000;
		aPosTexU[1] = s_aRankScore[i].nScore % 10000000 / 1000000;
		aPosTexU[2] = s_aRankScore[i].nScore % 1000000 / 100000;
		aPosTexU[3] = s_aRankScore[i].nScore % 100000 / 10000;
		aPosTexU[4] = s_aRankScore[i].nScore % 10000 / 1000;
		aPosTexU[5] = s_aRankScore[i].nScore % 1000 / 100;
		aPosTexU[6] = s_aRankScore[i].nScore % 100 / 10;
		aPosTexU[7] = s_aRankScore[i].nScore % 10 / 1;

		//�e�N�X�`�����W�̐ݒ�
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankScore->Unlock();
}

//============================================================================================================
//�����L���O�̃Z�[�u����
//============================================================================================================
void SaveRanking(void)
{
	FILE *pFile;			//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("RankingScore.txt", "w");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C���ɃX�R�A���Z�[�u����
		for (int i = 0; i < MAX_RANK; i++)
		{
			fprintf(pFile, "%d\n", s_aRankScore[i].nScore);
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf(" *** �t�@�C�����J���Ȃ��c���Ɓc *** \n");
	}
}

//============================================================================================================
//�����L���O�̃��[�h����
//============================================================================================================
void LoadRanking(void)
{
	FILE *pFile;			//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("RankingScore.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C������X�R�A�����[�h����
		for (int i = 0; i < MAX_RANK; i++)
		{
			fscanf(pFile, "%d\n", &s_aRankScore[i].nScore);
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf(" *** �t�@�C�����J���Ȃ��c���Ɓc *** \n");
	}
}