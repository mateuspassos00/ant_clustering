default:
	gcc -g -Wall src/*.c

rm_s1:
	del /q "snapshots_1/"

rm_s2:
	del /q "snapshots_2/"

rm_s3:
	del /q "snapshots_3/

rm_s4:
	del /q "snapshots_4/"

clean:
	del a.exe