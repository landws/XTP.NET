#include<vcclr.h>
#include"Stdafx.h"
#include"utils.h"
#include"XtpQuoteSpi.h"

namespace XTP
{
	namespace API {
		XtpQuoteSpi::XtpQuoteSpi(XtpQuoteAdapter^ pAdapter)
		{
			m_pAdapter = pAdapter;
		}
		void XtpQuoteSpi::OnError(XTPRI *error_info) {
			m_pAdapter->OnErrorEvent(RspInfoConverter(error_info));
		}
		void XtpQuoteSpi::OnDisconnected(int reason) {
			m_pAdapter->OnDisconnectedEvent(reason);
		}
		void XtpQuoteSpi::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {
			SpecificTickerStruct^ ticketInfo =MNConv<SpecificTickerStruct^, XTPST>::N2M(ticker);
			m_pAdapter->OnSubMarketDataEvent(RspInfoConverter(error_info), ticketInfo, is_last);
		}
		void XtpQuoteSpi::OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {

			SpecificTickerStruct^ ticketInfo = MNConv<SpecificTickerStruct^, XTPST>::N2M(ticker);//  gcnew SpecificTickerStruct();
			m_pAdapter->OnUnSubMarketDataEvent(RspInfoConverter(error_info), ticketInfo, is_last);
		}

