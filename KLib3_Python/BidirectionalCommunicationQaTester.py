# -*- coding: utf-8 -*-
"""
Created on Tue Oct 31 09:04:29 2023

@author: Kitronyx
"""

import os
import time
from ApiCppDllWrapper import CDllWrapper

import ctypes
from ctypes import cdll

import subprocess
import time
    
class ClientApiTester:
    def __init__(self):
        self.userPrintScript = """
1. 캘리브레이션 불러오기 (Page, 디바이스 번호, 센서 번호, Cal 경로)
2. 캘리브레이션 제거하기 (캘리브레이션이 설정되어 있을 경우) (Page, 디바이스 번호, 센서 번호)
3. 디바이스 센서 정보 (Page)
4. 연결 정보 (Page)
5. 버전
6. CommandUpdate
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

"""
        
        self.pageScript = "page=1"
        self.savePathScript = "savepath="
        self.deviceIndexScript = "deviceIndex="
        self.sensorIndexScript = "sensorIndex="
        self.calPathScript = "path="
        self.sensitivityScript = "value="
        self.memoScript = "memo="
        
        self.userInput = 0
        
        self.cDllWrapper = CDllWrapper()
        
        self.cDllWrapper.ApiOpen()
        #self.cDllWrappe.ApiStart()
        self.cDllWrapper.GetCommandList()
    
    
    def doWork(self):
        commandStr = ""
        commandSendData = ""
        
        while True:
            
            print("현재 선택된 페이지: " + self.pageScript)
            print("지정된 저장 경로: " + self.savePathScript)
            print("지정된 디바이스 번호: " + self.deviceIndexScript)
            print("지정된 센서 번호: " + self.sensorIndexScript)
            print("지정된 Cal 파일 경로: " + self.calPathScript)
            print("지정된 감도 값: " + self.sensitivityScript)
            print("메모: "+self.memoScript)
            
            print(self.userPrintScript)
            
            print("마지막 커맨드: " + str(self.userInput))
            
            self.userInput = int(input("수행할 커맨드 입력(Enter): "))
            
            if self.userInput == 0:
                print("Exit")
                break
            if self.userInput == 22:
                inputDataStr = input("변경할 Page 입력: ")
                self.pageScript = "page=" + str(inputDataStr)
                continue
            elif self.userInput == 23:
                inputDataStr = input("저장 경로 입력: ")
                self.savePathScript = "savepath=" + str(inputDataStr)
                continue
            elif self.userInput == 24:
                inputDataStr = input("변경할 디바이스 번호 입력: ")
                self.deviceIndexScript = "deviceindex=" + str(inputDataStr)
                continue
            elif self.userInput == 25:
                inputDataStr = input("변경할 센서 번호 입력: ")
                self.sensorIndexScript = "sensorindex=" + str(inputDataStr)
                continue
            elif self.userInput == 26:
                inputDataStr = input("Cal 경로 입력: ")
                self.calPathScript = "path=" + str(inputDataStr)
                continue
            elif self.userInput == 27:
                inputDataStr = input("변경할 감도 값 입력: ")
                self.sensitivityScript = "value=" + str(inputDataStr)
                continue
            elif self.userInput == 28:
                inputDataStr = input("메모 입력: ")
                self.memoScript = "memo=" + str(inputDataStr)
                continue
            elif self.userInput == 29:
                result = ""
                result = self.cDllWrapper.GetReceiveStackCommand();
                print()
                print("=================================================")
                print("지령 코드: "+ str(self.cDllWrapper.stackCommandType.value))
                print("수신 데이터: " + result)
                print("=================================================")
                print()
                continue;
            elif self.userInput == 30:
                inputSecondData = int(input("수신 데이터 내 검색 할 지령 코드 입력: "))
                receiveData = self.cDllWrapper.GetSearchReceiveStackCommand(inputSecondData)
                print()
                print("=================================================")
                print("수신 데이터: " + receiveData)
                print("=================================================")
                print()
                continue;
            elif self.userInput == 31:
                inputSecondData = int(input("수신 데이터 내 검색 할 지령 커맨드 번호 입력: "))
                commandStr, commandSendData = self.GetCommandStrmandStr(inputSecondData)
                receiveData = self.cDllWrapper.GetSearchReceiveStackCommandStr(commandStr)
                print()
                print("=================================================")
                print("수신 데이터: " + receiveData)
                print("=================================================")
                print()
                continue;
            
            commandStr, commandSendData = self.GetCommandStrmandStr(self.userInput)
            print(commandSendData)
            self.cDllWrapper.SendCommandByStr(commandStr, commandSendData)
            
    def GetArgStr(self,_option):
        commandSendData = ""
        result = ""
        
        # 1000 0000
        if (_option & 0x80) > 0:
            result += self.pageScript
            
        # 0100 0000
        if (_option & 0x40) > 0:
            if result != "":
                result += ","
            result += self.savePathScript

        # 0010 0000
        if (_option & 0x20) > 0:
            if result != "":
                result += ","
            result += self.deviceIndexScript

        # 0001 0000
        if (_option & 0x10) > 0:
            if result != "":
                result += ","
            result += self.sensorIndexScript

        # 0000 1000
        if (_option & 0x08) > 0:
            if result != "":
                result += ","
            result += self.calPathScript

        # 0000 0100
        if (_option & 0x04) > 0:
            if result != "":
                result += ","
            result += self.sensitivityScript
            
            # 0000 0010
        if (_option & 0x02) > 0:
            if result != u"":
                result += u","
            result += self.memoScript

        # 0000 0001
        if (_option & 0x01) > 0:
            pass
        
        commandSendData = result
    
        return commandSendData

        
    def GetCommandStrmandStr(self,_input):
        commandSendData = ""
        result = ""
        
        if _input == 0:
            result = "exit"
        elif _input == 1:
            result = "CalImport"
            commandSendData = self.GetArgStr(0xB8)
        elif _input == 2:
            result = "CalRelease"
            commandSendData = self.GetArgStr(0xB0)
        elif _input == 3:
            result = "DeviceSensorInfo"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 4:
            result = "ConnectionInfo"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 5:
            result = "Version"
        elif _input == 6:
            result = "Update"
        elif _input == 7:
            result = "EmergencyStop"
        elif _input == 8:
            result = "Pause"
        elif _input == 9:
            result = "Sensitivity"
            commandSendData = self.GetArgStr(0x34)
        elif _input == 10:
            result = "RtaStart"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 11:
            result = "RtaEnd"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 12:
            result = "LogStart"
            commandSendData = self.GetArgStr(0xC2)
        elif _input == 13:
            result = "LogEnd"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 14:
            result = "Snapshot"
            commandSendData = self.GetArgStr(0xC2)
        elif _input == 15:
            result = "Decision"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 16:
            result = "DecisionResultSave"
            commandSendData = self.GetArgStr(0xC2)
        elif _input == 17:
            result = "RealtimeDataOn"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 18:
            result = "RealtimeDataOff"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 19:
            result = "MatrixData"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 20:
            result = "AccumulOn"
            commandSendData = self.GetArgStr(0x80)
        elif _input == 21:
            result = "AccumulOff"
            commandSendData = self.GetArgStr(0x80)
            
        commandStr = result
        
        return commandStr, commandSendData

    
if __name__ == "__main__":
    # 프로그램 비동기 실행
    process = subprocess.Popen(["C:\\Program Files (x86)\\Kitronyx\\ForceLAB2\\ForceLAB2.exe"])#ForceLAB2 기본 설치 경로
    
    clientApiTester = ClientApiTester()
    clientApiTester.doWork()
    
    # 5초 대기
    time.sleep(5)

    # 종료
    process.terminate()  # 윈도우에서는 강제종료에 가까움
    process.wait()       # 종료 완료될 때까지 대기
    