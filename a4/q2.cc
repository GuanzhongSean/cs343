static void CriticalSection() {
	static uBaseTask * volatile curr;					// shared
	curr = &uThisTask();
	for ( unsigned int i = 0; i < 100; i += 1 ) {		// work
		if ( curr != &uThisTask() ) { abort( "Interference" ); } // check
	} // for
}

enum Intent { WantIn, DontWantIn };

_Task Tsay {
	static Intent * volatile Last;
	Intent & me, & you;

	void main() {
		for ( unsigned int i = 0; i < 10'000'000; i += 1 ) {
			me = WantIn;								// entry protocol
			Last = &me;									// race
			uFence();									// prevent hardware reordering (x86)
			if ( you == WantIn )
				while ( Last == &me ) {}
			CriticalSection();							// critical section
			me = DontWantIn;							// exit protocol
			Last = &me;
		}
	}
  public:
	Tsay( Intent & me, Intent & you ) : me( me ), you( you ) {}
};
Intent * volatile Tsay::Last;

int main() {
	uProcessor p;
	Intent me = DontWantIn, you = DontWantIn;			// shared
	Tsay t0( me, you ), t1( you, me );
}
