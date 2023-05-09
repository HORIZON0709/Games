//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// Author : KIshimoto Eiji
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>

//==================================================
// ��`
//==================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{
		NONE = -1,	//�g�p���Ȃ�

		/* ���w�i */
		Bg_Title_Tentative = 0,	//���w�i�F�^�C�g��
		Bg_Result_Tentative,	//���w�i�F���U���g

		/* Number_Single */
		Number_Single_1,	//�i���o�[�摜(�P��)[1]
		Number_Single_2,	//�i���o�[�摜(�P��)[2]
		Number_Single_3,	//�i���o�[�摜(�P��)[3]
		Number_Single_4,	//�i���o�[�摜(�P��)[4]
		Number_Single_5,	//�i���o�[�摜(�P��)[5]
		Number_Single_6,	//�i���o�[�摜(�P��)[6]
		Number_Single_7,	//�i���o�[�摜(�P��)[7]
		Number_Single_8,	//�i���o�[�摜(�P��)[8]
		Cross,				//�o�c

		/* Number */
		Number00,	//�i���o�[�摜

		/* �Q�[�����p�f�� */
		WallAndFloar,	//�ǂƏ��p
		GameUI_Purpose,	//�ړI
		GameUI_Coin,	//�l���R�C����

		/* �w�i */
		Bg_Title,	//�^�C�g���w�i
		Bg_Result,	//���U���g�w�i

		/* �^�C�g�����S */
		TitleLogo,	//�^�C�g�����S

		/* PressEnter */
		PressEnter,	//PressEnter

		/* �Q�[���N���A */
		GameClear,	//�Q�[���N���A

		MAX
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	void Load(TEXTURE inTexture);						// �w��̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
