// 
// 1 - obj name
// 2 - obj x
// 3 - obj y
// 4 - obj z
// 5 - maximum displacement
// 6 - speed
// 7 - node loop rate
// 

#include "ros/ros.h"
#include <gazebo_msgs/SetModelState.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/ModelState.h>
#include <geometry_msgs/Point.h>
#include <ctime>
#include <stdlib.h>
#include <string.h>

geometry_msgs::Point obstacle_coords;
geometry_msgs::Point origin;
std::string model_name;
float frame_displacement = 0.0;
float maximum_displacement = 0; // meters
double speed = 0.1; // м/c
ros::ServiceClient client;

std::vector<double_t> time_buffer;


// geometry_msgs::Point frame_position_calculator(std::vector<uint32_t> time_buffer_in, double speed, double current_x, double current_y, double current_z, double maximum_displacement)
// {
//     geometry_msgs::Point frame_position;

//     frame_position.x = current_x + speed * (time_buffer.at(0) - time_buffer.at(1));
//     frame_position.y = current_y + speed * (time_buffer.at(0) - time_buffer.at(1));
//     frame_position.z = current_z + speed * (time_buffer.at(0) - time_buffer.at(1));
//     return frame_position;
// }

void _init_time_buffer(std::vector<double> &time_buffer_in)
{
    std::vector<double> time_buffer;
    time_buffer.resize(2);
    if (time_buffer_in.size() != 2)
    {
        time_buffer_in.resize(2);
    }

    time_buffer.at(0) = time_buffer_in.at(0);
    time_buffer.at(1) = time_buffer_in.at(1);

    if(time_buffer.at(0) == 0 && time_buffer.at(1) == 0)
    {
        time_buffer.at(0) = ros::Time::now().toSec();
        time_buffer.at(1) = ros::Time::now().toSec();
    ROS_INFO_STREAM("Debug flag n9");
    }

    time_buffer_in = time_buffer;
}

void oscilate_by_x(std::vector<double_t> time_buffer_in, double &speed, double origin_x, double origin_y, double origin_z, double maximum_displacement)
{
    if(speed >= 0)
    {
        if(frame_displacement < maximum_displacement)
        {
            frame_displacement = frame_displacement + speed * (time_buffer.at(0) - time_buffer.at(1));
            obstacle_coords.x = origin_x + frame_displacement;
            ROS_INFO_STREAM("Debug flag n5");
        }
        else
        {
            speed = -speed;
            frame_displacement = frame_displacement + speed * (time_buffer.at(0) - time_buffer.at(1));
            obstacle_coords.x = origin_x + frame_displacement;
            ROS_INFO_STREAM("Debug flag n6");
        }
        ROS_INFO_STREAM("Debug flag n3");
    }
    else if (speed < 0)
    {
        if(frame_displacement > -maximum_displacement)
        {
            frame_displacement = frame_displacement + speed * (time_buffer.at(0) - time_buffer.at(1));
            obstacle_coords.x = origin_x + frame_displacement;
            ROS_INFO_STREAM("Debug flag n7");
        }
        else
        {
            speed = -speed;
            frame_displacement = frame_displacement + speed * (time_buffer.at(0) - time_buffer.at(1));
            obstacle_coords.x = origin_x + frame_displacement;
            ROS_INFO_STREAM("Debug flag n8");
        }
        ROS_INFO_STREAM("Debug flag n4");
    }

    //DEBUG rinfo
    //****************************************************************************************************
    if(1)
    {
        ROS_INFO_STREAM(std::endl << "_________________________________" << std::endl << "_________________________________" << std::endl 
        << "FUNCTION NAME: oscilate_by_x" << std::endl 
        << "VARIABLES: " << std::endl 
        << "speed in moment -->" << speed * (time_buffer.at(0) - time_buffer.at(1)) << std::endl 
        << "frame_displacement -->" << frame_displacement << std::endl 
        << "Frame pose x -->" << obstacle_coords.x << std::endl 
        << "time_buffer 0 -->" << time_buffer.at(0) << std::endl 
        << "time_buffer 1 -->" << time_buffer.at(1) << std::endl 
        << "_________________________________" << std::endl << "_________________________________" << std::endl);
    }
    //****************************************************************************************************

    obstacle_coords.y = origin_y;
    obstacle_coords.z = origin_z;

    // ROS_INFO_STREAM("Debug flag n2");
}


