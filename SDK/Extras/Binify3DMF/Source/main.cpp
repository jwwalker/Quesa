#include <iostream>
#include <cstring>

#include "Read3DMF.h"
#include "Write3DMF.h"

#include <CQ3ObjectRef.h>
#include <QuesaStorage.h>

using namespace std;


int main (int argc, char * const argv[])
{
	if ( argc < 2 )
	{
		cerr << "Too few arguments.\n" <<
					"Binify3DMF path.3dmf\n";
		return 1;
	}
	if ( argc > 2 )
	{
		cerr << "Too many arguments.\n" <<
					"Binify3DMF path.3dmf\n";
		return 1;
	}
	
	Q3Initialize();
	
	const char* pathName = argv[1];
	if (0 == std::strcmp( pathName, "-" ))
	{
		pathName = NULL;
	}
	
	std::vector<CQ3ObjectRef> objects;
	bool textForm;
	
	if (Read3DMF( pathName, objects, textForm ))
	{
		if (Write3DMF( objects, stdout ))
		{
			
		}
		else
		{
			cerr << "Failed to write.\n";
		}
	}
	else
	{
		cerr << "Failed to read.\n";
	}
	
	Q3Exit();

    return 0;
}
