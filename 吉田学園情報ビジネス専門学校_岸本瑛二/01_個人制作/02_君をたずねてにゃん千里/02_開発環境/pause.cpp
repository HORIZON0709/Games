//============================================================================================================
//
//�N�������˂Ăɂ��痢[pause.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "fade.h"

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9	   s_apTexture[NUM_PAUSE] = { NULL, NULL, NULL, NULL };		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPause		  = NULL;							//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseMenu	  = NULL;							//���_�o�b�t�@�ւ̃|�C���^
static PAUSE_MENU			   s_pauseMenu;												//�|�[�Y���j���[
static PauseMenu			   s_aPauseMenu[PAUSE_MENU_MAX];							//�|�[�Y���j���[�̏��
static D3DXVECTOR3			   s_posPause;												//�|�[�Y�̈ʒu

//============================================================================================================
//�|�[�Y�̏���������
//============================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^
	int nCntPause;								//�J�E���g

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�Q�[���ɖ߂�_Copy.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/���Ȃ���_Copy.png",
								&s_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�^�C�g����_Copy.png",
								&s_apTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/pause100.png",
								&s_apTexture[3]);

	//���̏�����
	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		s_aPauseMenu[nCntPause].pos = D3DXVECTOR3(640.0f, 260.0f + (nCntPause * PAUSEMENU_SPACE), 0.0f);
		s_aPauseMenu[nCntPause].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}

	s_pauseMenu = PAUSE_MENU_CONTINUE;					//�|�[�Y���j���[��������
	s_posPause	= D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//�|�[�Y�̈ʒu��������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPause,
								NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffPauseMenu,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//�|�[�Y�̔������|���S��
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		else if (nCntPause == 1)
		{//�|�[�Y
			//���_���W�̐ݒ�
			pVtx[0].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[0].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[1].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[2].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[3].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[3].pos.z = 0.0f;
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntPause == 0)
		{//�|�[�Y�̔������|���S��
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		}
		else if (nCntPause == 1)
		{//�|�[�Y
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.3f, 0.5f, 0.0f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPause->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{//�|�[�Y���j���[�̕�
		//���_���W�̐ݒ�
		pVtx[0].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[0].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[1].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[2].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[3].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
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
		SetTex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//�|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPauseMenu->Unlock();
}

//============================================================================================================
//�|�[�Y�̏I������
//============================================================================================================
void UninitPause(void)
{
	int nCntPause;		//�J�E���g

	//�T�E���h�̒�~
	StopSound();

	//�e�N�X�`���̔j��
	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (s_apTexture[nCntPause] != NULL)
		{
			s_apTexture[nCntPause]->Release();
			s_apTexture[nCntPause] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffPause != NULL)
	{
		s_pVtxBuffPause->Release();
		s_pVtxBuffPause = NULL;
	}

	if (s_pVtxBuffPauseMenu != NULL)
	{
		s_pVtxBuffPauseMenu->Release();
		s_pVtxBuffPauseMenu = NULL;
	}
}

//============================================================================================================
//�|�[�Y�̍X�V����
//============================================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

		if (GetKeyboardTrigger(DIK_W))
		{//W�L�[�������ꂽ
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_SELECT);

			//���ɂ���
			s_aPauseMenu[s_pauseMenu].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += (s_pauseMenu * 4);		//�|�C���^���Y���̈ʒu�܂Ői�߂�

			 //���_�J���[�̐ݒ�
			pVtx[0].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[1].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[2].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[3].col = s_aPauseMenu[s_pauseMenu].col;

			 //1���I��
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu - 1) + 3) % 3);
		}

		if (GetKeyboardTrigger(DIK_S))
		{//S�L�[�������ꂽ
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_SELECT);

			//���ɂ���
			s_aPauseMenu[s_pauseMenu].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += (s_pauseMenu * 4);		//�|�C���^���Y���̈ʒu�܂Ői�߂�

			//���_�J���[�̐ݒ�
			pVtx[0].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[1].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[2].col = s_aPauseMenu[s_pauseMenu].col;
			pVtx[3].col = s_aPauseMenu[s_pauseMenu].col;

			 //1����I��
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu + 1) + 3) % 3);
		}

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffPauseMenu->Unlock();

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (s_pauseMenu * 4);		//�|�C���^���Y���̈ʒu�܂Ői�߂�

		//�I�𕔕��̐F��ς���
		s_aPauseMenu[s_pauseMenu].col = D3DXCOLOR(1.0f, 0.4f, 0.5f, 1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = s_aPauseMenu[s_pauseMenu].col;
		pVtx[1].col = s_aPauseMenu[s_pauseMenu].col;
		pVtx[2].col = s_aPauseMenu[s_pauseMenu].col;
		pVtx[3].col = s_aPauseMenu[s_pauseMenu].col;

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffPauseMenu->Unlock();

		if (GetKeyboardTrigger(DIK_RETURN))
		{//Enter�L�[�������ꂽ
			switch (s_pauseMenu)
			{
			case PAUSE_MENU_CONTINUE:	//�Q�[���ɖ߂�
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_CANCEL);

				//�|�[�Y����
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:		//�Q�[������蒼��
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//�Q�[����ʂɈڍs
				SetFade(MODE_GAME);
				break;

			case PAUSE_MENU_QUIT:		//�^�C�g����ʂɖ߂�
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//�^�C�g����ʂɈڍs
				SetFade(MODE_TITLE);
				break;
			}
		}
	}
}

//============================================================================================================
//�|�[�Y�̕`�揈��
//============================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	int nCntPause;								//�J�E���g
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//�|�[�Y�̔������|���S��
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}
		else if (nCntPause == 1)
		{//�|�[�Y
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_apTexture[3]);
		}

		//�|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCntPause * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPauseMenu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_apTexture[nCntPause]);

		//�|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCntPause * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}
}