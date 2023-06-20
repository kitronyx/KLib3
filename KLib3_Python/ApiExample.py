# -*- coding: utf-8 -*-
"""
Created on Fri Jun 16 21:32:44 2023

@author: Kitronyx
"""
import time
from ApiCppDllWrapper import CDllWrapper

cDllWrappe = CDllWrapper()


cDllWrappe.ApiOpen()
cDllWrappe.ApiStart()
cDllWrappe.GetCommandList()

user_input = input("Press Enter to continue...")

cDllWrappe.SendCommandByStr("RtaStart","page=1")

user_input = input("Press Enter to continue...")

cDllWrappe.SendCommandByStr("Snapshot","page=1,savepath=C:\\Users\\Kitronyx\\Desktop\\,memo=Aㅁ")

user_input = input("Press Enter to continue...")

cDllWrappe.SendCommandByStr("RealtimeDataOn","page=1")

while True:
    user_input = input("Press Enter to continue...")
    
    if user_input =='a' or user_input=='A':
        break;
    
    cDllWrappe.GetReceiveStackCommand()
    
    DataStr = "CommandType: " + str(cDllWrappe.stackCommandType.value) + ", CommandData: "
    
    if cDllWrappe.stackCommandDataLength.value > 0:
        #DataStr = DataStr + cDllWrappe.stackCommandData.value.decode('utf-16le') + ", CommandDataLength: " + str(cDllWrappe.stackCommandDataLength.value)
        DataStr = DataStr + cDllWrappe.stackCommandData.value.decode() + ", CommandDataLength: " + str(cDllWrappe.stackCommandDataLength.value)
    else:
        DataStr = DataStr + ", CommandDataLength: " + str(cDllWrappe.stackCommandDataLength.value)
    
    print(DataStr)

