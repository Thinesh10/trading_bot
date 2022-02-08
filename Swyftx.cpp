#include <jsoncpp/json/json.h>
#include "Swyftx.h"

std::string Swyftx::jsonify(std::map<std::string, std::string> aKeyValues)
{
	std::cout << "Jsonfiying" << std::endl;
    Json::Value myJsonObj;

	for (const auto& myEntry: aKeyValues)
	{
		myJsonObj[(myEntry.first).c_str()] = myEntry.second;
	}

    return myJsonObj.asString();
}

std::string Swyftx::getValueFromJson(std::string aJsonString, std::string aResponseKey)
{
	Json::Value myJsonObj;   
    Json::Reader myJsonReader;
    if (!myJsonReader.parse(aJsonString.c_str(), myJsonObj))
	{
        std::cout << ERR_JSON_PARSE << ": "
                  << myJsonReader.getFormattedErrorMessages();
        return "";
    }

    return myJsonObj.get(aResponseKey, "").asString();;
}

void Swyftx::updateAuth()
{
	std::cout << "Updating Auth" << std::endl;
	std::string myType, myPath;
	if (theCallInfo.find("AUTH") != theCallInfo.end())
	{
		myType = (theCallInfo.find("AUTH")->second)[TYPE];
		myPath = (theCallInfo.find("AUTH")->second)[PATH];;
	} 
	else
	{
		std::cout << "NOT IN ROUTE MAP" << std::endl;
		return;
	} 

    theToken = handleRequest(myType, myPath, theAuthBody, "accessToken");
	theTokenGenTime = time(nullptr);
	
	std::cout << theToken << std::endl;
	std::cout << theTokenGenTime << std::endl;
}

std::string Swyftx::handleRequest(const std::string& aReqType, const std::string& aReqPath, const std::string& aReqBody, const std::string& aResponseKey)
{
    std::string myResponseString;
    HTTP myHTTPObj;

    CURLcode res = myHTTPObj.perform_request(
		aReqType, 
		aReqPath, 
		aReqBody, 
		&myResponseString
	);

    if (res != CURLE_OK)
    {
        std::cout << ERR_PERFORM_REQUEST << std::endl;
    } else {
        std::cout << myResponseString << std::endl;
    }

	return getValueFromJson(myResponseString, aResponseKey);
}
