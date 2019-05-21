#pragma once
#include"..\sdk\include\xoms_api_fund_struct.h"

using namespace System;
using namespace::Runtime::InteropServices;
namespace XTP {
	namespace API {

		/// 存放版本号的字符串长度
		#define VERSION_LEN 16
		/// 版本号类型
		typedef char VersionType[VERSION_LEN];
		/// 可交易日字符串长度
		#define TRADING_DAY_LEN 9
			/// 存放证券代码的字符串长度
		#define TICKER_LEN 16
			/// 存放证券名称的字符串长度
		#define TICKER_NAME_LEN 64
			/// 本地报单编号的字符串长度
		#define LOCAL_ORDER_LEN         11
			/// 交易所单号的字符串长度
		#define ORDER_EXCH_LEN          17
			/// 成交执行编号的字符串长度
		#define EXEC_ID_LEN             18
			/// 交易所交易员代码字符串长度
		#define BRANCH_PBU_LEN          7
			/// 用户资金账户的字符串长度
		#define ACCOUNT_NAME_LEN        16
			///错误信息的字符串长度
		#define ERR_MSG_LEN  124
			/// 用户资金账户的字符串长度
		#define ACCOUNT_NAME_LEN        16

			/// 用户资金账户密码的字符串长度
		#define ACCOUNT_PASSWORD_LEN 64
#pragma region basic struct

		/// <summary>
		/// XTP_PROTOCOL_TYPE 是一个通讯传输协议方式
		/// </summary>
		public enum class PROTOCOL_TYPE
		{
			XTP_PROTOCOL_TCP = 1,	///<采用TCP方式传输
			XTP_PROTOCOL_UDP		///<采用UDP方式传输
		};

		///日志输出级别类型
		public enum class LOG_LEVEL {
			XTP_LOG_LEVEL_FATAL, ///<严重错误级别
			XTP_LOG_LEVEL_ERROR, ///<错误级别
			XTP_LOG_LEVEL_WARNING, ///<警告级别
			XTP_LOG_LEVEL_INFO,   ///<info级别
			XTP_LOG_LEVEL_DEBUG,  ///<debug级别
			XTP_LOG_LEVEL_TRACE   ///<trace级别
		};

