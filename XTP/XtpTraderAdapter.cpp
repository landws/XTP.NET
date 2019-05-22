#pragma once
#include "stdafx.h"
#include <msclr/marshal.h>
#include "xtpstruct.h"
#include "utils.h"
#include <functional>
#include "XtpTraderSpi.h"
#include"../sdk/include/xtp_trader_api.h"
using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace XTP
{
	namespace API
	{
		XtpTraderAdapter::XtpTraderAdapter(int client_id, String^ account_key, String ^ log_path, TE_RESUME_TYPE resume_type)
		{
			
			pTdApi = XTP::API::TraderApi::CreateTraderApi(client_id, CAutoStrPtr(log_path).m_pChar);
			pTdApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
			pTdApi->SetSoftwareVersion("1.1.16.9");

			char* account_key_char = (char*)Marshal::StringToHGlobalAnsi(account_key).ToPointer();
			pTdApi->SetSoftwareKey(account_key_char);
			Marshal::FreeHGlobal((IntPtr)account_key_char);

			pTdSpi = new XtpTraderSpi(this);
			
			pTdApi->RegisterSpi(pTdSpi);
		}
		XtpTraderAdapter::~XtpTraderAdapter()
		{
			//
		}
		UInt64 XtpTraderAdapter::Login(String ^ ip, int port, String ^ investor_id, String ^ password, PROTOCOL_TYPE protocal_type) {
			char* investor_id_char = (char*)Marshal::StringToHGlobalAnsi(investor_id).ToPointer();
			char* password_char = (char*)Marshal::StringToHGlobalAnsi(password).ToPointer();
			char* ip_char = (char*)Marshal::StringToHGlobalAnsi(ip).ToPointer();
			uint64_t loginResult = pTdApi->Login(ip_char, port, investor_id_char, password_char, (XTP_PROTOCOL_TYPE)protocal_type);//XTP_PROTOCOL_TCP
			Marshal::FreeHGlobal((IntPtr)investor_id_char);
			Marshal::FreeHGlobal((IntPtr)password_char);
			Marshal::FreeHGlobal((IntPtr)ip_char);
			if (loginResult > 0) {
				IsLogin = true;
			}
			return loginResult;
		}
		///通过报单在xtp系统中的ID获取下单的客户端id
		///@return 返回客户端id，可以用此方法过滤自己下的订单
		///@param order_xtp_id 报单在xtp系统中的ID
		///@remark 由于系统允许同一用户在不同客户端上登录操作，每个客户端通过不同的client_id进行区分
		UInt64 XtpTraderAdapter::GetClientIDByXTPID(UInt64 order_xtp_id) {
			return pTdApi->GetClientIDByXTPID(order_xtp_id);
		}
		///通过报单在xtp系统中的ID获取相关资金账户名
		///@return 返回资金账户名
		///@param order_xtp_id 报单在xtp系统中的ID
		///@remark 只有资金账户登录成功后,才能得到正确的信息
		String^ XtpTraderAdapter::GetAccountByXTPID(UInt64 order_xtp_id) {
			return gcnew String(pTdApi->GetAccountByXTPID(order_xtp_id));
		}
		void XtpTraderAdapter::SubscribePublicTopic(TE_RESUME_TYPE resume_type) {
			pTdApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
		}

		void XtpTraderAdapter::SetHeartBeatInterval(UInt32 interval)
		{
			pTdApi->SetHeartBeatInterval(interval);
		}

		String^ XtpTraderAdapter::GetTradingDay() {
			return  gcnew String(pTdApi->GetTradingDay());
		}
		//获取API版本号
		String^ XtpTraderAdapter::GetApiVersion() {
			return  gcnew String(pTdApi->GetApiVersion());
		}
		///获取API的系统错误
		RspInfoStruct^ XtpTraderAdapter::GetApiLastError() {
			XTPRI* error_info = pTdApi->GetApiLastError();
			RspInfoStruct^ resInfo = gcnew RspInfoStruct();
			resInfo->error_id = error_info->error_id;
			resInfo->error_msg = gcnew String(error_info->error_msg);
			return resInfo;
		}
		///登出请求
		///@return 登出是否成功，“0”表示登出成功，“-1”表示登出失败
		///@param session_id 资金账户对应的session_id,登录时得到
		int XtpTraderAdapter::Logout(UInt64 session_id) {
			IsLogin = false;
			return pTdApi->Logout(session_id);
		}
		UInt64 XtpTraderAdapter::InsertOrder(OrderInsertInfo ^order, UInt64 session_id) {
			XTPOrderInsertInfo *order_info = new XTPOrderInsertInfo();
			order_info->order_xtp_id = order->order_xtp_id;
			order_info->order_client_id = order->order_client_id;
			order_info->market = (XTP_MARKET_TYPE)order->market;
			order_info->price = order->price;
			order_info->stop_price = order->stop_price;
			order_info->quantity = order->quantity;
			order_info->price_type = (XTP_PRICE_TYPE)order->price_type;
			order_info->side = (XTP_SIDE_TYPE)order->side;
			IntPtr ticker = Marshal::StringToHGlobalAnsi(order->ticker);
			strcpy(order_info->ticker, static_cast<char*>(ticker.ToPointer()));
			static_cast<char*>(ticker.ToPointer());
			Marshal::FreeHGlobal(ticker);
			return pTdApi->InsertOrder(order_info, session_id);
		}
		///报单操作请求
		///@return 撤单在XTP系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
		///@param order_xtp_id 需要撤销的委托单在XTP系统中的ID
		///@param session_id 资金账户对应的session_id,登录时得到
		///@remark 如果撤单成功，会在报单响应函数OnOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
		UInt64 XtpTraderAdapter::CancelOrder(UInt64 order_xtp_id, UInt64 session_id) {
			return pTdApi->CancelOrder(order_xtp_id, session_id);
		}
		///根据报单ID请求查询报单
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
		///@param session_id 资金账户对应的session_id，登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryOrderByXTPID(UInt64 order_xtp_id, UInt64 session_id, int request_id) {
			return pTdApi->QueryOrderByXTPID(order_xtp_id, session_id, request_id);
		}
		///请求查询报单
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
		///@param session_id 资金账户对应的session_id，登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应
		int XtpTraderAdapter::QueryOrders(QueryOrderReq ^query_param, UInt64 session_id, int request_id) {
			XTPQueryOrderReq *query = new XTPQueryOrderReq();
			IntPtr ticker = Marshal::StringToHGlobalAnsi(query_param->ticker);
			strcpy(query->ticker, static_cast<char*>(ticker.ToPointer()));
			Marshal::FreeHGlobal(ticker);
			query->begin_time = query_param->begin_time;
			query->end_time = query_param->end_time;
			return pTdApi->QueryOrders(query, session_id, request_id);
		}
		///根据委托编号请求查询相关成交
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param order_xtp_id 需要查询的委托编号，即InsertOrder()成功时返回的order_xtp_id
		///@param session_id 资金账户对应的session_id，登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		///@remark 此函数查询出的结果可能对应多个查询结果响应
		int XtpTraderAdapter::QueryTradesByXTPID(UInt64 order_xtp_id, UInt64 session_id, int request_id) {
			return pTdApi->QueryTradesByXTPID(order_xtp_id, session_id, request_id);
		}
		///请求查询已成交
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param query_param 需要查询的成交回报筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应
		int XtpTraderAdapter::QueryTrades(QueryTraderReq ^query_param, UInt64 session_id, int request_id) {
			XTPQueryTraderReq *query = new XTPQueryTraderReq();
			IntPtr ticker = Marshal::StringToHGlobalAnsi(query_param->ticker);
			strcpy(query->ticker, static_cast<char*>(ticker.ToPointer()));
			Marshal::FreeHGlobal(ticker);
			query->begin_time = query_param->begin_time;
			query->end_time = query_param->end_time;
			return pTdApi->QueryTrades(query, session_id, request_id);
		}
		///请求查询投资者持仓
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param ticker 需要查询的持仓合约代码，可以为空，如果不为空，请不带空格，并以'\0'结尾
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		///@remark 该方法如果用户提供了合约代码，则会查询此合约的持仓信息，如果合约代码为空，则默认查询所有持仓信息
		int XtpTraderAdapter::QueryPosition(String^ ticker, UInt64 session_id, int request_id) {
			char* ticker_char = (char*)Marshal::StringToHGlobalAnsi(ticker).ToPointer();
			int result = pTdApi->QueryPosition(ticker_char, session_id, request_id);
			Marshal::FreeHGlobal((IntPtr)ticker_char);
			return result;
		}
		///请求查询资产
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryAsset(UInt64 session_id, int request_id) {
			return pTdApi->QueryAsset(session_id, request_id);
		}

		///请求查询分级基金
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的分级基金筛选条件，其中母基金代码可以为空，则默认所有存在的母基金，如果不为空，请不带空格，并以'\0'结尾，其中交易市场不能为空
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
		int XtpTraderAdapter::QueryStructuredFund(QueryStructuredFundInfoReq^  query_param, UInt64 session_id, int request_id)
		{
			XTPQueryStructuredFundInfoReq native;
			MNConv<QueryStructuredFundInfoReq^, XTPQueryStructuredFundInfoReq>::M2N(query_param, &native);
			return pTdApi->QueryStructuredFund(&native, session_id, request_id);
		}

		///资金划拨请求
		///@return 资金划拨订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的serial_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
		///@param fund_transfer 资金划拨的请求信息
		///@param session_id 资金账户对应的session_id,登录时得到
		///@remark 此函数支持一号两中心节点之间的资金划拨，注意资金划拨的方向。
		UInt64  XtpTraderAdapter::FundTransfer(FundTransferReq^ fund_transfer, UInt64 session_id)
		{
			XTPFundTransferReq native;
			MNConv<FundTransferReq^, XTPFundTransferReq>::M2N(fund_transfer, &native);
			return pTdApi->FundTransfer(&native, session_id);
		}

		///请求查询资金划拨
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param query_param 需要查询的资金划拨订单筛选条件，其中serial_id可以为0，则默认所有资金划拨订单，如果不为0，则请求特定的资金划拨订单
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryFundTransfer(QueryFundTransferLogReq^ query_param, UInt64 session_id, int request_id)
		{
			XTPQueryFundTransferLogReq native;
			MNConv<QueryFundTransferLogReq^, XTPQueryFundTransferLogReq>::M2N(query_param, &native);
			return pTdApi->QueryFundTransfer(&native, session_id, request_id);
		}

		///请求查询ETF清单文件
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param query_param 需要查询的ETF清单文件的筛选条件，其中合约代码可以为空，则默认所有存在的ETF合约代码，market字段也可以为初始值，则默认所有市场的ETF合约
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryETF(QueryETFBaseReq^ query_param, UInt64 session_id, int request_id)
		{
			XTPQueryETFBaseReq native;
			MNConv<QueryETFBaseReq^, XTPQueryETFBaseReq>::M2N(query_param, &native);
			return pTdApi->QueryETF(&native, session_id, request_id);
		}

		///请求查询ETF股票篮
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param query_param 需要查询股票篮的的ETF合约，其中合约代码不可以为空，market字段也必须指定
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryETFTickerBasket(QueryETFComponentReq^ query_param, UInt64 session_id, int request_id)
		{
			XTPQueryETFComponentReq native;
			MNConv<QueryETFComponentReq^, XTPQueryETFComponentReq>::M2N(query_param, &native);
			return pTdApi->QueryETFTickerBasket(&native, session_id, request_id);
		}

		///请求查询今日新股申购信息列表
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryIPOInfoList(UInt64 session_id, int request_id)
		{
			return pTdApi->QueryIPOInfoList(session_id, request_id);
		}

		///请求查询用户新股申购额度信息
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryIPOQuotaInfo(UInt64 session_id, int request_id)
		{
			return pTdApi->QueryIPOQuotaInfo( session_id, request_id);
		}

		///请求查询期权合约
		///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@param query_param 需要查询的期权合约的筛选条件，可以为NULL（为NULL表示查询所有的期权合约）
		///@param session_id 资金账户对应的session_id,登录时得到
		///@param request_id 用于用户定位查询响应的ID，由用户自定义
		int XtpTraderAdapter::QueryOptionAuctionInfo(QueryOptionAuctionInfoReq^ query_param, UInt64 session_id, int request_id)
		{
			XTPQueryOptionAuctionInfoReq native;
			MNConv<QueryOptionAuctionInfoReq^, XTPQueryOptionAuctionInfoReq>::M2N(query_param, &native);
			return pTdApi->QueryOptionAuctionInfo(&native, session_id,request_id);
		}
	}
}