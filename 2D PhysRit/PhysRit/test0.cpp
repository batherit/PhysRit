#include<iostream>
using namespace std;

class CFirst
{
public:
	/*CFirst()
	{
		cout << "I am empty CFirst" << endl;			// ������ ����
	}*/
	CFirst(int)
	{
		cout << "I am some CFirst" << endl;
	}
};

class CSecond : public CFirst
{
public:
	CSecond() 
	{
		cout << "I am empty CSecond" << endl;
	};
};

int main(void)
{
	CSecond sec;

	return 0;
}