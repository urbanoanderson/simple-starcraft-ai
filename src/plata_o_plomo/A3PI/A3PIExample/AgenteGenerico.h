/*
	Modelo de Agente do qual todos os outros herdam.
	Possui a função Update que contém a lógica do agente e é
	chamada durante a execução da thread do agente.
	Essa função é modificada para cada classe filha.
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