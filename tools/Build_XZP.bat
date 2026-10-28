@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Build-Skin.ps1" -OutputFile "%~dp0..\Snes360.xzp"
exit /b %errorlevel%
