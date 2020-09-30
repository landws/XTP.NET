using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using XTP.API;
using System.IO;
using Newtonsoft.Json;

namespace XtpTest
{
    public partial class frmXtpTest : Form
    {
        //【中泰证券】马飞您好，您申请的XTP测试帐号已发放，帐号: 53191002073，密码: tdJClz8S，Key: b8aa7173bba3470e390d787219b2112e
        //QQ交流群： 188500929
        //支持电话： 021-20235564(徐士杰)
        //测试环境详细信息将通过邮件形式发送您的预留邮箱，请注意查收，谢谢回td退【中泰证券】
        public frmXtpTest()
        {
            InitializeComponent();
        }
        XtpTraderAdapter trader;
        XtpQuoteAdapter quoter;
        int client_id = 1;
        string log_path = "xtp_log";
        string key= "b8aa7173bba3470e390d787219b2112e";
        string userid= "53191002073";
        string pwd= "tdJClz8S";
        string quote_ip= "120.27.164.138";
        int quote_port= 6002;
        int trade_port= 6001;
        string trade_ip= "120.27.164.69";
        ulong tradeSessionID = 0;
        Dictionary<string, QuoteStaticInfoStruct> SymbolMap = new Dictionary<string, QuoteStaticInfoStruct>();
        Dictionary<string, double> tickMap = new Dictionary<string, double>();

        private void frmXtpTest_Load(object sender, EventArgs e)
        {
            if (Directory.Exists(log_path) == false)
            {
                Directory.CreateDirectory(log_path);
            }
            string fname = "account.json";
            if (File.Exists(fname) == false)
            {
                MessageBox.Show("请配置账户信息");
                return;
            }
            string content = File.ReadAllText(fname);
            config = JsonConvert.DeserializeObject<AccountConfig>(content);

            //加载账号配置
            client_id = 1;
            key = config.Token;
            userid = config.UserID;
            pwd = config.Password;
            quote_ip = config.QuoteIP;
            quote_port = config.QuotePort;
            trade_port = config.TradePort;
            trade_ip = config.TradeIP;
        }

        AccountConfig config = null;

        private void log(string fmt, params object[] args)
        {
            string msg = string.Format("{0}:  {1}", DateTime.Now.ToString("yyyyMMdd HH:mm:ss"),
                      string.Format(fmt, args));
            Console.WriteLine(msg);
            BeginInvoke((Action)delegate {               
                rtxtLog.AppendText(string.Format("{0}\r\n", msg));
                txtLog.AppendText(string.Format("{0}\r\n", msg));
            });
        }

        private bool checkStatus(bool isTrader=true)
        {
            bool result = false;
            if (isTrader)
            {
                result = trader == null || trader.IsLogin == false ? false : true;
                if (result == false)
                {
                    MessageBox.Show("TD 未登录");
                }
            }
            else
            {
                result = quoter == null || quoter.IsLogin == false ? false : true;
                if (result == false)
                {
                    MessageBox.Show("MD 未登录");
                }
            }
            return result;
        }
       
        #region TD 请求
        private void btnOrder_Click(object sender, EventArgs e)
        {
            if (checkStatus()==false)
            {
                return;
            }
            foreach (var item in tickMap)
            {
                var order = new OrderInsertInfo()
                {
                    market = MARKET_TYPE.XTP_MKT_SZ_A,
                    order_client_id = (uint)client_id,
                    quantity = 200,
                    //order_xtp_id = 0,
                    price = item.Value,
                    price_type = PRICE_TYPE.XTP_PRICE_LIMIT,
                    side = SIDE_TYPE.BUY,
                    ticker = item.Key
                };
                trader.InsertOrder(order, tradeSessionID);
            }
        }

        private void btnConnTrader_Click(object sender, EventArgs e)
        {
            if (trader != null && trader.IsLogin == true)
            {
                return;
            }
            trader = new XtpTraderAdapter(client_id, key, log_path, TE_RESUME_TYPE.XTP_TERT_QUICK);
            trader.OnDisconnectedEvent += Trader_OnDisconnectedEvent;
            trader.OnErrorEvent += Trader_OnErrorEvent;
            trader.OnOrderCancelEvent += Trader_OnOrderCancelEvent;
            trader.OnOrderEvent += Trader_OnOrderEvent;
            trader.OnQueryAssetEvent += Trader_OnQueryAssetEvent;
            trader.OnQueryOrderEvent += Trader_OnQueryOrderEvent;
            trader.OnQueryPositionEvent += Trader_OnQueryPositionEvent;
            trader.OnQueryTradeEvent += Trader_OnQueryTradeEvent;
            trader.OnTradeEvent += Trader_OnTradeEvent;

            tradeSessionID = trader.Login(trade_ip, trade_port, userid, pwd, PROTOCOL_TYPE.XTP_PROTOCOL_TCP);

            if (trader.IsLogin)
            {
                log("TD:{0} login success", userid);
            }
            else
            {
                var rsp = trader.GetApiLastError();
                log("TD {0} login fail:{1}/{2}", userid, rsp.error_id, rsp.error_msg);
            }
        }

