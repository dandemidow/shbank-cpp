#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE shmobank
#include <iostream>
#include <thread>

#include "default/producer.hpp"
#include "default/consumer.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

BOOST_AUTO_TEST_SUITE(SharedMemorySuite)


BOOST_AUTO_TEST_CASE(TestRawProcedurePushPopMessage) {
  shmobank::producer mem("acars_bank");
  auto playbank = mem.create<shmobank::rawbank>(shmobank::tags::playback, 2);
  msg::error err;
  auto ms = playbank.prep(err);
  BOOST_REQUIRE(ms);
  playbank.activate();
  ms->txt[0] = 'X';
  ms->txt[1] = '\0';
  bool ok = playbank.push(ms);
  BOOST_REQUIRE(ok);
  auto msg_1 = playbank.pop();
  BOOST_CHECK(msg_1);
  if (msg_1) {
    playbank.free(msg_1);
    BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char*>(msg_1->txt)), "X");
  } else {
    BOOST_FAIL("msg_1 should be not null");
  }
}
BOOST_AUTO_TEST_CASE(TestProcedurePushPopMessage) {
  shmobank::producer mem("acars_bank");
  auto playbank = mem.create<shmobank::bank>(shmobank::tags::playback, 2);
  msg::error err;
  msgblk_t ms;
  playbank.activate();
  ms.txt[0] = 'X';
  ms.txt[1] = '\0';
  bool ok = playbank.push(ms);
  BOOST_REQUIRE(ok);
  auto msg_1 = playbank.pop();
  if (msg_1.txt) {
    BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char*>(msg_1.txt)), "X");
  } else {
    BOOST_FAIL("msg_1 should be not null");
  }
}
BOOST_AUTO_TEST_CASE(TestPushPopMessage)
{
  { // open and close this shared memory for cleaning only
    shared_memory<Producer> mem("acars_bank");
  }
  shmobank::producer mem("acars_bank");
  auto playbank = mem.create<shmobank::bank>(shmobank::tags::playback, 2);

  // and now we start the cunsumer!
  std::thread thr([](){
    shmobank::consumer mem("acars_bank");
    auto playbank = mem.create<shmobank::bank>(shmobank::tags::playback);
    msgblk_t msg;
    msg.lvl = 10;
    msg.next = 0;
    msg.txt[0] = '0';
    msg.txt[1] = '1';
    msg.txt[2] = '2';
    msg.txt[3] = '\0';
    msg.err = 0;
    msg.len = 4;
    playbank.push(msg);
    usleep(100);
  });

  mem.wait_join();
  playbank.wait_activating();
  auto msg = playbank.pop();
  BOOST_REQUIRE_EQUAL(msg.lvl,10);
  BOOST_REQUIRE_EQUAL(msg.err,0);
  BOOST_REQUIRE_EQUAL(msg.len,4);
  BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char*>(msg.txt)), "012");
  mem.wait_unjoin();
  thr.join();
}

BOOST_AUTO_TEST_SUITE_END()
