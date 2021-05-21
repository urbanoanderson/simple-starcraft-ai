#include "AgenteColeta.h"

AgenteColeta::AgenteColeta(RandGenerator* randGen)
{ 
	this->u = NULL;
	this->randGen = randGen;

	mutex_mineradores = new Mutex();
	mutex_exploradores = new Mutex();
	mutex_comando = new Mutex();
	mutex_minerais = new Mutex();
}

void AgenteColeta::CadastrarEntidades(AgenteCentral* agenteCentral, AgenteConstrutor* agenteConstrutor, AgenteMilitar* agenteMilitar)
{
	this->agenteCentral = agenteCentral;
	this->agenteConstrutor = agenteConstrutor;
	this->agenteMilitar = agenteMilitar;
}

void AgenteColeta::CadastrarSCV(Unidade* unidadeSCV)
{
	printf("AgenteColeta: CadastrarSCV()\n");

	mutex_exploradores->Lock();
	int exp_size = lista_exploradores.size();
	mutex_exploradores->Unlock();

	//Se não tiver nenhum explorador cria um
	if(exp_size == 0) 
	{	
		mutex_exploradores->Lock();
		lista_exploradores.push_back(unidadeSCV);
		mutex_exploradores->Unlock();
	}

	//Caso contrário cria um minerador
	else 
	{	
		mutex_mineradores->Lock();
		lista_mineradores.push_back(unidadeSCV);
		mutex_mineradores->Unlock();
	}
}

void AgenteColeta::CadastrarCentroDeComando(Unidade* centro_de_comando)
{
	mutex_comando->Lock();
	lista_centros_de_comando.push_back(centro_de_comando);
	mutex_comando->Unlock();
}

void AgenteColeta::CadastrarMineral(Unidade* mineral)
{
	//Calcula o par (mineral, distancia até o centro de comando)
	int dist = SquareDistance(mineral->getPosition(), GetCommandCenterPosition());
	std::pair<int, Unidade*> novo_par(dist, mineral);

	mutex_minerais->Lock();
	lista_minerais.insert(novo_par);
	mutex_minerais->Unlock();
}

int AgenteColeta::GetNumMineradores()
{
	mutex_mineradores->Lock();
	int num_mineradores = lista_mineradores.size();
	mutex_mineradores->Unlock();

	return num_mineradores;
}

int AgenteColeta::GetNumExploradores()
{
	mutex_exploradores->Lock();
	int num_exploradores = lista_exploradores.size();
	mutex_exploradores->Unlock();

	return num_exploradores;
}

int AgenteColeta::GetNumCentrosDeComando()
{
	mutex_comando->Lock();
	int num_comando = lista_centros_de_comando.size();
	mutex_comando->Unlock();

	return num_comando;
}

BWAPI::Position AgenteColeta::GetCommandCenterPosition()
{
	BWAPI::Position pos = BWAPI::Position(0, 0);

	mutex_comando->Lock();
	if(lista_centros_de_comando.size() > 0)
		pos = lista_centros_de_comando.front()->getPosition();
	mutex_comando->Unlock();

	return pos;
}

Unidade* AgenteColeta::EmprestarSCV()
{
	Unidade* unidadeSCV = NULL;

	mutex_mineradores->Lock();
	if (lista_mineradores.size() > 0) 
	{
		unidadeSCV = lista_mineradores.front();
		lista_mineradores.pop_front();
	}
	mutex_mineradores->Unlock();
	
	return unidadeSCV;
}

void AgenteColeta::CriarSCV()
{
	mutex_comando->Lock();
	if(lista_centros_de_comando.size() > 0)
	{
		bool ok = false;
		for(int tentativas = 0; !ok && tentativas < 3; tentativas++) 
			ok = lista_centros_de_comando.front()->train(BWAPI::UnitTypes::Terran_SCV);
	}
	mutex_comando->Unlock();
}

Unidade* AgenteColeta::MineralLivre()
{
	Unidade* ret = NULL;
	mutex_minerais->Lock();

	//Pra cada mineral
	for(std::set<PairDistUnit>::iterator it2 = lista_minerais.begin(); ret == NULL && it2 != lista_minerais.end(); it2++)
	{
		Unidade* min = it2->second;
		//Se não está sendo minerado coloca alguem pra minerar
		if(!(min->isVisible()) || !(min->isBeingGathered())) 
			ret = min;
	}

	if (ret == NULL && !lista_minerais.empty())
		ret = lista_minerais.begin()->second;

	mutex_minerais->Unlock();

	return ret;
}

