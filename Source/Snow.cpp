#include<Snow.h>

/*
ss::Snow::Snow() {
	process_thread = thread(&_process, this);
	process_thread.join();
}

void ss::Snow::_process() {
	cout << "process thread started" << endl;
	while (_run);
	process_thread.join();
}

void ss::Snow::quit() {
	_run = false;
	cout << "All threads stopped" << endl;
}
*/

/*
namespace ss {
	bool _run = true;
	thread process_thread(ss::_process);


	void _process() {
		cout << "process thread started" << endl;
		while (_run);
		process_thread.join();
	}

	void quit() {
		_run = false;
		cout << "All threads stopped" << endl;
	}
}
*/
