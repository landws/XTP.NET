#pragma once
#include"utils.h"
#include"../sdk/include/xtp_quote_api.h"
#include"Delegates.h"

namespace XTP
{
	namespace API {
		class XtpQuoteSpi;
	}
}
namespace XTP {
	namespace API
	{
		/// <summary>
		/// 托管类,XTP Marcket Data Adapter
		/// </summary>
		public ref class XtpQuoteAdapter
		{
		private:
			QuoteApi * pUserApi;
			XtpQuoteSpi* pUserSpi;
		private:
			~XtpQuoteAdapter(void);
		public:
			bool IsLogin = false;
		public:
			///创建QuoteApi
			///@param client_id （必须输入）用于区分同一用户的不同客户端，由用户自定义
			///@param log_path （必须输入）存贮订阅信息文件的目录，请设定一个有可写权限的真实存在的路径
			///@param log_level 日志输出级别
			///@remark 如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接
			XtpQuoteAdapter(int client_id, String ^ log_path, LOG_LEVEL log_level);

		public:
			///用户登录请求
			///@return 登录是否成功，“0”表示登录成功，“-1”表示连接服务器出错，此时用户可以调用GetApiLastError()来获取错误代码，“-2”表示已存在连接，不允许重复登录，如果需要重连，请先logout，“-3”表示输入有错误
			///@param ip 服务器ip地址，类似“127.0.0.1”
			///@param port 服务器端口号
			///@param user 登陆用户名
			///@param password 登陆密码
			///@param sock_type “1”代表TCP，“2”代表UDP
			///@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api只能有一个连接
			int Login(String ^ ip, int port, String ^ investor_id, String ^ password, PROTOCOL_TYPE protocal_type);

			String^ GetTradingDay();

			//获取API版本号
			String^ GetApiVersion();

			///获取API的系统错误
			RspInfoStruct^ GetApiLastError();

			//查询所有股票代码
			int QueryAllTickers(EXCHANGE_TYPE exchange);

			///登出请求
			///@return 登出是否成功，“0”表示登出成功，非“0”表示登出出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@remark 此函数为同步阻塞式，不需要异步等待登出，当函数返回即可进行后续操作
			int Logout();


			///设置采用UDP方式连接时的接收缓冲区大小
			///@remark 需要在Login之前调用，默认大小和最小设置均为64MB。此缓存大小单位为MB，请输入2的次方数，例如128MB请输入128。
			void SetUDPBufferSize(UInt32 buff_size);

			///设置心跳检测时间间隔，单位为秒
			///@param interval 心跳检测时间间隔，单位为秒
			///@remark 此函数必须在Login之前调用
			void SetHeartBeatInterval(UInt32 interval);

			///订阅/退订行情。包括股票、指数和期权。
			///@return 订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
			///@param count 要订阅/退订行情的合约个数
			///@param exchange_id 交易所代码
			///@param is_subscribe 是否是订阅
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
			int SubscribeMarketData(array<String^>^ ticker, EXCHANGE_TYPE exchange, bool is_subscribe);

			///订阅/退订行情订单簿。包括股票、指数和期权。
			///@return 订阅/退订行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
			///@param exchange_id 交易所代码
			///@param is_subscribe 是否是订阅
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(暂不支持)
			int SubscribeOrderBook(array<String^>^ ticker, EXCHANGE_TYPE exchange_id, bool is_subscribe);

			///订阅/退订逐笔行情。包括股票、指数和期权。
			///@return 订阅/退订逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param exchange_id 交易所代码
			///@param  is_subscribe 是否是订阅
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(暂不支持)
			int SubscribeTickByTick(array<String^>^ ticker, EXCHANGE_TYPE exchange_id, bool is_subscribe);

			///订阅/退订全市场的行情
			///@return 订阅/退订全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param  is_subscribe 是否是订阅
			///@remark 需要与全市场退订行情接口配套使用
			int SubscribeAllMarketData(EXCHANGE_TYPE exchange_id, bool is_subscribe);

			///订阅/退订全市场的股票行情订单簿
			///@return 订阅/退订全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param  is_subscribe 是否是订阅
			///@remark 需要与全市场退订行情订单簿接口配套使用
			int SubscribeAllOrderBook(EXCHANGE_TYPE exchange_id, bool is_subscribe);

			///订阅/退订全市场的股票逐笔行情
			///@return 订阅/退订全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param  is_subscribe 是否是订阅
			///@remark 需要与全市场退订逐笔行情接口配套使用
			int SubscribeAllTickByTick(EXCHANGE_TYPE exchange_id, bool is_subscribe);

			///获取合约的最新价格信息
			///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param exchange_id 交易所代码
			int QueryTickersPriceInfo(array<String^>^ ticker, EXCHANGE_TYPE exchange_id);

			///获取所有合约的最新价格信息
			///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
			int QueryAllTickersPriceInfo();

			///订阅/退订全市场的期权行情
			///@return 订阅/退订全市期权场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场，XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场
			///@remark 需要与全市场退订期权行情接口配套使用
			virtual int SubscribeAllOptionMarketData(EXCHANGE_TYPE exchange_id, bool is_subscribe) ;

		
			///订阅/退订全市场的期权行情订单簿
			///@return 订阅/退订全市场期权行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场，XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场
			///@remark 需要与全市场退订期权行情订单簿接口配套使用
			virtual int SubscribeAllOptionOrderBook(EXCHANGE_TYPE exchange_id, bool is_subscribe);

			///订阅/退订全市场的期权逐笔行情
			///@return 订阅/退订全市场期权逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场，XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场
			///@remark 需要与全市场退订期权逐笔行情接口配套使用
			virtual int SubscribeAllOptionTickByTick(EXCHANGE_TYPE exchange_id, bool is_subscribe) ;

		public:
			event OnQuoterDisconnectedDelegate^ OnDisconnectedEvent
			{
				void add(OnQuoterDisconnectedDelegate^ handler)
				{
					OnDisconnected_delegate += handler;
				}
				void remove(OnQuoterDisconnectedDelegate^ handler)
				{
					OnDisconnected_delegate -= handler;
				}
				void raise(int reason)
				{
					if (OnDisconnected_delegate)
					{
						OnDisconnected_delegate(reason);
					}
				}
			}
			event OnErrorDelegate^ OnErrorEvent {
				void add(OnErrorDelegate^ handler)
				{
					OnError_delegate += handler;
				}
				void remove(OnErrorDelegate^ handler)
				{
					OnError_delegate -= handler;
				}
				void raise(RspInfoStruct^ rsp)
				{
					if (OnError_delegate)
					{
						OnError_delegate(rsp);
					}
				}
			}
			event OnSubMarketDataDelegate^ OnSubMarketDataEvent {
				void add(OnSubMarketDataDelegate^ handler)
				{
					OnSubMarketData_delegate += handler;
				}
				void remove(OnSubMarketDataDelegate^ handler)
				{
					OnSubMarketData_delegate -= handler;
				}
				void raise(RspInfoStruct^ rsp, SpecificTickerStruct^ tick, bool isLast)
				{
					if (OnSubMarketData_delegate)
					{
						OnSubMarketData_delegate(rsp, tick, isLast);
					}
				}
			}
			event OnUnSubMarketDataDelegate^ OnUnSubMarketDataEvent {
				void add(OnUnSubMarketDataDelegate^ handler)
				{
					OnUnSubMarketData_delegate += handler;
				}
				void remove(OnUnSubMarketDataDelegate^ handler)
				{
					OnUnSubMarketData_delegate -= handler;
				}
				void raise(RspInfoStruct^ rsp, SpecificTickerStruct^ tick, bool isLast)
				{
					if (OnUnSubMarketData_delegate)
					{
						OnUnSubMarketData_delegate(rsp, tick, isLast);
					}
				}
			}
			event OnDepthMarketDataDelegate^ OnDepthMarketDataEvent {
				void add(OnDepthMarketDataDelegate^ handler)
				{
					OnDepthMarketData_delegate += handler;
				}
				void remove(OnDepthMarketDataDelegate^ handler)
				{
					OnDepthMarketData_delegate -= handler;
				}
				void raise(MarketDataStruct^ marketdata, array<Int64>^ bid1Qty, Int32 bid1Count, Int32 bid1MaxCount, array<Int64>^ ask1Qty, Int32 ask1Count, Int32 ask1MaxCount)
				{
					if (OnDepthMarketData_delegate)
					{
						OnDepthMarketData_delegate(marketdata, bid1Qty, bid1Count, bid1MaxCount, ask1Qty, ask1Count, ask1MaxCount);
					}
				}
			}

			event OnQueryAllTickersDelegate^ OnQueryAllTickersEvent {
				void add(OnQueryAllTickersDelegate^ handler)
				{
					OnQueryAllTickers_delegate += handler;
				}
				void remove(OnQueryAllTickersDelegate^ handler)
				{
					OnQueryAllTickers_delegate -= handler;
				}
				void raise(RspInfoStruct^ rsp, QuoteStaticInfoStruct^ data, bool is_last)
				{
					if (OnQueryAllTickers_delegate)
					{
						OnQueryAllTickers_delegate(rsp, data, is_last);
					}
				}
			}
			event OnSubOrderBookDelegate^ OnSubOrderBookEvent{
				void add(OnSubOrderBookDelegate^ handler)
				{
					OnSubOrderBook_delegate += handler;
				}
				void remove(OnSubOrderBookDelegate^ handler)
				{
					OnSubOrderBook_delegate -= handler;
				}
				void raise(SpecificTickerStruct^ ticker, RspInfoStruct^ error_info, bool is_last)
				{
					if (OnSubOrderBook_delegate)
					{
						OnSubOrderBook_delegate(ticker, error_info, is_last);
					}
				}
			}

			event OnUnSubOrderBookDelegate^ OnUnSubOrderBookEvent
			{void add(OnUnSubOrderBookDelegate^ handler)
			{
				OnUnSubOrderBook_delegate += handler;
			}
			void remove(OnUnSubOrderBookDelegate^ handler)
			{
				OnUnSubOrderBook_delegate -= handler;
			}
			void raise(SpecificTickerStruct^  ticker, RspInfoStruct^ error_info, bool is_last)
			{
				if (OnUnSubOrderBook_delegate)
				{
					OnUnSubOrderBook_delegate(ticker, error_info, is_last);
				}
			}}
			event OnOrderBookDelegate^ OnOrderBookEvent{void add(OnOrderBookDelegate^ handler)
			{
				OnOrderBook_delegate += handler;
			}
			void remove(OnOrderBookDelegate^ handler)
			{
				OnOrderBook_delegate -= handler;
			}
			void raise(OrderBook^ order_book)
			{
				if (OnOrderBook_delegate)
				{
					OnOrderBook_delegate(order_book);
				}
			}}

			event OnSubTickByTickDelegate^ OnSubTickByTickEvent{void add(OnSubTickByTickDelegate^ handler)
			{
				OnSubTickByTick_delegate += handler;
			}
			void remove(OnSubTickByTickDelegate^ handler)
			{
				OnSubTickByTick_delegate -= handler;
			}
			void raise(SpecificTickerStruct^  ticker, RspInfoStruct^ error_info, bool is_last)
			{
				if (OnSubTickByTick_delegate)
				{
					OnSubTickByTick_delegate(ticker, error_info, is_last);
				}
			}}
			event OnUnSubTickByTickDelegate^ OnUnSubTickByTickEvent{void add(OnUnSubTickByTickDelegate^ handler)
			{
				OnUnSubTickByTick_delegate += handler;
			}
			void remove(OnUnSubTickByTickDelegate^ handler)
			{
				OnUnSubTickByTick_delegate -= handler;
			}
			void raise(SpecificTickerStruct^  ticker, RspInfoStruct^ error_info, bool is_last)
			{
				if (OnUnSubTickByTick_delegate)
				{
					OnUnSubTickByTick_delegate(ticker, error_info, is_last);
				}
			}}
			event OnTickByTickDelegate^ OnTickByTickEvent{void add(OnTickByTickDelegate^ handler)
			{
				OnTickByTick_delegate += handler;
			}
			void remove(OnTickByTickDelegate^ handler)
			{
				OnTickByTick_delegate -= handler;
			}
			void raise(TickByTickStruct^ tbt_data)
			{
				if (OnTickByTick_delegate)
				{
					OnTickByTick_delegate(tbt_data);
				}
			}}
			event OnSubscribeAllMarketDataDelegate^ OnSubscribeAllMarketDataEvent{void add(OnSubscribeAllMarketDataDelegate^ handler)
			{
				OnSubscribeAllMarketData_delegate += handler;
			}
			void remove(OnSubscribeAllMarketDataDelegate^ handler)
			{
				OnSubscribeAllMarketData_delegate -= handler;
			}
			void raise(RspInfoStruct^ rsp)
			{
				if (OnSubscribeAllMarketData_delegate)
				{
					OnSubscribeAllMarketData_delegate(rsp);
				}
			}}
			event OnUnSubscribeAllMarketDataDelegate^ OnUnSubscribeAllMarketDataEvent
			{void add(OnUnSubscribeAllMarketDataDelegate^ handler)
			{
				OnUnSubscribeAllMarketData_delegate += handler;
			}
			void remove(OnUnSubscribeAllMarketDataDelegate^ handler)
			{
				OnUnSubscribeAllMarketData_delegate -= handler;
			}
			void raise(RspInfoStruct^ rsp)
			{
				if (OnUnSubscribeAllMarketData_delegate)
				{
					OnUnSubscribeAllMarketData_delegate(rsp);
				}
			}}

			event OnSubscribeAllOrderBookDelegate^ OnSubscribeAllOrderBookEvent
			{void add(OnSubscribeAllOrderBookDelegate^ handler)
			{
				OnSubscribeAllOrderBook_delegate += handler;
			}
			void remove(OnSubscribeAllOrderBookDelegate^ handler)
			{
				OnSubscribeAllOrderBook_delegate -= handler;
			}
			void raise(RspInfoStruct^ rsp)
			{
				if (OnSubscribeAllOrderBook_delegate)
				{
					OnSubscribeAllOrderBook_delegate(rsp);
				}
			}}
			event OnUnSubscribeAllOrderBookDelegate^ OnUnSubscribeAllOrderBookEvent
			{void add(OnUnSubscribeAllOrderBookDelegate^ handler)
			{
				OnUnSubscribeAllOrderBook_delegate += handler;
			}
			void remove(OnUnSubscribeAllOrderBookDelegate^ handler)
			{
				OnUnSubscribeAllOrderBook_delegate -= handler;
			}
			void raise(RspInfoStruct^ rsp)
			{
				if (OnUnSubscribeAllOrderBook_delegate)
				{
					OnUnSubscribeAllOrderBook_delegate(rsp);
				}
			}}
			event OnSubscribeAllTickByTickDelegate^ OnSubscribeAllTickByTickEvent
			{void add(OnSubscribeAllTickByTickDelegate^ handler)
			{
				OnSubscribeAllTickByTick_delegate += handler;
			}
			void remove(OnSubscribeAllTickByTickDelegate^ handler)
			{
				OnSubscribeAllTickByTick_delegate -= handler;
			}
			void raise(RspInfoStruct^ rsp)
			{
				if (OnSubscribeAllTickByTick_delegate)
				{
					OnSubscribeAllTickByTick_delegate(rsp);
				}
			}}
			event OnUnSubscribeAllTickByTickDelegate^ OnUnSubscribeAllTickByTickEvent
			{void add(OnUnSubscribeAllTickByTickDelegate^ handler)
			{
				OnUnSubscribeAllTickByTick_delegate += handler;
			}
			void remove(OnUnSubscribeAllTickByTickDelegate^ handler)
			{
				OnUnSubscribeAllTickByTick_delegate -= handler;
			}
			void raise(RspInfoStruct^ rsp)
			{
				if (OnUnSubscribeAllTickByTick_delegate)
				{
					OnUnSubscribeAllTickByTick_delegate(rsp);
				}
			}}

			event OnQueryTickersPriceInfo^ OnQueryTickersPriceInfoEvent{void add(OnQueryTickersPriceInfo^ handler)
			{
				OnQueryTickersPriceInfo_delegate += handler;
			}
			void remove(OnQueryTickersPriceInfo^ handler)
			{
				OnQueryTickersPriceInfo_delegate -= handler;
			}
			void raise(TickerPriceInfo^ ticker_info, RspInfoStruct^ error_info, bool is_last)
			{
				if (OnQueryTickersPriceInfo_delegate)
				{
					OnQueryTickersPriceInfo_delegate(ticker_info, error_info, is_last);
				}
			}}

			

		private:
			//delegates
			OnQuoterDisconnectedDelegate ^ OnDisconnected_delegate;
			OnErrorDelegate^ OnError_delegate;
			OnSubMarketDataDelegate^ OnSubMarketData_delegate;
			OnUnSubMarketDataDelegate^ OnUnSubMarketData_delegate;
			OnDepthMarketDataDelegate^ OnDepthMarketData_delegate;
			OnQueryAllTickersDelegate^ OnQueryAllTickers_delegate;
			
			OnSubOrderBookDelegate^ OnSubOrderBook_delegate;
			OnUnSubOrderBookDelegate^ OnUnSubOrderBook_delegate;
			OnOrderBookDelegate^ OnOrderBook_delegate;
			OnSubTickByTickDelegate^ OnSubTickByTick_delegate;
			OnUnSubTickByTickDelegate^ OnUnSubTickByTick_delegate;
			OnTickByTickDelegate^ OnTickByTick_delegate;
			OnSubscribeAllMarketDataDelegate^ OnSubscribeAllMarketData_delegate;
			OnUnSubscribeAllMarketDataDelegate^ OnUnSubscribeAllMarketData_delegate;
			OnSubscribeAllOrderBookDelegate^ OnSubscribeAllOrderBook_delegate;
			OnUnSubscribeAllOrderBookDelegate^ OnUnSubscribeAllOrderBook_delegate;
			OnSubscribeAllTickByTickDelegate^ OnSubscribeAllTickByTick_delegate;
			OnUnSubscribeAllTickByTickDelegate^ OnUnSubscribeAllTickByTick_delegate;
			OnQueryTickersPriceInfo^ OnQueryTickersPriceInfo_delegate;
		};
	}
}