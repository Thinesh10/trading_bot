#pragma once

class Exchange
{
	protected:
		bool theOnline;
		uint32_t theCurrBalance;
		std::string theBaseURL;
			
	public:
		Exchange(const bool aOnline, const uint32_t aInitBalance, const std::string aBaseURL): 
				theOnline(aOnline), theCurrBalance(aInitBalance), theBaseURL(aBaseURL) {};	

		virtual void updateAuth() = 0;	
		virtual std::string handleRequest(const std::string&, const std::string&, const std::string&, const std::string&) = 0;

		bool validBalance(const uint32_t aPrice) const { return aPrice <= theCurrBalance; };
		void updateBalance(const uint32_t aPrice) { theCurrBalance -= aPrice; };
		std::string getBaseURL() const {return theBaseURL;};
	
		virtual ~Exchange() = default;
	
};
