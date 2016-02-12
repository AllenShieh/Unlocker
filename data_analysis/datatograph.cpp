#include <iostream>
#include <fstream>
using namespace std;

char graph[40][100];
float data[10][100];

void getdata(char * name){
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
}

void printdata(int x){
	for (int i = 0; i < 100; i++){
		cout << data[x][i] << endl;
	}
}

void tograph(int x){
	for (int i = 0; i < 40; i++){
		for (int j = 0; j < 100; j++){
			graph[i][j] = ' ';
		}
	}
	for (int y = 0; y < 100; y++){
		graph[int(data[x][y])+20][y] = '*';
	}
}

void drawgraph(char * name){
	ofstream fout(name);
	for (int i = 0; i < 40; i++){
		for (int j = 0; j < 100; j++){
			fout << graph[i][j];
		}
		fout << endl;
	}
}

int main(){
	getdata("sensor_data3.txt");
	//printdata(0);
	tograph(0);
	drawgraph("graph_Ax.txt");
	tograph(1);
	drawgraph("graph_Ay.txt");
	tograph(2);
	drawgraph("graph_Az.txt");
	tograph(3);
	drawgraph("graph_Lx.txt");
	tograph(4);
	drawgraph("graph_Ly.txt");
	tograph(5);
	drawgraph("graph_Lz.txt");
	tograph(6);
	drawgraph("graph_Gx.txt");
	tograph(7);
	drawgraph("graph_Gy.txt");
	tograph(8);
	drawgraph("graph_Gz.txt");
	tograph(9);
	drawgraph("graph_P.txt");
	//system("pause");
	return 0;
}