        private void btnCancelOrder_Click(object sender, EventArgs e)
        {
            if (checkStatus() == false)
            {
                return;
            }
        }

        private void btnQryOrder_Click(object sender, EventArgs e)
        {
            if (checkStatus() == false)
            {
                return;
            }
        }

        private void btnQryTrade_Click(object sender, EventArgs e)
        {
            if (checkStatus() == false)
            {
                return;
            }
            trader.QueryTrades(new QueryTraderReq(), tradeSessionID, 0);
        }
        #endregion

        #region TD 回调

        private void Trader_OnTradeEvent(TradeReportStruct trade, UInt64 session_id)
        {
            log("Trader_OnTradeEvent():{0} {1} {2}@{3}", trade.ticker, trade.side.ToString(), trade.quantity, trade.price);
        }

        private void Trader_OnQueryTradeEvent(RspInfoStruct A_0, TradeReportStruct A_1, int A_2, bool A_3, UInt64 session_id)
        {
            log("Trader_OnQueryTradeEvent");
        }

        private void Trader_OnQueryPositionEvent( QueryStkPositionRsp A_1, RspInfoStruct A_0, int A_2, bool A_3, UInt64 session_id)
        {
            log("Trader_OnQueryPositionEvent");
        }

        private void Trader_OnQueryOrderEvent(RspInfoStruct A_0, OrderInfoStruct A_1, int A_2, bool A_3, UInt64 session_id)
        {
            log("Trader_OnQueryOrderEvent");
        }

        private void Trader_OnQueryAssetEvent(QueryAssetRsp A_1, RspInfoStruct A_0, int A_2, bool A_3, UInt64 session_id)
        {
            log("Trader_OnQueryAssetEvent");
        }

        private void Trader_OnOrderEvent(RspInfoStruct rsp, OrderInfoStruct orderInfo, UInt64 session_id)
        {
            log("Trader_OnOrderEvent():");
            if (rsp.error_id == 0)
            {
                log("{0} {1} {2} Traded:{3} Remain:{4}", orderInfo.ticker, orderInfo.side.ToString(), orderInfo.quantity,
                    orderInfo.qty_traded, orderInfo.trade_amount);
            }
            else
            {
                log("{0}:{1}", rsp.error_id, rsp.error_msg);
            }
        }

        private void Trader_OnOrderCancelEvent(RspInfoStruct A_0, OrderCancelInfoStruct A_1, UInt64 session_id)
        {
            log("Trader_OnOrderCancelEvent():");
        }

        private void Trader_OnErrorEvent(RspInfoStruct A_0)
        {
            log("Trader_OnErrorEvent():{0}", A_0.error_msg);
        }

        private void Trader_OnDisconnectedEvent(ulong A_0, int A_1)
        {
            log("Trader_OnDisconnectedEvent");
        }

        #endregion
      
