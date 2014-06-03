xcopy /Y Z:\Engine2\dependencies\bin\libDRCore2.dll .
xcopy /Y Z:\Engine2\dependencies\bin\libDRCore2.dll import\bin\
xcopy /Y Z:\Engine2\dependencies\bin\libDRCore2.dll ..\thirdparty\bin\

xcopy /Y Z:\Engine2\dependencies\bin\Debug\DRCore2.dll .
xcopy /Y Z:\Engine2\dependencies\bin\Debug\DRCore2.dll import\bin\
xcopy /Y Z:\Engine2\dependencies\bin\Debug\DRCore2.dll ..\thirdparty\bin\

xcopy /Y Z:\Engine2\dependencies\bin\Debug\DRCore2.dll Debug\
xcopy /Y Z:\Engine2\dependencies\bin\Release\DRCore2.dll Release\
xcopy /Y Z:\Engine2\dependencies\bin\Release\DRCore2.dll ..\thirdparty\bin\

xcopy /Y /s Z:\Engine2\dependencies\Core2\include\* import\include\Core2\
xcopy /Y /s Z:\Engine2\dependencies\Core2\include\* ..\thirdparty\include\Core2\
xcopy /Y /s Z:\Engine2\dependencies\lib\Debug\DRCore2.lib import\lib\
xcopy /Y /s Z:\Engine2\dependencies\lib\Debug\DRCore2.lib ..\thirdparty\lib\

