#include <jsoncpp/json/reader.h>
#include <vector>
#include <map>
#include "comms.h"
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
    string file_name = "test_output.json";
    
    reader read(opt);
    
    input_opts current = read.read_message();
    while (current != input_opts::QUIT) {
        //do action on current.
        //every time frame, get updates.
        current = read.read_message();

    }

    try{
        COM curl_interface(url,file_name );
        curl_interface.send_msg();
    }
    catch(curl_exception & e){
        std::cout << e.what();
    }
}
