#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <robomas_plugins/msg/robomas_frame.hpp>
#include <robomas_plugins/msg/robomas_target.hpp>
#include <daikei_upper/robomas_utils.hpp>

class DaikeiUpperNode : public rclcpp::Node
{
  private:
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
    rclcpp::Publisher<robomas_plugins::msg::RobomasFrame>::SharedPtr frame_pub_;
    rclcpp::Publisher<robomas_plugins::msg::RobomasTarget>::SharedPtr target_pub4_;
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg);
  public:
    DaikeiUpperNode() : Node("daikei_upper_node")
    {
      sub_ = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, std::bind(&DaikeiUpperNode::joy_callback, this, std::placeholders::_1));
      frame_pub_ = this->create_publisher<robomas_plugins::msg::RobomasFrame>("robomas_frame2", 10);
      target_pub4_ = this->create_publisher<robomas_plugins::msg::RobomasTarget>("robomas_target2_4", 10);
    }
};

void DaikeiUpperNode::joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg){
  if(msg->axes[4] == -1){
    target_pub4_->publish(robomas::get_target(-1885));
  }
  else if(msg->axes[4] == 1){
    target_pub4_->publish(robomas::get_target(1885));
  }
  else{
    target_pub4_->publish(robomas::get_target(0));
  }
  if(msg->buttons[7] == 1){
    frame_pub_->publish(robomas::get_vel_frame(4,false));
  }
  if(msg->buttons[6] == 1){
    frame_pub_->publish(robomas::get_dis_frame(4,false));
  }
}

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DaikeiUpperNode>());
  rclcpp::shutdown();
  return 0;
}