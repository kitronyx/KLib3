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
            this.textBoxSavePath = new System.Windows.Forms.TextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonSavePathSelect = new System.Windows.Forms.Button();
            this.buttonCalPathSelect = new System.Windows.Forms.Button();
            this.textBoxCalPath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxPage = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxDeviceIndex = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBoxSensorIndex = new System.Windows.Forms.TextBox();
            this.textBoxMemo = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxSensitivityValue = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.buttonLogClear = new System.Windows.Forms.Button();
            this.buttonGetStackData = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(94, 380);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(391, 179);
            this.textBox1.TabIndex = 0;
            // 
            // comboBoxCommandList
            // 
            this.comboBoxCommandList.Enabled = false;
            this.comboBoxCommandList.FormattingEnabled = true;
            this.comboBoxCommandList.Location = new System.Drawing.Point(94, 38);
            this.comboBoxCommandList.Name = "comboBoxCommandList";
            this.comboBoxCommandList.Size = new System.Drawing.Size(310, 20);
            this.comboBoxCommandList.TabIndex = 1;
            // 
            // buttonSend
            // 
            this.buttonSend.Enabled = false;
            this.buttonSend.Location = new System.Drawing.Point(410, 35);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // textBoxSavePath
            // 
            this.textBoxSavePath.Enabled = false;
            this.textBoxSavePath.Location = new System.Drawing.Point(94, 64);
            this.textBoxSavePath.Name = "textBoxSavePath";
            this.textBoxSavePath.Size = new System.Drawing.Size(310, 21);
            this.textBoxSavePath.TabIndex = 3;
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(410, 7);
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
            this.label1.Location = new System.Drawing.Point(24, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "Command";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(62, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "Save Path";
            this.label2.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // buttonSavePathSelect
            // 
            this.buttonSavePathSelect.Location = new System.Drawing.Point(410, 63);
            this.buttonSavePathSelect.Name = "buttonSavePathSelect";
            this.buttonSavePathSelect.Size = new System.Drawing.Size(75, 23);
            this.buttonSavePathSelect.TabIndex = 5;
            this.buttonSavePathSelect.Text = "Select";
            this.buttonSavePathSelect.UseVisualStyleBackColor = true;
            this.buttonSavePathSelect.Click += new System.EventHandler(this.buttonSavePathSelect_Click);
            // 
            // buttonCalPathSelect
            // 
            this.buttonCalPathSelect.Location = new System.Drawing.Point(410, 91);
            this.buttonCalPathSelect.Name = "buttonCalPathSelect";
            this.buttonCalPathSelect.Size = new System.Drawing.Size(75, 23);
            this.buttonCalPathSelect.TabIndex = 6;
            this.buttonCalPathSelect.Text = "Select";
            this.buttonCalPathSelect.UseVisualStyleBackColor = true;
            this.buttonCalPathSelect.Click += new System.EventHandler(this.buttonCalPathSelect_Click);
            // 
            // textBoxCalPath
            // 
            this.textBoxCalPath.Enabled = false;
            this.textBoxCalPath.Location = new System.Drawing.Point(94, 91);
            this.textBoxCalPath.Name = "textBoxCalPath";
            this.textBoxCalPath.Size = new System.Drawing.Size(310, 21);
            this.textBoxCalPath.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(35, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "Cal Path";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // textBoxPage
            // 
            this.textBoxPage.Location = new System.Drawing.Point(94, 118);
            this.textBoxPage.Name = "textBoxPage";
            this.textBoxPage.Size = new System.Drawing.Size(46, 21);
            this.textBoxPage.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(54, 121);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "Page";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(146, 121);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(74, 12);
            this.label5.TabIndex = 4;
            this.label5.Text = "DeviceIndex";
            this.label5.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // textBoxDeviceIndex
            // 
            this.textBoxDeviceIndex.Location = new System.Drawing.Point(226, 118);
            this.textBoxDeviceIndex.Name = "textBoxDeviceIndex";
            this.textBoxDeviceIndex.Size = new System.Drawing.Size(44, 21);
            this.textBoxDeviceIndex.TabIndex = 7;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(278, 121);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(76, 12);
            this.label6.TabIndex = 4;
            this.label6.Text = "SensorIndex";
            this.label6.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // textBoxSensorIndex
            // 
            this.textBoxSensorIndex.Location = new System.Drawing.Point(360, 118);
            this.textBoxSensorIndex.Name = "textBoxSensorIndex";
            this.textBoxSensorIndex.Size = new System.Drawing.Size(44, 21);
            this.textBoxSensorIndex.TabIndex = 7;
            // 
            // textBoxMemo
            // 
            this.textBoxMemo.Location = new System.Drawing.Point(94, 172);
            this.textBoxMemo.Multiline = true;
            this.textBoxMemo.Name = "textBoxMemo";
            this.textBoxMemo.Size = new System.Drawing.Size(392, 202);
            this.textBoxMemo.TabIndex = 0;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(47, 172);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(41, 12);
            this.label7.TabIndex = 4;
            this.label7.Text = "Memo";
            this.label7.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(26, 148);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(62, 12);
            this.label8.TabIndex = 4;
            this.label8.Text = "Sensitivity";
            this.label8.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // textBoxSensitivityValue
            // 
            this.textBoxSensitivityValue.Location = new System.Drawing.Point(94, 145);
            this.textBoxSensitivityValue.Name = "textBoxSensitivityValue";
            this.textBoxSensitivityValue.Size = new System.Drawing.Size(46, 21);
            this.textBoxSensitivityValue.TabIndex = 7;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(47, 380);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(26, 12);
            this.label9.TabIndex = 4;
            this.label9.Text = "Log";
            this.label9.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // buttonLogClear
            // 
            this.buttonLogClear.Location = new System.Drawing.Point(410, 565);
            this.buttonLogClear.Name = "buttonLogClear";
            this.buttonLogClear.Size = new System.Drawing.Size(75, 23);
            this.buttonLogClear.TabIndex = 6;
            this.buttonLogClear.Text = "Clear";
            this.buttonLogClear.UseVisualStyleBackColor = true;
            this.buttonLogClear.Click += new System.EventHandler(this.buttonLogClear_Click);
            // 
            // buttonGetStackData
            // 
            this.buttonGetStackData.Location = new System.Drawing.Point(329, 565);
            this.buttonGetStackData.Name = "buttonGetStackData";
            this.buttonGetStackData.Size = new System.Drawing.Size(75, 23);
            this.buttonGetStackData.TabIndex = 6;
            this.buttonGetStackData.Text = "Receive";
            this.buttonGetStackData.UseVisualStyleBackColor = true;
            this.buttonGetStackData.Click += new System.EventHandler(this.buttonGetStackData_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(505, 593);
            this.Controls.Add(this.textBoxSensorIndex);
            this.Controls.Add(this.textBoxDeviceIndex);
            this.Controls.Add(this.textBoxSensitivityValue);
            this.Controls.Add(this.textBoxPage);
            this.Controls.Add(this.buttonGetStackData);
            this.Controls.Add(this.buttonLogClear);
            this.Controls.Add(this.buttonCalPathSelect);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.buttonSavePathSelect);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxCalPath);
            this.Controls.Add(this.textBoxSavePath);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.comboBoxCommandList);
            this.Controls.Add(this.textBoxMemo);
            this.Controls.Add(this.textBox1);
            this.Name = "MainForm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ComboBox comboBoxCommandList;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.TextBox textBoxSavePath;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonSavePathSelect;
        private System.Windows.Forms.Button buttonCalPathSelect;
        private System.Windows.Forms.TextBox textBoxCalPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxPage;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxDeviceIndex;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxSensorIndex;
        private System.Windows.Forms.TextBox textBoxMemo;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxSensitivityValue;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button buttonLogClear;
        private System.Windows.Forms.Button buttonGetStackData;
    }
}

