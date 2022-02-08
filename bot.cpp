#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <curl/curl.h>
#include "HTTP.h"
#include "Exchange.h"
#include "Swyftx.h"

int main()
{	
	Exchange *exch = new Swyftx(false, 10000);	
    return 0;
}
