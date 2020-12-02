set /p name=Name: 

del dumped\*.pcx
psxgfx.exe %name%.hdr %name%.tp palette.bin > whmt.txt