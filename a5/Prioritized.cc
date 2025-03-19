#include <iostream>
using namespace std;

static void CriticalSection() {
	static uBaseTask * volatile curr;					// shared
	curr = &uThisTask();
	for ( unsigned int i = 0; i < 100; i += 1 ) {		// work
		if ( curr != &uThisTask() ) { abort( "Interference" ); } // check
	} // for
}

volatile bool done = false;

_Task Prioritized {
  public:
	enum Intent { WantIn, DontWantIn };
	enum Priority { HIGH, low };
	Prioritized( Priority priority, Intent & me, Intent & you ) : priority( priority ),  me( me ), you( you ) {}
  private:
	Priority priority;
	volatile Intent & me, & you;

	void main() {
		unsigned int count = 0;
		for ( unsigned int i = 0; i < 100'000'000; i += 1 ) {
			for ( ;; ) {								// entry protocol
				me = WantIn;
				uFence();								// prevent hardware reordering (x86)
			  if ( you == DontWantIn ) break;
				if ( priority == low ) {
					me = DontWantIn;
					uFence();							// prevent hardware reordering (x86)
					while ( you == WantIn ) {}
				}
			}
			CriticalSection();							// critical section
			if ( ! done ) count += 1;					// count while both threads running
			me = DontWantIn;							// exit protocol
		} // for
		done = true;									// finished first!
		osacquire( cout ) << count << endl;
	}
};

int main() {
	uProcessor p;										// parallelism
	Prioritized::Intent me = Prioritized::DontWantIn, you = Prioritized::DontWantIn; // shared
	Prioritized t0( Prioritized::HIGH, me, you ), t1( Prioritized::low, you, me );
}

// compile-command: u++ Prioritized.cc -O3 -multi -nodebug
