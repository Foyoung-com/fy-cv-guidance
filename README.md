# DJI Guidance Application Module

## Guidance SDK examples

The Guidance SDK provides an USB and an UART example at guidance-sdk/examples directory. Follow the instructions to build and run guidance examples on Windows and Linux.

1. Build Guidance SDK examples on Windows

    * Requirements:

      Windows 7/10
   
      Visual Studio 2013 64bit

    * Build:

      Change to build\win64 directory: `cd build\win64`

      run build_guidance_examples.cmd

2. Build Guidance SDK examples on Linux

    * Requirements:

      Ubuntu 14.04/16.04 64bit

      gcc toolchain

    * Build:

      change to build\linux64 directory: `cd build/linux64`

      run build_guidance_examples.sh

3. Run USB example on Windows

    * Requirements:

      Guidance Assistant Software is installed

    * Connect Guidance system to your PC via USB

    * run build\win64\guidance_examples\bin\Release\dji_guidance_usb.exe

4. Run USB example on Linux

    * Connect Guidance system to you PC via USB

    * run build/linux64/guidance_examples/bin/dji_guidance_usb

      change to build/linux64/guidance_examples/bin

      run `sudo ./dji_guidance_usb`

5. Run UART example in a similar manner (If you'd like trying out :astonished: )

## Guidance SDK demos

The guidance SDK provides two demos at guidance-sdk/demos. The camshift demo implements a depth-based tracking with camshift algorithm. This demo is meant to run on both Windows and Linux.
The obstacle bypass demo implements fully autonomous flight runs onboard. The demo is built on **ROS**. 

1. Build Guidance SDK demos on Windows

    * Requirements
   
      Windows 7/10
	 
	  Visual Studio 2013 64 bit
	 
    * Build:

      Change to build\win64 directory: `cd build\win64`

      run build_guidance_demos.cmd
	 
2. Build Guidance SDK demos on Linux

    * Requirements:

      Ubuntu 14.04/16.04 64bit

      gcc toolchain

    * Build:

      change to build/linux64 directory: `cd build/linux64`

      run build_guidance_demos.sh

3. Build Guidance SDK demos on **ROS**

    * TODO
	
4. Run camshift demo on Windows

    * Requirements:

      Guidance Assistant Software is installed

    * Connect Guidance system to your PC via USB

    * run build\win64\guidance_demos\bin\Release\sdk_tracking_camshift.exe
	 
    * Select a bounding rectangle surrounding the interested object on the tracking window with your mouse.
	 
5. Run camshift demo on Linux

    * Connect Guidance system to you PC via USB

    * run build/linux64/guidance_demos/bin/sdk_tracking_camshift

      change to build/linux64/guidance_demos/bin

      run `sudo ./sdk_tracking_camshift`
		
	* Select a bounding rectangle surrounding the interested object on the tracking window with your mouse.
	
6. Run obstacle bypass demo onboard

    * TODO


## FY Guidance Checker board example
The checker board detector.

1. Build checker board detection example on Windows:

   * Requirements:

     Windows 7/10

         Visual Studio 2013 64 bit

   * Build:

     Change to build\win64 directory: 'cd build\win64'
   
     run build.cmd

2. Build checker board detection example on Linux:

   * Requirements:

     Ubuntu 14.04 64bit

     gcc toolchain

   * Build:

     Change to build/linux64 directory: 'cd build/linux64'

     run build.sh

3. Run checker board example on Windows:

   * Requirements:

     Guidace Assisant Software is installed

   * Run checkerboard.exe
   
     The application fails the first attempt, it merely generates a configuration file checker_conf0.xml

   * Modify the generated configuration file

     * enable_video: set to 1 to save a video, set to 0 to disable saving
     
     * source: set video source.
       * 0: guidance
       * 1: video file
       * 2: integrated system camera

     * source_file: video file path, only used when source is 1

     * camera_index: system camera index

     * vbus_index: guidance vbus index

     * horiz_corners: number of checker board grids horizontally

     * colum_corners: number of checker board grids vertically

   * Run checkerboard with the configuration file path as the parameter:

     checkerboard checker_conf0.xml

   * In case of guidance source, please connect guidance system to your PC via USB.

4. Run checker board example on Linux:

   * ditto