void AgenteColeta::Update()
{
	//printf("AgenteColeta: Update\n");

	//Verifica se algum centro de comando foi destruido e remove da lista
	mutex_comando->Lock();
	for(std::list<Unidade*>::iterator it = lista_centros_de_comando.begin(); it != lista_centros_de_comando.end();)
	{
		if(!(*it)->exists())
			it = lista_centros_de_comando.erase(it);
		else
			it++;
	}
	mutex_comando->Unlock();

	//Verifica se algum explorador morreu e remove da lista
	mutex_exploradores->Lock();
	for(std::list<Unidade*>::iterator it = lista_exploradores.begin(); it != lista_exploradores.end();)
	{
		if(!(*it)->exists())
			it = lista_exploradores.erase(it);
		else
			it++;
	}
	mutex_exploradores->Unlock();

	//Verifica se algum minerador morreu e remove da lista
	mutex_mineradores->Lock();
	for(std::list<Unidade*>::iterator it = lista_mineradores.begin(); it != lista_mineradores.end();)
	{
		if(!(*it)->exists()) 
			it = lista_mineradores.erase(it);
		else
			it++;
	}
	mutex_mineradores->Unlock();

	//Verifica a lista de minerais se tem algum que acabou e remove da lista
	mutex_minerais->Lock();
	for(SetPairDistUnit::iterator it = lista_minerais.begin(); it != lista_minerais.end();)
	{
		if(!(*it).second->exists())
			it = lista_minerais.erase(it);
		else
			it++;
	}
	mutex_minerais->Unlock();

	//Verifica se algum explorador achou minerios ou inimigos
	mutex_exploradores->Lock();
	for(std::list<Unidade*>::iterator it = lista_exploradores.begin(); it != lista_exploradores.end(); it++)
	{
		//Minérios
		std::set<Unidade*> minerais = (*it)->getMinerals();
		for(std::set<Unidade*>::iterator it2 = minerais.begin(); it2 != minerais.end(); it2++) 
			this->CadastrarMineral(*it2);
		//Inimigos
		std::set<Unidade*> inimigos = (*it)->getEnemyUnits();
		for(std::set<Unidade*>::iterator it2 = inimigos.begin(); it2 != inimigos.end(); it2++)
			agenteMilitar->CadastrarInimigo(*it2);
	}
	mutex_exploradores->Unlock();

	//Verifica se algum minerador achou minerios ou inimigos
	mutex_mineradores->Lock();
	for(std::list<Unidade*>::iterator it = lista_mineradores.begin(); it != lista_mineradores.end(); it++)
	{
		//Minérios
		std::set<Unidade*> minerais = (*it)->getMinerals();
		for(std::set<Unidade*>::iterator it2 = minerais.begin(); it2 != minerais.end(); it2++) 
			this->CadastrarMineral(*it2);
		//Inimigos
		std::set<Unidade*> inimigos = (*it)->getEnemyUnits();
		for(std::set<Unidade*>::iterator it2 = inimigos.begin(); it2 != inimigos.end(); it2++)
			agenteMilitar->CadastrarInimigo(*it2);
	}
	mutex_mineradores->Unlock();

	//Coloca os exploradores para andarem pelo mapa aleatoriamente
	mutex_exploradores->Lock();
	for(std::list<Unidade*>::iterator it = lista_exploradores.begin(); it != lista_exploradores.end(); it++)
	{
		int map_size_x = 2048;
		int map_size_y = 2048;

		//Anda pelo mapa aleatoriamente
		if((*it)->isIdle())
		{
			int pos_x = randGen->GetNumber(map_size_x);
			int pos_y = randGen->GetNumber(map_size_y);
			BWAPI::Position atp = BWAPI::Position(pos_x, pos_y);
			(*it)->move(atp);
		}
	}
	mutex_exploradores->Unlock();

	//Verifica mineradores ociosos e os aloca para os recursos disponíveis
	mutex_mineradores->Lock();
	for(std::list<Unidade*>::iterator it = lista_mineradores.begin(); it != lista_mineradores.end(); it++)
	{
		//Se está ocioso
		if((*it)->isIdle())
		{
			Unidade* mineral = MineralLivre();
			if (mineral != NULL)
				(*it)->rightClick(mineral);
		}
	}
	mutex_mineradores->Unlock();
}