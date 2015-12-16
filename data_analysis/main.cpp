#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

#define graph_row 50
#define graph_col 200
#define center_pro 0.5
#define other_pro 0.25
#define offset 25
#define file_num 6
#define max_x_count 2
#define min_x_count 2
#define max_y_count 1
#define min_y_count 2
#define max_z_count 2
#define min_z_count 1
#define max_x_threshold 4.0f
#define min_x_threshold -1.0f
#define max_y_threshold 1.5f
#define min_y_threshold -1.0f
#define max_z_threshold 10.0f
#define min_z_threshold 5.0f


float max_data = 100.0f;
float min_data = -100.0f;
float max_x_time[max_x_count] = { 0 };
float min_x_time[min_x_count] = { 0 };
float max_y_time[max_y_count] = { 0 };
float min_y_time[min_y_count] = { 0 };
float max_z_time[max_z_count] = { 0 };
float min_z_time[min_z_count] = { 0 };
float max_x_value[max_x_count] = { 0 };
float min_x_value[min_x_count] = { 0 };
float max_y_value[max_y_count] = { 0 };
float min_y_value[min_y_count] = { 0 };
float max_z_value[max_z_count] = { 0 };
float min_z_value[min_z_count] = { 0 };

float last = 0.0f;
int num_of_data = 0;
float data_x[200];
float data_y[200];
float data_z[200];
float store[200];
char graph[graph_row][graph_col];
vector<int> codes[file_num];
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


void fetch_x_data(char* x, int y){
	ifstream fin(x);
	for (int i = 0; i < 200; i++) data_x[i] = 0;
	for (int i = 0; i < max_x_count; i++){
		max_x_time[i] = 0;
		max_x_value[i] = 0;
	}
	for (int i = 0; i < min_x_count; i++){
		min_x_time[i] = 0;
		min_x_value[i] = 10;
	}
	num_of_data = 0;
	float temp;
	while (fin >> temp){
		data_x[num_of_data] = temp;
		num_of_data++;
	}
	bool record_max = false;
	bool record_min = false;
	last = 0.0f;
	for (int i = 0; i < num_of_data; i++){
		if (data_x[i] > last) record_max = true;
		if (data_x[i] < last) 
			record_min = true;
		int pos;
		if (data_x[i] < last && last > max_x_threshold && record_max){
			record_max = false;
			pos = 0;
			for (int j = 0; j < max_x_count; j++){
				if (max_x_value[j] < max_x_value[pos]) pos = j;
			}
			if (last > max_x_value[pos]) max_x_value[pos] = last;
		}
		if (data_x[i] > last && last < min_x_threshold && record_min){
			record_min = false;
			pos = 0;
			for (int j = 0; j < min_x_count; j++){
				if (min_x_value[j] > min_x_value[pos]) pos = j;
			}
			if (last < min_x_value[pos]) min_x_value[pos] = last;
		}
		last = data_x[i];
	}
	max_data = max_x_value[0];
	for (int i = 0; i < max_x_count; i++){
		if (max_x_value[i] < max_data) max_data = max_x_value[i];
	}
	min_data = min_x_value[0];
	for (int i = 0; i < min_x_count; i++){
		if (min_x_value[i] > min_data) min_data = min_x_value[i];
	}
	//for (int i = 0; i < max_x_count; i++) cout << max_x_value[i] << ' '; cout << endl;
	//for (int i = 0; i < min_x_count; i++) cout << min_x_value[i] << ' '; cout << endl;
	//cout << max_data << ' ' << min_data << endl;
	last = data_x[1];
	int m = 0;
	int n = 0;
	for (int i = 1; i < num_of_data; i++){
		if (last >= max_data && data_x[i] < max_data){
			max_x_time[m] = i;
			m++;
		}
		if (last <= min_data && data_x[i] > min_data){
			min_x_time[n] = i;
			n++;
		}
		last = data_x[i];
	}
	//for (int i = 0; i < max_x_count; i++) cout << int(100*max_x_time[i]/num_of_data) << ' ';
	//for (int i = 0; i < min_x_count; i++) cout << int(100*min_x_time[i]/num_of_data) << ' ';
	for (int i = 0; i < max_x_count; i++) codes[y].push_back(int(100*max_x_time[i]/num_of_data));
	for (int i = 0; i < min_x_count; i++) codes[y].push_back(int(100*min_x_time[i]/num_of_data));
	//cout << endl;
}

