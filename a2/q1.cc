_Coroutine Filter {
  protected:
	_Exception Eof {};					// no more characters
	Filter * next;						// next filter in chain
	unsigned char ch;					// communication variable
  public:
	Filter( Filter * next ) : next( next ) {}
	void put( unsigned char c ) {
		ch = c;
		resume();
	}
};

_Coroutine Reader : public Filter {
	// YOU ADD PRIVATE MEMBERS
  public:
	Reader( Filter * f, istream & i );
};

_Coroutine Writer : public Filter {
	// YOU ADD PRIVATE MEMBERS
  public:
	Writer( ostream & o );
};

_Coroutine filter-name : public Filter {
	// YOU ADD PRIVATE MEMBERS
  public:
	filter-name( Filter * f, ... );
};
