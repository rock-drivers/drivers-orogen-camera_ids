#!/usr/bin/env ruby

require 'orocos'
require 'readline'
include Orocos

Orocos::CORBA::max_message_size=8*1024*1024

Orocos.initialize

Orocos.run 'camera_ids::Task' => 'camera' do 
    camera = Orocos::TaskContext.get 'camera'
    camera.camera_id = '102'
    camera.width = 640
    camera.height = 480
    camera.camera_format = :MODE_RGB
    camera.pixel_clock = 60
    camera.fps = 20
    camera.exposure_mode = 'manual'
    camera.exposure = 5000
    camera.binning_x = 2
    camera.binning_y = 2
    camera.region_x = 0
    camera.region_y = 0
    camera.gain_mode_auto = 0
    camera.gain = 0
    camera.timeout_periods = 10
  
    Orocos.log_all

    camera.configure
    camera.start

    Readline.readline('Press Enter to exit') do
    end

end
