#ifndef MEUAGENTEPRINCIPAL_H_INCLUDED
#define MEUAGENTEPRINCIPAL_H_INCLUDED

#include "Includes.h"
#include "RandGenerator.h"
#include "AgenteGenerico.h"
#include "AgenteCentral.h"
#include "AgenteColeta.h"
#include "AgenteConstrutor.h"
#include "AgenteMilitar.h"

class MeuAgentePrincipal:public AgentePrincipal
{
	public:
		virtual void InicioDePartida();
		virtual void onEnd(bool isWinner);
		virtual void UnidadeCriada(Unidade* unidade);
};

#endif
