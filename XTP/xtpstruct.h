#pragma once
#include"..\sdk\include\xoms_api_fund_struct.h"

using namespace System;
using namespace::Runtime::InteropServices;
namespace XTP
{
	namespace API {

		/// 存放版本号的字符串长度
#define XTP_VERSION_LEN 16
	/// 版本号类型
		typedef char XTPVersionType[XTP_VERSION_LEN];
		/// 可交易日字符串长度
#define XTP_TRADING_DAY_LEN 9
	/// 存放证券代码的字符串长度
#define XTP_TICKER_LEN 16
	/// 存放证券名称的字符串长度
#define XTP_TICKER_NAME_LEN 64
	/// 本地报单编号的字符串长度
#define XTP_LOCAL_ORDER_LEN         11
	/// 交易所单号的字符串长度
#define XTP_ORDER_EXCH_LEN          17
	/// 成交执行编号的字符串长度
#define XTP_EXEC_ID_LEN             18
	/// 交易所交易员代码字符串长度
#define XTP_BRANCH_PBU_LEN          7
	/// 用户资金账户的字符串长度
#define XTP_ACCOUNT_NAME_LEN        16
	///错误信息的字符串长度
#define XTP_ERR_MSG_LEN  124
#pragma region basic struct

	/// <summary>
	/// XTP_PROTOCOL_TYPE 是一个通讯传输协议方式
	/// </summary>
		public enum class PROTOCOL_TYPE
		{
			XTP_PROTOCOL_TCP = 1,	///<采用TCP方式传输
			XTP_PROTOCOL_UDP		///<采用UDP方式传输
		};

		[StructLayout(LayoutKind::Sequential)] 
		public ref struct  RspInfoStruct
		{
			///错误代码
			Int32	error_id;
			///错误信息
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = XTP_ERR_MSG_LEN)]
			String^	error_msg;
		};

		///指定的合约
		[StructLayout(LayoutKind::Sequential)]
		public ref struct  SpecificTickerStruct
		{
			///交易所代码
			int exchange_id;
			///合约代码（不包含交易所信息）例如"600000  "
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = XTP_TICKER_LEN)]
			String^	ticker;
		};
#pragma endregion