		///深度行情通知，包含买一卖一队列
		///@param market_data 行情数据
		///@param bid1_qty 买一队列数据
		///@param bid1_count 买一队列的有效委托笔数
		///@param max_bid1_count 买一队列总委托笔数
		///@param ask1_qty 卖一队列数据
		///@param ask1_count 卖一队列的有效委托笔数
		///@param max_ask1_count 卖一队列总委托笔数
		///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count)
		{
			// 收到市场股价数据

			MarketDataStruct^ data = gcnew MarketDataStruct();
			// 代码
			///交易所代码
			data->exchange_id = market_data->exchange_id;
			///合约代码（不包含交易所信息）
			data->ticker = gcnew String(market_data->ticker);
			// 股票等价格
			///最新价
			data->last_price = market_data->last_price;
			///昨收盘
			data->pre_close_price = market_data->pre_close_price;
			///今开盘
			data->open_price = market_data->open_price;
			///最高价
			data->high_price = market_data->high_price;
			///最低价
			data->low_price = market_data->low_price;
			///今收盘
			data->close_price = market_data->close_price;

			// 期货等数据
			///昨持仓量（目前未填写）
			data->pre_open_interest = market_data->pre_open_interest;
			///持仓量（目前未填写）
			data->open_interest = market_data->open_interest;
			///上次结算价（目前未填写）
			data->pre_settlement_price = market_data->pre_settlement_price;
			///本次结算价（目前未填写）
			data->settlement_price = market_data->settlement_price;

			///涨停板价（目前未填写）
			data->upper_limit_price = market_data->upper_limit_price;
			///跌停板价（目前未填写）
			data->lower_limit_price = market_data->lower_limit_price;
			///昨虚实度（目前未填写）
			data->pre_delta = market_data->pre_delta;
			///今虚实度（目前未填写）
			data->curr_delta = market_data->curr_delta;

			/// 时间类
			data->data_time = market_data->data_time;

			// 量额数据
			///数量
			data->qty = market_data->qty;
			///成交金额
			data->turnover = market_data->turnover;
			///当日均价
			data->avg_price = market_data->avg_price;

			// 买卖盘
			///十档申买价
			array<double>^ bid = gcnew array<double>(sizeof(market_data->bid));
			for (int i = 0; i < sizeof(market_data->bid); i++) {
				bid[i] = market_data->bid[i];
			}
			data->bid = bid;
			///十档申卖价
			array<double>^	ask = gcnew array<double>(sizeof(market_data->ask));
			for (int i = 0; i < sizeof(market_data->ask); i++) {
				ask[i] = market_data->ask[i];
			}
			data->ask = ask;
			///十档申买量
			array<int64_t>^	bid_qty = gcnew array<int64_t>(sizeof(market_data->bid_qty));
			for (int i = 0; i < sizeof(market_data->bid_qty); i++) {
				bid_qty[i] = market_data->bid_qty[i];
			}
			data->bid_qty = bid_qty;
			///十档申卖量
			array<int64_t>^	ask_qty = gcnew array<int64_t>(sizeof(market_data->ask_qty));
			for (int i = 0; i < sizeof(market_data->ask_qty); i++) {
				ask_qty[i] = market_data->ask_qty[i];
			}
			data->ask_qty = ask_qty;

			// lts没有包含的数据（目前未填写）
			///成交笔数
			data->trades_count = market_data->trades_count;
			///当前交易状态说明
			data->ticker_status = gcnew String(market_data->ticker_status);
			///委托买入总量
			data->total_bid_qty = market_data->total_bid_qty;
			///委托卖出总量
			data->total_ask_qty = market_data->total_ask_qty;
			///加权平均委买价格
			data->ma_bid_price = market_data->ma_bid_price;
			///加权平均委卖价格
			data->ma_ask_price = market_data->ma_ask_price;
			///债券加权平均委买价格
			data->ma_bond_bid_price = market_data->ma_bond_bid_price;
			///债券加权平均委卖价格
			data->ma_bond_ask_price = market_data->ma_bond_ask_price;
			///债券到期收益率
			data->yield_to_maturity = market_data->yield_to_maturity;
			///ETF净值估值
			data->iopv = market_data->iopv;
			///ETF申购笔数
			data->etf_buy_count = market_data->etf_buy_count;
			///ETF赎回笔数
			data->etf_sell_count = market_data->etf_sell_count;
			///ETF申购数量
			data->etf_buy_qty = market_data->etf_buy_qty;
			///ETF申购金额
			data->etf_buy_money = market_data->etf_buy_money;
			///ETF赎回数量
			data->etf_sell_qty = market_data->etf_sell_qty;
			///ETF赎回金额
			data->etf_sell_money = market_data->etf_sell_money;
			///权证执行的总数量
			data->total_warrant_exec_qty = market_data->total_warrant_exec_qty;
			///权证跌停价格（元）
			data->warrant_lower_price = market_data->warrant_lower_price;
			///权证涨停价格（元）
			data->warrant_upper_price = market_data->warrant_upper_price;
			///买入撤单笔数
			data->cancel_buy_count = market_data->cancel_buy_count;
			///卖出撤单笔数
			data->cancel_sell_count = market_data->cancel_sell_count;
			///买入撤单数量
			data->cancel_buy_qty = market_data->cancel_buy_qty;
			///卖出撤单数量
			data->cancel_sell_qty = market_data->cancel_sell_qty;
			///买入撤单金额
			data->cancel_buy_money = market_data->cancel_buy_money;
			///卖出撤单金额
			data->cancel_sell_money = market_data->cancel_sell_money;
			///买入总笔数
			data->total_buy_count = market_data->total_buy_count;
			///卖出总笔数
			data->total_sell_count = market_data->total_sell_count;
			///买入委托成交最大等待时间
			data->duration_after_buy = market_data->duration_after_buy;
			///卖出委托成交最大等待时间
			data->duration_after_sell = market_data->duration_after_sell;
			///买方委托价位数
			data->num_bid_orders = market_data->num_bid_orders;
			///卖方委托价位数
			data->num_ask_orders = market_data->num_ask_orders;
			///成交时间（UA3113）
			data->exec_time = market_data->exec_time;
			///闭市标志（UA103/UA104）
			data->is_market_closed = gcnew String(market_data->is_market_closed);
			///合约持仓量（UA103）
			data->total_position = market_data->total_position;
			///市盈率1（UA103）
			data->pe_ratio1 = market_data->pe_ratio1;
			///市盈率2（UA103）
			data->pe_ratio2 = market_data->pe_ratio2;
			
			m_pAdapter->OnDepthMarketDataEvent(data, nullptr, 0, 0, nullptr, 0, 0);
			delete data;
		}
		
