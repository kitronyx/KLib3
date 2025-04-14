# KLib3 Full API Reference and Quick Starter Guide

> This document is intended for third-party developers who integrate ForceLAB2 with external systems.

Note:  
This guide assumes that you are using the official wrapper code provided for KLib3:

- For C++, use the `MyDLLWrapper` class implementation.
- For C#, use the `KLib3Csharp` class with `DllImport` bindings.
- For Python, use the `CDllWrapper` class via `ctypes` provided in `ApiCppDllWrapper.py`.

These wrapper classes are required to correctly access native functions in `KLib3.dll`.

---

## How to Run / Set Up

### Python
1. Place `KLib3.dll` in the same folder as your Python script or add to system PATH.
2. Make sure you're using 64-bit Python 3.11.7 or later.
3. Use `ApiCppDllWrapper.py` and call functions like `SendCommandByCode()`.

```bash
python example.py
```

### C# / C++
- Ensure project target platform is **x64**.
- Place `KLib3.dll` in the output directory (`bin\x64\Debug` or equivalent).
- Add interop class and start with the quick starter example below.

---

## Development Environment

| Item | Description |
|------|-------------|
| Operating System | Windows 10 or later (x64 required) |
| Development Tools | Visual Studio 2022 (MSVC v143, Windows SDK 10.0+) / Python 3.11.7 |
| Framework | .NET Framework 4.8.1 (for C#) |
| Required DLL | `KLib3.dll` (64-bit only — all projects must be built for x64) |

---

## Language Support

| Language |
|----------|
| C++14 (MSVC v143, Windows SDK 10.0+) |
| C# (.NET Framework 4.8.1) |
| Python 3.11.7 |

---

## Quick Starter Examples

### C++ (with MyDLLWrapper)
```cpp
MyDLLWrapper api;
api.LoadDLL("KLib3.dll");
api.Open();
char16_t data[] = u"page=1,savepath=C:\\your\path,memo=test";
api.SendCommand(4, data);  // 4 = Snapshot
std::string result = api.GetReceiveStackCommand();
api.Close();
```

### C# (.NET Framework 4.8.1)
```csharp
KLib3Csharp api = new KLib3Csharp();
api.Open();
string sendData = "page=1,savepath=C:\\your\path,memo=test";
api.SendCommand(4, sendData, sendData.Length);
api.GetSearchStackReceiveCommand("Snapshot", out string result);
api.Close();
```

### Python (with ApiCppDllWrapper.py)
```python
from ApiCppDllWrapper import CDllWrapper

api = CDllWrapper()
api.ApiOpen()
api.SendCommandByStr("RtaStart", "page=1")
api.SendCommandByStr("Snapshot", "page=1,savepath=C:\\your\path,memo=Test")
api.GetReceiveStackCommand()
api.ApiClose()
```

---

## Available Commands

| Command Name | Code | Description |
|--------------|------|-------------|
| `CalImport` | 0 | Load calibration file |
| `CalRelease` | 1 | Unload calibration file |
| `LogStart` / `LogEnd` | 2 / 3 | Start or stop logging |
| `Snapshot` | 4 | Capture a snapshot |
| `Decision` / `DecisionResultSave` | 5 / 8 | Run decision / save result |
| `RealtimeDataOn / Off` | 9 / 10 | Start/Stop real-time stream |
| `MatrixData` | 1048576 | Receive a single frame |
| `Sensitivity` / `GetSensitivity` | 1048579 / 1048580 | Adjust or get sensitivity |
| `DeviceSensorInfo` | 2097152 | Retrieve device/sensor info |
| `RtaStart / RtaEnd` | 15728643 / 15728645 | Start/stop real-time analyzer |
| `Version` | 15728640 | Request the current version of ForceLAB2 |
| `Update` | 15728641 | Request latest command info from ForceLAB2 and save to data.dat |

---

## Command Parameter Explanations

| Parameter | Description |
|-----------|-------------|
| `page` | Refers to the active page configuration in ForceLAB2. Each page can be configured with different sensor layouts. |
| `savepath` | Directory path where logs, snapshots, or decision results are saved. |
| `deviceindex` | Index of the device connected to ForceLAB2, based on order of connection. |
| `sensorindex` | Index of the sensor connected to each device. |
| `value` (Sensitivity) | Sensitivity multiplier for each sensor; the actual value is multiplied before transmission. |
| `path` (CalPath) | Directory where calibration data is stored. |
| `memo` | Optional comment to be saved alongside snapshots, logs, or decision outputs. |

---

## Command Code Usage

You can send commands by either string name or numeric code.

### Python Example
```python
api.SendCommandByCode(4, b"page=1,savepath=C:\\your\path,memo=test")
```

### C++
```cpp
char16_t data[] = u"page=1,savepath=C:\\your\path,memo=test";
dllWrapper.SendCommand(4, data);
```

### C#
```csharp
string data = "page=1,savepath=C:\\your\path,memo=test";
api.SendCommand(4, data, data.Length);
```

---

## Static vs Dynamic DLL Usage

| Language | Static Load | Dynamic Load |
|----------|-------------|--------------|
| C++ | `#pragma comment(lib, ...)` or link via project | `LoadLibrary()` + `GetProcAddress()` |
| C# | `[DllImport]` (P/Invoke) | Not applicable |
| Python | Not applicable | `ctypes.CDLL("KLib3.dll")` |

---

## DLL Distribution

`KLib3.dll` is a proprietary binary.  
This repository does not include the DLL for licensing reasons.  
Please contact **Kitronyx** to obtain a valid version of `KLib3.dll`.

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| DLL not found | Ensure `KLib3.dll` is in the working directory or system PATH |
| Python 32-bit error | Use 64-bit Python to match the 64-bit DLL |
| `SendCommand` succeeds but no response | Ensure ForceLAB2 is powered on, connected, and initialized |
| `GetCommandList()` returns empty | Call after successful `Open()` and verify ForceLAB2 is ready |

---

## Documentation Links

- [KLib Manual (Korean)](https://github.com/kitronyx/KLib3/blob/main/doc/240105_%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%ED%86%B5%EC%8B%A0%20%EB%A7%A4%EB%89%B4%EC%96%BC.pdf)
- [KLib Command List & Examples (Korean)](https://github.com/kitronyx/KLib3/blob/main/doc/%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%EC%A7%80%EB%A0%B9%20%EB%AA%A9%EB%A1%9D%20%EB%B0%8F%20%EC%98%88%EC%8B%9C%20v2.7.1%20Retail.pdf)

---

## License

This repository is distributed for documentation and code reference.  
The binary DLL and SDK may require a commercial license.  
For inquiries, contact **https://www.kitronyx.com/support_request** or your Kitronyx representative.

---

# KLib3 전체 API 레퍼런스 및 퀵 스타터 가이드

> 이 문서는 ForceLAB2를 외부 시스템과 연동하려는 개발자를 위한 안내서입니다.

본 문서는 다음과 같은 공식 래퍼 클래스를 사용하는 것을 전제로 합니다:

- **C++**: `MyDLLWrapper` 클래스 사용
- **C#**: `KLib3Csharp` 클래스 및 `[DllImport]` 인터페이스 사용
- **Python**: `ApiCppDllWrapper.py`의 `CDllWrapper` 클래스 사용 (`ctypes` 기반)

`KLib3.dll`의 네이티브 기능을 안전하게 사용하려면 위 래퍼 클래스가 필요합니다.

---

## 실행 및 구성 방법

### Python
1. `KLib3.dll`을 Python 스크립트와 같은 폴더에 두거나 시스템 PATH에 추가합니다.
2. 64비트 Python 3.11.7 이상을 사용해야 합니다.
3. `ApiCppDllWrapper.py`를 활용하여 명령어를 전송합니다.

```bash
python example.py
```

### C# / C++
- 반드시 프로젝트의 대상 플랫폼을 **x64**로 설정해야 합니다.
- 실행 파일(`bin\x64\Debug` 등)과 같은 경로에 `KLib3.dll`을 배치하세요.
- 퀵 스타터 예제를 참고하여 연결 후 명령을 송신할 수 있습니다.

---

## 개발 환경

| 항목 | 설명 |
|------|------|
| 운영체제 | Windows 10 이상 (64비트 필수) |
| 개발 도구 | Visual Studio 2022 (MSVC v143, Windows SDK 10.0+) / Python 3.11.7 |
| 프레임워크 | .NET Framework 4.8.1 (C#용) |
| 필수 DLL | `KLib3.dll` (64비트 전용 — 반드시 x64로 빌드해야 함) |

---

## 지원 언어

| 언어 |
|------|
| C++14 (MSVC v143, Windows SDK 10.0+) |
| C# (.NET Framework 4.8.1) |
| Python 3.11.7 |

---

## 퀵 스타터 예제

### C++ (`MyDLLWrapper` 사용)
```cpp
MyDLLWrapper api;
api.LoadDLL("KLib3.dll");
api.Open();
char16_t data[] = u"page=1,savepath=C:\\your\path,memo=test";
api.SendCommand(4, data);  // 4 = Snapshot
std::string result = api.GetReceiveStackCommand();
api.Close();
```

### C# (.NET Framework 4.8.1)
```csharp
KLib3Csharp api = new KLib3Csharp();
api.Open();
string sendData = "page=1,savepath=C:\\your\path,memo=test";
api.SendCommand(4, sendData, sendData.Length);
api.GetSearchStackReceiveCommand("Snapshot", out string result);
api.Close();
```

### Python (`ApiCppDllWrapper.py` 사용)
```python
from ApiCppDllWrapper import CDllWrapper

api = CDllWrapper()
api.ApiOpen()
api.SendCommandByStr("RtaStart", "page=1")
api.SendCommandByStr("Snapshot", "page=1,savepath=C:\\your\path,memo=Test")
api.GetReceiveStackCommand()
api.ApiClose()
```

---

## 사용 가능한 명령어

| 명령어 | 코드 | 설명 |
|--------|------|------|
| `CalImport` | 0 | 캘리브레이션 파일 로드 |
| `CalRelease` | 1 | 캘리브레이션 파일 언로드 |
| `LogStart` / `LogEnd` | 2 / 3 | 로그 시작 / 종료 |
| `Snapshot` | 4 | 스냅샷 캡처 |
| `Decision` / `DecisionResultSave` | 5 / 8 | 양불 판정 실행 / 저장 |
| `RealtimeDataOn / Off` | 9 / 10 | 실시간 데이터 송수신 ON/OFF |
| `MatrixData` | 1048576 | 단일 프레임 수신 |
| `Sensitivity` / `GetSensitivity` | 1048579 / 1048580 | 민감도 설정 / 조회 |
| `DeviceSensorInfo` | 2097152 | 디바이스 및 센서 정보 조회 |
| `RtaStart / RtaEnd` | 15728643 / 15728645 | 실시간 분석기 시작 / 종료 |
| `Version` | 15728640 | ForceLAB2의 버전 조회 |
| `Update` | 15728641 | 명령어 목록 업데이트 및 data.dat 저장 요청 |

---

## 명령어 인자 설명

| 인자 | 설명 |
|------|------|
| `page` | ForceLAB2의 페이지 설정. 각 페이지는 센서 배치를 다르게 구성할 수 있음 |
| `savepath` | 로그, 스냅샷, 양불 결과 저장 경로 |
| `deviceindex` | 연결된 디바이스의 순번 |
| `sensorindex` | 해당 디바이스에 연결된 센서의 순번 |
| `value` (민감도) | 센서에 곱해질 감도 배율 값 |
| `path` (CalPath) | 캘리브레이션 파일이 위치한 디렉토리 |
| `memo` | 로그나 판정 결과에 첨부될 사용자 메모 |

---

## 명령어 코드 사용 방법

문자열 또는 코드(정수)를 사용하여 명령어를 보낼 수 있습니다.

### Python 예시
```python
api.SendCommandByCode(4, b"page=1,savepath=C:\\your\path,memo=test")
```

### C++
```cpp
char16_t data[] = u"page=1,savepath=C:\\your\path,memo=test";
dllWrapper.SendCommand(4, data);
```

### C#
```csharp
string data = "page=1,savepath=C:\\your\path,memo=test";
api.SendCommand(4, data, data.Length);
```

---

## DLL 로딩 방식 (언어별)

| 언어 | 정적 로딩 | 동적 로딩 |
|------|------------|-----------|
| C++ | `#pragma comment(lib, ...)`, 프로젝트에 링크 | `LoadLibrary()` 및 `GetProcAddress()` |
| C# | `[DllImport]` 사용 (P/Invoke) | 없음 |
| Python | 없음 | `ctypes.CDLL("KLib3.dll")` 사용 |

---

## DLL 배포 안내

`KLib3.dll`은 Kitronyx의 상용 DLL로, 이 저장소에는 포함되어 있지 않습니다.  
**정식 사용을 위해 Kitronyx에 라이선스를 요청하시기 바랍니다.**

---

## 자주 발생하는 문제 해결

| 문제 | 해결 방법 |
|------|-----------|
| DLL not found | `KLib3.dll`이 실행 파일 폴더 또는 시스템 PATH에 있는지 확인 |
| Python 32비트 오류 | 반드시 64비트 Python 사용 |
| 명령 송신했으나 응답 없음 | ForceLAB2가 켜져 있고 연결된 상태인지 확인 |
| `GetCommandList()`가 비어 있음 | `Open()` 이후 ForceLAB2가 초기화되었는지 확인 |

---

## 공식 문서 링크

- [KLib 매뉴얼 (한글)](https://github.com/kitronyx/KLib3/blob/main/doc/240105_%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%ED%86%B5%EC%8B%A0%20%EB%A7%A4%EB%89%B4%EC%96%BC.pdf)
- [KLib 명령어 목록 및 예시 (한글)](https://github.com/kitronyx/KLib3/blob/main/doc/%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%EC%A7%80%EB%A0%B9%20%EB%AA%A9%EB%A1%9D%20%EB%B0%8F%20%EC%98%88%EC%8B%9C%20v2.7.1%20Retail.pdf)

---

## 라이선스

이 저장소는 문서 및 코드 예시 참고용으로 배포됩니다.  
`KLib3.dll`과 SDK는 상업적 사용을 위한 별도 라이선스가 필요할 수 있습니다.  
문의: **https://www.kitronyx.co.kr/support_request** 또는 담당자에게 문의해 주세요.