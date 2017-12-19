# mpu6050_mapleboard_driver

Goal :
Get the data from MPU6050 IMU and publish (Acc and Gyro) on ROS.


## Getting Started

Make sure that the MPU is correctly installed and that you can communicate with it (the user should be in the dialout group) 
Also check on which USB the device is connected (when connected with mapleBoard, the default is /dev/ttyACM0 if no other maple board is connected)

### running

```
roslaunch mpu6050_mappleboard_driver mpu6050.launch
```

