#include "AgenteCentral.h"

AgenteCentral::AgenteCentral(RandGenerator* randGen)
{ 
	this->u = NULL;
	this->randGen = randGen;
	this->firstUpdate = true;
}

void AgenteCentral::CadastrarEntidades(AgenteColeta* agenteColeta, AgenteConstrutor* agenteConstrutor, AgenteMilitar* agenteMilitar)
{
	this->agenteColeta = agenteColeta;
	this->agenteConstrutor = agenteConstrutor;
	this->agenteMilitar = agenteMilitar;
}

int AgenteCentral::getSupply()
{
	return (u->supplyTotal()/2 - u->supplyUsed()/2);
}

bool AgenteCentral::PrecisaCentroDeComando()
{
	return (agenteColeta->GetNumCentrosDeComando() == 0);
}

bool AgenteCentral::PrecisaCasa()
{
	//Se falta X supply pra chegar no limite
	return (getSupply()-2 <= 0);
}

bool AgenteCentral::PrecisaSCV()
{
	int num_mineradores = agenteColeta->GetNumMineradores();
	int num_exploradores = agenteColeta->GetNumExploradores();
	int num_construtores = agenteConstrutor->GetNumConstrutores();
	printf("AgenteCentral: PrecisaSCV() %d %d\n", num_mineradores + num_construtores, num_exploradores);
	if(num_mineradores + num_construtores < 7 || num_exploradores < 1)
		return true;
	else
		return false;
}

bool AgenteCentral::PrecisaBarraca()
{
	return (agenteMilitar->GetNumBarracas() == 0);
}

bool AgenteCentral::PrecisaMarine()
{
	return true;
}

int AgenteCentral::TomarDecisao()
{
	int decisao = DECISAO_CENTRAL_FAZ_NADA;

	if(PrecisaCentroDeComando())
		decisao = DECISAO_CENTRAL_COMMAND_CENTER;
	else if(PrecisaCasa())
		decisao = DECISAO_CENTRAL_CASA;
	else if(PrecisaSCV())
		decisao = DECISAO_CENTRAL_SCV;
	else if(PrecisaBarraca())
		decisao = DECISAO_CENTRAL_BARRACA;
	else if(PrecisaMarine())
		decisao = DECISAO_CENTRAL_MARINE;

	return decisao;
}

void AgenteCentral::Update()
{
	//Só pra dar tempo das unidades que comecam no jogo serem cadastradas
	//Se não ele pode pensar que não tem centro de comando, etc.
	if(firstUpdate == true)
	{
		Sleep(50);
		firstUpdate = false;
	}

	//Toma uma nova decisao se a ultima já foi cumprida
	int decisao_atual = TomarDecisao();
	
	//Centros de comando primeiro
	if(decisao_atual == DECISAO_CENTRAL_COMMAND_CENTER)
	{	
		if(u->minerals() >= 400)
		{
			printf("AgenteCentral: crie centro de comando\n");
			agenteConstrutor->CriarConstrucao(BWAPI::UnitTypes::Terran_Command_Center);
		}
	}

	//Casas depois
	else if(decisao_atual == DECISAO_CENTRAL_CASA)
	{
		if(u->minerals() >= 100)
		{
			printf("AgenteCentral: crie casa\n");
			agenteConstrutor->CriarConstrucao(BWAPI::UnitTypes::Terran_Supply_Depot);
		}
	}

	//SCVs depois
	else if(decisao_atual == DECISAO_CENTRAL_SCV)
	{
		if(u->minerals() >= 50 && getSupply() >= 1)
		{
			printf("AgenteCentral: crie scv\n");
			agenteColeta->CriarSCV();
		}
	}

	//Barracas depois
	else if(decisao_atual == DECISAO_CENTRAL_BARRACA)
	{	
		if(u->minerals() >= 150)
		{
			printf("AgenteCentral: crie barraca\n");
			agenteConstrutor->CriarConstrucao(BWAPI::UnitTypes::Terran_Barracks);
		}
	}
	
	//Marines depois
	else if(decisao_atual == DECISAO_CENTRAL_MARINE)
	{
		if(u->minerals() >= 50 && getSupply() >= 1)
		{
			printf("AgenteCentral: crie marine\n");
			agenteMilitar->CriarMarine();
		}
	}

	//Default: uma decisao de não fazer nada
	else if(decisao_atual == DECISAO_CENTRAL_FAZ_NADA)
	{
		printf("AgenteCentral: nao fazer nada\n");
	}

	//Após cada decisão espera um pouco
	Sleep(50);
}
