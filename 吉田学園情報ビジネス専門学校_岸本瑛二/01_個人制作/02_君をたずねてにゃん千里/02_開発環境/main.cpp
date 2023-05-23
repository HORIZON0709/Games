//============================================================================================================
//
//�N�������˂Ăɂ��痢[main.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "score.h"
#include "fade.h"
#include "sound.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define CLASS_NAME		"WindowsClass"				//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"�N�������˂Ăɂ��痢"	//�E�C���h�E�̖��O(�L���v�V�����ɕ\��)

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3D9			s_pD3D = NULL;				//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECT3DDEVICE9	s_pD3DDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^
static MODE					s_mode = MODE_TITLE;		//���݂̃��[�h
static LPD3DXFONT			s_pFont = NULL;				//�t�H���g�ւ̃|�C���^
static int					s_nCountFPS = 0;			//FPS�J�E���^�[

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//============================================================================================================
//���C���֐�
//============================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };		//��ʃT�C�Y�̍\����
	HWND hWnd;											//�E�C���h�E�n���h��(���ʎq)
	MSG msg;											//���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;								//���ݎ���
	DWORD dwExecLastTime;								//�Ō�ɏ�����������
	DWORD dwFrameCount;									//�t���[���J�E���g
	DWORD dwFPSLastTime;								//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�v���V�[�W��
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),		//�t�@�C���̃A�C�R��
	};

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,							//�g���E�C���h�E�X�^�C��
							CLASS_NAME,					//�E�C���h�E�N���X�̖��O
							WINDOW_NAME,				//�E�C���h�E�̖��O
							WS_OVERLAPPEDWINDOW,		//�E�C���h�E�X�^�C��
							CW_USEDEFAULT,				//�E�C���h�E�̍���X���W
							CW_USEDEFAULT,				//�E�C���h�E�̍���Y���W
							(rect.right - rect.left),	//�E�C���h�E�̕�
							(rect.bottom - rect.top),	//�E�C���h�E�̍���
							NULL,						//�e�E�C���h�E�̃n���h��
							NULL,						//���j���[�n���h���܂��͎q�E�C���h�EID
							hInstance,					//�C���X�^���X�n���h��
							NULL);						//�E�C���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾(�ۑ�)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�C���h�E�̕\��
	ShowWindow(hWnd,nCmdShow);		//�E�C���h�E�\����Ԃ�ݒ�
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS���v��
				s_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPS���v������������ۑ�
				dwFrameCount = 0;					//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;		//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME,wcex.hInstance);

	return (int)msg.wParam;
}

//============================================================================================================
//�E�C���h�E�v���V�[�W��
//============================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	VERTEX_2D *pVtx = 0;											//���_���ւ̃|�C���^
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			//�E�C���h�E�̗̈�(�Z�`)

	switch (uMsg)
	{
	case WM_DESTROY:        //�E�C���h�E�j���̃��b�Z�[�W
							//WM_QOIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:        //�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:        //[ESC]�L�[�������ꂽ
							   //�E�C���h�E��j������ (WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);        //�K��̏�����Ԃ�
}

//============================================================================================================
//����������
//============================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&s_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��)
		if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp,
										&s_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_������CPU�ōs��)
			if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&s_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�^�[�g�̐ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�N�\���p�̃t�H���g�̐���
	D3DXCreateFont(s_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
					"Terminal", &s_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	////�W���C�p�b�h�̏���������
	//InitJoypad();

	//�T�E���h�̏���������
	InitSound(hWnd);

	//�t�F�[�h�̐ݒ�
	InitFade(s_mode);

	//�����L���O�̃��Z�b�g
	ResetRanking();

	return S_OK;
}

//============================================================================================================
//�I������
//============================================================================================================
void Uninit(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	////�W���C�p�b�h�̏I������
	//UninitJoypad();

	//�T�E���h�̏I������
	UninitSound();

	//�t�F�[�h�̏I������
	UninitFade();

	//�^�C�g����ʂ̏I������
	UninitTitle();

	//�`���[�g���A���̏I������
	UninitTutorial();

	//�Q�[����ʂ̏I������
	UninitGame();

	//���U���g��ʂ̏I������
	UninitResult();

	//�����L���O��ʂ̏I������
	UninitRanking();

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (s_pD3DDevice != NULL)
	{
		s_pD3DDevice->Release();
		s_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (s_pD3D != NULL)
	{
		s_pD3D->Release();
		s_pD3D = NULL;
	}
}

//============================================================================================================
//�X�V����
//============================================================================================================
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	////�W���C�p�b�h�̍X�V����
	//UpdateJoypad();

	switch (s_mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		UpdateTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		UpdateGame();
		break;

	case MODE_RESULT:		//���U���g���
		UpdateResult();
		break;

	case MODE_RANKING:		//�����L���O���
		UpdateRanking();
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();
}

//============================================================================================================
//�`�揈��
//============================================================================================================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	s_pD3DDevice->Clear(0, NULL,
						(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
						D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//�`�揈��
	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		switch (s_mode)
		{
		case MODE_TITLE:		//�^�C�g�����
			DrawTitle();
			break;

		case MODE_TUTORIAL:		//�`���[�g���A�����
			DrawTutorial();
			break;

		case MODE_GAME:			//�Q�[�����
			DrawGame();
			break;

		case MODE_RESULT:		//���U���g���
			DrawResult();
			break;

		case MODE_RANKING:		//�����L���O���
			DrawRanking();
			break;
		}

		//�t�F�[�h�̕`�揈��
		DrawFade();

#ifdef _DEBUG
		//FPS�̕\��
		DrawFPS();
#endif

		//�`��I��
		s_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//============================================================================================================
//���[�h�̐ݒ�
//============================================================================================================
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	switch (s_mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		UninitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		UninitGame();
		break;

	case MODE_RESULT:		//���U���g���
		UninitResult();
		break;

	case MODE_RANKING:		//�����L���O���
		UninitRanking();
		break;
	}

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		InitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		InitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		InitGame();
		break;

	case MODE_RESULT:		//���U���g���
		InitResult();
		break;

	case MODE_RANKING:		//�����L���O���
		InitRanking();

		//�����L���O�̐ݒ�
		SetRanking(GetScore());
		break;
	}

	s_mode = mode;		//���݂̉��(���[�h)��؂�ւ���
}

//============================================================================================================
//�f�o�C�X�̎擾
//============================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return s_pD3DDevice;
}

//============================================================================================================
//���[�h�̎擾
//============================================================================================================
MODE GetMode(void)
{
	return s_mode;
}

//============================================================================================================
//FPS�̕\��
//============================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	int pTutorial = GetTutorial();

	//������ɑ��
	wsprintf(&aStr[0], "FPS : %2d\n", s_nCountFPS);				//FPS
	wsprintf(&aStr[9], "GameMode : %2d\n", s_mode);				//�Q�[�����[�h
	wsprintf(&aStr[23], "TutorialTex : %2d\n", pTutorial);		//�`���[�g���A���̃e�N�X�`��

	//�e�L�X�g�̕`��
	s_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 255, 255));
}

//============================================================================================================
//�e�N�X�`�����W�̐ݒ�
//============================================================================================================
void SetTex(VERTEX_2D *pVtx, float fLeft, float fRight, float fUp, float fDown)
{
	pVtx[0].tex = D3DXVECTOR2(fLeft, fUp);
	pVtx[1].tex = D3DXVECTOR2(fRight, fUp);
	pVtx[2].tex = D3DXVECTOR2(fLeft, fDown);
	pVtx[3].tex = D3DXVECTOR2(fRight, fDown);
}