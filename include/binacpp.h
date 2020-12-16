
/*
	Author: tensaix2j
	Date  : 2017/10/15
	
	C++ library for Binance API.
*/


#ifndef BINACPP_H
#define BINACPP_H


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>

#include <curl/curl.h>
#include <json/json.h>

#define BINANCE_SPOT_HOST "https://api.binance.com"
#define BINANCE_FUTURES_USDT_HOST "https://fapi.binance.com"
#define BINANCE_FUTURES_COIN_HOST "https://dapi.binance.com"

using namespace std;

class BinaCPP {

	static std::string api_key;
	static std::string secret_key;
	static CURL* curl;


	public:

		

		static void curl_api(std::string &url, std::string &result_json );
		static void curl_api_with_header(std::string &url, std::string &result_json , vector <std::string> &extra_http_header, std::string &post_data, std::string &action );
		static size_t curl_cb( void *content, size_t size, size_t nmemb, std::string *buffer ) ;

		static void init(std::string &api_key, std::string &secret_key);
		static void cleanup();


		// Public API
		static void get_exchangeInfo(std::string& binanceHost, Json::Value &json_result);
		static void get_serverTime(std::string& binanceHost, Json::Value &json_result);

		static void get_allPrices(std::string& binanceHost, Json::Value &json_result, const char* symbol);
		static double get_price(std::string& binanceHost, const char *symbol);
		static double get_volume(std::string& binanceHost, const char* symbol, const char* interval);
		static bool get_dualSidePosition(std::string& binanceHost);

		static void get_allBookTickers(std::string& binanceHost, Json::Value &json_result, const char* symbol);

		static void get_depth(std::string& binanceHost, const char *symbol, int limit, Json::Value &json_result );
		static void get_aggTrades(std::string& binanceHost, const char *symbol, int fromId, time_t startTime, time_t endTime, int limit, Json::Value &json_result );
		static void get_24hr(std::string& binanceHost, const char *symbol, Json::Value &json_result );
		static void get_klines(std::string& binanceHost, const char *symbol, const char *interval, int limit, time_t startTime, time_t endTime,  Json::Value &json_result );


		// API + Secret keys required

		static void get_balance(std::string& binanceHost, Json::Value& json_result);
		static void get_account(std::string& binanceHost, Json::Value &json_result );
		
		static void get_myTrades( 
			std::string& binanceHost,
			const char *symbol, 
			int limit,
			long fromId,
			Json::Value &json_result 
		);
		
		static void get_openOrders(  
			std::string& binanceHost,
			const char *symbol, 
			Json::Value &json_result 
		) ;
		

		static void get_allOrders(   
			std::string& binanceHost,
			const char *symbol, 
			long orderId,
			int limit,
			Json::Value &json_result 
		);


		static void send_order( 
			std::string& binanceHost,
			const char *symbol, 
			const char *side,
			const char *type,
			const char *timeInForce,
			double quantity,
			double price,
			const char *newClientOrderId,
			double stopPrice,
			double icebergQty,
			Json::Value &json_result ) ;


		static void get_order( 
			std::string& binanceHost,
			const char *symbol, 
			long orderId,
			const char *origClientOrderId,
			Json::Value &json_result ); 


		static void cancel_order( 
			std::string& binanceHost,
			const char *symbol, 
			long orderId,
			const char *origClientOrderId,
			const char *newClientOrderId,
			Json::Value &json_result 
		);

		// API key required
		static void start_userDataStream(std::string& binanceHost, Json::Value &json_result );
		static void keep_userDataStream(std::string& binanceHost, const char *listenKey  );
		static void close_userDataStream(std::string& binanceHost, const char *listenKey );


		// WAPI
		static void withdraw( 
			std::string& binanceHost,
			const char *asset,
			const char *address,
			const char *addressTag,
			double amount, 
			const char *name,
			Json::Value &json_result );

		static void get_depositHistory( 
			std::string& binanceHost,
			const char *asset,
			int  status,
			long startTime,
			long endTime, 
			Json::Value &json_result );

		static void get_withdrawHistory( 
			std::string& binanceHost,
			const char *asset,
			int  status,
			long startTime,
			long endTime, 
			Json::Value &json_result ); 

		static void get_depositAddress( 
			std::string& binanceHost,
			const char *asset,
			Json::Value &json_result );


};


#endif
