#include <bits/stdc++.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#define division 64
#define SIZ 64

int rows,columns;

struct trie
{
	int counter;
	std::vector< pair<string,struct trie*> > nodeList;
	trie()
	{
		counter=0;
		nodeList.clear();
	}
};

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
{
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


 Mat block1(img1,Rect(x1-1,y1+SIZ-1,SIZ,SIZ));
 Mat block2(img2,Rect(x2-1,y2+SIZ-1,SIZ,SIZ));

 namedWindow( "Window1",WINDOW_AUTOSIZE);
 imshow("Window1",block1);

 namedWindow( "Window2",WINDOW_AUTOSIZE);
 imshow("Window2",block2);

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
        if((int)bgrArr1[i][j](0)==(int)bgrArr2[i][j](0) && (int)bgrArr1[i][j](1)==(int)bgrArr2[i][j](1) && (int)bgrArr1[i][j](2)==(int)bgrArr2[i][j](2)){
        	count++;
        }
       
 	}
 }
 float match=((float)count/(SIZ*SIZ))*100;
 if(match>=perc)
 	return true;
 else false;
}

void addToTrie(struct trie *root,string imagePath,int blockNumber)
{
cout<<blockNumber<<endl;
	if(!isBlockPresent(blockNumber,imagePath))
	{	
		root->counter++;
		return ;
	}
	else
	{
		if((root->nodeList).empty())
		{
			struct trie temp ;
			struct trie * tempaddr=&temp;
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
			struct trie temp ;
			struct trie * tempaddr=&temp;
			root->nodeList.push_back(make_pair(imagePath,tempaddr));
			addToTrie(tempaddr,imagePath,blockNumber+1);	
		}
		return;
	}

}

void query(struct trie* root,string img,int blockNumber){
	if(!isBlockPresent(blockNumber,img))
	{
		cout<<root->counter<<endl;
		return ;
	}

	for(int i=0;i<(int)(root->nodeList.size());i++)
			{
				if(compareBlocks(root->nodeList[i].first,imagePath,blockNumber,100))
				{
					 query(root->nodeList[i].second,imagePath,blockNumber+1);
					return ;
				}
			}
	        cout<<0<<endl;	
	return ;
}

int main(int argc, char const *argv[])
{  
	struct trie root;
	struct trie * rootaddr=&root;
	// cout<<root.nodeList.empty()<<endl;
	addToTrie(rootaddr,"image1.jpg",1);
	addToTrie(rootaddr,"image2.jpg",1);
	return 0;
}
