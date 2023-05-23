//============================================================================================================
//
//�N�������˂Ăɂ��痢[title.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTextureBG		 = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureLogo		 = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTexturePressEnter = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBG		 = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPressEnter = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffLogo		 = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static PressEnter				s_PressEnter;					//PressEnter�̏��
static FADEENTER				s_FadeEnter;					//PressEnter�̓_�ŏ��
static D3DXVECTOR3				s_posTitleLogo;					//�^�C�g�����S�̈ʒu
static TITLELOGO				s_TitleLogo;					//�^�C�g�����S�̏��

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void TitleLogoMotion(void);

//============================================================================================================
//�^�C�g���̏���������
//============================================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/ACG_Title.png",
								&s_pTextureBG);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�N�������˂Ăɂ��痢_Copy.png",
								&s_pTextureLogo);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/PressEnter_Copy.png",
								&s_pTexturePressEnter);

	//���̏�����
	s_PressEnter.pos = D3DXVECTOR3(PRESSENTER_POS_X, PRESSENTER_POS_Y, 0.0f);
	s_PressEnter.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	s_FadeEnter		 = FADEENTER_IN;
	s_posTitleLogo	 = D3DXVECTOR3((SCREEN_WIDTH / 2), -170.0f, 0.0f);
	s_TitleLogo		 = TITLELOGO_POP;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffBG,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffLogo,
								NULL);
	
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPressEnter,
								NULL);

	/*���w�i�摜��*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

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
	s_pVtxBuffBG->Unlock();

	/*���^�C�g�����S��*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
	pVtx[0].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
	pVtx[1].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
	pVtx[2].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
	pVtx[3].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffLogo->Unlock();

	/*��Press Enter�̉摜��*/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = s_PressEnter.pos.x - PRESSENTER_WIDTH;
	pVtx[0].pos.y = s_PressEnter.pos.y - PRESSENTER_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_PressEnter.pos.x + PRESSENTER_WIDTH;
	pVtx[1].pos.y = s_PressEnter.pos.y - PRESSENTER_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_PressEnter.pos.x - PRESSENTER_WIDTH;
	pVtx[2].pos.y = s_PressEnter.pos.y + PRESSENTER_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_PressEnter.pos.x + PRESSENTER_WIDTH;
	pVtx[3].pos.y = s_PressEnter.pos.y + PRESSENTER_HEIGHT;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPressEnter->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_TITLE);
}

