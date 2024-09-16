#include <curl/curl.h>
#include <string>
#include <iostream>
#include <string_view>
#include <vector>
#include <functional>
#include <numeric>

#pragma once

class HTTP {
	private:
		CURL *theHandle;
		struct curl_slist *theHeaders = nullptr; 

		static constexpr auto CURL_HANDLER_ERR = "ERR: Could not create curl handler";
		static constexpr auto HEADER_APPEND_ERR = "ERR: Could not append to header struct";
		static constexpr auto USER_AGENT = "Trading Bot/0.1";
		static constexpr auto APPLICATION_TYPE = "Content-Type: application/json";

	public:
		HTTP()
		{	
			theHeaders = curl_slist_append(theHeaders, APPLICATION_TYPE);
			
			if (theHeaders == nullptr) 
			{
				std::cout << HEADER_APPEND_ERR << std::endl;
			}
		}
				
		static size_t response_callback(void* , size_t, size_t, void*);
		CURLcode perform_request (const std::string&, const std::string&, const std::string&, std::string *, const std::vector<std::string>& = std::vector<std::string>()
		); 
		
		~HTTP() = default;	
};
