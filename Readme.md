# hal2cpp

A HAL adapter layer to get LinuxCNC into your C++ program.

## What is this?

This is a system consisting of a realtime HAL component written in C, which dynamically loads a shared library written in C++. All supported LinuxCNC signals are thereby piped through a C++ function which you can modify.

LinuxCNC has a lot of supported hardware, but all of them are commercial plug-and-play packages and none allow you to really create your own. 

Hal2cpp can be used to get high-level C++ access to LinuxCNC's data and then you can do what you want with this data.

Example:  
You can retrieve the commanded positions for all axes and send them to an Arduino through a network in real time, that controls motors using custom PID controllers. Or you can send them to a non-realtime simulation. Or you pipe the positions straight back into LinuxCNC and you simulate a perfect machine. 

It allows you to use LinuxCNC's capabilities as a 5-axis CNC controller for any of your projects that need trajectory generation.

All of this is capable of realtime processing. It will stay that as long as you don't do anything weird. Involving other processes or networks or any other computers might introduce Jitter issues and it is your responsibility to keep the system realtime capable. But by default, it is.

### Why do we need this?

Somethimes you just want to tinker around with homemade CNC machines (e.g. Arduino based Servos) and you don't want to write your own trajectory planning system. But GRBL doesn't work either since you want true closed-loop servos, and you also don't want to buy expensive LinuxCNC-compatible hardware that completely destroys the point of doing your project in the first place.

Then you've come to the right place. Using Hal2cpp you can control your homemade Arduino servos and anything you can imagine using LinuxCNC as if it was a real CNC machine.

### How does it work? Why C++?

Because the HAL driver which interacts with LinuxCNC is written in a very old C standard with non-standard extensions. Also, there is no real build system that can build more than a single C file. It would be a major pain to develop a larger system under these circumstances.

The HAL driver is written in C and only does the minimum of dynamically loading a shared library and then forwarding the LinuxCNC calls by calling its functions. This is the only reason the HAL driver exists.

The shared library, on the other hand, is built using a proper CMake build system and modern C++17 standard. That means you can achieve anything much faster as there are also a lot more libraries available. Want to stream the commanded X position to a microcontroller using UDP? Simply download a CMake-compatible networking library for C++ and write around 3 to 5 lines of code. Doing the same thing in C without a proper build system would be a major pain. This is why the C++ shared library exists.

If you need a library for UDP sockets, I recommend https://github.com/fpagliughi/sockpp.

See below for info on how the HAL file is generated.

### Realtime concerns

Whenever you have motion control systems, realtime capabilities are important to keep in mind. By default, the C++ shared library allows true realtime processing since it is a realtime HAL component and is part of the LinuxCNC process.

This is why you should try to keep as much as possible in this one library. For example, streaming data to another process on the same PC would not work since the other process is not part of LinuxCNC and is not guaranteed to run deterministically.

Also, try to avoid multithreading. Only use other threads for non-realtime secondary processing, but the main processing must be done directly in the function called by LinuxCNC.

Similarly, if possible, try not to use exceptions. They are not supported by the C-based LinuxCNC backend and will cause undefined behavior.

### Hal2Arduino

A similar project called Hal2Arduino already exists, but it is no longer maintained, does not work out of the box, is non-realtime capable as it is written in Python, and sends data to an Arduino via USB Serial (which, again, is not realtime capable).

Hal2cpp tries to fix all of this, while staying realtime capable, and also allowing you to write your Arduino interface yourself. It should not just be used for Arduinos, but for anything that needs cartesian motion control.

# Usage

Here are the steps to use Hal2cpp on a new computer:

 - Install the latest version of LinuxCNC and start it
 - Run Stepconf and configure your machine
   - Create new configuration
   - Choose XYZA axis configuration and mm as units
   - Keep "One Parport"
   - Do not change the pin layout, it is irrelevant
   - Specify all your motor parameters. You cannot test them yet since hal2cpp is not installed yet.
   - Finish the wizard and remember where the config is saved.
 - Now, open a terminal, clone hal2cpp, tell it where the main .hal config file is at and build it:
    ```
    git clone https://github.com/HerrNamenlos123/hal2cpp.git
    cd hal2cpp
    ./build.sh <path-to-your-config.hal> # (e.g. ~/linuxcnc/configs/CNC/CNC.hal)
    ```
 - This is how you build and install everything.
 - Now, you modify the code in `src/` and tell it what to do.
 - After every change, simply rerun `./build.sh` and start LinuxCNC via the shortcut generated by Stepconf.
 - Whenever you need to change machine parameters, simply rerun Stepconf and then rerun `./build.sh`. If you get an error that a parallel port is missing, you forgot to run `build.sh` after Stepconf.

# HAL file generation

The main HAL file is automatically generated using python. This is because it is easier to make changes without breaking existing things. When you need additional signals from LinuxCNC, modify the `config.json` file and re-run the build system. Your change should now be visible in the HAL file.

# License

This library is licensed under the MIT license. This means you are free to do with it whatever you want, and you are allowed to re-upload the entire repository as part of your own.

# Disclaimer

This library comes without any warranty whatsoever, and any harm or damage caused by this library or the incorrect usage thereof is not taken care of. You are responsible for keeping your machine safe.