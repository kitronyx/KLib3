# -*- coding: utf-8 -*-
"""
Created on Fri Jun 16 20:50:37 2023

@author: Kitronyx
"""

import ctypes
from ctypes import cdll

class CDllWrapper:
    def __init__(self):
        self.dll = cdll.LoadLibrary('KLib3.dll')
        self.dllWrapperPtr = ctypes.c_void_p()
        self.dll.CreateApiClient.restype = ctypes.c_void_p
        self.dllWrapperPtr = self.dll.CreateApiClient()    
    
        self.dll.DisposeApiClient.restype = None
        self.dll.DisposeApiClient.argtypes = [ctypes.c_void_p]
        
        self.dll.ApiClient_Open.restype = None
        self.dll.ApiClient_Open.argtypes = [ctypes.c_void_p]
    
        self.dll.ApiClient_GetReceiveStackCommand.restype = None
        self.dll.ApiClient_GetReceiveStackCommand.argtypes = [ctypes.c_void_p,ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
    
        self.dll.ApiClient_GetCommandList.restype = None
        self.dll.ApiClient_GetCommandList.argtypes = [ctypes.c_void_p,ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_char_p),ctypes.POINTER(ctypes.c_int)]
        
        self.dll.ApiClient_SendCommandByStr.restype = None
        self.dll.ApiClient_SendCommandByStr.argtypes = [ctypes.c_void_p,ctypes.c_char_p,ctypes.c_int,ctypes.c_char_p,ctypes.c_int]
        
        self.dll.ApiClient_SendCommandByCode.restype = None
        self.dll.ApiClient_SendCommandByCode.argtypes = [ctypes.c_void_p,ctypes.c_char_p,ctypes.c_int]

        self.dll.ApiClient_Close.restype = ctypes.c_bool
        self.dll.ApiClient_Close.argtypes = [ctypes.c_void_p]

        self.CommandWorkListLength = ctypes.c_int()
        self.CommandWorkList = ctypes.c_char_p()
        self.CommandTypeListLength = ctypes.c_int()
        self.CommandTypeList = ctypes.c_char_p()
        self.stackCommandType = ctypes.c_int()
        self.stackCommandDataLength = ctypes.c_int()
        self.stackCommandData = ctypes.c_char_p()
        
    def ApiOpen(self):
        self.dll.ApiClient_Open(self.dllWrapperPtr)
        
    def GetReceiveStackCommand(self):
        self.dll.ApiClient_GetReceiveStackCommand(self.dllWrapperPtr, ctypes.byref(self.stackCommandType), ctypes.byref(self.stackCommandData), ctypes.byref(self.stackCommandDataLength))
        
    def GetCommandList(self):
        self.dll.ApiClient_GetCommandList(self.dllWrapperPtr, ctypes.byref(self.CommandWorkList), ctypes.byref(self.CommandWorkListLength), ctypes.byref(self.CommandTypeList), ctypes.byref(self.CommandTypeListLength))
    
    def SendCommandByStr(self, _commadCodeWorkStr,_commandData):
        commandWorkCharPointer = self.ConvertCharToStringPointer(_commadCodeWorkStr)
        commandDatawCharPointer = self.ConvertStringToUnicodeBytes(_commandData)
        self.dll.ApiClient_SendCommandByStr(self.dllWrapperPtr, commandWorkCharPointer,len(_commadCodeWorkStr), commandDatawCharPointer ,len(_commandData) * 2)
        
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