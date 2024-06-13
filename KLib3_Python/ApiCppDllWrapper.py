# -*- coding: utf-8 -*-
"""
Created on Fri Jun 16 20:50:37 2023

@author: Kitronyx
"""

import ctypes
from ctypes import cdll

class CDllWrapper:
    def __init__(self):
        
        self.dll = cdll.LoadLibrary('.\KLib3.dll')
        
        # 생성자 초기화
        self.dll.CreateApiClient.restype = ctypes.c_void_p
        # API Handler
        self.dllWrapperPtr = ctypes.c_void_p()
        self.dllWrapperPtr = self.dll.CreateApiClient()    
        # 소멸자
        self.dll.DisposeApiClient.restype = None
        self.dll.DisposeApiClient.argtypes = [ctypes.c_void_p]
        # API 통신 시작
        self.dll.ApiClient_Open.restype = ctypes.c_bool
        self.dll.ApiClient_Open.argtypes = [ctypes.c_void_p]
        # 수신 지령 중 가장 먼저 쌓인 지령을 출력
        self.dll.ApiClient_GetReceiveStackCommand.restype = None
        self.dll.ApiClient_GetReceiveStackCommand.argtypes = [ctypes.c_void_p,ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
        
        # 수신 지령 중 지령 코드를 이용해 특정 지령을 출력
        self.dll.ApiClient_GetSearchReceiveStackCommand.restype = None
        self.dll.ApiClient_GetSearchReceiveStackCommand.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
        # 수신 지령 중 지령 문자열을 이용해 특정 지령을 출력
        self.dll.ApiClient_GetSearchReceiveStackCommandStr.restype = None
        self.dll.ApiClient_GetSearchReceiveStackCommandStr.argtypes = [ctypes.c_void_p,ctypes.c_char_p,ctypes.c_int,ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
        
        # 수행 지령(Work Command) 목록와 지령 타입(Type Command)을 
        self.dll.ApiClient_GetCommandList.restype = None
        self.dll.ApiClient_GetCommandList.argtypes = [ctypes.c_void_p,ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
        # 지령 이름 문자열로 지령 송신
        self.dll.ApiClient_SendCommandByStr.restype = None
        self.dll.ApiClient_SendCommandByStr.argtypes = [ctypes.c_void_p,ctypes.c_char_p,ctypes.c_int,ctypes.c_char_p,ctypes.c_int]
        # 지령 코드로 지령 송신
        self.dll.ApiClient_SendCommandByCode.restype = None
        self.dll.ApiClient_SendCommandByCode.argtypes = [ctypes.c_void_p,ctypes.c_char_p,ctypes.c_int]
        # API 통신 종료
        self.dll.ApiClient_Close.restype = ctypes.c_bool
        self.dll.ApiClient_Close.argtypes = [ctypes.c_void_p]
        # 지령 인자 예시
        self.CommandWorkListLength = ctypes.c_int()
        self.CommandWorkList = ctypes.c_char_p()
        self.CommandTypeListLength = ctypes.c_int()
        self.CommandTypeList = ctypes.c_char_p()
        
        self.stackCommandType = ctypes.c_int()
        self.stackCommandDataLength = ctypes.c_int()
        self.stackCommandData = ctypes.c_char_p()
        
    def ApiOpen(self):
        return self.dll.ApiClient_Open(self.dllWrapperPtr)
        
    def GetReceiveStackCommand(self):
        self.dll.ApiClient_GetReceiveStackCommand(self.dllWrapperPtr, ctypes.byref(self.stackCommandType), ctypes.byref(self.stackCommandData), ctypes.byref(self.stackCommandDataLength))
        result = ""
        if self.stackCommandType == 9 or self.stackCommandType == 1048576:
            for i in range(self.stackCommandDataLength.value):
                if result != "" :
                    result += ", "
                result += str(self.stackCommandData.value[i])
        else:
            print(self.stackCommandData.value)
        return result
        
    def GetSearchReceiveStackCommand(self, _commandCode):
        self.dll.ApiClient_GetSearchReceiveStackCommand(self.dllWrapperPtr, _commandCode, ctypes.byref(self.stackCommandData), ctypes.byref(self.stackCommandDataLength))
        result = ""
        if _commandCode == 9 or _commandCode == 1048576:
            for i in range(self.stackCommandDataLength.value):
                if result != "" :
                    result += ", "
                result += str(self.stackCommandData.value[i])
        else:
            result = self.stackCommandData.value.decode()
        return result
        
    def GetSearchReceiveStackCommandStr(self, _commandStr):
        commandStr = self.ConvertCharToStringPointer(_commandStr)
        self.dll.ApiClient_GetSearchReceiveStackCommandStr(self.dllWrapperPtr, commandStr, len(_commandStr), ctypes.byref(self.stackCommandData), ctypes.byref(self.stackCommandDataLength))
        result = ""
        if _commandStr == "MatrixData" or _commandStr == "RealtimeDataOn" :
            for i in range(self.stackCommandDataLength.value):
                if result != "" :
                    result += ", "
                result += str(self.stackCommandData.value[i])
        else:
            result = self.stackCommandData.value.decode()
        return result
        
    def GetCommandList(self):
        self.dll.ApiClient_GetCommandList(self.dllWrapperPtr, ctypes.byref(self.CommandWorkList), ctypes.byref(self.CommandWorkListLength), ctypes.byref(self.CommandTypeList), ctypes.byref(self.CommandTypeListLength))
    
    def SendCommandByStr(self, _commadCodeWorkStr,_commandData):
        commandWorkCharPointer = self.ConvertCharToStringPointer(_commadCodeWorkStr)
        commandDatawCharPointer = self.ConvertStringToUnicodeBytes(_commandData)
        self.dll.ApiClient_SendCommandByStr(self.dllWrapperPtr, commandWorkCharPointer, len(_commadCodeWorkStr), commandDatawCharPointer ,len(_commandData) * 2)
        
    def SendCommandByCode(self, _commadCode,_commandData):
        self.dll.ApiClient_SendCommandByCode(self.dllWrapperPtr, _commadCode, self.ConvertWcharToStringPointer(_commandData),len(_commandData) * 2)
        
    def Close(self):
        self.dll.ApiClient_Close(self.dllWrapperPtr)
        
    def ConvertCharToStringPointer(self, string_value):
        c_string = ctypes.c_char_p(string_value.encode())
        #c_string_pointer = ctypes.pointer(c_string)
        return c_string
    
    def ConvertWcharToStringPointer(self, string_value):
        c_string = ctypes.c_wchar_p(string_value)
        c_string_pointer = ctypes.pointer(c_string)
        return c_string_pointer    
    
    def ConvertStringToUnicodeBytes(self, string_value):
        encoded_string =  ctypes.c_char_p(string_value.encode('utf-16le'))
        return encoded_string
