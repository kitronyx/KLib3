#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <locale>
#include <codecvt>

using namespace std;

typedef void* (*CreateApiClient)(); //생성자
typedef void (*DisposeApiClient)(void*); //소멸자
typedef void (*ApiClient_Open)(void*); //양방향 API 시작
typedef bool (*ApiClient_Close)(void*); //양방향 API 끝
typedef void (*ApiClient_GetReceiveStackCommand)(void*, int&, char*&, int&); //수신 지령 받기
typedef void (*ApiClient_SendCommandByCode)(void*, int, const char*, int); //지령을 Code(Integer)로, 지령 내용을 문자열로 보내기
typedef void (*ApiClient_SendCommandByStr)(void*, const char*, int, const char*, int); //지령 및 지령 내용을 문자열로 보내기
typedef void (*ApiClient_GetCommandList)(void*, char*&, int&, char*&, int&); //지령 리스트 얻기

class MyDLLWrapper {
private:
    HINSTANCE dllHandle;

    void* mApiClient;

    CreateApiClient FuncCreateApiClent; //생성지
    DisposeApiClient FuncDisposeApiClient; //소멸자
    ApiClient_Open FuncApiClient_Open; //양방향 API 통신 시작
    ApiClient_Close FuncApiClient_Close; // 양방향 API 통신 끝
    ApiClient_GetReceiveStackCommand FuncApiClient_GetReceiveStackCommand; // 받은 Command 확인
    ApiClient_SendCommandByCode FuncApiClient_SendCommandByCode; // 지령을 Code(integer)로, 지령 내용은 문자열로 보내기
    ApiClient_SendCommandByStr FuncApiClient_SendCommandByStr; // 지령 및 지령 내용을 문자열로 보내기
    ApiClient_GetCommandList FuncApiClient_GetCommandList; // 지령 리스트

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

        return true;
    }

    //양방향 통신 시작
     void Open()
     {
         //초기화
         mApiClient = FuncCreateApiClent();

         //양방향 통신 시작
         FuncApiClient_Open(mApiClient);
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
     void GetReceiveStackCommand(int& _getCommandCode, char*& _getCommandData, int& _getCommandDataLength)
     {
         FuncApiClient_GetReceiveStackCommand(mApiClient, _getCommandCode, _getCommandData, _getCommandDataLength);
     }

     //양방향 API 종료
     void Close()
     {
         FuncApiClient_Close(mApiClient);
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


int main() {
    MyDLLWrapper dllWrapper;

    if (!dllWrapper.LoadDLL("KLib3.dll")) {
        cout << "Fail load DLL file." << endl;
    }

    Sleep(1000);
    dllWrapper.Open(); // 양방향 통신 시작
    Sleep(1000);
    dllWrapper.GetCommandList(); // 지령 리스트 업데이트

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

    int commandCode;
    char* commandData;
    int commandDataLength;

    while (true)
    {
        //수신받은 지령 받음
        dllWrapper.GetReceiveStackCommand(commandCode, commandData, commandDataLength);

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

    return 0;
}