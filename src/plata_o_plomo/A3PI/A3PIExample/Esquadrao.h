#ifndef ESQUADRAO_H_INCLUDED
#define ESQUADRAO_H_INCLUDED

#include "Includes.h"
#include "RandGenerator.h"
#include "Util.h"
#include "AgenteMilitar.h"

class AgenteMilitar;

enum { ESQUADRAO_INCOMPLETO, ESQUADRAO_DEFESA, ESQUADRAO_ATAQUE };

class Esquadrao
{
	private:
		RandGenerator* randGen;
		AgenteMilitar* agenteMilitar;
		
		std::list<Unidade*> lista_unidades; //lista de unidades do esquadrao
		int estado;
		Unidade* alvo;
		
		int raio_atuacao;
		int numero_cadastrados;
		int numero_desejado;

		bool atacando;

	public:
		Esquadrao(RandGenerator* randGen, AgenteMilitar* agenteMilitar);
		Esquadrao(RandGenerator* randGen, AgenteMilitar* agenteMilitar, int numero_unidades);
		void Inicializar(int numero_unidades);
		void SetEstado(int estado);
		void CadastrarUnidade(Unidade* unidade);
		bool EsquadraoCheio(); 	//Se o esquadrao cadastrou o numero de unidades desejado
		bool EsquadraoTreinado(); //Se o esquadrao está com todas as unidades treinadas
		bool EsquadraoDestruido(); //se o esquadrao está sem unidades
		BWAPI::Position GetPosition();
		void ProcurarInimigos();
		std::set<Unidade*> InimigosProximos(std::set<Unidade*> lista_inimigos);
		bool EhUnidadeMilitar(Unidade* u);
		bool EhUnidadeCivil(Unidade* u);
		bool EhUnidadeCommandCenter(Unidade* u);
		void Atacar();
		void Andar(BWAPI::Position pos);
		Unidade* EscolherAlvo(std::set<Unidade*> inimigos_proximos);
		bool NovoAlvoEhMelhor(Unidade* alvo, Unidade* novo_alvo);
		void Update(std::set<Unidade*> lista_inimigos, BWAPI::Position commandCenterPos, BWAPI::Position enemyCommandCenterPos);
};

#endif