void logic()
{
    _init_time_buffer(time_buffer);
    time_buffer.at(1) = time_buffer.at(0);
    time_buffer.at(0) = ros::Time::now().toSec();

    oscilate_by_x(time_buffer, speed, origin.x, origin.y, origin.z, maximum_displacement);

    gazebo_msgs::SetModelState model_state_srv;

    model_state_srv.request.model_state.model_name = model_name;
    model_state_srv.request.model_state.pose.position = obstacle_coords;
    model_state_srv.request.model_state.reference_frame = "world";

    // geometry_msgs::TransformStamped transformStamped; // Создание объекта трансформации

    // transformStamped.header.stamp = ros::Time::now();
    // transformStamped.header.frame_id = "world"; // задается фрейм-отец
    // transformStamped.child_frame_id = "obstacle_1"; // задается фрейм-ребенок
    // transformStamped.transform.translation.x = obstacle_coords.x; // Задаются координаты смещения нового фрейма относительно фрейма-отца
    // transformStamped.transform.translation.y = obstacle_coords.y;
    // transformStamped.transform.translation.z = obstacle_coords.z;
    // transformStamped.transform.rotation.x = frame_position.orientation.w;
    // transformStamped.transform.rotation.y = frame_position.orientation.x;
    // transformStamped.transform.rotation.z = frame_position.orientation.y;
    // transformStamped.transform.rotation.w = frame_position.orientation.z;
    // tf2_p->sendTransform(transformStamped); // Трансформация отправляется
// ROS_INFO_STREAM("Debug flag n1")

    bool success_of_the_call = 0;

    if (client.call(model_state_srv))
    {
        success_of_the_call = model_state_srv.response.success;
        if (success_of_the_call != 0)
        {
            ROS_INFO_STREAM("Successfully called service" << model_state_srv.response.status_message); 
        }
        else
        {
            ROS_ERROR_STREAM("the call was successful but the model state was not set ---> " << model_state_srv.response.status_message);
        }
    }
    else
    {
        ROS_ERROR_STREAM("Failed to call service");
    }
    ROS_WARN_STREAM
    (
        // "Frame name ---> " << frame_name << std::endl <<
        "model_state_srv.request.model_state.reference_frame ---> " << model_state_srv.request.model_state.reference_frame << std::endl
    );
    ROS_INFO_STREAM("obstacle_coords.x ---> " << obstacle_coords.x << " " << "obstacle_coords.y ---> " << obstacle_coords.y << " " << "obstacle_coords.z ---> " << obstacle_coords.z);
}


int main(int argc, char **argv)
{
    srand(time(0));
    ros::init(argc, argv, "gazebo_model_state"/* + rand()*/);

    int node_loop_rate = 0;

    ros::NodeHandle n;
    client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
    gazebo_msgs::SetModelState model_state_srv;
    model_name = argv[1]; 
    // model_state_srv.request.model_state.model_name = *argv[1];
    obstacle_coords.x = atof(argv[2]);
    obstacle_coords.y = atof(argv[3]);
    obstacle_coords.z = atof(argv[4]);
    origin = obstacle_coords;
    maximum_displacement = atof(argv[5]);
    speed = atof(argv[6]);
    node_loop_rate = atof(argv[7]);
    
    // model_state_srv.request.model_state.twist.angular.x = 0; 
    // model_state_srv.request.model_state.twist.angular.y = 0;
    // model_state_srv.request.model_state.twist.angular.z = atof(argv[5]);
    // model_state_srv.request.model_state.pose.position.x = atof(argv[5]);
    // model_state_srv.request.model_state.pose.position.y = atof(argv[6]);
    // model_state_srv.request.model_state.pose.position.z = atof(argv[7]);
    // std::string frame_name;
    // frame_name = argv[9];

    // ROS_INFO_STREAM("argv[1]: " << argv[1] << " argv[2]: " << *argv[2] << " argv[3]: " << *argv[3] << " argv[4]: " << *argv[4] << " argv[5]: " << *argv[5] << " argv[6]: " << *argv[6] << " argv[7]: " << *argv[7] << " argv[8]: " << *argv[8]);


    ros::Rate loop_rate(node_loop_rate);

    while(ros::ok())
    {
        logic();
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}