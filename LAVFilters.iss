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
Name: lavs32_avi;    Description: AVI;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_bluray; Description: Blu-ray;  GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_mkv;    Description: Matroska; GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_lxf;    Description: LXF;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_gxf;    Description: GXF;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_mp4;    Description: MP4;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_ogg;    Description: Ogg;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_flv;    Description: FLV;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_ts;     Description: MPEG-TS;  GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_ps;     Description: MPEG-PS;  GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_rm;     Description: RealMedia;GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32
Name: lavs32_wtv;    Description: WTV;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_wmv;    Description: WMV;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_flac;   Description: FLAC;     GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_aac;    Description: AAC;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_amr;    Description: AMR;      GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_wv;     Description: WavPack;  GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_mpc;    Description: Musepack; GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;
Name: lavs32_tta;    Description: TrueAudio;GroupDescription: "Use VCExtras (x86) for these file formats:"; Components: lavsplitter32; Flags: unchecked;

Name: lavs64_avi;    Description: AVI;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_bluray; Description: Blu-ray;  GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_mkv;    Description: Matroska; GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_lxf;    Description: LXF;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_gxf;    Description: GXF;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_mp4;    Description: MP4;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_ogg;    Description: Ogg;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_flv;    Description: FLV;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_ts;     Description: MPEG-TS;  GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_ps;     Description: MPEG-PS;  GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_rm;     Description: RealMedia;GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64
Name: lavs64_wtv;    Description: WTV;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_wmv;    Description: WMV;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_flac;   Description: FLAC;     GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_aac;    Description: AAC;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_amr;    Description: AMR;      GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_wv;     Description: WavPack;  GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_mpc;    Description: Musepack; GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;
Name: lavs64_tta;    Description: TrueAudio;GroupDescription: "Use VCExtras (x64) for these file formats:"; Components: lavsplitter64; Flags: unchecked;

