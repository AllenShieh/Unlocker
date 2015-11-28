#include <iostream>
#include <fstream>

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
char fin_name[file_num][20] = { "sensor_data_Ax1.txt", "sensor_data_Ax2.txt", "sensor_data_Ax3.txt", "sensor_data_Ax4.txt",
					"sensor_data_Ax5.txt", "sensor_data_Ax6.txt" };
char fout_name[file_num][20] = { "graph1.txt", "graph2.txt", "graph3.txt", "graph4.txt",
					"graph5.txt", "graph6.txt" };
char store_file[file_num][20] = { "weighted_data1.txt", "weighted_data2.txt", "weighted_data3.txt", "weighted_data4.txt",
					"weighted_data5.txt", "weighted_data6.txt" };
char processed_file[file_num][20] = { "Ax1.txt", "Ax2.txt", "Ax3.txt", "Ax4.txt", "Ax5.txt", "Ax6.txt" };

void fetch_data(char* x){
	ifstream fin(x);
	max_data = -100.0f;
	min_data = 100.0f;
	for (int i = 0; i<num_of_data; i++) data[i] = 0;
	num_of_data = 0;
	num_of_max = 0;
	num_of_min = 0;
	float temp;
	while (fin >> temp){
		//record the time point at which the value cross the value of 5
		if (last > 5 && temp < 5){
			max_time[num_of_max] = num_of_data;
			num_of_max++;
		}
		if (temp > max_data){
			max_data = temp;
		}
		//record the time point at which the value cross the value of -5
		if (last < -5 && temp > -5){
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
	cout << "num: " << num_of_data << endl << endl;
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
	int i = 0;
	while (i != file_num){
		//fetch_data(processed_file[i]);
		fetch_data(fin_name[i]);
		//cout << max_data << endl << min_data << endl;
		//data_processing();

		data_to_file(store_file[i]);
		data_to_graph();
		show_graph(fout_name[i]);
		i++;
	}
	system("pause");
	return 0;
}