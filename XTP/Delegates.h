/*
*/
#pragma once
#include"utils.h"
#include"xtpstruct.h"
namespace XTP
{
	namespace API
	{
		//common delegates

		public delegate void OnErrorDelegate(RspInfoStruct^);

		//quote delegates

		public delegate void OnQuoterDisconnectedDelegate(int);
		public delegate void OnSubMarketDataDelegate(RspInfoStruct^, SpecificTickerStruct^, bool);
		public delegate void OnUnSubMarketDataDelegate(RspInfoStruct^, SpecificTickerStruct^, bool);
		
		public delegate void OnDepthMarketDataDelegate(MarketDataStruct^, array<Int64>^, Int32, Int32, array<Int64>^, Int32, Int32);
		public delegate void OnQueryAllTickersDelegate(RspInfoStruct^, QuoteStaticInfoStruct^, bool);

		///订阅行情订单簿应答
		///@param ticker 详细的合约订阅情况
		///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnSubOrderBookDelegate(SpecificTickerStruct^ ticker, RspInfoStruct^ error_info, bool is_last) ;

		///退订行情订单簿应答
		///@param ticker 详细的合约取消订阅情况
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void OnUnSubOrderBookDelegate(SpecificTickerStruct^  ticker, RspInfoStruct^ error_info, bool is_last) ;

		///行情订单簿通知
		///@param order_book 行情订单簿数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void OnOrderBookDelegate(OrderBook^ order_book) ;

		///订阅逐笔行情应答
		///@param ticker 详细的合约订阅情况
		///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void OnSubTickByTickDelegate(SpecificTickerStruct^  ticker, RspInfoStruct^ error_info, bool is_last) ;

		///退订逐笔行情应答
		///@param ticker 详细的合约取消订阅情况
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void OnUnSubTickByTickDelegate(SpecificTickerStruct^  ticker, RspInfoStruct^ error_info, bool is_last) ;

		///逐笔行情通知
		///@param tbt_data 逐笔行情数据，包括逐笔委托和逐笔成交，此为共用结构体，需要根据type来区分是逐笔委托还是逐笔成交，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void OnTickByTickDelegate(TickByTickStruct^ tbt_data) ;

		///订阅全市场的行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		public delegate void OnSubscribeAllMarketDataDelegate(RspInfoStruct^ error_info) ;

		///退订全市场的行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		public delegate void OnUnSubscribeAllMarketDataDelegate(RspInfoStruct^ error_info) ;

		///订阅全市场的行情订单簿应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		public delegate void OnSubscribeAllOrderBookDelegate(RspInfoStruct^ error_info) ;

		///退订全市场的行情订单簿应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		public delegate void OnUnSubscribeAllOrderBookDelegate(RspInfoStruct^ error_info) ;

		///订阅全市场的逐笔行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		public delegate void OnSubscribeAllTickByTickDelegate(RspInfoStruct^ error_info) ;

		///退订全市场的逐笔行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		public delegate void OnUnSubscribeAllTickByTickDelegate(RspInfoStruct^ error_info) ;


		///查询可交易合约的应答
		///@param ticker_info 可交易合约信息
		///@param error_info 查询可交易合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次查询可交易合约的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		public delegate void OnQueryAllTickers(QuoteStaticInfoStruct^ ticker_info, RspInfoStruct^ error_info, bool is_last) ;

		///查询合约的最新价格信息应答
		///@param ticker_info 合约的最新价格信息
		///@param error_info 查询合约的最新价格信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次查询的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		public delegate void OnQueryTickersPriceInfo(TickerPriceInfo^ ticker_info, RspInfoStruct^ error_info, bool is_last) ;


		//trader delegates
		public delegate void OnTraderDisconnectedDelegate(UInt64, int);
		public delegate void OnOrderDelegate(RspInfoStruct^, OrderInfoStruct^,UInt64);
		public delegate void OnQueryOrderDelegate(RspInfoStruct^, OrderInfoStruct^, int, bool, UInt64);
		public delegate void OnOrderCancelDelegate(RspInfoStruct^, OrderCancelInfoStruct^, UInt64);
		public delegate void OnTradeDelegate(TradeReportStruct^, UInt64);
		public delegate void OnQueryTradeDelegate(RspInfoStruct^, TradeReportStruct^, int, bool, UInt64);
		public delegate void OnQueryAssetDelegate( QueryAssetRspStruct^, RspInfoStruct^, int, bool, UInt64);
		public delegate void OnQueryPositionDelegate(QueryStkPositionStruct^, RspInfoStruct^, int, bool, UInt64);

		///请求查询分级基金信息响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param fund_info 查询到的分级基金情况
		///@param error_info 查询分级基金发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnQueryStructuredFundDelegate(StructuredFundInfo^ fund_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id) ;

		///请求查询资金划拨订单响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param fund_transfer_info 查询到的资金账户情况
		///@param error_info 查询资金账户发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnQueryFundTransferDelegate(FundTransferNotice ^ fund_transfer_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id) ;

		///资金划拨通知
		///@param fund_transfer_info 资金划拨通知的具体信息，用户可以通过fund_transfer_info.serial_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
		///@param error_info 资金划拨订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 当资金划拨订单有状态变化的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的资金划拨通知。
		public delegate void  OnFundTransferDelegate(FundTransferNotice^ fund_transfer_info, RspInfoStruct^ error_info, UInt64 session_id) ;

		///请求查询ETF清单文件的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param etf_info 查询到的ETF清单文件情况
		///@param error_info 查询ETF清单文件发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnQueryETFDelegate(QueryETFBaseRsp^ etf_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id) ;

		///请求查询ETF股票篮的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param etf_component_info 查询到的ETF合约的相关成分股信息
		///@param error_info 查询ETF股票篮发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnQueryETFBasketDelegate(QueryETFComponentRsp^ etf_component_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id) ;

		///请求查询今日新股申购信息列表的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param ipo_info 查询到的今日新股申购的一只股票信息
		///@param error_info 查询今日新股申购信息列表发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnQueryIPOInfoListDelegate(QueryIPOTickerRsp^ ipo_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id) ;

		///请求查询用户新股申购额度信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param quota_info 查询到的用户某个市场的今日新股申购额度信息
		///@param error_info 查查询用户新股申购额度信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		public delegate void  OnQueryIPOQuotaInfoDelegate(QueryIPOQuotaRsp^ quota_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id) ;
	}
}