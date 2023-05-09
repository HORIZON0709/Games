//================================================
//
//�f�o�b�O�\��[debug_proc.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "debug_proc.h"
#include "application.h"
#include "renderer.h"

#include <assert.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>

//***************************
//�ÓI�����o�ϐ�
//***************************
LPD3DXFONT CDebugProc::m_pFont = nullptr;	//�t�H���g���
char CDebugProc::m_str[MAX_TEXT] = {};		//�o�^������

//================================================
//������̓o�^
//================================================
void CDebugProc::Print(const char *pFormat, ...)
{
#ifdef _DEBUG
	// �ϐ�
	char str[MAX_TEXT] = {};

	// ���X�g�̍쐬
	va_list args;
	va_start(args, pFormat);
	vsprintf(&str[0], pFormat, args);
	va_end(args);

	// ������̘A��
	strcat(&m_str[0], &str[0]);
#endif // _DEBUG
}

//================================================
//�`��
//================================================
void CDebugProc::Draw()
{
#ifdef _DEBUG
	RECT rect = { 0, 0, CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT };

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	m_str[0] = '\0';
#endif // _DEBUG
}

//================================================
//�R���X�g���N�^
//================================================
CDebugProc::CDebugProc()
{
#ifdef _DEBUG
#endif // _DEBUG
}

//================================================
//�f�X�g���N�^
//================================================
CDebugProc::~CDebugProc()
{
#ifdef _DEBUG
#endif // _DEBUG
}

//================================================
//������
//================================================
void CDebugProc::Init()
{
#ifdef _DEBUG
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif // _DEBUG
}

//================================================
//�I��
//================================================
void CDebugProc::Uninit()
{
#ifdef _DEBUG
	if (m_pFont != nullptr)
	{// �t�H���g�̔j��
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG
}
