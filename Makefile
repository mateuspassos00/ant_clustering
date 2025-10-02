default:
	gcc -g -Wall src/*.c

clean_homo:
	del /q "snapshots_homo/"	

clean_4:
	del /q "snapshots_4/"	

clean_15:
	del /q "snapshots_15/"	

clean:
	del /q "snapshots_4/"
	del /q "snapshots_15/"
	del a.exe