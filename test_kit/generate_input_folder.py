#!/usr/bin/python
import sys,os, time

if (len(sys.argv)!=3):
	print "Usage: ", sys.argv[0], " <fold_name> <number_of_inputs>";
	exit();
	
if (not os.path.isdir(sys.argv[1])):
	os.mkdir(sys.argv[1]);

pre_command = "./generate_input config.txt " + sys.argv[1] + "/";

count_files = int(sys.argv[2]);

for i in range(count_files):
	os.system(pre_command+str(i)+".txt");
	time.sleep(2);
	print i;