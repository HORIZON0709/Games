//================================================
//
//3D�Q�[��(��)[object3D.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//***************************
//�C���N���[�h
//***************************
#include "object.h"
#include "texture.h"

//***************************
//�萔�̒�`
//***************************

/* ���_�t�H�[�}�b�g */
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//***************************
//�I�u�W�F�N�g�N���X�̒�`
//***************************
class CObject3D : public CObject
{/* CObject�̔h���N���X */
public: /* �\���̂̒�` */
	struct VERTEX_3D
	{//���_�f�[�^
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 nor;	//�@���x�N�g�� 
		D3DCOLOR col;		//�F
		D3DXVECTOR2 tex;	//�e�N�X�`�����W
	};

public: /* �ÓI�����o�֐� */
	static CObject3D* Create();	//����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CObject3D();
	~CObject3D() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

public: /* Pos */
	/*
		�ʒu��ݒ�
		const D3DXVECTOR3 &pos ---> �C�ӂ̈ʒu
	*/
	void SetPos(const D3DXVECTOR3 &pos);

	//�ʒu���擾
	D3DXVECTOR3 GetPos();

public: /* Rot */
	/*
		������ݒ�
		const D3DXVECTOR3 &rot ---> �C�ӂ̌���
	*/
	void SetRot(const D3DXVECTOR3 &rot);

	//�������擾
	D3DXVECTOR3 GetRot();

public: /* Size */
	/*
		�T�C�Y��ݒ�
		const D3DXVECTOR3 &size ---> �C�ӂ̃T�C�Y
	*/
	void SetSize(const D3DXVECTOR3 &size);

	//�T�C�Y���擾
	D3DXVECTOR3 GetSize();

public: /* Move */
	/*
		�ړ��ʂ�ݒ�
		const D3DXVECTOR3 &move ---> �C�ӂ̈ړ���
	*/
	void SetMove(const D3DXVECTOR3 &move);

	//�ړ��ʂ��擾
	D3DXVECTOR3 GetMove();

public: /* Texture */
	/*
		�e�N�X�`���̐ݒ�
		CTexture::TEXTURE texture ---> �C�ӂ̃e�N�X�`��
	*/
	void SetTexture(CTexture::TEXTURE texture);

	/*
		�e�N�X�`�����W�̐ݒ�(�e�N�X�`���A�j���[�V�����p)
		int nDivNumU ---> �e�N�X�`���̕�����( U )
		int nDivNumV ---> �e�N�X�`���̕�����( V )
		int nPtnAnim ---> �A�j���[�V�����p�^�[����
		�P�̉摜(�w�i�Ȃ�)�̏ꍇ�́u1,1,0�v
	*/
	void SetTexUV(int nDivNumU, int nDivNumV, int nPtnAnim);

	/*
		�e�N�X�`�����W�̐ݒ�(�X�N���[���p)
		bool bDirection ---> �X�N���[������[ false : X�� / true : Y�� ]
		float fMove ---> �ړ���
	*/
	void SetTexUV(bool bDirection, float fMove);

public: /* Color */
	/*
		�F�̐ݒ�
		const D3DXCOLOR &col ---> �C�ӂ̐F
	*/
	void SetCol(const D3DXCOLOR &col);

public: /* Collision */
	/*
		�����蔻��
		OBJ_TYPE myType ---> ���g�̃^�C�v
		OBJ_TYPE targetType ---> ��������Ώۂ̃^�C�v
	*/
	bool Collision(OBJ_TYPE myType, OBJ_TYPE targetType);

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX	m_mtxWorld;				//���[���h�}�g���b�N�X
	CTexture::TEXTURE m_texture;		//�e�N�X�`���̗񋓌^

	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR3 m_size;	//�T�C�Y
};

#endif