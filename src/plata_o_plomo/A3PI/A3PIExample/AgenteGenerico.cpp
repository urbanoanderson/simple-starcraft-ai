#include "AgenteGenerico.h"

AgenteGenerico::AgenteGenerico()
{
	this->randGen = NULL;
	this->u = NULL;
}

AgenteGenerico::AgenteGenerico(RandGenerator* randGen)
{
	this->randGen = randGen;
	this->u = NULL;
}

void AgenteGenerico::Update()
{	

}