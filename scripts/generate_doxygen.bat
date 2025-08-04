@echo off
cd %~dp0
SET PATH=%~dp0..\vendors\doxygen\Graphviz-13.1.1-win64\bin
".\..\vendors\doxygen\doxygen.exe" .\Doxyfile
pause