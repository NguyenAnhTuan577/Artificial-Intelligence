#include<iostream>
#include<string>
#include<stack>
#include<set>
#include<vector>
#include<fstream>

using namespace std;

int n = 0;//Bản đồ có kích thước n*n
int sx, sy;//Tọa độ điểm Start
int gx, gy;//Tọa độ điểm Goal
fstream fpInput, fpOutput;//Dòng nhập xuất file

typedef pair<int, int> Pair; //pair int
typedef pair<double, pair<int, int>> pPair;//pair double-pair

										   //Cấu trúc để lưu một số giá trị cho một ô
struct cell
{
	int parent_i, parent_j;
	// f = g + h 
	double f, g, h;
};

//Kiểm tra các ô đó có trong giới hạn của bản đồ không
bool isValid(int row, int col)
{
	return (row >= 0) && (row < n) && (col >= 0) && (col < n);
}

//Kiểm tra ô đó có trống không, returns true nếu ô đó trống
bool isEmptySquare(vector<vector<int>> grid, int row, int col)
{
	if (grid[row][col] == 0)
		return (true);
	return (false);
}

//Kiểm tra ô đó có phải là Goal không
bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

//Hàm tính heuristics(h). Heuristic Euclidean
double calculateHeuristic(int row, int col, Pair dest)
{
	return ((double)sqrt((row - dest.first)*(row - dest.first)
		+ (col - dest.second)*(col - dest.second)));
}

//Hàm xuất kết quả
void Output(vector<vector<cell>> cellDetails, vector<vector<int>>grid, Pair dest)
{
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;//Thứ tự các điểm của đường đi tìm được

	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
	{
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));//push điểm Star vào

								   //path lưu dưới dạng set (giống cấu trúc cây nhưng có xếp thứ tự)
	set<pair<int, int>>vPath;
	fpOutput << Path.size() << endl;//Độ dài đường đi
	while (!Path.empty())
	{
		pair<int, int> p = Path.top();
		vPath.insert(p);
		Path.pop();
		fpOutput << "(" << p.first << "," << p.second << ")";
		if (!Path.empty())
			fpOutput << " ";
	}

	fpOutput << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == sx && j == sy)
				fpOutput << "S";
			else if (i == gx && j == gy)
				fpOutput << "G";
			else if (grid[i][j] == 1)
				fpOutput << "o";
			else
			{
				if (vPath.empty() == false && (*vPath.begin()).first == sx && (*vPath.begin()).second == sy)
					vPath.erase(*vPath.begin());
				if (vPath.empty() == false && (*vPath.begin()).first == gx && (*vPath.begin()).second == gy)
					vPath.erase(*vPath.begin());
				if (vPath.empty() == false && (*vPath.begin()).first == i && (*vPath.begin()).second == j)
				{
					vPath.erase(*vPath.begin());
					fpOutput << "x";
				}
				else
					fpOutput << "-";
			}
			if (j != n - 1)
				fpOutput << " ";
		}
		fpOutput << endl;
	}
	return;
}


