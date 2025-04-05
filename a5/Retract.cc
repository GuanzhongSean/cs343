#include <iostream>
using namespace std;

static void CriticalSection() {
	static uBaseTask* volatile curr;  // shared
	curr = &uThisTask();
	for (unsigned int i = 0; i < 100; i += 1) {	 // work
		if (curr != &uThisTask()) {
			abort("Interference");
		}  // check
	}	   // for
}

volatile bool done = false;

_Task Retract {
   public:
	enum Intent { WantIn, DontWantIn };
	Retract(Intent & me, Intent & you) : me(me), you(you) {}

   private:
	volatile Intent &me, &you;

	void main() {
		unsigned int count = 0;
		for (unsigned int i = 0; i < 100'000'000; i += 1) {
			for (;;) {	// entry protocol
				me = WantIn;
				uFence();  // prevent hardware reordering (x86)
				if (you == DontWantIn) break;
				me = DontWantIn;
				uFence();  // prevent hardware reordering (x86)
				while (you == WantIn) {}
			}
			CriticalSection();		// critical section
			if (!done) count += 1;	// count while both threads running
			me = DontWantIn;		// exit protocol
		}							// for
		done = true;				// finished first!
		osacquire(cout) << count << endl;
	}
};

int main() {
	uProcessor p;														  // parallelism
	Retract::Intent me = Retract::DontWantIn, you = Retract::DontWantIn;  // shared
	Retract t0(me, you), t1(you, me);
}

// compile-command: u++ Retract.cc -O3 -multi -nodebug
