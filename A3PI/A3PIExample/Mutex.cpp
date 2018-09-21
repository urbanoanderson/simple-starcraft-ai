#include "Mutex.h"

Mutex::Mutex()
{
	InitializeCriticalSection(&mutex_handler);
}

Mutex::~Mutex()
{
	DeleteCriticalSection(&mutex_handler);
}

void Mutex::Lock()
{
	EnterCriticalSection(&mutex_handler);
}

void Mutex::Unlock()
{
	LeaveCriticalSection(&mutex_handler);
}