#define version_major = 0
#define version_minor = 37

; ToDo
; - Maybe create custom page for the format selection. That for example allows using two columns to use space more effectively.

[Setup]
AllowCancelDuringInstall=no
AllowNoIcons=yes
AllowUNCPath=no
AppId=vcextras
AppName=VCExtras
AppVerName=VCExtras
AppVersion={#=version_major}.{#=version_minor}
Compression=lzma/ultra
CreateAppDir=yes
DefaultDirName={pf}\VCExtras
DefaultGroupName=VCExtras
DisableStartupPrompt=yes
MinVersion=0,5.01SP2
OutputBaseFilename=VCExtras-{#=version_major}.{#=version_minor}
OutputDir=.
PrivilegesRequired=admin
SolidCompression=yes
Uninstallable=yes
VersionInfoVersion={#=version_major}.{#=version_minor}.0.0

[Messages]
WelcomeLabel1=[name/ver]
WelcomeLabel2=This will install [name] on your computer.%n%nIt is recommended that you close all other applications before continuing.
WinVersionTooLowError=This software only works on Windows XP SP2 and newer.

[Types]
Name: Normal; Description: Normal; Flags: iscustom

[Components]
Name: lavsplitter32; Description: VCExtras Splitter (x86); Types: Normal;
Name: lavsplitter64; Description: VCExtras Splitter (x64); Types: Normal; Check: IsWin64;
Name: lavaudio32;    Description: VCExtras Audio (x86);    Types: Normal;
Name: lavaudio64;    Description: VCExtras Audio (x64);    Types: Normal; Check: IsWin64;
Name: lavvideo32;    Description: VCExtras Video (x86);    Types: Normal;
Name: lavvideo64;    Description: VCExtras Video (x64);    Types: Normal; Check: IsWin64;

[Tasks]
Name: lavs32_mkv;    Description: Matroska; GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_lxf;    Description: LXF;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_gxf;    Description: GXF;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32

Name: lavs64_mkv;    Description: Matroska; GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_lxf;    Description: LXF;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_gxf;    Description: GXF;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64

[Files]
Source: bin_Win32\avcodec-vclav-53.dll;  DestDir: {app}\x86; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter32 lavaudio32 lavvideo32
Source: bin_Win32\avformat-vclav-53.dll; DestDir: {app}\x86; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter32 lavaudio32
Source: bin_Win32\avutil-vclav-51.dll;   DestDir: {app}\x86; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter32 lavaudio32 lavvideo32
Source: bin_Win32\swscale-vclav-2.dll;   DestDir: {app}\x86; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavvideo32
Source: bin_Win32\libbluray.dll;       DestDir: {app}\x86; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter32
Source: bin_Win32\LAVAudio.ax;         DestDir: {app}\x86; Flags: regserver ignoreversion restartreplace uninsrestartdelete; Components: lavaudio32
Source: bin_Win32\LAVSplitter.ax;      DestDir: {app}\x86; Flags: regserver ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter32
Source: bin_Win32\LAVVideo.ax;         DestDir: {app}\x86; Flags: regserver ignoreversion restartreplace uninsrestartdelete; Components: lavvideo32

Source: bin_x64\avcodec-vclav-53.dll;    DestDir: {app}\x64; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter64 lavaudio64 lavvideo64
Source: bin_x64\avformat-vclav-53.dll;   DestDir: {app}\x64; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter64 lavaudio64
Source: bin_x64\avutil-vclav-51.dll;     DestDir: {app}\x64; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter64 lavaudio64 lavvideo64
Source: bin_x64\swscale-vclav-2.dll;     DestDir: {app}\x64; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavvideo64
Source: bin_x64\libbluray.dll;         DestDir: {app}\x64; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter64
Source: bin_x64\LAVAudio.ax;           DestDir: {app}\x64; Flags: regserver ignoreversion restartreplace uninsrestartdelete; Components: lavaudio64
Source: bin_x64\LAVSplitter.ax;        DestDir: {app}\x64; Flags: regserver ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter64
Source: bin_x64\LAVVideo.ax;           DestDir: {app}\x64; Flags: regserver ignoreversion restartreplace uninsrestartdelete; Components: lavvideo64

Source: COPYING;                       DestDir: {app};     DestName: LICENSE.txt; Flags: ignoreversion restartreplace uninsrestartdelete; Components: lavsplitter64 lavsplitter32
Source: README.txt;                    DestDir: {app};     Flags: ignoreversion restartreplace uninsrestartdelete
Source: VCREADME.txt;                  DestDir: {app};     Flags: ignoreversion restartreplace uninsrestartdelete
Source: CHANGELOG.txt;                 DestDir: {app};     Flags: ignoreversion restartreplace uninsrestartdelete

[Icons]
Name: {group}\Uninstall VCExtras;             Filename: {uninstallexe};
Name: {group}\README;                         Filename: {app}\VCREADME.txt;
Name: {group}\LICENSE;                        Filename: {app}\LICENSE.txt;

[Registry]
Root: HKCU; Subkey: Software\VCExtras;                  Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: Software\VCExtras\Audio;            Flags: uninsdeletekey; Components: lavaudio32 lavaudio64
Root: HKCU; Subkey: Software\VCExtras\Splitter;         Flags: uninsdeletekey; Components: lavsplitter32 lavsplitter64
Root: HKCU; Subkey: Software\VCExtras\Splitter\Formats; Flags: uninsdeletekey; Components: lavsplitter32 lavsplitter64
Root: HKCU; Subkey: Software\VCExtras\Video;            Flags: uninsdeletekey; Components: lavvideo32 lavvideo64
Root: HKCU; Subkey: Software\VCExtras\Video\Formats;    Flags: uninsdeletekey; Components: lavvideo32 lavvideo64
Root: HKCU; Subkey: Software\VCExtras\Video\Output;     Flags: uninsdeletekey; Components: lavvideo32 lavvideo64

[Code]
procedure CleanMediaTypeExt(rootkey: Integer; extension, clsid: String);
var
  temp: String;
begin
  if RegQueryStringValue(rootkey, 'Media Type\Extensions\' + extension, 'Source Filter', temp) then begin
    if Lowercase(clsid) = Lowercase(temp) then begin
      RegDeleteValue(rootkey, 'Media Type\Extensions\' + extension, 'Source Filter');
      RegDeleteKeyIfEmpty(rootkey, 'Media Type\Extensions\' + extension);
    end;
  end;
end;

procedure ConfigureFormat(rootkey: Integer; format: String; value: Boolean);
begin
  RegWriteDWordValue(rootkey, 'Software\VCExtras\Splitter\Formats', format, ord(value));
end;

procedure ConfigureVideoFormat(format: String);
begin
  RegWriteDWordValue(HKCU, 'Software\VCExtras\Video\Formats', format, ord(IsTaskSelected('lavv_' + format)));
end;

procedure CleanMediaTypeExt32(extension: String);
begin
  CleanMediaTypeExt(HKCR32, extension, '{24B0A1DA-9F7A-40AD-871B-FFE999BA23A3}');
end;

procedure CleanMediaTypeExt64(extension: String);
begin
  CleanMediaTypeExt(HKCR64, extension, '{24B0A1DA-9F7A-40AD-871B-FFE999BA23A3}');
end;

procedure SetMediaTypeExt32(extension: String);
begin
  RegWriteStringValue(HKCR32, 'Media Type\Extensions\' + extension, 'Source Filter', '{24B0A1DA-9F7A-40AD-871B-FFE999BA23A3}');
end;

procedure SetMediaTypeExt64(extension: String);
begin
  RegWriteStringValue(HKCR64, 'Media Type\Extensions\' + extension, 'Source Filter', '{24B0A1DA-9F7A-40AD-871B-FFE999BA23A3}');
end;

procedure DoExtension32(extension, option: String);
begin
  if IsTaskSelected(option) then begin
    SetMediaTypeExt32(extension);
  end
  else begin
    CleanMediaTypeExt32(extension);
  end;
end;

procedure DoExtension64(extension, option: String);
begin
  if IsTaskSelected(option) then begin
    SetMediaTypeExt64(extension);
  end
  else begin
    CleanMediaTypeExt64(extension);
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep = ssPostInstall) then begin
    // Disable unwanted formats
    if IsComponentSelected('lavsplitter32') then begin
      ConfigureFormat(HKCU32, 'mkv', IsTaskSelected('lavs32_mkv'));
      DoExtension32('.mkv', 'lavs32_mkv');
      DoExtension32('.mka', 'lavs32_mkv');
      ConfigureFormat(HKCU32, 'lxf', IsTaskSelected('lavs32_lxf'));
      DoExtension32('.lxf', 'lavs32_lxf');
      ConfigureFormat(HKCU32, 'gxf', IsTaskSelected('lavs32_gxf'));
      DoExtension32('.gxf', 'lavs32_gxf');
    end;
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if (CurUninstallStep = usUninstall) then begin
    CleanMediaTypeExt32('.mkv');
    CleanMediaTypeExt32('.mka');
    CleanMediaTypeExt32('.gxf');
    CleanMediaTypeExt32('.lxf');

    if IsWin64 then begin
        CleanMediaTypeExt64('.mkv');
        CleanMediaTypeExt64('.mka');
        CleanMediaTypeExt64('.gxf');
        CleanMediaTypeExt64('.lxf');
    end;
  end;
end;

procedure InitializeWizard();
begin
  // Adjust tasks page
	WizardForm.SelectTasksLabel.Hide;
	WizardForm.TasksList.Top    := 0;
	WizardForm.TasksList.Height := PageFromID(wpSelectTasks).SurfaceHeight;
end;
