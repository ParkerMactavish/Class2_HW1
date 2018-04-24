#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstdlib>

using namespace std;

int mapping[16][2]={{0, 0}, {1, 0}, {3, 0}, {2, 0}, {0, 1}, {1, 1}, {3, 1}, {2, 1}, {0, 3}, {1, 3}, {3, 3}, {2, 3}, {0, 2}, {1, 2}, {3, 2}, {2, 2}};

int K2Mmapping[4][4]={
	{0, 4, 12, 8}, 
	{1, 5, 13, 9}, 
	{3, 7, 15, 11}, 
	{2, 6, 14, 10}};

int choose_word(char in)
{
	switch(in)
	{
		case 'a':return 8;
		case 'b':return 4;
		case 'c':return 2;
		case 'd':return 1;
	}
}

const void print_Kmap(int Kmap[4][4])
{
	cout<<	"======= K Map ========"<<endl<<
			"  \\AB|"<<endl<<
			"CD \\ | 00  01  11  10"<<endl<<
			"-----+----------------"<<endl;
	for(int i=0; i<4; i++)
	{
		switch(i)
		{
			case 0:cout<<"  00 |";break;
			case 1:cout<<"  01 |";break;
			case 2:cout<<"  11 |";break;
			case 3:cout<<"  10 |";break;
		}
		for(int j=0; j<4; j++)
		{
			cout<<' ';
			if(Kmap[i][j]>1) cout<<'X';
			else cout<<Kmap[i][j];
			cout<<' '<<'|';
		}
		cout<<endl<<"-----+---+---+---+---|"<<endl;
	}
}

void set_Kmap(int Kmap[4][4], int selected[4][4], int input[4], int type, int inputCnt)
{
	cout<<type<<' ';
	int existFlag[4]={0, 0, 0, 0};
	int initMinTerm=0;
	for(int i=0; i<inputCnt+1; i++)
	{
		switch(input[i])
		{
			case 1: existFlag[0]=1; initMinTerm+=1; break;
			case -1: existFlag[0]=1; break;
			case 2: existFlag[1]=1; initMinTerm+=2; break;
			case -2: existFlag[1]=1; break;
			case 4: existFlag[2]=1; initMinTerm+=4; break;
			case -4: existFlag[2]=1; break;
			case 8: existFlag[3]=1; initMinTerm+=8; break;
			case -8: existFlag[3]=1; break;
		}
	}
	int minTerm[16]; int minTermCnt=1;
	minTerm[0]=initMinTerm;
	for(int i=0; i<4; i++)
	{		
		if(existFlag[i]==0)
		{
			int tmpMinTermCnt=minTermCnt;
			for(int j=0; j<tmpMinTermCnt; j++)
			{
				minTerm[minTermCnt]=minTerm[j]+pow(2, i);
				minTermCnt++;
			}
		}
	}
	
	for(int i=0; i<minTermCnt; i++) cout<<minTerm[i]<<' ';
	cout<<endl;
	
	for(int i=0; i<minTermCnt; i++) 
	{
		Kmap[mapping[minTerm[i]][0]][mapping[minTerm[i]][1]]=type;
		if(type==1) selected[mapping[minTerm[i]][0]][mapping[minTerm[i]][1]]=1;
	}
}

int true_cnt(int Kmap[4][4])
{
	int tmpCnt=0;
	for(int i=0;i<4; i++)
		for(int j=0; j<4; j++)
			tmpCnt+=Kmap[i][j];
		
	return tmpCnt;
}

void print_minTerm(int minTerm[], int size)
{
	for(int i=0; i<size-1; i++) cout<<minTerm[i]<<", ";
	cout<<minTerm[size-1];
}

void sort(int arr[], int size)
{
	for(int i=0; i<size-1; i++)
	{
		for(int j=0; j<size-i-1; j++)
		{
			if(arr[j]>arr[j+1])
			{
				int tmp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=tmp;				
			}
		}
	}
}