		[StructLayout(LayoutKind::Sequential)]
		public ref struct  RspInfoStruct
		{
			///错误代码
			Int32	error_id;
			///错误信息
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = ERR_MSG_LEN)]
			String^	error_msg;
		};

		/// <summary>
		/// EXCHANGE_TYPE 是交易所类型
		/// </summary>
		public enum class EXCHANGE_TYPE
		{
			XTP_EXCHANGE_SH = 1,	///<上证
			XTP_EXCHANGE_SZ,		///<深证
			XTP_EXCHANGE_MAX	///<不存在的交易所类型
		};

		//////////////////////////////////////////////////////////////////////////
		///@brief ETF_REPLACE_TYPE现金替代标识定义
		//////////////////////////////////////////////////////////////////////////
		public enum class ETF_REPLACE_TYPE
		{
			ERT_CASH_FORBIDDEN = 0,             ///<禁止现金替代
			ERT_CASH_OPTIONAL,                  ///<可以现金替代
			ERT_CASH_MUST,                      ///<必须现金替代
			ERT_CASH_RECOMPUTE_INTER_SZ,        ///<深市退补现金替代
			ERT_CASH_MUST_INTER_SZ,             ///<深市必须现金替代
			ERT_CASH_RECOMPUTE_INTER_OTHER,     ///<非沪深市场成分证券退补现金替代
			ERT_CASH_MUST_INTER_OTHER,          ///<表示非沪深市场成份证券必须现金替代
			EPT_INVALID                         ///<无效值
		};
		///指定的合约
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  SpecificTickerStruct
		{
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息）例如"600000  "
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^	ticker;
		};


		/// <summary>
		///业务类型
		/// </summary>
		public enum class BUSINESS_TYPE
		{
			XTP_BUSINESS_TYPE_CASH = 0,            ///<普通股票业务（股票买卖，ETF买卖等）
			XTP_BUSINESS_TYPE_IPOS,                ///<新股申购业务（对应的price type需选择限价类型）
			XTP_BUSINESS_TYPE_REPO,                ///<回购业务 ( 对应的price type填为限价，side填为卖 )
			XTP_BUSINESS_TYPE_ETF,                 ///<ETF申赎业务
			XTP_BUSINESS_TYPE_MARGIN,              ///<融资融券业务（暂未支持）
			XTP_BUSINESS_TYPE_DESIGNATION,         ///<转托管（未支持）
			XTP_BUSINESS_TYPE_ALLOTMENT,		   ///<配股业务（对应的price type需选择限价类型,side填为买）
			XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION,	   ///<分级基金申赎业务
			XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE,	   ///<分级基金拆分合并业务
			XTP_BUSINESS_TYPE_MONEY_FUND,		   ///<货币基金业务（暂未支持）
			XTP_BUSINESS_TYPE_OPTION,              ///<期权业务
			XTP_BUSINESS_TYPE_EXECUTE,             ///<行权
			XTP_BUSINESS_TYPE_FREEZE,              ///<锁定解锁，暂不支持
			XTP_BUSINESS_TYPE_UNKNOWN              ///<未知类型
		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_TBT_TYPE是一个逐笔回报类型
		/////////////////////////////////////////////////////////////////////////
		public enum class TBT_TYPE {
			XTP_TBT_ENTRUST = 1,	///<逐笔委托
			XTP_TBT_TRADE = 2,		///<逐笔成交
		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_OPT_CALL_OR_PUT_TYPE是一个认沽或认购类型
		/////////////////////////////////////////////////////////////////////////
		public enum class OPT_CALL_OR_PUT_TYPE {
			XTP_OPT_CALL = 1,	    ///<认购
			XTP_OPT_PUT = 2,		///<认沽
		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_OPT_EXERCISE_TYPE_TYPE是一个行权方式类型
		/////////////////////////////////////////////////////////////////////////
		public enum class OPT_EXERCISE_TYPE_TYPE {
			XTP_OPT_EXERCISE_TYPE_EUR = 1,	    ///<欧式
			XTP_OPT_EXERCISE_TYPE_AME = 2,		///<美式
		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_POSITION_DIRECTION_TYPE是一个持仓方向类型
		/////////////////////////////////////////////////////////////////////////
		public enum class POSITION_DIRECTION_TYPE {
			XTP_POSITION_DIRECTION_NET = 0,	    ///<净
			XTP_POSITION_DIRECTION_LONG,		///<多（期权则为权利方）
			XTP_POSITION_DIRECTION_SHORT,       ///<空（期权则为义务方）
			XTP_POSITION_DIRECTION_COVERED,     ///<备兑（期权则为备兑义务方）
		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_CRD_CASH_REPAY_STATUS是一个融资融券直接还款状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class CRD_CR_STATUS {
			XTP_CRD_CR_INIT = 0,        ///< 初始、未处理状态
			XTP_CRD_CR_SUCCESS,         ///< 已成功处理状态
			XTP_CRD_CR_FAILED,          ///< 处理失败状态
		};
#pragma endregion


#pragma region api data struct
		//////////////////////////////////////////////////////////////////////////
		///@brief XTP_ACCOUNT_TYPE账户类型
		//////////////////////////////////////////////////////////////////////////
		public enum class ACCOUNT_TYPE
		{
			XTP_ACCOUNT_NORMAL = 0,	///<普通账户
			XTP_ACCOUNT_CREDIT,		///<信用账户
			XTP_ACCOUNT_DERIVE,		///<衍生品账户
			XTP_ACCOUNT_UNKNOWN		///<未知账户类型
		};
		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_FUND_TRANSFER_TYPE是资金流转方向类型
		/////////////////////////////////////////////////////////////////////////
		public enum class FUND_TRANSFER_TYPE
		{
			XTP_FUND_TRANSFER_OUT = 0,		///<转出 从XTP转出到柜台
			XTP_FUND_TRANSFER_IN,	        ///<转入 从柜台转入XTP
			XTP_FUND_INTER_TRANSFER_OUT,    ///<跨节点转出 从本XTP节点1，转出到对端XTP节点2，XTP服务器之间划拨，只能跨账户用户使用
			XTP_FUND_INTER_TRANSFER_IN,     ///<跨节点转入 从对端XTP节点2，转入到本XTP节点1，XTP服务器之间划拨，只能跨账户用户使用
			XTP_FUND_TRANSFER_UNKNOWN		///<未知类型
		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_FUND_OPER_STATUS柜台资金操作结果
		/////////////////////////////////////////////////////////////////////////
		public enum class FUND_OPER_STATUS {
			XTP_FUND_OPER_PROCESSING = 0,	///<XTP已收到，正在处理中
			XTP_FUND_OPER_SUCCESS,			///<成功
			XTP_FUND_OPER_FAILED,			///<失败
			XTP_FUND_OPER_SUBMITTED,		///<已提交到集中柜台处理
			XTP_FUND_OPER_UNKNOWN			///<未知
		};
		/////////////////////////////////////////////////////////////////////////
		///@brief SPLIT_MERGE_STATUS是一个基金当天拆分合并状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class SPLIT_MERGE_STATUS {
			XTP_SPLIT_MERGE_STATUS_ALLOW = 0,	///<允许拆分和合并
			XTP_SPLIT_MERGE_STATUS_ONLY_SPLIT,	///<只允许拆分，不允许合并
			XTP_SPLIT_MERGE_STATUS_ONLY_MERGE,	///<只允许合并，不允许拆分
			XTP_SPLIT_MERGE_STATUS_FORBIDDEN	///<不允许拆分合并
		};


		//////////////////////////////////////////////////////////////////////////
		///市场类型
		//////////////////////////////////////////////////////////////////////////
		public enum class MARKET_TYPE
		{
			XTP_MKT_INIT = 0,///<初始化值或者未知
			XTP_MKT_SZ_A = 1,///<深圳A股
			XTP_MKT_SH_A,    ///<上海A股
							 /*
							 XTP_MKT_SZ_B,    ///<深圳B股
							 XTP_MKT_SH_B,    ///<上海B股*/
			XTP_MKT_MAX  ///<市场类型个数
		};

		/////////////////////////////////////////////////////////////////////////
		///TXTPTradeTypeType是成交类型类型
		/////////////////////////////////////////////////////////////////////////
		public enum struct TradeTypeType :Byte
		{
			COMMON = '0',//普通成交
			CASH = '1',//现金替代
			PRIMARY = '2'//一级市场成交
		};


		//////////////////////////////////////////////////////////////////////////
		///@brief TICKER_TYPE证券类型
		//////////////////////////////////////////////////////////////////////////
		public enum struct TICKER_TYPE
		{
			XTP_TICKER_TYPE_STOCK = 0,            ///<普通股票
			XTP_TICKER_TYPE_INDEX,                ///<指数
			XTP_TICKER_TYPE_FUND,                 ///<基金
			XTP_TICKER_TYPE_BOND,                 ///<债券
			XTP_TICKER_TYPE_OPTION,               ///<期权
			XTP_TICKER_TYPE_UNKNOWN               ///<未知类型
		};

		
		/// <summary>
		/// 买卖方向类型
		/// </summary>
		public enum class SIDE_TYPE
		{
			BUY = 1,	///<买（新股申购、ETF买等）
			SELL = 2,		///<卖（逆回购）
			///申购
			PURCHASE = 7,
			///赎回
			REDEMPTION = 8,
			///拆分
			SPLIT = 9,
			///合并
			MERGE = 10,
			///改版之后的side的备兑，暂不支持
			COVER = 11,
			///改版之后的side锁定（对应开平标识为开）/解锁（对应开平标识为平）
			FREEZE = 12,
			/// 融资买入
			TRADE = 21,
			/// 融券卖出
			SHORT_SELL = 22,
			/// 卖券还款
			REPAY_MARGIN = 23,
			/// 买券还券
			REPAY_STOCK = 24,
			/// 现金还款（不放在普通订单协议，另加请求和查询协议）
			CASH_REPAY_MARGIN = 25,
			/// 现券还券
			STOCK_REPAY_STOCK = 26,
			/// 余券划转
			SURSTK_TRANS = 27,
			/// 担保品转入
			GRTSTK_TRANSIN = 28,
			/// 担保品转出
			GRTSTK_TRANSOUT = 29,
			UNKNOWN,	///<未知或者无效买卖方向
		};

		////////////////////////////////////////////////////////////////////////
///@brief XTP_POSITION_EFFECT_TYPE是开平标识类型
/////////////////////////////////////////////////////////////////////////
		public enum class POSITION_EFFECT_TYPE
		{
			/// 初始值或未知值开平标识，现货适用
			INIT = 0,
			/// 开
			OPEN = 1,
			/// 平
			CLOSE = 2,
			/// 强平
			FORCECLOSE = 3,
			/// 平今
			CLOSETODAY = 4,
			/// 平昨
			CLOSEYESTERDAY = 5,
			/// 强减
			FORCEOFF = 6,
			/// 本地强平
			LOCALFORCECLOSE = 7,
			/// 信用业务追保强平
			CREDIT_FORCE_COVER = 8,
			/// 信用业务清偿强平
			CREDIT_FORCE_CLEAR = 9,
			/// 信用业务合约到期强平
			CREDIT_FORCE_DEBT = 10,
			/// 信用业务无条件强平
			CREDIT_FORCE_UNCOND = 11,
			/// 未知的开平标识类型
			UNKNOWN = 12
		};
		/////////////////////////////////////////////////////////////////////////
		///TXTPOrderTypeType是一个报单类型类型
		/////////////////////////////////////////////////////////////////////////
		public enum class TOrderTypeType
		{
			Normal = '0',	///正常
			DeriveFromQuote = '1',		///报价衍生
			DeriveFromCombination = '2',		///组合衍生
			Combination = '3',		///组合报单
			ConditionalOrder = '4',		///条件单
			Swap = '5',		///互换单
		};
		/////////////////////////////////////////////////////////////////////////
		///TXTPTradeTypeType是一个成交类型类型
		/////////////////////////////////////////////////////////////////////////
		public enum class TTradeTypeType
		{
			XTP_TRDT_Common = '0',	///普通成交
			XTP_TRDT_OptionsExecution = '1',		///期权执行
			XTP_TRDT_OTC = '2',		///OTC成交
			XTP_TRDT_EFPDerived = '3',		///期转现衍生成交
			XTP_TRDT_CombinationDerived = '4',		///组合衍生成交
			XTP_TRDT_EFTPurchase = '5',		///ETF申购
			XTP_TRDT_EFTRedem = '6'		///ETF赎回
		};


		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_TE_RESUME_TYPE 是一个私有流重传方式
		/////////////////////////////////////////////////////////////////////////
		public enum class TE_RESUME_TYPE
		{
			XTP_TERT_RESTART = 0,	///<从本交易日开始重传
			XTP_TERT_RESUME,		///<从从上次收到的续传
			XTP_TERT_QUICK			///<只传送登录后私有流的内容
		};

		/////////////////////////////////////////////////////////////////////////
		///XTP_ORDER_SUBMIT_STATUS_TYPE是一个报单提交状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class ORDER_SUBMIT_STATUS_TYPE
		{
			XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED = 1, ///<订单已经提交
			XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED,///<订单已经被接受
			XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED,///<订单已经被拒绝
			XTP_ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED,///<撤单已经提交
			XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED,///<撤单已经被拒绝
			XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED ///<撤单已经被接受
		};

		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_ORDER_ACTION_STATUS_TYPE 是一个报单操作状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class ORDER_ACTION_STATUS_TYPE
		{
			XTP_ORDER_ACTION_STATUS_SUBMITTED = 1,	///<已经提交
			XTP_ORDER_ACTION_STATUS_ACCEPTED,		///<已经接受
			XTP_ORDER_ACTION_STATUS_REJECTED		///<已经被拒绝
		};

		/////////////////////////////////////////////////////////////////////////
		///XTP_ORDER_STATUS_TYPE是一个报单状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class ORDER_STATUS_TYPE
		{
			XTP_ORDER_STATUS_INIT = 0,///<初始化
			XTP_ORDER_STATUS_ALLTRADED = 1,           ///<全部成交
			XTP_ORDER_STATUS_PARTTRADEDQUEUEING,  ///<部分成交
			XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING, ///<部分撤单
			XTP_ORDER_STATUS_NOTRADEQUEUEING,   ///<未成交
			XTP_ORDER_STATUS_CANCELED,  ///<已撤单
			XTP_ORDER_STATUS_REJECTED,  ///<已拒绝
			XTP_ORDER_STATUS_UNKNOWN  ///<未知
		};
		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_PRICE_TYPE 是一个价格类型
		/////////////////////////////////////////////////////////////////////////
		public enum class PRICE_TYPE
		{
			XTP_PRICE_LIMIT = 1,           ///<限价单-沪/深 / 沪期权（除普通股票业务外，其余业务均使用此种类型）
			XTP_PRICE_BEST_OR_CANCEL,      ///<即时成交剩余转撤销，市价单-深 /沪期权
			XTP_PRICE_BEST5_OR_LIMIT,      ///<最优五档即时成交剩余转限价，市价单-沪
			XTP_PRICE_BEST5_OR_CANCEL,     ///<最优5档即时成交剩余转撤销，市价单-沪深
			XTP_PRICE_ALL_OR_CANCEL,       ///<全部成交或撤销,市价单-深/沪期权
			XTP_PRICE_FORWARD_BEST,        ///<本方最优，市价单-深
			XTP_PRICE_REVERSE_BEST_LIMIT,  ///<对方最优剩余转限价，市价单-深/沪期权
			XTP_PRICE_LIMIT_OR_CANCEL,	   ///<期权限价申报FOK
			XTP_PRICE_TYPE_UNKNOWN,		   ///<未知或者无效价格类型
		};

		///查询分级基金信息结构体
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryStructuredFundInfoReq
		{
			EXCHANGE_TYPE   exchange_id;  ///<交易所代码，不可为空
										  ///<分级基金母基金代码，可以为空，如果为空，则默认查询所有的分级基金
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                sf_ticker;
		};

		//////////////////////////////////////////////////////////////////////////
		///查询分级基金信息响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  StructuredFundInfo
		{
			///<交易所代码
			EXCHANGE_TYPE   exchange_id;
			///<分级基金母基金代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                sf_ticker;
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^                 sf_ticker_name; ///<分级基金母基金名称
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                 ticker;   ///<分级基金子基金代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^                 ticker_name; ///<分级基金子基金名称
			SPLIT_MERGE_STATUS	split_merge_status;   ///<基金允许拆分合并状态
			UInt32            ratio; ///<拆分合并比例
			UInt32            min_split_qty;///<最小拆分数量
			UInt32            min_merge_qty; ///<最小合并数量
			double              net_price;///<基金净值
		};

		//////////////////////////////////////////////////////////////////////////
		///查询股票ETF合约基本情况--请求结构体,
		///请求参数为多条件参数:1,不填则返回所有市场的ETF合约信息。
		///                  2,只填写market,返回该交易市场下结果
		///                   3,填写market及ticker参数,只返回该etf信息。
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryETFBaseReq
		{
			///交易市场
			MARKET_TYPE    market;
			///ETF买卖代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^               ticker;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票ETF合约基本情况--响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryETFBaseRsp
		{
			MARKET_TYPE     market;                             ///<交易市场
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                etf;                ///<etf代码,买卖,申赎统一使用该代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                subscribe_redemption_ticker;    ///<etf申购赎回代码
			Int32             unit;                               ///<最小申购赎回单位对应的ETF份数,例如上证"50ETF"就是900000
			Int32             subscribe_status;                   ///<是否允许申购,1-允许,0-禁止
			Int32             redemption_status;                  ///<是否允许赎回,1-允许,0-禁止
			double              max_cash_ratio;                     ///<最大现金替代比例,小于1的数值   TODO 是否采用double
			double              estimate_amount;                    ///<T日预估金额
			double              cash_component;                     ///<T-X日现金差额
			double              net_value;                          ///<基金单位净值
			double              total_amount;                       ///<最小申赎单位净值总金额=net_value*unit
		};



		//////////////////////////////////////////////////////////////////////////
		///查询股票ETF合约成分股信息--请求结构体,请求参数为:交易市场+ETF买卖代码
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryETFComponentReq
		{
			///交易市场
			MARKET_TYPE     market;
			///ETF买卖代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                ticker;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票ETF合约成分股信息--响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryETFComponentRsp
		{
			///交易市场
			MARKET_TYPE     market;
			///ETF代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                ticker;
			///成份股代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                component_ticker;
			///成份股名称
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^                component_name;
			///成份股数量
			Int64             quantity;
			///成份股交易市场
			MARKET_TYPE     component_market;
			///成份股替代标识
			ETF_REPLACE_TYPE    replace_type;
			///溢价比例
			double              premium_ratio;
			///成分股替代标识为必须现金替代时候的总金额
			double              amount;

		};

		//////////////////////////////////////////////////////////////////////////
		///查询当日可申购新股信息
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryIPOTickerRsp {
			///交易市场
			MARKET_TYPE     market;
			///申购代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                ticker;
			///申购股票名称
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^                ticker_name;
			///申购价格
			double              price;
			///申购单元         
			Int32             unit;
			///最大允许申购数量
			Int32             qty_upper_limit;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询用户申购额度
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryIPOQuotaRsp {
			///交易市场
			MARKET_TYPE     market;
			///可申购额度
			Int32             quantity;
		};

		//////////////////////////////////////////////////////////////////////////
///查询期权竞价交易业务参考信息--请求结构体,请求参数为:交易市场+8位期权代码
//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryOptionAuctionInfoReq {
			///交易市场
			MARKET_TYPE     market;
			///8位期权合约代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                                ticker;
		};

		//////////////////////////////////////////////////////////////////////////
		///查询期权竞价交易业务参考信息
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryOptionAuctionInfoRsp {
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                                ticker;             ///<合约编码，报单ticker采用本字段
			MARKET_TYPE     security_id_source;                 ///<证券代码源
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^                                symbol;        ///<合约简称
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^                                contract_id;   ///<合约交易代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                                underlying_security_id; ///<基础证券代码
			MARKET_TYPE     underlying_security_id_source;      ///<基础证券代码源

			UInt32            list_date;                          ///<上市日期，格式为YYYYMMDD
			UInt32            last_trade_date;                    ///<最后交易日，格式为YYYYMMDD
			TICKER_TYPE     ticker_type;                        ///<证券类别
			Int32             day_trading;                        ///<是否支持当日回转交易，1-允许，0-不允许

			OPT_CALL_OR_PUT_TYPE    call_or_put;                ///<认购或认沽
			UInt32            delivery_day;                       ///<行权交割日，格式为YYYYMMDD
			UInt32            delivery_month;                     ///<交割月份，格式为YYYYMM

			OPT_EXERCISE_TYPE_TYPE  exercise_type;              ///<行权方式
			UInt32            exercise_begin_date;                ///<行权起始日期，格式为YYYYMMDD
			UInt32            exercise_end_date;                  ///<行权结束日期，格式为YYYYMMDD
			double              exercise_price;                     ///<行权价格

			Int64             qty_unit;                           ///<数量单位，对于某一证券申报的委托，其委托数量字段必须为该证券数量单位的整数倍
			Int64             contract_unit;                      ///<合约单位
			Int64             contract_position;                  ///<合约持仓量

			double              prev_close_price;                   ///<合约前收盘价
			double              prev_clearing_price;                ///<合约前结算价

			Int64             lmt_buy_max_qty;                    ///<限价买最大量
			Int64             lmt_buy_min_qty;                    ///<限价买最小量
			Int64             lmt_sell_max_qty;                   ///<限价卖最大量
			Int64             lmt_sell_min_qty;                   ///<限价卖最小量
			Int64             mkt_buy_max_qty;                    ///<市价买最大量
			Int64             mkt_buy_min_qty;                    ///<市价买最小量
			Int64             mkt_sell_max_qty;                   ///<市价卖最大量
			Int64             mkt_sell_min_qty;                   ///<市价卖最小量

			double              price_tick;                         ///<最小报价单位
			double              upper_limit_price;                  ///<涨停价
			double              lower_limit_price;                  ///<跌停价
			double              sell_margin;                        ///<今卖开每张保证金
			double              margin_ratio_param1;                ///<交易所保证金比例计算参数一
			double              margin_ratio_param2;                ///<交易所保证金比例计算参数二

			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 20)]
			array<UInt64>^ unknown;                        ///<（保留字段）
		};


		//////////////////////////////////////////////////////////////////////////
		///融资融券直接还款响应信息
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  CrdCashRepayRsp
		{
			Int64 xtp_id;             ///< 直接还款操作的XTPID
			double  request_amount;     ///< 直接还款的申请金额
			double  cash_repay_amount;  ///< 实际还款使用金额
		};

		//////////////////////////////////////////////////////////////////////////
		///单条融资融券直接还款记录信息
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  CrdCashRepayInfo
		{
			Int64                     xtp_id;             ///< 直接还款操作的XTPID
			CRD_CR_STATUS           status;             ///< 直接还款处理状态
			double                      request_amount;     ///< 直接还款的申请金额
			double                      cash_repay_amount;  ///< 实际还款使用金额
			POSITION_EFFECT_TYPE    position_effect;    ///< 强平标志
		};

		//////////////////////////////////////////////////////////////////////////
		///单条融资融券负债记录信息
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  CrdDebtInfo
		{
			Int32             debt_type;              ///< 负债合约类型
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = 33)]
			String^                                debt_id;            ///< 负债合约编号
			Int64             position_id;            ///< 负债对应两融头寸编号
			UInt64            order_xtp_id;           ///< 生成负债的订单编号，非当日负债无此项
			Int32             debt_status;            ///< 负债合约状态
			MARKET_TYPE     market;                 ///< 市场
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                                ticker; ///< 证券代码
			UInt64            order_date;             ///< 委托日期
			UInt64            end_date;               ///< 负债截止日期
			UInt64            orig_end_date;          ///< 负债原始截止日期
			bool                is_extended;            ///< 当日是否接收到展期请求
			double              remain_amt;             ///< 未偿还金额
			Int64             remain_qty;             ///< 未偿还数量
			double              remain_principal;       ///< 未偿还本金金额
		};

		//////////////////////////////////////////////////////////////////////////
		///融资融券特有帐户数据
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  CrdFundInfo
		{
			double maintenance_ratio;       ///< 维持担保品比例
			double line_of_credit;          ///< 两融授信额度
			double guaranty;                ///< 两融保证金可用数
			double position_amount;         ///< 融资头寸可用金额，内部接口，正式版本需要删除
		};

		//////////////////////////////////////////////////////////////////////////
		///融资融券指定证券上的负债未还数量
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  ClientQueryCrdDebtStockReq
		{
			MARKET_TYPE market;                 ///< 市场
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                            ticker; ///< 证券代码
		};

		[StructLayout(LayoutKind::Sequential)]
		public ref struct  CrdDebtStockInfo
		{
			MARKET_TYPE market;                     ///< 市场
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                            ticker;     ///< 证券代码
			Int64         remain_quantity;            ///< 负债未还数量
			Int64         order_withhold_quantity;    ///< 挂单未成还券数量
		};

		//////////////////////////////////////////////////////////////////////////
		///融券头寸证券查询
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  ClientQueryCrdPositionStockReq
		{
			MARKET_TYPE market;                 ///< 证券市场
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                            ticker; ///< 证券代码
		};

		[StructLayout(LayoutKind::Sequential)]
		public ref struct  ClientQueryCrdPositionStkInfo {

			MARKET_TYPE market;                 ///< 证券市场
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                            ticker; ///< 证券代码
			Int64         limit_qty;              ///< 融券限量
			Int64         yesterday_qty;          ///< 昨日日融券数量
			Int64         left_qty;               ///< 剩余可融券数量
			Int64         frozen_qty;             ///< 冻结融券数量
		};


#pragma endregion

#pragma region quote struct

		public	enum class MARKETDATA_TYPE {
			XTP_MARKETDATA_ACTUAL = 0, // 现货(股票/基金/债券等)
			XTP_MARKETDATA_OPTION = 1, // 期权
		};

		///股票、基金、债券等额外数据
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  MarketDataStockExData {
			///委托买入总量(SH,SZ)
			Int64 total_bid_qty;
			///委托卖出总量(SH,SZ)
			Int64 total_ask_qty;
			///加权平均委买价格(SH,SZ)
			double ma_bid_price;
			///加权平均委卖价格(SH,SZ)
			double ma_ask_price;
			///债券加权平均委买价格(SH)
			double ma_bond_bid_price;
			///债券加权平均委卖价格(SH)
			double ma_bond_ask_price;
			///债券到期收益率(SH)
			double yield_to_maturity;
			///基金实时参考净值(SH,SZ)
			double iopv;
			///ETF申购笔数(SH)
			Int32 etf_buy_count;
			///ETF赎回笔数(SH)
			Int32 etf_sell_count;
			///ETF申购数量(SH)
			double etf_buy_qty;
			///ETF申购金额(SH)
			double etf_buy_money;
			///ETF赎回数量(SH)
			double etf_sell_qty;
			///ETF赎回金额(SH)
			double etf_sell_money;
			///权证执行的总数量(SH)
			double total_warrant_exec_qty;
			///权证跌停价格（元）(SH)
			double warrant_lower_price;
			///权证涨停价格（元）(SH)
			double warrant_upper_price;
			///买入撤单笔数(SH)
			Int32 cancel_buy_count;
			///卖出撤单笔数(SH)
			Int32 cancel_sell_count;
			///买入撤单数量(SH)
			double cancel_buy_qty;
			///卖出撤单数量(SH)
			double cancel_sell_qty;
			///买入撤单金额(SH)
			double cancel_buy_money;
			///卖出撤单金额(SH)
			double cancel_sell_money;
			///买入总笔数(SH)
			Int64 total_buy_count;
			///卖出总笔数(SH)
			Int64 total_sell_count;
			///买入委托成交最大等待时间(SH)
			Int32 duration_after_buy;
			///卖出委托成交最大等待时间(SH)
			Int32 duration_after_sell;
			///买方委托价位数(SH)
			Int32 num_bid_orders;
			///卖方委托价位数(SH)
			Int32 num_ask_orders;

			///基金T-1日净值(SZ)
			double pre_iopv;
			///预留
			Int64 r1;
			///预留
			Int64 r2;
		};

		// 期权额外数据
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  MarketDataOptionExData {
			///波段性中断参考价(SH)
			double  auction_price;
			///波段性中断集合竞价虚拟匹配量(SH)
			Int64 auction_qty;
			///最近询价时间(SH)
			Int64 last_enquiry_time;
		};
		///行情
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  MarketDataStruct
		{
			// 代码
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息）
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^	ticker;

			// 股票等价格
			///最新价
			double	last_price;
			///昨收盘
			double	pre_close_price;
			///今开盘
			double	open_price;
			///最高价
			double	high_price;
			///最低价
			double	low_price;
			///今收盘
			double	close_price;

			// 期权等数据
			Int64 pre_total_long_positon;
			///持仓量(张)
			Int64	total_long_positon;
			///昨日结算价
			double	pre_settl_price;
			///今日结算价
			double	settl_price;

			// 涨跌停
			///涨停价
			double	upper_limit_price;
			///跌停价
			double	lower_limit_price;
			///预留
			double	pre_delta;
			///预留
			double	curr_delta;

			/// 时间类
			Int64 data_time;

			// 量额数据
			///数量
			Int64	qty;
			///成交金额
			double	turnover;
			///当日均价
			double	avg_price;

			// 买卖盘
			///十档申买价
			array<double>^ bid;
			///十档申卖价
			array<double>^	ask;
			///十档申买量
			array<Int64>^	bid_qty;
			///十档申卖量
			array<Int64>^	ask_qty;

			// lts没有包含的数据（目前未填写）
			///成交笔数
			Int64 trades_count;
			///当前交易状态说明
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = 8)]
			String^ ticker_status;
			MarketDataStockExData stk;
			MarketDataOptionExData opt;
			///决定了union是哪种数据类型
			MARKETDATA_TYPE data_type;
			///预留
			Int32 r4;
		};

		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QuoteStaticInfoStruct {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^ ticker;
			/// 合约名称
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^ ticker_name;
			/// 合约类型
			TICKER_TYPE ticker_type;
			///昨收盘
			double  pre_close_price;
			///涨停板价
			double  upper_limit_price;
			///跌停板价
			double  lower_limit_price;
			///最小变动价位
			double  price_tick;
			/// 合约最小交易量(买)
			Int32  buy_qty_unit;
			/// 合约最小交易量(卖)
			Int32 sell_qty_unit;
		};
		///定单薄
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  OrderBook {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^    ticker;

			///最新价
			double last_price;
			///数量，为总成交量
			Int64 qty;
			///成交金额，为总成交金额
			double  turnover;
			///成交笔数
			Int64 trades_count;

			// 买卖盘
			///十档申买价
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 10)]
			array<Double>^ bid;
			//double bid[10];
			///十档申卖价
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 10)]
			array<Double>^  ask;
			///十档申买量
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 10)]
			array<Int64>^ bid_qty;
			///十档申卖量
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 10)]
			array<Int64>^ ask_qty;
			/// 时间类
			Int64 data_time;
		};

		///逐笔委托(仅适用深交所)
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  TickByTickEntrust {
			///频道代码
			Int32 channel_no;
			///委托序号(在同一个channel_no内唯一，从1开始连续)
			Int64 seq;
			///委托价格
			double  price;
			///委托数量
			Int64 qty;
			///'1':买; '2':卖; 'G':借入; 'F':出借
			char  side;
			///订单类别: '1': 市价; '2': 限价; 'U': 本方最优
			char ord_type;
		};

		///逐笔成交
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  TickByTickTrade {
			///频道代码
			Int32 channel_no;
			///委托序号(在同一个channel_no内唯一，从1开始连续)
			Int64 seq;
			///成交价格
			double price;
			///成交量
			Int64 qty;
			///成交金额(仅适用上交所)
			double money;
			///买方订单号
			Int64 bid_no;
			///卖方订单号
			Int64 ask_no;
			/// SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)
			/// SZ: 成交标识('4':撤; 'F':成交)
			char trade_flag;
		};

		///逐笔数据信息
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  TickByTickStruct {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^ ticker;
			///预留
			Int64 seq;
			///委托时间 or 成交时间
			Int64 data_time;
			///委托 or 成交
			//TBT_TYPE type;
			//TODO:define XTPTickByTickStruct
			TickByTickEntrust entrust;
			TickByTickTrade     trade;

			/*union {
				TickByTickEntrust entrust;
				TickByTickTrade     trade;
			};*/
		};


		///供查询的最新信息
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  TickerPriceInfo {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^ ticker;
			///最新价
			double last_price;
		};

