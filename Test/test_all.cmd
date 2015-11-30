
SETLOCAL
SET SKYRIM=E:\Games\JustCause\SteamApps\common\Skyrim
SET BSAOUT=I:\Patches\Skyrim\BSAopt-247-1-6-3\out

Champollion.exe -p decompiled -t %BSAOUT%\scripts > output.txt
"%SKYRIM%\Papyrus compiler\PapyrusCompiler.exe" decompiled -a -f="%SKYRIM%\Data\Scripts\Source\TESV_Papyrus_Flags.flg" -i="decompiled" -o="Output" -keepasm > compile.txt

ENDLOCAL
