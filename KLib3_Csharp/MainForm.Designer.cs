namespace KLib3_Csharp
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.comboBoxCommandList = new System.Windows.Forms.ComboBox();
            this.buttonSend = new System.Windows.Forms.Button();
            this.textBoxPath = new System.Windows.Forms.TextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 72);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(473, 344);
            this.textBox1.TabIndex = 0;
            // 
            // comboBoxCommandList
            // 
            this.comboBoxCommandList.Enabled = false;
            this.comboBoxCommandList.FormattingEnabled = true;
            this.comboBoxCommandList.Location = new System.Drawing.Point(94, 14);
            this.comboBoxCommandList.Name = "comboBoxCommandList";
            this.comboBoxCommandList.Size = new System.Drawing.Size(310, 20);
            this.comboBoxCommandList.TabIndex = 1;
            // 
            // buttonSend
            // 
            this.buttonSend.Enabled = false;
            this.buttonSend.Location = new System.Drawing.Point(410, 12);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // textBoxPath
            // 
            this.textBoxPath.Enabled = false;
            this.textBoxPath.Location = new System.Drawing.Point(94, 40);
            this.textBoxPath.Name = "textBoxPath";
            this.textBoxPath.Size = new System.Drawing.Size(310, 21);
            this.textBoxPath.TabIndex = 3;
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(410, 38);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 2;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "Command";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(58, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "Path";
            this.label2.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(497, 428);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxPath);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.comboBoxCommandList);
            this.Controls.Add(this.textBox1);
            this.Name = "MainForm";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ComboBox comboBoxCommandList;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.TextBox textBoxPath;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}

