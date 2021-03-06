#include <pcap.h>
#include <iostream>
#include <sys/types.h>

#include "lidsrunner.h"
#include "lidsdebug.h"
#include "packet_wrap.h"

using namespace std;

/* global pointer to our core, declared in lids.cxx */
extern LIDSCore *pcore;

LIDSRunner::LIDSRunner()
{
}

LIDSRunner::~LIDSRunner()
{
}

void LIDSRunner::callback(u_char *args, const struct pcap_pkthdr *header,
            const u_char *packet)
{
      IN();
      cout << ".";
      fflush(stdout);
	pcore->core_process(header, packet);

	const struct port_test *Xdp;
	/* And define the size of the structures we're using */
	u_short sport = 0;
	u_short dport = 0;

	/* -- Define our packet's attributes -- */
	Xdp = (struct port_test*)(packet);

	/* TODO: find the right offset for source port. */

	sport = Xdp->th_sport;
	if (sport != 0)
		printf("sport: %d\n",sport);
	dport = Xdp->th_dport;
	if (dport != 0)
		printf("dport: %d\n",dport);
	OUT();
}

void LIDSRunner::start()
{
	IN();
	/* initialize the class static core class */
	pcap_t* handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_program filter;
	bpf_u_int32 net;
	bpf_u_int32 mask;
	char *dev;

	dev = pcap_lookupdev(errbuf);

	if(dev == NULL) {
		cout << "pcap_lookupdev(): " << errbuf << endl;
		exit(1);
	}

	/* print out device name */
	//dev = "en1";
	cout << "Dev: " << dev << endl;

	pcap_lookupnet(dev,&net,&mask,errbuf);

	handle = pcap_open_live(dev,BUFSIZ,1,0,errbuf);

	if(handle == NULL) {
		cout << "pcap_open_live(): " << errbuf << endl;
		exit(1);
	}

	if(pcap_compile(handle,&filter,"tcp",0,net) == -1) {
		cout << "Error calling pcap_compile" << endl;
		exit(1);
	}

	/* set the compiled program as the filter */
	if(pcap_setfilter(handle,&filter) == -1) {
		cout << "Error calling pcap_setfilter" << endl;
		exit(1);
	}

	cout << "Capturing..." << endl;
	pcap_loop(handle,-1,(pcap_handler)callback,NULL);

	free(handle);
	free(dev);
	OUT();
}

