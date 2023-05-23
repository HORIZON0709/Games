//============================================================================================================
//
//�N�������˂Ăɂ��痢[goal.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "goal.h"
#include "game.h"
#include "player.h"
#include "fade.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static GOAL						s_goal;					//�S�[���̏��
static bool						bGoal;					//�S�[���������ǂ���
static bool						bPosGoal;				//�S�[�����ǂ���

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void TexAnimGoal(void);

//============================================================================================================
//�S�[���̏���������
//============================================================================================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Goal_Copy.png",
								&s_pTexture);

	//���̏�����
	s_goal.pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_goal.move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_goal.fWidth	= 0.0f;
	s_goal.fHeight	= 0.0f;
	s_goal.nCntAnim = 0;
	s_goal.nPtnAnim = 0;
	s_goal.bUse		= false;	//�g�p����Ă��Ȃ���Ԃɂ���

	bGoal = false;		//�S�[�����Ă��Ȃ���Ԃɂ���
	bPosGoal = false;	//�w��̈ʒu�ɂ��Ȃ���Ԃɂ���

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	SetTex(pVtx, 0.0f, 0.25f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�S�[���̏I������
//============================================================================================================
void UninitGoal(void)
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
//�S�[���̍X�V����
//============================================================================================================
void UpdateGoal(void)
{
	Player *pPlayer = GetPlayer();

	if (!bPosGoal)
	{//�w��̈ʒu�ɂ��Ȃ��Ƃ�
		//�v���C���[�̈ړ��ʂ�(������Ƃ���������)�����
		s_goal.move.x = -(pPlayer->move.x * 0.7f);

		if (pPlayer->pos.x + pPlayer->fWidth >= (SCREEN_WIDTH / 2) && pPlayer->move.x != SPEED_STOP)
		{//�v���C���[�̉E�[����ʒ��� & �����Ă���
			//�ʒu���X�V
			s_goal.pos.x += s_goal.move.x;
		}

		if (s_goal.pos.x <= (SCREEN_WIDTH - s_goal.fWidth))
		{//�w��̈ʒu�ɒ�������
			bPosGoal = true;	//�w��̈ʒu�ɒ�����
		}
	}

	//�S�[���̃e�N�X�`���A�j���[�V����
	TexAnimGoal();

	if (pPlayer->pos.x > s_goal.pos.x - s_goal.fWidth &&
		pPlayer->pos.x < s_goal.pos.x + s_goal.fWidth &&
		pPlayer->pos.y - pPlayer->fHeight < s_goal.pos.y + s_goal.fHeight &&
		pPlayer->pos.y					  > s_goal.pos.y - s_goal.fHeight)
	{//�v���C���[���S�[���ɐG�ꂽ��
		if (GetFade() == FADE_NONE && GetGamaState() != GAMESTATE_END)
		{//�t�F�[�h���Ă��Ȃ��Ƃ� & �I����Ԃł͂Ȃ��Ƃ�
			bGoal = true;	//�S�[������

			//�I����Ԃɐݒ�
			SetGameState(GAMESTATE_END);
		}
	}

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	float fLeft		= (s_goal.pos.x - s_goal.fWidth);	//��
	float fRight	= (s_goal.pos.x + s_goal.fWidth);	//�E
	float fUp		= (s_goal.pos.y - s_goal.fHeight);	//��
	float fDown		= (s_goal.pos.y + s_goal.fHeight);	//��

	pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	SetTex(pVtx,
			(1.0f / GOAL_MAX_PATTERN) *	 s_goal.nPtnAnim,
			(1.0f / GOAL_MAX_PATTERN) * (s_goal.nPtnAnim + 1),
			0.0f,1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�S�[���̕`�揈��
//============================================================================================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	if (s_goal.bUse)
	{//�S�[�����g�p����Ă���
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�S�[���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}
}

//============================================================================================================
//�S�[���̐ݒ菈��
//============================================================================================================
void SetGoal(D3DXVECTOR3 pos)
{
	if (!s_goal.bUse)
	{//�S�[�����g�p����Ă��Ȃ��Ƃ�
		s_goal.pos		= pos;
		s_goal.fWidth	= (GOAL_WIDTH * 0.5f);
		s_goal.fHeight	= (GOAL_HEIGHT * 0.5f);
		s_goal.bUse		= true;			//�g�p���Ă����Ԃɂ���

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		float fLeft		= (s_goal.pos.x - s_goal.fWidth);	//��
		float fRight	= (s_goal.pos.x + s_goal.fWidth);	//�E
		float fUp		= (s_goal.pos.y - s_goal.fHeight);	//��
		float fDown		= (s_goal.pos.y + s_goal.fHeight);	//��

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//============================================================================================================
//�S�[���̃e�N�X�`���A�j���[�V����
//============================================================================================================
void TexAnimGoal(void)
{
	s_goal.nCntAnim++;		//�J�E���g�A�b�v

	if (s_goal.nCntAnim % GOAL_ANIM_SPEED == 0)
	{//��莞�Ԍo�߂�����
		s_goal.nPtnAnim = (s_goal.nPtnAnim + 1) % GOAL_MAX_PATTERN;	//�p�^�[���ԍ����X�V����
	}
}

//============================================================================================================
//�S�[���������ǂ������擾
//============================================================================================================
bool GetGoal(void)
{
	return bGoal;
}

//============================================================================================================
//�S�[�����w��̈ʒu�ɂ��邩�ǂ������擾
//============================================================================================================
bool GetPosGoal(void)
{
	return bPosGoal;
}