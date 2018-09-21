#pragma once

#include <Unidade.h>
#include <AgentePrincipal.h>
#include <vector>

class MeuAgentePrincipal:public AgentePrincipal{
public:
	
	virtual void InicioDePartida();

	virtual void onEnd(bool isWinner);

	virtual void UnidadeCriada(Unidade* unidade);

};
