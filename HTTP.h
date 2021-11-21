#include <curl/curl.h>
#include <string>
#include <iostream>
#include <string_view>
#include <vector>
#include <functional>
#include <numeric>

#pragma once

static constexpr auto THE_CURL_HANDLER_ERR = "ERR: Could not create curl handler";
static constexpr auto THE_HEADER_APPEND_ERR = "ERR: Could not append to header struct";
static constexpr auto THE_USER_AGENT = "Trading Bot/0.1";
static constexpr auto THE_APPLICATION_TYPE = "Content-Type: application/json";

class HTTP {
	private:
		CURL *theHandle;
		struct curl_slist *theHeaders = NULL; 

	public:
		static size_t response_callback(void* , size_t, size_t, void*);
		
		HTTP()
		{	
			theHeaders = NULL;
			theHeaders = curl_slist_append(theHeaders, THE_APPLICATION_TYPE);
			
			if (theHeaders == NULL) 
			{
				std::cout << THE_HEADER_APPEND_ERR << std::endl;
			}
		}
				
		CURLcode perform_request (
			const std::string& aType,
			const std::string& aURL,	
			const std::string& aBody,
			std::string *aJsonRespBuf,
			const std::vector<std::string>& aHeaders = std::vector<std::string>()
		) 
		{	
			CURLcode myResult;

			curl_global_init(CURL_GLOBAL_ALL);
			
			theHandle = curl_easy_init();
			
			if (theHandle) {
				curl_easy_setopt(theHandle, CURLOPT_CUSTOMREQUEST, aType.c_str());
				curl_easy_setopt(theHandle, CURLOPT_URL, aURL.c_str());
				curl_easy_setopt(theHandle, CURLOPT_USERAGENT, THE_USER_AGENT);
				curl_easy_setopt(theHandle, CURLOPT_WRITEFUNCTION, &response_callback);
    			curl_easy_setopt(theHandle, CURLOPT_WRITEDATA, aJsonRespBuf);
				curl_easy_setopt(theHandle, CURLOPT_POSTFIELDS, aBody.c_str());	

				auto myHeaderAppendLamba = [](const auto& aHeaders, const std::string& aHeaderEntry)
				{
					if (aHeaders == NULL) 
					{
						return aHeaders;
					}

					return curl_slist_append(aHeaders, aHeaderEntry.c_str());					
				};

				theHeaders = std::accumulate(aHeaders.begin(), aHeaders.end(), theHeaders, myHeaderAppendLamba);
				if (theHeaders == NULL) 
				{
					std::cout << THE_HEADER_APPEND_ERR << std::endl;
					myResult = CURLE_FAILED_INIT;
				} 
				else 
				{
    				curl_easy_setopt(theHandle, CURLOPT_HTTPHEADER, theHeaders);
					myResult = curl_easy_perform(theHandle);
				}	
			
			} 
			else 
			{
				std::cout << THE_CURL_HANDLER_ERR << std::endl;
				myResult = CURLE_FAILED_INIT;
			}
			
			curl_easy_cleanup(theHandle);
			curl_slist_free_all(theHeaders);	
			curl_global_cleanup();

			return myResult;	
		};

		~HTTP() = default;	
};

size_t HTTP::response_callback(void* aRespBuf, size_t aSize, size_t aNumMembers, void* aResJson)
{
	if(aResJson)
	{
		std::string& myResJson = *(static_cast<std::string*>(aResJson));
		std::string::size_type myBufLen = aSize * aNumMembers;
		myResJson.append(static_cast<char*>(aRespBuf), myBufLen);
		return aNumMembers;
	}

	return 0;
}
