//================================================
//
//3D�Q�[��(��)[object2D.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//***************************
//�C���N���[�h
//***************************
#include "object.h"
#include "texture.h"

//***************************
//�萔�̒�`
//***************************
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//���_�t�H�[�}�b�g

//***************************
//�I�u�W�F�N�g�N���X�̒�`
//***************************
class CObject2D : public CObject
{/* CObject�̔h���N���X */
public: /* �\���̂̒�` */
	struct VERTEX_2D
	{//���_�f�[�^
		D3DXVECTOR3 pos;	//�ʒu
		float rhw;			//�@��
		D3DCOLOR col;		//�F
		D3DXVECTOR2 tex;	//�e�N�X�`�����W
	};

public: /* �ÓI�����o�֐� */
	static CObject2D* Create();	//����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CObject2D();
	//explicit CObject2D(const int &nPriority = 3);
	~CObject2D() override;

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

	/*
		�ʒu���擾
	*/
	D3DXVECTOR3 GetPos();

public: /* Size */
	/*
		�T�C�Y��ݒ�
		const D3DXVECTOR2 &size ---> �C�ӂ̃T�C�Y
	*/
	void SetSize(const D3DXVECTOR2 &size);

	/*
		�T�C�Y���擾
	*/
	D3DXVECTOR2 GetSize();

public: /* Move */
	/*
		�ړ��ʂ�ݒ�
		const D3DXVECTOR3 &move ---> �C�ӂ̈ړ���
	*/
	void SetMove(const D3DXVECTOR3 &move);

	/*
		�ړ��ʂ��擾
	*/
	D3DXVECTOR3 GetMove();

public: /* Color */
	/*
		�F��ݒ�
		const D3DXCOLOR &col ---> �C�ӂ̐F
	*/
	void SetCol(const D3DXCOLOR &col);

	/*
		�F���擾
	*/
	D3DXCOLOR GetCol();

public: /* Texture */
	/*
		�e�N�X�`���̐ݒ�
		CTexture::TEXTURE texture ---> �C�ӂ̃e�N�X�`��
	*/
	void SetTexture(CTexture::TEXTURE texture);

	/*
		�e�N�X�`�����W�̐ݒ�(�A�j���[�V�����Ή�)
		const int &nDivNum ---> �e�N�X�`���̕�����
		const int &nPtnAnim ---> �A�j���[�V�����p�^�[����
		(�P�̉摜�̏ꍇ�́u1,0�v�𑗂�B)
	*/
	void SetTexUV(const int nDivNum,const int nPtnAnim);

	CTexture::TEXTURE GetTexture();	//�e�N�X�`���̎擾

public: /* IsDraw */
	/*
		//�`�悷�邩�ǂ����̐ݒ�
		bool bDraw ---> �`�悷�邩�ǂ���
		false : �`�悵�Ȃ�
		true  : �`�悷��
	*/
	void SetIsDraw(bool bDraw);

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	CTexture::TEXTURE m_texture;		//�e�N�X�`���̗񋓌^

	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR2 m_size;	//�T�C�Y
	D3DXCOLOR m_col;	//�F

	bool m_bDraw;	//�`�悷�邩�ǂ���
};

#endif