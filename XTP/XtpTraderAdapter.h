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
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应..此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
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
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应.此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
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

			///请求查询分级基金
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的分级基金筛选条件，其中母基金代码可以为空，则默认所有存在的母基金，如果不为空，请不带空格，并以'\0'结尾，其中交易市场不能为空
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
			int QueryStructuredFund(QueryStructuredFundInfoReq^  query_param, UInt64 session_id, int request_id);

			///资金划拨请求
			///@return 资金划拨订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的serial_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param fund_transfer 资金划拨的请求信息
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 此函数支持一号两中心节点之间的资金划拨，注意资金划拨的方向。
			UInt64  FundTransfer(FundTransferReq^ fund_transfer, UInt64 session_id);

			///请求查询资金划拨
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的资金划拨订单筛选条件，其中serial_id可以为0，则默认所有资金划拨订单，如果不为0，则请求特定的资金划拨订单
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryFundTransfer(QueryFundTransferLogReq^ query_param, UInt64 session_id, int request_id);

			///请求查询ETF清单文件
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的ETF清单文件的筛选条件，其中合约代码可以为空，则默认所有存在的ETF合约代码，market字段也可以为初始值，则默认所有市场的ETF合约
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryETF(QueryETFBaseReq^ query_param, UInt64 session_id, int request_id);

			///请求查询ETF股票篮
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询股票篮的的ETF合约，其中合约代码不可以为空，market字段也必须指定
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryETFTickerBasket(QueryETFComponentReq^ query_param, UInt64 session_id, int request_id);

			///请求查询今日新股申购信息列表
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryIPOInfoList(UInt64 session_id, int request_id);

			///请求查询用户新股申购额度信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryIPOQuotaInfo(UInt64 session_id, int request_id);

			///请求查询期权合约
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的期权合约的筛选条件，可以为NULL（为NULL表示查询所有的期权合约）
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryOptionAuctionInfo(QueryOptionAuctionInfoReq^ query_param, UInt64 session_id, int request_id);


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
				void raise(QueryAssetRsp^ asset, RspInfoStruct^ rsp, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryAsset_delegate)
					{
						OnQueryAsset_delegate(asset, rsp, request_id, is_last, session_id);
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
				void raise(QueryStkPositionRsp^ pos, RspInfoStruct^ rsp, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryPosition_delegate)
					{
						OnQueryPosition_delegate(pos, rsp, request_id, is_last, session_id);
					}
				}
			}
			///
			event OnQueryStructuredFundDelegate ^OnQueryStructuredFundEvent
			{
				void add(OnQueryStructuredFundDelegate^ hanlder)
				{
					OnQueryStructuredFund_delegate += hanlder;
				}
				void remove(OnQueryStructuredFundDelegate^ hanlder)
				{
					OnQueryStructuredFund_delegate -= hanlder;
				}
				void raise(StructuredFundInfo^ fund_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryStructuredFund_delegate)
					{
						OnQueryStructuredFund_delegate(fund_info, error_info, request_id, is_last, session_id);
					}
				}
			}			

			event OnQueryFundTransferDelegate ^OnQueryFundTransferEvent
			{
				void add(OnQueryFundTransferDelegate^ hanlder)
				{
					OnQueryFundTransfer_delegate += hanlder;
				}
				void remove(OnQueryFundTransferDelegate^ hanlder)
				{
					OnQueryFundTransfer_delegate -= hanlder;
				}
				void raise(FundTransferNotice ^ fund_transfer_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryFundTransfer_delegate)
					{
						OnQueryFundTransfer_delegate(fund_transfer_info, error_info, request_id, is_last, session_id);
					}
				}
			}
			
			event OnFundTransferDelegate ^OnFundTransferEvent
			{
				void add(OnFundTransferDelegate^ hanlder)
				{
					OnFundTransfer_delegate += hanlder;
				}
				void remove(OnFundTransferDelegate^ hanlder)
				{
					OnFundTransfer_delegate -= hanlder;
				}
				void raise(FundTransferNotice^ fund_transfer_info, RspInfoStruct^ error_info, UInt64 session_id)
				{
					if (OnFundTransfer_delegate)
					{
						OnFundTransfer_delegate(fund_transfer_info, error_info, session_id);
					}
				}
			}event OnQueryETFDelegate ^OnQueryETFEvent
			{
				void add(OnQueryETFDelegate^ hanlder)
				{
					OnQueryETF_delegate += hanlder;
				}
				void remove(OnQueryETFDelegate^ hanlder)
				{
					OnQueryETF_delegate -= hanlder;
				}
				void raise(QueryETFBaseRsp^ etf_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryETF_delegate)
					{
						OnQueryETF_delegate(etf_info, error_info, request_id, is_last, session_id);
					}
				}
			}event OnQueryETFBasketDelegate ^OnQueryETFBasketEvent
			{
				void add(OnQueryETFBasketDelegate^ hanlder)
				{
					OnQueryETFBasket_delegate += hanlder;
				}
				void remove(OnQueryETFBasketDelegate^ hanlder)
				{
					OnQueryETFBasket_delegate -= hanlder;
				}
				void raise(QueryETFComponentRsp^ etf_component_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryETFBasket_delegate)
					{
						OnQueryETFBasket_delegate(etf_component_info, error_info, request_id, is_last, session_id);
					}
				}
			}
			

			event OnQueryIPOInfoListDelegate ^OnQueryIPOInfoListEvent
			{
				void add(OnQueryIPOInfoListDelegate^ hanlder)
				{
					OnQueryIPOInfoList_delegate += hanlder;
				}
				void remove(OnQueryIPOInfoListDelegate^ hanlder)
				{
					OnQueryIPOInfoList_delegate -= hanlder;
				}
				void raise(QueryIPOTickerRsp^ ipo_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryIPOInfoList_delegate)
					{
						OnQueryIPOInfoList_delegate(ipo_info, error_info, request_id, is_last, session_id);
					}
				}
			}


			event OnQueryIPOQuotaInfoDelegate ^OnQueryIPOQuotaInfoEvent
			{
				void add(OnQueryIPOQuotaInfoDelegate^ hanlder)
				{
					OnQueryIPOQuotaInfo_delegate += hanlder;
				}
				void remove(OnQueryIPOQuotaInfoDelegate^ hanlder)
				{
					OnQueryIPOQuotaInfo_delegate -= hanlder;
				}
				void raise(QueryIPOQuotaRsp^ quota_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryIPOQuotaInfo_delegate)
					{
						OnQueryIPOQuotaInfo_delegate(quota_info, error_info, request_id, is_last, session_id);
					}
				}
			}
			
			event OnQueryOptionAuctionInfoDelegate ^OnQueryOptionAuctionInfoEvent
			{
				void add(OnQueryOptionAuctionInfoDelegate^ hanlder)
				{
					OnQueryOptionAuctionInfo_delegate += hanlder;
				}
				void remove(OnQueryOptionAuctionInfoDelegate^ hanlder)
				{
					OnQueryOptionAuctionInfo_delegate -= hanlder;
				}
				void raise(QueryOptionAuctionInfoRsp^ option_info, RspInfoStruct^ error_info, int request_id, bool is_last, UInt64 session_id)
				{
					if (OnQueryOptionAuctionInfo_delegate)
					{
						OnQueryOptionAuctionInfo_delegate(option_info, error_info, request_id, is_last, session_id);
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
			OnQueryStructuredFundDelegate^ OnQueryStructuredFund_delegate;
			OnQueryFundTransferDelegate^ OnQueryFundTransfer_delegate;
			OnFundTransferDelegate^ OnFundTransfer_delegate;
			OnQueryETFDelegate^ OnQueryETF_delegate;
			OnQueryETFBasketDelegate^ OnQueryETFBasket_delegate;
			OnQueryIPOInfoListDelegate^ OnQueryIPOInfoList_delegate;
			OnQueryIPOQuotaInfoDelegate^ OnQueryIPOQuotaInfo_delegate;
			OnQueryOptionAuctionInfoDelegate^ OnQueryOptionAuctionInfo_delegate;

		};
	}
}