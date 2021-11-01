#include<Snow.h>

namespace ss {
	bool _run = true;
	thread process_thread(ss::_process);


	void _process() {
		cout << "process thread started" << endl;
		while (_run);
	}

	void quit() {
		_run = false;
		process_thread.join();
		cout << "All threads stopped" << endl;
	}
}
