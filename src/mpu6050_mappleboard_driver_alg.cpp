#include "mpu6050_mappleboard_driver_alg.h"

Mpu6050MappleboardDriverAlgorithm::Mpu6050MappleboardDriverAlgorithm(void)
{
}

Mpu6050MappleboardDriverAlgorithm::~Mpu6050MappleboardDriverAlgorithm(void)
{
	CloseSerial();
}

// Mpu6050MappleboardDriverAlgorithm Public API

bool Mpu6050MappleboardDriverAlgorithm::OpenSerial(const std::string &serial)
{
	this->serial_name_ = serial;

  	/* open serial port */
  	printf("openning port...\n");
  	this->serial_ = open(this->serial_name_.c_str(), O_RDWR | O_NOCTTY);
  	if (this->serial_ != -1)
  	{
		printf("Serial %s open ok\n",this->serial_name_.c_str());
		return 1;
  	}
  	else 
  	{
  		printf("Could not open serial port %s\n",this->serial_name_.c_str());
  		return 0;
  	}
}

bool Mpu6050MappleboardDriverAlgorithm::CloseSerial()
{
	if (this->serial_!=-1)
	{		
		close(this->serial_);
		printf("Serial %s closed\n",this->serial_name_.c_str());
		return 1;
	}
	else
	{
		printf("Serial %s was already closed\n",this->serial_name_.c_str());
		return 0;
	}
}

bool Mpu6050MappleboardDriverAlgorithm::ReadSerial(double &ax, double &ay, double &az, double &gx, double &gy, double &gz)
{
   	int n;
  	int i;
  	unsigned char buf[64] = {0};
  	double gravity = 9.8;
  	double sec_to_rad = 3.14159265359/180.0;
  	double accel_LSB = 1.0/8192.0; 
  	double gyro_LSB = 1.0/131.0; 
  	double accel_LSB_g = accel_LSB * gravity;
  	double gyro_LSB_rad = gyro_LSB * sec_to_rad;

	if (this->serial_==-1)
	{		
		close(this->serial_);
		printf("Read unsuccessful. Serial %s is closed\n",this->serial_name_.c_str());
		return 0;
	}

  	struct termios toptions;

  	tcgetattr(this->serial_, &toptions);

	cfsetispeed(&toptions, B1000000);
  	cfsetospeed(&toptions, B1000000);
  	toptions.c_cflag     |= (CLOCAL | CREAD);
  	toptions.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
  	toptions.c_oflag     &= ~OPOST;
  	toptions.c_cc[VMIN]  = 0;
  	toptions.c_cc[VTIME] = 10;
  	tcsetattr(this->serial_, TCSANOW, &toptions);

    do n = read(this->serial_, buf, 1);//READ IT
    while (buf[0]!=0x47);
    	n = read(this->serial_, buf, 12);
      
    if (n>3) 
    {
		ax   = (double)((int16_t)((buf[1]<<8)|buf[0]))*accel_LSB_g;
		ay   = (double)((int16_t)((buf[3]<<8)|buf[2]))*accel_LSB_g;
		az   = (double)((int16_t)((buf[5]<<8)|buf[4]))*accel_LSB_g;
		gx   = (double)((int16_t)((buf[7]<<8)|buf[6]))*gyro_LSB_rad;
		gy   = (double)((int16_t)((buf[9]<<8)|buf[8]))*gyro_LSB_rad;
		gz   = (double)((int16_t)((buf[11]<<8)|buf[10]))*gyro_LSB_rad;
    }
	fflush(stdin);
	return 1;
}