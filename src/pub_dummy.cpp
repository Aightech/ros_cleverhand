// src/publisher_node.cpp
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher")
  {
    // Create a publisher that publishes String messages on the "chatter" topic.
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // Create a timer to publish a message periodically (every 500ms).
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&MinimalPublisher::timerCallback, this)
    );
  }

private:
  void timerCallback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, ROS 2!";
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
