#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<time.h>
//#include<string.h>

using namespace std;
//�إߵ{�������� 
class position
{
	private:
		int a,b,num; 
	public:
		position(int n,int x,int y){
			a=x;b=y;num=n;
		}
		int geta(){
			return a;
		}
		int getb(){
			return b;
		}
		int getnum(){
			return num; //city number 
		}
		void out(){
			cout<<a<<" "<<b<<endl;
		}
};

int main(){
	int i,j;
	int tmp1,tmp2,tmp3; //tmp1:city tmp2:x tmp3:y 
	vector<position> city;  
	//string input;
	char v[100];
	cin>>v;
	ifstream fptr; //�ϥΪ̿�J 
	fptr.open(v);
	while(fptr>>tmp1){ //readfile
		fptr>>tmp2>>tmp3;
		position tmp(tmp1,tmp2,tmp3);
		city.push_back(tmp); //�N��Ʃ�Jvector�x�s
	}
	fptr.close();
	// �ΰ}�C�s��ӫ������Z�� 
	float distance[city.size()][city.size()];		
	for(i=0;i<city.size();i++){
		for(j=0;j<city.size();j++){
			distance[i][j]=(float)sqrt(fabs(pow(city[i].geta()-city[j].geta(),2)+pow(city[i].getb()-city[j].getb(),2)));
		}
	}
	
	int ans[city.size()],a[city.size()],size=1;
	float sum,min=1000.000;
	for(i=0;i<city.size();i++){//�ⶥ�� 
		a[i]=i;
		if(i>0)
			size=size*i;	
	}//assume the first city is 1 
		
	clock_t start, end;
	start = clock();//count time
	for(i=0;i<=size;i++){//i���`�@�ƦC���Ӽ� �� 10�I
    	sum=0;
        for(j=1;j<city.size();j++){
        	sum+=distance[a[j]][a[j-1]];
		}        	
		sum+=distance[a[j-1]][a[0]]; //�̫᫰����_�I    
        if(min>sum){
        	for(j=0;j<city.size();j++){
        		ans[j]=a[j];
			}
			min=sum;
		}
        next_permutation(a,a+city.size());// ���禡�O��o11�y�������ƦC�զX
    }
    end = clock();
    //output
    cout<<"Best Visit Order : ";
	for(j=0;j<city.size();j++){
        cout<<city[ans[j]].getnum()<<" ";
	}
	cout<<endl<<"Best Distance : "<<min<<endl;
	cout<<"Execution Time: "<<(double)(end-start)/CLOCKS_PER_SEC<<"(s)\n";
	
	ofstream fout("output.txt");
	if(fout){
		//fout<<"City  X    Y"<<endl;
		for(j=0;j<city.size();j++)
			fout<<setw(3)<<city[ans[j]].getnum()<<setw(5)<<city[ans[j]].geta()<<setw(4)<<city[ans[j]].getb()<<endl;
		fout.close();
	}
	//plot 'output.txt' using 2:3 with linespoints notitle, \
     '' using 2:3:(sprintf("(%d,%d)",$1,$2)) with labels    
} 


