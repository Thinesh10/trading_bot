#include <string>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <map>
#include <stdlib.h>
#include <time.h>
#include "HTTP.h"
#include "Exchange.h"

#pragma once

class Swyftx final: public Exchange
{
	private:
		time_t theTokenGenTime;	
		std::string theToken;
		std::string theAuthBody;
		
		static constexpr auto BASE_URL = "https://api.swyftx.com.au/";
		static constexpr auto FILE_ERR = "Unable to open file";
		static constexpr auto ERR_PERFORM_REQUEST = "Error while performing HTTP request";
		static constexpr auto ERR_JSON_PARSE = "Failed to parse json string";

		enum theRouteInfo {PATH, TYPE};
		
		const std::map<std::string, std::vector<std::string>> theCallInfo = 
		{
			{"AUTH", {std::string(BASE_URL) + "refesh/", "POST"}},
		};	

	public:
		Swyftx(const bool aOnline, const uint32_t aInitBalance, const std::string aFile = "../swyftxAPIDetails.txt"): 
		Exchange(aOnline, aInitBalance, BASE_URL)
		{
			std::string myAPIKey;
			std::ifstream myFile(aFile); 
			if (myFile.is_open())
			{
				getline(myFile, myAPIKey);
				myFile.close();
			}
			else
			{
				std::cout << FILE_ERR << std::endl;
				exit(EXIT_FAILURE);
			}

			const std::map<std::string, std::string> myAuthBody = 
			{
				{"apiKey", myAPIKey},
			};

			theAuthBody = jsonify(myAuthBody);
			updateAuth();
		}; 
				
		std::string jsonify(std::map<std::string, std::string>);
		std::string getValueFromJson(std::string, std::string);
		
		void updateAuth() override;
		std::string handleRequest(const std::string& aReqType, const std::string& aReqPath, const std::string& aReqBody, const std::string& aResponseKey) override;

		~Swyftx() override = default;	
};
