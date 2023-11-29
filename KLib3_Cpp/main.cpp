#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <locale>
#include <codecvt>
#include <ShlObj.h>

using namespace std;

typedef void* (*CreateApiClient)(); //생성자
typedef void (*DisposeApiClient)(void*); //소멸자
typedef bool (*ApiClient_Open)(void*); //양방향 API 시작
typedef bool (*ApiClient_Close)(void*); //양방향 API 끝
typedef void (*ApiClient_GetReceiveStackCommand)(void*, int&, char*&, int&); //수신 지령 받기
typedef void (*ApiClient_GetSearchReceiveStackCommand)(void*, int, char*&, int&);
typedef void (*ApiClient_GetSearchReceiveStackCommandStr)(void*,const char*, int, char*&, int&);
typedef void (*ApiClient_SendCommandByCode)(void*, int, const char*, int); //지령을 Code(Integer)로, 지령 내용을 문자열로 보내기
typedef void (*ApiClient_SendCommandByStr)(void*, const char*, int, const char*, int); //지령 및 지령 내용을 문자열로 보내기
typedef void (*ApiClient_GetCommandList)(void*, char*&, int&, char*&, int&); //지령 리스트 얻기
typedef void (*ApiClient_GetTimeOut)(void*, int&, int&);
typedef void (*ApiClient_SetTimeOut)(void*, int, int);

class MyDLLWrapper {
private:
    HINSTANCE dllHandle;

    void* mApiClient;

    CreateApiClient FuncCreateApiClent; //생성지
    DisposeApiClient FuncDisposeApiClient; //소멸자 
    ApiClient_Open FuncApiClient_Open; //양방향 API 통신 시작
    ApiClient_Close FuncApiClient_Close; // 양방향 API 통신 끝
    ApiClient_GetReceiveStackCommand FuncApiClient_GetReceiveStackCommand; // 받은 Command 확인
    ApiClient_GetSearchReceiveStackCommand FuncApiClient_GetSearchReceiveStackCommand;
    ApiClient_GetSearchReceiveStackCommandStr FuncApiClient_GetSearchReceiveStackCommandStr;
    ApiClient_SendCommandByCode FuncApiClient_SendCommandByCode; // 지령을 Code(integer)로, 지령 내용은 문자열로 보내기
    ApiClient_SendCommandByStr FuncApiClient_SendCommandByStr; // 지령 및 지령 내용을 문자열로 보내기
    ApiClient_GetCommandList FuncApiClient_GetCommandList; // 지령 리스트
    ApiClient_GetTimeOut FuncApiClient_GetTimeOut;
    ApiClient_SetTimeOut FuncApiClient_SetTimeOut;

public:
    int mCommandWorkListLength;//행동 지령 문자열 길이
    int mCommandTypeListLength;//지령 종류 문자열 길이
    char* mCommandWorkList;//행동 지령 문자열 - (지령이름):(지령코드)
    char* mCommandTypeList;//지령 종류 문자열 - (지령타입이름):(지령타입코드)

    MyDLLWrapper() : dllHandle(nullptr) {}

    ~MyDLLWrapper() {
        if (dllHandle != nullptr) {
            FreeLibrary(dllHandle);
        }
    }

