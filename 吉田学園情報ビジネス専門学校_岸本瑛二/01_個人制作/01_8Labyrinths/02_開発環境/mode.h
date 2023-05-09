//================================================
//
//3D�Q�[��(��)[mode.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _MODE_H_
#define _MODE_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//���[�h�N���X�̒�`
//***************************
class CMode
{/* ��{�N���X */
public: /* �񋓌^�̒�` */
	enum MODE	//���[�h�̎��
	{
		NONE = -1,
		TITLE = 0,	//�^�C�g��
		GAME,		//�Q�[��
		RESULT,		//���U���g
		MAX
	};

public: /* �ÓI�����o�֐� */
	/*
		����
		const MODE &mode ---> �������郂�[�h
	*/
	static CMode* Create(const MODE &mode);

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CMode() = delete;	//�f�t�H���g(�g��Ȃ�)
	CMode(MODE mode);
	virtual ~CMode();

public: /* �������z�֐� */
	virtual HRESULT Init() = 0;	//������
	virtual void Uninit() = 0;	//�I��
	virtual void Update() = 0;	//�X�V
	virtual void Draw() = 0;	//�`��

public: /* �����o�֐� */
	CMode* Set();					//���[�h�̐ݒ�
	MODE Get();						//���[�h�̎擾
	void Change(const MODE &mode);	//���[�h�̕ύX

private: /* �����o�ϐ� */
	MODE m_mode;		//���݂̃��[�h
	MODE m_modeNext;	//���̃��[�h
};
#endif