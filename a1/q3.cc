_Coroutine FloatConstantHex {
	char ch;							// character passed by cocaller
	// YOU ADD MEMBERS HERE
	void main();						// coroutine main
  public:
	enum { EOT = '\003' };				// end of text
	_Exception Match {					// last character match
	  public:
		double value;					// floating-point value
		Match( double value ) : value( value ) {}
	};
	_Exception Error {};				// last character invalid
	void next( char c ) {
		ch = c;							// communication input
		resume();						// activate
	}
};

int main() {
	return 0;
}

/*
Test data

0x1234567890123456.p0
0xff.p-2
0x12E.5p0
0Xe2e.p0
0X00.5p0
0X.1P+123
0X12.5p0F
0xfff.fffp-5L

a
+.
X12.0
0x12.0
1.2.0a
-12.5F
gh.ff
0123456789.0123456P-0124
*/
