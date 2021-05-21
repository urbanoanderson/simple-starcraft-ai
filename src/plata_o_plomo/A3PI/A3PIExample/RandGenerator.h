/*
	Classe para gerar números aleatórios que seja
	thread safe.
*/

#ifndef RANDGENERATOR_H_INCLUDED
#define RANDGENERATOR_H_INCLUDED

#include "Includes.h"
#include "Mutex.h"

class RandGenerator
{
	private:
		bool seeded;
		Mutex* mutex_rand;

	public:
		RandGenerator();
		int GetNumber();
		int GetNumber(int max);
		int GetSignal();
};

#endif