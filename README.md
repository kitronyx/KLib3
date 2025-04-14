# KLib3 Integrated Quick Starter Guide (C++ / C# / Python)

> Note:**Note**  
> This guide assumes that you are using the official wrapper code provided for KLib3:
> 
> - For **C++**, use the `MyDLLWrapper` class implementation.
> - For **C#**, use the `KLib3Csharp` class with `DllImport` bindings.
> - For **Python**, use the `CDllWrapper` class via `ctypes` provided in `ApiCppDllWrapper.py`.
>
> These wrapper classes are required to correctly access native functions in `KLib3.dll`.

---

## Development Environment

| Item | Description |
|------|-------------|
| Operating System | Windows 10 or later (x64 recommended) |
| Development Tools | Visual Studio 2022 (MSVC v143, Windows SDK 10.0+) / Python 3.11.7 |
| Framework | .NET Framework 4.6.1 (for C#) |
| Required DLL | `KLib3.dll` (must match 64-bit application) |

---

## Core Functionalities

| Feature | Description |
|--------|-------------|
| API Client Management | `CreateApiClient`, `DisposeApiClient` |
| Communication | `ApiClient_Open`, `ApiClient_Close` |
| Command Transmission | `SendCommandByCode`, `SendCommandByStr` |
| Response Handling | `GetReceiveStackCommand`, `GetSearchReceiveStackCommand` |
| Command List Retrieval | `GetCommandList` |
| Timeout Configuration | `GetTimeOut`, `SetTimeOut` |

---

## Sample: C++

```cpp
MyDLLWrapper dllWrapper;
dllWrapper.LoadDLL("KLib3.dll");
dllWrapper.Open();
dllWrapper.SendCommand("RtaStart", u"page=1");
dllWrapper.SendCommand("Snapshot", u"page=1,savepath=...,memo=Test");
dllWrapper.Close();
```

---

## Sample: C# (WinForms)

```csharp
var api = new KLib3Csharp();
api.Open();
api.SendCommand("Snapshot", "page=1,savepath=...,memo=Test", data.Length);
api.GetSearchStackReceiveCommand("Snapshot", out var result);
api.Close();
```

---

## Sample: Python

```python
from ApiCppDllWrapper import CDllWrapper

api = CDllWrapper()
api.ApiOpen()
api.SendCommandByStr("RtaStart", "page=1")
api.SendCommandByStr("Snapshot", "page=1,savepath=...,memo=Test")
api.GetReceiveStackCommand()
api.ApiClose()
```

---

## Available Commands

| Command Name | Code | Description |
|--------------|------|-------------|
| `CalImport` | 0 | Load calibration file |
| `CalRelease` | 1 | Unload calibration |
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

## Project Files

| File | Purpose |
|------|---------|
| `ApiCppDllWrapper.py` | Python ctypes wrapper for KLib3 |
| `KLib3_API_Example_Rewritten_WithComments.py` | Main Python example |
| `MyDLLWrapper.cpp/h` | C++ wrapper for DLL interface |
| `MainForm.cs` + `KLib3Csharp.cs` | C# WinForms controller with API usage |

---

## Tips

- Ensure DLL path and architecture match your runtime (Python/C#/C++).
- Always close the API gracefully to prevent memory leaks.
- For testing: use `page=1` or actual page ID used in your sensor setup.

---

© Kitronyx - KLib3 Development SDK Starter

## Command Argument Details

| Command | Required Parameters | Example |
|---------|---------------------|---------|
| `CalImport` | `page`, `deviceindex`, `sensorindex`, `path` | `page=1,deviceindex=1,sensorindex=1,path=C:\cal\file\` |
| `CalRelease` | `page`, `deviceindex`, `sensorindex` | `page=1,deviceindex=1,sensorindex=1` |
| `LogStart` | `page`, `savepath`, `memo` | `page=1,savepath=C:\log\,memo=test` |
| `LogEnd` | `page` | `page=1` |
| `Snapshot` | `page`, `savepath`, `memo` | `page=1,savepath=C:\snap\,memo=example` |
| `Decision` | `page` | `page=1` |
| `DecisionResultSave` | `page`, `savepath`, `memo` | `page=1,savepath=C:\result\,memo=ok` |
| `RealtimeDataOn` / `Off` | `page` | `page=1` |
| `MatrixData` | `page` | `page=1` |
| `Sensitivity` | `deviceindex`, `sensorindex`, `value` | `deviceindex=1,sensorindex=1,value=25` |
| `GetSensitivity` | *(none)* | - |
| `DeviceSensorInfo` | `page` | `page=1` |
| `ConnectionInfo` | `page` | `page=1` |
| `RtaStart` / `RtaEnd` | `page` | `page=1` |
| `AccumulOn` / `Off` | `page` | `page=1` |
| `Version` | *(none)* | - |
| `Update` | *(none)* | - |

## Documentation Links

- [KLib Manual (Korean)](https://github.com/kitronyx/KLib3/blob/main/doc/240105_%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%ED%86%B5%EC%8B%A0%20%EB%A7%A4%EB%89%B4%EC%96%BC.pdf)
- [KLib Command List & Examples (Korean)](https://github.com/kitronyx/KLib3/blob/main/doc/%EC%96%91%EB%B0%A9%ED%96%A5%20API%20%EC%A7%80%EB%A0%B9%20%EB%AA%A9%EB%A1%9D%20%EB%B0%8F%20%EC%98%88%EC%8B%9C%20v2.7.1%20Retail.pdf)
