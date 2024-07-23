#include "engine.h"

int main()
{
	Engine* renderer=Engine::getEngine();
	renderer->start();

	return 1;
}