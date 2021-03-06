# monitor_generator
This project is a realtime verification software for the R5-COP project. Project website: http://www.r5-cop.eu/en/
The package contains a demo of a monitor code generator and the generated monitor frame which subscribes to the velocity commands of a turtlesim turtle.

If any of the expressions down below are new to you, search them on http://wiki.ros.org/

##GENERATOR

###Required Software

  * C++11 compatible C++ compiler
  * Boost 1.58 library
  * CMake

###Getting all the stuff for the generator

[Download boost 1.58](http://www.boost.org/users/history/version_1_58_0.html)

  ```
  sudo apt-get update
  sudo apt-get upgrade
  sudo apt-get install build-essential
  sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  sudo apt-get update
  sudo apt-get install gcc-5 g++-5
  sudo rm /usr/bin/gcc
  sudo rm /usr/bin/g++
  sudo ln -s /usr/bin/gcc-5 /usr/bin/gcc
  sudo ln -s /usr/bin/g++-5 /usr/bin/g++
  cd ~/Downloads/
  tar --bzip2 -xf boost_1_58_0.tar.bz2
  cd boost_1_58_0/
  sudo ./bootstrap.sh --prefix=/usr/local
  sudo ./b2 install
  ```

Open up an admin command line
  ```
  git clone https://github.com/gazben/monitor_generator.git
  cd <unzipped boost library>
  bootstrap.bat
  b2 install toolset=gcc
  ```

###Compiling the generator

Before, you can run a monitor instance, you have to generate one first. The monitor, is only available, after running the generator.

  ```
  git clone https://github.com/gazben/monitor_generator.git
  cd monitor_generator
  mkdir generator_build
  cd generator_build
  cmake ..
  make
  ```

###Running the generator

The generator can be controlled with command line arguments. 
  ```
  --help                 See available options.
  --source-path arg      Root directory of the monitor frame source.
  --output-path arg      Root directory, of the generated monitor.
  --debug-output arg     Directory of the log files, and generation information.
  --input-expression arg Expression that defines, the monitor behavior.
  --monitor-name arg     Name of the generated monitor.
  --true-command arg     System call if the result is TRUE
  --false-command arg    System call for the result is FALSE
  ```


####Example:
  ```
  ./monitor_generator --input-expression="G('event_r' => ('event_p' U 'event_d'))" --source-path="/home/user/Documents/monitor_generator/monitor" --output-path="/home/user/Documents/monitor_generator/generated" --monitor-name="monitor_test" --wait --true-command="echo The_result_is_TRUE!" --false-command="echo The_result_is_FALSE!"
  ```


##MONITOR


###Required Software
  * [Ubuntu 14.04](http://releases.ubuntu.com/14.04)
  * [ROS Jade](http://wiki.ros.org/jade/Installation/Ubuntu)
  * [turtlesim](http://wiki.ros.org/turtlesim)
  * C++11 compatible compiler

The monitor supported on Ubuntu 14.04 and Linux Mint 17.3 but feel free, to try it out on other distros.

Getting all the stuff for the monitor:
Warning: The monitor is not supported on windows! You can try [win_ros](http://wiki.ros.org/win_ros) but it's not supported.

  ```
  sudo apt-get update
  sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu trusty main" > /etc/apt/sources.list.d/ros-latest.list'
  wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
  sudo apt-get update
  sudo apt-get install ros-jade-desktop-full
  sudo rosdep init
  rosdep update
  echo "source /opt/ros/jade/setup.bash" >> ~/.bashrc
  source ~/.bashrc
  sudo apt-get install python-rosinstall
  source /opt/ros/jade/setup.bash
  mkdir -p ~/catkin_ws/src
  cd ~/catkin_ws/src
  catkin_init_workspace
  cd ~/catkin_ws/
  catkin_make
  source devel/setup.bash
  ```

##Monitor deploying

You should put a sylink into the catkin_ws/src tha will lead to the generated folder.
For example:
  * Symlink: /home/user/catkin_ws/src/monitor_generator
  * Folder: /home/user/projects/monitor_generator/generated


  ```
  sudo ln -s /home/user/projects/monitor_generator/generated /home/user/catkin_ws/src/monitor_generator
  ```


How to run the demo:
  * Open a terminal in the catkin_ws folder (/home/user/catkin_ws)
  * Create the symlink to the /src folder by running the ln -s command above


  ```
 roscore 
  ```

In a separate terminal window type the following command: 

 ```
 source devel/setup.bash
 ```
 
 Build the executables in the root folder of your catkin workspace using the command : 
 
 ```  
 catkin_make install --only-pkg-with-deps <monitor_name>
 ```

 By running the following command the launchfile will set up the demo:

 ``` 
 roslaunch <monitor_name> test.launch
 ```
