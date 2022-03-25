#include <opencv2/opencv.hpp>            // opencv에서 지원하는 모든 기능을 포함
#include <iostream>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctime>

using namespace std;
using namespace cv;


Point ptOld, pt0, pt1;
Point ptTitleL, ptTitleR, ptContL, ptContR, ptNumL, ptNumR;
Mat settingImage;


// 파일 여는 함수
string OpenFileDialog()
{
   char name[MAX_PATH] = { 0, };               // 파일 이름을 저장할 배열
   OPENFILENAMEA ofn;                          // 파일 열기 대화상자 나타나는 구조체 정의
   ZeroMemory(&ofn, sizeof(ofn));              // ZeroMemory는 메모리 영역을 0x00으로 채우는 메크로 (함수가 아니다)
   ofn.lStructSize = sizeof(OPENFILENAMEA);    // 구조의 길이
   ofn.hwndOwner = NULL;                       // 대화 상자를 소유하는 창에 대한 핸들 (유효한 창 핸들이거나 소유자가 없다면 null)
   ofn.lpstrFilter = "모든파일(*.*)\0*.*\0";   // 파일 찾기 (필터링 없이 바로가기 대상 검색)
   ofn.lpstrFile = name;                       // 파일 이름 지정
   ofn.nMaxFile = MAX_PATH;                    // 버퍼(문자)의 크기
   ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;    // 대화 상자를 초기화하는데 사용할 수 있는 비트 플래그 집합
   // 대화 상자가 반환되면 이러한 플래그를 설정하여 사용자의 입력을 나타냄
   // OFN_EXPLORER: 열기 또는 다른 이름으로 저장 대화 상자 에 대한 모든 사용자 지정
   // OFN_FILEMUSTEXIST: 사용자는 파일 이름 입력 필드 에 기존 파일의 이름만 입력할 수 있다.
   // OFN_HIDEREADONLY: 읽기 전용 확인란을 숨김
   ofn.lpstrDefExt = "";                        // 기본 확장자 (사용자가 확장자명을 입력하지 않은 경우 이 확장자를 파일에 추가)

   string strName;

   if (GetOpenFileNameA(&ofn))                  // GetOpenFileNameA 함수: 사용자가 드라이브, 디렉터리 및 열려는 파일 또는 파일 집합의 이름을 지정할 수 있는 열기 대화 상자를 만든다
      strName = name;

   return strName;

}

void on_mouse(int event, int x, int y, int flags, void*)
{
   switch (event) {
   case EVENT_LBUTTONDOWN:
      ptOld = Point(x, y);
      pt0 = ptOld;
      cout << "EVENT_LBUTTONDOWN: " << pt0 << endl;
      circle(settingImage,pt0,5,Scalar(0, 0, 255),-1,0);
      imshow("select title", settingImage);
      break;
   case EVENT_LBUTTONUP:
      pt1 = Point(x, y);
      cout << "EVENT_LBUTTONUP: " << pt1 << endl;
      circle(settingImage,pt1,5,Scalar(0, 0, 255),-1,0);
      rectangle(settingImage, Rect(pt0, pt1), Scalar(0, 0, 255), 2);
      imshow("select title", settingImage);
      break;
   case EVENT_MOUSEMOVE:
      if (flags & EVENT_FLAG_LBUTTON) {
         ptOld = Point(x, y);
    
      }
      break;
   default:
      break;
   }
}

double check(const Mat& I1, const Mat& I2, int w1, int w2, int h1, int h2) {
   Mat st, gray, sumimage;

   //몰랐는데 mat에도 res가 있고 결과값구하는 res도 있어서 이름바꿈

   Mat img1 = I1(Range(h1, h2), Range(w1, w2));
   Mat img2 = I2(Range(h1, h2), Range(w1, w2));

   st = Mat::zeros(h2 - h1, w2 - w1, COLOR_BGR2GRAY);
   gray = Mat::zeros(h2 - h1, w2 - w1, COLOR_BGR2GRAY);
   sumimage = Mat::zeros(h2 - h1, w2 - w1, COLOR_BGR2GRAY);

   absdiff(img1, img2, st);
   cvtColor(st, gray, cv::COLOR_BGR2GRAY);
   threshold(gray, sumimage, 30, 255, THRESH_BINARY);

   Scalar total_s = sum(sumimage);

   double size = sumimage.rows * sumimage.cols;

   double result = total_s[0] * (1 / size) *(1 / 255.0);
   
   //골라야하는 임계값, 바뀌었으면 바뀐 비율 아니면 1 리턴
   if (result>0.05)
      return result;
   return 0;
}

