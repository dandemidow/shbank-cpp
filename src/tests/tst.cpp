#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE shmobank

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "message_bank.h"
#include "message.h"


BOOST_AUTO_TEST_SUITE(SharedMemorySuite)

BOOST_AUTO_TEST_CASE(TestPushPopMessage)
{
    pid_t pid = fork();
    if ( pid )
    {
        shared_memory mem;
        mem.open("acars_banks");
        message_bank<producer,message>bank(mem,special_bank_tags::playback);
        bank.wait_connect();
        bank.wait_activate();
        msgblk_t *msg = (msgblk_t*)malloc(sizeof(msgblk_t));
        msg->lvl = 10;
        msg->next = 0;
        msg->txt[0] = '0';
        msg->txt[1] = '1';
        msg->txt[2] = '2';
        msg->txt[3] = '\0';
        msg->err = 0;
        msg->len = 4;
        bank.push_message(message::transform(msg));
        return;
    }
    shared_memory joined_mem;
    joined_mem.join("acars_banks");
    message_bank<consumer,message>bank(joined_mem,special_bank_tags::playback);
    auto mess = bank.pop_message();
    msgblk_t *msg = message::transform(mess);
    BOOST_REQUIRE_EQUAL(msg->lvl,10);
    BOOST_REQUIRE_EQUAL(msg->err,0);
    BOOST_REQUIRE_EQUAL(msg->len,4);
    BOOST_REQUIRE_EQUAL(msg->txt[0],'0');
    BOOST_REQUIRE_EQUAL(msg->txt[1],'1');
    BOOST_REQUIRE_EQUAL(msg->txt[2],'2');
    int status;
    waitpid(pid, &status, 0);

}
BOOST_AUTO_TEST_SUITE_END()
