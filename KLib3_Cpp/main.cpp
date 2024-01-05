#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <locale>
#include <codecvt>
#include <ShlObj.h>

using namespace std;

typedef void* (*CreateApiClient)(); //������
typedef void (*DisposeApiClient)(void*); //�Ҹ���
typedef bool (*ApiClient_Open)(void*); //����� API ����
typedef bool (*ApiClient_Close)(void*); //����� API ��
typedef void (*ApiClient_GetReceiveStackCommand)(void*, int&, char*&, int&); //���� ���� �ޱ�
typedef void (*ApiClient_GetSearchReceiveStackCommand)(void*, int, char*&, int&);
typedef void (*ApiClient_GetSearchReceiveStackCommandStr)(void*,const char*, int, char*&, int&);
typedef void (*ApiClient_SendCommandByCode)(void*, int, const char*, int); //������ Code(Integer)��, ���� ������ ���ڿ��� ������
typedef void (*ApiClient_SendCommandByStr)(void*, const char*, int, const char*, int); //���� �� ���� ������ ���ڿ��� ������
typedef void (*ApiClient_GetCommandList)(void*, char*&, int&, char*&, int&); //���� ����Ʈ ���
typedef void (*ApiClient_GetTimeOut)(void*, int&, int&);
typedef void (*ApiClient_SetTimeOut)(void*, int, int);

class MyDLLWrapper {
private:
    HINSTANCE dllHandle;

    void* mApiClient;

    CreateApiClient FuncCreateApiClent; //������
    DisposeApiClient FuncDisposeApiClient; //�Ҹ��� 
    ApiClient_Open FuncApiClient_Open; //����� API ��� ����
    ApiClient_Close FuncApiClient_Close; // ����� API ��� ��
    ApiClient_GetReceiveStackCommand FuncApiClient_GetReceiveStackCommand; // ���� Command Ȯ��
    ApiClient_GetSearchReceiveStackCommand FuncApiClient_GetSearchReceiveStackCommand;
    ApiClient_GetSearchReceiveStackCommandStr FuncApiClient_GetSearchReceiveStackCommandStr;
    ApiClient_SendCommandByCode FuncApiClient_SendCommandByCode; // ������ Code(integer)��, ���� ������ ���ڿ��� ������
    ApiClient_SendCommandByStr FuncApiClient_SendCommandByStr; // ���� �� ���� ������ ���ڿ��� ������
    ApiClient_GetCommandList FuncApiClient_GetCommandList; // ���� ����Ʈ
    ApiClient_GetTimeOut FuncApiClient_GetTimeOut;
    ApiClient_SetTimeOut FuncApiClient_SetTimeOut;

public:
    int mCommandWorkListLength;//�ൿ ���� ���ڿ� ����
    int mCommandTypeListLength;//���� ���� ���ڿ� ����
    char* mCommandWorkList;//�ൿ ���� ���ڿ� - (�����̸�):(�����ڵ�)
    char* mCommandTypeList;//���� ���� ���ڿ� - (����Ÿ���̸�):(����Ÿ���ڵ�)

    MyDLLWrapper() : dllHandle(nullptr) {}

    ~MyDLLWrapper() {
        if (dllHandle != nullptr) {
            FreeLibrary(dllHandle);
        }
    }

