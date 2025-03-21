#ifndef __AUTOMATICSIGNAL_H__
#define __AUTOMATICSIGNAL_H__

#define AUTOMATIC_SIGNAL uCondition bench

/* From Table 9.4 */
// #define WAITUNTIL(pred, before, after) \
// 	if (!(pred)) {                     \
// 		while (!bench.empty()) {       \
// 			bench.signal();            \
// 		}                              \
// 		do {                           \
// 			before;                    \
// 			bench.wait();              \
// 			after;                     \
// 		} while (!(pred));             \
// 	}

// #define EXIT() \
// while (!bench.empty()) bench.signal();

// *************************************
/* From Table 9.5 */
#define WAITUNTIL(pred, before, after) \
	while (!(pred)) {                  \
		bench.signalBlock();           \
		if (pred) break;               \
		before;                        \
		bench.wait();                  \
		after;                         \
	}

#define EXIT() bench.signalBlock();

#endif	// __AUTOMATICSIGNAL_H__
