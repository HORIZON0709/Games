//================================================
//
//3D�Q�[��(��)[input.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//***************************
//�C���N���[�h
//***************************
#include "inputKeyboard.h"

//***************************
//�C���v�b�g�N���X�̒�`
//***************************
class CInput
{/* ��{�N���X */
public: /* �񋓌^�̒�` */
	enum STANDARD_KEY
	{/* �ʏ푀��Ŏg�p����L�[ */
		UP = 0,		//��
		DOWN,		//��
		LEFT,		//��
		RIGHT,		//�E
		DECISION,	//����
		SHOT		//����
	};

public: /* �ÓI�����o�֐� */
	static CInput* Create();	//����
	static CInput* GetKey();	//�擾

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CInput();
	~CInput();

public: /* �����o�֐� */
	HRESULT Init(HINSTANCE hInstance,HWND hWnd);	//������
	void Uninit();									//�I��
	void Update();									//�X�V

	bool Press(STANDARD_KEY key);	//�v���X
	bool Release(STANDARD_KEY key);	//�����[�X
	bool Trigger(STANDARD_KEY key);	//�g���K�[

private: /* �ÓI�����o�ϐ� */
	static CInput* m_pInput;

private: /* �����o�ϐ� */
	CInputKeyboard* m_pInputKeyboard;	//�L�[�{�[�h�̃|�C���^
};

#endif