//============================================================================================================
//
//�N�������˂Ăɂ��痢[fade.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "fade.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static FADE					   s_fade;					//�t�F�[�h�̏��
static MODE					   s_modeNext;				//���̉��(���[�h)
static D3DXCOLOR			   s_colorFade;				//�|���S��(�t�F�[�h)�̐F

//============================================================================================================
//�t�F�[�h�̏���������
//============================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^

	s_fade		= FADE_IN;								//�t�F�[�h�C�����
	s_modeNext	= modeNext;								//���̉��(���[�h)��ݒ�
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S��(�s����)�ɂ��Ă���

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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���[�h�̐ݒ�
	SetMode(s_modeNext);
}

//============================================================================================================
//�t�F�[�h�̏I������
//============================================================================================================
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//�t�F�[�h�̍X�V����
//============================================================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (s_fade != FADE_NONE)
	{//�t�F�[�h���Ă���Ƃ�
		if (s_fade == FADE_IN)
		{//�t�F�[�h�C�����
			s_colorFade.a -= FADE_SPEED;	//�|���S���𓧖��ɂ��Ă���

			if (s_colorFade.a <= 0.0f)
			{
				s_colorFade.a = 0.0f;
				s_fade		  = FADE_NONE;		//�������Ă��Ȃ���Ԃ�
			}
		}
		else if (s_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���
			s_colorFade.a += FADE_SPEED;	//�|���S����s�����ɂ��Ă���

			if (s_colorFade.a >= 1.0f)
			{
				s_colorFade.a = 1.0f;
				s_fade		  = FADE_IN;	//�t�F�[�h�C�����

				//���[�h�ݒ�(���̉�ʂɈڍs)
				SetMode(s_modeNext);
			}
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, s_colorFade.a);
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//�t�F�[�h�̕`�揈��
//============================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,NULL);

	//�G�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��
}

//============================================================================================================
//�t�F�[�h�̐ݒ菈��
//============================================================================================================
void SetFade(MODE modeNext)
{
	s_fade		= FADE_OUT;								//�t�F�[�h�A�E�g��Ԃ�
	s_modeNext	= modeNext;								//���̉��(���[�h)��ݒ�
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�����|���S��(����)�ɂ��Ă���
}

//============================================================================================================
//�t�F�[�h�̎擾����
//============================================================================================================
FADE GetFade(void)
{
	return s_fade;
}