#pragma endregion


#pragma region trade struct

		
		/// <summary>
		///报单信息
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  OrderInsertInfo
		{
			///XTP系统订单ID，无需用户填写
			UInt64                order_xtp_id;
			///报单引用，由客户自定义，小于1000000
			UInt32	            order_client_id;
			///合约代码 客户端请求不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                    ticker;
			///交易市场
			MARKET_TYPE         market;
			///价格
			double                  price;
			///止损价（保留字段）
			double                  stop_price;
			///数量
			Int64                 quantity;
			///报单价格
			PRICE_TYPE          price_type;
			///买卖方向
			SIDE_TYPE           side;
			///业务类型
			BUSINESS_TYPE       business_type;
		};

		//////////////////////////////////////////////////////////////////////////
		///报单查询
		//////////////////////////////////////////////////////////////////////////
		///报单查询请求-条件查询
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryOrderReq
		{
			///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^    ticker;
			///格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
			UInt64   begin_time;
			///格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			UInt64   end_time;
		};

		///查询成交回报请求-查询条件(和之前其实一样）
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryTraderReq
		{
			///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^    ticker;
			///开始时间，格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
			Int64   begin_time;
			///结束时间，格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			Int64   end_time;
		};

		//////////////////////////////////////////////////////////////////////////
		///账户资金查询响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryAssetRspStruct
		{
			///总资产
			double total_asset;
			///可用资金
			double buying_power;
			///证券资产
			double security_asset;
			///累计买入成交证券占用资金（保留字段）
			double fund_buy_amount;
			///累计买入成交交易费用（保留字段）
			double fund_buy_fee;
			///累计卖出成交证券所得资金（保留字段）
			double fund_sell_amount;
			///累计卖出成交交易费用（保留字段）
			double fund_sell_fee;
			//XTP系统预扣的资金（包括购买卖股票时预扣的交易资金+预扣手续费）
			double withholding_amount;
			///账户类型
			ACCOUNT_TYPE account_type;

			//UInt64 unknown[43];
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 43)]
			array<UInt64>^ unknown;
		};


		///报单响应结构体
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  OrderInfoStruct
		{
			///XTP系统订单ID
			UInt64                order_xtp_id;
			///报单引用，用户自定义，小于1000000
			UInt32	            order_client_id;
			///报单操作引用，用户自定义，小于1000000
			UInt32                order_cancel_client_id;
			///撤单在XTP系统中的id
			UInt64                order_cancel_xtp_id;
			///合约代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                    ticker;
			///交易市场
			MARKET_TYPE         market;
			///价格
			double                  price;
			///数量
			Int64                 quantity;
			///报单价格条件
			PRICE_TYPE          price_type;
			///买卖方向
			SIDE_TYPE           side;
			///业务类型
			BUSINESS_TYPE       business_type;
			///今成交数量
			Int64                 qty_traded;
			///剩余数量，当撤单成功时，表示撤单数量
			Int64                 qty_left;
			///委托时间
			Int64                 insert_time;
			///最后修改时间
			Int64                 update_time;
			///撤销时间
			Int64                 cancel_time;
			///成交金额
			double                  trade_amount;
			///本地报单编号 OMS生成的单号
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = LOCAL_ORDER_LEN)]
			String^                    order_local_id;
			///报单状态
			ORDER_STATUS_TYPE   order_status;
			///报单提交状态
			ORDER_SUBMIT_STATUS_TYPE   order_submit_status;
			///TODO:报单类型
			TOrderTypeType       order_type;
		};

		///撤单失败响应消息
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  OrderCancelInfoStruct
		{
			///撤单XTPID
			UInt64                 order_cancel_xtp_id;
			///原始订单XTPID
			UInt64                 order_xtp_id;
		};

		///报单成交结构体
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  TradeReportStruct
		{
			///XTP系统订单ID
			UInt64                 order_xtp_id;
			///报单引用，小于1000000
			UInt32                order_client_id;
			///合约代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^                     ticker;
			///交易市场
			MARKET_TYPE          market;
			///订单号
			UInt64                 local_order_id;
			///成交编号，深交所唯一，上交所每笔交易唯一
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = EXEC_ID_LEN)]
			String^                    exec_id;
			///价格
			double                   price;
			///数量
			Int64                  quantity;
			///成交时间
			Int64                  trade_time;
			///成交金额
			double                   trade_amount;
			///成交序号 --回报记录号，每个交易所唯一
			UInt64                 report_index;
			///报单编号 --交易所单号
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = ORDER_EXCH_LEN)]
			String^                     order_exch_id;
			///成交类型  --成交回报中的执行类型
			TTradeTypeType        trade_type;
			///买卖方向
			SIDE_TYPE            side;
			///交易所交易员代码 
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = BRANCH_PBU_LEN)]
			String^                    branch_pbu;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票持仓情况
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  QueryStkPositionStruct
		{
			///证券代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_LEN)]
			String^	 ticker;
			///证券名称
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = TICKER_NAME_LEN)]
			String^		ticker_name;
			///交易市场
			MARKET_TYPE     market;
			///当前持仓
			Int64             total_qty;
			///可用股份数
			Int64             sellable_qty;
			///持仓成本
			double              avg_price;
			///浮动盈亏
			double              unrealized_pnl;
			///昨日持仓
			Int64             yesterday_position;
			///今日申购赎回数量（申购和赎回数量不可能同时存在，因此可以共用一个字段）
			Int64				purchase_redeemable_qty;
			///(保留字段)
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 50)]
			array<UInt64>^ unknown;
		};

		/////////////////////////////////////////////////////////////////////////
		///资金内转流水通知
		/////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  FundTransferNotice
		{
			///资金内转编号
			UInt64	            serial_id;
			///内转类型
			FUND_TRANSFER_TYPE	transfer_type;
			///金额
			double	                amount;
			///操作结果 
			FUND_OPER_STATUS    oper_status;
			///操作时间
			UInt64	            transfer_time;
		};

		/////////////////////////////////////////////////////////////////////////
		///用户资金请求
		/////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  FundTransferReq
		{
			///资金内转编号，无需用户填写，类似于xtp_id
			UInt64	serial_id;
			///资金账户代码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = ACCOUNT_NAME_LEN)]
			String^        fund_account;
			///资金账户密码
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = ACCOUNT_PASSWORD_LEN)]
			String^	    password;
			///金额
			double	    amount;
			///内转类型
			FUND_TRANSFER_TYPE	transfer_type;
		};
		/////////////////////////////////////////////////////////////////////////
		///资金内转流水查询请求与响应
		/////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)]
		public ref struct QueryFundTransferLogReq {
			///资金内转编号
			UInt64	serial_id;
		};

#pragma endregion

	}
}