double getDiff(const Mat& I1, const Mat& I2) {

   double xyz = 0.05; //임의로 주어진 임계값
   double r1=0;
   if (ptNumL != ptNumR) {
      r1 = check(I1, I2, ptNumL.x, ptNumR.x, ptNumL.y, ptNumR.y);
      if (r1 > xyz) {
         printf("검출 \n ");
         return r1;
      }
   } //페이지넘버가 바뀌면 당연히 바뀐거니까 제일 먼저 실행

   if (ptTitleL != ptTitleR) {
      r1 = check(I1, I2, ptTitleL.x, ptTitleR.x, ptTitleL.y, ptTitleR.y);
      if (r1 > xyz) {
         printf("검출 \n ");
         return r1;
      }
   } //타이틀부분이 바뀌면 당연히 바뀐거니까 다음으로 실행
   

   if (ptContL != ptContR) {
      ///*
      //4등분으로 계산하기
      double r1, r2, r3, r4;
      int x1, x2, x3, y1, y2, y3;

      x1 = ptContL.x;
      x2 = ptContL.x + (ptContR.x - ptContL.x)* (1.0 / 2.0);
      x3 = ptContR.x;

      y1 = ptContL.y;
      y2 = ptContL.y + (ptContR.y - ptContL.y)* (1.0 / 2.0);
      y3 = ptContR.y;

      r1 = check(I1, I2, x1, x2, y1, y2);
      r2 = check(I1, I2, x2, x3, y1, y2);
      r3 = check(I1, I2, x1, x2, y2, y3);
      r4 = check(I1, I2, x2, x3, y2, y3);

      //cout << r1<<", "<<r2<< ", " << r3<< ", " << r4<<" no" << endl;
      //요거 보면서 임계값을 고민하면 될 것같습니다

      if (r1+r2+r3+r4 > xyz*4) {
         //imshow("Before", I1); //(확인용)
         //imshow("After", I2);
         printf("검출 \n ");
         return (r1 + r2 + r3 + r4)*(1.0 / 4.0);
      }
   }
   return 0;

   /*

   
   //가로 3등분
   double r1, r2, r3;
   double x1, x2, x3, x4, y1, y2;

   x1 = ptContL.x;
   x2 = ptContL.x + (ptContR.x - ptContL.x) *(1.0 / 3.0);
   x3 = ptContL.x + (ptContR.x - ptContL.x) *(2.0 / 3.0);
   x4 = ptContR.x;
   y1 = ptContL.y;
   y2 = ptContR.y;

   r1 = check(I1, I2, x1, x2, y1, y2);
   r2 = check(I1, I2, x2, x3, y1, y2);
   r3 = check(I1, I2, x3, x4, y1, y2);

   if (r1*r2*r3 != 0) {
      printf("검출 \n ");
      return (r1 + r2 + r3)*(1.0 / 3.0);
   }
   return 0;
   


   //세로 3등분
   double r1, r2, r3;
   double x1, x2, y1, y2, y3, y4;

   x1 = ptContL.x;
   x2 = ptContR.x;
   y1 = ptContL.y;
   y2 = ptContL.y + (ptContR.y - ptContL.y) *(1.0 / 3.0);
   y3 = ptContL.y + (ptContR.y - ptContL.y) *(2.0 / 3.0);
   y4 = ptContR.y;

   r1 = check(I1, I2, x1, x2, y1, y2);
   r2 = check(I1, I2, x1, x2, y2, y3);
   r3 = check(I1, I2, x1, x2, y3, y4);
   if (r1*r2*r3 != 0) {
      printf("검출 \n ");
      return (r1 + r2 + r3)*(1.0 / 3.0);
   }
   return 0;
   */

}


