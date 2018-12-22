#include "Solution.cpp"




class CFLP
{
public:
	CFLP(string filePath){
		this->filePath = filePath;
		init_data();
		//test();
	}

	~CFLP(){
	}
	Solution SA() {
		srand(time(0));//之后用到很多随机数，设定种子
		int i = 0;
		double r = coolSpeed;
		double t = initialT;//获取初温
		Solution s;
		// 查看初解
		s.printAnswer();
		/*for(int i = 0; i < nCustomer; i++){
			for (int j = 0; j < nFacility; j++){
				cout << customerVector[i].cost[j].id << " " << customerVector[i].cost[j].value << "\n ";
			}
		}*/
		//int str;
		//cin >> str;
		Solution best = s;
		int addT = 0;
		while (t > acceptT) {
			//设定一个固定的循环次数
			for (i = 0; i < turn; i++) {
				Solution next = s;
				
				getNewSolution(next);//获取邻域解
				//cout << next.CheckSolution() << " " << i << endl;
				if (Accept(next, s, t)) {
					s = next;//接受新解为当前最优
					if(next < best){//记录全局最优
						best = next;
					}
				}
			}
			//cout << "当前解" << s.getTotalCost() << endl;
			//cout << "现在的温度是" << t << "现在的全局最优解为" << s.getTotalCost() << endl;
			//s.printAnswer();
			t *= r;//降温
			//升温 
			/*if(addT < 3 && t < 20){
				t *= 20;
				addT++;
			}*/
		}
		return best;
	}

private:
	string filePath;
	// 判断是否接受新解
	bool Accept(Solution &temp, Solution &best, double t) {
		// 好解全部接受
		if (temp.getTotalCost() < best.getTotalCost()) return true;
		// 差解概率接受
		else if (rand() / (double)RAND_MAX < 1 / (1 + exp(-(best.getTotalCost() - temp.getTotalCost()) / t)))
			return true;
		return false;
	}

