#include<vcclr.h>
#include"Stdafx.h"
#include"utils.h"
#include"XtpTraderSpi.h"
#include"XtpTraderAdapter.h"
//#include"Delegates.h"

namespace XTP
{
	namespace API
	{
		XtpTraderSpi::XtpTraderSpi(XtpTraderAdapter^ pAdapter)
		{
			m_pAdapter = pAdapter;
		}
		///当客户端的某个连接与交易后台通信连接断开时，该方法被调用。
		///@param reason 错误原因，请与错误代码表对应
		///@param session_id 资金账户对应的session_id，登录时得到
		///@remark 用户主动调用logout导致的断线，不会触发此函数。api不会自动重连，当断线发生时，请用户自行选择后续操作，可以在此函数中调用Login重新登录，并更新session_id，此时用户收到的数据跟断线之前是连续的
		void XtpTraderSpi::OnDisconnected(uint64_t session_id, int reason)
		{
			m_pAdapter->OnDisconnectedEvent(session_id, reason);
		}

		///错误应答
		///@param error_info 当服务器响应发生错误时的具体的错误代码和错误信息,当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 此函数只有在服务器发生错误时才会调用，一般无需用户处理
		void XtpTraderSpi::OnError(XTPRI *error_info)
		{
			m_pAdapter->OnErrorEvent(RspInfoConverter(error_info));
		}

		///报单通知
		///@param order_info 订单响应具体信息，用户可以通过order_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单，order_info.qty_left字段在订单为未成交、部成、全成、废单状态时，表示此订单还没有成交的数量，在部撤、全撤状态时，表示此订单被撤的数量。order_info.order_cancel_xtp_id为其所对应的撤单ID，不为0时表示此单被撤成功
		///@param error_info 订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 每次订单状态更新时，都会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，在订单未成交、全部成交、全部撤单、部分撤单、已拒绝这些状态时会有响应，对于部分成交的情况，请由订单的成交回报来自行确认。所有登录了此用户的客户端都将收到此用户的订单响应
		void XtpTraderSpi::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id)
		{
			OrderInfoStruct^ order = MNConv<OrderInfoStruct^,XTPOrderInfo>::N2M( order_info);
			m_pAdapter->OnOrderEvent(RspInfoConverter(error_info), order, session_id);
		}

		///成交通知
		///@param trade_info 成交回报的具体信息，用户可以通过trade_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。对于上交所，exec_id可以唯一标识一笔成交。当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交了。对于深交所，exec_id是唯一的，暂时无此判断机制。report_index+market字段可以组成唯一标识表示成交回报。
		///@remark 订单有成交发生的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的成交回报。相关订单为部成状态，需要用户通过成交回报的成交数量来确定，OnOrderEvent()不会推送部成状态。
		void XtpTraderSpi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
		{
			TradeReportStruct^ report = MNConv<TradeReportStruct^, XTPTradeReport>::N2M(trade_info);
			m_pAdapter->OnTradeEvent(report,session_id);
		}

