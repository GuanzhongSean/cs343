#ifndef Q1BASEFILTER_H
#define Q1BASEFILTER_H

_Coroutine Filter {
   protected:
	_Exception Eof{};  // Exception to signal end-of-file
	Filter *next;	   // Next filter in the pipeline
	unsigned char ch;  // Communication variable

   public:
	Filter(Filter * next) : next(next) {}
	~Filter() {
		delete next;
	}
	virtual void put(unsigned char c) {
		ch = c;
		resume();
	}
};

#endif	// Q1BASEFILTER_H
