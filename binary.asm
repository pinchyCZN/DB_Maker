;nasmw -t -f  win32 -o$(IntDir)\$(InputName).obj -Xvc $(InputName).asm

SECTION .data


global ?line8x8@@3PADA
global ?line8x8@@3PAEA

global ?charset@@3PADA
global ?charset@@3PAEA

?line8x8@@3PADA:
?line8x8@@3PAEA:
?charset@@3PADA:
?charset@@3PAEA:
 incbin "line8x8.bin"
