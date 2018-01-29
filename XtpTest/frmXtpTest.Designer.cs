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
            this.tcMain = new System.Windows.Forms.TabControl();
            this.tpLog = new System.Windows.Forms.TabPage();
            this.rtxtLog = new System.Windows.Forms.RichTextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnOrder = new System.Windows.Forms.Button();
            this.tpTrade.SuspendLayout();
            this.tcMain.SuspendLayout();
            this.tpLog.SuspendLayout();
            this.SuspendLayout();
            // 
            // tpTrade
            // 
            this.tpTrade.Controls.Add(this.btnOrder);
            this.tpTrade.Controls.Add(this.btnConnect);
            this.tpTrade.Location = new System.Drawing.Point(4, 22);
            this.tpTrade.Name = "tpTrade";
            this.tpTrade.Padding = new System.Windows.Forms.Padding(3);
            this.tpTrade.Size = new System.Drawing.Size(720, 355);
            this.tpTrade.TabIndex = 0;
            this.tpTrade.Text = "Trade";
            this.tpTrade.UseVisualStyleBackColor = true;
            // 
            // tcMain
            // 
            this.tcMain.Controls.Add(this.tpTrade);
            this.tcMain.Controls.Add(this.tpLog);
            this.tcMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tcMain.Location = new System.Drawing.Point(0, 0);
            this.tcMain.Name = "tcMain";
            this.tcMain.SelectedIndex = 0;
            this.tcMain.Size = new System.Drawing.Size(728, 381);
            this.tcMain.TabIndex = 3;
            // 
            // tpLog
            // 
            this.tpLog.Controls.Add(this.rtxtLog);
            this.tpLog.Location = new System.Drawing.Point(4, 22);
            this.tpLog.Name = "tpLog";
            this.tpLog.Padding = new System.Windows.Forms.Padding(3);
            this.tpLog.Size = new System.Drawing.Size(720, 355);
            this.tpLog.TabIndex = 1;
            this.tpLog.Text = "Log";
            this.tpLog.UseVisualStyleBackColor = true;
            // 
            // rtxtLog
            // 
            this.rtxtLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtxtLog.Location = new System.Drawing.Point(3, 3);
            this.rtxtLog.Name = "rtxtLog";
            this.rtxtLog.Size = new System.Drawing.Size(714, 349);
            this.rtxtLog.TabIndex = 0;
            this.rtxtLog.Text = "";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 381);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(728, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(36, 33);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnOrder
            // 
            this.btnOrder.Location = new System.Drawing.Point(151, 32);
            this.btnOrder.Name = "btnOrder";
            this.btnOrder.Size = new System.Drawing.Size(75, 23);
            this.btnOrder.TabIndex = 1;
            this.btnOrder.Text = "Order";
            this.btnOrder.UseVisualStyleBackColor = true;
            this.btnOrder.Click += new System.EventHandler(this.btnOrder_Click);
            // 
            // frmXtpTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(728, 403);
            this.Controls.Add(this.tcMain);
            this.Controls.Add(this.statusStrip1);
            this.Name = "frmXtpTest";
            this.Text = "XTP.NET Test";
            this.Load += new System.EventHandler(this.frmXtpTest_Load);
            this.tpTrade.ResumeLayout(false);
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
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnOrder;
    }
}