//A* Search Algorithm, return 0 nếu tìm được đường đi và return -1 nếu không
int aStarSearch(vector<vector<int>> grid, Pair src, Pair dest)
{
	//reachGoal = true nếu tìm được đường đến Goal
	bool reachGoal = false;

	//Kiểm tra Start và Goal có đúng hay không
	if (isValid(src.first, src.second) == false)
	{
		return -1;
	}
	if (isValid(dest.first, dest.second) == false)
	{
		return -1;
	}

	//Kiểm tra Start và Goal có chướng ngại vật không
	if (isEmptySquare(grid, src.first, src.second) == false || isEmptySquare(grid, dest.first, dest.second) == false)
	{
		return -1;
	}


	//Trạng thái của các ô có vào danh sách đóng lại hay không. Khởi tạo tất cả các ô đều là false
	vector < vector<bool>>closeList(n, vector<bool>(n));
	for each (vector<bool> var in closeList)
	{
		for each (bool b in var)
		{
			b = false;
		}
	}

	//Lưu theo cấu trúc cell cho các ô (gồm có vị trí ô cha và f,g,h)
	vector<vector<cell>> cellDetails(n, vector<cell>(n));
	int i, j;

	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			cellDetails[i][j].f = DBL_MAX;//Gán nó cho giá trị lớn nhất của kiểu DOUBLE
			cellDetails[i][j].g = DBL_MAX;
			cellDetails[i][j].h = DBL_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Khởi tạo cho ô Start
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	//Kiểm tra Goal và Start có bị trùng không 
	if (isDestination(src.first, src.second, dest) == true)
	{
		//fpOutput << 0;
		//cellDetails[de]
		Output(cellDetails, grid, dest);
		return 0;
	}
	//Danh sách thứ tự các ô cần mở
	set<pPair> openList;

	//Chèn ô Start vào
	openList.insert(make_pair(0.0, make_pair(i, j)));



	while (!openList.empty())
	{
		pPair p = *openList.begin();
		openList.erase(openList.begin());

		//Cập nhật đỉnh vừa xét vào closeList
		i = p.second.first;
		j = p.second.second;
		closeList[i][j] = true;

		// Duyệt các ô xung quanh ô đang xét
		double gNew, hNew, fNew;
		//Thứ tự duyệt các điểm lân cận
		//			1 2 3
		//			8 x 4
		//			7 6 5
		//
		//----------- vị trí số 1 ------------ 
		if (isValid(i - 1, j - 1) == true)
		{
			//Kiểm tra ô này có phải Goal không
			if (isDestination(i - 1, j - 1, dest) == true)
			{
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i - 1][j - 1] == false && isEmptySquare(grid, i - 1, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;//Thêm căn 2 do là ô chéo
				hNew = calculateHeuristic(i - 1, j - 1, dest);
				fNew = gNew + hNew;

				//Cập nhật giá trị tại ô này khi ô này chưa từng ở trong openList 
				//hoặc giá trị hiện tại tốt hơn giá trị đã lưu trong openList			
				if (cellDetails[i - 1][j - 1].f == DBL_MAX || cellDetails[i - 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;
					cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i;
					cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}

		//----------- vị trí số 2 ------------ 


		if (isValid(i - 1, j) == true)
		{
			if (isDestination(i - 1, j, dest) == true)
			{
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i - 1][j] == false && isEmptySquare(grid, i - 1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHeuristic(i - 1, j, dest);
				fNew = gNew + hNew;
				if (cellDetails[i - 1][j].f == DBL_MAX || cellDetails[i - 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j)));
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}
		//----------- vị trí số 3 ------------ 

		if (isValid(i - 1, j + 1) == true)
		{
			if (isDestination(i - 1, j + 1, dest) == true)
			{
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i - 1][j + 1] == false && isEmptySquare(grid, i - 1, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHeuristic(i - 1, j + 1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i - 1][j + 1].f == DBL_MAX ||
					cellDetails[i - 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j + 1)));
					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;
					cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i;
					cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}

		//-----------vị trí số 4 ------------ 


		if (isValid(i, j + 1) == true)
		{
			if (isDestination(i, j + 1, dest) == true)
			{
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i][j + 1] == false && isEmptySquare(grid, i, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHeuristic(i, j + 1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i][j + 1].f == DBL_MAX || cellDetails[i][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i, j + 1)));
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}
		//----------- vị trí số 5 ------------ 

		if (isValid(i + 1, j + 1) == true)
		{
			if (isDestination(i + 1, j + 1, dest) == true)
			{
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i + 1][j + 1] == false && isEmptySquare(grid, i + 1, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHeuristic(i + 1, j + 1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i + 1][j + 1].f == DBL_MAX ||
					cellDetails[i + 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j + 1)));
					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;
					cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i;
					cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- vị trí số 6 ------------ 


		if (isValid(i + 1, j) == true)
		{
			if (isDestination(i + 1, j, dest) == true)
			{
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i + 1][j] == false && isEmptySquare(grid, i + 1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHeuristic(i + 1, j, dest);
				fNew = gNew + hNew;
				if (cellDetails[i + 1][j].f == DBL_MAX || cellDetails[i + 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j)));
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//----------- vị trí số 7 ------------ 

		if (isValid(i + 1, j - 1) == true)
		{
			if (isDestination(i + 1, j - 1, dest) == true)
			{
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i + 1][j - 1] == false && isEmptySquare(grid, i + 1, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHeuristic(i + 1, j - 1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i + 1][j - 1].f == DBL_MAX || cellDetails[i + 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j - 1)));
					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;
					cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i;
					cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}



		//----------- vị trí số 8 ------------ 


		if (isValid(i, j - 1) == true)
		{
			if (isDestination(i, j - 1, dest) == true)
			{
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				Output(cellDetails, grid, dest);
				reachGoal = true;
				return 0;
			}
			else if (closeList[i][j - 1] == false && isEmptySquare(grid, i, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHeuristic(i, j - 1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i][j - 1].f == DBL_MAX || cellDetails[i][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i, j - 1)));
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}
	}

	if (reachGoal == false)
		return -1;

	return 0;
}

int main(int argc, char** argv)
{
	string nameFileInput, nameFileOutput;
	nameFileInput = argv[1];
	nameFileOutput = argv[2];
	/*nameFileInput = "INPUT.txt";
	nameFileOutput = "out.txt";*/
	fpInput.open(nameFileInput, ios_base::in);
	fpOutput.open(nameFileOutput, ios_base::out);
	if (fpInput.fail())
	{
		fpOutput << "Mo file input that bai!";
		return 0;
	}
	if (fpOutput.fail())
	{
		fpOutput << "Mo file output that bai!";
		return 0;
	}
	int temp;
	fpInput >> n;
	fpInput >> sx >> sy;
	fpInput >> gx >> gy;

	vector<vector<int>> grid;//Lưu bản đồ
	vector<int> v;//vector tạm
	for (int i = 0; i < n; i++)
	{
		for (int i = 0; i < n; i++)
		{
			fpInput >> temp;
			v.push_back(temp);
		}
		grid.push_back(v);
		v.clear();
	}
	Pair src = make_pair(sx, sy);//Start
	Pair dest = make_pair(gx, gy);//Goal

	if (aStarSearch(grid, src, dest) == -1)
		fpOutput << "-1\n";
	system("pause");
	return(0);
}