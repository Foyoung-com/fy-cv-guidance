# DJI Guidance Application Module

## Guidance SDK examples

The guidance SDK provide a USB and a UART example at guidance-sdk/examples directory. Follow the instructions to build and run guidance examples on Windows and Linux.

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