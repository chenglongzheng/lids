/* this will be a test event, that will generate a message for every packet
 * that it handles, just to make sure everything is working.
 */

#ifndef __TEST_EVENT__
#define __TEST_EVENT__

#include <pcap.h>
#include <sys/types.h>

#include "lidsevent.h"

class TestEvent: LIDSEvent
{
      public:
            TestEvent();
            ~TestEvent();
            int process_packet(const struct pcap_pkthdr *header, const u_char *packet);
};

#endif