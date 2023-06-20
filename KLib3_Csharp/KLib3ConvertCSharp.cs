using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Data;
using System.Collections;
using System.Drawing;

namespace KLib3_Csharp
{
    [StructLayout(LayoutKind.Sequential)]
    public class KLib3Csharp
    {
        //초기화
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateApiClient();

        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr DisposeApiClient(IntPtr _apiClientPtr);

        //양방향 통신 시작
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_Open(IntPtr _apiClientPtr);

        //양방향 통신 끝
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_Close(IntPtr _apiClientPtr);

        //지령을 Code(integer)로, 지령 내용은 문자열로 송신
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_SendCommandByCode(IntPtr _apiClientPtr, int _commandCode, IntPtr _commandData, int _commandLength);

        //지령 및 지령 내용을 문자열로 송신
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_SendCommandByStr(IntPtr _apiClientPtr, IntPtr _commandWorkCodeStr, int _commandWorkCodeStrLength, IntPtr _commandData, int _commandLength);

        //비동기 수신 이벤트
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_AsyncReceive(IntPtr _apiClientPtr, ApiCallbackDelegate _apiCallback);
        public delegate void ApiCallbackDelegate(int _commandType, IntPtr _data, int _dataLength, IntPtr _ref);

        //비동기 수신 이벤트 Instance 생성
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_SetInstance(IntPtr _apiClientPtr, IntPtr _ref);

        //수신 지령 확인 및 가져오기
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_GetReceiveStackCommand(IntPtr _apiClientPtr, ref int _commandType,ref IntPtr _commandData, ref int _commandDataLength);

        //지령 리스트 업데이트
        [DllImport("KLib3.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApiClient_GetCommandList(IntPtr _apiClientPtr, ref IntPtr _resultCommandWorkListData, ref int _resultCommandWorkDataLength, ref IntPtr _resultCommandTypeListData, ref int _resultCommandTypeDataLength);

        private IntPtr mApiClientPtr;
        public byte[] mData;
        public int mIndex;

        public readonly int MAX_PACKET_LENGTH = 100000;
        
        public KLib3Csharp(KLib3Csharp _temp)
        {
            mApiClientPtr = _temp.mApiClientPtr;
            mData = _temp.mData;
            mIndex = _temp.mIndex;
        }

        //초기화
        public KLib3Csharp()
        {
            /*
            // Allocate an unmanaged memory block to hold the ApiClient object
            mApiClientPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(ApiClient)));
            // Copy the managed ApiClient object to the unmanaged memory block
            Marshal.StructureToPtr(this, mApiClientPtr, false);*/
            if (mApiClientPtr == null || mApiClientPtr == IntPtr.Zero)
            {
                mApiClientPtr = CreateApiClient();
            }
            //ApiClient_SetInstance(mApiClientPtr, this);
            if (mData == null)
            {
                mData = new byte[MAX_PACKET_LENGTH];
                mIndex = 0;
            }
        }

        //소멸자
        ~KLib3Csharp()
        {
            // Free the unmanaged memory block when the object is finalized
            DisposeApiClient(mApiClientPtr);
        }

        //양방향 API 시작
        public void Open()
        {
            ApiClient_Open(mApiClientPtr);
        }

        //양방향 API 끝
        public void Close()
        {
            ApiClient_Close(mApiClientPtr);
        }

        //지령을 Code(integer)로, 지령 내용은 문자열로 송신
        public void SendCommand(int _commandCode, string _commandData,int _commandLength)
        {
            IntPtr commandStringDataPtr = Marshal.StringToHGlobalUni(_commandData);

            ApiClient_SendCommandByCode(mApiClientPtr,_commandCode, commandStringDataPtr, _commandLength);
        }

        //지령 및 지령 내용을 문자열로 송신
        public void SendCommand(string _commandStr, string _commandData, int _commandLength)
        {
            IntPtr CommandCodeStringPtr = Marshal.StringToHGlobalAnsi(_commandStr);
            IntPtr commandStringDataPtr = Marshal.StringToHGlobalUni(_commandData);

            ApiClient_SendCommandByStr(mApiClientPtr, CommandCodeStringPtr, _commandStr.Length, commandStringDataPtr, _commandLength*2);
        }

        //비동기 수신 이벤트 Instance 생성
        public void SetInstance(KLib3Csharp _obj)
        {
            ApiClient_SetInstance(mApiClientPtr, Marshal.GetIUnknownForObject(this));
        }

        //비동기 수신 이벤트
        public void AsyncReceive(ApiCallbackDelegate _apiCallback)
        {
            ApiClient_AsyncReceive(mApiClientPtr,_apiCallback);
        }

        //수신 지령 확인 및 가져오기
        public void GetStackReceiveCommand(out int _commandType, out string _commandData)
        {
            int mCommandType = 0;
            byte[] mCommandData;
            int mCommandDataLength = 0;
            IntPtr mCommandDataPtr = IntPtr.Zero;
            _commandData = null;

            ApiClient_GetReceiveStackCommand(mApiClientPtr, ref mCommandType, ref mCommandDataPtr, ref mCommandDataLength);
            
            if (mCommandDataLength > 0)
            {
                mCommandData = new byte[mCommandDataLength];
                Marshal.Copy(mCommandDataPtr, mCommandData, 0, mCommandDataLength);
                _commandData = Encoding.UTF8.GetString(mCommandData);
            }
            _commandType = mCommandType;
        }

        //지령 리스트 업데이트
        public void GetCommandList(out string _commandWorkList, out string _commandTypeList)
        {
            IntPtr commandWorkListPtr = IntPtr.Zero;
            int commandWorkListStrLength = 0;
            
            IntPtr commandTypeListPtr = IntPtr.Zero;
            int commandTypeListStrLength = 0;

            ApiClient_GetCommandList(mApiClientPtr, ref commandWorkListPtr,ref commandWorkListStrLength, ref commandTypeListPtr, ref commandTypeListStrLength);
            byte[] mCommandWorkListData = new byte[commandWorkListStrLength];
            Marshal.Copy(commandWorkListPtr, mCommandWorkListData, 0, commandWorkListStrLength);

            byte[] mCommandTypeListData = new byte[commandTypeListStrLength];
            Marshal.Copy(commandTypeListPtr, mCommandTypeListData, 0, commandTypeListStrLength);

            _commandWorkList = Encoding.UTF8.GetString(mCommandWorkListData);
            _commandTypeList = Encoding.UTF8.GetString(mCommandTypeListData);
        }
    }
}