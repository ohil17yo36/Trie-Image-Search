#include <bits/stdc++.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <omp.h>
#include <cmath>
#include <sys/time.h>
using namespace std;
using namespace cv;

#define division 64
#define SIZ 64
#define DIFF 10

int rows,columns;

struct trie
{
	int counter;
	std::vector< pair<string,struct trie*> > nodeList;
	
};

struct trie* newNode()
{
	struct trie* temp=(struct trie*)calloc(1,sizeof(struct trie));
	temp->counter=0;
	temp->nodeList.clear();
	return temp;
}
bool isBlockPresent(int blockNumber,string image1)
{
 Mat img1=imread(image1,CV_LOAD_IMAGE_COLOR);
 if(!img1.data )                              // Check for invalid input
 {
 	cout <<  "Could not open or find the image" << std::endl ;
 	return false;
 }
 int imgrows=img1.rows;
 int imgcols=img1.cols;

 if(((imgrows*imgcols)/(SIZ*SIZ))>=blockNumber)
 	return true;
 else
 	return false;
}


bool compareBlocks(string image1,string image2,int blockNumber,int perc)
{//cout<<blockNumber<<" "<<image1<<" "<<image2<<" "<<endl;
	Mat img1=imread(image1,CV_LOAD_IMAGE_COLOR);
 if(!img1.data )                              // Check for invalid input
 {
 	cout <<  "Could not open or find the image" << std::endl ;
 	return false;
 }
 Mat img2=imread(image2,CV_LOAD_IMAGE_COLOR);
 if(!img2.data )                              // Check for invalid input
 {
 	cout <<  "Could not open or find the image" << std::endl ;
 	return false;
 }
 int rows1=img1.rows;
 int cols1=img1.cols;
 
 int tcols1=cols1/SIZ;
 int trows1=rows1/SIZ;
 int x1=(blockNumber%tcols1)==0?tcols1:(blockNumber%tcols1);
 x1=SIZ*(x1-1)+1;
 int y1=(blockNumber%trows1)==0?trows1:(blockNumber%trows1);
 y1=SIZ*(y1-1)+1;
 
 int rows2=img2.rows;
 int cols2=img2.cols;
 
 int tcols2=cols2/SIZ;
 int trows2=rows2/SIZ;
 int x2=(blockNumber%tcols2)==0?tcols2:(blockNumber%tcols2);
 x2=SIZ*(x2-1)+1;
 int y2=(blockNumber%trows2)==0?trows2:(blockNumber%trows2);
 y2=SIZ*(y2-1)+1;


 Mat block1(img1,Rect(x1-1,y1-1,SIZ,SIZ));
 Mat block2(img2,Rect(x2-1,y2-1,SIZ,SIZ));

 //namedWindow( "Window1",WINDOW_AUTOSIZE);
 //imshow("Window1",block1);

 //namedWindow( "Window2",WINDOW_AUTOSIZE);
 //imshow("Window2",block2);

 Vec3b bgrArr1[SIZ][SIZ],bgrArr2[SIZ][SIZ];

 for(int i=0;i<SIZ;i++){
 	for(int j=0;j<SIZ;j++){
        bgrArr1[i][j]=block1.at<Vec3b>(i,j);
 	    bgrArr2[i][j]=block2.at<Vec3b>(i,j);
 	}
 }
 
 int count=0;
 for(int i=0;i<SIZ;i++){
 	for(int j=0;j<SIZ;j++){
        if(abs((int)bgrArr1[i][j](0)-(int)bgrArr2[i][j](0))<=DIFF && abs((int)bgrArr1[i][j](1)-(int)bgrArr2[i][j](1))<=DIFF && abs((int)bgrArr1[i][j](2)-(int)bgrArr2[i][j](2))<=DIFF){
        	count++;
        }
       
 	}
 }
 float match=((float)count/(SIZ*SIZ))*100;
 if(match>=(float)perc)
 	return true;
 else {return false;}
}

void addToTrie(struct trie *root,string imagePath,int blockNumber)
{  
	if(!isBlockPresent(blockNumber,imagePath))
	{	
		struct trie * temp;     //not necessary ,just for make_pair
		root->nodeList.push_back(make_pair(imagePath,temp));
		root->counter++;
		return ;
	}
	else
	{
		if((root->nodeList).empty())
		{
			struct trie * tempaddr=newNode();
			root->nodeList.push_back(make_pair(imagePath,tempaddr));
			addToTrie(tempaddr,imagePath,blockNumber+1);	
		}
		else
		{
			for(int i=0;i<(int)(root->nodeList.size());i++)
			{
				if(compareBlocks(root->nodeList[i].first,imagePath,blockNumber,100))
				{
					addToTrie(root->nodeList[i].second,imagePath,blockNumber+1);
					return ;
				}
			}
			
			struct trie * tempaddr=newNode();
			root->nodeList.push_back(make_pair(imagePath,tempaddr));
			addToTrie(tempaddr,imagePath,blockNumber+1);	
		}
	}
	return;

}
void query(struct trie* root,string imagePath,int blockNumber,int perc){
	if(!isBlockPresent(blockNumber,imagePath))
	{
		//cout<<root->counter<<endl;
		for(int i=0;i<(int)root->nodeList.size();i++){
			cout<<root->nodeList[i].first<<" ";
		}
		return ;
	}

    #pragma omp parallel for
	for(int i=0;i<(int)(root->nodeList.size());i++)
	{
		if(compareBlocks(root->nodeList[i].first,imagePath,blockNumber,perc))
		{
			 query(root->nodeList[i].second,imagePath,blockNumber+1,perc);
		}
	}
	return ;
}
int main(int argc, char const *argv[])
{  
	struct trie * rootaddr=newNode();

	struct timeval TimeValue_Start;
	struct timezone TimeZone_Start;
	struct timeval TimeValue_Final;
	struct timezone TimeZone_Final;
	long   time_start, time_end;
    double time_overhead;
	clock_t begin,end;
	double timeSpent;

	gettimeofday(&TimeValue_Start, &TimeZone_Start);
    #pragma omp parallel for
	for(int i=0;i<100;i++){
	string str="./images2/"+to_string(i)+".jpg";
	cout<<"Adding "<<str<<" to trie"<<endl;
	struct trie* temp=rootaddr;	
	addToTrie(temp,str,1);	
	}
	gettimeofday(&TimeValue_Final, &TimeZone_Final);
	time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
	time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
	time_overhead = (time_end - time_start)/1000000.0;
	printf("Parallel Computation time %lf\n",time_overhead);

	cout<<"Trie created"<<endl;
	getchar();
	//addToTrie(rootaddr,"image1.jpg",1);addToTrie(rootaddr,"image2.jpg",1);addToTrie(rootaddr,"image1.jpg",1);addToTrie(rootaddr,"image2.jpg",1);addToTrie(rootaddr,"image1.jpg",1);addToTrie(rootaddr,"image2.jpg",1);
	gettimeofday(&TimeValue_Start, &TimeZone_Start);
	

	
	for(int i=45;i<=45;i++){
	 string str="./images2/"+to_string(i)+".jpg";
	 cout<<"Quering the string "<<str <<" : ";
	 query(rootaddr,str,1,2);
	 cout<<endl;
	}
	gettimeofday(&TimeValue_Final, &TimeZone_Final);
	time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
	time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
	time_overhead = (time_end - time_start)/1000000.0;
	printf("Parallel Computation time to query %d images is %lf\n",100,time_overhead);	
	return 0;
}