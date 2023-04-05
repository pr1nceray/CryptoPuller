#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
//made for easier input /output opts



enum class input_opts{
    BUY,
    SELL,
    INFO,
    UPDATE,
    QUIT
};


struct user_opt{
    bool is_verbose = false;
    int update_timeframe = 60;

};

class reader{
    public:
    reader(user_opt & opt_in):is_verbose(opt_in.is_verbose),opt(opt_in){
        if(!opt.is_verbose){
            return;
        }

        std::cout << "Starting non headless - mode. \n";
        std::cout << "Accepts input on buying/selling/price.\n";
        std::cout << "Options : \n";
        std::cout << "BUY <num_orders> <coin> for <price>  - creates num_orders buy orders at price for specified coin.\n";
        std::cout << "SELL <num_orders> <coin> for <price>  - creates num_orders sell orders at price for specified coin.\n";
        std::cout << "INFO <opt>- Displayes current holdings, and the price of the current holding. Opt specifices the amount of information, from 0 -> 2\n";
        std::cout << "UPDATE <interval> - changes how often you would like to see updates on the price of the coin. minimum of one minute\n";
    }

    static input_opts read_message(){
        string cur_input;
        cin >> cur_input;
        //should also do lower case opts.
        switch (cur_input.at(0)) {
            case 'B'  | 'b':{
                return input_opts::BUY;
                break; 
            }
            case 'S' | 's' :{
                return input_opts::SELL;
                break; 
            }
            case 'U'  | 'u':{
                return input_opts::UPDATE;
                break; 
            }
            case 'I' | 'i':{
                return input_opts::INFO;
                break;
            }
            case 'Q' | 'q':{
                return  input_opts::QUIT;
                break;
            }
            default:{
                std::cout << "unknown option; quiting.\n";
                getline(cin, cur_input);
                return input_opts::QUIT;
                break;
            }

        }
    }

    private:
    
    void update_timeframe(){
        int time_frame;
        cin >> time_frame;
        if(time_frame < 70){
            std::cout << "Failed to update time interal to " <<  time_frame << ". Time interval provided is too low.\n";
            return;
        }
        opt.update_timeframe = time_frame;
        if(!opt.is_verbose){
            return;
        }
        std::cout << "Updated time interval to update on to" <<  time_frame << "\n";
    }




    bool is_verbose;
    user_opt & opt;

};