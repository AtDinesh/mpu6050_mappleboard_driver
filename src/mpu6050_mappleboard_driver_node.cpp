#include "mpu6050_mappleboard_driver_node.h"

Mpu6050MappleboardDriverNode::Mpu6050MappleboardDriverNode(ros::NodeHandle &nh, dynamic_reconfigure::Server<Config> &dsrv) 
{
  // Dynamic Reconfigure
  dynamic_reconfigure::Server<Config>::CallbackType dsrv_cb;
  dsrv_cb = boost::bind(&Mpu6050MappleboardDriverNode::DynRecCallback, this, _1, _2);
  dsrv.setCallback(dsrv_cb);

  //init class attributes if necessary

  // Get launch parameters
  nh.param<std::string>("serial_name",this->serial_name_,"/dev/ttyACM0");

  nh.param<std::string>("frame_id",this->frame_id_,"imu");

  std::string topic_name;
  nh.param<std::string>("topic_name",topic_name,"/imu_mpu6050");

  if (!this->alg_.OpenSerial(this->serial_name_))
  {
    ROS_ERROR("Could not open serial port %s.",this->serial_name_.c_str());
    ros::shutdown();
  }


  // [init publishers]
  this->imu_publisher_ = nh.advertise<sensor_msgs::Imu>(topic_name, 1);
  
  // [init subscribers]
  
  // [init services]
  
  // [init clients]
  
  // [init action servers]
  
  // [init action clients]
}

Mpu6050MappleboardDriverNode::~Mpu6050MappleboardDriverNode(void)
{
  // [free dynamic memory]
}

void Mpu6050MappleboardDriverNode::mainNodeThread(void)
{
  // [fill msg structures]
  // Initialize the topic message structure

  double ax,ay,az,gx,gy,gz;
  if (this->alg_.ReadSerial(ax,ay,az,gx,gy,gz))
  {
    this->imu_msg_.header.stamp = ros::Time::now();
    this->imu_msg_.header.frame_id = this->frame_id_;
    this->imu_msg_.orientation.w = 1.0;
    this->imu_msg_.linear_acceleration.x=ax;  
    this->imu_msg_.linear_acceleration.y=ay;  
    this->imu_msg_.linear_acceleration.z=az;  
    this->imu_msg_.angular_velocity.x=gx;  
    this->imu_msg_.angular_velocity.y=gy;  
    this->imu_msg_.angular_velocity.z=gz;  

    this->imu_publisher_.publish(this->imu_msg_);
  }
  else
  {
    ROS_ERROR("Serial %s is closed.",this->serial_name_.c_str());
    ros::shutdown();
  }

  // [fill srv structure and make request to the server]
  
  // [fill action structure and make request to the action server]

  // [publish messages]
}

/*  [subscriber callbacks] */

/*  [service callbacks] */

/*  [action callbacks] */

/*  [action requests] */

void Mpu6050MappleboardDriverNode::DynRecCallback(Config &config, uint32_t level)
{
  this->alg_.lock();
  this->config_=config;
  this->alg_.unlock();
}

/* main function */
int main(int argc,char *argv[])
{
  ros::init(argc, argv, "mpu6050_mappleboard_driver");
  ros::NodeHandle nh(ros::this_node::getName());
  ros::Rate loop_rate(2000);

  dynamic_reconfigure::Server<Config> dsrv;    

  Mpu6050MappleboardDriverNode node(nh, dsrv);

  while (ros::ok()) 
  {
    node.mainNodeThread();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
