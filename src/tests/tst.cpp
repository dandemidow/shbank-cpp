#include "message_bank.h"
#include "message.h"

int main(){
    shared_memory mem,mem2;
    mem.open("acars_bank");
    message_bank<producer,message>bank(mem,special_bank_tags::playback,2);
    bank.wait_connect();
    bank.wait_activate();
    mem2.join("acars_bank");
    message_bank<consumer,message>bank_2(mem2,special_bank_tags::playback,2);
    bank_2.wait_activate();
}
