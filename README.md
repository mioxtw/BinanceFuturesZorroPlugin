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

## TO-DO List
