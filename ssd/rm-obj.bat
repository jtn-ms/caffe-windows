del /Q /S *.aps
del /Q /S *.sdf
del /Q /S *.suo
del /Q /S *.pdb
del /Q /S *.ilk
del /Q /S *.exp

rmdir /Q /S windows\ipch
rmdir /Q /S windows\mfcdemo\x64
rmdir /Q /S VAEngine\Win32
rmdir /Q /S VAEngine\GeneratedFiles

pause
