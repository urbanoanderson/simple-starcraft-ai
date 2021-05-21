/*
	Entidade responsável por coordenar a construção de edifícios.
	Pega agentes SCV emprestados do AgenteColeta para serem 
	construtores durante um projeto e os devolve ao final.
	Recebe projetos de construção do AgenteCentral.
*/

#ifndef AGENTECONSTRUTOR_H_INCLUDED
#define AGENTECONSTRUTOR_H_INCLUDED

#include "Includes.h"
#include "Mutex.h"
#include "AgenteGenerico.h"
#include "AgenteCentral.h"
#include "AgenteColeta.h"
#include "AgenteMilitar.h"

class AgenteCentral;
class AgenteColeta;
class AgenteMilitar;

class AgenteConstrutor : public AgenteGenerico
{
	private:
		AgenteCentral* agenteCentral;
		AgenteColeta* agenteColeta;
		AgenteMilitar* agenteMilitar;

		std::list<Unidade*> lista_construtores;
		std::list<Unidade*> lista_casas;

		Mutex* mutex_construtores; //controla a lista de construtores
		Mutex* mutex_casas; //controla a lista de construtores

	public:
		AgenteConstrutor(RandGenerator* randGen);
		void CadastrarEntidades(AgenteCentral* agenteCentral, AgenteColeta* agenteColeta, AgenteMilitar* agenteMilitar);
		void CadastrarCasa(Unidade* casa);
		int GetNumConstrutores();
		Unidade* PegarSCVEmprestado();
		void DevolverSCV(Unidade* unidadeSCV);
		void CriarConstrucao(BWAPI::UnitType tipo_construcao);
		void Update();
};

#endif