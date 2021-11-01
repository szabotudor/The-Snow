#include<Snow.h>

void ss::_process() {
	cout << "process thread started";
}

namespace ss {
	thread process_thread(ss::_process);

	void quit() {
		process_thread.join();
	}
}
