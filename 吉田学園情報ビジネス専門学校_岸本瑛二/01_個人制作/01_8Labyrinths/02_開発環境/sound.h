//================================================
//
//3D�Q�[��(��)[sound.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>
#include <xaudio2.h>

//***************************
//�T�E���h�N���X��`
//***************************
class CSound
{/* ��{�N���X */
public: /* �񋓌^�̒�` */
	enum ELabel
	{
		LABEL_NONE = -1,
		LABEL_BGM_Title = 0,	//�^�C�g��
		LABEL_BGM_Game,			//�Q�[��
		LABEL_BGM_Result,		//���U���g
		LABEL_SE_Enter,			//����
		LABEL_SE_Select,		//�I��
		LABEL_MAX,
	};

public: /* �\���̂̒�` */
	struct SParam
	{
		char *pFileName;	//�t�@�C����
		int loop;			//���[�v
	};

public: /* �萔�̒�` */
	static const SParam PARAM[];	//�p�����[�^�̏��

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CSound();
	~CSound();

public: /* �����o�֐� */
	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT Play(ELabel sound);
	void Stop(ELabel label);
	void Stop();

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	//�`�����N�̃`�F�b�N
	HRESULT LoadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	//�`�����N�f�[�^�̓ǂݍ���

private: /* �����o�ϐ� */
	IXAudio2* m_pXAudio2;							//XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice;		//�}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_pSourceVoice[LABEL_MAX];	//�\�[�X�{�C�X
	BYTE* m_pDataAudio[LABEL_MAX];					//�I�[�f�B�I�f�[�^
	DWORD m_sizeAudio[LABEL_MAX];					//�I�[�f�B�I�f�[�^�T�C�Y
};

#endif