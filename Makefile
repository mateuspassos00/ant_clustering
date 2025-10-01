default:
	gcc -g -Wall src/*.c

rm_s4:
	del /q "snapshots_4/"

rm_s15:
	del /q "snapshots_15/"

clean:
	del /q "snapshots_4/"
	del /q "snapshots_15/"
	del a.exe