#include<iostream>
#include<bitset> 
#include<fstream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<time.h>
using namespace std;

int *order;
double **dis;
double **dp;
double ANS;

class position
{ 
	public:
		position(int n,int x,int y){a=x;b=y;num=n;}
		int geta(){return a;}
		int getb(){return b;}
		int getnum(){return num;} //city number
		void out(){cout<<a<<" "<<b<<endl;} //output
	private:
		int a,b,num;
};

double MIN(double A, double B) //找最短路徑
{
	if(A>=B) return B;
	else return A;
}

void init(int nCity) //初始化陣列
{
	/*  0:城市n不再集合內 1:城市n在集合內
	      0            1           10 110      11                          101                         111         ...111
	\j   { }  |       {1}        |{2}|{3}|...|{1,2}|                      {1,3}                 |...|{1,2,3}|...|{1,2,3,...}
	i\------------------------------------------------------------------------------------------------------------------------------
	0|    X
	1|dis(1,0)|         X        |
	2|dis(2,0)|dis(2,1)+dp[1][{}]|.......|...|  X  |min{dis(2,1)+dp[1][{}]}, dis(2,3)+dp[3][{}]}| 
	3|dis(3,0)|dis(3,1)+dp[1][{}]|
	.
	.
	.
	*/
	dp=new double*[nCity];
  	for(int i=0;i<nCity;i++)
	{
    	dp[i]=new double[(1<<nCity-1)];
    		for(int j=0;j<(1<<nCity-1);j++)
    			dp[i][j]=0; 
  	}
	//所有城市到第0個(起點)城市的距離初始化
	for(int i=1;i<nCity;i++)
		dp[i][0]=dis[i][0];
	
	order=new int[nCity]; //動態分配city順序
	for(int i=0;i<nCity;i++)
		order[i]=0; //初始化
}

void DP(int nCity) //DP[j][i]
{
	//找每一個集合(i)，由1開始
	for(int i=1;i<(1<<nCity-1);i++)
	{
		//建立dp
		for(int j=1;j<nCity;j++) //每個city(j)
		{
			if((i&(1<<j-1))==0) //尋找不在集合中的city
			{
				dp[j][i]=10000; //尋找dp最小值

				for(int k=1;k<nCity;k++) //每個集合裡的city
				{
					if((1<<k-1)&i) //找city有沒有在集合裡，1<<k-1轉換成二進位
						dp[j][i]=MIN(dis[j][k]+dp[k][i-(1<<k-1)],dp[j][i]); //從集合中移除當前元素:i-(1 << k-1)
				}
			}
			
		}
	}
	//固定起點(0)經過每個點的距離最小值 dp[0][{1,2,3...nCity}] 
	dp[0][(1<<nCity-1)-1]=1000;
	int N=(1<<nCity-1)-1; //集合數量，換成二進位
	for(int j=1;j<nCity;j++)
		dp[0][N]=MIN(dis[0][j]+dp[j][N-(1<<j-1)],dp[0][N]);

	ANS=dp[0][N]; //存找到的距離
}

void TSP(int nCity) //記錄走的順序(最小值的距離)
{
	double tmp=1000; //tmp=dis(i,j)+dp[j][{}]
	int N=(1<<nCity-1)-1; //集合數量，換成二進位
	double min=1000; //最小值距離
	for(int k=1; k<nCity; k++) //經過city順序
	{
		int flag=0;
		
		for(int j=1;j<nCity;j++)
		{
			if((N&(1<<j-1))) //尋找在集合中的city
			{
				if(flag==0)
				{
					//從集合中移除當前元素:N-(1<<j-1) 
					tmp=dis[order[k-1]][j]+dp[j][N-(1<<j-1)]; //算tmp
					min=tmp; //找到最小值的tmp
					flag=1;
				}
				
				tmp=dis[order[k-1]][j]+dp[j][N-(1<<j-1)];

				if(tmp<=min)
				{
					min=tmp;
					order[k]=j; //紀錄最小值的city，經過city的順序	
				}
			}	
		}
		N=N-(1<<order[k]-1); //從集合中移除當前元素:N-(1<<j-1) 
	}
}

int main()
{
	int i, j;
	int tmp1, tmp2, tmp3, nCity; //tmp1:city tmp2:x tmp3:y 
	vector<position> city;  //建立vector
	char v[100];
	cout<<"Input file: ";
	cin>>v; //file name
	ifstream fptr; //read file
	fptr.open(v); //open file

	while(fptr>>tmp1) //readfile
	{
		fptr>>tmp2>>tmp3;
		position tmp(tmp1,tmp2,tmp3);
		city.push_back(tmp); //將檔案內容存入vector
	}
	fptr.close(); //close file
	
	//計算每個城市之間的距離並記錄
	nCity=city.size(); //count number of city in file
	dis=new double*[nCity];		
	for(i=0;i<nCity;i++)
	{
		dis[i]=new double[nCity];
		for(j=0;j<nCity;j++) // 計算每兩個城市之間的距離
			dis[i][j]=(double)sqrt(fabs(pow(city[i].geta()-city[j].geta(),2)+pow(city[i].getb()-city[j].getb(),2)));
	}
	
	init(nCity);
	//count time
	clock_t start, end;
	start=clock();
	DP(nCity);
	TSP(nCity);
    end=clock();
	
	//output
    cout<<"Best Visit Order : ";
    cout<<city[order[0]].getnum()<<" ";
	for(int i=nCity-1;i>0;i--)
        cout<<city[order[i]].getnum()<<" ";
	cout<<endl<<"Best Distance : "<<ANS<<endl;
	cout<<"Execution Time: "<<(double)(end-start)/CLOCKS_PER_SEC << "(s)\n";
	
	ofstream fout("output.txt"); //output file

	if(fout) //output.txt 輸出內容
	{
		//fout<<"City  X    Y"<<endl;
		fout<<setw(3)<<city[order[0]].getnum()<<setw(5)<<city[order[0]].geta()<<setw(4)<<city[order[0]].getb()<<endl;
		for(j=nCity-1;j>0;j--)
			fout<<setw(3)<<city[order[j]].getnum()<<setw(5)<<city[order[j]].geta()<<setw(4)<<city[order[j]].getb()<<endl;
		
		fout<<endl<<"Best Visit Order : ";
    	fout<<city[order[0]].getnum()<<" ";
		for(int i=nCity-1;i>0;i--)
        	fout<<city[order[i]].getnum()<<" ";
		fout<<endl<<"Best Distance : "<<ANS<<endl;
		fout<<"Execution Time: "<<(double)(end-start)/CLOCKS_PER_SEC << "(s)\n";
		
		fout.close();
	}
	//graph
/*
	ofstream out("graph.txt");

	if(out)
	{
		out<<setw(3)<<city[order[0]].getnum()<<setw(5)<<city[order[0]].geta()<<setw(4)<<city[order[0]].getb()<<endl;
		for(j=nCity-1;j>=0;j--)
			out<<setw(3)<<city[order[j]].getnum()<<setw(5)<<city[order[j]].geta()<<setw(4)<<city[order[j]].getb()<<endl;
		out.close();
	}
	//plot 'graph.txt' using 2:3 with linespoints notitle, \
     '' using 2:3:(sprintf("(%d,%d)",$1,$2)) with labels   
*/	
	return 0;
}