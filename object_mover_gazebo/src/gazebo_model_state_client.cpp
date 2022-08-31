#include "ros/ros.h"
#include <gazebo_msgs/SetModelState.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/ModelState.h>
#include <ctime>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    srand(time(0));
    ros::init(argc, argv, "gazebo_model_state"/* + rand()*/);

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
    gazebo_msgs::SetModelState model_state_srv;
    std::string model_name = argv[1]; 
    model_state_srv.request.model_state.model_name = model_name;
    // model_state_srv.request.model_state.model_name = *argv[1];
    model_state_srv.request.model_state.twist.linear.x = atof(argv[2]);
    model_state_srv.request.model_state.twist.linear.y = atof(argv[3]);
    model_state_srv.request.model_state.twist.linear.z = atof(argv[4]);
    model_state_srv.request.model_state.twist.angular.x = 0; 
    model_state_srv.request.model_state.twist.angular.y = 0;
    model_state_srv.request.model_state.twist.angular.z = atof(argv[5]);
    model_state_srv.request.model_state.pose.position.x = atof(argv[6]);
    model_state_srv.request.model_state.pose.position.y = atof(argv[7]);
    model_state_srv.request.model_state.pose.position.z = atof(argv[8]);
    model_state_srv.request.model_state.reference_frame = argv[9];

    ROS_INFO_STREAM("argv[1]: " << argv[1] << " argv[2]: " << *argv[2] << " argv[3]: " << *argv[3] << " argv[4]: " << *argv[4] << " argv[5]: " << *argv[5] << " argv[6]: " << *argv[6] << " argv[7]: " << *argv[7] << " argv[8]: " << *argv[8]);

    bool success_of_the_call = 0;

    while(ros::ok())
    {
        if (client.call(model_state_srv))
        {
            success_of_the_call = model_state_srv.response.success;
            if (success_of_the_call != 0)
            {
                ROS_INFO_STREAM("Successfully called service" << model_state_srv.response.status_message); 

                gazebo_msgs::GetModelState get_model_state_srv;

                get_model_state_srv.request.model_name;
                get_model_state_srv.request.relative_entity_name;

                if (client.call(get_model_state_srv))
                {
                    ROS_INFO_STREAM("Robot state parameters ---> " << "pose position x" << get_model_state_srv.response.pose.position.x << " " << "pose position y" << get_model_state_srv.response.pose.position.y << " " << "pose position z" << get_model_state_srv.response.pose.position.z << " " << "pose orientation x" << get_model_state_srv.response.pose.orientation.x << " " << "pose orientation y" << get_model_state_srv.response.pose.orientation.y << " " << "pose orientation z" << get_model_state_srv.response.pose.orientation.z << " " << "pose orientation w" << get_model_state_srv.response.pose.orientation.w << " " << "twist linear x" << get_model_state_srv.response.twist.linear.x << " " << "twist linear y" << get_model_state_srv.response.twist.linear.y << " " << "twist linear z" << get_model_state_srv.response.twist.linear.z << " " << "twist angular x" << get_model_state_srv.response.twist.angular.x << " " << "twist angular y" << get_model_state_srv.response.twist.angular.y << " " << "twist angular z" << get_model_state_srv.response.twist.angular.z);
                } 

            }
            else
            {
                ROS_ERROR_STREAM("the call was successful but the model state was not set ---> " << model_state_srv.response.status_message);
            }
        }
        else
        {
            ROS_ERROR_STREAM("Failed to call service");
            return 1;
        }
    }

    return 0;
}