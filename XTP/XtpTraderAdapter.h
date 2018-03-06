#pragma once
#include"utils.h"
#include"../sdk/include/xtp_trader_api.h"
#include"Delegates.h"

namespace XTP
{
	namespace API {
		class XtpTraderSpi;
	}
}
namespace XTP
{
	namespace API
	{
		public ref class XtpTraderAdapter
		{
		private:
			~XtpTraderAdapter(void);
		private:
			TraderApi * pTdApi;

			XtpTraderSpi* pTdSpi;
		public:
			bool IsLogin = false;
		public:
			XtpTraderAdapter(int client_id, String^ account_key, String ^ log_path, TE_RESUME_TYPE resume_type);

		public:
			UInt64 Login(String ^ ip, int port, String ^ investor_id, String ^ password, PROTOCOL_TYPE protocal_type);

			///通过报单在xtp系统中的ID获取下单的客户端id
			///@return 返回客户端id，可以用此方法过滤自己下的订单
			///@param order_xtp_id 报单在xtp系统中的ID
			///@remark 由于系统允许同一用户在不同客户端上登录操作，每个客户端通过不同的client_id进行区分
			UInt64 GetClientIDByXTPID(UInt64 order_xtp_id);

			///通过报单在xtp系统中的ID获取相关资金账户名
			///@return 返回资金账户名
			///@param order_xtp_id 报单在xtp系统中的ID
			///@remark 只有资金账户登录成功后,才能得到正确的信息
			String^ GetAccountByXTPID(UInt64 order_xtp_id);

			void SubscribePublicTopic(TE_RESUME_TYPE resume_type);

			///设置心跳检测时间间隔，单位为秒
			///@param interval 心跳检测时间间隔，单位为秒
			///@remark 此函数必须在Login之前调用
			void SetHeartBeatInterval(UInt32 interval);

			String^ GetTradingDay();

			//获取API版本号
			String^ GetApiVersion();
			///获取API的系统错误
			RspInfoStruct^ GetApiLastError();

			///登出请求
			///@return 登出是否成功，“0”表示登出成功，“-1”表示登出失败
			///@param session_id 资金账户对应的session_id,登录时得到
			int Logout(UInt64 session_id);

			UInt64 InsertOrder(OrderInsertInfo ^order, UInt64 session_id);

			///报单操作请求
			///@return 撤单在XTP系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order_xtp_id 需要撤销的委托单在XTP系统中的ID
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 如果撤单成功，会在报单响应函数OnOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
			UInt64 CancelOrder(UInt64 order_xtp_id, UInt64 session_id);

			///根据报单ID请求查询报单
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryOrderByXTPID(UInt64 order_xtp_id, UInt64 session_id, int request_id);

			///请求查询报单
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应
			int QueryOrders(QueryOrderReq ^query_param, UInt64 session_id, int request_id);

			///根据委托编号请求查询相关成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的委托编号，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
			int QueryTradesByXTPID(UInt64 order_xtp_id, UInt64 session_id, int request_id);

			///请求查询已成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的成交回报筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应
			int QueryTrades(QueryTraderReq ^query_param, UInt64 session_id, int request_id);

			///请求查询投资者持仓
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param ticker 需要查询的持仓合约代码，可以为空，如果不为空，请不带空格，并以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法如果用户提供了合约代码，则会查询此合约的持仓信息，如果合约代码为空，则默认查询所有持仓信息
			int QueryPosition(String^ ticker, UInt64 session_id, int request_id);

			///请求查询资产
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryAsset(UInt64 session_id, int request_id);

		public:
			//events//
			event OnTraderDisconnectedDelegate^ OnDisconnectedEvent
			{
				void add(OnTraderDisconnectedDelegate^ hanlder)
				{
					OnDisconnected_delegate += hanlder;
				}
				void remove(OnTraderDisconnectedDelegate^ hanlder)
				{
					OnDisconnected_delegate -= hanlder;
				}
				void raise(UInt64 session_id, Int32 reason)
				{
					if (OnDisconnected_delegate)
					{
						OnDisconnected_delegate(session_id, reason);
					}
				}
			}
			event OnErrorDelegate^ OnErrorEvent
			{
				void add(OnErrorDelegate^ hanlder)
				{
					OnError_delegate += hanlder;
				}
				void remove(OnErrorDelegate^ hanlder)
				{
					OnError_delegate -= hanlder;
				}
				void raise(RspInfoStruct^ rsp)
				{
					if (OnError_delegate)
					{
						OnError_delegate(rsp);
					}
				}
			}
			event OnOrderDelegate^ OnOrderEvent
			{
				void add(OnOrderDelegate^ hanlder)
				{
					OnOrder_delegate += hanlder;
				}
				void remove(OnOrderDelegate^ hanlder)
				{
					OnOrder_delegate -= hanlder;
				}
				void raise(RspInfoStruct^ rsp, OrderInfoStruct^ order, UInt64 session_id)
				{
					if (OnOrder_delegate)
					{
						OnOrder_delegate(rsp, order, session_id);
					}
				}
			}
			event OnQueryOrderDelegate^ OnQueryOrderEvent
			{
				void add(OnQueryOrderDelegate^ hanlder)
				{
					OnQueryOrder_delegate += hanlder;
				}
				void remove(OnQueryOrderDelegate^ hanlder)
				{
					OnQueryOrder_delegate -= hanlder;
				}
				void raise(RspInfoStruct^ rsp, OrderInfoStruct^ order, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryOrder_delegate)
					{
						OnQueryOrder_delegate(rsp, order, request_id, is_last, session_id);
					}
				}
			}
			event OnOrderCancelDelegate^ OnOrderCancelEvent
			{
				void add(OnOrderCancelDelegate^ hanlder)
				{
					OnOrderCancel_delegate += hanlder;
				}
				void remove(OnOrderCancelDelegate^ hanlder)
				{
					OnOrderCancel_delegate -= hanlder;
				}
				void raise(RspInfoStruct^ rsp, OrderCancelInfoStruct^ order_cancel, UInt64 session_id)
				{
					if (OnOrderCancel_delegate)
					{
						OnOrderCancel_delegate(rsp, order_cancel, session_id);
					}
				}
			}
			event OnTradeDelegate^ OnTradeEvent
			{
				void add(OnTradeDelegate^ hanlder)
				{
					OnTrade_delegate += hanlder;
				}
				void remove(OnTradeDelegate^ hanlder)
				{
					OnTrade_delegate -= hanlder;
				}
				void raise(TradeReportStruct^ trade_report, UInt64 session_id)
				{
					if (OnTrade_delegate)
					{
						OnTrade_delegate(trade_report, session_id);
					}
				}
			}
			event OnQueryTradeDelegate^ OnQueryTradeEvent
			{
				void add(OnQueryTradeDelegate^ hanlder)
				{
					OnQueryTrade_delegate += hanlder;
				}
				void remove(OnQueryTradeDelegate^ hanlder)
				{
					OnQueryTrade_delegate -= hanlder;
				}
				void raise(RspInfoStruct^ rsp, TradeReportStruct^ trade_report, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryTrade_delegate)
					{
						OnQueryTrade_delegate(rsp, trade_report, request_id, is_last, session_id);
					}
				}
			}
			event OnQueryAssetDelegate^ OnQueryAssetEvent
			{
				void add(OnQueryAssetDelegate^ hanlder)
				{
					OnQueryAsset_delegate += hanlder;
				}
				void remove(OnQueryAssetDelegate^ hanlder)
				{
					OnQueryAsset_delegate -= hanlder;
				}
				void raise( QueryAssetRspStruct^ asset, RspInfoStruct^ rsp, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryAsset_delegate)
					{
						OnQueryAsset_delegate(asset, rsp,  request_id, is_last, session_id);
					}
				}
			}
			event OnQueryPositionDelegate ^OnQueryPositionEvent
			{
				void add(OnQueryPositionDelegate^ hanlder)
				{
					OnQueryPosition_delegate += hanlder;
				}
				void remove(OnQueryPositionDelegate^ hanlder)
				{
					OnQueryPosition_delegate -= hanlder;
				}
				void raise( QueryStkPositionStruct^ pos, RspInfoStruct^ rsp, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryPosition_delegate)
					{
						OnQueryPosition_delegate(pos, rsp, request_id, is_last, session_id);
					}
				}
			}
		private:
			//delegates
			OnTraderDisconnectedDelegate ^ OnDisconnected_delegate;
			OnErrorDelegate^ OnError_delegate;
			OnOrderDelegate^ OnOrder_delegate;
			OnQueryOrderDelegate^ OnQueryOrder_delegate;
			OnOrderCancelDelegate^ OnOrderCancel_delegate;
			OnTradeDelegate^ OnTrade_delegate;
			OnQueryTradeDelegate^ OnQueryTrade_delegate;
			OnQueryAssetDelegate^ OnQueryAsset_delegate;
			OnQueryPositionDelegate ^OnQueryPosition_delegate;
		};
	}
}