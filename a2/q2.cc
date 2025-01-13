_Coroutine Player {
	// YOU ADD PRIVATE MEMBERS, INCLUDING STATICS
  public:
	enum { DEATH_DECK_DIVISOR = 7 };
	static void players( unsigned int num );
	Player( PRNG & prng, unsigned int id, Printer & printer );
	void start( Player & lp, Player & rp );				// supply partners
	void play( unsigned int deck );
	void drink();
};

class Printer {
    struct PImpl;										// hidden implementation
    PImpl * pimpl;
  public:
    Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards );
	void print( unsigned int id, int took, unsigned int RemainingPlayers );  // card play
	void print( unsigned int id );  // drink (Schmilblick)
};
