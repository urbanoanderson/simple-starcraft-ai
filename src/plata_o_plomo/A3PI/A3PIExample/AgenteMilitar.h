/*
	Entidade responsável por coordenar ações militares.
	Controla unidades militares para ataque ou defesa.
*/

#ifndef AGENTEMILITAR_H_INCLUDED
#define AGENTEMILITAR_H_INCLUDED

#include "Includes.h"
#include "Mutex.h"
#include "Esquadrao.h"
#include "AgenteGenerico.h"
#include "AgenteCentral.h"
#include "AgenteColeta.h"
#include "AgenteConstrutor.h"

class AgenteCentral;
class AgenteColeta;
class AgenteConstrutor;
class Esquadrao;

class AgenteMilitar : public AgenteGenerico
{
	private:
		AgenteCentral* agenteCentral;
		AgenteColeta* agenteColeta;
		AgenteConstrutor* agenteConstrutor;

		std::list<Unidade*> lista_marines;
		std::list<Unidade*> lista_barracas;
		std::set<Unidade*> lista_inimigos;
		std::list<Esquadrao*> esquadroes_novos;
		std::list<Esquadrao*> esquadroes_defesa;
		std::list<Esquadrao*> esquadroes_ataque;
		
		Mutex* mutex_marines;		//Controla a lista de marines
		Mutex* mutex_barracas;		//Controla a lista de barracas
		Mutex* mutex_inimigos;		//Controla a lista de inimigos
		Mutex* mutex_esquadroes;	//Controla a lista de esquadroes

		Unidade* enemyCommandCenter;
		BWAPI::Position enemyCommandCenterPos;

	public:
		AgenteMilitar(RandGenerator* randGen);
		void CadastrarEntidades(AgenteCentral* agenteCentral, AgenteColeta* agenteColeta, AgenteConstrutor* agenteConstrutor);
		void CadastrarInimigo(Unidade* inimigo);
		void CadastrarBarraca(Unidade* barraca);
		void CadastrarMarine(Unidade* unidadeMarine);
		int GetNumMarines();
		int GetNumBarracas();
		int GetNumEsquadroes();
		BWAPI::Position AgenteMilitar::GetEnemyCommandCenterPosition();
		void CriarMarine();
		void CriarEsquadrao();
		void AlocarNovoEsquadrao(Esquadrao* novo);
		void Update();
};

#endif