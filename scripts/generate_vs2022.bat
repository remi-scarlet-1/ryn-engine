@echo off
cd %~dp0
".\..\vendors\premake\premake5.exe" --file=".\premake5.lua" vs2022
pause