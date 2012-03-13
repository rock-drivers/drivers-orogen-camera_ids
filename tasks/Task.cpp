/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace camera;
using namespace camera_ids;
using namespace base::samples;

Task::Task(std::string const& name)
    : TaskBase(name)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
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

     unsigned long camera_id;
     std::stringstream ss(_camera_id.value());
     ss >> camera_id;

     std::string modeString = _mode.value();
     if(modeString == "Master") {
         this->camera_access_mode = Master;
     }
     else if (modeString == "Monitor") {
         this->camera_access_mode = Monitor;
     }
     else if (modeString == "MasterMulticast") {
         this->camera_access_mode = MasterMulticast;
     }
     else {
         RTT::log(RTT::Error) << "unsupported camera mode: "
             << _mode.value() << RTT::endlog();
         return false;
     }

     try {
         std::auto_ptr<camera::CamIds> camera(new camera::CamIds());
         RTT::log(RTT::Info) << "opening camera" << RTT::endlog();


        CamInfo cam_info;
        cam_info.unique_id = camera_id;
        camera->open(cam_info, this->camera_access_mode);

        /*  
        //std::vector<CamInfo> cameraList;
         //camera->listCameras(cameraList);   // get the list of cameras

         for (unsigned int it = 0; it < cameraList.size(); ++it) {
             if (cameraList[it].unique_id == camera_id) {
                 camera->open(cameraList[it], this->camera_access_mode);
                 break;
             }
         }
        */
         if (camera->isOpen() == false) {
             RTT::log(RTT::Error) << "camera not found" << RTT::endlog();
         }

         cam_interface = camera.release();
     }
     catch (std::runtime_error e) {
         RTT::log(RTT::Error) << "failed to initialize camera: " << e.what() << RTT::endlog();
         return false;
     }

     return true;
 }

bool Task::startHook()
{
    configureCameraIDS();

     if (! TaskBase::startHook())
         return false;
   
    return true; 
}
// void Task::updateHook()
// {
//     TaskBase::updateHook();
// }
// void Task::errorHook()
// {
//     TaskBase::errorHook();
// }
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
// void Task::cleanupHook()
// {
//     TaskBase::cleanupHook();
// }

void Task::configureCameraIDS()
{
    if(cam_interface->isAttribAvail(int_attrib::PixelClock))
    {
        cam_interface->setAttrib(int_attrib::PixelClock, _pixel_clock.get());
    }

    if( cam_interface->isAttribAvail(enum_attrib::MirrorXToOn) &&
        cam_interface->isAttribAvail(enum_attrib::MirrorXToOff) )
    {
        if ( _mirror_x.get() )
            cam_interface->setAttrib(enum_attrib::MirrorXToOn);
        else
            cam_interface->setAttrib(enum_attrib::MirrorXToOff);
    }
}

