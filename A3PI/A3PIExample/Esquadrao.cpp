#include "Esquadrao.h"

Esquadrao::Esquadrao(RandGenerator* randGen, AgenteMilitar* agenteMilitar)
{
	this->randGen = randGen;
	this->agenteMilitar = agenteMilitar;
	Inicializar(5);
}

Esquadrao::Esquadrao(RandGenerator* randGen, AgenteMilitar* agenteMilitar, int numero_unidades)
{
	this->randGen = randGen;
	this->agenteMilitar = agenteMilitar;
	Inicializar(numero_unidades);
}

void Esquadrao::Inicializar(int numero_unidades)
{
	this->numero_cadastrados = 0;
	this->numero_desejado = numero_unidades;
	this->raio_atuacao = 700*700;
	this->estado = ESQUADRAO_INCOMPLETO;
	this->alvo = NULL;
}

void Esquadrao::SetEstado(int estado)
{
	this->estado = estado;
}

void Esquadrao::CadastrarUnidade(Unidade* unidade)
{
	if(unidade != NULL)
	{
		lista_unidades.push_back(unidade);
		numero_cadastrados++;
	}
}

bool Esquadrao::EsquadraoCheio()
{
	if(numero_cadastrados >= numero_desejado)
		return true;
	return false;
}

bool Esquadrao::EsquadraoTreinado()
{
	if(!EsquadraoCheio())
		return false;

	//Verifica se todas as unidades estao treinadas
	for(std::list<Unidade*>::iterator it = lista_unidades.begin(); it != lista_unidades.end(); it++)
	{
		//Se algum ainda está sendo construido nao está preparado
		if(!(*it)->isCompleted())
			return false;
	}

	return true;
}

bool Esquadrao::EsquadraoDestruido()
{
	//Conta quantas unidades vivas ainda tem
	int unidades_vivas = 0;
	for(std::list<Unidade*>::iterator it = lista_unidades.begin(); it != lista_unidades.end(); it++)
	{
		if((*it) != NULL && (*it)->exists())
			unidades_vivas++;
	}

	return (unidades_vivas == 0);
}

BWAPI::Position Esquadrao::GetPosition()
{
	for(std::list<Unidade*>::iterator it = lista_unidades.begin(); it != lista_unidades.end(); it++)
	{
		if((*it) != NULL && (*it)->exists())
			return (*it)->getPosition();
	}
	
	return BWAPI::Position(0,0);	
}

void Esquadrao::ProcurarInimigos()
{
	for(std::list<Unidade*>::iterator it = lista_unidades.begin(); it != lista_unidades.end(); it++)
	{
		std::set<Unidade*> inimigos = (*it)->getEnemyUnits();

		for(std::set<Unidade*>::iterator it2 = inimigos.begin(); it2 != inimigos.end(); it2++)
			agenteMilitar->CadastrarInimigo(*it2);
	}
}

std::set<Unidade*> Esquadrao::InimigosProximos(std::set<Unidade*> lista_inimigos)
{
	//Filtra a lista de inimigos para ficar só com os inimigos proximos
	for(std::set<Unidade*>::iterator it = lista_inimigos.begin(); it != lista_inimigos.end(); )
	{
		if(!(*it)->exists() || !(*it)->isVisible())
		{
			it = lista_inimigos.erase(it);
			continue;
		}
		
		if(SquareDistance((*it)->getPosition(), this->GetPosition()) > raio_atuacao)
		{
			it = lista_inimigos.erase(it);
			continue;
		}

		it++;		
	}
	printf("Inimigos proximos encontrados: %d\n", (int)lista_inimigos.size());
	return lista_inimigos;
}

bool Esquadrao::EhUnidadeMilitar(Unidade* u)
{
	BWAPI::UnitType tipo = u->getType();

	if(tipo == BWAPI::UnitTypes::Terran_Marine
	|| tipo == BWAPI::UnitTypes::Protoss_Zealot
	|| tipo == BWAPI::UnitTypes::Zerg_Zergling)
		return true;

	return false;
}

bool Esquadrao::EhUnidadeCivil(Unidade* u)
{
	BWAPI::UnitType tipo = u->getType();

	if(tipo == BWAPI::UnitTypes::Terran_SCV
	|| tipo == BWAPI::UnitTypes::Protoss_Probe
	|| tipo == BWAPI::UnitTypes::Zerg_Drone)
		return true;

	return false;
}