		void XtpQuoteSpi::OnQueryAllTickers(XTPQSI* ticker_info, XTPRI *error_info, bool is_last) {
			QuoteStaticInfoStruct^ data = gcnew QuoteStaticInfoStruct();
			RspInfoStruct^ resInfo = gcnew RspInfoStruct();
			if (error_info == nullptr || error_info->error_id == 0) {
				resInfo->error_id = 0;
				resInfo->error_msg = "";
				// 收到市场股价数据
				// 代码
				///交易所代码
				data->exchange_id = ticker_info->exchange_id;
				///合约代码（不包含交易所信息）
				data->ticker = gcnew String(ticker_info->ticker);
				/// 合约名称
				data->ticker_name = gcnew String(ticker_info->ticker_name, 0, sizeof(ticker_info->ticker_name), System::Text::Encoding::UTF8);
				/// 合约类型
				data->ticker_type = ticker_info->ticker_type;
				///昨收盘
				data->pre_close_price = ticker_info->pre_close_price;
				///涨停板价
				data->upper_limit_price = ticker_info->upper_limit_price;
				///跌停板价
				data->lower_limit_price = ticker_info->lower_limit_price;
				///最小变动价位
				data->price_tick = ticker_info->price_tick;
				/// 合约最小交易量(买)
				data->buy_qty_unit = ticker_info->buy_qty_unit;
				/// 合约最小交易量(卖)
				data->sell_qty_unit = ticker_info->sell_qty_unit;
			}
			else {
				resInfo->error_id = error_info->error_id;
				resInfo->error_msg = gcnew String(error_info->error_msg);
			}
			m_pAdapter->OnQueryAllTickersEvent(resInfo, data, is_last);
			
			delete data;
		}
				
		///订阅行情订单簿应答
		///@param ticker 详细的合约订阅情况
		///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last)
		{}

		///退订行情订单簿应答
		///@param ticker 详细的合约取消订阅情况
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnUnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last)
		{}

		///行情订单簿通知
		///@param order_book 行情订单簿数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnOrderBook(XTPOB *order_book)
		{}

		///订阅逐笔行情应答
		///@param ticker 详细的合约订阅情况
		///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last)
		{}

		///退订逐笔行情应答
		///@param ticker 详细的合约取消订阅情况
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnUnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last)
		{}

		///逐笔行情通知
		///@param tbt_data 逐笔行情数据，包括逐笔委托和逐笔成交，此为共用结构体，需要根据type来区分是逐笔委托还是逐笔成交，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
		void XtpQuoteSpi::OnTickByTick(XTPTBT *tbt_data)
		{}

		///订阅全市场的行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		void XtpQuoteSpi::OnSubscribeAllMarketData(XTPRI *error_info)
		{}

		///退订全市场的行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		void XtpQuoteSpi::OnUnSubscribeAllMarketData(XTPRI *error_info)
		{}

		///订阅全市场的行情订单簿应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		void XtpQuoteSpi::OnSubscribeAllOrderBook(XTPRI *error_info)
		{}

		///退订全市场的行情订单簿应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		void XtpQuoteSpi::OnUnSubscribeAllOrderBook(XTPRI *error_info)
		{}

		///订阅全市场的逐笔行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		void XtpQuoteSpi::OnSubscribeAllTickByTick(XTPRI *error_info)
		{}

		///退订全市场的逐笔行情应答
		///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@remark 需要快速返回
		void XtpQuoteSpi::OnUnSubscribeAllTickByTick(XTPRI *error_info)
		{
			
		}
		
		///查询合约的最新价格信息应答
		///@param ticker_info 合约的最新价格信息
		///@param error_info 查询合约的最新价格信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
		///@param is_last 是否此次查询的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
		void XtpQuoteSpi::OnQueryTickersPriceInfo(XTPTPI* ticker_info, XTPRI *error_info, bool is_last)
		{}
	}
}