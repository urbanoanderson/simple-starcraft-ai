/*
	Entidade responsável pela tomada de decisões administrativas.
	Dá ordem de construção de unidades e prédios a outras entidades.
*/

#ifndef AGENTECENTRAL_H_INCLUDED
#define AGENTECENTRAL_H_INCLUDED

#include "Includes.h"
#include "AgenteGenerico.h"
#include "AgenteColeta.h"
#include "AgenteConstrutor.h"
#include "AgenteMilitar.h"

class AgenteColeta;
class AgenteConstrutor;
class AgenteMilitar;

enum
{ 
	DECISAO_CENTRAL_FAZ_NADA,
	DECISAO_CENTRAL_COMMAND_CENTER,
	DECISAO_CENTRAL_CASA,
	DECISAO_CENTRAL_SCV,
	DECISAO_CENTRAL_BARRACA,
	DECISAO_CENTRAL_MARINE
};

class AgenteCentral : public AgenteGenerico
{
	private:
		AgenteColeta* agenteColeta;
		AgenteConstrutor* agenteConstrutor;
		AgenteMilitar* agenteMilitar;
		bool firstUpdate;

	public:
		AgenteCentral(RandGenerator* randGen);
		void CadastrarEntidades(AgenteColeta* agenteColeta, AgenteConstrutor* agenteConstrutor, AgenteMilitar* agenteMilitar);
		int getSupply();
		bool PrecisaCentroDeComando();
		bool PrecisaCasa();
		bool PrecisaSCV();
		bool PrecisaBarraca();
		bool PrecisaMarine();
		int TomarDecisao();
		void Update();
};

#endif