        #region MD 功能请求
        private void btnConnectQuote_Click(object sender, EventArgs e)
        {
            quoter = new XtpQuoteAdapter(client_id, log_path, LOG_LEVEL.XTP_LOG_LEVEL_WARNING);
            quoter.OnSubMarketDataEvent += Quoter_OnSubMarketDataEvent;
            quoter.OnDisconnectedEvent += Quoter_OnDisconnectedEvent;
            quoter.OnErrorEvent += Quoter_OnErrorEvent;
            quoter.OnDepthMarketDataEvent += Quoter_OnDepthMarketDataEvent;
            quoter.OnQueryAllTickersEvent += Quoter_OnQueryAllTickersEvent;
            quoter.OnUnSubMarketDataEvent += Quoter_OnUnSubMarketDataEvent;
            quoter.OnOrderBookEvent += Quoter_OnOrderBookEvent;
            quoter.OnQueryTickersPriceInfoEvent += Quoter_OnQueryTickersPriceInfoEvent;
            quoter.OnSubOrderBookEvent += Quoter_OnSubOrderBookEvent;
            quoter.OnSubscribeAllMarketDataEvent += Quoter_OnSubscribeAllMarketDataEvent;
            quoter.OnSubscribeAllOrderBookEvent += Quoter_OnSubscribeAllOrderBookEvent;
            quoter.OnSubscribeAllTickByTickEvent += Quoter_OnSubscribeAllTickByTickEvent;
            quoter.OnSubTickByTickEvent += Quoter_OnSubTickByTickEvent;
            quoter.OnTickByTickEvent += Quoter_OnTickByTickEvent;
            quoter.OnUnSubOrderBookEvent += Quoter_OnUnSubOrderBookEvent;
            quoter.OnUnSubscribeAllMarketDataEvent += Quoter_OnUnSubscribeAllMarketDataEvent;
            quoter.OnUnSubscribeAllOrderBookEvent += Quoter_OnUnSubscribeAllOrderBookEvent;
            quoter.OnUnSubscribeAllTickByTickEvent += Quoter_OnUnSubscribeAllTickByTickEvent;
            quoter.OnUnSubTickByTickEvent += Quoter_OnUnSubTickByTickEvent;
            quoter.Login(quote_ip, quote_port, userid, pwd, PROTOCOL_TYPE.XTP_PROTOCOL_TCP);
            if (quoter.IsLogin)
            {
                log("MD {0} login success", userid);
            }
            else
            {
                var rsp = quoter.GetApiLastError();
                log("MD {0} login fail:{1}/{2}", userid, rsp.error_id, rsp.error_msg);
            }
        }

