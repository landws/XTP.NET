// XTPDll.cpp: 主项目文件。
#pragma once
#include "stdafx.h"
#include <msclr/marshal.h>
#include "xtpstruct.h"
#include "utils.h"
#include <functional>
#include"XtpQuoteAdapter.h"
#include"XtpQuoteSpi.h"
#include "..\sdk\include\xtp_quote_api.h"

using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace XTP {
	namespace API {
		
		///创建QuoteApi
		///@param client_id （必须输入）用于区分同一用户的不同客户端，由用户自定义
		///@param log_path （必须输入）存贮订阅信息文件的目录，请设定一个有可写权限的真实存在的路径
		///@param log_level 日志输出级别
		///@remark 如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接
		XtpQuoteAdapter::XtpQuoteAdapter(int client_id, String ^ log_path, LOG_LEVEL log_level)
		{
			pUserApi = XTP::API::QuoteApi::CreateQuoteApi(client_id, CAutoStrPtr(log_path).m_pChar, (XTP_LOG_LEVEL)log_level);

			pUserSpi = new XtpQuoteSpi(this);
		
			pUserApi->RegisterSpi(pUserSpi);
		}
		XtpQuoteAdapter::~XtpQuoteAdapter(void)
		{

		}

		///用户登录请求
		///@return 登录是否成功，“0”表示登录成功，“-1”表示连接服务器出错，此时用户可以调用GetApiLastError()来获取错误代码，“-2”表示已存在连接，不允许重复登录，如果需要重连，请先logout，“-3”表示输入有错误
		///@param ip 服务器ip地址，类似“127.0.0.1”
		///@param port 服务器端口号
		///@param user 登陆用户名
		///@param password 登陆密码
		///@param sock_type “1”代表TCP，“2”代表UDP
		///@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api只能有一个连接
		int XtpQuoteAdapter::Login(String ^ ip, int port, String ^ investor_id, String ^ password, PROTOCOL_TYPE protocal_type) {
			char* investor_id_char = (char*)Marshal::StringToHGlobalAnsi(investor_id).ToPointer();
			char* password_char = (char*)Marshal::StringToHGlobalAnsi(password).ToPointer();
			char* ip_char = (char*)Marshal::StringToHGlobalAnsi(ip).ToPointer();
			int loginResult = pUserApi->Login(ip_char, port, investor_id_char, password_char, (XTP_PROTOCOL_TYPE)protocal_type);//XTP_PROTOCOL_TCP
			Marshal::FreeHGlobal((IntPtr)investor_id_char);
			Marshal::FreeHGlobal((IntPtr)password_char);
			Marshal::FreeHGlobal((IntPtr)ip_char);
			if (loginResult == 0) {
				IsLogin = true;
			}
			return loginResult;
		}

		String^ XtpQuoteAdapter::GetTradingDay() {
			return  gcnew String(pUserApi->GetTradingDay());
		}
		//获取API版本号
		String^ XtpQuoteAdapter::GetApiVersion() {
			return  gcnew String(pUserApi->GetApiVersion());
		}
		///获取API的系统错误
		RspInfoStruct^ XtpQuoteAdapter::GetApiLastError() {
			XTPRI* error_info = pUserApi->GetApiLastError();
			RspInfoStruct^ resInfo = gcnew RspInfoStruct();
			resInfo->error_id = error_info->error_id;
			resInfo->error_msg = gcnew String(error_info->error_msg);
			return resInfo;
		}
		//查询所有股票代码
		int XtpQuoteAdapter::QueryAllTickers(EXCHANGE_TYPE exchange) {
			return pUserApi->QueryAllTickers((XTP_EXCHANGE_TYPE)exchange);
		}
		
		///登出请求
		///@return 登出是否成功，“0”表示登出成功，非“0”表示登出出错，此时用户可以调用GetApiLastError()来获取错误代码
		///@remark 此函数为同步阻塞式，不需要异步等待登出，当函数返回即可进行后续操作
		int XtpQuoteAdapter::Logout() {
			IsLogin = false;
			return pUserApi->Logout();
		}
		
		///设置采用UDP方式连接时的接收缓冲区大小
		///@remark 需要在Login之前调用，默认大小和最小设置均为64MB。此缓存大小单位为MB，请输入2的次方数，例如128MB请输入128。
		void XtpQuoteAdapter::SetUDPBufferSize(UInt32 buff_size)
		{
			pUserApi->SetUDPBufferSize(buff_size);
		}

		///设置心跳检测时间间隔，单位为秒
		///@param interval 心跳检测时间间隔，单位为秒
		///@remark 此函数必须在Login之前调用
		void XtpQuoteAdapter::SetHeartBeatInterval(UInt32 interval)
		{
			pUserApi->SetHeartBeatInterval(interval);
		}

		///订阅/退订行情。
		///@return 订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
		///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
		///@param count 要订阅/退订行情的合约个数
		///@param exchage_id 交易所代码
		///@param is_subscribe 是否是订阅
		///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
		int XtpQuoteAdapter::SubscribeMarketData(array<String^>^ ticker, EXCHANGE_TYPE exchange, bool is_subscribe) {
			//marshal_context ^ context = gcnew marshal_context();
			char** tokensAsAnsi = new char*[ticker->Length];
			for (int i = 0; i < ticker->Length; i++)
			{
				//IntPtr p = Marshal::StringToHGlobalAnsi(ticker[i]);
				//tokensAsAnsi[i] = static_cast<char*>(p.ToPointer());
				//Marshal::FreeHGlobal(p);
				tokensAsAnsi[i] = static_cast<char*>(Marshal::StringToHGlobalAnsi(ticker[i]).ToPointer());
				//const char* str4 = context->marshal_as<const char*>(ticker[i]);
				//tokensAsAnsi[i] = str4;
			}
			// The marshalled results are freed when context goes out of scope
			int result = 0;
			if (is_subscribe) {
				result = pUserApi->SubscribeMarketData(tokensAsAnsi, ticker->Length, (XTP_EXCHANGE_TYPE)exchange);//订阅股票行情
			}
			else {
				result = pUserApi->UnSubscribeMarketData(tokensAsAnsi, ticker->Length, XTP_EXCHANGE_TYPE(exchange));//取消订阅股票行情
			}
			delete[] tokensAsAnsi;    // Please note you must use delete[] here!
									  //delete context;
			return result;
		}

		///订阅/退订行情订单簿。
		///@return 订阅/退订行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
		///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
		///@param exchage_id 交易所代码
		///@param is_subscribe 是否是订阅
		///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(暂不支持)
		int XtpQuoteAdapter::SubscribeOrderBook(array<String^>^ ticker, EXCHANGE_TYPE exchage_id, bool is_subscribe)
		{///@param count 要订阅/退订行情订单簿的合约个数

			int count = ticker->Length;
			char** chTicker = new char*[count];
			for (int i = 0; i < count; ++i)
			{
				chTicker[i] =  CAutoStrPtr::CAutoStrPtr(ticker[i]).m_pChar;
			}
			int result = 0;
			if (is_subscribe)
			{
				result = pUserApi->SubscribeOrderBook(chTicker, count, (XTP_EXCHANGE_TYPE)exchage_id);
			}
			else
			{
				result = pUserApi->UnSubscribeOrderBook(chTicker, count, (XTP_EXCHANGE_TYPE)exchage_id);
			}
			delete[] chTicker;
			return result;
		}

		///订阅/退订逐笔行情。
		///@return 订阅/退订逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
		///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
		///@param exchage_id 交易所代码
		///@param  is_subscribe 是否是订阅
		///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(暂不支持)
		int XtpQuoteAdapter::SubscribeTickByTick(array<String^>^ ticker, EXCHANGE_TYPE exchage_id, bool is_subscribe)
		{
			int count = ticker->Length;
			char** chTicker = new char*[count];
			for (int i = 0; i < count; ++i)
			{
				chTicker[i] =  CAutoStrPtr::CAutoStrPtr(ticker[i]).m_pChar;
			}
			int result = 0;
			if (is_subscribe)
			{
				result = pUserApi->SubscribeTickByTick(chTicker, count, (XTP_EXCHANGE_TYPE)exchage_id);
			}
			else
			{
				result = pUserApi->UnSubscribeTickByTick(chTicker, count, (XTP_EXCHANGE_TYPE)exchage_id);
			}
			delete[] chTicker;
			return result;
		}

		///订阅/退订全市场的行情
		///@return 订阅/退订全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
		///@param  is_subscribe 是否是订阅
		///@remark 需要与全市场退订行情接口配套使用
		int XtpQuoteAdapter::SubscribeAllMarketData(bool is_subscribe)
		{
			if (is_subscribe)
			{
				return pUserApi->SubscribeAllMarketData();
			}
			else
			{
				return pUserApi->UnSubscribeAllMarketData();
			}
		}

		///订阅/退订全市场的行情订单簿
		///@return 订阅/退订全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
		///@param  is_subscribe 是否是订阅
		///@remark 需要与全市场退订行情订单簿接口配套使用
		int XtpQuoteAdapter::SubscribeAllOrderBook(bool is_subscribe)
		{
			if (is_subscribe)
			{
				return pUserApi->SubscribeAllOrderBook();
			}
			else
			{
				return pUserApi->UnSubscribeAllOrderBook();
			}
		}
		
		///订阅/退订全市场的逐笔行情
		///@return 订阅/退订全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
		///@param  is_subscribe 是否是订阅
		///@remark 需要与全市场退订逐笔行情接口配套使用
		int XtpQuoteAdapter::SubscribeAllTickByTick(bool is_subscribe)
		{
			if (is_subscribe)
			{
				return pUserApi->SubscribeAllTickByTick();
			}
			else
			{
				return pUserApi->UnSubscribeAllTickByTick();
			}
		}
				
		///获取合约的最新价格信息
		///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
		///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
		///@param exchage_id 交易所代码
		int XtpQuoteAdapter::QueryTickersPriceInfo(array<String^>^ ticker, EXCHANGE_TYPE exchage_id)
		{
			int count = ticker->Length;
			char** chTicker = new char*[count];
			for (int i = 0; i < count; ++i)
			{
				chTicker[i] =  CAutoStrPtr::CAutoStrPtr(ticker[i]).m_pChar;
			}
			int result = pUserApi->QueryTickersPriceInfo(chTicker, count, (XTP_EXCHANGE_TYPE)exchage_id);
			delete[] chTicker;
			return result;
		}

		///获取所有合约的最新价格信息
		///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
		int XtpQuoteAdapter::QueryAllTickersPriceInfo()
		{
			return pUserApi->QueryAllTickersPriceInfo();
		}
	}
}
