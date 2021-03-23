//#include "audiostream.hpp"
#include "audiomanager.h"
#include <iostream>
#include <atomic>
#include <iostream>
#include <thread>

audiostream a;
std::atomic_bool active = true;
auto threadfunc = [&]() -> void {
	while (active) {
		ring_buffer_size_t elementsinbuff = PaUtil_GetRingBufferReadAvailable(a.buffer());
		std::cout << elementsinbuff << " elements in buff\n";

		if (elementsinbuff >= a.buffer()->bufferSize / 4) {
			void* ptr[2] = { 0 };
			ring_buffer_size_t sizes[2] = { 0 };

			/* By using PaUtil_GetRingBufferReadRegions, we can read directly from the ring buffer */
			ring_buffer_size_t elementsRead = 
				PaUtil_GetRingBufferReadRegions(a.buffer(), elementsinbuff, ptr + 0, sizes + 0, ptr + 1, sizes + 1);
			std::cout << elementsRead << " elements read\n";

			if (elementsRead > 0) {
				for (int i = 0; i < 2 && ptr[i] != nullptr; ++i) std::cout << ptr[i] << '\n';
			}
		}
	}

};

int main(void)
{
	/*/
	Pa_Initialize();

	a.start();

	std::thread t(threadfunc);
	
	using namespace std::literals;
	std::this_thread::sleep_for(5s);
	
	active = false;
	t.join();

	a.stop();

	Pa_Terminate();
	*/

	audiomanager a;

	for (const auto& dev : a.devices()) std::cout << dev.first << " " << dev.second << "\n";


	return 0;
}

