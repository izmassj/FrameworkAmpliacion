// main.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "Game.h"

int main()
{
	Game::Create();

	Game::Play();

	Game::Destroy();
}
