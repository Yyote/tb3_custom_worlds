#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>

geometry_msgs::Pose frame_position;
float frame_displacement;
double speed = 0.1; // м/c
float maximum_displacement = 1; // meters
std::vector<double_t> time_buffer;
tf2_ros::TransformBroadcaster *tf2_p;


// geometry_msgs::Point frame_position_calculator(std::vector<uint32_t> time_buffer_in, double speed, double current_x, double current_y, double current_z, double maximum_displacement)
// {
//     geometry_msgs::Point frame_position;

//     frame_position.x = current_x + speed * (time_buffer.at(0) - time_buffer.at(1));
//     frame_position.y = current_y + speed * (time_buffer.at(0) - time_buffer.at(1));
//     frame_position.z = current_z + speed * (time_buffer.at(0) - time_buffer.at(1));
//     return frame_position;
// }


geometry_msgs::Pose oscilate_by_x(std::vector<double_t> time_buffer_in, double &speed, double origin_x, double origin_y, double origin_z, double maximum_displacement)
{
    if(speed > 0)
    {
        if(frame_displacement < maximum_displacement)
        {
            frame_displacement = frame_displacement + speed * (time_buffer.at(0) - time_buffer.at(0));
            frame_position.position.x = origin_x + frame_displacement;
        }
        else
        {
            frame_displacement = frame_displacement + speed * (time_buffer.at(0) - time_buffer.at(0));
            frame_position.position.x = origin_x + frame_displacement;
        }
    }

    frame_position.position.y = origin_y;
    frame_position.position.z = origin_z;

    frame_position.orientation.w = 1;
    frame_position.orientation.x = 0;
    frame_position.orientation.y = 0;
    frame_position.orientation.z = 0;

    return frame_position;
}


void logic()
{
    time_buffer.at(1) = time_buffer.at(0);
    time_buffer.at(0) = ros::Time::now().toSec();

    oscilate_by_x(time_buffer, speed, 2, 0, 0, maximum_displacement);

    geometry_msgs::TransformStamped transformStamped; // Создание объекта трансформации

    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "world"; // задается фрейм-отец
    transformStamped.child_frame_id = "obstacle_1"; // задается фрейм-ребенок
    transformStamped.transform.translation.x = frame_position.position.x; // Задаются координаты смещения нового фрейма относительно фрейма-отца
    transformStamped.transform.translation.y = frame_position.position.y;
    transformStamped.transform.translation.z = frame_position.position.z;
    transformStamped.transform.rotation.x = frame_position.orientation.w;
    transformStamped.transform.rotation.y = frame_position.orientation.x;
    transformStamped.transform.rotation.z = frame_position.orientation.y;
    transformStamped.transform.rotation.w = frame_position.orientation.z;
    tf2_p->sendTransform(transformStamped); // Трансформация отправляется
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_tf2_broadcaster");
    ros::NodeHandle nh;
    
    time_buffer.resize(2);
    time_buffer.at(0) = 0;

    static tf2_ros::TransformBroadcaster br; // Создание объекта транслятора трансформаций
    tf2_p = &br;

    logic();

    ros::spin();

    return 0;
};
