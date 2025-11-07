// Original script written by Byrom90, edited by frankischilling for Snes360-enhanced

@if exist Snes360.xzp del Snes360.xzp

@pushd "%~dp0snes9x-1.51-src-d3d\win32"

@set XUIPKG="%XEDK%\bin\win32\xuipkg.exe" /nologo

@set XUI_XZP="%~dp0Snes360.xzp"

@echo Building Snes360.xzp with relative paths to match original structure

@REM First add files from the source xbox\Skin directory (original location)
@echo Adding files from ..\xbox\Skin\...
%XUIPKG% /nologo /d /a %XUI_XZP% "..\xbox\Skin\*" "..\xbox\Skin\Graphics\*"

@REM Then append files from Xbox\Xbox\Skin to create duplicates with different table offsets
@REM But exclude graphics files (PNG, JPG, etc.) - only duplicate XUR, XUI, and other non-graphics files
@echo Adding non-graphics files from ..\..\Xbox\Xbox\Skin\...

@REM Create a temporary directory for filtered files
@set TEMP_SKIN="%~dp0temp_skin_dup"
@if exist %TEMP_SKIN% rmdir /s /q %TEMP_SKIN%
@mkdir %TEMP_SKIN%

@REM Copy only non-graphics files from Xbox\Xbox\Skin (XUR, XUI, XML, TTF, XMA, WAV, etc.)
@REM Exclude: PNG, JPG, JPEG, GIF, BMP and other image files
@for %%f in ("..\..\Xbox\Xbox\Skin\*.xur") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
@for %%f in ("..\..\Xbox\Xbox\Skin\*.xui") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
@for %%f in ("..\..\Xbox\Xbox\Skin\*.xml") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
@for %%f in ("..\..\Xbox\Xbox\Skin\*.ttf") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
@for %%f in ("..\..\Xbox\Xbox\Skin\*.xma") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
@for %%f in ("..\..\Xbox\Xbox\Skin\*.3xui") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1

@REM Also copy non-graphics files from Graphics subdirectory (WAV files, etc.)
@if exist "..\..\Xbox\Xbox\Skin\Graphics" (
    @for %%f in ("..\..\Xbox\Xbox\Skin\Graphics\*.wav") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
    @for %%f in ("..\..\Xbox\Xbox\Skin\Graphics\*.xur") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
    @for %%f in ("..\..\Xbox\Xbox\Skin\Graphics\*.xui") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
    @for %%f in ("..\..\Xbox\Xbox\Skin\Graphics\*.xml") do @copy /Y "%%f" %TEMP_SKIN%\ >nul 2>&1
)

@REM Add the filtered files to the XZP
%XUIPKG% /nologo /d /a %XUI_XZP% "%TEMP_SKIN%\*"

@REM Clean up temporary directory
@if exist %TEMP_SKIN% rmdir /s /q %TEMP_SKIN%

@popd
@cmd /k
