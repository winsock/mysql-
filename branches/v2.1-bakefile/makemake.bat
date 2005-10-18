@echo off

if "%1" == "vc" goto build_makefiles
if "%1" == "mingw" goto build_makefiles
echo Unrecognized build system %1.
echo.
goto usage

:build_makefiles
rem Create Makefiles in subdirectories
for %%d in ( lib examples ) do call buildmf.bat %1 %%d

rem Create top-level Makefile
echo all: > Makefile
echo 	@cmd /c mkbindirs.bat >> Makefile
echo 	cd lib ^&^& $(MAKE) BIN_DIR=debug all example_setup >> Makefile
echo 	cd lib ^&^& $(MAKE) BIN_DIR=release >> Makefile
echo 	cd examples ^&^& $(MAKE) >> Makefile
echo. >> Makefile
echo install: all >> Makefile
echo 	@cmd /c install.bat %1 >> Makefile
echo. >> Makefile
echo clean debug release: >> Makefile
echo 	cd lib ^&^& $(MAKE) $@ >> Makefile
echo 	cd examples ^&^& $(MAKE) $@ >> Makefile


echo Created Makefiles successfully.
goto end


rem Display usage message
:usage
echo usage: makemake {vc, mingw}
echo.
echo     You must give one of the compiler parameters:
echo.
echo         vc: Visual C++ command-line compiler (cl)
echo         mingw: MinGW GCC (g++)
goto end


:end
