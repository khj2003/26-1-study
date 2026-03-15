#include <iostream>
using namespace std;

struct Num {
	int a;
	Num* next; // 다른 Num 자료형의 저장장소를 가르키기 위함
};

int main() {
	Num* p1 = new Num; //Num 자료형으로 동적 메모리 할당
	p1->a = 10; // p1이 저장하고 있는 데이터는 10

	Num* p2 = new Num;
	p2->a = 20;

	Num* p3 = new Num;
	p3->a = 30;

	p1->next = p3; //p1이 p3를 가르킴
	p3->next = p2; //p3이 p2를 가르킴
	p2->next = NULL; //p2가 마지막임을 명시

	cout << p1->next->a << endl; //출력 결과 : 30(p3를 가르키기 때문)
	cout << p3->next->a << endl; //출력 결과 : 20(p2를 가르키기 때문)
	
}