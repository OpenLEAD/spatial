/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <rtt/Logger.hpp>

using namespace spatial;

Task::Task(std::string const& name)
    : TaskBase(name)
{
	an_decoder_initialise(&an_decoder);
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
	an_decoder_initialise(&an_decoder);
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;
    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();

	int index;
	int len;
	base::Time time = base::Time::now();

	int k = 0;
	
	while (_com_input.read(newpacket) == RTT::NewData){
			RTT::log(RTT::Warning) << "NEW DATA " << RTT::endlog();
		rawpacket.data.insert( rawpacket.data.end(), newpacket.data.begin(), newpacket.data.end() );
		index = 0;
		len = rawpacket.data.size();
		while ( len > 0 ){

			RTT::log(RTT::Warning) << k++ << ") bytes_received = " << an_decoder_size(&an_decoder) << " ou " << len << RTT::endlog();

			bytes_received = an_decoder_size(&an_decoder) < len ? an_decoder_size(&an_decoder) : len;
			
//			for(int i=0;i<bytes_received;i++)
//				*(an_decoder_pointer(&an_decoder)+i) = rawpacket.data[index+i];
			
			RTT::log(RTT::Warning) << "index = " << index << ", bytes_received = " << bytes_received << RTT::endlog();
			memcpy(an_decoder_pointer(&an_decoder), &(rawpacket.data[index]),bytes_received);
			index+=bytes_received;

			/* increment the decode buffer length by the number of bytes received */
			an_decoder_increment(&an_decoder, bytes_received);

			/* decode all the packets in the buffer */
			while ((an_packet = an_packet_decode(&an_decoder)) != NULL)
			{
				rawpacket.data.erase(rawpacket.data.begin(),rawpacket.data.begin()+an_packet_size(an_packet));
				RTT::log(RTT::Warning) << "(an_packet = an_packet_decode(&an_decoder)) != NULL" << RTT::endlog();
				if (an_packet->id == packet_id_system_state) /* system state packet */
				{
					/* copy all the binary data into the typedef struct for the packet */
					/* this allows easy access to all the different values             */
					if(decode_system_state_packet(&system_state_packet, an_packet) == 0)
					{

						angle.time = time;
						angle.roll = base::Angle::fromRad(system_state_packet.orientation[0]);
						angle.yaw = base::Angle::fromRad(system_state_packet.orientation[1]);
						angle.pitch = base::Angle::fromRad(system_state_packet.orientation[2]);
						RTT::log(RTT::Warning) << "GONNA WRITE!" << RTT::endlog();
						_angle.write(angle);
						RTT::log(RTT::Warning) << "WRITTEN!" << RTT::endlog();
					}
				}
				else if (an_packet->id == packet_id_raw_sensors) /* raw sensors packet */
				{
					/* copy all the binary data into the typedef struct for the packet */
					/* this allows easy access to all the different values             */
					if(decode_raw_sensors_packet(&raw_sensors_packet, an_packet) == 0)
					{
						imu.time = time;
						imu.acc = base::Vector3d(raw_sensors_packet.accelerometers[0],raw_sensors_packet.accelerometers[1],raw_sensors_packet.accelerometers[2]);
						imu.gyro = base::Vector3d(raw_sensors_packet.gyroscopes[0], raw_sensors_packet.gyroscopes[1], raw_sensors_packet.gyroscopes[2]);
						_imu.write(imu);
					}
				}
				else
				{
					
					RTT::log(RTT::Warning) << "Packet ID " << an_packet->id << " of Length " << an_packet->length << RTT::endlog();
				}
	
				/* Ensure that you free the an_packet when your done with it or you will leak memory */
				an_packet_free(&an_packet);
			}

			
			len -= bytes_received;
		}
	}


}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}

//						printf("System State Packet:\n");
//						printf("\tLatitude = %f, Longitude = %f, Height = %f\n", system_state_packet.latitude * RADIANS_TO_DEGREES, system_state_packet.longitude * RADIANS_TO_DEGREES, system_state_packet.height);
//						printf("\tRoll = %f, Pitch = %f, Heading = %f\n", system_state_packet.orientation[0] * RADIANS_TO_DEGREES, system_state_packet.orientation[1] * RADIANS_TO_DEGREES, system_state_packet.orientation[2] * RADIANS_TO_DEGREES);

//						Eigen::AngleAxisd rollAngle(0, Eigen::Vector3d::UnitZ());
//						Eigen::AngleAxisd yawAngle(0, Eigen::Vector3d::UnitY());
//						Eigen::AngleAxisd pitchAngle(0, Eigen::Vector3d::UnitX());
//						pose.orientation = rollAngle*yawAngle*pitchAngle;



//						printf("Raw Sensors Packet:\n");
//						printf("\tAccelerometers X: %f Y: %f Z: %f\n", raw_sensors_packet.accelerometers[0], raw_sensors_packet.accelerometers[1], raw_sensors_packet.accelerometers[2]);
//						printf("\tGyroscopes X: %f Y: %f Z: %f\n", raw_sensors_packet.gyroscopes[0] * RADIANS_TO_DEGREES, raw_sensors_packet.gyroscopes[1] * RADIANS_TO_DEGREES, raw_sensors_packet.gyroscopes[2] * RADIANS_TO_DEGREES);




