

/*
	Author: tensaix2j
	Date  : 2017/10/15
	
	C++ library for Binance API.
*/





#include "binacpp.h"
#include "binacpp_logger.h"
#include "binacpp_utils.h"

string BinaCPP::api_key = "";
string BinaCPP::secret_key = "";
CURL* BinaCPP::curl = NULL;
long recvWindow = 50000;

//---------------------------------
void 
BinaCPP::init( string &api_key, string &secret_key ) 
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	BinaCPP::curl = curl_easy_init();
	BinaCPP::api_key = api_key;
	BinaCPP::secret_key = secret_key;
}


void
BinaCPP::cleanup()
{
	curl_easy_cleanup(BinaCPP::curl);
	curl_global_cleanup();
}

//------------------
//GET api/v1/exchangeInfo
//------------------
void 
BinaCPP::get_exchangeInfo(string& binanceHost, Json::Value &json_result)
{
	BinaCPP_logger::write_log( "<BinaCPP::get_exchangeInfo>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
	    url += "/api/v3/exchangeInfo";
	else if(binanceHost == BINANCE_FUTURES_COIN_HOST)
	    url += "/dapi/v1/exchangeInfo";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
	    url += "/fapi/v1/exchangeInfo";


	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_exchangeInfo> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_exchangeInfo> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_exchangeInfo> Failed to get anything." ) ;
	}
}
//------------------
//GET /api/v1/time
//------------
void 
BinaCPP::get_serverTime(string& binanceHost, Json::Value &json_result)
{
	BinaCPP_logger::write_log( "<BinaCPP::get_serverTime>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/time";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/time";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/time";


	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_serverTime> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_serverTime> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_serverTime> Failed to get anything." ) ;
	}
}



//--------------------
// Get Latest price for all symbols.
/*
	GET /api/v1/ticker/allPrices
*/
void 
BinaCPP::get_allPrices(string& binanceHost, Json::Value &json_result, const char* symbol=NULL)
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_allPrices>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/ticker/price";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/ticker/price";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/ticker/price";

	if (symbol) {
		string str_symbol = string_toupper(symbol);
		url += "?symbol=" + str_symbol;
	}

	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			//cout << "Mio: " << str_result << endl;
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_allPrices> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_allPrices> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_allPrices> Failed to get anything." ) ;
	}
}


//----------
// Get Single Pair's Price
double
BinaCPP::get_price(string& binanceHost, const char *symbol )
{
	BinaCPP_logger::write_log( "<BinaCPP::get_price>" ) ;

	double ret = 0.0;
	Json::Value tickers;
	get_allPrices(binanceHost, tickers, symbol);

	ret = atof(tickers["price"].asString().c_str() );
	
	return ret;
}

