@echo off
cd %~dp0
".\..\vendors\doxygen\doxygen.exe" .\Doxyfile
pause