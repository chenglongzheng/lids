/*
 * LIDSCore will be the core of the lids system. Eventually it will be able to
 * capture packets using pcap, hand them off the event handlers and manage
 * threads
 */

class LIDSCore {
      public:
            /* initializer */
            LIDSCore();
            /* destructor */
            ~LIDSCore();

            /* set up (initialize) the LIDS Core */
            int setup();
            /* register an event handler with the Core */
            int register_event(LIDSEvent *e);
            /* callback function for libpcap to call */
            void callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
            /* store packet info a/o metadata */
            int store(const struct pcap_pkthdr *header, const u_char *packet);
            /* kick off the server */
            int run();
};

