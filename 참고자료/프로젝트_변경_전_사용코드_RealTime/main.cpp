#include <iostream>
#include "Engine.h"

int main(void)
{
	Engine engine;

	bool success = engine.InitializeEngine(L"Software Renderer Engine", 1000, 800, 0, 0, 0, true);

	if (success)
		return engine.Run();
	else
		return -1;
}