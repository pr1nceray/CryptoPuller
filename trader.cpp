#include <jsoncpp/json/reader.h>
#include <chrono>
#include <ratio>
#include <vector>
#include <map>
#include <future>

#include "comms.h"
#include "clock.h"
#include "reader.h"
#include "getopt.h"


void get_options(int argc, char** argv, user_opt& opt) {
    option long_opts[] {
        { "help", no_argument, nullptr,  'h'},
        {"verbose", no_argument, nullptr,  'v'},
        {nullptr,           0, nullptr, '\0'}
    };

    int choice;
    int index = 0;


    while ((choice = getopt_long(argc, argv, "hv", long_opts, &index)) != -1) {
        switch (choice) {
        case 'h': {
            cout << "A helpfull message";
            exit(0);
            break;
        }
        case 'v': {
            opt.is_verbose = true;
            break;
        }
        default: {
            std::cout << "Error: Unknown command line option";
            exit(1);
        }
        }
    }
}



using namespace std;



int main(int argc, char** argv){
    std::ios_base::sync_with_stdio(false);
    user_opt opt;

    get_options(argc, argv, opt);

    string url = "https://sandbox-api.coinmarketcap.com/v1/cryptocurrency/listings/latest";
    string file_name = "test_output";
    
    reader read(opt);
    


    std::future<input_opts> current;
    input_opts loop_opt = input_opts::INFO;

    //timer_c loop_clock(opt.update_timeframe);
    timer_c loop_clock(15);

    try{
        COM curl_interface(url,file_name);

        while(loop_opt != input_opts::QUIT){
            //current = std::async(read.read_message);

            //if the current time is ready for a status update, then do the following loop
            if(loop_clock.is_ready()){
                try{
                    curl_interface.send_msg();
                    loop_clock.reset_update();
                }
                catch(curl_exception & e){
                    std::cout << e.what();
                }
            }
            //loop_clock.print_diff();
        }
    }
    catch(curl_exception & e){
                std::cout << e.what();
    }
}
