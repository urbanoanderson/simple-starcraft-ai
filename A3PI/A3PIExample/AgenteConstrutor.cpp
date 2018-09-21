#include "AgenteConstrutor.h"

AgenteConstrutor::AgenteConstrutor(RandGenerator* randGen)
{ 
	this->u = NULL;
	this->randGen = randGen;
	mutex_construtores = new Mutex(); 
	mutex_casas = new Mutex(); 
}

void AgenteConstrutor::CadastrarEntidades(AgenteCentral* agenteCentral, AgenteColeta* agenteColeta, AgenteMilitar* agenteMilitar)
{
	this->agenteCentral = agenteCentral;
	this->agenteColeta = agenteColeta;
	this->agenteMilitar = agenteMilitar;
}

void AgenteConstrutor::CadastrarCasa(Unidade* casa)
{
	mutex_casas->Lock();
	lista_casas.push_back(casa);
	mutex_casas->Unlock();
}

int AgenteConstrutor::GetNumConstrutores()
{
	mutex_construtores->Lock();
	int num_construtores = lista_construtores.size();
	mutex_construtores->Unlock();

	return num_construtores;
}

Unidade* AgenteConstrutor::PegarSCVEmprestado()
{
	Unidade* unidadeSCV = agenteColeta->EmprestarSCV();

	if(unidadeSCV != NULL) 
	{
		mutex_construtores->Lock();
		lista_construtores.push_front(unidadeSCV);
		mutex_construtores->Unlock();
	}

	return unidadeSCV;
}

void AgenteConstrutor::DevolverSCV(Unidade* unidadeSCV)
{
	agenteColeta->CadastrarSCV(unidadeSCV);
}

void AgenteConstrutor::CriarConstrucao(BWAPI::UnitType tipo_construcao)
{
	//Impede que muitos SCVs sejam mandados para construção
	mutex_construtores->Lock();
	int tam = lista_construtores.size();
	mutex_construtores->Unlock();
	if(tam >= 2)
		return;

	Unidade* operario = PegarSCVEmprestado();

	//Se arrumou um operario para a obra, executa
	if(operario != NULL)
	{
		bool built = false;

		BWAPI::TilePosition tp;
		BWAPI::TilePosition atp = operario->getTilePosition();
		//BWAPI::Position ccp = agenteColeta->GetCommandCenterPosition();
		//BWAPI::TilePosition atp = BWAPI::TilePosition(ccp.x(), ccp.y());

		//TODO: Testar esses limites
		for (int dx = -5; !built && dx <= 5; dx++)
		{
			for (int dy = -5; !built && dy <= 5; dy++)
			{
				tp = BWAPI::TilePosition(atp.x()+dx, atp.y()+dy);
				tp.makeValid();
					
				//Mandar executar o projeto
				if(operario->build(tp, tipo_construcao))
					built = true;
			}
		}
	}
}

void AgenteConstrutor::Update()
{
	//Verifica se possui operarios ociosos (que já terminaram a obra) e os devolve ao AgenteColeta
	for(std::list<Unidade*>::iterator it = lista_construtores.begin(); it != lista_construtores.end();)
	{
		if((*it)->isIdle())
		{
			DevolverSCV(*it);
			it = lista_construtores.erase(it);
		}
		else 
			it++;
	}
}