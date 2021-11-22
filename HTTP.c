#include "HTTP.h"

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

CURLcode HTTP::perform_request (
	const std::string& aType,
	const std::string& aURL,	
	const std::string& aBody,
	std::string *aJsonRespBuf,
	const std::vector<std::string>& aHeaders
) 
{	
	CURLcode myResult;

	curl_global_init(CURL_GLOBAL_ALL);
	
	theHandle = curl_easy_init();
	
	if (theHandle) {
		curl_easy_setopt(theHandle, CURLOPT_CUSTOMREQUEST, aType.c_str());
		curl_easy_setopt(theHandle, CURLOPT_URL, aURL.c_str());
		curl_easy_setopt(theHandle, CURLOPT_USERAGENT, USER_AGENT);
		curl_easy_setopt(theHandle, CURLOPT_WRITEFUNCTION, &response_callback);
		curl_easy_setopt(theHandle, CURLOPT_WRITEDATA, aJsonRespBuf);
		curl_easy_setopt(theHandle, CURLOPT_POSTFIELDS, aBody.c_str());	

		theHeaders = std::accumulate(aHeaders.begin(), aHeaders.end(), theHeaders, 
			[](const auto& aHeaders, const std::string& aHeaderEntry)
			{
				if (aHeaders == nullptr) 
				{
					return aHeaders;
				}

				return curl_slist_append(aHeaders, aHeaderEntry.c_str());					
			}
		);

		if (theHeaders == nullptr) 
		{
			std::cout << HEADER_APPEND_ERR << std::endl;
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
		std::cout << CURL_HANDLER_ERR << std::endl;
		myResult = CURLE_FAILED_INIT;
	}
	
	curl_easy_cleanup(theHandle);
	curl_slist_free_all(theHeaders);	
	curl_global_cleanup();

	return myResult;	
};

