/*
	Classe que encapsula o mutex para uso nas diversas classes do projeto
*/

#ifndef MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED

#include "Includes.h"

class Mutex
{
	private:
		CRITICAL_SECTION mutex_handler;

	public:
		Mutex();
		~Mutex();
		void Lock();
		void Unlock();
};

#endif