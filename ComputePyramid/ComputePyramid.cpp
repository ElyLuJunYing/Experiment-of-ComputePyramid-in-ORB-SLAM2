#include <iostream>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  cv::Mat image;
  image = cv::imread("../test.png");

  // 判断图像文件是否正确读取
  if (image.data == nullptr) {
    cerr << "图片不存在." << endl;
    return 0;
  }

  // BGR—>GRAY
  cvtColor(image,image,CV_BGR2GRAY);
  cv::imshow("origin image", image);

  // 配置参数
  int nlevels = 8;
  float scaleFactor = 1.2;
  int EDGE_THRESHOLD = 19;

  // mvScaleFactor
  vector<float> mvScaleFactor(nlevels);
  mvScaleFactor[0] = 1.0;
  for (int i=1; i<nlevels; i++)
    mvScaleFactor[i]=mvScaleFactor[i-1]*scaleFactor;

  // mvInvScaleFactor
  vector<float> mvInvScaleFactor(nlevels);
  for (int level = 0; level < nlevels; ++level){
    mvInvScaleFactor[level] = 1.0f/mvScaleFactor[level];
    cout << mvInvScaleFactor[level] << endl;
  }

  // mvImagePyramid
  vector<Mat>mvImagePyramid(nlevels); 
  for (int level = 0; level < nlevels; ++level){
    float scale = mvInvScaleFactor[level];
    Size sz(cvRound((float)image.cols*scale), cvRound((float)image.rows*scale));
    Size wholeSize(sz.width + EDGE_THRESHOLD*2, sz.height + EDGE_THRESHOLD*2);
    Mat temp(wholeSize, image.type()), masktemp;
    //mvImagePyramid[level] = temp(Rect(EDGE_THRESHOLD, EDGE_THRESHOLD, sz.width, sz.height)); //old  1) 如果下面是new那么可以注释掉

    // Compute the resized image
    if( level != 0 )
    {
      // 缩放
      //resize(mvImagePyramid[level-1], mvImagePyramid[level], sz, 0, 0, INTER_LINEAR); //old 2) 
      resize(image, mvImagePyramid[level], sz, 0, 0, INTER_LINEAR); //new 更清楚 2)

      copyMakeBorder(mvImagePyramid[level], temp, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD, BORDER_REFLECT_101+BORDER_ISOLATED);            
      /*
      imshow("temp",temp);
      cv::waitKey(0);
       */
    }
    else{
      copyMakeBorder(image, temp, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD, BORDER_REFLECT_101);
    /*
    imshow("temp",temp);
    cv::waitKey(0);
    */
    }
    
    mvImagePyramid[level] = temp; //new 3)
  }

  // show
  for (int level = 0; level < nlevels; level++){
    string name = to_string(level+1)+"层金字塔  "+to_string(mvImagePyramid[level].cols)+"*"+to_string(mvImagePyramid[level].rows);
    imshow(name,mvImagePyramid[level]);
  }

  // write
  //string save_name = "../old/"+to_string(nlevels)+"层金字塔  "+to_string(mvImagePyramid[nlevels-1].cols)+"*"+to_string(mvImagePyramid[nlevels-1].rows)+".png"; //old 4)
  string save_name = "../new/"+to_string(nlevels)+"层金字塔  "+to_string(mvImagePyramid[nlevels-1].cols)+"*"+to_string(mvImagePyramid[nlevels-1].rows)+".png";  //new 4)
  imwrite(save_name,mvImagePyramid[nlevels-1]);
  
  waitKey(0);
  return 0;
}