//----------
// Get Single Pair's Price
double
BinaCPP::get_volume(string& binanceHost, const char* symbol, const char* interval)
{
	BinaCPP_logger::write_log( "<BinaCPP::get_volume>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/klines";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/klines";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/klines";

	string querystring("?symbol=");
	querystring.append( symbol );
	querystring.append( "&interval=" );
	querystring.append( interval );
	querystring.append("&limit=2");
	
	url.append( querystring );
	BinaCPP_logger::write_log( "<BinaCPP::get_volume> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api( url, str_result ) ;
	Json::Value json_result;
	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
	    	json_result.clear();	
			reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_volume> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_volume> Done." ) ;

		cout << json_result << endl;

		__time64_t Time;
		_time64(&Time);
		if (Time * 1000 > json_result[0][6].asInt64() && Time * 1000 < json_result[1][6].asInt64()) {
			return atof(json_result[0][5].asString().c_str());
		}
		else if (Time * 1000 > json_result[0][6].asInt64() && Time * 1000 > json_result[1][6].asInt64()) {
			return atof(json_result[1][5].asString().c_str());
		}
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_volume> Failed to get anything." ) ;
	}

	return 0;
}


bool BinaCPP::get_dualSidePosition(string &binanceHost) {

	BinaCPP_logger::write_log("<BinaCPP::get_positionSide>");

	if (api_key.size() == 0 || secret_key.size() == 0) {
		BinaCPP_logger::write_log("<BinaCPP::get_positionSide> API Key and Secret Key has not been set.");
		return false;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		return false;
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/positionSide/dual?";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/positionSide/dual?";


	string action = "GET";
	string querystring("timestamp=");
	querystring.append(to_string(get_current_ms_epoch()));

	querystring.append("&recvWindow=");
	querystring.append(to_string(recvWindow));

	string signature = hmac_sha256(secret_key.c_str(), querystring.c_str());
	querystring.append("&signature=");
	querystring.append(signature);
	url.append(querystring);
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append(api_key);
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log("<BinaCPP::get_positionSide> url = |%s|", url.c_str());

	string post_data = "";
	string str_result;
	curl_api_with_header(url, str_result, extra_http_header, post_data, action);


	bool ret = false;
	Json::Value json_result;


	if (str_result.size() > 0) {

		try {
			//cout << "Mio: " << str_result << endl;
			Json::Reader reader;
			json_result.clear();
			reader.parse(str_result, json_result);

		}
		catch (exception& e) {
			BinaCPP_logger::write_log("<BinaCPP::get_positionSide> Error ! %s", e.what());
		}
		BinaCPP_logger::write_log("<BinaCPP::get_positionSide> Done.");

		ret = json_result["dualSidePosition"].asBool();

	}
	else {
		BinaCPP_logger::write_log("<BinaCPP::get_positionSide> Failed to get anything.");
	}

	return ret;
}


//--------------------
// Get Best price/qty on the order book for all symbols.
/*
	GET /api/v1/ticker/allBookTickers
	
*/

void 
BinaCPP::get_allBookTickers(string& binanceHost, Json::Value &json_result , const char* symbol=NULL)
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_allBookTickers>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "api/v3/ticker/bookTicker";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "dapi/v1/ticker/bookTicker";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "fapi/v1/ticker/bookTicker";

	if (symbol) {
		string str_symbol = string_toupper(symbol);
		url += "?symbol=" + str_symbol;
	}

	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
	    		reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_allBookTickers> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_allBookTickers> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_allBookTickers> Failed to get anything." ) ;
	}
}


//--------------------
// Get Market Depth
/*
GET /api/v1/depth

Name	Type		Mandatory	Description
symbol	STRING		YES	
limit	INT		NO		Default 100; max 100.

*/

void 
BinaCPP::get_depth(
	string& binanceHost,
	const char *symbol, 
	int limit, 
	Json::Value &json_result ) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_depth>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/depth";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/depth";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/depth";


	string querystring("?symbol=");
	querystring.append( symbol );
	querystring.append("&limit=");
	querystring.append( to_string( limit ) );

	url.append( querystring );
	BinaCPP_logger::write_log( "<BinaCPP::get_depth> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
	    		reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_depth> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_depth> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_depth> Failed to get anything." ) ;
	}
}







//--------------------
// Get Aggregated Trades list
/*

GET /api/v1/aggTrades

Name		Type	Mandatory	Description
symbol		STRING	YES	
fromId		LONG	NO		ID to get aggregate trades from INCLUSIVE.
startTime	LONG	NO		Timestamp in ms to get aggregate trades from INCLUSIVE.
endTime		LONG	NO		Timestamp in ms to get aggregate trades until INCLUSIVE.
limit		INT	NO		Default 500; max 500.
*/

void 
BinaCPP::get_aggTrades( 
	string& binanceHost,
	const char *symbol, 
	int fromId, 
	time_t startTime, 
	time_t endTime, 
	int limit, 
	Json::Value &json_result 
) 
{	
	BinaCPP_logger::write_log( "<BinaCPP::get_aggTrades>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/aggTrades";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/aggTrades";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/aggTrades";


	string querystring("?symbol=");
	querystring.append( symbol );

	
	if ( startTime != 0 && endTime != 0 ) {

		querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );

		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
	
	} else {
		querystring.append("&fromId=");
		querystring.append( to_string( fromId ) );

		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

	url.append( querystring );
	BinaCPP_logger::write_log( "<BinaCPP::get_aggTrades> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api( url, str_result ) ;
	
	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
	    		reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_aggTrades> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_aggTrades> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_aggTrades> Failed to get anything." ) ;
	}
}









//--------------------
// Get 24hr ticker price change statistics
/*
Name	Type	Mandatory	Description
symbol	STRING	YES	
*/
void 
BinaCPP::get_24hr(string& binanceHost, const char *symbol, Json::Value &json_result )
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_24hr>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/ticker/24hr";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/ticker/24hr";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/ticker/24hr";

	string querystring("?symbol=");
	querystring.append( symbol );


	
	url.append( querystring );
	BinaCPP_logger::write_log( "<BinaCPP::get_24hr> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
	    		reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_24hr> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_24hr> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_24hr> Failed to get anything." ) ;
	}
}





