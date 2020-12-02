set /p name=Name: 

del dumped\*.pcx
psxgfx.exe %name%.hdr %name%.dat palette.bin > whmt.txt