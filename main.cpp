#include "Application.h"

int main(int argc, char *argv[])
{
	Application app;
	while (!app.UpdateAndRender()) {}
	return 0;
}