    //DLL �ҷ�����
    bool LoadDLL(const string dllPath) {

        dllHandle = LoadLibraryA(dllPath.c_str());

        if (dllHandle == nullptr) {
            std::cout << "DLL �ε� ����" << std::endl;
            return false;
        }

        FuncCreateApiClent = (CreateApiClient)GetProcAddress(dllHandle, "CreateApiClient");
        if (FuncCreateApiClent == nullptr) {
            std::cout << "DLL �Լ� ������ mDisposeApiClient �������� ����" << std::endl;
            return false;
        }

        FuncDisposeApiClient = (DisposeApiClient)GetProcAddress(dllHandle, "DisposeApiClient");
        if (FuncDisposeApiClient == nullptr) {
            std::cout << "DLL �Լ� ������ DisposeApiClient �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_Open = (ApiClient_Open)GetProcAddress(dllHandle, "ApiClient_Open");
        if (FuncApiClient_Open == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_Open �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_Close = (ApiClient_Close)GetProcAddress(dllHandle, "ApiClient_Close");
        if (FuncApiClient_Close == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_Close �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_GetReceiveStackCommand = (ApiClient_GetReceiveStackCommand)GetProcAddress(dllHandle, "ApiClient_GetReceiveStackCommand");
        if (FuncApiClient_GetReceiveStackCommand == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_GetReceiveStackCommand �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_GetSearchReceiveStackCommand = (ApiClient_GetSearchReceiveStackCommand)GetProcAddress(dllHandle, "ApiClient_GetSearchReceiveStackCommand");
        if (FuncApiClient_GetSearchReceiveStackCommand == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_GetReceiveStackCommand �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_GetSearchReceiveStackCommandStr = (ApiClient_GetSearchReceiveStackCommandStr)GetProcAddress(dllHandle, "ApiClient_GetSearchReceiveStackCommandStr");
        if (FuncApiClient_GetSearchReceiveStackCommandStr == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_GetReceiveStackCommand �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_SendCommandByCode = (ApiClient_SendCommandByCode)GetProcAddress(dllHandle, "ApiClient_SendCommandByCode");
        if (FuncApiClient_SendCommandByCode == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_SendCommandByCode �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_SendCommandByStr = (ApiClient_SendCommandByStr)GetProcAddress(dllHandle, "ApiClient_SendCommandByStr");
        if (FuncApiClient_SendCommandByStr == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_SendCommandByStr �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_GetCommandList = (ApiClient_GetCommandList)GetProcAddress(dllHandle, "ApiClient_GetCommandList");
        if (FuncApiClient_GetCommandList == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_GetCommandList �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_GetTimeOut = (ApiClient_GetTimeOut)GetProcAddress(dllHandle, "ApiClient_GetTimeOut");
        if (FuncApiClient_GetCommandList == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_GetTimeOut �������� ����" << std::endl;
            return false;
        }

        FuncApiClient_SetTimeOut = (ApiClient_SetTimeOut)GetProcAddress(dllHandle, "ApiClient_SetTimeOut");
        if (FuncApiClient_GetCommandList == nullptr) {
            std::cout << "DLL �Լ� ������ ApiClient_SetTimeOut �������� ����" << std::endl;
            return false;
        }

        return true;
    }

    //����� ��� ����
    bool Open()
     {
         //�ʱ�ȭ
         mApiClient = FuncCreateApiClent();

         //����� ��� ����
         return FuncApiClient_Open(mApiClient);
     }

     //���� ����Ʈ ������Ʈ
     void GetCommandList()
     {
         FuncApiClient_GetCommandList(mApiClient, mCommandWorkList,mCommandWorkListLength, mCommandTypeList, mCommandTypeListLength);
     }

     //���� ���ڿ��� �۽�
     void SendCommand(const char* _commandStr, char16_t* _commandData)
     {
         //FuncApiClient_SendCommandByStr(mApiClient,_commandStr,_commandStrLenght,ConvertUnicodeToCharPointer(ConvertToUnicode(_commandData)),_commandDataLength);
         int commandDataLength = 0;
         char* commandDataChar = ConvertChar16ToByte(_commandData, commandDataLength);

         FuncApiClient_SendCommandByStr(mApiClient, _commandStr, strlen(_commandStr), commandDataChar, commandDataLength);
     }

     //���� Code�� �۽�
     void SendCommand(int _commandCode, char16_t* _commandData)
     {
         int commandDataLength = 0;
         char* commandDataChar = ConvertChar16ToByte(_commandData, commandDataLength);

         FuncApiClient_SendCommandByCode(mApiClient, _commandCode, commandDataChar, commandDataLength);
     }

     //���Ź��� ���� ���
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
             result = "���� ���� �ڵ�: "+ to_string(commandCode) + ", ���� ������ : " + receiveData;
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

     //����� API ����
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

     //���ڿ����� Unicode wchar_t*�� ��ȯ
     wchar_t* ConvertToUnicode(const char* input)
     {
         // �Էµ� char* ���ڿ��� ���� ���
         int length = strlen(input);

         // �ʿ��� ���� ũ�� ��� (���ڿ� ���� + NULL ����)
         int bufferSize = MultiByteToWideChar(CP_UTF8, 0, input, length, nullptr, 0);

         // ���� ũ�⸸ŭ�� wchar_t �迭 ����
         wchar_t* buffer = new wchar_t[bufferSize];

         // char* ���ڿ��� wchar_t*�� ��ȯ
         MultiByteToWideChar(CP_UTF8, 0, input, length, buffer, bufferSize);

         return buffer;
     }

     //Unicode wchar_t*���� ���ڿ��� ��ȯ
     char* ConvertUnicodeToCharPointer(const wchar_t* _input)
     {
         // wchar_t* ���ڿ��� ���� ��� (NULL ���� ����)
         int length = wcslen(_input) + 1;

         // �ʿ��� ���� ũ�� ��� (wchar_t ���� * sizeof(wchar_t))
         int bufferSize = length * sizeof(wchar_t);

         // ���� �Ҵ�
         char* buffer = new char[bufferSize];

         // wchar_t* ���ڿ��� byte*�� ��ȯ
         memcpy(buffer, reinterpret_cast<const char*>(_input), bufferSize);

         return buffer;
     }

     //wchar_t* ���� ���
     size_t GetChar16Length(const char16_t* u16String) {
         const char16_t* p = u16String;
         while (*p)
             ++p;
         return p - u16String;
     }

     //16��Ʈ char���� 8��Ʈ char�� ��ȯ
     char* ConvertChar16ToByte(char16_t* u16String, int& _length) {
         // �ʿ��� ���� ũ�� ���
         _length = sizeof(char16_t) * GetChar16Length(u16String);

         // BYTE* ���� �Ҵ�
         char* byteArray = new char[_length];

         // char16_t*�� BYTE*�� ��ȯ
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

    //RTA ����
    dllWrapper.SendCommand("RtaStart", dataCommand1);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    char16_t dataCommand2[] = u"page=1,savepath=C:\\Users\\Kitronyx\\Desktop\\,memo=A��";

    //RTA���� ������ ����
    dllWrapper.SendCommand("Snapshot", dataCommand2);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    cout << dllWrapper.mCommandWorkList << endl;
    cout << dllWrapper.mCommandTypeList << endl;

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    //�ǽð� RTA ������ ������ �۽� ON
    dllWrapper.SendCommand("RealtimeDataOn", dataCommand1);

    std::cout << "Press enter for continue... ";
    std::getline(std::cin, input);

    int commandCode=0;
    string commandData = "";
    int commandDataLength=0;

    while (true)
    {
        //���Ź��� ���� ����
        dllWrapper.GetReceiveStackCommand();

        cout << "commandCode: " << commandCode;

        //���Ź��� ���� ���
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
    //�ǽð� RTA ������ ������ �۽� OFF
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
1. Ķ���극�̼� �ҷ����� (Page, ����̽� ��ȣ, ���� ��ȣ, Cal ���)
2. Ķ���극�̼� �����ϱ� (Ķ���극�̼��� �����Ǿ� ���� ���) (Page, ����̽� ��ȣ, ���� ��ȣ)
3. ����̽� ���� ���� (Page)
4. ���� ���� (Page)
5. ����
6. ���� ������Ʈ
7. ��� ���� (���� ���°� �ƴ� ���)
8. �Ͻ� ���� (���� ���°� �ƴ� ���)
9. �������� (����̽� ��ȣ, ���� ��ȣ, ���� ��)
10. RTA ���� (RTA�� ������� �ʰ� ���� ���) (Page)
11. RTA ���� (RTA�� ����ǰ� ���� ���) (Page)

(RTA ���� �߿���)
12. �α� �����ϱ�(�α� ���� ���� ���� ���) (Page, ���� ���, �޸�)
13. �α� ������(�α� ���� ���� ���) (Page)
14. ������ (Page, ���� ���, �޸�)
15. ��� ���� (������� ������ ������ ���ǵǾ� ���� ���) (Page)
16. ��� ���� ���� (������� ������ ������ ���ǵǾ� ���� ���) (Page, ���� ���, �޸�)
17. �ǽð� ������ ���� �ѱ� (�ǽð� ������ ������ ���� ���� ���) (Page)
18. �ǽð� ������ ���� ���� (�ǽð� ������ ������ ���� ���� ���) (Page)
19. 1�� ������ �ޱ� (Page)
20. ������� ON (������尡 OFF�� ���) (Page)
21. ������� OFF (������尡 ON�� ���) (Page)

22. page ����
23. ���� ��� ����
24. device ��ȣ ����
25. sensor ��ȣ ����
26. cal ���� ��� ����
27. ���� �� ����
28. �޸� ����

29. �ֻ��� ���� ���� �� ������ ���
30. ���� �ڵ� �Է����� ���� ������ ���
31. ���� ���� �Է����� ���� ������ ���

0. ����
)";

    //memo = new char16_t[6];
    char16_t test[] = u"��ȣ";
    u16string st = u"��ȣ";
    //string str = multibyte_to_utf8("��ȣ");
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
        //���Ź��� ���� ����
        /*dllWrapper.GetReceiveStackCommand(commandCode, commandData, commandDataLength);

        cout << "commandCode: " << commandCode;

        //���Ź��� ���� ���
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
        
        cout << "���� ���õ� ������: " << pageStr << endl;
        cout << "������ ���� ���: " << savepathStr << endl;
        cout << "������ ����̽� ��ȣ: " << deviceIndexStr << endl;
        cout << "������ ���� ��ȣ: " << sensorIndexStr << endl;
        cout << "������ Cal ���� ���: " << calPathStr << endl;
        cout << "������ ���� ��: " << sensitivityValueStr << endl;
        cout << "�޸� : "<< memoStr << endl;
       // wstring wString(memo.begin(), memo.end());

        // wstring ���
        //std::wcout << wString << std::endl;
        cout << endl;
        
        //dllWrapper.GetReceiveStackCommand(receiveCommandCode, receiveCommandData, receiveCommandDataLength);
        /*
        if (sendCommand != "")
        {
            dllWrapper.GetSearchReceiveStackCommandStr(sendCommand.c_str(), size(sendCommand), receiveCommandData, receiveCommandDataLength);
            
            cout << "commandCode: " << sendCommand;
            //���Ź��� ���� ���
            if (receiveCommandDataLength > 0)
            {
                cout << ", commandData:" << receiveCommandData << ", commandDataLength:" << receiveCommandDataLength;
            }
            cout << endl;
        }*/

        cout << selectScript << endl;
        cout << "������ Ŀ�ǵ�: " << inputCommand << endl;
        cout << endl;

        cout << "������ Ŀ�ǵ� �Է�(Enter): ";

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
                cout << "������ Page �Է�: ";
                std::getline(std::cin, inputValue);
                pageStr = "page=" + inputValue;
                pageSendData = u"page=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 23:
                cout << "���� ��� �Է�: ";
                std::getline(std::cin, inputValue);
                savepathStr = "savepath=" + inputValue;
                savepathSendData = u"savepath=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 24:
                cout << "������ ����̽� ��ȣ �Է�: ";
                std::getline(std::cin, inputValue);
                deviceIndexStr = "deviceindex=" + inputValue;
                deviceIndexSendData = u"deviceindex=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 25:
                cout << "������ ���� ��ȣ �Է�: ";
                std::getline(std::cin, inputValue);
                sensorIndexStr = "sensorindex=" + inputValue;
                sensorIndexSendData = u"sensorindex=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 26:
                cout << "Cal ��� �Է�: ";
                std::getline(std::cin, inputValue);
                calPathStr = "path=" + inputValue;
                calPathSendData = u"path=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 27:
                cout << "������ ���� �� �Է�: ";
                std::getline(std::cin, inputValue);
                sensitivityValueStr = "value=" + inputValue;
                sensitivityValueSendData = u"value=" + convertToUTF16(multibyte_to_utf8(inputValue));
                continue;
            case 28:
                cout << "�޸� �Է�: ";
                std::getline(std::cin, inputValue);
                memoSendData = u"memo=" + convertToUTF16(multibyte_to_utf8(inputValue));
                memoStr = "memo=" + inputValue;
                continue;
            case 29:
                resultData = dllWrapper.GetReceiveStackCommand();
                cout << endl;
                cout << "���� ������: " << resultData << endl;
                cout << endl;
                continue;
            case 30:
                commandCode = 0;
                while (true)
                {
                    cout << "���� ������ �� �˻� �� ���� �ڵ�(����) �Է�: ";
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
                cout << "���� ������: " << resultData << endl;
                cout << endl;
                continue;
            case 31:
                cout << "���� ������ �� �˻� �� ���� ��ȣ �Է�: ";
                std::getline(std::cin, inputValue);
                sendCommand = GetCommandStr(stoi(inputValue), sendCommandData, sendCommandDataLength, isGetReceiveData);
                resultData = dllWrapper.GetSearchReceiveStackCommandStr(sendCommand);
                cout << endl;
                cout << "���� ������: " << resultData << endl;
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
        //dllWrapper.SendCommand("��ȣ", sendCommandData);

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
        dllWrapper.Open(); // ����� ��� ����
        Sleep(1000);
        dllWrapper.GetCommandList(); // ���� ����Ʈ ������Ʈ

        //CommandTest1();
        QATest();
    }
    return 0;
}