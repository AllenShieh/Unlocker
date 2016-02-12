#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

float data[10][100];
// Ax Ay Az Lx Ly Lz Gx Gy Gz P
float code[10][10];

void getdata(char * name){
	cout << endl;
	for (int x = 0; x < 10; x++){
		for (int y = 0; y < 100; y++){
			data[x][y] = 0;
		}
	}
	ifstream fin(name);
	int i = 0;
	while (fin >> data[0][i]){
		fin >> data[1][i];
		fin >> data[2][i];
		fin >> data[3][i];
		fin >> data[4][i];
		fin >> data[5][i];
		fin >> data[6][i];
		fin >> data[7][i];
		fin >> data[8][i];
		fin >> data[9][i];
		i++;
	}
	cout << name << " ";
}

void analyzedata(int which, int maxcount, int mincount){
	//cout << which << " begin ";
	int codecount = 0;
	int start = 0;
	int end = 99;
	while (data[which][start] == 0) start++;
	while (data[which][end] == 0) end--;
	//cout << endl << start << endl << end << endl;
	int* max_time = (int*)malloc(sizeof(int)*maxcount);
	float* max_value = (float*)malloc(sizeof(float)*maxcount);
	int* min_time = (int*)malloc(sizeof(int)*mincount);
	float* min_value = (float*)malloc(sizeof(float)*mincount);
	for (int i = 0; i < maxcount; i++){
		max_time[i] = 0;
		max_value[i] = -20;
	}
	for (int i = 0; i < mincount; i++){
		min_time[i] = 0;
		min_value[i] = 20;
	}
	bool record_max = false;
	bool record_min = false;
	for (int i = start+1; i <= end; i++){
		if (data[which][i] > data[which][i - 1]) record_max = true;
		if (data[which][i] < data[which][i - 1]) record_min = true;
		if (data[which][i] < data[which][i - 1] && record_max){
			record_max = false;
			int pos = 0;
			for (int j = 0; j < maxcount; j++){
				if (max_value[j] < max_value[pos]) pos = j;
			}
			if (data[which][i - 1] > max_value[pos]){
				max_value[pos] = data[which][i - 1];
				max_time[pos] = i - 1;
			}
		}
		if (data[which][i] > data[which][i - 1] && record_min){
			record_min = false;
			int pos = 0;
			for (int j = 0; j < mincount; j++){
				if (min_value[j]>min_value[pos]) pos = j;
			}
			if (data[which][i - 1] < min_value[pos]){
				min_value[pos] = data[which][i - 1];
				min_time[pos] = i - 1;
			}
		}
	}

	for (int i = 0; i < maxcount; i++){
		cout << setw(2) << max_time[i] - start << " ";
	}
	for (int i = 0; i < mincount; i++){
		cout << setw(2) << min_time[i] - start << " ";
	}
	cout << "/ ";
}

void analyzefromto(int a, int b){
	for (int i = a; i <= b; i++){
		analyzedata(i, 1, 1);
	}
}

int main(){
	getdata("sensor_data1.txt");
	analyzefromto(0, 8);
	getdata("sensor_data2.txt");
	analyzefromto(0, 8);
	getdata("sensor_data3.txt");
	analyzefromto(0, 8);
	getdata("sensor_data4.txt");
	analyzefromto(0, 8);
	getdata("sensor_data5.txt");
	analyzefromto(0, 8);
	getdata("sensor_data6.txt");
	analyzefromto(0, 8);
	getdata("sensor_data7.txt");
	analyzefromto(0, 8);
	getdata("sensor_data8.txt");
	analyzefromto(0, 8);
	getdata("sensor_data9.txt");
	analyzefromto(0, 8);

	cout << endl;
	system("pause");
}
