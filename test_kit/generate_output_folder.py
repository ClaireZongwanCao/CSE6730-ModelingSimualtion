#!/usr/bin/python
import sys,os

if (len(sys.argv)!=2):
	print "Usage: ", sys.argv[0], " <input_folder_name>";
	exit();
	
input_list = os.listdir(os.path.join(os.getcwd(), sys.argv[1]));
#print input_list;

out_dir = sys.argv[1] + "_out"
if (not os.path.isdir(out_dir)):
	os.mkdir(out_dir);

pre_command = "./simulation " +sys.argv[1] + "/";


for i in range(len(input_list)):
	command = pre_command + input_list[i] + " > " + out_dir + "/" + input_list[i][:-4] + "_out.txt";
	os.system(command);