#include "MeuAgentePrincipal.h"

//Variáveis Globais
bool GameOver = false;
bool log_opened = false;
bool done_initializing = false;
RandGenerator* randGen;
AgenteCentral* agenteCentral;
AgenteColeta* agenteColeta;
AgenteConstrutor* agenteConstrutor;
AgenteMilitar* agenteMilitar;

/*
	>> Thread que governa um AgenteGenerico
	- Chamar a função de update para cada agente
*/
DWORD WINAPI threadAgente(LPVOID param)
{
	AgenteGenerico* agente = (AgenteGenerico*) param;

	while(true)
	{
		//Se o jogo encerrou encerra a thread
		if(GameOver == true) 
			return 0;

		//O Agente executa seu trabalho
		agente->Update();

		//Dorme pelo menos 10ms no final do loop
		//pois uma iteracao da thread é muito mais rápida do que um turno do bwapi.
		Sleep(10);
	}
}

/*
	>> Rotina executada durante a criação de uma nova unidade do jogo (incluindo as que iniciam na partida)
	- Associa a unidade a uma entidade de trabalho (AgenteColeta, AgenteMilitar, etc)
	- Cria o arquivo de log
*/
void MeuAgentePrincipal::UnidadeCriada(Unidade* u)
{
	BWAPI::UnitType tipo = u->getType();

	//Abre o log para debugar
	if(!log_opened)
	{
		freopen("log.txt", "w", stdout);
		log_opened = true;
	}

	//Espera a criação das entidades
	while(!done_initializing)
	{
		printf("MeuAgentePrincipal: esperando InicioDePartida()\n");
		Sleep(10);
	}

	//Se for operário, cadastra no agenteColeta.
	if(tipo == BWAPI::UnitTypes::Terran_SCV)
	{
		printf("MeuAgentePrincipal: scv criado\n");
		agenteColeta->CadastrarSCV(u);
	}

	//Se for marine, cadastra no agenteMilitar.
	else if(tipo == BWAPI::UnitTypes::Terran_Marine)
	{
		printf("MeuAgentePrincipal: marine criado\n");
		agenteMilitar->CadastrarMarine(u);
	}

	//Se for Command Center, cadastra no agenteColeta. 
	else if(tipo == BWAPI::UnitTypes::Terran_Command_Center)
	{
		printf("MeuAgentePrincipal: centro de comando criado\n");
		agenteColeta->CadastrarCentroDeComando(u);
	}

	//Se for Casa, cadastra no agenteConstrutor. 
	else if(tipo == BWAPI::UnitTypes::Terran_Supply_Depot)
	{
		printf("MeuAgentePrincipal: casa criada\n");
		agenteConstrutor->CadastrarCasa(u);
	}

	//Se for Barraca, cadastra no agenteMilitar. 
	else if(tipo == BWAPI::UnitTypes::Terran_Barracks)
	{
		printf("MeuAgentePrincipal: barraca criada\n");
		agenteMilitar->CadastrarBarraca(u);
	}
}

/*
	>> Ponto de encerramento do programa
*/
void MeuAgentePrincipal::onEnd(bool isWinner)
{  
	GameOver = true;
	Sleep(550);
}

/*
	>> Ponto de partida do programa
	- Cria os Agentes que são as entidades e os inicializa
	- Inicializa o gerador de números aleatórios thread-safe
*/
void MeuAgentePrincipal::InicioDePartida()
{
	GameOver = false;
	randGen = new RandGenerator();

	//Cria as entidades
	agenteCentral = new AgenteCentral(randGen);
	agenteColeta = new AgenteColeta(randGen);
	agenteConstrutor = new AgenteConstrutor(randGen);
	agenteMilitar = new AgenteMilitar(randGen);

	//Cadastra os ponteiros das entidades umas nas outras para comunicação
	agenteCentral->CadastrarEntidades(agenteColeta, agenteConstrutor, agenteMilitar);
	agenteColeta->CadastrarEntidades(agenteCentral, agenteConstrutor, agenteMilitar);
	agenteConstrutor->CadastrarEntidades(agenteCentral, agenteColeta, agenteMilitar);
	agenteMilitar->CadastrarEntidades(agenteCentral, agenteColeta, agenteConstrutor);
		
	//Cria e coloca a thread de cada entidade para rodar
	CreateThread(NULL, 0, threadAgente, (void*)agenteCentral, 0, NULL);
	CreateThread(NULL, 0, threadAgente, (void*)agenteColeta, 0, NULL);
	CreateThread(NULL, 0, threadAgente, (void*)agenteConstrutor, 0, NULL);
	CreateThread(NULL, 0, threadAgente, (void*)agenteMilitar, 0, NULL);

	done_initializing = true;
}