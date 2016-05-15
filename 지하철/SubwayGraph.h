#ifndef SUBWAYGRAPH_H
#define SUBWAYGRAPH_H

#include <iostream>
#include <fstream>
#include <stack> // 지하철 경로를 출력할때 필요
#include <string> 
using namespace std;

enum color { white, gray }; // 역을 지났는지 안지났는지 판단

class Station // 역에 대한 정보
{
public:
	int num; // 역에 매겨진 숫자
	int time; // 이동시간
	int transfer; // 환승 횟수	
	Station* next; // 다음 역을 가르키는 포인터
	bool check; // 환승역인지 아닌지 check
	Station() : next(NULL) {}
};

class SubwayGraph // 지하철망
{
private:
	string* station_name; // 역이름
	Station* map; // 다익스트라 알고리즘의 적용된 새로운 지하철 망
	color* check; // white는 아직 지나지 않은 역, gray는 지난 역
	int n; // 지하철 역 갯수
public:
	void Subway(const char* name, int N);
	void Dijkstra(int start, bool t); // Dijkstra Algorithm 구현부분
	int ChooseSubwayPath(bool choose); // 최단시간, 최소환승중 보고싶은 부분 선
	bool SubwayRoute(stack<int>& s, int start, int end); // Dijkstra Algorithm을 실행한 후의 그래프에서 경로를 스택에 옮겨주는 재귀함수
	void PrintRoute(stack<int>& s, int start, int end); // 찾은 경로는 출력해주는 함수.
};

#endif
