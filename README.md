# SH_control_example_ws
## N.B.
First of all, you need to download: qbAPI repository (remove the -master suffix), qbadmin repository, SH_control_example.cpp, and sine.txt, and be sure to have a file tree organized like this:

your_workingcopy
- qbadmin
- qbAPI
- SH_control_example.cpp
- sine.txt
  
Click on the qbAPI submodule and follow the instructions to install the package.
Click on the qbadmin submodule and follow the instructions to install the package.

Connect the SoftHand via USB cable to the computer. 

Open a new terminal in the folder "qbadmin/src/bin_win" (Windows) or "qbadmin/src/bin_unix" (Linux) and call _qbadmin -t_ to get the COM port used by the board. Assign this value to COMPORT variable in in _SH_control_example.cpp_
In the same folder call _qbadmin -p_. Read the value assigned to the first parameter __Device_ID__. Assign this value to device_id variable in in _SH_control_example.cpp_
Compile SH_control_example.cpp with "g++ ReadFile_SetInput.cpp QB_lib/cp_communications.cpp QB_lib/qbmove_communications.cpp -o SH_control_example".
The code reads data from the sine.csv file and sends them as input values to the SoftHand using the RS485 protocol. 

Connect the SoftHand using the USB cable.  
In the folder "qbadmin/src/bin_win" (Windows) or "qbadmin/src/bin_unix" (Linux)  call _qbadmin -p_  and make sure that these parameters are set as follow:
__Input mode: USB__
__Control mode: Position__
__Resolutions: 3 3 3__
__Position limits active: YES__
__Pos. limits [inf, sup]:  0 16000__ --> For the SoftHand with 2 motors, the value might be __Pos. limits [inf, sup]: -16000 0__ (we can't remember, ask who is working with it right now) 
In this case the 'sign' variable in _SH_control_example.cpp_ should be set to -1

To change the value of the parameters call 'qbparam' and choose 'set'
Run SH_control_example

# Clone from terminal
```
git clone --recurse https://github.com/IIT-SoftBots/SH_control_example_ws.git
```
