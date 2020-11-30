# BinanceFuturesZorroPlugin

**BinanceFuturesZorroPlugin** is a plugin for **[Zorro](https://zorro-project.com/)**, an institutional-grade development tool fro financial research and automatic traiding system.

## Install

To install the plugin, download the [latest release](https://github.com/mioxtw/BinanceFuturesZorroPlugin/releases/download/v0.1.0/BinanceFuturesPlugin_v0.1.0.zip) and place the BinanceFutures.dll file into the **Plugin** folder under Zorro's root path.

## How to Use
* First generate a API Key in Binance website.
* Create accounts.csv and fill your created ID to User table and fill Secret to Pass table.

|Name|Broker|Account|User|Pass|Assets|CCY|Real|NFA|Plugin|Source|
|----|----|----|----|----|----|----|----|----|----|----|
|BinanceFuturesUSDT|Binance Futures	USDT|USDT|---|---|AssetsBinacneFutures|USDT|1|1|	BinanceFutures|
|BinanceFuturesCoin1|Binance Futures BTC|BTC|---|---|AssetsBinacneFutures|BTC|1|1|	BinanceFutures|
|BinanceFuturesCoin2|Binance Futures ETH|ETH|---|---|AssetsBinacneFutures|ETH|1|1|	BinanceFutures|

* Create AssetsBinacneFutures.csv (Binance does not use PIP/PIPCost, but we have to use a mapping value to calculate profit and equity. I'm still testing)

|Name|Price|Spread|RollLong|RollShort|PIP|PIPCost|MarginCost|Leverage|LotAmount|Commission|Symbol|
|----|----|----|----|----|----|----|----|----|----|----|----|
|BTCUSDT|9244.42|0.01|0|0|1|0.001|0|20|0.001|0|BTCUSDT|
|ETHUSDT|458.8|0.01|0|0|1|0.001|0|20|0.001|0|ETHUSDT|
|BTCUSD_PERP|9244.42|0.01|0|0|1|0.000001|0|20|1|0|BTCUSD_PERP|
|ETHUSD_PERP|458.8|0.01|0|0|1|1|0|20|1|0|ETHUSD_PERP|

* In Zorro, select BinanceFutures.

## Features
* Following Zorro Broker API functions has been implemented:

  * BrokerOpen
  * BrokerHTTP
  * BrokerLogin
  * BrokerTime
  * BrokerAccount
    * Only Support Balance, TradeVal and MarginVal
    * If you forget to fill account value in accounts.csv, it uses Asset to know what account balance.
  * BrokerAsset
    * Only Support traded price and traded volume.
    * Traded volume use 1 minute timeframe defaultly. So it needs to set BarPeriod from BrokerCommand.
      ``` C++
      #define SET_TRADEVOLINTERVAL   2004
      brokerCommand(SET_TRADEVOLINTERVAL, BarPeriod);
      ```
  * BrokerHistory2
    * Support download all the histories, so wait patiently for downloading.
    * Store "taker buy volume" value to T6 fVal instead of Spreads.
  * BrokerTrade
  * BrokerBuy2
    * Support MARKET and LIMIT order
  * BrokerSell2
    * Enable it for NFA=0, still need to test.
  * BrokerCommand
    * GET_COMPLIANCE
      * Always return 2(no hedging)
    * SET_DIAGNOSTICS
    * SET_AMOUNT
    * GET_MAXTICKS
    * GET_MAXREQUESTS
      * always return 1 for the moment, because ratelimit is 2400/min, but actually I measure it for a smaller value. 
      * I will implement websocket instead of request price/volume, and this command will be removed in the future.   
    * GET_POSITION
      * return Balance
    * SET_SYMBOL
    * SET_ORDERTYPE
    * GET_BROKERZONE
    * DO_CANCEL
    * SET_HWND
      * I will implement websocket and trigger BrokerAsset to get ticks in the future.
    * SET_COMMENT
      * Currently I use it to set telegram's commends.
    
    * SET_TGTOKEN (private API)
      * Support telegram token
    * SET_TGCHATID (private API)
      * Support telegram chat id
      
      ``` C++      
      brokerCommand(SET_TGTOKEN, "1234567890:AABBCCDDEEFFGGHH");
		  brokerCommand(SET_TGCHATID, 123456789);
		  brokerCommand(SET_COMMENT, "Telegrame Test");
      ```
      
    * SET_USETESTNET (private API)
      * Enable testnet for Binance Futures
    * SET_TRADEVOLINTERVAL (private API)
    
      ``` C++
      #define SET_TRADEVOLINTERVAL   2004
      brokerCommand(SET_TRADEVOLINTERVAL, BarPeriod);
      ```
    * POSTMESSAGE_TEST (private API)
    * RATELIMIT_TEST (private API)


## TO-DO List

  * Add websocket streaming support to lower number of API requests and support high resolution ticks.
  * Please report bugs to fix.
  
  
