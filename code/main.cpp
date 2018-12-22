#include "CFLP.cpp"
#include <cstdlib>
#include <ctime>

int main(){
	string str;
	ofstream outfile;
	outfile.open("result.txt");
	for(int i = 51; i < 72;i++){
		time_t start, finish;
  		start = clock(); // 开始计时
		str = "p";
		str += to_string(i);
		cout << "正在计算"+str<<endl; 
		CFLP cflp(str);
		cout << nFacility <<" "<< nCustomer<< endl;
		Solution s = cflp.SA();
		//s.printAnswer();
		int totalCost = s.getTotalCost();
		vector<int> facilityResult = s.getFacilityResult();
		vector<int> customerResult = s.getCustomerResult();
		outfile << "花费：" << totalCost << endl;
		for (int i = 0; i < nFacility; ++i)
		{
			outfile <<  facilityResult[i] << " ";
		}
		outfile << endl;
		for (int i = 0; i < nCustomer; ++i)
		{
			outfile << customerResult[i] << " ";
		}
		//cout << endl;
		outfile << endl;
		finish = clock(); // 结束计时
		double duration = ((double)(finish - start)) / CLOCKS_PER_SEC;
		cout << str + " 的计算时间为" << duration << " right: "<< s.CheckSolution() << endl;
	}
	outfile.close();
	return 0;
}


