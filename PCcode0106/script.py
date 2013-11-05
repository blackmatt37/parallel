import os

for i in range(2,21):
	print i-1
	os.system("time mpirun -np " + str(i) + " -machinefile hostlist.txt a.out")