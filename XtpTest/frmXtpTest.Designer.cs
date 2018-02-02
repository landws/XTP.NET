namespace XtpTest
{
    partial class frmXtpTest
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.tpTrade = new System.Windows.Forms.TabPage();
            this.scTrade = new System.Windows.Forms.SplitContainer();
            this.txtMdSymbol = new System.Windows.Forms.TextBox();
            this.btnUnsubTick = new System.Windows.Forms.Button();
            this.btnUnsubOrderBook = new System.Windows.Forms.Button();
            this.btnUnsubMD = new System.Windows.Forms.Button();
            this.btnSubTick = new System.Windows.Forms.Button();
            this.btnSubOrderBook = new System.Windows.Forms.Button();
            this.btnSubMD = new System.Windows.Forms.Button();
            this.btnQryTickers = new System.Windows.Forms.Button();
            this.btnConnectQuote = new System.Windows.Forms.Button();
            this.btnQryTrade = new System.Windows.Forms.Button();
            this.btnQryOrder = new System.Windows.Forms.Button();
            this.btnCancelOrder = new System.Windows.Forms.Button();
            this.btnConnTrader = new System.Windows.Forms.Button();
            this.btnOrder = new System.Windows.Forms.Button();
            this.txtLog = new System.Windows.Forms.RichTextBox();
            this.tcMain = new System.Windows.Forms.TabControl();
            this.tpLog = new System.Windows.Forms.TabPage();
            this.rtxtLog = new System.Windows.Forms.RichTextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tpTrade.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.scTrade)).BeginInit();
            this.scTrade.Panel1.SuspendLayout();
            this.scTrade.Panel2.SuspendLayout();
            this.scTrade.SuspendLayout();
            this.tcMain.SuspendLayout();
            this.tpLog.SuspendLayout();
            this.SuspendLayout();
            // 
            // tpTrade
            // 
            this.tpTrade.Controls.Add(this.scTrade);
            this.tpTrade.Controls.Add(this.txtLog);
            this.tpTrade.Location = new System.Drawing.Point(4, 22);
            this.tpTrade.Name = "tpTrade";
            this.tpTrade.Padding = new System.Windows.Forms.Padding(3);
            this.tpTrade.Size = new System.Drawing.Size(930, 355);
            this.tpTrade.TabIndex = 0;
            this.tpTrade.Text = "Trade";
            this.tpTrade.UseVisualStyleBackColor = true;
            // 
            // scTrade
            // 
            this.scTrade.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scTrade.Location = new System.Drawing.Point(3, 3);
            this.scTrade.Name = "scTrade";
            // 
            // scTrade.Panel1
            // 
            this.scTrade.Panel1.Controls.Add(this.txtMdSymbol);
            this.scTrade.Panel1.Controls.Add(this.btnUnsubTick);
            this.scTrade.Panel1.Controls.Add(this.btnUnsubOrderBook);
            this.scTrade.Panel1.Controls.Add(this.btnUnsubMD);
            this.scTrade.Panel1.Controls.Add(this.btnSubTick);
            this.scTrade.Panel1.Controls.Add(this.btnSubOrderBook);
            this.scTrade.Panel1.Controls.Add(this.btnSubMD);
            this.scTrade.Panel1.Controls.Add(this.btnQryTickers);
            this.scTrade.Panel1.Controls.Add(this.btnConnectQuote);
            // 
            // scTrade.Panel2
            // 
            this.scTrade.Panel2.Controls.Add(this.btnQryTrade);
            this.scTrade.Panel2.Controls.Add(this.btnQryOrder);
            this.scTrade.Panel2.Controls.Add(this.btnCancelOrder);
            this.scTrade.Panel2.Controls.Add(this.btnConnTrader);
            this.scTrade.Panel2.Controls.Add(this.btnOrder);
            this.scTrade.Size = new System.Drawing.Size(924, 253);
            this.scTrade.SplitterDistance = 308;
            this.scTrade.TabIndex = 3;
            // 
            // txtMdSymbol
            // 
            this.txtMdSymbol.Location = new System.Drawing.Point(154, 18);
            this.txtMdSymbol.Name = "txtMdSymbol";
            this.txtMdSymbol.Size = new System.Drawing.Size(100, 21);
            this.txtMdSymbol.TabIndex = 11;
            this.txtMdSymbol.Text = "要订阅的股票";
            // 
            // btnUnsubTick
            // 
            this.btnUnsubTick.Location = new System.Drawing.Point(154, 199);
            this.btnUnsubTick.Name = "btnUnsubTick";
            this.btnUnsubTick.Size = new System.Drawing.Size(88, 23);
            this.btnUnsubTick.TabIndex = 10;
            this.btnUnsubTick.Text = "Unsub Tick";
            this.btnUnsubTick.UseVisualStyleBackColor = true;
            this.btnUnsubTick.Click += new System.EventHandler(this.btnUnsubTick_Click);
            // 
            // btnUnsubOrderBook
            // 
            this.btnUnsubOrderBook.Location = new System.Drawing.Point(154, 151);
            this.btnUnsubOrderBook.Name = "btnUnsubOrderBook";
            this.btnUnsubOrderBook.Size = new System.Drawing.Size(88, 23);
            this.btnUnsubOrderBook.TabIndex = 9;
            this.btnUnsubOrderBook.Text = "UnsubOrderBook";
            this.btnUnsubOrderBook.UseVisualStyleBackColor = true;
            this.btnUnsubOrderBook.Click += new System.EventHandler(this.btnUnsubOrderBook_Click);
            // 
            // btnUnsubMD
            // 
            this.btnUnsubMD.Location = new System.Drawing.Point(154, 107);
            this.btnUnsubMD.Name = "btnUnsubMD";
            this.btnUnsubMD.Size = new System.Drawing.Size(88, 23);
            this.btnUnsubMD.TabIndex = 8;
            this.btnUnsubMD.Text = "Unsub MD";
            this.btnUnsubMD.UseVisualStyleBackColor = true;
            this.btnUnsubMD.Click += new System.EventHandler(this.btnUnsubMD_Click);
            // 
            // btnSubTick
            // 
            this.btnSubTick.Location = new System.Drawing.Point(22, 199);
            this.btnSubTick.Name = "btnSubTick";
            this.btnSubTick.Size = new System.Drawing.Size(88, 23);
            this.btnSubTick.TabIndex = 5;
            this.btnSubTick.Text = "Sub Tick";
            this.btnSubTick.UseVisualStyleBackColor = true;
            this.btnSubTick.Click += new System.EventHandler(this.btnSubTick_Click);
            // 
            // btnSubOrderBook
            // 
            this.btnSubOrderBook.Location = new System.Drawing.Point(22, 151);
            this.btnSubOrderBook.Name = "btnSubOrderBook";
            this.btnSubOrderBook.Size = new System.Drawing.Size(88, 23);
            this.btnSubOrderBook.TabIndex = 4;
            this.btnSubOrderBook.Text = "SubOrderBook";
            this.btnSubOrderBook.UseVisualStyleBackColor = true;
            this.btnSubOrderBook.Click += new System.EventHandler(this.btnSubOrderBook_Click);
            // 
            // btnSubMD
            // 
            this.btnSubMD.Location = new System.Drawing.Point(22, 107);
            this.btnSubMD.Name = "btnSubMD";
            this.btnSubMD.Size = new System.Drawing.Size(88, 23);
            this.btnSubMD.TabIndex = 3;
            this.btnSubMD.Text = "Sub MD";
            this.btnSubMD.UseVisualStyleBackColor = true;
            this.btnSubMD.Click += new System.EventHandler(this.btnSubMD_Click);
            // 
            // btnQryTickers
            // 
            this.btnQryTickers.Location = new System.Drawing.Point(22, 62);
            this.btnQryTickers.Name = "btnQryTickers";
            this.btnQryTickers.Size = new System.Drawing.Size(88, 23);
            this.btnQryTickers.TabIndex = 2;
            this.btnQryTickers.Text = "QueryTicker";
            this.btnQryTickers.UseVisualStyleBackColor = true;
            this.btnQryTickers.Click += new System.EventHandler(this.btnQryTickers_Click);
            // 
            // btnConnectQuote
            // 
            this.btnConnectQuote.Location = new System.Drawing.Point(22, 18);
            this.btnConnectQuote.Name = "btnConnectQuote";
            this.btnConnectQuote.Size = new System.Drawing.Size(88, 23);
            this.btnConnectQuote.TabIndex = 0;
            this.btnConnectQuote.Text = "MD Connect";
            this.btnConnectQuote.UseVisualStyleBackColor = true;
            this.btnConnectQuote.Click += new System.EventHandler(this.btnConnectQuote_Click);
            // 
            // btnQryTrade
            // 
            this.btnQryTrade.Location = new System.Drawing.Point(22, 199);
            this.btnQryTrade.Name = "btnQryTrade";
            this.btnQryTrade.Size = new System.Drawing.Size(88, 23);
            this.btnQryTrade.TabIndex = 5;
            this.btnQryTrade.Text = "QryTrade";
            this.btnQryTrade.UseVisualStyleBackColor = true;
            this.btnQryTrade.Click += new System.EventHandler(this.btnQryTrade_Click);
            // 
            // btnQryOrder
            // 
            this.btnQryOrder.Location = new System.Drawing.Point(22, 151);
            this.btnQryOrder.Name = "btnQryOrder";
            this.btnQryOrder.Size = new System.Drawing.Size(88, 23);
            this.btnQryOrder.TabIndex = 4;
            this.btnQryOrder.Text = "QryOrder";
            this.btnQryOrder.UseVisualStyleBackColor = true;
            this.btnQryOrder.Click += new System.EventHandler(this.btnQryOrder_Click);
            // 
            // btnCancelOrder
            // 
            this.btnCancelOrder.Location = new System.Drawing.Point(22, 107);
            this.btnCancelOrder.Name = "btnCancelOrder";
            this.btnCancelOrder.Size = new System.Drawing.Size(88, 23);
            this.btnCancelOrder.TabIndex = 3;
            this.btnCancelOrder.Text = "CancelOrder";
            this.btnCancelOrder.UseVisualStyleBackColor = true;
            this.btnCancelOrder.Click += new System.EventHandler(this.btnCancelOrder_Click);
            // 
            // btnConnTrader
            // 
            this.btnConnTrader.Location = new System.Drawing.Point(22, 18);
            this.btnConnTrader.Name = "btnConnTrader";
            this.btnConnTrader.Size = new System.Drawing.Size(88, 23);
            this.btnConnTrader.TabIndex = 2;
            this.btnConnTrader.Text = "TD Connect";
            this.btnConnTrader.UseVisualStyleBackColor = true;
            this.btnConnTrader.Click += new System.EventHandler(this.btnConnTrader_Click);
            // 
            // btnOrder
            // 
            this.btnOrder.Location = new System.Drawing.Point(22, 62);
            this.btnOrder.Name = "btnOrder";
            this.btnOrder.Size = new System.Drawing.Size(88, 23);
            this.btnOrder.TabIndex = 1;
            this.btnOrder.Text = "Order";
            this.btnOrder.UseVisualStyleBackColor = true;
            this.btnOrder.Click += new System.EventHandler(this.btnOrder_Click);
            // 
            // txtLog
            // 
            this.txtLog.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.txtLog.Location = new System.Drawing.Point(3, 256);
            this.txtLog.Name = "txtLog";
            this.txtLog.Size = new System.Drawing.Size(924, 96);
            this.txtLog.TabIndex = 2;
            this.txtLog.Text = "";
            // 
            // tcMain
            // 
            this.tcMain.Controls.Add(this.tpTrade);
            this.tcMain.Controls.Add(this.tpLog);
            this.tcMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tcMain.Location = new System.Drawing.Point(0, 0);
            this.tcMain.Name = "tcMain";
            this.tcMain.SelectedIndex = 0;
            this.tcMain.Size = new System.Drawing.Size(938, 381);
            this.tcMain.TabIndex = 3;
            // 
            // tpLog
            // 
            this.tpLog.Controls.Add(this.rtxtLog);
            this.tpLog.Location = new System.Drawing.Point(4, 22);
            this.tpLog.Name = "tpLog";
            this.tpLog.Padding = new System.Windows.Forms.Padding(3);
            this.tpLog.Size = new System.Drawing.Size(930, 355);
            this.tpLog.TabIndex = 1;
            this.tpLog.Text = "Log";
            this.tpLog.UseVisualStyleBackColor = true;
            // 
            // rtxtLog
            // 
            this.rtxtLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtxtLog.Location = new System.Drawing.Point(3, 3);
            this.rtxtLog.Name = "rtxtLog";
            this.rtxtLog.Size = new System.Drawing.Size(924, 349);
            this.rtxtLog.TabIndex = 0;
            this.rtxtLog.Text = "";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 381);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(938, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // frmXtpTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(938, 403);
            this.Controls.Add(this.tcMain);
            this.Controls.Add(this.statusStrip1);
            this.Name = "frmXtpTest";
            this.Text = "XTP.NET Test";
            this.Load += new System.EventHandler(this.frmXtpTest_Load);
            this.tpTrade.ResumeLayout(false);
            this.scTrade.Panel1.ResumeLayout(false);
            this.scTrade.Panel1.PerformLayout();
            this.scTrade.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.scTrade)).EndInit();
            this.scTrade.ResumeLayout(false);
            this.tcMain.ResumeLayout(false);
            this.tpLog.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabPage tpTrade;
        private System.Windows.Forms.TabControl tcMain;
        private System.Windows.Forms.TabPage tpLog;
        private System.Windows.Forms.RichTextBox rtxtLog;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.Button btnConnectQuote;
        private System.Windows.Forms.Button btnOrder;
        private System.Windows.Forms.RichTextBox txtLog;
        private System.Windows.Forms.SplitContainer scTrade;
        private System.Windows.Forms.Button btnQryTickers;
        private System.Windows.Forms.Button btnSubTick;
        private System.Windows.Forms.Button btnSubOrderBook;
        private System.Windows.Forms.Button btnSubMD;
        private System.Windows.Forms.Button btnUnsubTick;
        private System.Windows.Forms.Button btnUnsubOrderBook;
        private System.Windows.Forms.Button btnUnsubMD;
        private System.Windows.Forms.Button btnConnTrader;
        private System.Windows.Forms.Button btnCancelOrder;
        private System.Windows.Forms.Button btnQryOrder;
        private System.Windows.Forms.Button btnQryTrade;
        private System.Windows.Forms.TextBox txtMdSymbol;
    }
}