	// // 产生新解函数
	void getNewSolution(Solution &t) {
		int choose = rand() % 2;
		//if(choose==1) choose++;
		vector<int> facilityResult = t.getFacilityResult();
		vector<int> customerResult = t.getCustomerResult();
		vector<spare> spareCapacity = t.getSpareCapacity();
		vector<int> facilitySequence = t.getFacilitySequence();
		Solution old = t;
		int count;
		// 交换，找到0与1的位置，将两者交换，查看合法性，重新统计长度，顾客改变
		switch(choose){
			case 0:{
				count = 40;
				while(count > 0){
					int i = rand() % nFacility;
					int j = rand() % nFacility;
					if (facilityResult[i] == 1 && facilityResult[j] == 0)
					{
						facilityResult[i] = 0;
						facilityResult[j] = 1;
						// 需要改变的用户
						std::vector<int> changeCustomer;
						for (int k = 0; k < nCustomer; ++k)
						{
							if (customerResult[k] == i)
							{
								changeCustomer.push_back(k);
							}
						}
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity = facilityVector[x].capacity;
								break;
							}
						}
						for (int k = 0; k < changeCustomer.size(); ++k)
						{
							// 采用best fit 
							sort(spareCapacity.begin(),spareCapacity.end());
							for(int x = 0; x < nFacility; x++){
								if(customerVector[changeCustomer[k]].demand < spareCapacity[x].capacity){
									spareCapacity[x].capacity -= customerVector[changeCustomer[k]].demand;
									customerResult[changeCustomer[k]] = spareCapacity[x].id;
									facilityResult[spareCapacity[x].id] = 1;
									break;
								}
							}
						}
						break;
					}
					count--;
				}
				break;
			}
			case 1:{
				int i1 = rand() % nFacility;
				int i2 = rand() % nFacility;
				while(i1 == i2) i2 = rand() % nFacility;
				// 随机搜索一些用户，拿出来重新放置 
				int count = nCustomer;
				// 需要改变的用户
				std::vector<int> changeCustomer;
				for (int k = 0; k < count; ++k)
				{
					if (customerResult[k] == i1)
					{
						changeCustomer.push_back(k);
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i1){
								spareCapacity[x].capacity += customerVector[k].demand;
								break;
							}
						}
					}
				}
				for (int k = 0; k < count; ++k)
				{
					if (customerResult[k] == i2)
					{
						changeCustomer.push_back(k);
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i2){
								spareCapacity[x].capacity += customerVector[k].demand;
								break;
							}
						}
					}
				}
				facilityResult[i1] = 0;
				facilityResult[i2] = 0;
				// 从下一个开始装填
				int j = (i1+i2) % nFacility;
				for (int k = 0; k < changeCustomer.size(); ++k)
				{
					// 采用best fit 
					sort(spareCapacity.begin(),spareCapacity.end());
					for(int x = 0; x < nFacility; x++){
						if(customerVector[changeCustomer[k]].demand < spareCapacity[x].capacity){
							spareCapacity[x].capacity -= customerVector[changeCustomer[k]].demand;
							customerResult[changeCustomer[k]] = spareCapacity[x].id;
							//cout << "spareCapacity[x].id " << spareCapacity[x].id << endl;
							facilityResult[spareCapacity[x].id] = 1;
							break;
						}
					}
				}
				break;
			}
			case 2:{
				// 将每个设备的最后两个客户拿出来，重新分配
				// 统计每个设备的顾客
				vector<vector<int> > v(nFacility);
				// 需要改变的用户
				std::vector<int> changeCustomer;
				for (int i = 0; i < nCustomer; ++i)
				{
					v[customerResult[i]].push_back(i);
				}
				for (int i = 0; i < v.size(); ++i)
				{
					if(v[i].size() > 2){
						int temp1 = v[i].back();
						v[i].pop_back();
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity += customerVector[temp1].demand;
								break;
							}
						}
						int temp2 = v[i].back();
						v[i].pop_back();
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity += customerVector[temp2].demand;
								break;
							}
						}
						changeCustomer.push_back(temp1);
						changeCustomer.push_back(temp2);
					}
				}
				for (int k = 0; k < changeCustomer.size(); ++k)
				{
					// 采用容量best fit 
					sort(spareCapacity.begin(),spareCapacity.end());
					for(int x = 0; x < nFacility; x++){
						if(customerVector[changeCustomer[k]].demand < spareCapacity[facilitySequence[x]].capacity){
							spareCapacity[facilitySequence[x]].capacity -= customerVector[changeCustomer[k]].demand;
							customerResult[changeCustomer[k]] = spareCapacity[facilitySequence[x]].id;
							facilityResult[spareCapacity[facilitySequence[x]].id] = 1;
							break;
						}
					}
				}
				break;
			}
			case 3:{
				// 将每个设备的后面4个客户拿出来，重新分配
				// 统计每个设备的顾客
				vector<vector<int> > v(nFacility);
				// 需要改变的用户
				std::vector<int> changeCustomer;
				for (int i = 0; i < nCustomer; ++i)
				{
					v[customerResult[i]].push_back(i);
				}
				for (int i = 0; i < v.size(); ++i)
				{
					if(v[i].size() > 4){
						int temp1 = v[i].back();
						v[i].pop_back();
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity += customerVector[temp1].demand;
								break;
							}
						}
						int temp2 = v[i].back();
						v[i].pop_back();
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity += customerVector[temp2].demand;
								break;
							}
						}
						int temp3 = v[i].back();
						v[i].pop_back();
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity += customerVector[temp3].demand;
								break;
							}
						}
						int temp4 = v[i].back();
						v[i].pop_back();
						// 返还空间给设备
						for (int x = 0; x < nFacility; ++x)
						{
							if(spareCapacity[x].id == i){
								spareCapacity[x].capacity += customerVector[temp4].demand;
								break;
							}
						}
						changeCustomer.push_back(temp1);
						changeCustomer.push_back(temp2);
						changeCustomer.push_back(temp3);
						changeCustomer.push_back(temp4);
					}
				}
				for (int k = 0; k < changeCustomer.size(); ++k)
				{
					// 采用价格best fit 
					std::vector<customer_cost> vc;
					vc = customerVector[k].cost;
					sort(vc.begin(),vc.end());
					for(int x = 0; x < nFacility; x++){
						int fa_subscript = 0;
						// 找下标 
						for(int q = 0; q < nFacility; q++){
							if(vc[x].id == spareCapacity[q].id){
								fa_subscript = q;
								break;
							}
						}
						// 减容量
						if(spareCapacity[fa_subscript].capacity > customerVector[k].demand){
							spareCapacity[fa_subscript].capacity -= customerVector[k].demand;
							customerResult[changeCustomer[k]] = vc[x].id;
							facilityResult[vc[x].id] = 1;
							break;
						} 
					}
				}
				break;
			}
		}		
		t.setFacilityResult(facilityResult);
		t.setCustomerResult(customerResult);
		t.setSpareCapacity(spareCapacity);
		t.CalculateTotalCost();
		if(!t.CheckSolution()){
			t = old;
			//cout << "not change\n";
		}
	}

	// 读取文件中的数据
	void init_data(){
		int i, j;
		ifstream in("./instances/" + filePath);
		in >> nFacility >> nCustomer;
		for (i = 0; i < nFacility; i++)
		{
			facility temp;
			in >> temp.capacity >> temp.cost;
			//cout << temp.capacity << " " << temp.cost << endl;
			facilityVector.push_back(temp);
		}
		for (i = 0; i < nCustomer; i++)
		{
			customer temp;
			in >> temp.demand;
			customerVector.push_back(temp);
		}
		for (i = 0; i < nCustomer; i++)
		{
			customer temp = customerVector[i];
			float costTemp = 0;
			for (j = 0; j < nFacility; ++j)
			{
				in >> costTemp;
				customer_cost c;
				c.id = j;
				c.value = costTemp;
				customerVector[i].cost.push_back(c);
			}
		}
	}
	// 输出测试读入数据的正确性
	void test(){
		cout << "facility" << endl;
		int i, j; 
		for (i = 0; i < nFacility; i++)
		{
			facility temp = facilityVector[i];
			cout << temp.capacity << " " << temp.cost << endl;
		}
		cout << "customer" << endl;
		for (i = 0; i < nCustomer; i++)
		{
			customer temp = customerVector[i];
			cout << temp.demand << " ";
			for (j = 0; j < nFacility; ++j)
			{
				cout << temp.cost[j].value << " ";
			}
			//cout << temp.cost.size();
			cout << endl;
		}
	}
};








