using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KLib3_Csharp
{
    public partial class MainForm : Form
    {
        KLib3Csharp apiClient;
        bool mRunning;
        Dictionary<string, int> mCommandWorkList;
        Dictionary<string, int> mCommandTypeList;

        public MainForm()
        {
            InitializeComponent();

            mCommandWorkList = new Dictionary<string, int>();
            mCommandTypeList = new Dictionary<string, int>();
            mRunning = false;
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (!mRunning)
            {
                apiClient = new KLib3Csharp();
                apiClient.SetInstance(apiClient);
                apiClient.AsyncReceive(new KLib3Csharp.ApiCallbackDelegate(Callback));
                apiClient.Open();
                mRunning = true;
                Thread.Sleep(1000);

                buttonConnect.Text = "Disconnect";

                buttonSend.Enabled = true;
                comboBoxCommandList.Enabled = true;
                textBoxPath.Enabled = true;
                GetCommandList();
            }
            else
            {
                mRunning = false;
                apiClient.Close();
                apiClient = null;

                buttonConnect.Text = "Connect";

                buttonSend.Enabled = false;
                comboBoxCommandList.Enabled = false;
                textBoxPath.Enabled = false;
            }
        }

        private void GetCommandList()
        {
            //지령 리스트 업데이트
            apiClient.GetCommandList(out string commandWorkList,out string commandTypeList);

            //수신 받은 지령 받기
            apiClient.GetStackReceiveCommand(out int commandType, out string commandData);

            string[] tempSplitCommandWorkList = commandWorkList.Split(',');

            //comboBoxCommandList에 지령 리스트 업데이트
            foreach(var commandWork in tempSplitCommandWorkList)
            {
                string[] command = commandWork.Split(':');
                mCommandWorkList.Add(command[0], int.Parse(command[1]));
                comboBoxCommandList.Items.Add(command[0].Replace(" ",""));
            }

            string[] tempSplitCommandTypeList = commandTypeList.Split(',');

            //지령 종류 리스트 업데이트
            foreach (var eachCommandType in tempSplitCommandTypeList)
            {
                string[] command = eachCommandType.Split(':');
                mCommandTypeList.Add(command[0].Replace(" ", ""), int.Parse(command[1]));
            }
        }

        static async void Callback(int _dataType, IntPtr _data, int _dataLength, IntPtr _ref)
        {
            KLib3Csharp myClass = (KLib3Csharp)Marshal.GetObjectForIUnknown(_ref);
            byte[] rowdata = new byte[_dataLength];
            try
            {
                Marshal.Copy(_data, rowdata, 0, _dataLength);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            if (myClass.mIndex + _dataLength >= myClass.MAX_PACKET_LENGTH)
            {
                myClass.mIndex = 0;
            }
            string result = Encoding.UTF8.GetString(rowdata);
            Array.Copy(rowdata, 0, myClass.mData, myClass.mIndex, _dataLength);
            myClass.mIndex += _dataLength;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            string command = comboBoxCommandList.Text;
            string sendData = "";

            //각 지령에 필요한 내용 기입
            if (command.IndexOf("LogStart") >= 0 || command.IndexOf("Snapshot") >= 0 || command.IndexOf("DecisionResultSave") >= 0)
            {
                sendData = @"page=1,savepath=C:\Users\Kitronyx\Desktop\,memo=Aㅁ";
            }
            else if (command.IndexOf("CalImport") >= 0)
            {
                sendData = @"deviceindex=1,sensorindex=1,path=C:\Users\Kitronyx\Documents\ForceLAB2\sample_projects\Baikal II - SFC4300CX\\cal\\20230328T135340\";
            }
            else if (command.IndexOf("CalRelease") >= 0)
            {
                sendData = @"deviceindex=1,sensorindex=1";
            }
            else if (command.IndexOf("Sensitivity") >= 0)
            {
                sendData = @"deviceindex=1,sensorindex=1,value=25";
            }
            else
            {
                sendData = "page=1";
            }

            //지령 송신
            apiClient.SendCommand(command,sendData,sendData.Count());
        }
    }
}
