# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import ctypes
from ctypes import cdll

# DLL을 로드합니다.
dll = cdll.LoadLibrary('Api_v3_0_Client_DLL.dll')

# 반환 값을 저장할 변수를 생성합니다.
result_ptr = ctypes.c_void_p()
# DLL 함수의 반환 타입을 설정합니다.
dll.CreateApiClient.restype = ctypes.c_void_p
result_ptr = dll.CreateApiClient()



dll.DisposeApiClient.restype = None
dll.DisposeApiClient.argtypes = [ctypes.c_void_p]

    
dll.ApiClient_Open.restype = None
dll.ApiClient_Open.argtypes = [ctypes.c_void_p]
dll.ApiClient_Open(result_ptr)

dll.ApiClient_Start.restype = None
dll.ApiClient_Start.argtypes = [ctypes.c_void_p]
dll.ApiClient_Start(result_ptr)


#dll.ApiClient_AsyncReceive.restype = None
#dll.ApiClient_AsyncReceive.argtypes = [ctypes.c_void_p]
#ApiClient_AsyncReceive(ApiClient* apiClientPtr, dataReceiveCallback* _callback);

#dll.ApiClient_SetInstance.restype = None
#dll.ApiClient_SetInstance.argtypes = [ctypes.c_void_p]
#void ApiClient_SetInstance(ApiClient*, void*);

stackCommandType = ctypes.c_int()
stackCommandDataLength = ctypes.c_int()
stackCommandData = ctypes.c_char_p()
dll.ApiClient_GetReceiveStackCommand.restype = None
dll.ApiClient_GetReceiveStackCommand.argtypes = [ctypes.c_void_p,ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
dll.ApiClient_GetReceiveStackCommand(result_ptr, ctypes.byref(stackCommandType), ctypes.byref(stackCommandData), ctypes.byref(stackCommandDataLength))

#messageType = 1
#messageLength = 0
#dll.ApiClient_SendCommand.restype = None
#dll.ApiClient_SendCommand.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.byref(str_value),ctypes.c_int]
#dll.ApiClient_SendCommand(result_ptr,messageType,ctypes.byref(str_value),messageLength)

CommandWorkListLength = ctypes.c_int()
CommandWorkList = ctypes.c_char_p()
CommandTypeListLength = ctypes.c_int()
CommandTypeList = ctypes.c_char_p()
dll.ApiClient_GetCommandList.restype = None
dll.ApiClient_GetCommandList.argtypes = [ctypes.c_void_p,ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
dll.ApiClient_GetCommandList(result_ptr, ctypes.byref(CommandWorkList), ctypes.byref(CommandWorkListLength), ctypes.byref(CommandTypeList), ctypes.byref(CommandTypeListLength))

print(CommandWorkList.value)

#dll.ApiClient_Stop.restype = None
#dll.ApiClient_Stop.argtypes = [ctypes.c_void_p]
#dll.ApiClient_Stop(result_ptr)

#dll.ApiClient_Close.restype = ctypes.c_bool
#dll.ApiClient_Close.argtypes = [ctypes.c_void_p]
#dll.ApiClient_Close(result_ptr)