void find_Kmap(int Kmap[4][4], int selected[4][4])
{
	int groupCnt=1;
	if(true_cnt(Kmap)==16)
	{
		int ans[16];
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				ans[i*4+j]=K2Mmapping[i][j];
		cout<<"group "<<groupCnt<<": ";
		sort(ans, 16);
		print_minTerm(ans, 16);
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				selected[i][j]=0;
		cout<<"simplification of group "<<groupCnt<<" -> "<<1;
		
		return;
	}
	{
		int ans[8];
		int testKmap[2][4];
		int testSelected[2][4];
		for(int pos=0; pos<4; pos++)
		{
			int tmpCntKmap=0;
			int tmpCntSelected=0;
			for(int row=0; row<2; row++)
			{
				for(int col=0; col<4; col++)
				{
					//cout<<(pos+row)%4<<' '<<col<<endl;
					if(Kmap[(row+pos)%4][col]>0)tmpCntKmap++;
					if(selected[(row+pos)%4][col]>0)tmpCntSelected++;
					ans[row*4+col]=K2Mmapping[(row+pos)%4][col];
				}
			}
			if(tmpCntKmap==8&&tmpCntSelected>0)
			{				
				//cout<<tmpCntKmap<<' '<<tmpCntSelected<<endl;
				cout<<"group "<<groupCnt<<": ";
				sort(ans, 8);
				print_minTerm(ans, 8);
				for(int i=0; i<2; i++)
					for(int j=0; j<4; j++)
						selected[(i+pos)%4][j]=0;
				cout<<endl<<"simplification of group "<<groupCnt<<" -> ";
				switch(pos)
				{
					case 0: cout<<"c\'";break;
					case 1: cout<<"d";break;
					case 2: cout<<"c";break;
					case 3: cout<<"d\'";break;
				}
			}
			//print_Kmap(selected);
		}
	}
	{
		int ans[8];
		int testKmap[4][2];
		int testSelected[4][2];
		for(int pos=0; pos<4; pos++)
		{
			int tmpCntKmap=0;
			int tmpCntSelected=0;
			for(int row=0; row<4; row++)
			{
				for(int col=0; col<2; col++)
				{
					//cout<<row<<' '<<(col+pos)%4<<endl;
					if(Kmap[row][(col+pos)%4]>0)tmpCntKmap++;
					if(selected[row][(col+pos)%4]>0)tmpCntSelected++;
					ans[row*2+col]=K2Mmapping[row][(col+pos)%4];
				}
			}
			if(tmpCntKmap==8&&tmpCntSelected>0)
			{				
				//cout<<tmpCntKmap<<' '<<tmpCntSelected<<endl;
				cout<<"group "<<groupCnt<<": ";
				sort(ans, 8);
				print_minTerm(ans, 8);
				for(int i=0; i<2; i++)
					for(int j=0; j<4; j++)
						selected[i][(j+pos)%4]=0;
				cout<<endl<<"simplification of group "<<groupCnt<<" -> ";
				switch(pos)
				{
					case 0: cout<<"a\'";break;
					case 1: cout<<"b";break;
					case 2: cout<<"a";break;
					case 3: cout<<"b\'";break;
				}
			}
			print_Kmap(selected);
		}
	}
	{
		int ans[4];
		int testKmap[4][1];
		int testSelected[4][1];
		for(int pos=0; pos<4; pos++)
		{
			int tmpCntKmap=0;
			int tmpCntSelected=0;
			for(int row=0; row<4; row++)
			{
				//cout<<row<<' '<<(col+pos)%4<<endl;
				if(Kmap[row][pos]>0)tmpCntKmap++;
				if(selected[row][pos]>0)tmpCntSelected++;
				ans[row]=K2Mmapping[row][pos];
			}
			if(tmpCntKmap==4&&tmpCntSelected>0)
			{				
				//cout<<tmpCntKmap<<' '<<tmpCntSelected<<endl;
				cout<<"group "<<groupCnt<<": ";
				sort(ans, 4);
				print_minTerm(ans, 4);
				for(int i=0; i<1; i++)
					for(int j=0; j<4; j++)
						selected[i][(j+pos)%4]=0;
				cout<<endl<<"simplification of group "<<groupCnt<<" -> ";
				switch(pos)
				{
					case 0: cout<<"a\'";break;
					case 1: cout<<"b";break;
					case 2: cout<<"a";break;
					case 3: cout<<"b\'";break;
				}
			}
			//print_Kmap(selected);
		}
	}
}

int main()
{
	fstream fs("test.txt",ios::in);
	int Kmap[4][4]={{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
	int selected[4][4]={{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
	int input[4];
	int inputCnt=0;
	char tmp[2]; tmp[0]=0;
	while(fs.peek()!=EOF)
	{
		fs>>tmp[1];
		if(tmp[1]=='(')
		{
			tmp[0]=0;tmp[1]=0;
		}
		else if(tmp[1]==')')
		{
			input[inputCnt]=choose_word(tmp[0]);
			/*for(int i=0; i<inputCnt+1; i++)
				cout<<input[i]<<' ';
			cout<<endl<<inputCnt<<endl;*/
			set_Kmap(Kmap, selected,input, 2, inputCnt);			
			tmp[0]=0;tmp[1]=0; inputCnt=0;
		}
		else if(tmp[1]=='+'&&tmp[0]!=0)
		{
			input[inputCnt]=choose_word(tmp[0]);
			
			/*for(int i=0; i<inputCnt+1; i++)
				cout<<input[i]<<' ';
			cout<<endl<<inputCnt<<endl;*/
			set_Kmap(Kmap, selected,input, 1, inputCnt);			
			
			tmp[0]=0;tmp[1]=0;inputCnt=0;
		}
		else if(tmp[1]=='\'')
		{
			input[inputCnt]=-choose_word(tmp[0]);			
			tmp[0]=0;tmp[1]=0;inputCnt++;
		}
		else if(tmp[0]!=0&&tmp[0]!=')'&&tmp[0]!='+')
		{
			input[inputCnt]=choose_word(tmp[0]);
			tmp[0]=tmp[1];inputCnt++;
		}
		else if(tmp[0]==0)
		{
			tmp[0]=tmp[1];tmp[1]=0;
		}
	}
	
	print_Kmap(Kmap);
	print_Kmap(selected);
	
	find_Kmap(Kmap, selected);
	
	system("PAUSE");
}