#include "RandGenerator.h"

RandGenerator::RandGenerator()
{
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);
	mutex_rand = new Mutex();
	printf("seed %d\n", seed);
}
		
int RandGenerator::GetNumber()
{
	mutex_rand->Lock();
	int numero = rand();
	mutex_rand->Unlock();

	return numero;
}
		
int RandGenerator::GetNumber(int max)
{
	mutex_rand->Lock();
	int numero = rand()%max;
	mutex_rand->Unlock();

	return numero;
}

int RandGenerator::GetSignal()
{
	mutex_rand->Lock();
	int numero = rand()%2;
	mutex_rand->Unlock();

	if(numero == 0)
		numero = -1;

	return numero;
}