#pragma region api data struct
		//////////////////////////////////////////////////////////////////////////
		///@brief XTP_ACCOUNT_TYPE账户类型
		//////////////////////////////////////////////////////////////////////////
		public enum ACCOUNT_TYPE
		{
			XTP_ACCOUNT_NORMAL = 0,	///<普通账户
			XTP_ACCOUNT_CREDIT,		///<信用账户
			XTP_ACCOUNT_DERIVE,		///<衍生品账户
			XTP_ACCOUNT_UNKNOWN		///<未知账户类型
		};

		/// <summary>
		/// XTP_EXCHANGE_TYPE 是交易所类型
		/// </summary>
		public enum class EXCHANGE_TYPE
		{
			XTP_EXCHANGE_SH = 1,	///<上证
			XTP_EXCHANGE_SZ,		///<深证
			XTP_EXCHANGE_MAX	///<不存在的交易所类型
		};
		/////////////////////////////////////////////////////////////////////////
		///@brief SPLIT_MERGE_STATUS是一个基金当天拆分合并状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum SPLIT_MERGE_STATUS {
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
		public enum struct TradeTypeType:Byte
		{			
			COMMON = '0',//普通成交
			CASH = '1',//现金替代
			PRIMARY ='2'//一级市场成交
		};
		///查询分级基金信息结构体
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryStructuredFundInfoReq
		{
			EXCHANGE_TYPE   exchange_id;  ///<交易所代码，不可为空
										  ///<分级基金母基金代码，可以为空，如果为空，则默认查询所有的分级基金
			String^                sf_ticker;
		};

		//////////////////////////////////////////////////////////////////////////
		///查询分级基金信息响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)] public ref struct  StructuredFundInfo
		{
			///<交易所代码
			EXCHANGE_TYPE   exchange_id;
			///<分级基金母基金代码
			String^                sf_ticker;
			String^                 sf_ticker_name; ///<分级基金母基金名称
			String^                 ticker;   ///<分级基金子基金代码
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryETFBaseReq
		{
			///交易市场
			MARKET_TYPE    market;
			///ETF买卖代码
			String^               ticker;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票ETF合约基本情况--响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryETFBaseRsp
		{
			MARKET_TYPE     market;                             ///<交易市场
			String^                etf;                ///<etf代码,买卖,申赎统一使用该代码
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryETFComponentReq
		{
			///交易市场
			MARKET_TYPE     market;
			///ETF买卖代码
			String^                ticker;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票ETF合约成分股信息--响应结构体
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryETFComponentRsp
		{
			///交易市场
			MARKET_TYPE     market;
			///ETF代码
			String^                ticker;
			///成份股代码
			String^                component_ticker;
			///成份股名称
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryIPOTickerRsp {
			///交易市场
			MARKET_TYPE     market;
			///申购代码
			String^                ticker;
			///申购股票名称
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryIPOQuotaRsp {
			///交易市场
			MARKET_TYPE     market;
			///可申购额度
			Int32             quantity;
		};

		


		//////////////////////////////////////////////////////////////////////////
		///@brief XTP_TICKER_TYPE证券类型
		//////////////////////////////////////////////////////////////////////////
		public enum TICKER_TYPE
		{
			XTP_TICKER_TYPE_BOND,                 ///<债券

		};


		public enum BUSINESS_TYPE
		{
			XTP_BUSINESS_TYPE_CASH = 0,            ///<普通股票业务（股票买卖，ETF买卖等）
			XTP_BUSINESS_TYPE_IPOS,                ///<新股申购业务（对应的price type需选择限价类型）
			XTP_BUSINESS_TYPE_REPO,                ///<回购业务 ( 对应的price type填为限价，side填为卖 )
			XTP_BUSINESS_TYPE_ETF,                 ///<ETF申赎业务（暂未支持）
			XTP_BUSINESS_TYPE_MARGIN,              ///<融资融券业务（暂未支持）
			XTP_BUSINESS_TYPE_DESIGNATION,         ///<转托管（未支持）
			XTP_BUSINESS_TYPE_ALLOTMENT,		   ///<配股业务（对应的price type需选择限价类型,side填为买）
			XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION,	   ///<分级基金申赎业务
			XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE,	   ///<分级基金拆分合并业务
			XTP_BUSINESS_TYPE_MONEY_FUND,		   ///<货币基金业务（暂未支持）
			XTP_BUSINESS_TYPE_UNKNOWN              ///<未知类型

		};

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_TBT_TYPE是一个逐笔回报类型
		/////////////////////////////////////////////////////////////////////////
		public enum TBT_TYPE {
			XTP_TBT_ENTRUST = 1,	///<逐笔委托
			XTP_TBT_TRADE = 2,		///<逐笔成交
		};

		#pragma endregion

		#pragma region quote struct


		///行情
		[StructLayout(LayoutKind::Sequential)] 
		public ref struct  MarketDataStruct
		{
			// 代码
			///交易所代码
			int exchange_id;
			///合约代码（不包含交易所信息）
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = XTP_TICKER_LEN)]
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

			// 期货等数据
			///昨持仓量（目前未填写）
			double	pre_open_interest;
			///持仓量（目前未填写）
			double	open_interest;
			///上次结算价（目前未填写）
			double	pre_settlement_price;
			///本次结算价（目前未填写）
			double	settlement_price;

			///涨停板价（目前未填写）
			double	upper_limit_price;
			///跌停板价（目前未填写）
			double	lower_limit_price;
			///昨虚实度（目前未填写）
			double	pre_delta;
			///今虚实度（目前未填写）
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
			String^ ticker_status;
			///委托买入总量
			Int64 total_bid_qty;
			///委托卖出总量
			Int64 total_ask_qty;
			///加权平均委买价格
			double ma_bid_price;
			///加权平均委卖价格
			double ma_ask_price;
			///债券加权平均委买价格
			double ma_bond_bid_price;
			///债券加权平均委卖价格
			double ma_bond_ask_price;
			///债券到期收益率
			double yield_to_maturity;
			///ETF净值估值
			double iopv;
			///ETF申购笔数
			Int32 etf_buy_count;
			///ETF赎回笔数
			Int32 etf_sell_count;
			///ETF申购数量
			double etf_buy_qty;
			///ETF申购金额
			double etf_buy_money;
			///ETF赎回数量
			double etf_sell_qty;
			///ETF赎回金额
			double etf_sell_money;
			///权证执行的总数量
			double total_warrant_exec_qty;
			///权证跌停价格（元）
			double warrant_lower_price;
			///权证涨停价格（元）
			double warrant_upper_price;
			///买入撤单笔数
			Int32 cancel_buy_count;
			///卖出撤单笔数
			Int32 cancel_sell_count;
			///买入撤单数量
			double cancel_buy_qty;
			///卖出撤单数量
			double cancel_sell_qty;
			///买入撤单金额
			double cancel_buy_money;
			///卖出撤单金额
			double cancel_sell_money;
			///买入总笔数
			Int64 total_buy_count;
			///卖出总笔数
			Int64 total_sell_count;
			///买入委托成交最大等待时间
			Int32 duration_after_buy;
			///卖出委托成交最大等待时间
			Int32 duration_after_sell;
			///买方委托价位数
			Int32 num_bid_orders;
			///卖方委托价位数
			Int32 num_ask_orders;
			///成交时间（UA3113）
			Int32 exec_time;
			///闭市标志（UA103/UA104）
			String^ is_market_closed;
			///合约持仓量（UA103）
			double total_position;
			///市盈率1（UA103）
			double pe_ratio1;
			///市盈率2（UA103）
			double pe_ratio2;
		};

		[StructLayout(LayoutKind::Sequential)] public ref struct  QuoteStaticInfoStruct {
			///交易所代码
			int exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = XTP_TICKER_LEN)]
			String^ ticker;
			/// 合约名称
			String^ ticker_name;
			/// 合约类型
			int ticker_type;
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  OrderBook {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			[MarshalAs(UnmanagedType::ByValTStr, SizeConst = XTP_TICKER_LEN)]
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  TickByTickEntrust {
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
			///订单类别: '1': 市价; '2': 限价; '3': 本方最优
			char ord_type;
		};

		///逐笔成交
		[StructLayout(LayoutKind::Sequential)] public ref struct  TickByTickTrade {
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
		public ref struct  XTPTickByTickStruct {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  XTPTickerPriceInfo {
			///交易所代码
			EXCHANGE_TYPE exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			String^ ticker;
			///最新价
			double last_price;
		};

#pragma endregion


#pragma region trade struct



		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_SIDE_TYPE 是一个买卖方向类型
		/////////////////////////////////////////////////////////////////////////
		public enum class SIDE_TYPE
		{
			XTP_SIDE_BUY = 1,	///<买
			XTP_SIDE_SELL,		///<卖
			XTP_SIDE_BUY_OPEN,		///<买开
			XTP_SIDE_SELL_OPEN,		///<卖开
			XTP_SIDE_BUY_CLOSE,		///<买平
			XTP_SIDE_SELL_CLOSE		///<卖平
		};


		/////////////////////////////////////////////////////////////////////////
		///TXTPOrderTypeType是一个报单类型类型
		/////////////////////////////////////////////////////////////////////////
		public enum class TOrderTypeType
		{
			Normal = '0',	///正常
			DeriveFromQuote= '1',		///报价衍生
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
			XTP_PRICE_LIMIT = 1,           ///<限价单
			XTP_PRICE_BEST_OR_CANCEL,      ///<即时成交剩余转撤销，市价单
			XTP_PRICE_BEST5_OR_LIMIT,      ///<最优五档即时成交剩余转限价，市价单
			XTP_PRICE_BEST5_OR_CANCEL,     ///<最优5档即时成交剩余转撤销，市价单
			XTP_PRICE_ALL_OR_CANCEL,       ///<全部成交或撤销,市价单
			XTP_PRICE_FORWARD_BEST,        ///<本方最优，市价单
			XTP_PRICE_REVERSE_BEST_LIMIT,  ///<对方最优剩余转限价，市价单
			XTP_PRICE_TYPE_MAX,			///<价格类型个数
		};

		[StructLayout(LayoutKind::Sequential)] public ref struct  XTPOrderInsert
		{
			///XTP系统订单ID，无需用户填写
			UInt64                order_xtp_id;
			///报单引用，由客户自定义，小于1000000
			UInt32	            order_client_id;
			///合约代码 客户端请求不带空格，以'\0'结尾
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
		};

		//////////////////////////////////////////////////////////////////////////
		///报单查询
		//////////////////////////////////////////////////////////////////////////
		///报单查询请求-条件查询
		[StructLayout(LayoutKind::Sequential)] public ref struct  XTPQueryOrder
		{
			///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
			String^    ticker;
			///格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
			UInt64   begin_time;
			///格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			UInt64   end_time;
		};

		///查询成交回报请求-查询条件(和之前其实一样）
		[StructLayout(LayoutKind::Sequential)] public ref struct  XTPQueryTrader
		{
			///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
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
		[StructLayout(LayoutKind::Sequential)] public ref struct  OrderInfoStruct
		{
			///XTP系统订单ID
			UInt64                order_xtp_id;
			///报单引用，用户自定义，小于1000000
			UInt32	            order_client_id;
			///报单操作引用，用户自定义，小于1000000
			UInt32                order_cancel_client_id;
			///撤单在XTP系统中的id
			UInt32                order_cancel_xtp_id;
			///合约代码
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
			String^                    order_local_id;
			///报单状态
			ORDER_STATUS_TYPE   order_status;
			///报单提交状态
			ORDER_SUBMIT_STATUS_TYPE   order_submit_status;
			///TODO:报单类型
			TOrderTypeType       order_type;
		};

		///撤单失败响应消息
		[StructLayout(LayoutKind::Sequential)] public ref struct  OrderCancelInfoStruct
		{
			///撤单XTPID
			UInt64                 order_cancel_xtp_id;
			///原始订单XTPID
			UInt64                 order_xtp_id;
		};

		///报单成交结构体
		[StructLayout(LayoutKind::Sequential)] public ref struct  TradeReportStruct
		{
			///XTP系统订单ID
			UInt64                 order_xtp_id;
			///报单引用，小于1000000
			UInt32                order_client_id;
			///合约代码
			String^                     ticker;
			///交易市场
			MARKET_TYPE          market;
			///订单号
			UInt64                 local_order_id;
			///成交编号，深交所唯一，上交所每笔交易唯一
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
			String^                     order_exch_id;
			///成交类型  --成交回报中的执行类型
			TTradeTypeType        trade_type;
			///买卖方向
			SIDE_TYPE            side;
			///交易所交易员代码 
			String^                    branch_pbu;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票持仓情况
		//////////////////////////////////////////////////////////////////////////
		[StructLayout(LayoutKind::Sequential)] public ref struct  QueryStkPositionStruct
		{
			///证券代码
			String^	 ticker;
			///证券名称
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

#pragma endregion

}
}