    //DLL 불러오기
    bool LoadDLL(const string dllPath) {

        dllHandle = LoadLibraryA(dllPath.c_str());

        if (dllHandle == nullptr) {
            std::cout << "DLL 로드 실패" << std::endl;
            return false;
        }

        FuncCreateApiClent = (CreateApiClient)GetProcAddress(dllHandle, "CreateApiClient");
        if (FuncCreateApiClent == nullptr) {
            std::cout << "DLL 함수 포인터 mDisposeApiClient 가져오기 실패" << std::endl;
            return false;
        }

        FuncDisposeApiClient = (DisposeApiClient)GetProcAddress(dllHandle, "DisposeApiClient");
        if (FuncDisposeApiClient == nullptr) {
            std::cout << "DLL 함수 포인터 DisposeApiClient 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_Open = (ApiClient_Open)GetProcAddress(dllHandle, "ApiClient_Open");
        if (FuncApiClient_Open == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_Open 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_Close = (ApiClient_Close)GetProcAddress(dllHandle, "ApiClient_Close");
        if (FuncApiClient_Close == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_Close 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_GetReceiveStackCommand = (ApiClient_GetReceiveStackCommand)GetProcAddress(dllHandle, "ApiClient_GetReceiveStackCommand");
        if (FuncApiClient_GetReceiveStackCommand == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_GetReceiveStackCommand 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_GetSearchReceiveStackCommand = (ApiClient_GetSearchReceiveStackCommand)GetProcAddress(dllHandle, "ApiClient_GetSearchReceiveStackCommand");
        if (FuncApiClient_GetSearchReceiveStackCommand == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_GetReceiveStackCommand 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_GetSearchReceiveStackCommandStr = (ApiClient_GetSearchReceiveStackCommandStr)GetProcAddress(dllHandle, "ApiClient_GetSearchReceiveStackCommandStr");
        if (FuncApiClient_GetSearchReceiveStackCommandStr == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_GetReceiveStackCommand 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_SendCommandByCode = (ApiClient_SendCommandByCode)GetProcAddress(dllHandle, "ApiClient_SendCommandByCode");
        if (FuncApiClient_SendCommandByCode == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_SendCommandByCode 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_SendCommandByStr = (ApiClient_SendCommandByStr)GetProcAddress(dllHandle, "ApiClient_SendCommandByStr");
        if (FuncApiClient_SendCommandByStr == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_SendCommandByStr 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_GetCommandList = (ApiClient_GetCommandList)GetProcAddress(dllHandle, "ApiClient_GetCommandList");
        if (FuncApiClient_GetCommandList == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_GetCommandList 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_GetTimeOut = (ApiClient_GetTimeOut)GetProcAddress(dllHandle, "ApiClient_GetTimeOut");
        if (FuncApiClient_GetCommandList == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_GetTimeOut 가져오기 실패" << std::endl;
            return false;
        }

        FuncApiClient_SetTimeOut = (ApiClient_SetTimeOut)GetProcAddress(dllHandle, "ApiClient_SetTimeOut");
        if (FuncApiClient_GetCommandList == nullptr) {
            std::cout << "DLL 함수 포인터 ApiClient_SetTimeOut 가져오기 실패" << std::endl;
            return false;
        }

        return true;
    }

    //양방향 통신 시작
    bool Open()
     {
         //초기화
         mApiClient = FuncCreateApiClent();

         //양방향 통신 시작
         return FuncApiClient_Open(mApiClient);
     }

     //지령 리스트 업데이트
     void GetCommandList()
     {
         FuncApiClient_GetCommandList(mApiClient, mCommandWorkList,mCommandWorkListLength, mCommandTypeList, mCommandTypeListLength);
     }

     //지령 문자열로 송신
     void SendCommand(const char* _commandStr, char16_t* _commandData)
     {
         //FuncApiClient_SendCommandByStr(mApiClient,_commandStr,_commandStrLenght,ConvertUnicodeToCharPointer(ConvertToUnicode(_commandData)),_commandDataLength);
         int commandDataLength = 0;
         char* commandDataChar = ConvertChar16ToByte(_commandData, commandDataLength);

         FuncApiClient_SendCommandByStr(mApiClient, _commandStr, strlen(_commandStr), commandDataChar, commandDataLength);
     }

     //지령 Code로 송신
     void SendCommand(int _commandCode, char16_t* _commandData)
     {
         int commandDataLength = 0;
         char* commandDataChar = ConvertChar16ToByte(_commandData, commandDataLength);

         FuncApiClient_SendCommandByCode(mApiClient, _commandCode, commandDataChar, commandDataLength);
     }

     //수신받은 지령 얻기
     std::string GetReceiveStackCommand()
     {
         int commandCode;
         char* receiveData;
         int receiveDataLength;

         std::string result = "";

         FuncApiClient_GetReceiveStackCommand(mApiClient, commandCode, receiveData, receiveDataLength);

         if ((commandCode & 0xF0FFFF) == 9 || (commandCode & 0xF0FFFF) == 1048576)
         {
             for (int i = 0; i < receiveDataLength; ++i)
             {
                 if (result != "")
                     result += ", ";
                 result += to_string(static_cast<int>(static_cast<unsigned char>(receiveData[i])));
             }
         }
         else
         {
             if (commandCode == 0)
             {
                 result = "No receive data";
                 return result;
             }
             result = "수신 지령 코드: "+ to_string(commandCode) + ", 수신 데이터 : " + receiveData;
         }

         return result;
     }

     std::string GetSearchReceiveStackCommand(int _getCommandCode)
     {
         char* receiveData;
         int receiveDataLength;

         std::string result = "";
         FuncApiClient_GetSearchReceiveStackCommand(mApiClient, _getCommandCode, receiveData, receiveDataLength);

         if ((_getCommandCode & 0xF0FFFF) == 9 || (_getCommandCode & 0xF0FFFF) == 1048576)
         {
             for (int i = 0; i < receiveDataLength; ++i)
             {
                 if (result != "")
                     result += ", ";
                 result += to_string(static_cast<int>(static_cast<unsigned char>(receiveData[i])));
             }
         }
         else
         {
             result = receiveData;
         }

         return result;
     }

     std::string GetSearchReceiveStackCommandStr(const std::string _getCommandCodeStr)
     {
         char* receiveData;
         int receiveDataLength;
         
         std::string result = "";

         FuncApiClient_GetSearchReceiveStackCommandStr(mApiClient, _getCommandCodeStr.c_str(), size(_getCommandCodeStr), receiveData, receiveDataLength);

         if (_getCommandCodeStr == "MatrixData" || _getCommandCodeStr == "RealtimeDataOn")
         {
             for (int i = 0; i < receiveDataLength; ++i)
             {
                 if (result != "")
                     result += ", ";
                 result += to_string(static_cast<int>(static_cast<unsigned char>(receiveData[i])));
             }
         }
         else
         {
             result = receiveData;
         }

         return result;
     }

     //양방향 API 종료
     void Close()
     {
         FuncApiClient_Close(mApiClient);
     }

     void GetTimeOut(int& _connectTimeOut, int& _sendTimeOut)
     {
         FuncApiClient_GetTimeOut(mApiClient, _connectTimeOut, _sendTimeOut);
     }

     void SetTimeOut(int _connectTimeOut, int _sendTimeOut)
     {
         FuncApiClient_SetTimeOut(mApiClient, _connectTimeOut, _sendTimeOut);
     }

     //문자열에서 Unicode wchar_t*로 변환
     wchar_t* ConvertToUnicode(const char* input)
     {
         // 입력된 char* 문자열의 길이 계산
         int length = strlen(input);

         // 필요한 버퍼 크기 계산 (문자열 길이 + NULL 문자)
         int bufferSize = MultiByteToWideChar(CP_UTF8, 0, input, length, nullptr, 0);

         // 버퍼 크기만큼의 wchar_t 배열 생성
         wchar_t* buffer = new wchar_t[bufferSize];

         // char* 문자열을 wchar_t*로 변환
         MultiByteToWideChar(CP_UTF8, 0, input, length, buffer, bufferSize);

         return buffer;
     }

     //Unicode wchar_t*에서 문자열로 변환
     char* ConvertUnicodeToCharPointer(const wchar_t* _input)
     {
         // wchar_t* 문자열의 길이 계산 (NULL 문자 포함)
         int length = wcslen(_input) + 1;

         // 필요한 버퍼 크기 계산 (wchar_t 길이 * sizeof(wchar_t))
         int bufferSize = length * sizeof(wchar_t);

         // 버퍼 할당
         char* buffer = new char[bufferSize];

         // wchar_t* 문자열을 byte*로 변환
         memcpy(buffer, reinterpret_cast<const char*>(_input), bufferSize);

         return buffer;
     }

     //wchar_t* 길이 계산
     size_t GetChar16Length(const char16_t* u16String) {
         const char16_t* p = u16String;
         while (*p)
             ++p;
         return p - u16String;
     }

     //16비트 char에서 8비트 char로 변환
     char* ConvertChar16ToByte(char16_t* u16String, int& _length) {
         // 필요한 버퍼 크기 계산
         _length = sizeof(char16_t) * GetChar16Length(u16String);

         // BYTE* 버퍼 할당
         char* byteArray = new char[_length];

         // char16_t*을 BYTE*로 변환
         memcpy(byteArray, reinterpret_cast<BYTE*>(u16String), _length);

         return byteArray;
     }
};



MyDLLWrapper dllWrapper;

void CommandTest1()
{
    std::string input;

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    char16_t dataCommand1[] = u"page=1";

    //RTA 실행
    dllWrapper.SendCommand("RtaStart", dataCommand1);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    char16_t dataCommand2[] = u"page=1,savepath=C:\\Users\\Kitronyx\\Desktop\\,memo=Aㅁ";

    //RTA내에 스냅샷 실행
    dllWrapper.SendCommand("Snapshot", dataCommand2);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    cout << dllWrapper.mCommandWorkList << endl;
    cout << dllWrapper.mCommandTypeList << endl;

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    //실시간 RTA 프레임 데이터 송신 ON
    dllWrapper.SendCommand("RealtimeDataOn", dataCommand1);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    int commandCode=0;
    string commandData = "";
    int commandDataLength=0;

    while (true)
    {
        //수신받은 지령 받음
        dllWrapper.GetReceiveStackCommand();

        cout << "commandCode: " << commandCode;

        //수신받은 지령 출력
        if (commandDataLength > 0)
        {
            cout << ", commandData:" << commandData << ", commandDataLength:" << commandDataLength;
        }

        cout << endl;

        std::cout << "Press enter for continue...(If exit press \"A\")";
        std::getline(std::cin, input);

        if (input == "a" || input == "A")
            break;
    }
    //실시간 RTA 프레임 데이터 송신 OFF
    dllWrapper.SendCommand("RealtimeDataOff", dataCommand1);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);
}

int* sendCommandDataLength;
char16_t* sendCommandData;

u16string pageSendData = u"page=1";
u16string savepathSendData = u"savepath=";
u16string deviceIndexSendData = u"deviceIndex=";
u16string sensorIndexSendData = u"sensorIndex=";
u16string calPathSendData = u"path=";
u16string sensitivityValueSendData = u"value=";
u16string memoSendData;

string pageStr = "page=1";
string savepathStr = "savepath=";
string deviceIndexStr = "deviceIndex=";
string sensorIndexStr = "sensorIndex=";
string calPathStr = "path=";
string sensitivityValueStr = "value=";
string memoStr = "memo=";

void GetArgStr(char _option, char16_t*& _outputStr, int*& _length)
{
    u16string result = u"";
    // 1000 0000
    if ((_option & 0x80) > 0)
    {
        result += pageSendData;
    }

    // 0100 0000
    if ((_option & 0x40) > 0)
    {
        if (result != u"")
            result += u",";
        result += savepathSendData;
    }

    // 0010 0000
    if ((_option & 0x20) > 0)
    {
        if (result != u"")
            result += u",";
        result += deviceIndexSendData;
    }

    // 0001 0000
    if ((_option & 0x10) > 0)
    {
        if (result != u"")
            result += u",";
        result += sensorIndexSendData;
    }

    // 0000 1000
    if ((_option & 0x08) > 0)
    {
        if (result != u"")
            result += u",";
        result += calPathSendData;
    }

    // 0000 0100
    if ((_option & 0x04) > 0)
    {
        if (result != u"")
            result += u",";
        result += sensitivityValueSendData;
    }

    // 0000 0010
    if ((_option & 0x02) > 0)
    {
        if (result != u"")
            result += u",";
        result += memoSendData;
    }

    // 0000 0001
    if ((_option & 0x01) > 0)
    {

    }

    *_length = result.size() + 1;
    _outputStr = new char16_t[*_length];
    
    memcpy(_outputStr, result.c_str(), *_length*2);
    _outputStr[*_length - 1] = u'\0';
    //wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    //string utf8String = converter.to_bytes(result);
    //return utf8String;
}

string GetCommandStr(int _input, char16_t*& _commandSendData, int*& _length, bool*& _getReceive)
{
    string result = "";
    *_getReceive = false;

    switch (_input)
    {
    case 0:
        result = "exit";
        break;
    case 1:
        result = "CalImport";
        GetArgStr(0xB8, _commandSendData, _length);
        break;
    case 2:
        result = "CalRelease";
        GetArgStr(0xB0, _commandSendData, _length);
        break;
    case 3:
        result = "DeviceSensorInfo";
        GetArgStr(0x80, _commandSendData, _length);
        *_getReceive = true;
        break;
    case 4:
        result = "ConnectionInfo";
        GetArgStr(0x80, _commandSendData, _length);
        *_getReceive = true;
        break;
    case 5:
        result = "Version";
        //*_getReceive = true;
        break;
    case 6:
        result = "Update";
        break;
    case 7:
        result = "EmergencyStop";
        break;
    case 8:
        result = "Pause";
        break;
    case 9:
        result = "Sensitivity";
        GetArgStr(0x34, _commandSendData, _length);
        break;
    case 10:
        result = "RtaStart";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 11:
        result = "RtaEnd";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 12:
        result = "LogStart";
        GetArgStr(0xC2, _commandSendData, _length);
        break;
    case 13:
        result = "LogEnd";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 14:
        result = "Snapshot";
        GetArgStr(0xC2, _commandSendData, _length);
        break;
    case 15:
        result = "Decision";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 16:
        result = "DecisionResultSave";
        GetArgStr(0xC2, _commandSendData, _length);
        break;
    case 17:
        result = "RealtimeDataOn";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 18:
        result = "RealtimeDataOff";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 19:
        result = "MatrixData";
        GetArgStr(0x80, _commandSendData, _length);
        *_getReceive = true;
        break;
    case 20:
        result = "AccumulOn";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    case 21:
        result = "AccumulOff";
        GetArgStr(0x80, _commandSendData, _length);
        break;
    default:
        break;
    }
    return result;
}

std::u16string convertToUTF16(const std::string& utf8String) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    return converter.from_bytes(utf8String);
}

std::u16string convertToUTF16(const std::wstring& utf8String) {
    std::u16string u16str;
    for (wchar_t wch : utf8String) {
        if (wch <= 0xFFFF) {
            u16str.push_back(static_cast<char16_t>(wch));
        }
    }
    return u16str;
}

std::u16string utf8ToUtf16(const std::string& utf8String) {
    std::u16string utf16String;
    utf16String.reserve(utf8String.size());

    for(int i = 0; i < utf8String.size()/2;++i)
        utf16String.push_back(static_cast<char16_t>((utf8String[i*2]&0xFF) + ((utf8String[i*2+1] << 8)&0xFF00)));

    return utf16String;
}

string StringToUtf16(const string& _str)
{
    std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> convert;
    std::u16string u16 = convert.from_bytes(_str);
    return convert.to_bytes(u16);
}

std::string u16string_to_string(const std::u16string& u16Str) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    return converter.to_bytes(u16Str);
}

std::string utf16_to_utf8(std::u16string const& s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
        std::codecvt_mode::little_endian>, char16_t> cnv;
    std::string utf8 = cnv.to_bytes(s);
    if (cnv.converted() < s.size())
        throw std::runtime_error("incomplete conversion");
    return utf8;
}

std::string multibyte_to_utf8(const std::string& str) {
    int nLen = str.size();
    wchar_t warr[256];
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, warr, 256);
    char carr[256];
    memset(carr, '\0', sizeof(carr));
    WideCharToMultiByte(CP_UTF8, 0, warr, -1, carr, 256, NULL, NULL);
    return carr;
}

std::u16string utf8_to_utf16(std::string const& utf8)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
        std::codecvt_mode::little_endian>, char16_t> cnv;
    std::u16string s = cnv.from_bytes(utf8);
    if (cnv.converted() < utf8.size())
        throw std::runtime_error("incomplete conversion");
    return s;
}

std::string u16stringToUtf8(const std::u16string& utf16String) {
    //std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    string utf8String;
    utf8String.reserve(utf16String.size());

    for (int i = 0; i < utf16String.size(); ++i)
    {
        utf8String.push_back(static_cast<char>(utf16String[i] & 0xFF));
        if(utf16String[i] >> 8 != 0)
            utf8String.push_back(static_cast<char>(utf16String[i]>>8));
    }

    return utf8String;
}

void QATest()
{
    std::string inputCommand;
    std::string inputValue;
    wstring u16InputValue;

    PWSTR desktopPath;
    setlocale(LC_ALL, ".UTF-16LE");
    const char* selectScript = R"(
1. 캘리브레이션 불러오기 (Page, 디바이스 번호, 센서 번호, Cal 경로)
2. 캘리브레이션 제거하기 (캘리브레이션이 설정되어 있을 경우) (Page, 디바이스 번호, 센서 번호)
3. 디바이스 센서 정보 (Page)
4. 연결 정보 (Page)
5. 버전
6. 지령 업데이트
7. 긴급 정지 (정지 상태가 아닐 경우)
8. 일시 정지 (정지 상태가 아닐 경우)
9. 감도조절 (디바이스 번호, 센서 번호, 감도 값)
10. RTA 실행 (RTA가 실행되지 않고 있을 경우) (Page)
11. RTA 종료 (RTA가 실행되고 있을 경우) (Page)

(RTA 동작 중에만)
12. 로그 시작하기(로그 저장 하지 않을 경우) (Page, 저장 경로, 메모)
13. 로그 끝내기(로그 저장 중일 경우) (Page)
14. 스냅샷 (Page, 저장 경로, 메모)
15. 양불 판정 (양불판정 설정이 사전에 정의되어 있을 경우) (Page)
16. 양불 판정 저장 (양불판정 설정이 사전에 정의되어 있을 경우) (Page, 저장 경로, 메모)
17. 실시간 데이터 전송 켜기 (실시간 데이터 전송이 꺼져 있을 경우) (Page)
18. 실시간 데이터 전송 끄기 (실시간 데이터 전송이 켜져 있을 경우) (Page)
19. 1개 프레임 받기 (Page)
20. 누적모드 ON (누적모드가 OFF일 경우) (Page)
21. 누적모드 OFF (누적모드가 ON일 경우) (Page)

22. page 변경
23. 저장 경로 설정
24. device 번호 설정
25. sensor 번호 설정
26. cal 파일 경로 설정
27. 감도 값 설정
28. 메모 설정

29. 최상위 수신 지령 및 데이터 출력
30. 지령 코드 입력으로 수신 데이터 출력
31. 지령 숫자 입력으로 수신 데이터 출력

0. 종료
)";

    //memo = new char16_t[6];
    char16_t test[] = u"야호";
    u16string st = u"야호";
    //string str = multibyte_to_utf8("야호");
    //u16string str2 = convertToUTF16(str);
    
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath))) {
        std::wstring widePath(desktopPath);
        CoTaskMemFree(desktopPath);
        std::string desktopPathString(widePath.begin(), widePath.end());
        savepathSendData += convertToUTF16(desktopPathString);
    }

