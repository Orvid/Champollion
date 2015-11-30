SETLOCAL
SET SKYRIM=E:\Games\JustCause\SteamApps\common\Skyrim

SET IN=%1
if [%1]==[] SET IN=decompiled -a

"%SKYRIM%\Papyrus compiler\PapyrusCompiler.exe" %IN% -f="%SKYRIM%\Data\Scripts\Source\TESV_Papyrus_Flags.flg" -i="decompiled" -o="Output" -keepasm

ENDLOCAL