		///撤单出错响应
		///@param cancel_info 撤单具体信息，包括撤单的order_cancel_xtp_id和待撤单的order_xtp_id
		///@param error_info 撤单被拒绝或者发生错误时错误代码和错误信息，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 此响应只会在撤单发生错误时被回调
		void XtpTraderSpi::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id){}

		///请求查询报单响应
		///@param order_info 查询到的一个报单
		///@param error_info 查询报单时发生错误时，返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
		{
			OrderInfoStruct^ order = gcnew OrderInfoStruct();
			///XTP系统订单ID
			order->order_xtp_id = order_info->order_xtp_id;
			///报单引用，用户自定义，小于1000000
			order->order_client_id = order_info->order_client_id;
			///报单操作引用，用户自定义，小于1000000
			order->order_cancel_client_id = order_info->order_cancel_client_id;
			///撤单在XTP系统中的id
			order->order_cancel_xtp_id = order_info->order_cancel_xtp_id;
			///合约代码
			order->ticker = gcnew String(order_info->ticker);
			///交易市场
			order->market = (MARKET_TYPE)order_info->market;
			///价格
			order->price = order_info->price;
			///数量
			order->quantity = order_info->quantity;
			///报单价格条件
			order->price_type = (PRICE_TYPE)order_info->price_type;
			///买卖方向
			order->side = (SIDE_TYPE)order_info->side;
			///今成交数量
			order->qty_traded = order_info->qty_traded;
			///剩余数量，当撤单成功时，表示撤单数量
			order->qty_left = order_info->qty_left;
			///委托时间
			order->insert_time = order_info->insert_time;
			///最后修改时间
			order->update_time = order_info->update_time;
			///撤销时间
			order->cancel_time = order_info->cancel_time;
			///成交金额
			order->trade_amount = order_info->trade_amount;
			///本地报单编号 OMS生成的单号
			order->order_local_id = gcnew String(order_info->order_local_id);
			///报单状态
			order->order_status = (ORDER_STATUS_TYPE)order_info->order_status;
			///报单提交状态
			order->order_submit_status = (ORDER_SUBMIT_STATUS_TYPE)order_info->order_submit_status;
			///报单类型
			order->order_type = (TOrderTypeType)(order_info->order_type);
			//order->order_type = gcnew String((char*)order_info->order_type);

			m_pAdapter->OnQueryOrderEvent(RspInfoConverter(error_info), order, request_id, is_last, session_id);
		}

		///请求查询成交响应
		///@param trade_info 查询到的一个成交回报
		///@param error_info 查询成交回报发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
		{
			TradeReportStruct^ report = gcnew TradeReportStruct();
			///XTP系统订单ID
			report->order_xtp_id = trade_info->order_xtp_id;
			///报单引用，小于1000000
			report->order_client_id = trade_info->order_client_id;
			///合约代码
			report->ticker = gcnew String(trade_info->ticker);
			///交易市场
			report->market = (MARKET_TYPE)trade_info->market;
			///订单号
			report->local_order_id = trade_info->local_order_id;
			///成交编号，深交所唯一，上交所每笔交易唯一
			report->exec_id = gcnew String(trade_info->exec_id);
			///价格
			report->price = trade_info->price;
			///数量
			report->quantity = trade_info->quantity;
			///成交时间
			report->trade_time = trade_info->trade_time;
			///成交金额
			report->trade_amount = trade_info->trade_amount;
			///成交序号 --回报记录号，每个交易所唯一
			report->report_index = trade_info->report_index;
			///报单编号 --交易所单号
			report->order_exch_id = gcnew String(trade_info->order_exch_id);
			///成交类型  --成交回报中的执行类型
			report->trade_type = (TTradeTypeType)trade_info->trade_type;
			///买卖方向
			report->side = (SIDE_TYPE)trade_info->side;
			///交易所交易员代码 
			report->branch_pbu = gcnew String(trade_info->branch_pbu);

			m_pAdapter->OnQueryTradeEvent(RspInfoConverter(error_info), report, request_id, is_last, session_id);
		}

		///请求查询投资者持仓响应
		///@param position 查询到的一只股票的持仓情况
		///@param error_info 查询账户持仓发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 由于用户可能持有多个股票，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询资金账户响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param asset 查询到的资金账户情况
		///@param error_info 查询资金账户发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询分级基金信息响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param fund_info 查询到的分级基金情况
		///@param error_info 查询分级基金发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryStructuredFund(XTPStructuredFundInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询资金划拨订单响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param fund_transfer_info 查询到的资金账户情况
		///@param error_info 查询资金账户发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///资金划拨通知
		///@param fund_transfer_info 资金划拨通知的具体信息，用户可以通过fund_transfer_info.serial_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
		///@param error_info 资金划拨订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 当资金划拨订单有状态变化的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的资金划拨通知。
		void XtpTraderSpi::OnFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, uint64_t session_id){}

		///请求查询ETF清单文件的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param etf_info 查询到的ETF清单文件情况
		///@param error_info 查询ETF清单文件发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询ETF股票篮的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param etf_component_info 查询到的ETF合约的相关成分股信息
		///@param error_info 查询ETF股票篮发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询今日新股申购信息列表的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param ipo_info 查询到的今日新股申购的一只股票信息
		///@param error_info 查询今日新股申购信息列表发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询用户新股申购额度信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param quota_info 查询到的用户某个市场的今日新股申购额度信息
		///@param error_info 查查询用户新股申购额度信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id){}

		///请求查询期权合约的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		///@param option_info 查询到的期权合约情况
		///@param error_info 查询期权合约发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param request_id 此消息响应函数对应的请求ID
		///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@param session_id 资金账户对应的session_id，登录时得到
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpTraderSpi::OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
		{
		}

	}
}