//-----------------
/*

Get KLines( Candle stick / OHLC )
GET /api/v1/klines

Name		Type	Mandatory	Description
symbol		STRING	YES	
interval	ENUM	YES	
limit		INT		NO	Default 500; max 500.
startTime	LONG	NO	
endTime		LONG	NO	

*/

void 
BinaCPP::get_klines(
	string& binanceHost,
	const char *symbol, 
	const char *interval, 
	int limit, 
	time_t startTime, 
	time_t endTime,  
	Json::Value &json_result ) 
{		

	BinaCPP_logger::write_log( "<BinaCPP::get_klines>" ) ;

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/klines";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/klines";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/klines";

	string querystring("?symbol=");
	querystring.append( symbol );

	querystring.append( "&interval=" );
	querystring.append( interval );

	if ( startTime > 0 && endTime > 0 ) {

		querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );

		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
	
	} else if ( limit > 0 ) {
		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

	
	url.append( querystring );
	BinaCPP_logger::write_log( "<BinaCPP::get_klines> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
	    		json_result.clear();	
			reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_klines> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_klines> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_klines> Failed to get anything." ) ;
	}
}














//--------------------
// Get current account information. (SIGNED)
/*
GET /api/v3/account

Parameters:
Name		Type	Mandatory	Description
recvWindow	LONG	NO	
timestamp	LONG	YES
*/


void 
BinaCPP::get_account(string& binanceHost, Json::Value &json_result )
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_account>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_account> API Key and Secret Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/account?";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/account?";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v2/account?";


	string action = "GET";



	string querystring("timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow ) );

	string signature =  hmac_sha256( secret_key.c_str() , querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log( "<BinaCPP::get_account> url = |%s|" , url.c_str() ) ;
	
	string post_data = "";
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header , post_data , action ) ;


	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_account> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_account> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_account> Failed to get anything." ) ;
	}

	BinaCPP_logger::write_log( "<BinaCPP::get_account> Done.\n" ) ;

}








//--------------------
// Get trades for a specific account and symbol. (SIGNED)
/*
GET /api/v3/myTrades
Name		Type	Mandatory	Description
symbol		STRING	YES	
limit		INT		NO	Default 500; max 500.
fromId		LONG	NO	TradeId to fetch from. Default gets most recent trades.
recvWindow	LONG	NO	
timestamp	LONG	YES
	
*/


void 
BinaCPP::get_myTrades(
	string& binanceHost,
	const char *symbol,
	int limit,
	long fromId,
	Json::Value &json_result ) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_myTrades>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_myTrades> API Key and Secret Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/myTrades?";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/userTrades?";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/userTrades?";

	string querystring("symbol=");
	querystring.append( symbol );

	if ( limit > 0 ) {
		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

	if ( fromId > 0 ) {
		querystring.append("&fromId=");
		querystring.append( to_string( fromId ) );
	}

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow ) );


	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str() , querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log( "<BinaCPP::get_myTrades> url = |%s|" , url.c_str() ) ;
	
	string action = "GET";
	string post_data = "";

	string str_result;
	curl_api_with_header( url, str_result , extra_http_header , post_data , action ) ;


	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_myTrades> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_myTrades> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_myTrades> Failed to get anything." ) ;
	}

	BinaCPP_logger::write_log( "<BinaCPP::get_myTrades> Done.\n" ) ;

}











//--------------------
// Open Orders (SIGNED)
/*
GET /api/v3/openOrders

Name		Type	Mandatory	Description
symbol		STRING	YES	
recvWindow	LONG	NO	
timestamp	LONG	YES	
*/

