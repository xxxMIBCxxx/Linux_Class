#pragma once
//*****************************************************************************
// Event�N���X
//*****************************************************************************
#include "Type.h"


typedef enum
{
	CEVENT_RET_SUCCESS = 0x00000000,				// ����
	CEVENT_RET_RECIVE_EVENT = 0x01000000,			// �C�x���g�҂��ɂăC�x���g����M
	CEVENT_RET_WAIT_TIMEOUT = 0x09999999,			// �C�x���g�҂��ɂă^�C���A�E�g������
	CEVENT_RET_ERROR_EVENT_FD = 0xE0000001,			// �C�x���g�t�@�C���f�B�X�N���v�^���擾�ł��Ȃ�����
	CEVENT_RET_ERROR_EVENT_SET = 0xE0000002,		// �C�x���g�ݒ莸�s
	CEVENT_RET_ERROR_EVENT_RESET = 0xE0000003,		// �C�x���g���Z�b�g���s
	CEVENT_RET_ERROR_EVENT_WAIT = 0xE0000004,		// �C�x���g�҂����s
	CEVENT_RET_ERROR_SYSTEM = 0xE9999999,			// �V�X�e���ُ�
} CEVENT_RET_ENUM;


class CEvent
{
private:
	int					m_efd;				// �C�x���g�t�@�C���f�B�X�N���v�^
	int					m_errno;			// �G���[�ԍ�

public:
	CEvent();
	~CEvent();
	CEVENT_RET_ENUM Init();
	int GetEdf();
	int GetErrorNo();
	CEVENT_RET_ENUM SetEvent();
	CEVENT_RET_ENUM ResetEvent();
	CEVENT_RET_ENUM Wait(DWORD dwTimeout = 0);
};