Name: lavv_h264;      Description: H.264/AVC1;         GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_vc1;       Description: VC-1;               GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64; Flags: unchecked;
Name: lavv_mpeg1;     Description: MPEG1;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_mpeg2;     Description: MPEG2;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_mpeg4;     Description: MPEG4;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_msmpeg4;   Description: MS-MPEG4;           GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_vp8;       Description: VP8;                GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_wmv3;      Description: WMV3;               GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_wmv12;     Description: WMV1/2;             GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_mjpeg;     Description: M-JPEG;             GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_theora;    Description: Theora;             GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_flv;       Description: Flash Video 1;      GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_vp6;       Description: VP6;                GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_svq;       Description: SVQ 1/3;            GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_h261;      Description: H.261;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_h263;      Description: H.263;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_indeo;     Description: Intel Indea;        GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_camtasia;  Description: TechSmith/Camtasia; GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_fraps;     Description: Fraps;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_huffyuv;   Description: HuffYUV;            GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_qtrle;     Description: QTRle;              GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_dvvideo;   Description: DV;                 GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_bink;      Description: Bink;               GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_smackvid;  Description: Smacker;            GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_rv12;      Description: Real Video 1/2;     GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64; Flags: unchecked;
Name: lavv_rv34;      Description: Real Video 3/4;     GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_lagarith;  Description: Lagarith;           GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64; Flags: unchecked;
Name: lavv_cinepak;   Description: Cinepak;            GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64; Flags: unchecked;
Name: lavv_camstudio; Description: Camstudio;          GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_qpeg;      Description: QPEG;               GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64; Flags: unchecked;
Name: lavv_zlib;      Description: ZLIB;               GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64
Name: lavv_rpza;      Description: QTRpza;             GroupDescription: "Use VCExtras for these codecs:"; Components: lavvideo32 lavvideo64

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
      ConfigureFormat(HKCU32, 'bluray', IsTaskSelected('lavs32_bluray'));
      DoExtension32('.bdmv', 'lavs32_bluray');
      DoExtension32('.mpls', 'lavs32_bluray');
      ConfigureFormat(HKCU32, 'lxf', IsTaskSelected('lavs32_lxf'));
      DoExtension32('.lxf', 'lavs32_lxf');
      ConfigureFormat(HKCU32, 'gxf', IsTaskSelected('lavs32_gxf'));
      DoExtension32('.gxf', 'lavs32_gxf');
      ConfigureFormat(HKCU32, 'mp4', IsTaskSelected('lavs32_mp4'));
      DoExtension32('.mp4', 'lavs32_mp4');
      DoExtension32('.mov', 'lavs32_mp4');
      DoExtension32('.3gp', 'lavs32_mp4');
      ConfigureFormat(HKCU32, 'avi', IsTaskSelected('lavs32_avi'));
      DoExtension32('.avi', 'lavs32_avi');
      DoExtension32('.divx', 'lavs32_avi');
      ConfigureFormat(HKCU32, 'flv', IsTaskSelected('lavs32_flv'));
      DoExtension32('.flv', 'lavs32_flv');
      ConfigureFormat(HKCU32, 'ogg', IsTaskSelected('lavs32_ogg'));
      DoExtension32('.ogg', 'lavs32_ogg');
      DoExtension32('.ogm', 'lavs32_ogg');
      DoExtension32('.ogv', 'lavs32_ogg');
      ConfigureFormat(HKCU32, 'mpeg', IsTaskSelected('lavs32_ps'));
      DoExtension32('.mpg', 'lavs32_ps');
      DoExtension32('.vob', 'lavs32_ps');
      DoExtension32('.evo', 'lavs32_ps');
      ConfigureFormat(HKCU32, 'mpegts', IsTaskSelected('lavs32_ts'));
      DoExtension32('.ts', 'lavs32_ts');
      DoExtension32('.mts', 'lavs32_ts');
      DoExtension32('.m2ts', 'lavs32_ts');
      DoExtension32('.wtv', 'lavs32_wtv');
      DoExtension32('.wmv', 'lavs32_wmv');
      DoExtension32('.rm', 'lavs32_rm');
      DoExtension32('.rmvb', 'lavs32_rm');
      DoExtension32('.flac', 'lavs32_flac');
      DoExtension32('.aac', 'lavs32_aac');
      DoExtension32('.amr', 'lavs32_amr');
      DoExtension32('.wv', 'lavs32_amr');
      DoExtension32('.mpc', 'lavs32_mpc');
      DoExtension32('.tta', 'lavs32_tta');
    end;

    if IsComponentSelected('lavsplitter64') then begin
      ConfigureFormat(HKCU64, 'mkv', IsTaskSelected('lavs64_mkv'));
      DoExtension64('.mkv', 'lavs64_mkv');
      DoExtension64('.mka', 'lavs64_mkv');
      ConfigureFormat(HKCU64, 'bluray', IsTaskSelected('lavs64_bluray'));
      DoExtension64('.bdmv', 'lavs64_bluray');
      DoExtension64('.mpls', 'lavs64_bluray');
      ConfigureFormat(HKCU64, 'lxf', IsTaskSelected('lavs64_lxf'));
      DoExtension64('.lxf', 'lavs64_lxf');
      ConfigureFormat(HKCU64, 'gxf', IsTaskSelected('lavs64_gxf'));
      DoExtension64('.gxf', 'lavs64_gxf');
      ConfigureFormat(HKCU64, 'mp4', IsTaskSelected('lavs64_mp4'));
      DoExtension64('.mp4', 'lavs64_mp4');
      DoExtension64('.mov', 'lavs64_mp4');
      DoExtension64('.3gp', 'lavs64_mp4');
      ConfigureFormat(HKCU64, 'avi', IsTaskSelected('lavs64_avi'));
      DoExtension64('.avi', 'lavs64_avi');
      DoExtension64('.divx', 'lavs64_avi');
      ConfigureFormat(HKCU64, 'flv', IsTaskSelected('lavs64_flv'));
      DoExtension64('.flv', 'lavs64_flv');
      ConfigureFormat(HKCU64, 'ogg', IsTaskSelected('lavs64_ogg'));
      DoExtension64('.ogg', 'lavs64_ogg');
      DoExtension64('.ogm', 'lavs64_ogg');
      DoExtension64('.ogv', 'lavs64_ogg');
      ConfigureFormat(HKCU64, 'mpeg', IsTaskSelected('lavs64_ps'));
      DoExtension64('.mpg', 'lavs64_ps');
      DoExtension64('.vob', 'lavs64_ps');
      DoExtension64('.evo', 'lavs64_ps');
      ConfigureFormat(HKCU64, 'mpegts', IsTaskSelected('lavs64_ts'));
      DoExtension64('.ts', 'lavs64_ts');
      DoExtension64('.mts', 'lavs64_ts');
      DoExtension64('.m2ts', 'lavs64_ts');
      DoExtension64('.wtv', 'lavs64_wtv');
      DoExtension64('.wmv', 'lavs64_wmv');
      DoExtension64('.rm', 'lavs64_rm');
      DoExtension64('.rmvb', 'lavs64_rm');
      DoExtension64('.flac', 'lavs64_flac');
      DoExtension64('.aac', 'lavs64_aac');
      DoExtension64('.amr', 'lavs64_amr');
      DoExtension64('.wv', 'lavs64_amr');
      DoExtension64('.mpc', 'lavs64_mpc');
      DoExtension64('.tta', 'lavs64_tta');
    end;

    if IsComponentSelected('lavvideo32') or IsComponentSelected('lavvideo64') then begin
        ConfigureVideoFormat('h264');
        ConfigureVideoFormat('vc1');
        ConfigureVideoFormat('mpeg1');
        ConfigureVideoFormat('mpeg2');
        ConfigureVideoFormat('mpeg4');
        ConfigureVideoFormat('msmpeg4');
        ConfigureVideoFormat('vp8');
        ConfigureVideoFormat('wmv3');
        ConfigureVideoFormat('wmv12');
        ConfigureVideoFormat('mjpeg');
        ConfigureVideoFormat('theora');
        ConfigureVideoFormat('flv');
        ConfigureVideoFormat('vp6');
        ConfigureVideoFormat('svq');
        ConfigureVideoFormat('h261');
        ConfigureVideoFormat('h263');
        ConfigureVideoFormat('indeo');
        ConfigureVideoFormat('camtasia');
        ConfigureVideoFormat('fraps');
        ConfigureVideoFormat('huffyuv');
        ConfigureVideoFormat('qtrle');
        ConfigureVideoFormat('dvvideo');
        ConfigureVideoFormat('bink');
        ConfigureVideoFormat('smackvid');
        ConfigureVideoFormat('rv12');
        ConfigureVideoFormat('rv34');
        ConfigureVideoFormat('lagarith');
        ConfigureVideoFormat('cinepak');
        ConfigureVideoFormat('camstudio');
        ConfigureVideoFormat('qpeg');
        ConfigureVideoFormat('zlib');
        ConfigureVideoFormat('rpza');
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
    CleanMediaTypeExt32('.bdmv');
    CleanMediaTypeExt32('.mpls');
    CleanMediaTypeExt32('.mp4');
    CleanMediaTypeExt32('.mov');
    CleanMediaTypeExt32('.3gp');
    CleanMediaTypeExt32('.avi');
    CleanMediaTypeExt32('.divx');
    CleanMediaTypeExt32('.flv');
    CleanMediaTypeExt32('.ogg');
    CleanMediaTypeExt32('.ogm');
    CleanMediaTypeExt32('.ogv');
    CleanMediaTypeExt32('.mpg');
    CleanMediaTypeExt32('.vob');
    CleanMediaTypeExt32('.evo');
    CleanMediaTypeExt32('.ts');
    CleanMediaTypeExt32('.mts');
    CleanMediaTypeExt32('.m2ts');
    CleanMediaTypeExt32('.wtv');
    CleanMediaTypeExt32('.wmv');
    CleanMediaTypeExt32('.rm');
    CleanMediaTypeExt32('.rmvb');
    CleanMediaTypeExt32('.flac');
    CleanMediaTypeExt32('.aac');
    CleanMediaTypeExt32('.amr');
    CleanMediaTypeExt32('.wv');
    CleanMediaTypeExt32('.mpc');
    CleanMediaTypeExt32('.tta');

    if IsWin64 then begin
        CleanMediaTypeExt64('.mkv');
        CleanMediaTypeExt64('.mka');
        CleanMediaTypeExt64('.gxf');
        CleanMediaTypeExt64('.lxf');
        CleanMediaTypeExt64('.bdmv');
        CleanMediaTypeExt64('.mpls');
        CleanMediaTypeExt64('.mp4');
        CleanMediaTypeExt64('.mov');
        CleanMediaTypeExt64('.3gp');
        CleanMediaTypeExt64('.avi');
        CleanMediaTypeExt64('.divx');
        CleanMediaTypeExt64('.flv');
        CleanMediaTypeExt64('.ogg');
        CleanMediaTypeExt64('.ogm');
        CleanMediaTypeExt64('.ogv');
        CleanMediaTypeExt64('.mpg');
        CleanMediaTypeExt64('.vob');
        CleanMediaTypeExt64('.evo');
        CleanMediaTypeExt64('.ts');
        CleanMediaTypeExt64('.mts');
        CleanMediaTypeExt64('.m2ts');
        CleanMediaTypeExt64('.wtv');
        CleanMediaTypeExt64('.wmv');
        CleanMediaTypeExt64('.rm');
        CleanMediaTypeExt64('.rmvb');
        CleanMediaTypeExt64('.flac');
        CleanMediaTypeExt64('.aac');
        CleanMediaTypeExt64('.amr');
        CleanMediaTypeExt64('.wv');
        CleanMediaTypeExt64('.mpc');
        CleanMediaTypeExt64('.tta');
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
