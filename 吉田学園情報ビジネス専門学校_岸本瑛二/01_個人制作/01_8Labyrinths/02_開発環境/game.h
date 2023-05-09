//================================================
//
//3D�Q�[��(��)[game.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _GAME_H_
#define _GAME_H_

//***************************
//�C���N���[�h
//***************************
#include "mode.h"

#include <d3dx9.h>

//***************************
//�O���錾
//***************************
class CCamera;
class CLight;
class CPlayer;
class CItem;
class CStage;
class CPanel;
class CScore;
class CObject2D;

//***************************
//�Q�[���N���X�̒�`
//***************************
class CGame : public CMode
{/* CMode�̔h���N���X */
private: /* �񋓌^�̒�` */
	enum UI_TYPE	//UI�^�C�v
	{
		NONE = -1,
		PURPOSE = 0,	//�ړI
		COIN,			//�l���R�C����
		MAX
	};

private: /* �萔�̒�` */
	static const int INTERVAL_STRAIGHT;			//�����G�̐����Ԋu
	static const int FADE_INTERVAL_GAMEOVER;	//�t�F�[�h�܂ł̊Ԋu(�Q�[���I�[�o�[��)
	static const int FADE_INTERVAL_GAMECLEAR;	//�t�F�[�h�܂ł̊Ԋu(�Q�[���N���A��)

	static const float UI_PURPOSE_WIDTH;	//UI(�ړI)�̕�
	static const float UI_PURPOSE_HEIGHT;	//UI(�ړI)�̍���
	static const float UI_COIN_WIDTH;		//UI(�l���R�C����)�̕�
	static const float UI_COIN_HEIGHT;		//UI(�l���R�C����)�̍���

public: /* �ÓI�����o�֐� */
	static CCamera* GetCamera();	//�J�����̏����擾
	static CLight* GetLight();		//���C�g�̏����擾
	static CPlayer* GetPlayer();	//�v���C���[�̏����擾
	static CStage* GetStage();		//�X�e�[�W�̏����擾
	static CPanel* GetPanel();		//�p�l���̏����擾
	static CScore* GetScore();		//�X�R�A�̏����擾

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CGame();
	~CGame() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

private: /* �ÓI�����o�ϐ� */
	static CCamera* m_pCamera;		//�J����
	static CLight* m_pLight;		//���C�g
	static CPlayer* m_pPlayer;		//�v���C���[
	static CStage* m_pStage;		//�X�e�[�W
	static CPanel* m_pPanel;		//�p�l��
	static CScore* m_pScore;		//�X�R�A

	static CObject2D* m_apUI[UI_TYPE::MAX];	//UI

private: /* �����o�֐� */
	void SwitchWireFrame();	//���C���[�t���[���̐؂�ւ�

private: /* �����o�ϐ� */
	int m_nCntIntervalFade;	//�t�F�[�h����܂ł̊Ԋu�J�E���g�p

	bool m_bFadeOut;	//�Ó]�������ǂ���
	bool m_bWireFrame;	//���C���[�t���[���\���؂�ւ�
};
#endif