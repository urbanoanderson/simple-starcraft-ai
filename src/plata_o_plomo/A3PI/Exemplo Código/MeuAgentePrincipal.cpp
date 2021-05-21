#include "MeuAgentePrincipal.h"
#include <AgentePrincipal.h>
#include <Windows.h>
#include <BWAPI\Position.h>
#include <BWAPI\UnitType.h>

//blackboard!
bool GameOver = false;
Unidade* amigoDaVez = NULL;
//

void AITrabalhador (Unidade* u){
	double distance = 99999;
	Unidade* mineralPerto = NULL;
	std::set<Unidade*> minerais = u->getMinerals();
	for(std::set<Unidade*>::iterator it = minerais.begin(); it != minerais.end(); it++){
		if(u->getDistance(*it) < distance){
			distance = u->getDistance(*it);
			mineralPerto = *it;
		}//vai minerar no mineral mais perto
	}
	if(mineralPerto != NULL) u->rightClick(mineralPerto);
}

void AIConstrutora (Unidade* u){
	if(u->minerals() < 149) return; //por conveniencia alguns metodos de Player estao sendo providos diretamente para as unidades. Vide inicio de Unidade.h
	BWAPI::TilePosition tp = u->getTilePosition();
	int limite = 0;
	int adi = 6;
	//Construir algo em algum lugar
	while(!(u)->build(tp, u->minerals() > 449 ? BWAPI::UnitTypes::Protoss_Nexus : BWAPI::UnitTypes::Protoss_Pylon)){
		if(((u->minerals()&0xF)>>2) < 2) 
			tp = BWAPI::TilePosition(tp.x(), tp.y()+adi);
		else
			tp = BWAPI::TilePosition(tp.x()+adi, tp.y());
		tp.makeValid();
		limite++;
		if(limite > 50) break;
		adi = -adi + (adi > 0 ? -2 : +2);
	}
	amigoDaVez = NULL;//Bug aqui: amigoDaVez vai ser escolhido de novo antes mesmo do predio terminar...
}

void AICentroComando (Unidade* u){
	u->train(BWAPI::UnitTypes::Protoss_Probe);
	u->train(BWAPI::UnitTypes::Protoss_Zealot);//se for um gateway
	if((u->supplyTotal() - u->supplyUsed() < 5 || u->minerals() > 449) && amigoDaVez == NULL){
		//botar no "blackboard" para alguem construir predios de supply
		std::set<Unidade*> amigos = u->getAllyUnits();
		for(std::set<Unidade*>::iterator it = amigos.begin(); it != amigos.end(); it++){
			if((*it)->getType().isWorker()){
					amigoDaVez = *it;
					break;
			}
		}
	}//Lembrar que ha varias threads rodando em paralelo. O erro presente neste metodo (qual?) nao resulta em crash do jogo, mas outros poderiam.
}

void AIGuerreiro (Unidade* u){
	u->attack(*(u->getEnemyUnits().begin())); //ataca uma unidade inimiga aleatoria. Assume que existe uma.
	//cuidado com bugs como este. O codigo acima daria crash de null pointer no exato momento que o time inimigo
	//nao possuisse mais unidades, antes da partida de fato acabar.
}

DWORD WINAPI threadAgente(LPVOID param){
	
	Unidade *u = (Unidade*) param;

	//exemplos de metodos uteis para construir predios
	bool x = u->hasPower(3,4,50,60);
	u->isBuildable(50,50);
	u->isBuildable(BWAPI::TilePosition(3,5));

	int heig = AgentePrincipal::mapHeight();
	int wid = AgentePrincipal::mapWidth();
	//A classe Agente Principal ainda tem o metodo AgentePrincipal bool isWalkable(int x, int y).

	while(true){
		//Se houve algum problema (ex: o jogo foi fechado) ou a unidade estah morta, finalizar a thread
		if(GameOver || u == NULL || !u->exists()) return 0;
		//Enquanto a unidade ainda nao terminou de ser construida ou o seu comando ainda nao foi
		//processado (ou seja, continua no mesmo turno), aguardar e poupar processamento
		if(!u->isCompleted()){
			Sleep(500);
			continue;
		}
		if(!u->checkNovoTurno()){
			Sleep(10);
			continue;
		}
		//Inserir o codigo de voces a partir daqui//
		if(u->isIdle()){ //nao ta fazendo nada, fazer algo util
			if(u == amigoDaVez) AIConstrutora(u);
			else if(u->getType().isWorker()) AITrabalhador(u);
			else if(u->getType().canProduce()) AICentroComando(u);
			else AIGuerreiro(u);
		}
		else if(u->getType().isWorker() && u == amigoDaVez) AIConstrutora(u); //construir msm q estivesse fazendo algo
		Sleep(10);//Sempre dormir pelo menos 10ms no final do loop, pois uma iteracao da thread é muito mais rápida do que um turno do bwapi.
	}
}

void MeuAgentePrincipal::InicioDePartida(){
	//Inicializar estruturas de dados necessarias, ou outras rotinas de inicio do seu programa. Cuidado com concorrencia, 
	//em alguns casos pode ser recomendavel que seja feito antes do while na criacao de uma nova thread.
	GameOver = false;
}

void MeuAgentePrincipal::onEnd(bool isWinner){  
	//sinalizar e aguardar o tempo necessario para todas as threads se terminarem antes do jogo sair, evitando erros.
	GameOver = true;
	Sleep(550);
}

void MeuAgentePrincipal::UnidadeCriada(Unidade* unidade){
	//Uma nova unidade sua foi criada (isto inclui as do inicio da partida). Implemente aqui como quer tratar ela.
	BWAPI::UnitType tipo = unidade->getType();
	
	//Nao desperdicar threads com predios que nao fazem nada
	if(tipo != BWAPI::UnitTypes::Protoss_Pylon && tipo != BWAPI::UnitTypes::Protoss_Assimilator){
		CreateThread(NULL,0,threadAgente,(void*)unidade,0,NULL);
	}
	else{
		//talvez voce queira manter a referencia a estes predios em algum lugar
	}
}

/*
	Os outros eventos nao existem numa arquitetura orientada a Agentes Autonomos, pois eram relacionados ao Player do Broodwar
	de maneira generica, nao sendo especificamente ligados a alguma unidade do jogador. Se desejado, seus comportamentos podem
	ser simulados através de técnicas ou estruturas de comunicação dos agentes, como por exemplo Blackboards.
*/