void 
BinaCPP::get_openOrders( 
	string& binanceHost,
	const char *symbol, 
	Json::Value &json_result 
) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_openOrders>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_openOrders> API Key and Secret Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/openOrders?";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/openOrders?";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/openOrders?";

	string querystring("symbol=");
	querystring.append( symbol );

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow) );


	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );


	string signature =  hmac_sha256( secret_key.c_str(), querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	
	string action = "GET";
	string post_data ="";
		
	BinaCPP_logger::write_log( "<BinaCPP::get_openOrders> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data , action ) ;


	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_openOrders> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_openOrders> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_openOrders> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::get_openOrders> Done.\n" ) ;

}

















//--------------------
// All Orders (SIGNED)
/*
GET /api/v3/allOrders

Name		Type	Mandatory	Description
symbol		STRING	YES	
orderId		LONG	NO	
limit		INT		NO		Default 500; max 500.
recvWindow	LONG	NO	
timestamp	LONG	YES	
*/

void 
BinaCPP::get_allOrders( 
	string& binanceHost,
	const char *symbol, 
	long orderId,
	int limit,
	Json::Value &json_result 
) 

{	

	BinaCPP_logger::write_log( "<BinaCPP::get_allOrders>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_allOrders> API Key and Secret Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/allOrders?";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/allOrders?";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/allOrders?";


	string querystring("symbol=");
	querystring.append( symbol );

	if ( orderId > 0 ) {
		querystring.append("&orderId=");
		querystring.append( to_string( orderId ) );
	}

	if ( limit > 0 ) {
		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow ) );


	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	
	string action = "GET";
	string post_data ="";
		
	BinaCPP_logger::write_log( "<BinaCPP::get_allOrders> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data , action ) ;


	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_allOrders> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_allOrders> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_allOrders> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::get_allOrders> Done.\n" ) ;

}



//------------
/*
send order (SIGNED)
POST /api/v3/order

Name				Type		Mandatory	Description
symbol				STRING		YES	
side				ENUM		YES	
type				ENUM		YES	
timeInForce			ENUM		YES	
quantity			DECIMAL		YES	
price				DECIMAL		YES	
newClientOrderId		STRING		NO		A unique id for the order. Automatically generated by default.
stopPrice			DECIMAL		NO		Used with STOP orders
icebergQty			DECIMAL		NO		Used with icebergOrders
recvWindow			LONG		NO	
timestamp			LONG		YES	
*/

void 
BinaCPP::send_order( 
	string& binanceHost,
	const char *symbol, 
	const char *side,
	const char *type,
	const char *timeInForce,
	double quantity,
	double price,
	const char *newClientOrderId,
	double stopPrice,
	double icebergQty,
	Json::Value &json_result ) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::send_order>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::send_order> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/order";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/order";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/order";

	string action = "POST";
	
	string post_data("symbol=");
	post_data.append( symbol );
	
	post_data.append("&side=");
	post_data.append( side );

	post_data.append("&type=");
	post_data.append( type );
	if(strcmp(type, "MARKET"))//type != MARKET
	{
		post_data.append("&timeInForce=");
		post_data.append( timeInForce );
		post_data.append("&price=");
		post_data.append( to_string( price) );
	}
	post_data.append("&quantity=");
	post_data.append( to_string( quantity) );
	

	if ( strlen( newClientOrderId ) > 0 ) {
		post_data.append("&newClientOrderId=");
		post_data.append( newClientOrderId );
	}

	if ( stopPrice > 0.0 ) {
		post_data.append("&stopPrice=");
		post_data.append( to_string( stopPrice ) );
	}

	if ( icebergQty > 0.0 ) {
		post_data.append("&icebergQty=");
		post_data.append( to_string( icebergQty ) );
	}

	post_data.append("&recvWindow=");
	post_data.append( to_string( recvWindow) );

	post_data.append("&timestamp=");
	post_data.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), post_data.c_str() );
	post_data.append( "&signature=");
	post_data.append( signature );


	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log( "<BinaCPP::send_order> url = |%s|, post_data = |%s|" , url.c_str(), post_data.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data, action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::send_order> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::send_order> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::send_order> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::send_order> Done.\n" ) ;

}


//------------------
/*
// get order (SIGNED)
GET /api/v3/order

Name				Type	Mandatory	Description
symbol				STRING	YES	
orderId				LONG	NO	
origClientOrderId		STRING	NO	
recvWindow			LONG	NO	
timestamp			LONG	YES	
*/

void 
BinaCPP::get_order( 
	string& binanceHost,
	const char *symbol, 
	long orderId,
	const char *origClientOrderId,
	Json::Value &json_result ) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::get_order>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_order> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/order?";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/order?";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/order?";


	string action = "GET";
	

	string querystring("symbol=");
	querystring.append( symbol );
	
	if ( orderId > 0 ) {
		querystring.append("&orderId=");
		querystring.append( to_string( orderId ) );
	}

	if ( strlen( origClientOrderId ) > 0 ) {
		querystring.append("&origClientOrderId=");
		querystring.append( origClientOrderId );
	}

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow) );

	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string post_data = "";
	
	BinaCPP_logger::write_log( "<BinaCPP::get_order> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_order> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_order> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_order> Failed to get anything." ) ;
	}

	
	BinaCPP_logger::write_log( "<BinaCPP::get_order> Done.\n" ) ;

}








