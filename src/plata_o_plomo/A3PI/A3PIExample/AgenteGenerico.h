/*
	Modelo de Agente do qual todos os outros herdam.
	Possui a fun��o Update que cont�m a l�gica do agente e �
	chamada durante a execu��o da thread do agente.
	Essa fun��o � modificada para cada classe filha.
*/

#ifndef AGENTEGENERICO_H_INCLUDED
#define AGENTEGENERICO_H_INCLUDED

#include "Includes.h"
#include "RandGenerator.h"

class AgenteGenerico
{
	protected:
		RandGenerator* randGen;
		Unidade *u;

	public:
		AgenteGenerico();
		AgenteGenerico(RandGenerator* randGen);
		virtual void Update();
};

#endif