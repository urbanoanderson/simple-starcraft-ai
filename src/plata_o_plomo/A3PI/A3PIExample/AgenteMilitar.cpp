#include "AgenteMilitar.h"

AgenteMilitar::AgenteMilitar(RandGenerator* randGen)
{ 
	this->u = NULL;
	this->randGen = randGen;

	this->mutex_marines = new Mutex();
	this->mutex_esquadroes = new Mutex(); 
	this->mutex_barracas = new Mutex();
	this->mutex_inimigos = new Mutex(); 

	//se eu nao souber a posicao do inimigo manda a posicao do nosso centro
	enemyCommandCenter = NULL;
	enemyCommandCenterPos = BWAPI::Position(0,0);
}

void AgenteMilitar::CadastrarEntidades(AgenteCentral* agenteCentral, AgenteColeta* agenteColeta, AgenteConstrutor* agenteConstrutor)
{
	this->agenteCentral = agenteCentral;
	this->agenteColeta = agenteColeta;
	this->agenteConstrutor = agenteConstrutor;
}

void AgenteMilitar::CadastrarInimigo(Unidade* inimigo)
{
	if(inimigo == NULL)
		return;

	mutex_inimigos->Lock();
	lista_inimigos.insert(inimigo);
	mutex_inimigos->Unlock();

	//Se o inimigo for o Centro de Comando guarda informações dele 
	if(inimigo->getType() == BWAPI::UnitTypes::Terran_Command_Center
		|| inimigo->getType() == BWAPI::UnitTypes::Protoss_Nexus
		|| inimigo->getType() == BWAPI::UnitTypes::Zerg_Hatchery)
	{
		enemyCommandCenter = inimigo;	
		enemyCommandCenterPos = inimigo->getPosition();
	}
}

void AgenteMilitar::CadastrarBarraca(Unidade* barraca)
{
	mutex_barracas->Lock();
	lista_barracas.push_back(barraca);
	mutex_barracas->Unlock();
}

void AgenteMilitar::CadastrarMarine(Unidade* unidadeMarine)
{
	//Coloca o marine no cadastro geral
	mutex_marines->Lock();
	lista_marines.push_back(unidadeMarine);
	mutex_marines->Unlock();

	//Afilia o marine em um esquadrao
	mutex_esquadroes->Lock();
	
	//Se não tem esquadrao incompleto para colocar o marine, cria um.
	if(esquadroes_novos.empty() || esquadroes_novos.front()->EsquadraoCheio())
	{
		//Esquadrao* novo_esquadrao = new Esquadrao(randGen, this, 4);
		//esquadroes_novos.push_front(novo_esquadrao);
		CriarEsquadrao();
	}

	//Coloca o marine em um esquadrão. 
	esquadroes_novos.front()->CadastrarUnidade(unidadeMarine);
	mutex_esquadroes->Unlock();
}

void AgenteMilitar::CriarEsquadrao()
{
	//Ataca em ondas: primeiro esquadrao tem 2 marines, segundo  tem 3
	//e os outros tem 4
	static int numero_unidades = 2;

	Esquadrao* novo_esquadrao = new Esquadrao(randGen, this, numero_unidades);
	esquadroes_novos.push_front(novo_esquadrao);

	if(numero_unidades < 4)
		numero_unidades++;
}

int AgenteMilitar::GetNumMarines()
{
	mutex_marines->Lock();
	int num_marines = lista_marines.size();
	mutex_marines->Unlock();

	return num_marines;
}

int AgenteMilitar::GetNumBarracas()
{
	mutex_barracas->Lock();
	int num_barracas = lista_barracas.size();
	mutex_barracas->Unlock();

	return num_barracas;
}

int AgenteMilitar::GetNumEsquadroes()
{
	mutex_esquadroes->Lock();
	int num_esquadroes = esquadroes_defesa.size() + esquadroes_ataque.size();
	mutex_esquadroes->Unlock();

	return num_esquadroes;
}

BWAPI::Position AgenteMilitar::GetEnemyCommandCenterPosition()
{
	return enemyCommandCenterPos;
}

void AgenteMilitar::CriarMarine()
{
	mutex_barracas->Lock();
	if (!lista_barracas.empty())
		lista_barracas.front()->train(BWAPI::UnitTypes::Terran_Marine);
	mutex_barracas->Unlock();
}

//TODO: melhorar esse algoritmo
void AgenteMilitar::AlocarNovoEsquadrao(Esquadrao* novo)
{
	/*if(esquadroes_defesa.size() == 0)
	{
		novo->SetEstado(ESQUADRAO_DEFESA);
		esquadroes_defesa.push_back(novo);
	}
			
	else*/
	{
		novo->SetEstado(ESQUADRAO_ATAQUE);
		esquadroes_ataque.push_back(novo);
	}
}

void AgenteMilitar::Update()
{
	//Verifica se algum inimigo foi destruido e remove da lista
	mutex_inimigos->Lock();
	for(std::set<Unidade*>::iterator it = lista_inimigos.begin(); it != lista_inimigos.end();)
	{
		if(!(*it)->exists())
			it = lista_inimigos.erase(it);
		else
			it++;
	}
	mutex_inimigos->Unlock();

	//Verifica se alguma barraca foi destruida e remove da lista
	mutex_barracas->Lock();
	for(std::list<Unidade*>::iterator it = lista_barracas.begin(); it != lista_barracas.end();)
	{
		if(!(*it)->exists())
			it = lista_barracas.erase(it);
		else
			it++;
	}
	mutex_barracas->Unlock();
	
	//Verifica se algum Esquadrao foi destruido e remove da lista
	mutex_esquadroes->Lock();
	for(std::list<Esquadrao*>::iterator it = esquadroes_defesa.begin(); it != esquadroes_defesa.end();)
	{
		if((*it)->EsquadraoDestruido())
			it = esquadroes_defesa.erase(it);
		else
			it++;
	}
	for(std::list<Esquadrao*>::iterator it = esquadroes_ataque.begin(); it != esquadroes_ataque.end();)
	{
		if((*it)->EsquadraoDestruido())
			it = esquadroes_ataque.erase(it);
		else
			it++;
	}
	mutex_esquadroes->Unlock();
	
	//Verifica se há um novo esquadrao e o coloca para defesa ou ataque
	mutex_esquadroes->Lock();
	for(std::list<Esquadrao*>::iterator it = esquadroes_novos.begin(); it != esquadroes_novos.end();)
	{
		if((*it)->EsquadraoTreinado())
		{
			Esquadrao* novo = (*it);
			it = esquadroes_novos.erase(it);
			AlocarNovoEsquadrao(novo);
		}
		else
			it++;
	}
	mutex_esquadroes->Unlock();

	//Atualiza os esquadrões
	mutex_esquadroes->Lock();
	for(std::list<Esquadrao*>::iterator it = esquadroes_defesa.begin(); it != esquadroes_defesa.end(); it++)
		(*it)->Update(lista_inimigos, agenteColeta->GetCommandCenterPosition(), this->GetEnemyCommandCenterPosition());
	for(std::list<Esquadrao*>::iterator it = esquadroes_ataque.begin(); it != esquadroes_ataque.end(); it++)
		(*it)->Update(lista_inimigos, agenteColeta->GetCommandCenterPosition(), this->GetEnemyCommandCenterPosition());
	mutex_esquadroes->Unlock();
}
