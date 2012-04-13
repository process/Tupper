@echo off
echo Build Started...

windres -o Temp/res.o Source/resource.rc
gcc Source/main.c -c -o Temp/main.o
gcc -o Build/Tupper.exe Temp/main.o Temp/res.o -lgmp -lopengl32 -mwindows
 
pause