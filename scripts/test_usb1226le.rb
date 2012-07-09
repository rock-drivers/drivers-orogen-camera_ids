#!/usr/bin/env ruby

require 'orocos'
require 'readline'
include Orocos

Orocos::CORBA::max_message_size=8*1024*1024

Orocos.initialize

Orocos.run 'camera_ids::Task' => 'camera' do 
    camera = Orocos::TaskContext.get 'camera'
    camera.camera_id = '101'
    camera.width = 752
    camera.height = 480
    camera.camera_format = :MODE_GRAYSCALE
    camera.pixel_clock=20
    camera.fps = 30
    camera.exposure_mode = 'manual'
    camera.exposure = 1000
    camera.binning_x = 1
    camera.binning_y = 1
    camera.region_x = 0
    camera.region_y = 0
    camera.gain_mode_auto = 0
    camera.gain = 0
    #camera.mode = 'Master'
    #camera.whitebalance_mode = 'manual'
    #camera.trigger_mode = 'fixed'
    #camera.camera_format = :MODE_BAYER
    #camera.output_format = :MODE_BAYER
    #camera.resize_algorithm = :BAYER_RESIZE
    #camera.scale_x = 0.5
    #camera.scale_y = 0.5
    #camera.log_interval_in_sec = 5
    #camera.disable_frame_raw = false
   
    camera.configure
    camera.start

    Readline.readline('Press Enter to exit') do
    end

end
