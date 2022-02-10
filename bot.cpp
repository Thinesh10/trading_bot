#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <curl/curl.h>
#include <memory>
#include "HTTP.h"
#include "Exchange.h"
#include "Swyftx.h"

int main()
{	
	auto myExch = std::make_unique<Swyftx>(false, 10000);	
    return 0;
}
