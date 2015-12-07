#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#define graph_row 50
#define graph_col 200
#define center_pro 0.5
#define other_pro 0.25
#define offset 25
#define file_num 6

float max_data = -100.0f;
float min_data = 100.0f;
float max_time[5] = { 0 };
float min_time[5] = { 0 };
float last = 0.0f;
int num_of_data = 0;
int num_of_max = 0;
int num_of_min = 0;
float data[200];
float store[200];
char graph[graph_row][graph_col];
int codes[file_num][10] = { 0 };
char fin_namex[file_num][20] = { "sensor_data_Ax1.txt", "sensor_data_Ax2.txt", "sensor_data_Ax3.txt", "sensor_data_Ax4.txt",
								"sensor_data_Ax5.txt", "sensor_data_Ax6.txt" };
char fin_namey[file_num][20] = { "sensor_data_Ay1.txt", "sensor_data_Ay2.txt", "sensor_data_Ay3.txt", "sensor_data_Ay4.txt",
								"sensor_data_Ay5.txt", "sensor_data_Ay6.txt" };
char fin_namez[file_num][20] = { "sensor_data_Az1.txt", "sensor_data_Az2.txt", "sensor_data_Az3.txt", "sensor_data_Az4.txt",
								"sensor_data_Az5.txt", "sensor_data_Az6.txt" };
char fout_name[file_num][20] = { "graph1.txt", "graph2.txt", "graph3.txt", "graph4.txt",
								"graph5.txt", "graph6.txt" };
char store_file[file_num][20] = { "weighted_data1.txt", "weighted_data2.txt", "weighted_data3.txt", "weighted_data4.txt",
								"weighted_data5.txt", "weighted_data6.txt" };
char code_file[file_num][20] = { "code1.txt", "code2.txt", "code3.txt", "code4.txt", "code5.txt", "code6.txt" };

void save_codex(int i){
	codes[i][0] = max_time[0] / (float)num_of_data * 100;
	codes[i][1] = max_time[1] / (float)num_of_data * 100;
	codes[i][2] = min_time[0] / (float)num_of_data * 100;
	codes[i][3] = min_time[1] / (float)num_of_data * 100;
}

void save_codey(int i){
	codes[i][4] = max_time[0] / (float)num_of_data * 100;
	codes[i][5] = min_time[0] / (float)num_of_data * 100;
	codes[i][6] = min_time[1] / (float)num_of_data * 100;
}

void save_codez(int i){
	codes[i][7] = max_time[0] / (float)num_of_data * 100;
	codes[i][8] = max_time[1] / (float)num_of_data * 100;
	codes[i][9] = min_time[0] / (float)num_of_data * 100;
}

void fetch_data(char* x, float high, float low){
	ifstream fin(x);
	max_data = -100.0f;
	min_data = 100.0f;
	for (int i = 0; i<num_of_data; i++) data[i] = 0;
	for (int i = 0; i < 5; i++){
		max_time[i] = 0;
		min_time[i] = 0;
	}
	num_of_data = 0;
	num_of_max = 0;
	num_of_min = 0;
	float temp;
	while (fin >> temp){
		//record the time point at which the value cross the value of 5
		if (last > high && temp < high){
			max_time[num_of_max] = num_of_data;
			num_of_max++;
		}
		if (temp > max_data){
			max_data = temp;
		}
		//record the time point at which the value cross the value of -5
		if (last < low && temp > low){
			min_time[num_of_min] = num_of_data;
			num_of_min++;
		}
		if (temp < min_data){
			min_data = temp;
		}
		data[num_of_data] = temp;
		last = temp;
		num_of_data++;
	}
	//print the relative time point
	for (int i = 0; i < num_of_max; i++){
		cout << max_time[i] / (float)num_of_data << ' ';
	}
	cout << endl;
	for (int i = 0; i < num_of_min; i++){
		cout << min_time[i] / (float)num_of_data << ' ';
	}
	cout << endl;
	cout << "num: " << num_of_data << endl;
}

void show_graph(char* x){
	ofstream fout(x);
	for (int i = 0; i < graph_row; i++){
		for (int j = 0; j < graph_col; j++){
			fout << graph[i][j];
		}
		fout << endl;
	}
}

void data_to_graph(){
	for (int i = 0; i < graph_row; i++){
		for (int j = 0; j < graph_col; j++){
			graph[i][j] = ' ';
		}
	}
	for (int i = 0; i < num_of_data; i++){
		graph[int(graph_row - (offset + data[i]))][i] = '*';
		graph[int(graph_row - 1)][i] = '-';
	}
}

void data_to_file(char *x){
	ofstream fout(x);
	for (int i = 0; i <= num_of_data; i++){
		fout << data[i] << endl;
	}
}

/* fetch the weighted average of the prev, current and the next */
void data_processing(){
	for (int i = 0; i < num_of_data; i++){
		if (i == 0) data[0] = data[0]*center_pro + data[1]*2*other_pro;
		else if (i == num_of_data - 1) data[i] = data[i]*center_pro + data[i-1]*2*other_pro;
		else data[i] = data[i]*center_pro + data[i + 1]*other_pro + data[i - 1]*other_pro;
	}
}

int main(){
	int i;
	i = 0;
	while (i != file_num){
		fetch_data(fin_namex[i], 5, -5);
		save_codex(i);
		cout << "max " << max_data << " min " << min_data << endl << endl;
		//data_processing();

		//data_to_file(store_file[i]);
		//data_to_graph();
		//show_graph(fout_name[i]);
		i++;
	}
	cout << "----------------------------------------------";
	i = 0;
	while (i != file_num){
		fetch_data(fin_namey[i], 2.5, -2.4);
		save_codey(i);
		cout << "max " << max_data << " min " << min_data << endl << endl;
		//data_processing();

		//data_to_file(store_file[i]);
		//data_to_graph();
		//show_graph(fout_name[i]);
		i++;
	}
	cout << "----------------------------------------------";
	i = 0;
	while (i != file_num){
		fetch_data(fin_namez[i], 18, 2);
		save_codez(i);
		cout << "max " << max_data << " min " << min_data << endl << endl;
		//data_processing();

		//data_to_file(store_file[i]);
		//data_to_graph();
		//show_graph(fout_name[i]);
		i++;
	}
	for (int i = 0; i < file_num; i++){
		cout << "code " << i+1 << ": ";
		for (int j = 0; j < 10; j++){
			cout << setw(2) << codes[i][j] << ' ';
		}
		cout << endl;
	}
	system("pause");
	return 0;
}