// opencv_empty_proj.cpp : 定义控制台应用程序的入口点。  
//
   
#include <opencv2/opencv.hpp>  
#include <opencv2/features2d/features2d.hpp>  
#include<opencv2/nonfree/nonfree.hpp>  
#include<opencv2/legacy/legacy.hpp>  
#include<vector>  
using namespace std;  
using namespace cv;  
   
/* 交叉检查算法
 * 该算法进行两次匹配，第一次匹配可以使用前两种的匹配算法，第二次匹配时，使用的匹配算法的执行顺序与第一次匹配的顺序相反，
 * 将第二幅图像的每个关键点逐个与第一幅图像的全部关键点进行比较。
 * 只有两个方向上都匹配到了同一对特征点，才认为是一个有效的匹配对。
*/

void symmetryTest(std::vector<cv::DMatch>& matches1,
                       std::vector<cv::DMatch>& matches2,
                       std::vector<cv::DMatch>& symMatches)
{
   // 遍历图像1到图像2的匹配
   for (std::vector<cv::DMatch>::const_iterator matchIterator1= matches1.begin();
        matchIterator1!= matches1.end(); ++matchIterator1)
   {
       //  遍历图像2到图像1的匹配
       for (std::vector<cv::DMatch>::const_iterator matchIterator2= matches2.begin();
           matchIterator2!= matches2.end(); ++matchIterator2)
       {
           // 进行匹配测试
           if (matchIterator1->queryIdx == matchIterator2->trainIdx  &&
               matchIterator2->queryIdx == matchIterator1->trainIdx)
           {
               // 若是最好匹配，则加入
               symMatches.push_back(*matchIterator1);
               break;
           }
       }
   }
}

//优选匹配点
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
    //const char* imagename = "C:/Users/14518/Desktop/毕业设计/用到的图片/testt.jpg";  
	const char* imagename = "C:/Users/14518/Desktop/2.jpg";
    
    //从文件中读入图像  
    Mat img = imread(imagename);  
    //Mat img2=imread("C:/Users/14518/Desktop/毕业设计/用到的图片/test.jpg");  
	Mat img2=imread("C:/Users/14518/Desktop/1.jpg"); 

    //显示图像  
    imshow("image before", img);  
    imshow("image2 before",img2);  
       
   
    //sift特征检测  
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
   
   //在检测到的特征点上生成特征描述符
    SiftDescriptorExtractor extractor;  
    Mat descriptor1,descriptor2;  
    extractor.compute(img,kp1,descriptor1);  //第一个描述符
    extractor.compute(img2,kp2,descriptor2);  //第二个描述符
   
	FlannBasedMatcher matcher;
	//BFMatcher matcher;
	//vector<Mat> train_dest_collection(1,descriptor1);
	//matcher.add(train_dest_collection);
	//matcher.train();
    
	vector<DMatch> matches1,matches2;   //定义连接对象
    matcher.match(descriptor1,descriptor2,matches1);  //生成匹配对
	matcher.match(descriptor2,descriptor1,matches2);
   //匹配
	vector<DMatch> goodMatches1,goodMatches2,symMatches;
	goodMatches1=chooseGood(descriptor1,matches1);
	goodMatches2=chooseGood(descriptor2,matches2);

	Mat img_matches;
	symmetryTest(goodMatches1,goodMatches2,symMatches);
	drawMatches(img,kp1,img2,kp2,symMatches,img_matches);
	namedWindow("matches",CV_WINDOW_NORMAL);
    imshow("matches",img_matches);  
   
    //此函数等待按键，按键盘任意键就返回  
    waitKey();  
    return 0;  
}