//============================================================================================================
//�^�C�g���̏I������
//============================================================================================================
void UninitTitle(void)
{
	//�T�E���h�̒�~
	StopSound();

	/*���e�N�X�`���̔j����*/

	if (s_pTextureBG != NULL)
	{//�w�i�摜�̕�
		s_pTextureBG->Release();
		s_pTextureBG = NULL;
	}

	if (s_pTextureLogo != NULL)
	{//�^�C�g�����S�̕�
		s_pTextureLogo->Release();
		s_pTextureLogo = NULL;
	}

	if (s_pTextureBG != NULL)
	{//Press Enter�̕�
		s_pTexturePressEnter->Release();
		s_pTexturePressEnter = NULL;
	}

	/*�����_�o�b�t�@�̔j����*/

	if (s_pVtxBuffBG != NULL)
	{//�w�i�摜�̕�
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	if (s_pVtxBuffLogo != NULL)
	{//�^�C�g�����S�̕�
		s_pVtxBuffLogo->Release();
		s_pVtxBuffLogo = NULL;
	}

	if (s_pVtxBuffPressEnter != NULL)
	{//Press Enter�̕�
		s_pVtxBuffPressEnter->Release();
		s_pVtxBuffPressEnter = NULL;
	}
}

//============================================================================================================
//�^�C�g���̍X�V����
//============================================================================================================
void UpdateTitle(void)
{
	//�^�C�g�����S�̓���
	TitleLogoMotion();

	/*��Press Enter�̓_�Ł�*/

	if (GetFade() == FADE_NONE || GetFade() == FADE_IN)
	{//�t�F�[�h���Ă��Ȃ�or�t�F�[�h�C����Ԃ̂Ƃ�
		if (s_FadeEnter == FADEENTER_IN)
		{//�����ɂ���Ƃ�
			s_PressEnter.col.a -= PRESSENTER_SPEED;	//�����ɂ��Ă���

			if (s_PressEnter.col.a <= 0.0f)
			{//0.0f���߂�����
				s_PressEnter.col.a = 0.0f;			//0.0f�ɖ߂�
				s_FadeEnter		   = FADEENTER_OUT;	//�s�����ɂ����Ԃ�
			}
		}
		else if (s_FadeEnter == FADEENTER_OUT)
		{//�s�����ɂ���Ƃ�
			s_PressEnter.col.a += PRESSENTER_SPEED;	//�s�����ɂ��Ă���

			if (s_PressEnter.col.a >= 1.0f)
			{//1.0f���߂�����
				s_PressEnter.col.a = 1.0f;			//1.0f�ɖ߂�
				s_FadeEnter		   = FADEENTER_IN;	//�����ɂ����Ԃ�
			}
		}
	}
	else if (GetFade() == FADE_OUT)
	{//�t�F�[�h�A�E�g��Ԃ̂Ƃ�
		if (s_FadeEnter == FADEENTER_IN)
		{//�����ɂ���Ƃ�
			s_PressEnter.col.a -= PRESSENTER_HIGHSPEED;	//�����ɂ��Ă���

			if (s_PressEnter.col.a <= 0.0f)
			{//0.0f���߂�����
				s_PressEnter.col.a = 0.0f;			//0.0f�ɖ߂�
				s_FadeEnter		   = FADEENTER_OUT;	//�s�����ɂ����Ԃ�
			}
		}
		else if (s_FadeEnter == FADEENTER_OUT)
		{//�s�����ɂ���Ƃ�
			s_PressEnter.col.a += PRESSENTER_HIGHSPEED;	//�s�����ɂ��Ă���

			if (s_PressEnter.col.a >= 1.0f)
			{//1.0f���߂�����
				s_PressEnter.col.a = 1.0f;			//1.0f�ɖ߂�
				s_FadeEnter		   = FADEENTER_IN;	//�����ɂ����Ԃ�
			}
		}
	}

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, s_PressEnter.col.a);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPressEnter->Unlock();
}

//============================================================================================================
//�^�C�g���̕`�揈��
//============================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	/*���w�i�摜�̕���*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureBG);

	//�^�C�g���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��

	/*���^�C�g�����S�̕���*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureLogo);

	//�^�C�g���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��

	/*��Press Enter�̕���*/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexturePressEnter);

	//�^�C�g���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��
}

//============================================================================================================
//�^�C�g�����S�̓���
//============================================================================================================
void TitleLogoMotion(void)
{
	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�

		switch (s_TitleLogo)
		{
		case TITLELOGO_NORMAL:		//�ʏ���

			if (GetKeyboardTrigger(DIK_RETURN))
			{//Enter�L�[�������ꂽ��
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//�`���[�g���A����ʂɈڍs
				SetFade(MODE_TUTORIAL);
			}
			break;

		case TITLELOGO_POP:		//�o�����

								//�^�C�g�����S�����X�ɍ~�낷
			s_posTitleLogo.y += LOGO_SPEED;

			if (s_posTitleLogo.y >= (SCREEN_HEIGHT / 5 * 2) || GetKeyboardTrigger(DIK_RETURN))
			{//��ʒu���z������ or Enter�L�[�������ꂽ��
				//��ʒu�ɂ���
				s_posTitleLogo.y = (SCREEN_HEIGHT / 5 * 2);

				//�ʏ��Ԃɂ���
				s_TitleLogo = TITLELOGO_NORMAL;
			}

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̍X�V
			pVtx[0].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
			pVtx[0].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
			pVtx[1].pos.y = s_posTitleLogo.y - LOGO_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = s_posTitleLogo.x - LOGO_WIDTH;
			pVtx[2].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = s_posTitleLogo.x + LOGO_WIDTH;
			pVtx[3].pos.y = s_posTitleLogo.y + LOGO_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuffLogo->Unlock();
			break;
		}
	}
}