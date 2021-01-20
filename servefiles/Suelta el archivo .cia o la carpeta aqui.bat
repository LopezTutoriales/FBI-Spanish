@ECHO OFF
set /p DSIP="Introduce la IP de tu Nintendo 3DS: "
for %%a in (%*) do (
	if "%%~xa"==".cia" (
		python "%~dp0servefiles.py" %DSIP% "%%~a"
	)
	if "%%~xa"=="" (
		python "%~dp0servefiles.py" %DSIP% "%%~a"
	)
)
pause
