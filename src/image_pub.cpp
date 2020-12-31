/*
 * @Author: wbzhang 
 * @Date: 2020-12-30 16:04:18 
 * @Last Modified by:   wbzhang 
 * @Last Modified time: 2020-12-30 16:04:18 
 */
#include<ros/ros.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<image_transport/image_transport.h>

#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<stdio.h>


using namespace cv;
using namespace std;

#define NODE_NAME "image_publisher"
string image_pub_topic,video_path,seq_path;

int main(int argc, char** argv)
{
    // ROS节点初始化
    ros::init(argc, argv, NODE_NAME);
    ros::NodeHandle n; 
    ros::Time time = ros::Time::now();
    ros::Rate loop_rate(25);
    
     // 定义节点句柄   
    image_transport::ImageTransport it(n);
    // 从参数服务器获取消息话题，如果没有则采用后面的默认值
    image_pub_topic = n.param<string>("image_pub_topic","/image_pub/rgb/image"); 
    image_transport::Publisher pub = it.advertise(image_pub_topic, 1);
    sensor_msgs::ImagePtr msg;
        
    video_path = n.param<string>("video_path","/home/wbzhang/CLionProjects/chatper3_opticalflow/vtest.avi");
    seq_path = n.param<string>("seq_path","/home/wbzhang/bags/obs6-1/");
    // opencv准备读取视频
    cv::VideoCapture video;
    video.open(video_path);  

    if( !video.isOpened() )
    {
        ROS_INFO("Read Video failed!\n");
        return 0;
    }
    Mat frame;
    int count = 0;
    while(1)
    {
        video >> frame;
        if( frame.empty() )
            break;
        count++;
        
        msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        pub.publish(msg);
        
        ROS_INFO( "read the %dth frame successfully!", count );
        loop_rate.sleep();
        ros::spinOnce();
    }
    video.release();
    
    return 0;
}

