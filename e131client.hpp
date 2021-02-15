/*
E1.31 connection mgmt and packet mgmt
Encapsulates the C library.
*/

#ifndef E131MANAGER_H
#define E131MANAGER_H

#include "e131.h"
#include <string>
#include <vector>

class e131client {
private:
	std::string _addr; //ip address
	int _sockfd; //socket
	size_t _stripsize, _nuniverses; //strip size and number of universes needed for # LEDs
	e131_addr_t _dest; //initialize destination object w/ ip address and port.
	std::vector<e131_packet_t> _packets; //loop thru packets in case we have more than 1 universe.

	void e131prepare(); //prepare/initialize packets w/ universes and slots.
	void e131connect(); //activate unicast e131 connection
	void e131populatedmx(const std::vector<int>& pixels); //assemble dmx data in the packets before sending

public:
	e131client() = delete; //forbidden
	e131client(const std::string& _ip, size_t length);
	~e131client();

	void flush(const std::vector<int>& pixels); //send frame data to destination
};
#endif