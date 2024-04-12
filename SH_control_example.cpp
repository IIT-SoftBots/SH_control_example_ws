// ----------------------------------------------------------------------------
// BSD 3-Clause License

// Copyright (c) 2016, qbrobotics
// Copyright (c) 2017-2020, Centro "E.Piaggio"
// All rights reserved.


// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:


// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.


// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE

// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

/**
 *  \file       SH_control_example.cpp
 *
 *  \brief      Library of functions for serial port communication with a board
 * \date        April 12th, 2024
 * \author       _Centro "E.Piaggio"_
 * \copyright    (C) 2012-2016 qbrobotics. All rights reserved.
 * \copyright    (C) 2017-2020 Centro "E.Piaggio". All rights reserved.


 *  \details
 *
 *  Check the \ref qbmove_communications.h "qbmove_communications.h" file
 *  for a complete description of the public functions implemented in
 *  SH_control_example.cpp
**/




//############################################################################################################################
// Compile with "g++ ReadFile_SetInput.cpp QB_lib/cp_communications.cpp QB_lib/qbmove_communications.cpp -o SH_control_example"
//############################################################################################################################

// -------------------------------------------------------------------------------- includes

#include "qbAPI/src/qbmove_communications.h"
#include "qbAPI/src/cp_communications.h"
#include "iostream"
#include <fstream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <io.h>
#else
    #include <sys/stat.h>
#endif

//Modify device_id and COMPORT according to the board 
int device_id = 1; // Device ID for the QB device
const char* COMPORT = "COM3"; // COM port used for RS485 communication
                              //for UBUNTU: use COMPORT = "/dev/ttyUSB0"; 
comm_settings comm_settings_t; // Communication settings structure for QB device
int i = 0; // Loop counter
short int inputs[2]; // Array to hold input values for the device

using namespace std;

// -------------------------------------------------------------------------------- sleep_us
//function waits for the specified number of number of microseconds
void sleep_us(uint32_t microseconds)
{using namespace std::chrono;
  high_resolution_clock::time_point t0 = high_resolution_clock::now();
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t1 - t0);
  do{ 
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t1 - t0);
    }
  while(time_span.count()*1000000 < microseconds);
}

// -------------------------------------------------------------------------------- main

int main()
{
    // Read a .csv file containing sinusoid samples
    string filename = "sine.csv";
    vector<vector<string>> content;
    vector<string> row;
    std::vector<double> time_stamp(10000);
    std::vector<double> sine_sample(10000);
    string line, word;
    fstream file (filename, ios::in);

    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while(getline(str, word, ','))
                row.push_back(word);
                content.push_back(row);
        }
         cout<<"\nData imported succesfully!\n\n";
    }
    else{
        cout<<"Could not open the file\n";
        }
 
    for(int i = 0; i < content.size(); i++)
    {
        sine_sample[i] = std::stod(content[i][1]);
    }

    openRS485( &comm_settings_t , COMPORT , 2000000) ;       // Open RS485 communication

    sleep_us(10000);

	using namespace std::chrono;
	auto start = std::chrono::system_clock::now();
	auto end = start;
	std::chrono::duration<double> elapsed_mseconds_t = (end -start)*1000;
    i=0;
    
    cout<<"------------------------- Start cycling ------------------------- \n";

	while(elapsed_mseconds_t.count() < 10000 ) // Loop for 10 seconds
    {
        // SoftHand with 2 motors: 
            //Send the same input to both motors to use only the 1st sinergy.

        // SoftHand with 1 motor (the SoftHand-X for example): 
            //Only the values in inputs[0] will be considered by the Firmware,  
            // In this case, set inputs[1] = 0 for a better readability.

        inputs[0] = sine_sample[i] * 16000; // Scale sine sample (value 0 to 1) for motor1 input (value 0 to 16000)
        inputs[1] = sine_sample[i] * 16000; // Duplicate input for motor2 input
        commSetInputs(&comm_settings_t, device_id, inputs); // Send inputs to device
        end = std::chrono::system_clock::now();
        elapsed_mseconds_t = (end - start) * 1000;
        i = i + 1;
        sleep_us(10000); // Delay to maintain timing
    }

    inputs[0] = 0; // Reset inputs
    inputs[1] = 0;
    commSetInputs(&comm_settings_t, device_id, inputs); // Send reset to device
    
  	closeRS485( &comm_settings_t ); // Close RS485 communication
    std::cout << "------------------------- End of Execution -------------------------n"  << std::endl;
    return 0;
}