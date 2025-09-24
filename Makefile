default:
	gcc -g -Wall src/*.c

clean:
	del a.exe
	del /s /q "snapshots"