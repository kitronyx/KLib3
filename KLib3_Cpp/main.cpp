#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <locale>
#include <codecvt>

using namespace std;

typedef void* (*CreateApiClient)(); //������
typedef void (*DisposeApiClient)(void*); //�Ҹ���
typedef void (*ApiClient_Open)(void*); //����� API ����
typedef bool (*ApiClient_Close)(void*); //����� API ��
typedef void (*ApiClient_GetReceiveStackCommand)(void*, int&, char*&, int&); //���� ���� �ޱ�
typedef void (*ApiClient_SendCommandByCode)(void*, int, const char*, int); //������ Code(Integer)��, ���� ������ ���ڿ��� ������
typedef void (*ApiClient_SendCommandByStr)(void*, const char*, int, const char*, int); //���� �� ���� ������ ���ڿ��� ������
typedef void (*ApiClient_GetCommandList)(void*, char*&, int&, char*&, int&); //���� ����Ʈ ���

class MyDLLWrapper {
private:
    HINSTANCE dllHandle;

    void* mApiClient;

    CreateApiClient FuncCreateApiClent; //������
    DisposeApiClient FuncDisposeApiClient; //�Ҹ���
    ApiClient_Open FuncApiClient_Open; //����� API ��� ����
    ApiClient_Close FuncApiClient_Close; // ����� API ��� ��
    ApiClient_GetReceiveStackCommand FuncApiClient_GetReceiveStackCommand; // ���� Command Ȯ��
    ApiClient_SendCommandByCode FuncApiClient_SendCommandByCode; // ������ Code(integer)��, ���� ������ ���ڿ��� ������
    ApiClient_SendCommandByStr FuncApiClient_SendCommandByStr; // ���� �� ���� ������ ���ڿ��� ������
    ApiClient_GetCommandList FuncApiClient_GetCommandList; // ���� ����Ʈ

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

        return true;
    }

    //����� ��� ����
     void Open()
     {
         //�ʱ�ȭ
         mApiClient = FuncCreateApiClent();

         //����� ��� ����
         FuncApiClient_Open(mApiClient);
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
     void GetReceiveStackCommand(int& _getCommandCode, char*& _getCommandData, int& _getCommandDataLength)
     {
         FuncApiClient_GetReceiveStackCommand(mApiClient, _getCommandCode, _getCommandData, _getCommandDataLength);
     }

     //����� API ����
     void Close()
     {
         FuncApiClient_Close(mApiClient);
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


int main() {
    MyDLLWrapper dllWrapper;

    if (!dllWrapper.LoadDLL("KLib3.dll")) {
        cout << "Fail load DLL file." << endl;
    }

    Sleep(1000);
    dllWrapper.Open(); // ����� ��� ����
    Sleep(1000);
    dllWrapper.GetCommandList(); // ���� ����Ʈ ������Ʈ

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

    int commandCode;
    char* commandData;
    int commandDataLength;

    while (true)
    {
        //���Ź��� ���� ����
        dllWrapper.GetReceiveStackCommand(commandCode, commandData, commandDataLength);

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

    return 0;
}