//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// Author : KIshimoto Eiji
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "application.h"
#include "renderer.h"
#include "texture.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
const char* CTexture::s_FileName[] =
{// �e�N�X�`���̃p�X
	/* ���w�i */
	"data/TEXTURE/Bg_Title_Tentative.png",	//���w�i�F�^�C�g��
	"data/TEXTURE/Bg_Result_Tentative.png",	//���w�i�F���U���g

	/* Number_Single */
	"data/TEXTURE/Number_Single/Number_Single_1.png",	//�i���o�[�摜(�P��)[1]
	"data/TEXTURE/Number_Single/Number_Single_2.png",	//�i���o�[�摜(�P��)[2]
	"data/TEXTURE/Number_Single/Number_Single_3.png",	//�i���o�[�摜(�P��)[3]
	"data/TEXTURE/Number_Single/Number_Single_4.png",	//�i���o�[�摜(�P��)[4]
	"data/TEXTURE/Number_Single/Number_Single_5.png",	//�i���o�[�摜(�P��)[5]
	"data/TEXTURE/Number_Single/Number_Single_6.png",	//�i���o�[�摜(�P��)[6]
	"data/TEXTURE/Number_Single/Number_Single_7.png",	//�i���o�[�摜(�P��)[7]
	"data/TEXTURE/Number_Single/Number_Single_8.png",	//�i���o�[�摜(�P��)[8]
	"data/TEXTURE/Number_Single/Cross.png",				//�o�c

	/* Number */
	"data/TEXTURE/Number00.png",	//�i���o�[�摜

	/* �Q�[�����p�f�� */
	"data/TEXTURE/WallAndFloar.jpg",	//�ǂƏ��p
	"data/TEXTURE/GameUI_Purpose.png",	//�ړI
	"data/TEXTURE/GameUI_Coin.png",		//�l���R�C����

	/* �w�i */
	"data/TEXTURE/Bg_Title.jpg",	//�^�C�g���w�i
	"data/TEXTURE/Bg_Result.jpg",	//���U���g�w�i


	/* �^�C�g�����S */
	"data/TEXTURE/TitleLogo.png",	//�^�C�g�����S

	/* PressEnter */
	"data/TEXTURE/PressEnter.png",	//PressEnter

	/* �Q�[���N���A */
	"data/TEXTURE/GameClear.png",	//�Q�[���N���A
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::MAX, "aho");

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// �S�Ă̓ǂݍ���
//--------------------------------------------------
void CTexture::LoadAll()
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	
	for (int i = 0; i < MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̓ǂݍ��݂�����Ă���
			continue;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// �e�N�X�`���̓ǂݍ��݂�����Ă���
		return;
	}

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// �S�Ẳ��
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// �e�N�X�`���̉��
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// ���
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < MAX);

	if (s_pTexture[inTexture] != NULL)
	{// �e�N�X�`���̉��
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == NONE)
	{// �e�N�X�`�����g�p���Ȃ�
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < MAX);

	// �ǂݍ���
	Load(inTexture);

	return s_pTexture[inTexture];
}
