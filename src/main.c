#include "srdefs.h"
#include "application.h"

int main(int argc, char* args[]) {
    if( Application_Init() != NULL )
		Application_Run();
	
	Application_Close();

    return 0;
}