    string errorStr = "";
    wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    sendCommandDataLength = new int;
    bool *isGetReceiveData = new bool;

    int receiveCommandCode;
    char* receiveCommandData;
    int receiveCommandDataLength;
    string sendCommand;
    string resultData;
    int commandCode;
    *isGetReceiveData = false;
    while (true)
    {
        //수신받은 지령 받음
        /*dllWrapper.GetReceiveStackCommand(commandCode, commandData, commandDataLength);

        cout << "commandCode: " << commandCode;

        //수신받은 지령 출력
        if (commandDataLength > 0)
        {
            cout << ", commandData:" << commandData << ", commandDataLength:" << commandDataLength;
        }

        cout << endl;
        */
        //system("cls");
        
        /*
        if (errorStr != "")
        {
            cout << errorStr << endl;
            errorStr = "";
        }*/
        
        cout << "현재 선택된 페이지: " << pageStr << endl;
        cout << "지정된 저장 경로: " << savepathStr << endl;
        cout << "지정된 디바이스 번호: " << deviceIndexStr << endl;
        cout << "지정된 센서 번호: " << sensorIndexStr << endl;
        cout << "지정된 Cal 파일 경로: " << calPathStr << endl;
        cout << "지정된 감도 값: " << sensitivityValueStr << endl;
        cout << "메모 : "<< memoStr << endl;
       // wstring wString(memo.begin(), memo.end());

        // wstring 출력
        //std::wcout << wString << std::endl;
        cout << endl;
        
        //dllWrapper.GetReceiveStackCommand(receiveCommandCode, receiveCommandData, receiveCommandDataLength);
        /*
        if (sendCommand != "")
        {
            dllWrapper.GetSearchReceiveStackCommandStr(sendCommand.c_str(), size(sendCommand), receiveCommandData, receiveCommandDataLength);
            
            cout << "commandCode: " << sendCommand;
            //수신받은 지령 출력
            if (receiveCommandDataLength > 0)
            {
                cout << ", commandData:" << receiveCommandData << ", commandDataLength:" << receiveCommandDataLength;
            }
            cout << endl;
        }*/

        cout << selectScript << endl;
        cout << "마지막 커맨드: " << inputCommand << endl;
        cout << endl;

        cout << "수행할 커맨드 입력(Enter): ";

        std::getline(std::cin, inputCommand);
        int command = -1;
        try {
            command = stoi(inputCommand);
        }
        catch (exception ex)
        {
            errorStr += ex.what();
            continue;
        }

        try {
            switch (command)
            {
            case 22:
                cout << "변경할 Page 입력: ";
                std::getline(std::cin, inputValue);
                pageStr = "page=" + inputValue;
                pageSendData = u"page=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 23:
                cout << "저장 경로 입력: ";
                std::getline(std::cin, inputValue);
                savepathStr = "savepath=" + inputValue;
                savepathSendData = u"savepath=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 24:
                cout << "변경할 디바이스 번호 입력: ";
                std::getline(std::cin, inputValue);
                deviceIndexStr = "deviceindex=" + inputValue;
                deviceIndexSendData = u"deviceindex=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 25:
                cout << "변경할 센서 번호 입력: ";
                std::getline(std::cin, inputValue);
                sensorIndexStr = "sensorindex=" + inputValue;
                sensorIndexSendData = u"sensorindex=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 26:
                cout << "Cal 경로 입력: ";
                std::getline(std::cin, inputValue);
                calPathStr = "path=" + inputValue;
                calPathSendData = u"path=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 27:
                cout << "변경할 감도 값 입력: ";
                std::getline(std::cin, inputValue);
                sensitivityValueStr = "value=" + inputValue;
                sensitivityValueSendData = u"value=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 28:
                cout << "메모 입력: ";
                std::getline(std::cin, inputValue);
                memoSendData = u"memo=" + convertToUTF16(multibyte_to_utf8(inputValue));
                memoStr = "memo=" + inputValue;
                continue;
            case 29:
                resultData = dllWrapper.GetReceiveStackCommand();
                cout << endl;
                cout << "수신 데이터: " << resultData << endl;
                cout << endl;
                continue;
            case 30:
                commandCode = 0;
                while (true)
                {
                    cout << "수신 데이터 내 검색 할 지령 코드(숫자) 입력: ";
                    std::getline(std::cin, inputValue);
                    //string commandStr = GetCommandStr(stoi(inputValue), sendCommandData, sendCommandDataLength, isGetReceiveData);
                    
                    try {
                        commandCode = stoi(inputValue);
                    }
                    catch (exception ex)
                    {
                        cout << ex.what() << endl;
                        continue;
                    }
                    break;
                }
                resultData = dllWrapper.GetSearchReceiveStackCommand(commandCode);
                cout << endl;
                cout << "수신 데이터: " << resultData << endl;
                cout << endl;
                continue;
            case 31:
                cout << "수신 데이터 내 검색 할 지령 번호 입력: ";
                std::getline(std::cin, inputValue);
                sendCommand = GetCommandStr(stoi(inputValue), sendCommandData, sendCommandDataLength, isGetReceiveData);
                resultData = dllWrapper.GetSearchReceiveStackCommandStr(sendCommand);
                cout << endl;
                cout << "수신 데이터: " << resultData << endl;
                cout << endl;
                continue;

            default:
                if (command < 22)
                {
                    break;
                }
                else
                {
                    sendCommand = "";
                    continue;
                }
            }
        }
        catch (exception ex)
        {
            errorStr += ex.what();
            continue;
        }

        sendCommand = GetCommandStr(command, sendCommandData,sendCommandDataLength,isGetReceiveData);

        dllWrapper.SendCommand(sendCommand.c_str(), sendCommandData);
        //dllWrapper.SendCommand("야호", sendCommandData);

        if (sendCommand == "exit")
            break;
    }
    delete(sendCommandDataLength);
    delete[](sendCommandData);

    delete(isGetReceiveData);
}

int main() {
    while (true)
    {
        if (!dllWrapper.LoadDLL("KLib3.dll")) {
            cout << "Fail load DLL file." << endl;
        }

        Sleep(1000);
        dllWrapper.Open(); // 양방향 통신 시작
        Sleep(1000);
        dllWrapper.GetCommandList(); // 지령 리스트 업데이트

        //CommandTest1();
        QATest();
    }
    return 0;
}