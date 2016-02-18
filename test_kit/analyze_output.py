#!/usr/bin/python
import sys,os

if (len(sys.argv)!=2):
	print "Usage: ", sys.argv[0], " <output_folder_name>";
	exit();
	
output_list = os.listdir(os.path.join(os.getcwd(), sys.argv[1]));

dist=[[0 for x in range(10)] for x in range(10)];
dist[0] = [0,1,2,3,1,2,3,1,2,3];
dist[1] = [1,0,1,2,1,1,2,1,1,2];
dist[2] = [2,1,0,1,2,1,1,2,1,1];
dist[3] = [3,2,1,0,3,2,1,3,2,1];
dist[4] = [1,1,2,3,0,2,3,1,2,3];
dist[5] = [2,1,1,2,2,0,2,2,1,2];
dist[6] = [3,2,1,1,3,2,0,3,2,1];
dist[7] = [1,1,2,3,1,2,3,0,2,3];
dist[8] = [2,1,1,2,2,1,2,2,0,2];
dist[9] = [3,2,1,1,3,2,1,3,2,0];

#print dist;
avg_speed=[];

os.chdir(sys.argv[1]);

for i in range(len(output_list)):
	f=open(output_list[i], 'r');
	data=[];
	while(True):
		line=f.readline();
		if (line==''):
			break;
		arr=line.split("\t");
		if (int(arr[3])>=int(arr[4]) or int(arr[1])==int(arr[2])):
			continue;
		data.append((int(arr[4]) - int(arr[3]))/(dist[int(arr[1])][int(arr[2])]));
	f.close();
	#print len(data);
	
	if (len(data)!=0):
		avg_speed.append(sum(data)/len(data));
	#print data;
	#we have the data for 1 file, do something with it
print avg_speed;