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

			///订阅/退订行情。
			///@return 订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
			///@param count 要订阅/退订行情的合约个数
			///@param exchage_id 交易所代码
			///@param is_subscribe 是否是订阅
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
			int SubscribeMarketData(array<String^>^ ticker, EXCHANGE_TYPE exchange, bool is_subscribe);

			///订阅/退订行情订单簿。
			///@return 订阅/退订行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
			///@param exchage_id 交易所代码
			///@param is_subscribe 是否是订阅
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(暂不支持)
			int SubscribeOrderBook(array<String^>^ ticker, EXCHANGE_TYPE exchage_id, bool is_subscribe);

			///订阅/退订逐笔行情。
			///@return 订阅/退订逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param exchage_id 交易所代码
			///@param  is_subscribe 是否是订阅
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(暂不支持)
			int SubscribeTickByTick(array<String^>^ ticker, EXCHANGE_TYPE exchage_id, bool is_subscribe);

			///订阅/退订全市场的行情
			///@return 订阅/退订全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param  is_subscribe 是否是订阅
			///@remark 需要与全市场退订行情接口配套使用
			int SubscribeAllMarketData(bool is_subscribe);

			///订阅/退订全市场的行情订单簿
			///@return 订阅/退订全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param  is_subscribe 是否是订阅
			///@remark 需要与全市场退订行情订单簿接口配套使用
			int SubscribeAllOrderBook(bool is_subscribe);

			///订阅/退订全市场的逐笔行情
			///@return 订阅/退订全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param  is_subscribe 是否是订阅
			///@remark 需要与全市场退订逐笔行情接口配套使用
			int SubscribeAllTickByTick(bool is_subscribe);

			///获取合约的最新价格信息
			///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param exchage_id 交易所代码
			int QueryTickersPriceInfo(array<String^>^ ticker, EXCHANGE_TYPE exchage_id);

			///获取所有合约的最新价格信息
			///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
			int QueryAllTickersPriceInfo();

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

		private:
			//delegates
			OnQuoterDisconnectedDelegate ^ OnDisconnected_delegate;
			OnErrorDelegate^ OnError_delegate;
			OnSubMarketDataDelegate^ OnSubMarketData_delegate;
			OnUnSubMarketDataDelegate^ OnUnSubMarketData_delegate;
			OnDepthMarketDataDelegate^ OnDepthMarketData_delegate;
			OnQueryAllTickersDelegate^ OnQueryAllTickers_delegate;

		};
	}
}