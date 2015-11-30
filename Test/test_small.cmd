
SETLOCAL
SET SKYRIM=E:\Games\JustCause\SteamApps\common\Skyrim
SET BSAOUT=I:\Patches\Skyrim\BSAopt-247-1-6-3\out

DEL /Q Output\*.pex
DEL /Q *.psc
DEL /Q *.pex
DEL /Q *.pas
DEL /S *.pasm

ECHO "Failed recompilation" > failed.txt
IF [%1] == [] GOTO all
CALL :test %1
GOTO end

:all
FOR %%f IN (
	ArenaScript
	GlobalVariable
	AltarofMolagBalScript
	barreddoor
	bwclightdamagescript
	AbTGTrapsightScript
	BladeTrap
	CompanionsBladeFragmentTracking
	BFBdragonFlyoverSCRIPT
	ASX_Potions_drinkale01
	qf_mq101_0003372b
	ArenaExitDoorScript
	ActivateAgainAfterDelay
	cwarrowvolleyparentscript
) DO (
	call :test %%f
)
goto end


:test 
COPY %BSAOUT%\Scripts\%1.pex .
Champollion.exe -p decompiled -a asm %BSAOUT%\scripts\%1.pex 
"%SKYRIM%\Papyrus compiler\PapyrusCompiler.exe" decompiled\%1.psc -f="TESV_Papyrus_Flags.flg" -i="%SKYRIM%\Data\Scripts\Source" -o="Output"
IF NOT EXIST Output\%1.pex ECHO %1 >> failed.txt
goto :eof
:end
ENDLOCAL