//------------
/*
DELETE /api/v3/order
cancel order (SIGNED)

symbol				STRING	YES	
orderId				LONG	NO	
origClientOrderId		STRING	NO	
newClientOrderId		STRING	NO	Used to uniquely identify this cancel. Automatically generated by default.
recvWindow			LONG	NO	
timestamp			LONG	YES	

*/

void 
BinaCPP::cancel_order(
	string& binanceHost,
	const char *symbol, 
	long orderId,
	const char *origClientOrderId,
	const char *newClientOrderId,
	Json::Value &json_result ) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::send_order>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::send_order> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/order";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/order";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/order";


	string action = "DELETE";
	
	string post_data("symbol=");
	post_data.append( symbol );

	if ( orderId > 0 ) {	
		post_data.append("&orderId=");
		post_data.append( to_string( orderId ) );
	}

	if ( strlen( origClientOrderId ) > 0 ) {
		post_data.append("&origClientOrderId=");
		post_data.append( origClientOrderId );
	}

	if ( strlen( newClientOrderId ) > 0 ) {
		post_data.append("&newClientOrderId=");
		post_data.append( newClientOrderId );
	}

	post_data.append("&recvWindow=");
	post_data.append( to_string( recvWindow) );


	post_data.append("&timestamp=");
	post_data.append( to_string( get_current_ms_epoch() ) );


	string signature =  hmac_sha256( secret_key.c_str(), post_data.c_str() );
	post_data.append( "&signature=");
	post_data.append( signature );


	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log( "<BinaCPP::send_order> url = |%s|, post_data = |%s|" , url.c_str(), post_data.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data, action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::send_order> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::send_order> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::send_order> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::send_order> Done.\n" ) ;

}





//--------------------
//Start user data stream (API-KEY)

