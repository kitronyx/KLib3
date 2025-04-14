# KLib3 Full API Reference and Quick Starter Guide

Note:  
This guide assumes that you are using the official wrapper code provided for KLib3:

- For C++, use the `MyDLLWrapper` class implementation.
- For C#, use the `KLib3Csharp` class with `DllImport` bindings.
- For Python, use the `CDllWrapper` class via `ctypes` provided in `ApiCppDllWrapper.py`.

These wrapper classes are required to correctly access native functions in `KLib3.dll`.

## Development Environment

| Item | Description |
|------|-------------|
| Operating System | Windows 10 or later (x64 required) |
| Development Tools | Visual Studio 2022 (MSVC v143, Windows SDK 10.0+) / Python 3.11.7 |
| Framework | .NET Framework 4.8.1 (for C#) |
| Required DLL | `KLib3.dll` (64-bit only — all projects must be built for x64) |

## Language Support

| Language |
|----------|
| C++14 (MSVC v143, Windows SDK 10.0+) |
| C# (.NET Framework 4.8.1) |
| Python 3.11.7 |

## Quick Starter Examples

Here’s how to quickly connect, send a command, and receive data using each supported language.

### C++ (with MyDLLWrapper)
```cpp
MyDLLWrapper api;
api.LoadDLL("KLib3.dll");
api.Open();
char16_t data[] = u"page=1,savepath=C:\\snap,memo=test";
api.SendCommand(4, data);  // 4 = Snapshot
std::string result = api.GetReceiveStackCommand();
api.Close();
```

### C# (.NET Framework 4.8.1)
```csharp
KLib3Csharp api = new KLib3Csharp();
api.Open();
string sendData = "page=1,savepath=C:\\snap,memo=test";
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
api.SendCommandByStr("Snapshot", "page=1,savepath=...,memo=Test")
api.GetReceiveStackCommand()
api.ApiClose()
```

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

## Documentation Links

- [KLib Manual (Korean)](https://github.com/kitronyx/KLib3/blob/main/doc/240105_%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%ED%86%B5%EC%8B%A0%20%EB%A7%A4%EB%89%B4%EC%96%BC.pdf)
- [KLib Command List & Examples (Korean)](https://github.com/kitronyx/KLib3/blob/main/doc/%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%EC%A7%80%EB%A0%B9%20%EB%AA%A9%EB%A1%9D%20%EB%B0%8F%20%EC%98%88%EC%8B%9C%20v2.7.1%20Retail.pdf)

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

Each command in KLib3 can be sent either by its **string name** or by a **numeric code**.  
The numeric code represents the actual operation ID internally used by ForceLAB2.

### Send by Command Code (Python Example)
```python
api.SendCommandByCode(4, b"page=1,savepath=C:\\snap,memo=test")  # 4 = Snapshot
```

### Common Command Code Mapping

| Command | Code |
|---------|------|
| `CalImport` | 0 |
| `CalRelease` | 1 |
| `LogStart` | 2 |
| `LogEnd` | 3 |
| `Snapshot` | 4 |
| `Decision` | 5 |
| `DecisionResultSave` | 8 |
| `RealtimeDataOn` | 9 |
| `MatrixData` | 1048576 |
| `RtaStart` | 15728643 |

Use these codes when using `SendCommandByCode()` functions.

---

## Static vs Dynamic DLL Usage by Language

| Language | Static DLL Load | Dynamic DLL Load |
|----------|------------------|------------------|
| C++ | Use `#pragma comment(lib, ...)` and link during build | Use `LoadLibrary()` and `GetProcAddress()` for runtime loading |
| C# | Use `[DllImport("KLib3.dll")]` (P/Invoke) | N/A (C# always uses static-like load via P/Invoke) |
| Python | N/A | Use `ctypes.CDLL("KLib3.dll")` or `windll.LoadLibrary()` |

- **Static Load**: DLL linked at compile/startup time (e.g., P/Invoke in C#)
- **Dynamic Load**: DLL loaded at runtime (e.g., `LoadLibrary` in C++ or `ctypes` in Python)

Note: In all cases, `KLib3.dll` must be 64-bit and match the application's architecture.

---

## Sending Commands by Code: Examples

KLib3 supports sending commands by either their **string name** or **numeric code**.  
When using numeric codes, you use `SendCommandByCode()` with the corresponding command number.

### Example: Send `Snapshot` (Code 4) with parameters

#### C++ (using wrapper)
```cpp
char16_t data[] = u"page=1,savepath=C:\\snap,memo=test";
dllWrapper.SendCommand(4, data);  // 4 = Snapshot
```

#### C# (.NET)
```csharp
string sendData = "page=1,savepath=C:\\snap,memo=test";
api.SendCommand(4, sendData, sendData.Length);
```

#### Python
```python
api.SendCommandByCode(4, b"page=1,savepath=C:\\snap,memo=test")
```

Use the command code mappings in the earlier table to reference supported commands.

---

# KLib3 전체 API 레퍼런스 및 퀵 스타터 가이드

안내:  
이 문서는 KLib3 API를 활용할 때 사용하는 공식 래퍼 클래스를 전제로 작성되었습니다.

- C++에서는 `MyDLLWrapper` 클래스를 사용합니다.
- C#에서는 `DllImport` 기반의 `KLib3Csharp` 클래스를 사용합니다.
- Python에서는 `ApiCppDllWrapper.py`에 정의된 `CDllWrapper` 클래스를 사용합니다.

이 래퍼 클래스는 `KLib3.dll`의 네이티브 함수를 올바르게 호출하는 데 필요합니다.

## 개발 환경

| 항목 | 설명 |
|------|------|
| 운영체제 | Windows 10 이상 (64비트 필수) |
| 개발 도구 | Visual Studio 2022 (MSVC v143, Windows SDK 10.0+) / Python 3.11.7 |
| 프레임워크 | .NET Framework 4.8.1 (C#용) |
| 필수 DLL | `KLib3.dll` (64비트 전용 — 모든 프로젝트는 x64로 빌드해야 함) |

## 지원 언어

| 언어 |
|------|
| C++14 (MSVC v143, Windows SDK 10.0+) |
| C# (.NET Framework 4.8.1) |
| Python 3.11.7 |

## 퀵 스타터 예제

지원되는 언어에서 연결, 명령어 전송, 수신을 빠르게 테스트하는 예제입니다.

### C++ (MyDLLWrapper 사용)
```cpp
MyDLLWrapper api;
api.LoadDLL("KLib3.dll");
api.Open();
char16_t data[] = u"page=1,savepath=C:\\snap,memo=test";
api.SendCommand(4, data);  // 4 = Snapshot
std::string result = api.GetReceiveStackCommand();
api.Close();
```

### C# (.NET Framework 4.8.1)
```csharp
KLib3Csharp api = new KLib3Csharp();
api.Open();
string sendData = "page=1,savepath=C:\\snap,memo=test";
api.SendCommand(4, sendData, sendData.Length);
api.GetSearchStackReceiveCommand("Snapshot", out string result);
api.Close();
```

### Python (ApiCppDllWrapper.py 사용)
```python
from ApiCppDllWrapper import CDllWrapper

api = CDllWrapper()
api.ApiOpen()
api.SendCommandByStr("RtaStart", "page=1")
api.SendCommandByStr("Snapshot", "page=1,savepath=...,memo=Test")
api.GetReceiveStackCommand()
api.ApiClose()
```

## 사용 가능한 명령어

| 명령어 | 코드 | 설명 |
|--------|------|------|
| `CalImport` | 0 | 캘리브레이션 파일 로드 |
| `CalRelease` | 1 | 캘리브레이션 파일 언로드 |
| `LogStart` / `LogEnd` | 2 / 3 | 로그 시작 / 종료 |
| `Snapshot` | 4 | 스냅샷 캡처 |
| `Decision` / `DecisionResultSave` | 5 / 8 | 양불 판정 실행 / 저장 |
| `RealtimeDataOn / Off` | 9 / 10 | 실시간 데이터 전송 ON/OFF |
| `MatrixData` | 1048576 | 단일 프레임 수신 |
| `Sensitivity` / `GetSensitivity` | 1048579 / 1048580 | 민감도 설정 / 조회 |
| `DeviceSensorInfo` | 2097152 | 디바이스 및 센서 정보 조회 |
| `RtaStart / RtaEnd` | 15728643 / 15728645 | 실시간 분석기 시작 / 종료 |
| `Version` | 15728640 | ForceLAB2의 버전 요청 |
| `Update` | 15728641 | 명령어 목록 최신화 및 data.dat 저장 요청 |

## 공식 문서 링크

- [KLib 매뉴얼 (한글)](https://github.com/kitronyx/KLib3/blob/main/doc/240105_%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%ED%86%B5%EC%8B%A0%20%EB%A7%A4%EB%89%B4%EC%96%BC.pdf)
- [KLib 명령어 목록 및 예시 (한글)](https://github.com/kitronyx/KLib3/blob/main/doc/%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%EC%A7%80%EB%A0%B9%20%EB%AA%A9%EB%A1%9D%20%EB%B0%8F%20%EC%98%88%EC%8B%9C%20v2.7.1%20Retail.pdf)

## 명령어 인자 설명

| 인자 | 설명 |
|------|------|
| `page` | ForceLAB2의 RTA 페이지 설정. 각 페이지마다 센서 배치가 달라질 수 있음 |
| `savepath` | 로그, 스냅샷, 판정 결과 저장 디렉토리 |
| `deviceindex` | 연결된 디바이스의 순서 번호 |
| `sensorindex` | 각 디바이스에 연결된 센서 순서 번호 |
| `value` (민감도) | 각 센서에 곱해질 민감도 배율 |
| `path` (CalPath) | 캘리브레이션 파일이 위치한 경로 |
| `memo` | 로그, 스냅샷, 판정 결과에 첨부되는 메모 텍스트 |

---

## 명령어 코드 사용 방법

KLib3의 모든 명령은 문자열 또는 정수 코드로 전송할 수 있습니다.  
정수 코드는 내부 명령 ID이며 `SendCommandByCode()`로 전송합니다.

### Python 예시
```python
api.SendCommandByCode(4, b"page=1,savepath=C:\\snap,memo=test")  # 4 = Snapshot
```

---

## 언어별 DLL 로딩 방식 (정적 vs 동적)

| 언어 | 정적 로딩 | 동적 로딩 |
|------|------------|-----------|
| C++ | `#pragma comment(lib, ...)`, 링커 설정 | `LoadLibrary()` 및 `GetProcAddress()` |
| C# | `[DllImport("KLib3.dll")]` 사용 (P/Invoke) | 해당 없음 |
| Python | 해당 없음 | `ctypes.CDLL("KLib3.dll")` 또는 `windll.LoadLibrary()` 사용 |

> 모든 DLL은 반드시 64비트여야 하며, 실행 파일과 아키텍처가 일치해야 합니다.

---

## 명령어 코드 송신 예시

각 언어에서 명령어 코드를 이용한 명령 송신 예시입니다.

### C++
```cpp
char16_t data[] = u"page=1,savepath=C:\\snap,memo=test";
dllWrapper.SendCommand(4, data);  // 4 = Snapshot
```

### C#
```csharp
string sendData = "page=1,savepath=C:\\snap,memo=test";
api.SendCommand(4, sendData, sendData.Length);
```

### Python
```python
api.SendCommandByCode(4, b"page=1,savepath=C:\\snap,memo=test")
```