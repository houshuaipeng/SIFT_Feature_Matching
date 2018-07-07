// opencv_empty_proj.cpp : �������̨Ӧ�ó������ڵ㡣  
//
   
#include <opencv2/opencv.hpp>  
#include <opencv2/features2d/features2d.hpp>  
#include<opencv2/nonfree/nonfree.hpp>  
#include<opencv2/legacy/legacy.hpp>  
#include<vector>  
using namespace std;  
using namespace cv;  
   
/* �������㷨
 * ���㷨��������ƥ�䣬��һ��ƥ�����ʹ��ǰ���ֵ�ƥ���㷨���ڶ���ƥ��ʱ��ʹ�õ�ƥ���㷨��ִ��˳�����һ��ƥ���˳���෴��
 * ���ڶ���ͼ���ÿ���ؼ���������һ��ͼ���ȫ���ؼ�����бȽϡ�
 * ֻ�����������϶�ƥ�䵽��ͬһ�������㣬����Ϊ��һ����Ч��ƥ��ԡ�
*/

void symmetryTest(std::vector<cv::DMatch>& matches1,
                       std::vector<cv::DMatch>& matches2,
                       std::vector<cv::DMatch>& symMatches)
{
   // ����ͼ��1��ͼ��2��ƥ��
   for (std::vector<cv::DMatch>::const_iterator matchIterator1= matches1.begin();
        matchIterator1!= matches1.end(); ++matchIterator1)
   {
       //  ����ͼ��2��ͼ��1��ƥ��
       for (std::vector<cv::DMatch>::const_iterator matchIterator2= matches2.begin();
           matchIterator2!= matches2.end(); ++matchIterator2)
       {
           // ����ƥ�����
           if (matchIterator1->queryIdx == matchIterator2->trainIdx  &&
               matchIterator2->queryIdx == matchIterator1->trainIdx)
           {
               // �������ƥ�䣬�����
               symMatches.push_back(*matchIterator1);
               break;
           }
       }
   }
}

//��ѡƥ���
vector<DMatch> chooseGood(Mat descriptor,vector<DMatch> matches)
{
	double max_dist = 0; double min_dist = 100;
	 for( int i = 0; i < descriptor.rows; i++ )
		{ double dist = matches[i].distance;
		  if( dist < min_dist ) 
			  min_dist = dist;
		  if( dist > max_dist ) 
			  max_dist = dist;
	    }
	vector<DMatch> goodMatches;
	for(int i=0;i<descriptor.rows;i++)
	{
		if(matches[i].distance<3*min_dist)
			goodMatches.push_back(matches[i]);
	}
	return goodMatches;
}
int main(int argc, char* argv[])  
{  
    //const char* imagename = "C:/Users/14518/Desktop/��ҵ���/�õ���ͼƬ/testt.jpg";  
	const char* imagename = "C:/Users/14518/Desktop/2.jpg";
    
    //���ļ��ж���ͼ��  
    Mat img = imread(imagename);  
    //Mat img2=imread("C:/Users/14518/Desktop/��ҵ���/�õ���ͼƬ/test.jpg");  
	Mat img2=imread("C:/Users/14518/Desktop/1.jpg"); 

    //��ʾͼ��  
    imshow("image before", img);  
    imshow("image2 before",img2);  
       
   
    //sift�������  
    SiftFeatureDetector  siftdtc;  
    vector<KeyPoint>kp1,kp2;  
   
    siftdtc.detect(img,kp1);  
    Mat outimg1;  
    drawKeypoints(img,kp1,outimg1);  
    imshow("image1 keypoints",outimg1);  
   
    siftdtc.detect(img2,kp2);  
    Mat outimg2;  
    drawKeypoints(img2,kp2,outimg2);   
    imshow("image2 keypoints",outimg2);  
   
   //�ڼ�⵽������������������������
    SiftDescriptorExtractor extractor;  
    Mat descriptor1,descriptor2;  
    extractor.compute(img,kp1,descriptor1);  //��һ��������
    extractor.compute(img2,kp2,descriptor2);  //�ڶ���������
   
	FlannBasedMatcher matcher;
	//BFMatcher matcher;
	//vector<Mat> train_dest_collection(1,descriptor1);
	//matcher.add(train_dest_collection);
	//matcher.train();
    
	vector<DMatch> matches1,matches2;   //�������Ӷ���
    matcher.match(descriptor1,descriptor2,matches1);  //����ƥ���
	matcher.match(descriptor2,descriptor1,matches2);
   //ƥ��
	vector<DMatch> goodMatches1,goodMatches2,symMatches;
	goodMatches1=chooseGood(descriptor1,matches1);
	goodMatches2=chooseGood(descriptor2,matches2);

	Mat img_matches;
	symmetryTest(goodMatches1,goodMatches2,symMatches);
	drawMatches(img,kp1,img2,kp2,symMatches,img_matches);
	namedWindow("matches",CV_WINDOW_NORMAL);
    imshow("matches",img_matches);  
   
    //�˺����ȴ�������������������ͷ���  
    waitKey();  
    return 0;  
}