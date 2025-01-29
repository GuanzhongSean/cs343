#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#include <malloc.h>										// malloc_stats

int main( int argc, char * argv[] ) {
	intmax_t times = 1'000'000, size = 20;				// defaults
	const char * nosummary = getenv( "NOSUMMARY" );		// print summary ?
	struct cmd_error {};								// command-line errors

	try {
		switch ( argc ) {
		  case 3: size = convert( argv[2] ); if ( size <= 0 ) { throw cmd_error(); }
		  case 2: times = convert( argv[1] ); if ( times <= 0 ) { throw cmd_error(); }
		  case 1: break;								// use defaults
		  default: throw cmd_error();
		} // switch
	} catch( ... ) {
		cerr << "Usage: " << argv[0] << " [ times (> 0) [ size (> 0) ] ]" << endl;
		exit( EXIT_FAILURE );
	} // try

	#if defined( ARRAY )
	struct S { long long int i, j, k, l; };
	S buf[size];										// internal-data buffer
	#elif defined( STRING )
	string strbuf;										// external-data buffer
	#elif defined( STRSTREAM )
	stringstream ssbuf;									// external-data buffer
	#else
		#error unknown buffering style
	#endif

	enum { C = LLONG_MAX };								// print multiple characters
	int trick = rand();									// prevent dead-code removal

	for ( int t = 0; t < times; t += 1 ) {
		#if defined( ARRAY )
		for ( int s = 0; s < size; s += 1 ) {
			buf[s] = (S){ C, C, C, C };
			if ( trick == 0 ) cout << buf[s].i;
		} // for
		#elif defined( STRING )
		for ( int s = 0; s < size; s += 1 ) {
			strbuf += to_string(C) + '\t' + to_string(C) + '\t'
				+ to_string(C) + '\t' + to_string(C) + '\t';
			if ( trick == 0 ) cout << strbuf;
		} // for
		strbuf.clear();									// reset string
		#elif defined( STRSTREAM )
		for ( int s = 0; s < size; s += 1 ) {
			ssbuf << C << '\t' << C << '\t' << C << '\t' << C << '\t';
			if ( trick == 0 ) cout << ssbuf.str();
		} // for
		ssbuf.seekp( 0 );								// reset stream
		#else
			#error unknown buffering style
		#endif
	} // for

	if ( ! nosummary ) { malloc_stats(); }				// print heap statistics
}