bool Esquadrao::EhUnidadeCommandCenter(Unidade* u)
{
	BWAPI::UnitType tipo = u->getType();

	if(tipo == BWAPI::UnitTypes::Terran_Command_Center
	|| tipo == BWAPI::UnitTypes::Protoss_Nexus
	|| tipo == BWAPI::UnitTypes::Zerg_Hatchery)
		return true;

	return false;
}

void Esquadrao::Atacar()
{
	//Manda todos os soldados que ainda estiverem vivos atacarem o alvo
	for(std::list<Unidade*>::iterator it = lista_unidades.begin(); it != lista_unidades.end(); it++)
	{
		(*it)->attack(alvo);
	}
}

void Esquadrao::Andar(BWAPI::Position pos)
{
	//Manda todos os soldados que ainda estiverem vivos atacarem o alvo
	for(std::list<Unidade*>::iterator it = lista_unidades.begin(); it != lista_unidades.end(); it++)
	{
		(*it)->move(pos);
	}	
}

//TODO: melhorar essa escolha
Unidade* Esquadrao::EscolherAlvo(std::set<Unidade*> inimigos_proximos) 
{
	Unidade* unidade_militar = NULL;
	Unidade* unidade_civil = NULL;
	Unidade* unidade_command_center = NULL;
	Unidade* unidade_outra = NULL;

	for(std::set<Unidade*>::iterator it = inimigos_proximos.begin(); it != inimigos_proximos.end(); it++)
	{
		if((*it)->exists() && (*it)->isVisible())
		{
			if(EhUnidadeMilitar(*it))
				unidade_militar = (*it);
			else if(EhUnidadeCivil(*it))
				unidade_civil = (*it);
			else if(EhUnidadeCommandCenter(*it))
				unidade_command_center = (*it);
			else
				unidade_outra = (*it);
		}
	}
	
	if(unidade_militar != NULL)
		return unidade_militar;
	else if(unidade_civil != NULL)
		return unidade_civil;
	else if(unidade_command_center != NULL)
		return unidade_command_center;
	else
		return unidade_outra;
}

bool Esquadrao::NovoAlvoEhMelhor(Unidade* alvo, Unidade* novo_alvo)
{
	if(alvo == NULL || !alvo->exists() || !alvo->isVisible())
		return true;
	if(novo_alvo == NULL || !novo_alvo->exists() || !novo_alvo->isVisible())
		return false;

	BWAPI::UnitType tipo = alvo->getType();
	BWAPI::UnitType novo_tipo = novo_alvo->getType();

	if(EhUnidadeMilitar(alvo))
		return false;
	else if(EhUnidadeCivil(alvo) && EhUnidadeMilitar(novo_alvo))
		return true;
	else if(EhUnidadeCommandCenter(alvo) && (EhUnidadeMilitar(novo_alvo) || EhUnidadeCivil(novo_alvo)))
		return true;
	else
		return false;
}

void Esquadrao::Update(std::set<Unidade*> lista_inimigos, BWAPI::Position commandCenterPos, BWAPI::Position enemyCommandCenterPos)
{
	//Se o esquadão não está preparado retorna
	if(!EsquadraoTreinado())
		return;

	//Unidades do esquadrao buscam inimigos e inserem na lista do AgenteMilitar
	ProcurarInimigos();

	//Busca alvos próximos
	std::set<Unidade*> inimigos_proximos = InimigosProximos(lista_inimigos);
	Unidade* novo_alvo = EscolherAlvo(inimigos_proximos);

	//Se achou um alvo melhor troca de alvo
	if(NovoAlvoEhMelhor(alvo, novo_alvo))
	{
		alvo = novo_alvo;
		
		if(alvo != NULL)
		{
			Atacar();
			return;
		}
	}

	//Se está em modo de defesa tenta ficar na base 
	if(estado == ESQUADRAO_DEFESA && (alvo == NULL || !alvo->exists() || !alvo->isVisible()))	
		Andar(commandCenterPos);
	
	//Se está em ataque tenta ir para a base inimiga
	if(estado == ESQUADRAO_ATAQUE && (alvo == NULL || !alvo->exists() || !alvo->isVisible()))
		Andar(enemyCommandCenterPos);
}