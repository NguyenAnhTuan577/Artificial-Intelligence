#include <iostream>
#include <string>
#include <string.h>
#include <vector>


using namespace std;


struct Luat
{
	string TenLuat;
	char **bien;
	//VD:father(X,Y):-parent(X,Y),male(X).
	//		0	1	2	 
	//	0	x	y	0
	//	1	x	y	0
	//	2	x	0	0
	//dùng mảng 2 chiều để xem coi truyền hệ số như thế nào
	vector<string>Heso;
	vector<Luat>Thanhphan;
};

struct MangGT
{
	string tenvitu;
	vector<Giathiet> thanhphan;
};

struct Giathiet
{
	string TenGiaThiet;
	vector<string>Heso;
};


bool ChungMinh(Giathiet Vephai, vector<Luat> MangLuat, vector<MangGT> MangGiaThiet)
{
	vector<Giathiet> vetrai;
	vetrai.push_back(Vephai);
	int i = 0;
	while (!vetrai.empty())
	{
		if (SoSanh(vetrai[0], MangGiaThiet))//phần tử đầu có trong gt hay không
		{
			vetrai.erase(vetrai.begin());
		}
		else if (SosanhLuat(vetrai[0].TenGiaThiet, MangLuat e) != -1)//phần tử đầu có trong luật hay không
		{
			//cho vào 1 hàm lấy phần tử đầu tiên của vector tìm luật tương ứng bỏ phần tử đó ra, rồi thêm vào các luật thành 
			//phần tương ứng VD: 
			//phần tử: father(tom,will)
			//luật : father(X,Y):-parent(X,Y),male(X).
			//bỏ father(tom,will) ra khỏi vector(dùng erase) thêm vào parent(tom,will),male(tom)(dùng push_back)
			//có thể dùng hàm này để giải câu hỏi dạng tìm X
		}
		else if (SosanhLuat(vetrai[0].TenGiaThiet, MangLuat) == -1)//phần tử không có trong luật không có trong giả thiết nên không tồn tại =>trà về false
		{
			return false;
		}
	}
	return true;
}
//tìm xem giả thiết cần chứng minh của đề bài có nằm trong luật nào không
//VD: giả thiết cần chứng minh father(will,tom)
//tìm xem có luật nào có vế phải là father(X,Y) không
//nếu có trả về số thứ tự của luật đó trong mảng nếu không trả về -1
int SosanhLuat(string tenluat, vector<Luat> MangLuat)
{
	for (int i = 0; i < MangLuat.size(); i++)
	{
		if (tenluat == MangLuat[i].TenLuat)
		{
			return i;
		}
	}
	return -1;
}

//lấy 1 giả thiết tìm xem nó có trong mảng giả thiết ban đầu hay không, nếu có trả vể true nếu không trả về false
bool SoSanh(Giathiet ten1, vector<MangGT> MangGiaThiet)
{
	for (int i = 0; i < MangGiaThiet.size(); i++)
	{
		if (ten1.TenGiaThiet == MangGiaThiet[i].tenvitu)
		{
			int e = 0;
			while (e<MangGiaThiet[i].thanhphan.size())
			{
				int t = 0;
				while (ten1.Heso[t] != MangGiaThiet[i].thanhphan[e].Heso[t])
				{
					if (t == MangGiaThiet[i].thanhphan[e].Heso.size() - 1)
					{
						return true;
					}
					t++;
				}
				e++;
			}
		}
	}
	return false;
}

int main()
{
	std::cout << "Hello World!\n";
}

