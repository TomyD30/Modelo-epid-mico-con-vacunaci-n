@echo off

:: aca no se ponen espacios entre = y los numeros y variables
set betaMin=0.1
set betaMax=1.0
set betaSaltos=0.1

:Menu Red
color b
echo Creacion de la red:
echo 1. Elegir configuracion de la red
echo 2. Usar configuracion predeterminada
set /p var=
if %var%==1 goto :PrimeroR
if %var%==2 goto :SegundoR
if %var% GTR 2 cls
echo Error
goto :Menu Red

:PrimeroR
cls
echo Nodos de la red:
set /p n=
echo Valor medio conexiones:
set /p vm=
echo Valor minimo conexiones:
set /p min=
echo Valor maximo conexiones:
set /p max=
pause
goto :Menu Infeccion

:SegundoR
cls
color c
set n=1000000
echo Nodos de la red: %n%
set vm=5
echo Valor medio conexiones: %vm%
set min=1
echo Valor minimo conexiones: %min%
set max=30
echo Valor maximo conexiones: %max%
pause
goto :Menu Infeccion

:Menu Infeccion
cls
color b
echo Configurar la infeccion:
echo 1. Elegir configuracion de la infeccion
echo 2. Usar configuracion predeterminada
echo 3. Varias infecciones
set /p var=
if %var%==1 goto :PrimeroI
if %var%==2 goto :SegundoI
if %var%==3 goto :TerceroI
if %var% GTR 3 cls
echo Error
goto :Menu Infeccon

:PrimeroI
cls
echo Probabilidad de infectarse:
set /p beta=
echo Tiempo de recuperacion:
set /p t_r=
echo Probabilidad de vacunarse:
set /p vacProb=
pause
goto :Programa

:SegundoI
cls
color c
set beta=0.3
echo Probabilidad de infectarse: %beta%
set t_r=6
echo Tiempo de recuperacion: %t_r%
set vacProb=0.5
echo Probabilidad de vacunarse: %vacProb%
pause
goto :Programa

:TerceroI
cls
color c
echo Probabilidad de infectarse minima:
set /p betaMin=
echo Probabilidad de infectarse maxima:
set /p betaMax=
echo Con saltos de:
set /p betaSaltos=
echo Redes totales:
set /p redesTotales=
pause
goto :Programa

:Programa 

cls
color f

if not exist datos mkdir datos
if not exist tiempo mkdir tiempo

setlocal EnableDelayedExpansion

set nombreArchivo=datos/%beta%_%t_r%_%vacProb%.txt
set archivoTiempo=tiempo/%beta%_%t_r%_%vacProb%.txt
call "bin/Debug/RedesComplejas.exe" %n% %vm% %min% %max% %beta% %t_r% %vacProb% !nombreArchivo! !archivoTiempo!

pause