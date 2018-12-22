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
// �˿ͽṹ��
struct customer
{
	float demand;
	std::vector<customer_cost> cost;	
};

// �豸�ṹ��
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
int nFacility;							// ��������
int nCustomer;							// �˿�����
std::vector<customer> customerVector;	// �˿͵���Ϣ
std::vector<facility> facilityVector;	// �豸����Ϣ

const double coolSpeed = 0.99; 		// �˻��ٶ�
const double acceptT = 0.00001;		// �����¶�
const double initialT = 2000;		// ��ʼ�¶�
const int turn = 100;				// ��ѭ������

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

	// ͳ��һ������ܻ���
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

	// �������һ���⣬���ݿ��ŵ�˳��˳�����˿�
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
		// ����豸����˳��
		for (int i = 0; i < nChange1; ++i)
		{
			int pos1 = rand() % nFacility;
			int pos2 = rand() % nFacility;
			int temp = facilitySequence[pos1];
			facilitySequence[pos1] = facilitySequence[pos2];
			facilitySequence[pos2] = temp;
		}
		// ����˿ͼ���˳��
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
		// ����ǰ���˳�������һ����
		int j = 0; // ͳ���豸�����
		facilityResult[facilitySequence[0]] = 1;
		for (int i = 0; i < nCustomer; ++i)
		{
			// ����ǰ�豸װ�����û������󣬲���һ��������װ�µ��豸
			while(customerVector[customerSequence[i]].demand > spareCapacity[facilitySequence[j]].capacity){
				j++;
				j = j % nFacility;
				// �µ�
				if (facilityResult[facilitySequence[j]] == 0)
				{
					facilityResult[facilitySequence[j]] = 1;
					spareCapacity[facilitySequence[j]].capacity = facilityVector[facilitySequence[j]].capacity;
				}
			}
			// ���û�װ���豸���j
			spareCapacity[facilitySequence[j]].capacity -= customerVector[i].demand;
			customerResult[i] = facilitySequence[j];
		}
	} 
	// ���ý�ĺϷ���
	bool CheckSolution(){
		int facilityCapacity[50];
		for(int i = 0; i < nFacility; i++){
			facilityCapacity[i] = 0;
		}
		// �������
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
  		cout << "���ѣ�" << totalCost << endl;
		//outfile << "���ѣ�" << totalCost << endl;
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

	// �ع��ȽϺ������ж���һ���𰸸���
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
	int totalCost;							// �ܻ���
	vector<int> facilityResult;				// ������Щ��������(01��ʾ)
	vector<int> customerResult;				// ����ÿ���˿�ȥ�ĸ��豸
	vector<int> facilitySequence;			// �豸���ŵ�˳��
	vector<int> customerSequence;			// �ͻ������˳��
	vector<spare> spareCapacity;			// �豸��ʣ��ռ�
};





