using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace KLib3_Csharp
{
    public enum SendDataEnum { Page,SavePath,DeviceIndex,SensorIndex,Sensitivity,CalPath,Memo }

    public struct BidirectionalApiCommand
    {
        public int mCommandCode;
        public List<SendDataEnum> mSendDatas;
        public bool mShowData;

        public BidirectionalApiCommand(int _commandCode, List<SendDataEnum> _sendDatas, bool _showData)
        {
            mCommandCode = _commandCode;
            mSendDatas = _sendDatas;
            mShowData = _showData;
        }
    }


    public partial class MainForm : Form
    {
        KLib3Csharp apiClient;
        bool mRunning;
        bool mShowData;

        Dictionary<string, Tuple<string, BidirectionalApiCommand>> mDicCommandList;
        Dictionary<string, int> mCommandWorkList;
        Dictionary<string, int> mCommandTypeList;

        public MainForm()
        {
            InitializeComponent();
            mDicCommandList = new Dictionary<string, Tuple<string, BidirectionalApiCommand>>();
            mCommandWorkList = new Dictionary<string, int>();
            mCommandTypeList = new Dictionary<string, int>();
            mRunning = false;
            mShowData = false;
            SetDicCommandList();
        }

        private void SetDicCommandList()
        {
            mDicCommandList.Add("캘리브레이션 불러오기", new Tuple<string, BidirectionalApiCommand>("CalImport", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page, SendDataEnum.DeviceIndex, SendDataEnum.SensorIndex, SendDataEnum.CalPath },false)));
            mDicCommandList.Add("캘리브레이션 제거하기", new Tuple<string, BidirectionalApiCommand>("CalRelease", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page, SendDataEnum.DeviceIndex, SendDataEnum.SensorIndex }, false)));
            mDicCommandList.Add("디바이스 센서 정보", new Tuple<string, BidirectionalApiCommand>("DeviceSensorInfo", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page },true)));
            mDicCommandList.Add("디바이스 연결 정보", new Tuple<string, BidirectionalApiCommand>("ConnectionInfo", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page }, true)));
            mDicCommandList.Add("ForceLAB2 버전", new Tuple<string, BidirectionalApiCommand>("Version", new BidirectionalApiCommand(0, new List<SendDataEnum> { }, true)));
            mDicCommandList.Add("지령 업데이트",  new Tuple<string, BidirectionalApiCommand>("Update", new BidirectionalApiCommand(0, new List<SendDataEnum> { }, false)));
            mDicCommandList.Add("긴급 정지",  new Tuple<string, BidirectionalApiCommand>("EmergencyStop", new BidirectionalApiCommand(0, new List<SendDataEnum> {  }, false)));
            mDicCommandList.Add("일시 정지",  new Tuple<string, BidirectionalApiCommand>("Pause", new BidirectionalApiCommand(0, new List<SendDataEnum> {  }, false)));
            mDicCommandList.Add("감도 조절",  new Tuple<string, BidirectionalApiCommand>("Sensitivity", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.DeviceIndex, SendDataEnum.SensorIndex, SendDataEnum.Sensitivity }, false)));
            mDicCommandList.Add("RTA 실행",  new Tuple<string, BidirectionalApiCommand>("RtaStart", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page },false)));
            mDicCommandList.Add("RTA 종료",  new Tuple<string, BidirectionalApiCommand>("RtaEnd", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page },false)));
            mDicCommandList.Add("로그 시작하기",  new Tuple<string, BidirectionalApiCommand>("LogStart", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page, SendDataEnum.SavePath, SendDataEnum.Memo }, false)));
            mDicCommandList.Add("로그 끝내기",  new Tuple<string, BidirectionalApiCommand>("LogEnd", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page },false)));
            mDicCommandList.Add("스냅샷",  new Tuple<string, BidirectionalApiCommand>("Snapshot", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page, SendDataEnum.SavePath, SendDataEnum.Memo }, false)));
            mDicCommandList.Add("양불 판정",  new Tuple<string, BidirectionalApiCommand>("Decision", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page }, true)));
            mDicCommandList.Add("양불 판정 저장",  new Tuple<string, BidirectionalApiCommand>("DecisionResultSave", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page, SendDataEnum.SavePath, SendDataEnum.Memo }, true)));
            mDicCommandList.Add("실시간 데이터 전송 켜기",  new Tuple<string, BidirectionalApiCommand>("RealtimeDataOn", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page },true)));
            mDicCommandList.Add("실시간 데이터 전송 끄기",  new Tuple<string, BidirectionalApiCommand>("RealtimeDataOff", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page }, false)));
            mDicCommandList.Add("1개 프레임 데이터 받기",  new Tuple<string, BidirectionalApiCommand>("MatrixData", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page }, true)));
            mDicCommandList.Add("누적모드 ON",  new Tuple<string, BidirectionalApiCommand>("AccumulOn", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page }, false)));
            mDicCommandList.Add("누적모드 OFF",  new Tuple<string, BidirectionalApiCommand>("AccumulOff", new BidirectionalApiCommand(0, new List<SendDataEnum> { SendDataEnum.Page },false)));

            /*
22.page 변경
23.저장 경로 설정
24.device 번호 설정
25.sensor 번호 설정
26.cal 파일 경로 설정
27.감도 값 설정
28.메모 설정

0.종료
            */

        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (!mRunning)
            {
                apiClient = new KLib3Csharp();
                apiClient.SetInstance(apiClient);
                apiClient.AsyncReceive(new KLib3Csharp.ApiCallbackDelegate(Callback));
                
                int connectTimeOut = 0;
                int sendTimeOut = 0;

                //apiClient.SetTimeOut(1000, 1000);
                apiClient.GetTimeOut(ref connectTimeOut, ref sendTimeOut);

                Console.WriteLine("ConnectTimeOut: " + connectTimeOut.ToString() + ", SendTimeOut: " + sendTimeOut.ToString());

                if(!apiClient.Open())
                {
                    Console.WriteLine("CSharp Tcp/Ip Connect Fail");
                    return;
                }
                mRunning = true;
                Thread.Sleep(1000);

                buttonConnect.Text = "Disconnect";

                buttonSend.Enabled = true;
                comboBoxCommandList.Enabled = true;
                textBoxSavePath.Enabled = true;
                GetCommandList();
            }
            else
            {
                mRunning = false;
                apiClient.Close();
                apiClient = null;

                mCommandWorkList.Clear();
                mCommandTypeList.Clear();

                buttonConnect.Text = "Connect";

                buttonSend.Enabled = false;
                comboBoxCommandList.Enabled = false;
                textBoxSavePath.Enabled = false;
            }
        }

        private void GetCommandList()
        {
            //지령 리스트 업데이트
            apiClient.GetCommandList(out string commandWorkList,out string commandTypeList);

            //수신 받은 지령 받기
            apiClient.GetStackReceiveCommand(out int commandType, out string commandData);

            if(commandWorkList=="")
            {
                return;
            }

            string[] tempSplitCommandWorkList = commandWorkList.Split(',');

            //comboBoxCommandList에 지령 리스트 업데이트
            foreach(var commandWork in tempSplitCommandWorkList)
            {
                string[] command = commandWork.Split(':');
                mCommandWorkList.Add(command[0].Replace(" ", ""), int.Parse(command[1]));
                //comboBoxCommandList.Items.Add(command[0].Replace(" ",""));
            }

            Dictionary<string, Tuple<string, BidirectionalApiCommand>> temp = new Dictionary<string, Tuple<string, BidirectionalApiCommand>>();

            foreach (var commandStr in mDicCommandList)
            {
                temp.Add(commandStr.Key, new Tuple<string, BidirectionalApiCommand>(commandStr.Value.Item1, new BidirectionalApiCommand(mCommandWorkList[commandStr.Value.Item1], commandStr.Value.Item2.mSendDatas, commandStr.Value.Item2.mShowData)));
                comboBoxCommandList.Items.Add(commandStr.Key);
            }

            mDicCommandList = temp;

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
            string command = mDicCommandList[comboBoxCommandList.Text].Item1;
            string sendData = "";

            /*
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
            }*/

            sendData = GetSendData(mDicCommandList[comboBoxCommandList.Text].Item2.mSendDatas);

            //지령 송신
            apiClient.SendCommand(command,sendData,sendData.Count());


            if(mDicCommandList[comboBoxCommandList.Text].Item2.mShowData)
            {
                Thread.Sleep(500);
                apiClient.GetSearchStackReceiveCommand(command, out string receiveData);
                
                textBox1.Text += receiveData + "\r\n";
            }
            //apiClient.SendCommand("얗호", sendData, sendData.Count());
        }

        private string GetSendData(List<SendDataEnum> _sendDatas)
        {
            string result = @"";

            foreach(var sendData in _sendDatas)
            {
                if(result != "")
                {
                    result += ",";
                }
                switch( sendData )
                {
                    case SendDataEnum.Page:
                        result += "page=" + textBoxPage.Text;
                        break;
                    case SendDataEnum.SavePath:
                        result += "savepath=" + textBoxSavePath.Text;
                        break;
                    case SendDataEnum.DeviceIndex:
                        result += "deviceindex=" + textBoxDeviceIndex.Text;
                        break;
                    case SendDataEnum.SensorIndex:
                        result += "sensorindex=" + textBoxSensorIndex.Text;
                        break;
                    case SendDataEnum.CalPath:
                        result += "path=" + textBoxCalPath.Text;
                        break;
                    case SendDataEnum.Sensitivity:
                        result += "value=" + textBoxSensitivityValue.Text;
                        break;
                    case SendDataEnum.Memo:
                        result += "memo=" + textBoxMemo.Text;
                        break;
                    default:
                        break;
                }
            }

            return result;
        }

        private void buttonSavePathSelect_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Title = "폴더 선택",
                CheckFileExists = false,
                FileName = "폴더 선택",
                Filter = "폴더|noexist.nof",
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string selectedFolder = System.IO.Path.GetDirectoryName(openFileDialog.FileName);
                textBoxSavePath.Text = selectedFolder;
                Console.WriteLine("선택한 폴더: " + selectedFolder);
            }
            else
            {
                Console.WriteLine("폴더 선택이 취소되었습니다.");
            }
        }

        private void buttonCalPathSelect_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Title = "폴더 선택",
                CheckFileExists = false,
                FileName = "폴더 선택",
                Filter = "폴더|noexist.nof",
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string selectedFolder = System.IO.Path.GetDirectoryName(openFileDialog.FileName);
                textBoxCalPath.Text = selectedFolder;
                Console.WriteLine("선택한 폴더: " + selectedFolder);
            }
            else
            {
                Console.WriteLine("폴더 선택이 취소되었습니다.");
            }
        }

        private void buttonLogClear_Click(object sender, EventArgs e)
        {
            textBox1.Text = string.Empty;
        }

        private void buttonGetStackData_Click(object sender, EventArgs e)
        {
            apiClient.GetStackReceiveCommand(out int commandCode, out string message);
            
                textBox1.Text += "CommandCode: " + commandCode.ToString() + ", Receive Data: " + message + "\r\n";
        }
    }
}