void 
BinaCPP::start_userDataStream(string& binanceHost, Json::Value &json_result )
{	
	BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream>" ) ;

	if ( api_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream> API Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/userDataStream";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/listenKey";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/listenKey";

	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	

	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream> url = |%s|" , url.c_str() ) ;
	
	string action = "POST";
	string post_data = "";

	string str_result;
	curl_api_with_header( url, str_result , extra_http_header , post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream> Failed to get anything." ) ;
	}

	BinaCPP_logger::write_log( "<BinaCPP::start_userDataStream> Done.\n" ) ;

}









//--------------------
//Keepalive user data stream (API-KEY)
void 
BinaCPP::keep_userDataStream(string& binanceHost, const char *listenKey )
{	
	BinaCPP_logger::write_log( "<BinaCPP::keep_userDataStream>" ) ;

	if ( api_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::keep_userDataStream> API Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/userDataStream";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/listenKey";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/listenKey";

	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	

	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string action = "PUT";
	string post_data("listenKey=");
	post_data.append( listenKey );

	BinaCPP_logger::write_log( "<BinaCPP::keep_userDataStream> url = |%s|, post_data = |%s|" , url.c_str() , post_data.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header , post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		BinaCPP_logger::write_log( "<BinaCPP::keep_userDataStream> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::keep_userDataStream> Failed to get anything." ) ;
	}

	BinaCPP_logger::write_log( "<BinaCPP::keep_userDataStream> Done.\n" ) ;

}





//--------------------
//Keepalive user data stream (API-KEY)
void 
BinaCPP::close_userDataStream(string& binanceHost, const char *listenKey )
{	
	BinaCPP_logger::write_log( "<BinaCPP::close_userDataStream>" ) ;

	if ( api_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::close_userDataStream> API Key has not been set." ) ;
		return ;
	}


	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/api/v3/userDataStream";
	else if (binanceHost == BINANCE_FUTURES_COIN_HOST)
		url += "/dapi/v1/listenKey";
	else if (binanceHost == BINANCE_FUTURES_USDT_HOST)
		url += "/fapi/v1/listenKey";

	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	

	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string action = "DELETE";
	string post_data("listenKey=");
	post_data.append( listenKey );

	BinaCPP_logger::write_log( "<BinaCPP::close_userDataStream> url = |%s|, post_data = |%s|" , url.c_str() , post_data.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header , post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		BinaCPP_logger::write_log( "<BinaCPP::close_userDataStream> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::close_userDataStream> Failed to get anything." ) ;
	}

	BinaCPP_logger::write_log( "<BinaCPP::close_userDataStream> Done.\n" ) ;

}




//-------------
/*
Submit a withdraw request.
 
POST /wapi/v3/withdraw.html

Name		Type	Mandatory	Description
asset		STRING	YES	
address		STRING	YES	
addressTag	STRING	NO	Secondary address identifier for coins like XRP,XMR etc.
amount		DECIMAL	YES	
name		STRING	NO	Description of the address
recvWindow	LONG	NO	
timestamp	LONG	YES

*/
void 
BinaCPP::withdraw(
	string& binanceHost,
	const char *asset,
	const char *address,
	const char *addressTag,
	double amount, 
	const char *name,
	Json::Value &json_result ) 
{	

	BinaCPP_logger::write_log( "<BinaCPP::withdraw>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::send_order> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/wapi/v3/withdraw.html";
	else
		return;

	string action = "POST";
	
	string post_data("asset=");
	post_data.append( asset );
	
	post_data.append("&address=" );
	post_data.append( address );

	if ( strlen(addressTag) > 0 ) {
		post_data.append( "&addressTag=");
		post_data.append( addressTag );
	}

	post_data.append( "&amount=");
	post_data.append( to_string( amount ));	

	if ( strlen( name ) > 0 ) {
		post_data.append("&name=");
		post_data.append(name);
	}

	post_data.append("&recvWindow=");
	post_data.append( to_string( recvWindow) );

	post_data.append("&timestamp=");
	post_data.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), post_data.c_str() );
	post_data.append( "&signature=");
	post_data.append( signature );


	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	BinaCPP_logger::write_log( "<BinaCPP::withdraw> url = |%s|, post_data = |%s|" , url.c_str(), post_data.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data, action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::withdraw> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::withdraw> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::withdraw> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::withdraw> Done.\n" ) ;

}



/*
-GET /wapi/v3/depositHistory.html
Fetch deposit history.

Parameters:

Name		Type	Mandatory	Description
asset		STRING	NO	
status		INT	NO	0(0:pending,1:success)
startTime	LONG	NO	
endTime	LONG		NO	
recvWindow	LONG	NO	
timestamp	LONG	YES	
*/
void 
BinaCPP::get_depositHistory( 
	string& binanceHost,
	const char *asset,
	int  status,
	long startTime,
	long endTime, 
	Json::Value &json_result ) 
{	


	BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/wapi/v3/depositHistory.html?";
	else
		return;


	string action = "GET";
	
	string querystring("");

	if ( strlen( asset ) > 0 ) {
		querystring.append( "asset=" );
		querystring.append( asset );
	}

	if ( status > 0 ) {
		querystring.append("&status=");
		querystring.append( to_string( status ) );
	}

	if ( startTime > 0 ) {
		querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );
	}

	if ( endTime > 0 ) {
		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
	}

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow) );

	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string post_data = "";
	
	BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::get_depostHistory> Done.\n" ) ;
}










//---------

/*
-GET /wapi/v3/withdrawHistory.html
Fetch withdraw history.

Parameters:

Name		Type	Mandatory	Description
asset		STRING	NO	
status		INT	NO	0(0:Email Sent,1:Cancelled 2:Awaiting Approval 3:Rejected 4:Processing 5:Failure 6Completed)
startTime	LONG	NO	
endTime	LONG		NO	
recvWindow	LONG	NO	
timestamp	LONG	YES	
*/

void 
BinaCPP::get_withdrawHistory( 
	string& binanceHost,
	const char *asset,
	int  status,
	long startTime,
	long endTime, 
	Json::Value &json_result ) 
{	


	BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/wapi/v3/withdrawHistory.html?";
	else
		return;


	string action = "GET";
	
	string querystring("");

	if ( strlen( asset ) > 0 ) {
		querystring.append( "asset=" );
		querystring.append( asset );
	}

	if ( status > 0 ) {
		querystring.append("&status=");
		querystring.append( to_string( status ) );
	}

	if ( startTime > 0 ) {
		querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );
	}

	if ( endTime > 0 ) {
		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
	}

	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow) );

	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string post_data = "";
	
	BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::get_withdrawHistory> Done.\n" ) ;
}






