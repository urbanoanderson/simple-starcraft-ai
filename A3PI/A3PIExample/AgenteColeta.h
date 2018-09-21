/*
	Entidade responsável por coordenar a coleta de recursos.
	Controla agentes SVC para serem coletores ou exploradores.
	Cria novos operários no centro de comando.
	Empresta operários para o AgenteConstrutor.
*/

#ifndef AGENTECOLETA_H_INCLUDED
#define AGENTECOLETA_H_INCLUDED

#include "Includes.h"
#include "Mutex.h"
#include "Util.h"
#include "AgenteGenerico.h"
#include "AgenteCentral.h"
#include "AgenteConstrutor.h"
#include "AgenteMilitar.h"

class AgenteCentral;
class AgenteConstrutor;
class AgenteMilitar;

typedef std::pair<int, Unidade*> PairDistUnit; //a unidade e a distancia dela até o centro de comando
typedef std::set< std::pair<int, Unidade*> > SetPairDistUnit;

class AgenteColeta : public AgenteGenerico
{
	private:
		AgenteCentral* agenteCentral;
		AgenteConstrutor* agenteConstrutor;
		AgenteMilitar* agenteMilitar;
		
		std::list<Unidade*> lista_mineradores;
		std::list<Unidade*> lista_exploradores;
		std::list<Unidade*> lista_centros_de_comando;
		SetPairDistUnit lista_minerais;

		Mutex* mutex_mineradores;	//Controla insercao na lista de mineradores
		Mutex* mutex_exploradores;	//Controla insercao na lista de exploradores
		Mutex* mutex_comando;		//Controla a lista de centros de comando
		Mutex* mutex_minerais;		//Unidades vão inserir minerais nessa lista

	public:
		AgenteColeta(RandGenerator* randGen);
		void CadastrarEntidades(AgenteCentral* agenteCentral, AgenteConstrutor* agenteConstrutor, AgenteMilitar* agenteMilitar);
		void CadastrarSCV(Unidade* unidadeSCV); //Recebe o novo SCV criado e o aloca para coleta ou exploracao
		void CadastrarCentroDeComando(Unidade* centroDeComando); //Cadastra novos centros de comando
		void CadastrarMineral(Unidade* mineral); //Cadastra uma nova reserva de minerais que foi encontrada
		int GetNumMineradores();
		int GetNumExploradores();
		int GetNumCentrosDeComando();
		BWAPI::Position GetCommandCenterPosition();
		Unidade* EmprestarSCV(); //emprestar SCV para o AgenteConstrucao
		void CriarSCV();
		Unidade* MineralLivre();
		void Update();
};

#endif