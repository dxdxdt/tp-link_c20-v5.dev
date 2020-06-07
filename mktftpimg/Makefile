all: mktftpimg.debug mktftpimg mktftpimg.debug.exe mktftpimg.exe

mktftpimg.debug: mktftpimg.c
	gcc -std=c11 -Wall -Wextra -O0 -g mktftpimg.c -o mktftpimg.debug

mktftpimg: mktftpimg.c
	gcc -std=c11 -Wall -Wextra -O2 -g mktftpimg.c -o mktftpimg

mktftpimg.debug.exe: mktftpimg.c
	i686-w64-mingw32-gcc -std=c11 -Wall -Wextra -O0 -g mktftpimg.c -o mktftpimg.debug.exe

mktftpimg.exe: mktftpimg.c
	i686-w64-mingw32-gcc -std=c11 -Wall -Wextra -O2 -g mktftpimg.c -o mktftpimg.exe

clean:
	rm -f mktftpimg.debug mktftpimg mktftpimg.debug.exe mktftpimg.exe
