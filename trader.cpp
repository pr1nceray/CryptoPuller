



#include <jsoncpp/json/reader.h>
#include <vector>
#include <map>
#include "comms.h"

using namespace std;



int main(){
    string url = "https://sandbox-api.coinmarketcap.com/v1/cryptocurrency/listings/latest";
    string file_name = "test_output.json";
    
    try{
        COM curl_interface(url,file_name );
        //curl_interface.send_msg();
    }
    catch(curl_exception & e){
        std::cout << e.what();
    }
}
