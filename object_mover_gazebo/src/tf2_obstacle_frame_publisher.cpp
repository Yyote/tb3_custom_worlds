#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Point.h>


double speed = 0.1; // м/c
std::vector<uint32_t> time_buffer;

time_buffer.resize(2);

geometry_msgs::Point frame_position_calculator(std::vector<uint32_t> time_buffer_in, double speed, double current_x, double current_y, double current_z)
{
    geometry_msgs::Point frame_position;
    frame_position.x = current_x + speed * time;
    frame_position.y = current_y + speed * time;
    frame_position.z = current_z + speed * time;
    return frame_position;
}


void logic()
{
    static tf2_ros::TransformBroadcaster br; // Создание объекта транслятора трансформаций
    geometry_msgs::TransformStamped transformStamped; // Создание объекта трансформации

    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "world"; // задается фрейм-отец
    transformStamped.child_frame_id = "obstacle_1"; // задается фрейм-ребенок
    transformStamped.transform.translation.x = 0.0; // Задаются координаты смещения нового фрейма относительно фрейма-отца
    transformStamped.transform.translation.y = 0.0;
    transformStamped.transform.translation.z = 0.0;
    tf2::Quaternion q; // Задается поворот(в кватернионе) фрейма-ребенка относительно фрейма-отца
    q.setRPY(0, 0, 0);
    transformStamped.transform.rotation.x = q.x();
    transformStamped.transform.rotation.y = q.y();
    transformStamped.transform.rotation.z = q.z();
    transformStamped.transform.rotation.w = q.w();
    br.sendTransform(transformStamped); // Трансформация отправляется
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_tf2_broadcaster");
    ros::NodeHandle nh;
    
    ros::NodeHandle node;
    ros::spin();

    return 0;
};
