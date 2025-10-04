/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int current_time = 0;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {

        auto [activity, duration_intr] = parse_trace(trace);

        if (activity == "CPU")
        {
            // CPU burst
            execution += std::to_string(current_time) + ", " +
                         std::to_string(duration_intr) + ", CPU burst\n";
            current_time += duration_intr;
        }
        else if (activity == "SYSCALL")
        {
            // System call interrupt
            auto [boiler, next_time] = intr_boilerplate(current_time, duration_intr, 10, vectors);
            execution += boiler;
            current_time = next_time;

            execution += std::to_string(current_time) + ", 40, execute ISR for device " +
                         std::to_string(duration_intr) + "\n";
            current_time += 40;

            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time++;
        }
        else if (activity == "END_IO")
        {
            // End of I/O interrupt
            auto [boiler, next_time] = intr_boilerplate(current_time, duration_intr, 10, vectors);
            execution += boiler;
            current_time = next_time;

            int device_delay = delays.at(duration_intr);
            execution += std::to_string(current_time) + ", " +
                         std::to_string(device_delay) + ", complete I/O for device " +
                         std::to_string(duration_intr) + "\n";
            current_time += device_delay;

            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time++;
        }

    }

    input_file.close();

    write_output(execution);

    return 0;
}
