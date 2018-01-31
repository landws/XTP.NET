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
        public frmXtpTest()
        {
            InitializeComponent();
        }
        XtpTrader trader;
        XtpQuote quoter;
        int client_id = 1;
        string log_path = "xtp_log";
        string key;
        string userid;
        string pwd;
        string quote_ip;
        int quote_port;
        int trade_port;
        string trade_ip;
        ulong tradeSessionID = 0;
        Dictionary<string, double> tickMap = new Dictionary<string, double>();

        private void init()
        {
            quoter = new XtpQuote(client_id, log_path);
            quoter.OnSubMarketDataEvent += Quoter_OnSubMarketDataEvent;
            quoter.OnDisconnectedEvent += Quoter_OnDisconnectedEvent;
            quoter.OnErrorEvent += Quoter_OnErrorEvent;
            quoter.OnDepthMarketDataEvent += Quoter_OnDepthMarketDataEvent;
            quoter.OnQueryAllTickersEvent += Quoter_OnQueryAllTickersEvent;
            quoter.OnUnSubMarketDataEvent += Quoter_OnUnSubMarketDataEvent;
            quoter.Login(quote_ip, quote_port, userid, pwd, PROTOCOL_TYPE.XTP_PROTOCOL_TCP);
            List<string> insts = new List<string>() { "000001", "300500" };
            if (quoter.IsLogin)
            {
                log("MD {0} login success", userid);

                quoter.SubscribeMarketData(insts.ToArray(), EXCHANGE_TYPE.XTP_EXCHANGE_SZ, true);

                quoter.QueryAllTickers(EXCHANGE_TYPE.XTP_EXCHANGE_SH);
            }
            else
            {
                var rsp = quoter.GetApiLastError();
                log("MD {0} login fail:{1}/{2}", userid, rsp.error_id, rsp.error_msg);
            }
            trader = new XtpTrader(client_id, key, log_path, TE_RESUME_TYPE.XTP_TERT_QUICK);
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
        private void Trader_OnTradeEvent(TradeReportStruct trade)
        {
            log("Trader_OnTradeEvent():{0} {1} {2}@{3}", trade.ticker, trade.side.ToString(), trade.quantity, trade.price);
        }

        private void Trader_OnQueryTradeEvent(RspInfoStruct A_0, TradeReportStruct A_1, int A_2, bool A_3)
        {
            log("Trader_OnQueryTradeEvent");
        }

        private void Trader_OnQueryPositionEvent(RspInfoStruct A_0, QueryStkPositionStruct A_1, int A_2, bool A_3)
        {
            log("Trader_OnQueryPositionEvent");
        }

        private void Trader_OnQueryOrderEvent(RspInfoStruct A_0, OrderInfoStruct A_1, int A_2, bool A_3)
        {
            log("Trader_OnQueryOrderEvent");
        }

        private void Trader_OnQueryAssetEvent(RspInfoStruct A_0, QueryAssetRspStruct A_1, int A_2, bool A_3)
        {
            log("Trader_OnQueryAssetEvent");
        }

        private void Trader_OnOrderEvent(RspInfoStruct rsp, OrderInfoStruct orderInfo)
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

        private void Trader_OnOrderCancelEvent(RspInfoStruct A_0, OrderCancelInfoStruct A_1)
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

        private void Quoter_OnUnSubMarketDataEvent(RspInfoStruct A_0, SpecificTickerStruct A_1, bool A_2)
        {
            log("OnUnSubMarketDataEvent():{0}", A_1.ticker);
        }

        private void Quoter_OnQueryAllTickersEvent(RspInfoStruct A_0, QuoteStaticInfoStruct A_1, bool A_2)
        {
            string msg = string.Format("Quoter_OnQueryAllTickersEvent():{0}({1}),PreClose={2}", A_1.ticker, A_1.ticker_name.Trim('\0'),A_1.pre_close_price);
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
            log("Quoter_OnDisconnectedEvent()");
        }

        private void Quoter_OnSubMarketDataEvent(RspInfoStruct rspinfo, SpecificTickerStruct tick, bool A_2)
        {
            log("Quoter_OnSubMarketDataEvent():{0} subscribe {1} {2}",
                tick.ticker, rspinfo.error_id == 0 ? "success" : "fail", rspinfo.error_msg);
        }


        private void log(string fmt, params object[] args)
        {
            BeginInvoke((Action)delegate {
                string msg = string.Format("{0}:  {1}", DateTime.Now.ToString("yyyyMMdd HH:mm:ss"),
                    string.Format(fmt, args));
                rtxtLog.AppendText(string.Format("{0}\r\n", msg));
            });
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            init();
        }

        private void btnOrder_Click(object sender, EventArgs e)
        {
            if (trader == null || trader.IsLogin == false)
            {
                return;
            }
            foreach (var item in tickMap)
            {
                var order = new XTPOrderInsert()
                {
                    market = MARKET_TYPE.XTP_MKT_SZ_A,
                    order_client_id = (uint)client_id,
                    quantity = 200,
                    //order_xtp_id = 0,
                    price = item.Value,
                    price_type = PRICE_TYPE.XTP_PRICE_LIMIT,
                    side = SIDE_TYPE.XTP_SIDE_BUY,
                    ticker = item.Key
                };
                trader.InsertOrder(order, tradeSessionID);
            }
        }

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
