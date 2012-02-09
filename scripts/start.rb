#!/usr/bin/env ruby

require 'orocos'
require 'readline'
include Orocos

Orocos.initialize

Orocos.run 'camera_ids::Task' => 'camera' do 
    camera = Orocos::TaskContext.get 'camera'
    camera.camera_id = '2'
   # camera.binning_x = 1
   # camera.binning_y = 1
   # camera.region_x = 0
   # camera.region_y = 0
   # camera.width = 1280
   # camera.height = 1024
   # camera.trigger_mode = 'fixed'
   # camera.exposure = 50000
   # camera.exposure_mode = 'manual'
   # camera.whitebalance_mode = 'manual'
   # camera.fps = 24
   # camera.gain = 0
   # camera.gain_mode_auto = 0
   # camera.camera_format = :MODE_RGB
   # camera.output_format = :MODE_RGB
   # #camera.camera_format = :MODE_BAYER
   # #camera.output_format = :MODE_BAYER
   # #camera.resize_algorithm = :BAYER_RESIZE
   # camera.scale_x = 0.5
   # camera.scale_y = 0.5
   # camera.log_interval_in_sec = 5
   # camera.mode = 'Master'
   # camera.disable_frame_raw = false

    camera.configure
    camera.start

    Readline.readline('Press Enter to exit\n') do
    end
end
