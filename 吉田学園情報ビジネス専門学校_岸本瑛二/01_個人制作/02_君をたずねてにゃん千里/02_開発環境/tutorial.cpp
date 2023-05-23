//============================================================================================================
//
//�N�������˂Ăɂ��痢[tutorial.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "tutorial.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_apTexture[NUM_TUTORIAL] = { NULL,NULL };	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff				  = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static int						s_nCountTex;								//�e�N�X�`���̃J�E���g

//============================================================================================================
//�`���[�g���A���̏���������
//============================================================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Tutorial_01.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Tutorial_02.png",
								&s_apTexture[1]);

	s_nCountTex = 0;		//�e�N�X�`���̃J�E���g������������

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
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�`���[�g���A���̏I������
//============================================================================================================
void UninitTutorial(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�e�N�X�`���̔j��
	for (int i = 0; i < NUM_TUTORIAL; i++)
	{
		if (s_apTexture[i] != NULL)
		{
			s_apTexture[i]->Release();
			s_apTexture[i] = NULL;
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
//�`���[�g���A���̍X�V����
//============================================================================================================
void UpdateTutorial(void)
{
	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�
		if (GetKeyboardTrigger(DIK_D))
		{//D�L�[�������ꂽ
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_SELECT);

			s_nCountTex = 1;
		}
		else if (GetKeyboardTrigger(DIK_A))
		{//A�L�[�������ꂽ
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_SELECT);

			s_nCountTex = 0;
		}
		else if (GetKeyboardTrigger(DIK_RETURN))
		{//Enter�L�[�������ꂽ
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DECIDE);

			//�Q�[����ʂɈڍs
			SetFade(MODE_GAME);
		}
	}
}

//============================================================================================================
//�`���[�g���A���̕`�揈��
//============================================================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_apTexture[s_nCountTex]);

	//�`���[�g���A���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��
}

//============================================================================================================
//�`���[�g���A���̎擾
//============================================================================================================
int GetTutorial(void)
{
	return s_nCountTex;
}