//--------------
/*
-GET /wapi/v3/depositAddress.html
Fetch deposit address.

Parameters:

Name		Type	Mandatory	Description
asset		STRING	YES	
recvWindow	LONG	NO	
timestamp	LONG	YES	

*/


void 
BinaCPP::get_depositAddress( 
	string& binanceHost,
	const char *asset,
	Json::Value &json_result ) 
{	


	BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress>" ) ;

	if ( api_key.size() == 0 || secret_key.size() == 0 ) {
		BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress> API Key and Secret Key has not been set." ) ;
		return ;
	}

	string url(binanceHost);
	if (binanceHost == BINANCE_SPOT_HOST)
		url += "/wapi/v3/depositAddress.html?";
	else
		return;


	string action = "GET";
	
	string querystring("asset=");
	querystring.append( asset );
	
	querystring.append("&recvWindow=");
	querystring.append( to_string( recvWindow) );

	querystring.append("&timestamp=");
	querystring.append( to_string( get_current_ms_epoch() ) );

	string signature =  hmac_sha256( secret_key.c_str(), querystring.c_str() );
	querystring.append( "&signature=");
	querystring.append( signature );

	url.append( querystring );
	
	vector <string> extra_http_header;
	string header_chunk("X-MBX-APIKEY: ");
	header_chunk.append( api_key );
	extra_http_header.push_back(header_chunk);

	string post_data = "";
	
	BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api_with_header( url, str_result , extra_http_header, post_data , action ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
			json_result.clear();	
			reader.parse( str_result , json_result );
	    		
	    	} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress> Failed to get anything." ) ;
	}
	
	BinaCPP_logger::write_log( "<BinaCPP::get_depositAddress> Done.\n" ) ;
}












//-----------------
// Curl's callback
size_t 
BinaCPP::curl_cb( void *content, size_t size, size_t nmemb, std::string *buffer ) 
{	
	BinaCPP_logger::write_log( "<BinaCPP::curl_cb> " ) ;

	buffer->append((char*)content, size*nmemb);

	BinaCPP_logger::write_log( "<BinaCPP::curl_cb> done" ) ;
	return size*nmemb;
}






//--------------------------------------------------
void 
BinaCPP::curl_api( string &url, string &result_json ) {
	vector <string> v;
	string action = "GET";
	string post_data = "";
	curl_api_with_header( url , result_json , v, post_data , action );	
} 



//--------------------
// Do the curl
void 
BinaCPP::curl_api_with_header( string &url, string &str_result, vector <string> &extra_http_header , string &post_data , string &action ) 
{

	BinaCPP_logger::write_log( "<BinaCPP::curl_api>" ) ;

	CURLcode res;

	if( BinaCPP::curl ) {

		curl_easy_setopt(BinaCPP::curl, CURLOPT_URL, url.c_str() );
		curl_easy_setopt(BinaCPP::curl, CURLOPT_WRITEFUNCTION, BinaCPP::curl_cb);
		curl_easy_setopt(BinaCPP::curl, CURLOPT_WRITEDATA, &str_result );
		curl_easy_setopt(BinaCPP::curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(BinaCPP::curl, CURLOPT_ENCODING, "gzip");



		if ( extra_http_header.size() > 0 ) {
			
			struct curl_slist *chunk = NULL;
			for ( int i = 0 ; i < extra_http_header.size() ;i++ ) {
				chunk = curl_slist_append(chunk, extra_http_header[i].c_str() );
			}
			curl_easy_setopt(BinaCPP::curl, CURLOPT_HTTPHEADER, chunk);
	
		}

		if ( post_data.size() > 0 || action == "POST" || action == "PUT" || action == "DELETE" ) {

			if ( action == "PUT" || action == "DELETE" ) {
				curl_easy_setopt(BinaCPP::curl, CURLOPT_CUSTOMREQUEST, action.c_str() );
			}
			curl_easy_setopt(BinaCPP::curl, CURLOPT_POSTFIELDS, post_data.c_str() );
 		}

		res = curl_easy_perform(BinaCPP::curl);

		/* Check for errors */ 
		if ( res != CURLE_OK ) {
			BinaCPP_logger::write_log( "<BinaCPP::curl_api> curl_easy_perform() failed: %s" , curl_easy_strerror(res) ) ;
		} 	

	}

	BinaCPP_logger::write_log( "<BinaCPP::curl_api> done" ) ;

}