void fetch_y_data(char* x, int y){
	ifstream fin(x);
	for (int i = 0; i < 200; i++) data_y[i] = 0;
	for (int i = 0; i < max_y_count; i++){
		max_y_time[i] = 0;
		max_y_value[i] = 0;
	}
	for (int i = 0; i < min_y_count; i++){
		min_y_time[i] = 0;
		min_y_value[i] = 10;
	}
	num_of_data = 0;
	float temp;
	while (fin >> temp){
		data_y[num_of_data] = temp;
		num_of_data++;
	}
	bool record_max = false;
	bool record_min = false;
	last = 0.0f;
	for (int i = 0; i < num_of_data; i++){
		if (data_y[i] > last) record_max = true;
		if (data_y[i] < last) record_min = true;
		int pos;
		if (data_y[i] < last && last > max_y_threshold && record_max){
			record_max = false;
			pos = 0;
			for (int j = 0; j < max_y_count; j++){
				if (max_y_value[j] < max_y_value[pos]) pos = j;
			}
			if (last > max_y_value[pos]) max_y_value[pos] = last;
		}
		if (data_y[i] > last && last < min_y_threshold && record_min){
			record_min = false;
			pos = 0;
			for (int j = 0; j < min_y_count; j++){
				if (min_y_value[j] > min_y_value[pos]) 
					pos = j;
			}
			if (last < min_y_value[pos]) min_y_value[pos] = last;
		}
		last = data_y[i];
	}
	max_data = max_y_value[0];
	for (int i = 0; i < max_y_count; i++){
		if (max_y_value[i] < max_data) max_data = max_y_value[i];
	}
	min_data = min_y_value[0];
	for (int i = 0; i < min_y_count; i++){
		if (min_y_value[i] > min_data) min_data = min_y_value[i];
	}
	//for (int i = 0; i < max_y_count; i++) cout << max_y_value[i] << ' '; cout << endl;
	//for (int i = 0; i < min_y_count; i++) cout << min_y_value[i] << ' '; cout << endl;
	//cout << max_data << ' ' << min_data << endl;
	last = 0.0f;
	int m = 0;
	int n = 0;
	for (int i = 0; i < num_of_data; i++){
		if (last >= max_data && data_y[i] < max_data){
			max_y_time[m] = i;
			m++;
		}
		if (last <= min_data && data_y[i] > min_data){
			min_y_time[n] = i;
			n++;
		}
		last = data_y[i];
	}
	//for (int i = 0; i < max_y_count; i++) cout << int(100 * max_y_time[i] / num_of_data) << ' ';
	//for (int i = 0; i < min_y_count; i++) cout << int(100 * min_y_time[i] / num_of_data) << ' ';
	for (int i = 0; i < max_y_count; i++) codes[y].push_back(int(100 * max_y_time[i] / num_of_data));
	for (int i = 0; i < min_y_count; i++) codes[y].push_back(int(100 * min_y_time[i] / num_of_data));
	//cout << endl;
}

