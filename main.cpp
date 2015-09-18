#include <stdio.h>
#include <stdlib.h>

#include "Globals.h"
#include "Application.h"

enum MainStates
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

Application *app = NULL;

int main(int argc, char** argv)
{

	LOG("======> Application initiating <=======");

	MainStates state = CREATE;
	int exit = EXIT_FAILURE;

	while (state != EXIT)
	{
		switch (state)
		{
		case(CREATE) :

			LOG("======> Creation Step...");
			app = new Application(argc, argv);
			if (app == NULL)
			{
				LOG("Creation Error!");
				state = FAIL;
			}
			else
				state = AWAKE;
			break;

		case(AWAKE) :

			LOG("======> Awake Step...");
			if (app->awake() != true)
			{
				LOG("Awake Error!");
				state = FAIL;
			}
			else
				state = START;
			break;

		case(START) :

			LOG("======> Start Step...");
			if (app->start() != true)
			{
				LOG(" Start Error!");
				state = FAIL;
			}
			else
			{
				LOG("Update Step...");
				state = LOOP;
			}
			break;

		case(LOOP) :
			if (app->update() == false)
				state = CLEAN;
			break;

		case(CLEAN) :

			LOG("Clean Step...");
			if (app->clean() != true)
				state = FAIL;
			else
			{
				// RELEASE(app);
				state = EXIT;
				exit = EXIT_SUCCESS;
			}
			break;

		case(FAIL) :

			LOG("Exiting program due to errors.");
			state = EXIT;
			exit = EXIT_FAILURE;
			break;

		}
	}

	LOG("Exiting program.");

	return exit;
}