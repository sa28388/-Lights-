/*
E1.31 connection mgmt and packet mgmt
Encapsulates the C library.

Note: The first spot in the dmp prop val array (size=513) is the dmx start code, and then the remaining slots are the data.
But for some reason, the packet(s) in the next universe(s) don't need a start code and start on index 0 instead of 1. Also, when I init the packet w/ 510 slots, 
WLED still looks in the last 2 slots in the array (511, 512) for data. So that means if I start universe 2 packet at index 1, it will skip 
one LED in the strip between the end of one universe and start of the next (511 = r, 512 = g, 0 = b). 
So I am filling index 1-512 in universe 1 then 0-512 in subsequent universes, and don't care about the universe size or slots i init the packet with.
*/

#include "e131client.hpp"

constexpr auto UNIVERSE_SIZE = 512; //Normally should be 510. 

e131client::e131client(const std::string& _ip, size_t length) :
	_addr(_ip),
	_sockfd(e131_socket()),
	_stripsize(length),
	_nuniverses(((length * 3) + UNIVERSE_SIZE) / UNIVERSE_SIZE) //Round to # universes. Hope no wrong answers for edge cases of strip size?!
{
	e131prepare();
	e131connect();
}

e131client::~e131client() {}

void e131client::e131prepare() {
	//populate the vector w/ the right amt of packets for the number of universes
	for (size_t i = 0; i < _nuniverses; ++i) {
		_packets.push_back(e131_packet_t());
		e131_pkt_init(&_packets[i], i + 1, UNIVERSE_SIZE); //Initialize all packets w/ 512 slots, and appropriate universe number
	}
}

void e131client::e131connect() {
	//Currently unicast support only :D I dont know anything else
	e131_unicast_dest(&_dest, _addr.c_str(), E131_DEFAULT_PORT); //Default is Port 5568 (see e131.c)
}

void e131client::e131populatedmx(const std::vector<int>& pixels) {
	//get the data from the strip and distribute the frame data into the e131 packet(s).
	//the vector that's sent is already flattened, so it should be stripsize*3. (ledstrip.h has flattening function)
	auto it = pixels.cbegin(), end = pixels.cend();

	size_t currpacket = 0, //I have to keep track whether a packet is full.
		currpos = 1; //make sure we start at position 1 in the first universe (only the first universe)
		
	while (it != end) {
		_packets[currpacket].dmp.prop_val[currpos] = *it; //put the value into position of the current packet.
		++it; ++currpos;

		//If we fill a packet's dmx data, rollover to the next packet and reset the position counter.
		//However, universe 2 will start at index 0 instead of 1, b/c we don't have to send the DMX start code
		//at the beginning of the data apparently
		if (currpos > UNIVERSE_SIZE) {
			currpacket++;
			currpos = 0;
		}
		//shouldn't go out of bounds w/ the packet, b/c we will reach end of pixel array first. 
	}
}

void e131client::flush(const std::vector<int>& pixels) {
	//first put the frame data from pixels into dmx field of the packets
	//pixels is already a flattened array.
	e131populatedmx(pixels);

	//now send each packet and increment all frame counters.
	for (auto& packet : _packets) {
		e131_send(_sockfd, &packet, &_dest);
		++packet.frame.seq_number;
	}
}
