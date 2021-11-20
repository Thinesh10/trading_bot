#include <curl/curl.h>
#include <string>
#include <map>
#include <vector>

#pragma once

static constexpr std::string_view the_curl_handler_err = "ERR: error creating curl handler";

static size_t response_callback(void* aRespBuf, size_t aSize, size_t aNumMembers, void* aResJson)
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

class HTTP {
	private:
		CURL *myHandle;
		struct curl_slist *myHeaders; 
		const std::string myUserAgent = "Trading Bot/0.1";

	public:
		HTTP () {
			myHeaders = NULL;	
		};
			
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
			
			myHandle = curl_easy_init();
			
			if (myHandle) {
				curl_easy_setopt(myHandle, CURLOPT_CUSTOMREQUEST, aType.c_str());
				curl_easy_setopt(myHandle, CURLOPT_URL, aURL.c_str());
				curl_easy_setopt(myHandle, CURLOPT_USERAGENT, myUserAgent.c_str());
				curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, &response_callback);
    			curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, aJsonRespBuf);
				curl_easy_setopt(myHandle, CURLOPT_POSTFIELDS, aBody.c_str());	

				myHeaders = curl_slist_append(myHeaders, "Content-Type: application/json");
				for (const auto& myHeaderEntry: aHeaders)
					myHeaders = curl_slist_append(myHeaders, myHeaderEntry.c_str());
			
    			curl_easy_setopt(myHandle, CURLOPT_HTTPHEADER, myHeaders);

				myResult = curl_easy_perform(myHandle);
				curl_easy_cleanup(myHandle);
			} 
			else 
			{
				std::cout << the_curl_handler_err << std::endl;
    			curl_global_cleanup();
				return CURLE_FAILED_INIT;
			}
			
			curl_global_cleanup();
			return myResult;	
		};

		~HTTP() = default;	
};

