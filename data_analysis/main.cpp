#include <iostream>
#include <fstream>

using namespace std;

#define graph_row 50
#define graph_col 200
#define center_pro 0.5
#define other_pro 0.25
#define offset 25

float max_data = -100.0f;
float min_data = 100.0f;
int num_of_data = 0;
float data[200];
char graph[graph_row][graph_col];
char fin_name[8][20] = { "sensor_data_Ax1.txt", "sensor_data_Ax2.txt", "sensor_data_Ax3.txt", "sensor_data_Ax4.txt",
					"sensor_data_Ax5.txt", "sensor_data_Ax6.txt", "sensor_data_Ax7.txt", "sensor_data_Ax8.txt" };
char fout_name[8][20] = { "graph1.txt", "graph2.txt", "graph3.txt", "graph4.txt",
					"graph5.txt", "graph6.txt", "graph7.txt", "graph8.txt" };
char store_file[8][20] = { "weighted_data1.txt", "weighted_data2.txt", "weighted_data3.txt", "weighted_data4.txt",
					"weighted_data5.txt", "weighted_data6.txt", "weighted_data7.txt", "weighted_data8.txt" };

void fetch_data(char* x){
	ifstream fin(x);
	max_data = -100.0f;
	min_data = 100.0f;
	for (int i = 0; i<num_of_data; i++) data[i] = 0;
	num_of_data = 0;
	float temp;
	while (fin >> temp){
		if (temp > max_data) max_data = temp;
		if (temp < min_data) min_data = temp;
		data[num_of_data] = temp;
		num_of_data++;
	}
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
	while (i != 8){
		fetch_data(fin_name[i]);
		cout << max_data << endl << min_data << endl;
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_processing();
		data_to_file(store_file[i]);
		data_to_graph();
		show_graph(fout_name[i]);
		i++;
	}
	system("pause");
	return 0;
}