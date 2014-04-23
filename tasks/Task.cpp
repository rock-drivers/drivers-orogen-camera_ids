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

         if (camera->isOpen() == false) {
             RTT::log(RTT::Error) << "camera not found" << RTT::endlog();
             return false;
         }

         cam_interface = camera.release();
     }
     catch (std::runtime_error e) {
         RTT::log(RTT::Error) << "failed to initialize camera: " << e.what() << RTT::endlog();
         return false;
     }
    
     double period = 1. / _fps.get();

     RTT::log(RTT::Info) << "period is " << period << " s" << RTT::endlog();

     camera::CamIds* cids_ptr = static_cast<camera::CamIds*>(cam_interface);

     cids_ptr->setEventTimeout(int(_timeout_periods.get() * period * 1000.));

     if (_enable_api_log.get()) cids_ptr->setErrorReport(true);

     cids_ptr->setGetEveryFrame(_get_every_frame.get());

     // set the gain boost if applicable
     // the camera instance is accessed directly as the interface does not support the gain boost feature
     if(!cids_ptr->setGainBoost(_gain_boost.get())) {
         return false;
     }
     
     return true;
 }

bool Task::startHook()
{

     if (! TaskBase::startHook())
         return false;

    RTT::log(RTT::Info) << "Start camera " << _camera_id.value() << RTT::endlog();
    
    return true; 
}

void Task::updateHook()
{
    static base::Time tlast = base::Time::now();
    static unsigned int counter = 0;

    if ( counter % _capture_status_divider.get() == 0 )
        _capture_status.write(
                static_cast<camera::CamIds*>(cam_interface)->getCaptureStatus());
    counter++;

    base::Time tstart = base::Time::now();
    if (!cam_interface->isFrameAvailable()) {
        RTT::log(RTT::Error) << "No frame received during timeout period." << RTT::endlog();
        report(TIMEOUT_ERROR);
    }
    base::Time there = base::Time::now();
    RTT::log(RTT::Debug) << "time for triggering: " << (tstart-tlast).toSeconds()
        << "; time to wait: " << (there - tstart).toSeconds() << RTT::endlog();
    if(getFrame()) {

        base::samples::frame::Frame *frame_ptr = camera_frame.write_access();
        camera_frame.reset(frame_ptr);

        _frame.write(camera_frame);

    } else {

        RTT::log(RTT::Warning) << "Error during frame retrieval!" << RTT::endlog();
    }

    tlast = base::Time::now();
    this->getActivity()->trigger();
}

void Task::errorHook()
{
    TaskBase::errorHook();
}
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}

bool Task::configureCamera()
{
    // Pixelclock
    cam_interface->setAttrib(int_attrib::PixelClock, _pixel_clock.get());

    // set mirrors
    if( cam_interface->isAttribAvail(enum_attrib::MirrorXToOn) &&
        cam_interface->isAttribAvail(enum_attrib::MirrorXToOff) )
    {
        if ( _mirror_x.get() )
            cam_interface->setAttrib(enum_attrib::MirrorXToOn);
        else
            cam_interface->setAttrib(enum_attrib::MirrorXToOff);
    }
    
    if( cam_interface->isAttribAvail(enum_attrib::MirrorYToOn) &&
        cam_interface->isAttribAvail(enum_attrib::MirrorYToOff) )
    {
        if ( _mirror_y.get() )
            cam_interface->setAttrib(enum_attrib::MirrorYToOn);
        else
            cam_interface->setAttrib(enum_attrib::MirrorYToOff);
    }

    // auto gain
    if( cam_interface->isAttribAvail(enum_attrib::GainModeToAuto) &&
        cam_interface->isAttribAvail(enum_attrib::GainModeToManual) )
    {
        if ( _gain_mode_auto.get() )
            cam_interface->setAttrib(enum_attrib::GainModeToAuto);
        else
            cam_interface->setAttrib(enum_attrib::GainModeToManual);
    }

    // call the configureCamera method of the super class to apply the remaining settings
    TaskBase::configureCamera();

    return true;
}