void fetch_z_data(char* x, int y){
	ifstream fin(x);
	for (int i = 0; i < 200; i++) data_z[i] = 0;
	for (int i = 0; i < max_z_count; i++){
		max_z_time[i] = 0;
		max_z_value[i] = 0;
	}
	for (int i = 0; i < min_z_count; i++){
		min_z_time[i] = 0;
		min_z_value[i] = 10;
	}
	num_of_data = 0;
	float temp;
	while (fin >> temp){
		data_z[num_of_data] = temp;
		num_of_data++;
	}
	bool record_max = false;
	bool record_min = false;
	last = 0.0f;
	for (int i = 0; i < num_of_data; i++){
		if (data_z[i] > last) record_max = true;
		if (data_z[i] < last) record_min = true;
		int pos;
		if (data_z[i] < last && last > max_z_threshold && record_max){
			record_max = false;
			pos = 0;
			for (int j = 0; j < max_z_count; j++){
				if (max_z_value[j] < max_z_value[pos]) pos = j;
			}
			if (last > max_z_value[pos]) max_z_value[pos] = last;
		}
		if (data_z[i] > last && last < min_z_threshold && record_min){
			record_min = false;
			pos = 0;
			for (int j = 0; j < min_z_count; j++){
				if (min_z_value[j] > min_z_value[pos]) 
					pos = j;
			}
			if (last < min_z_value[pos]) min_z_value[pos] = last;
		}
		last = data_z[i];
	}
	max_data = max_z_value[0];
	for (int i = 0; i < max_z_count; i++){
		if (max_z_value[i] < max_data) max_data = max_z_value[i];
	}
	min_data = min_z_value[0];
	for (int i = 0; i < min_z_count; i++){
		if (min_z_value[i] > min_data) min_data = min_z_value[i];
	}
	//for (int i = 0; i < max_z_count; i++) cout << max_z_value[i] << ' '; cout << endl;
	//for (int i = 0; i < min_z_count; i++) cout << min_z_value[i] << ' '; cout << endl;
	//cout << max_data << ' ' << min_data << endl;
	last = data_z[1];      //modification for adapting the first element
	int m = 0;
	int n = 0;
	for (int i = 1; i < num_of_data; i++){
		if (last >= max_data && data_z[i] < max_data){
			max_z_time[m] = i;
			m++;
		}
		if (last <= min_data && data_z[i] > min_data){
			min_z_time[n] = i;
			n++;
		}
		last = data_z[i];
	}
	//for (int i = 0; i < max_z_count; i++) cout << int(100 * max_z_time[i] / num_of_data) << ' ';
	//for (int i = 0; i < min_z_count; i++) cout << int(100 * min_z_time[i] / num_of_data) << ' ';
	for (int i = 0; i < max_z_count; i++) codes[y].push_back(int(100 * max_z_time[i] / num_of_data));
	for (int i = 0; i < min_z_count; i++) codes[y].push_back(int(100 * min_z_time[i] / num_of_data));
	//cout << endl;
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
		graph[int(graph_row - (offset + data_x[i]))][i] = '*';
		graph[int(graph_row - 1)][i] = '-';
	}
}

void data_to_file(char *x){
	ofstream fout(x);
	for (int i = 0; i <= num_of_data; i++){
		fout << data_x[i] << endl;
	}
}

/* fetch the weighted average of the prev, current and the next */
void data_processing(){
	for (int i = 0; i < num_of_data; i++){
		if (i == 0) data_x[0] = data_x[0] * center_pro + data_x[1] * 2 * other_pro;
		else if (i == num_of_data - 1) data_x[i] = data_x[i] * center_pro + data_x[i - 1] * 2 * other_pro;
		else data_x[i] = data_x[i] * center_pro + data_x[i + 1] * other_pro + data_x[i - 1] * other_pro;
	}
}

int main(){
	int i;
	i = 0;
	while (i != file_num){
		fetch_x_data(fin_namex[i],i);
		i++;
	}
	i = 0;
	while (i != file_num){
		fetch_y_data(fin_namey[i],i);
		i++;
	}
	i = 0;
	while (i != file_num){
		fetch_z_data(fin_namez[i],i);
		i++;
	}
	
	for (int i = 0; i < file_num; i++){
		cout << "code " << i+1 << ": ";
		for (int j = 0; j < codes[i].size(); j++){
			cout << codes[i][j] << ' ';
		}
		cout << endl;
	}
	system("pause");
	return 0;
}