        private void btnQryTickers_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.QueryAllTickers(EXCHANGE_TYPE.XTP_EXCHANGE_SH);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.QueryTickersPriceInfo(new string[] { symbol }, exchange);
            }
        }

        private void btnSubMD_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.SubscribeAllMarketData( EXCHANGE_TYPE.XTP_EXCHANGE_SZ,true);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.SubscribeMarketData(new string[] { symbol }, exchange, true);
            }
        }

        private void btnUnsubMD_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.SubscribeAllMarketData( EXCHANGE_TYPE.XTP_EXCHANGE_SZ,false);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.SubscribeMarketData(new string[] { symbol }, exchange, false);
            }
        }

        private void btnSubOrderBook_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.SubscribeAllOrderBook( EXCHANGE_TYPE.XTP_EXCHANGE_SZ,true);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.SubscribeOrderBook(new string[] { symbol }, exchange, true);
            }
        }

        private void btnUnsubOrderBook_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.SubscribeAllOrderBook( EXCHANGE_TYPE.XTP_EXCHANGE_SZ,false);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.SubscribeOrderBook(new string[] { symbol }, exchange, false);
            }
        }

        private void btnSubTick_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.SubscribeAllTickByTick( EXCHANGE_TYPE.XTP_EXCHANGE_SZ,true);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.SubscribeTickByTick(new string[] { symbol }, exchange, true);
            }
        }

        private void btnUnsubTick_Click(object sender, EventArgs e)
        {
            if (checkStatus(false) == false)
            {
                return;
            }
            string symbol = txtMdSymbol.Text.Trim();
            if (string.IsNullOrEmpty(symbol))
            {
                quoter.SubscribeAllTickByTick( EXCHANGE_TYPE.XTP_EXCHANGE_SZ,false);
            }
            else
            {
                var exchange = symbol.StartsWith("6") ? EXCHANGE_TYPE.XTP_EXCHANGE_SH : EXCHANGE_TYPE.XTP_EXCHANGE_SZ;
                quoter.SubscribeTickByTick(new string[] { symbol }, exchange, false);
            }
        }
        #endregion

        #region MD 回调

        private void Quoter_OnUnSubMarketDataEvent(RspInfoStruct A_0, SpecificTickerStruct A_1, bool A_2)
        {
            log("OnUnSubMarketDataEvent():{0}", A_1.ticker);
        }

        private void Quoter_OnQueryAllTickersEvent(RspInfoStruct A_0, QuoteStaticInfoStruct quoteInfo, bool A_2)
        {
            SymbolMap.Add(quoteInfo.ticker, quoteInfo);
            string msg = string.Format("Quoter_OnQueryAllTickersEvent():{0}({1}),PreClose={2}", quoteInfo.ticker, quoteInfo.ticker_name.Trim('\0'), quoteInfo.pre_close_price);
            log(msg);
        }
        
        private void Quoter_OnDepthMarketDataEvent(MarketDataStruct md, long[] A_1, int A_2, int A_3, long[] A_4, int A_5, int A_6)
        {
            string msg = string.Format("Quoter_OnDepthMarketDataEvent():{0}:Last {1}/{2},Bid {3}/{4},Ask {5}/{6}",
               md.ticker, md.last_price, md.qty, md.bid[0], md.bid_qty[0], md.ask[0], md.ask_qty[0]);
            log(msg);
            double price;
            if (tickMap.TryGetValue(md.ticker, out price))
            {
                price = md.last_price;
            }
            else
            {
                price = md.last_price;
                tickMap.Add(md.ticker, price);
            }
        }

        private void Quoter_OnErrorEvent(RspInfoStruct A_0)
        {
            log("Quoter_OnErrorEvent():{0}", A_0.error_msg);
        }

        private void Quoter_OnDisconnectedEvent(int A_0)
        {
            log("Quoter_OnDisconnectedEvent():reason {0}", A_0);
        }

        private void Quoter_OnSubMarketDataEvent(RspInfoStruct rspinfo, SpecificTickerStruct tick, bool A_2)
        {
            log("Quoter_OnSubMarketDataEvent():{0} subscribe {1} {2}",
                tick.ticker, rspinfo.error_id == 0 ? "success" : "fail", rspinfo.error_msg);
        }

        private void Quoter_OnUnSubTickByTickEvent(SpecificTickerStruct ticker, RspInfoStruct error_info, bool is_last)
        {
            log("Quoter_OnUnSubTickByTickEvent():{0}", ticker.ticker);
        }

        private void Quoter_OnUnSubscribeAllTickByTickEvent(RspInfoStruct error_info)
        {
            log("Quoter_OnUnSubscribeAllTickByTickEvent():{0}", error_info.error_id == 0 ? "success" : "fail");
        }

        private void Quoter_OnUnSubscribeAllOrderBookEvent(RspInfoStruct error_info)
        {
            log("Quoter_OnUnSubscribeAllOrderBookEvent():{0}", error_info.error_id == 0 ? "success" : "fail");

        }

        private void Quoter_OnUnSubscribeAllMarketDataEvent(RspInfoStruct error_info)
        {
            log("Quoter_OnUnSubscribeAllMarketDataEvent():{0}", error_info.error_id == 0 ? "success" : "fail");
        }

        private void Quoter_OnUnSubOrderBookEvent(SpecificTickerStruct ticker, RspInfoStruct error_info, bool is_last)
        {
            log("Quoter_OnUnSubOrderBookEvent():{0}", ticker.ticker);
        }

        private void Quoter_OnTickByTickEvent(TickByTickStruct tbt_data)
        {
            log("Quoter_OnTickByTickEvent():{0} Time {1}", tbt_data.ticker, tbt_data.data_time);
        }

        private void Quoter_OnSubTickByTickEvent(SpecificTickerStruct ticker, RspInfoStruct error_info, bool is_last)
        {
            log("Quoter_OnSubTickByTickEvent():{0}", ticker.ticker);
        }

        private void Quoter_OnSubscribeAllTickByTickEvent(RspInfoStruct error_info)
        {
            log("Quoter_OnSubscribeAllTickByTickEvent():{0}", error_info.error_id == 0 ? "success" : "fail");
        }

        private void Quoter_OnSubscribeAllOrderBookEvent(RspInfoStruct error_info)
        {
            log("Quoter_OnSubscribeAllOrderBookEvent():{0}", error_info.error_id == 0 ? "success" : "fail");
        }

        private void Quoter_OnSubscribeAllMarketDataEvent(RspInfoStruct error_info)
        {
            log("Quoter_OnSubscribeAllMarketDataEvent():{0}", error_info.error_id == 0 ? "success" : "fail");
        }

        private void Quoter_OnSubOrderBookEvent(SpecificTickerStruct ticker, RspInfoStruct error_info, bool is_last)
        {
            log("Quoter_OnSubOrderBookEvent():{0}", ticker.ticker);
        }

        private void Quoter_OnQueryTickersPriceInfoEvent(TickerPriceInfo ticker_info, RspInfoStruct error_info, bool is_last)
        {
            log("Quoter_OnQueryTickersPriceInfoEvent():{0} Last {1}",
                ticker_info.ticker, ticker_info.last_price);
        }

        private void Quoter_OnOrderBookEvent(OrderBook order_book)
        {
            log("Quoter_OnOrderBookEvent():{0} Last {1}/{2}",
                order_book.ticker, order_book.last_price, order_book.trades_count);
        }

        #endregion
    }

    public class AccountConfig
    {
        public string Token { get; set; }
        public string UserID { get; set; }
        public string Password { get; set; }
        public string QuoteIP { get; set; }
        public int QuotePort { get; set; }
        public string TradeIP { get; set; }
        public int TradePort { get; set; }
    }
}
