#define MyAppName                    "DSI3PAS"
#define MyAppVersion                 "00.0000 Fri Jan 27 09:00:35 2023"
#define MyAppPublisher               "onsemi"
#define MyAppURL                     "http://www.onsemi.com/"
#define MyAppExeName                 "dsi3pas.exe"
#define CompanyName                  "onsemi"
#define CompanyFolder                "onsemi"
#define CompanyCommonFolder          "Common"
#define CompanyDocumentationFolder   "Documentation"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=c:\Users\Public\{#MyAppName}
DefaultGroupName={#CompanyFolder}\{#MyAppName}
OutputBaseFilename=DSI3PAS_setup
OutputDir=..\Install
SetupIconFile=bin\onlogo.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\vcredist_x64.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\IDC_setup.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\dsi3pas.exe"; DestDir: "{app}"; Flags: ignoreversion



[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\IDC_setup" ; Parameters: IDC_setup.exe /passive /norestart ;  StatusMsg: Install IDC tool...
Filename: "{app}\vcredist_x86" ; Parameters: vcredist_x86.exe /passive /norestart ;  StatusMsg: Install VS2010 vcredist_x86 redistributable package...
Filename: "{app}\vcredist_x64" ; Parameters: vcredist_x64.exe /passive /norestart ;  StatusMsg: Install VS2010 vcredist_x64 redistributable package...
Filename: "{pf32}\IDC Tool\idc.exe" ; Parameters: {#MyAppExeName} /regserver;  StatusMsg: Registering ActiveX components...
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent