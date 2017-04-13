#include "iostream"
#include "fstream"
//#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

void draw_graph(Mat);
void plot(Mat,char const *,char const *);
void encircle(Mat,Point);

#define con_factor 3.779528  // 1mm = 3.779528px
#define left_border int(2*100*con_factor) + 1	//now in px
#define border int(1*100*con_factor) + 1
#define height  int(26*100*con_factor) + 1
#define width int(19*100*con_factor) + 1
#define ssd 35//int(1*con_factor) + 1
//#define line_thickness 1  //1px thick line 

int main(int argc, char const *argv[])
{

	Mat graph = Mat(height+2*border,width+left_border+border,CV_8UC1,Scalar(255));

	draw_graph(graph);
	cout<<"\nGraph drawn!\nNow plotting points...";
	plot(graph,"vrd1.txt","vgs1.txt");

	cout<<"\nDone!";

	vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

	imwrite("final_graph.png",graph,compression_params);
	cout<<"\nGraph saved.\n";

	//imshow("A plot indeed!",graph);
	waitKey(0);
	return 0;

}

void draw_graph(Mat graph)
{
	int count = 0;

	for(int i=left_border;i<left_border+width;i+=ssd)
	{
		for(int j=border;j<height+border;j++)
		{
			if(count % 10==0)
			{
				for(int a=0;a<10;a++) graph.at<uchar>(j,i+a) = 0;
				/*graph.at<uchar>(j,i+1) = 0;
				graph.at<uchar>(j,i+2) = 0;
				graph.at<uchar>(j,i+3) = 0;
				graph.at<uchar>(j,i+4) = 0;
				graph.at<uchar>(j,i+5) = 0;
				graph.at<uchar>(j,i+6) = 0;
				graph.at<uchar>(j,i+7) = 0;*/
			}
			else if(count % 5==0)
			{
				for(int a=0;a<6;a++) graph.at<uchar>(j,i+a) = 0;
				//graph.at<uchar>(j,i+1) = 0;
			}
			else
			{
				for(int a=0;a<4;a++) graph.at<uchar>(j,i+a) = 0;		//the order may be confucsing but (row,col) = (height,width)
			}
			
		}
		if(count%10==0) i+=9;
		else if(count%5==0) i+=5;
		else i+=3;
		//cout<<count<<endl;
		/*"if(count>=20*10)
		{
			break;
		}		*/
		count++;
	}

	count = 0;

	for(int j=border;j<height+border;j+=ssd)
	{
		for(int i=left_border;i<width+left_border;i++)
		{
			if(count % 10==0)
			{
				for(int a=0;a<10;a++) graph.at<uchar>(j+a,i) = 0;
				/*graph.at<uchar>(j+1,i) = 0;
				graph.at<uchar>(j+2,i) = 0;
				graph.at<uchar>(j+3,i) = 0;
				graph.at<uchar>(j+4,i) = 0;
				graph.at<uchar>(j+5,i) = 0;
				graph.at<uchar>(j+6,i) = 0;
				graph.at<uchar>(j+7,i) = 0;*/
			}
			else if(count % 5==0)
			{
				for(int a=0;a<6;a++) graph.at<uchar>(j+a,i) = 0;
				//graph.at<uchar>(j+1,i) = 0;
			}
			else
			{
				for(int a=0;a<4;a++) graph.at<uchar>(j+a,i) = 0;		//the order may be confucsing but (row,col) = (height,width)
			}
		}
		count++;
		if(count%10==0) j+=9;
		else if(count%5==0) j+=5;
		else j+=3;
	}


	return;		//since the graph is a ptr , so we need not return it.
}

void plot(Mat graph,char const *f1,char const *f2)
{
	int xo = left_border + 10 + ssd*10 + 6 + 8*4 + 5 - 10; 
	int yo = border+height-3*ssd-3*4-5;  //border + 5 + 24*(10 + ssd*10 + 6 + 8*4);
	encircle(graph,Point(xo,yo));

	float x[50],y[50],id[50];
	memset(x,0,sizeof(x));
	memset(y,0,sizeof(y));
	int i = 0;

	cout<<"\nOrigin located at: "<<xo<<" &"<<yo<<endl;

	ifstream vrd,vgs;
	vrd.open(f1);
	vgs.open(f2);
	
	while(!vrd.eof())
	{
		
		vrd >> x[i];  //cout<<x[i]<<endl;
		id[i] = (x[i])/1.140;  cout<<id[i]<<endl;
		i++;
		
	}
	int idata_size = i-1;
	i = 0;

	while(!vgs.eof())
	{
		
		vgs >> y[i];  //cout<<y[i]<<endl;
		i++;

	}
	int odata_size = i-1;
	i = 0;


	if(odata_size != idata_size) 
	{
		cout<<"\n Check the data size"<<idata_size<<odata_size<<endl;
		return;
	}
	vrd.close();
	vgs.close();

	cout<<"\nNow plotting...";

	float xscale = (left_border+width-xo)/5;
	float yscale = (yo-border)/5;
	for(int a=0;a<odata_size;a++)
	{
		int xf = int(xo + xscale*y[a]);
		int yf = int(yo - yscale*id[a]);
		encircle(graph,Point(xf,yf));
	}

}

void encircle(Mat graph,Point center)
{
	int rad = 12;
	cout<<"\nEncirlling "<<center;
	circle(graph,center,rad,Scalar(0),-1,8);
}