void merge(const Mat& m1, const Mat& m2, Mat& result)
{
   resize(result, result, Size(m1.cols + m2.cols, m1.rows));

   m1.copyTo(result(Rect(0, 0, m1.cols, m2.rows)));
   m2.copyTo(result(Rect(m1.cols, 0, m2.cols, m2.rows)));
}

//-----------------------------------------추가 부분-----------------------------------------

void print_options(Mat&settingImage,String mystr,Point pt){
   putText(settingImage, mystr, pt, 1, 1.5, Scalar(100, 150, 20), 2, 4);
   imshow("select title",settingImage);
}
//-------------------------------------------------------------------------------------------

int main()
{
   //c가 char인 이유를 모르겠어서 int로 변경했읍
   int c;
   int frameNum = -1;
   int pix;
   string videoPath = OpenFileDialog();
   //string videoPath = "onlyPpt1.mp4";
   char buf[256];
   int index = 0;
   int flag = 0;
   int width, height;

   clock_t start = clock();
   // 프로그램 수행시간 부터 현재까지의 시간

   VideoCapture cap(videoPath);

   if (!cap.isOpened()) {
      cout << "Could not open video - " << videoPath << endl;
      return -1;
   }

   //수정한 부분, 처음에 볼 떄 너무 넓게 보는 것 같아서 보통 강의시작 제목은 중앙에 몰려있으니까 중앙부분만 확인하는 방면으로 했습니다
   width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
   height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);

   Size s = Size(width, height);
   
   Mat prevFrame, currFrame, changeFrame, result(s, CV_8UC3);
   ptContL = Point(width*(1.0/4.0), height*(1.0/4.0)); //가로반 세로반만 보는거임
   ptContR = Point(width*(3.0/4.0), height*(3.0/4.0));

   // int frameNum1=0;
   // while(1){
   //     ++frameNum1;
   //     cap >> currFrame;

   //     if(frameNum1 < 1){
   //         prevFrame = currFrame.clone();
   //         continue;
   //     }
   //     if (currFrame.rows == 0 && currFrame.cols == 0) break;

   //     if(frameNum1==1200){
   //         prevFrame = currFrame.clone();
   //         cout << "강의자료 타이틀 부분 영역을 드래그하세요." << endl;
   //         imshow("select title", prevFrame);
   //         // break;
   //     }
   //     // if (frameNum % 12 == 0)
   //     //  prevFrame = currFrame.clone();
   //     // cout << frameNum1 ;
   //     // imshow("test", currFrame);
   //     c = (char)waitKey(1);
   //     if (c == 27)       // ESC 누르면 꺼짐
   //         break;
   // }
   //namedWindow("Scene Change Detection");
   //resizeWindow("Scene Change Detection", s.width * 2, s.height);

   while (1)
   {
      ++frameNum;
      cap >> currFrame;

      if (frameNum < 1)
      {
         prevFrame = currFrame.clone();
         changeFrame = currFrame.clone();
         //  sprintf_s(buf, "C:\\\\img\\\\khk_220323_5_img_%06d.jpg", index);
         //  imwrite(buf, changeFrame);
         cout << index << endl;
         index++;
         // clone 함수: 복사본 만든다
         continue;
      }
      if (currFrame.rows == 0 && currFrame.cols == 0)
         break;
      if (frameNum % 12 != 0)
         continue;

      //——————————————————————————————————
      //사용자가 좌표 입력하는 부분
      if (index == 2 && flag < 4) {
         char c;
         String mystr="";
         Point pt_Option(30,30), pt_Option2(30,50); 
         settingImage = currFrame.clone();
         namedWindow("select title");
         setMouseCallback("select title", on_mouse);
         imshow("select title", settingImage);


         if (flag == 0) {
            // inputOptions();
            mystr="Drag a title section and Enter any key.";
            print_options(settingImage, mystr,pt_Option);
            cout << "강의자료 타이틀 부분 영역을 드래그하세요." << endl;
            cout << "드래그 후 아무키 입력" << endl;
            cout << "없다면 드래그하지 않고 아무키 입력" << endl;

            c = waitKey(0);
            cout << c << endl;
            flag++;
            if (c != -1) {
               cout << "다음" << flag << endl;
            }
            ptTitleL = pt0;
            ptTitleR = pt1;
            cout << ptTitleL << ptTitleR << endl;
            continue;
            //continue없이 else if문으로 돌리니까 다음프레임에서 한 번 더 받아오는 형태여서 변경함
            //pt도 게 자리에서 잘 받아져서 제 자리는 제 자리로 입력해줌
         }
         if (flag == 1) {
            mystr="Drag a Content section and Enter any key.";
            print_options(settingImage, mystr,pt_Option);  
            cout << "[필수] 강의자료 내용 부분 영역을 드래그하세요." << endl;
            cout << "드래그 후 아무키 입력" << endl;
            cout << "없다면 드래그하지 않고 아무키 입력" << endl;

            c = waitKey(0);
            flag++;
            if (c != -1) {
               cout << "다음" << flag << endl;
            }
            ptContL = pt0;
            ptContR = pt1;
            cout << ptContL << ptContR << endl;
            continue;
            //continue없이 else if문으로 돌리니까 다음프레임에서 한 번 더 받아오는 형태여서 변경함
            //pt도 게 자리에서 잘 받아져서 제 자리는 제 자리로 입력해줌
         }
         if (flag == 2) {
            char a;
            mystr="Is the number of pages written on the PPT? [y/n] : ";
            print_options(settingImage, mystr,pt_Option);
            cout << "강의자료에 쪽수가 나타나있습니까? [y/n]" << endl;
            a = waitKey(0);
            mystr+=a;
            print_options(settingImage, mystr,pt_Option);
            //cin 쓰니까 나는 자꾸 응답없음 뜨고 에러나서 찾아보고 이렇게 변경했습니다
            if (a == 'y' || a == 'Y')
            {
               mystr="Drag the number of pages and Enter any key.";
               print_options(settingImage, mystr,pt_Option2);
               cout << "강의자료 쪽수 부분 영역을 드래그하세요." << endl;
               cout << "드래그 후 아무키 입력" << endl;
               c = waitKey(0);
               flag++;
               if (c != -1) {
                  cout << "설정종료" << flag << endl;
               }
               ptNumL = pt0;
               ptNumR = pt1;
               cout << ptNumL << ptNumR << endl;
               continue;
            }
            else if (a == 'n' || a == 'N') {
               mystr="Enter any key.";
               print_options(settingImage, mystr,pt_Option2);
               cout << "없다면 드래그하지 않고 아무키 입력" << endl;
               ptNumL = Point(0, 0);
               ptNumR = Point(0, 0);
               c = waitKey(0);
               if (c != -1) {
                  cout << "설정종료" << flag << endl;
               }
               flag++;
               continue;
            }
            else {
               cout << "다시 입력해주세요" << endl;
               continue;
            }
            //혹시나 잘못입력해서 2개 중 아무것도 입력안해서 튕겨서 열받아서 예외처리했습니다
         }
         if (flag == 3) {
            flag++;
            destroyWindow("select title");
         }

         //굳이 flag를 하나 더 둘 이유가 없어보여서 없앰


         //   imshow("select title", settingImage);
         //   waitKey(0);
      }


      double res = getDiff(prevFrame, currFrame);

      //cout << res << endl;

      if (res != 0) {
         changeFrame = currFrame.clone();
         cout << "차이: " << res << endl;
         //  sprintf_s(buf, "C:\\\\img\\\\khk_220323_5_img_%06d.jpg", index);
         //  imwrite(buf, changeFrame);
         cout << index << endl;
         index++;
      }

//      merge(currFrame, changeFrame, result);


      if (frameNum % 2 == 0) //frameNum이 짝수일때만
         prevFrame = currFrame.clone(); // prevFrame에 currFrame 복사해넣음

      c = waitKey(1);
      if (c == 27) //Esc
         break;

      if (frameNum % 12 == 0)
         prevFrame = currFrame.clone();

      c = (char)waitKey(1);
      if (c == 27)       // ESC 누르면 꺼짐
         break;
   }

   printf("%0.5f\n", (float)(clock() - start) / CLOCKS_PER_SEC);


   return 0;


}