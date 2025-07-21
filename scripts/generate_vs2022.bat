@echo off
cd %~dp0
".\..\vendors\premake\bin\premake5.exe" --file=".\premake5.lua" vs2022
pause