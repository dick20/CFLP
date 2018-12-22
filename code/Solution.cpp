#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

struct customer_cost
{
	float value;
	int id;
};
// 顾客结构体
struct customer
{
	float demand;
	std::vector<customer_cost> cost;	
};

// 设备结构体
struct facility
{
	int capacity;
	int cost;
};

struct spare
{
	int id;
	int capacity;
};

bool operator< (const customer_cost& temp1, const customer_cost& temp2){
	return temp1.value < temp2.value;
}

bool operator< (const spare& temp1, const spare& temp2){
	return temp1.capacity < temp2.capacity;
}
int nFacility;							// 工厂数量
int nCustomer;							// 顾客数量
std::vector<customer> customerVector;	// 顾客的信息
std::vector<facility> facilityVector;	// 设备的信息

const double coolSpeed = 0.99; 		// 退火速度
const double acceptT = 0.00001;		// 接受温度
const double initialT = 2000;		// 初始温度
const int turn = 100;				// 内循环次数

class Solution
{
public:
	Solution(){
		totalCost = 0;
		facilityResult.resize(nFacility,0);
		customerResult.resize(nCustomer,0);			
		facilitySequence.resize(nFacility,0); 
		customerSequence.resize(nCustomer,0);	
		spareCapacity.resize(nFacility);
		for (int i = 0; i < nFacility; ++i)
		{
			facilityResult[i] = 0;
			spareCapacity[i].capacity = facilityVector[i].capacity;
			spareCapacity[i].id = i;
		}
		for (int i = 0; i < nCustomer; ++i)
		{
			customerResult[i] = -1;
		}
		RandomGenerate();
		CalculateTotalCost();
	}
	~Solution(){
		totalCost = 0;
	}

	// 统计一个解的总花费
	void CalculateTotalCost(){
		totalCost = 0;
		for (int i = 0; i < nFacility; ++i)
		{
			if (facilityResult[i])
			{
				totalCost += facilityVector[i].cost;
			}
		}
		for (int i = 0; i < nCustomer; ++i)
		{
			//cout << customerResult[i] << " ";
			for (int j = 0; j < nFacility; ++j)
			{
				if (customerResult[i] == customerVector[i].cost[j].id)
				{
					//cout <<  " customerVector[i].cost[j].id "<< customerVector[i].cost[j].value << " " << customerVector[i].cost[j].id<< endl;
					totalCost += customerVector[i].cost[j].value;
					break;
				}
			}
		}
	}

	// 随机生成一个解，根据开放的顺序，顺序填充顾客
	void RandomGenerate(){
		/*for (int i = 0; i < nFacility; ++i){
			facilityResult[i] = 0;
		}
		do{
			srand(time(0));
			int pos = rand()% nFacility;
			for(int i = 0; i < nCustomer; ++i){
				customerResult[i] = pos;
				facilityResult[pos] = 1;
				pos = rand()% nFacility;			
			}
		}while(!CheckSolution());*/
		
		srand(time(0));
		int nChange1 = rand() % nFacility;
		int nChange2 = rand() % nCustomer;
		for (int i = 0; i < nFacility; ++i)
		{
			facilitySequence[i] = i;
		}
		// 随机设备开放顺序
		for (int i = 0; i < nChange1; ++i)
		{
			int pos1 = rand() % nFacility;
			int pos2 = rand() % nFacility;
			int temp = facilitySequence[pos1];
			facilitySequence[pos1] = facilitySequence[pos2];
			facilitySequence[pos2] = temp;
		}
		// 随机顾客加入顺序
		for (int i = 0; i < nCustomer; ++i){
			customerSequence[i] = i;
		}
		for (int i = 0; i < nChange2; ++i)
		{
			int pos1 = rand() % nCustomer;
			int pos2 = rand() % nCustomer;
			int temp = customerSequence[pos1];
			customerSequence[pos1] = customerSequence[pos2];
			customerSequence[pos2] = temp;
		}
		// 按照前面的顺序来填充一个解
		int j = 0; // 统计设备的序号
		facilityResult[facilitySequence[0]] = 1;
		for (int i = 0; i < nCustomer; ++i)
		{
			// 当当前设备装不下用户的需求，查找一个容量能装下的设备
			while(customerVector[customerSequence[i]].demand > spareCapacity[facilitySequence[j]].capacity){
				j++;
				j = j % nFacility;
				// 新的
				if (facilityResult[facilitySequence[j]] == 0)
				{
					facilityResult[facilitySequence[j]] = 1;
					spareCapacity[facilitySequence[j]].capacity = facilityVector[facilitySequence[j]].capacity;
				}
			}
			// 将用户装入设备序号j
			spareCapacity[facilitySequence[j]].capacity -= customerVector[i].demand;
			customerResult[i] = facilitySequence[j];
		}
	} 
	// 检查该解的合法性
	bool CheckSolution(){
		int facilityCapacity[50];
		for(int i = 0; i < nFacility; i++){
			facilityCapacity[i] = 0;
		}
		// 检查容量
		for (int i = 0; i < nCustomer; ++i)
		{
			facilityCapacity[customerResult[i]] += customerVector[i].demand;
		}
		for (int i = 0; i < nFacility; ++i)
		{
			if (facilityCapacity[i] > facilityVector[i].capacity)
			{
				return false;
			}
		}
		return true;
	}

	void printAnswer(){
  		cout << "花费：" << totalCost << endl;
		//outfile << "花费：" << totalCost << endl;
		for (int i = 0; i < nFacility; ++i)
		{
			cout << facilityResult[i] << " ";
			//outfile <<  facilityResult[i] << " ";
		}
		cout << endl;
		//outfile << endl;
		for (int i = 0; i < nCustomer; ++i)
		{
			cout << customerResult[i] << " ";
			//outfile << customerResult[i] << " ";
		}
		cout << endl;
		//outfile << endl;
	}

	// 重构比较函数，判断哪一个答案更好
	bool operator < (const Solution &other) const
	{
		return totalCost < other.totalCost;
	}

	vector<int> getFacilityResult(){
		return facilityResult;
	}

	vector<int> getCustomerResult(){
		return customerResult;
	}

	vector<int> getFacilitySequence(){
		return facilitySequence;
	}

	vector<int> getCustomerSequence(){
		return customerSequence;
	}

	vector<spare> getSpareCapacity(){
		return spareCapacity;
	}

	int getTotalCost(){
		return totalCost;
	}

	void setFacilityResult(vector<int> facilityResult){
		this->facilityResult = facilityResult;
	}

	void setCustomerResult(vector<int> customerResult){
		this->customerResult = customerResult;
	}

	void setFacilitySequence(vector<int> facilitySequence){
		this->facilitySequence = facilitySequence;
	}

	void setCustomerSequence(vector<int> customerSequence){
		this->customerSequence = customerSequence;
	}

	void setSpareCapacity(vector<spare> spareCapacity){
		this->spareCapacity = spareCapacity;
	}
	
private:
	int totalCost;							// 总花费
	vector<int> facilityResult;				// 安排哪些工厂开放(01表示)
	vector<int> customerResult;				// 安排每个顾客去哪个设备
	vector<int> facilitySequence;			// 设备开放的顺序
	vector<int> customerSequence;			// 客户加入的顺序
	vector<spare> spareCapacity;			// 设备的剩余空间
};





