#include <iostream>
#include <typeinfo>

#include <uCobegin.h>
#include <uActor.h>

using namespace std;

_Exception a{};
_Exception b : public a{};
_Exception c{};

void foo(a& a1) {
    try {
        _Resume a1;
    } catch (b&) {
        cout << "b0" << endl;
        _Throw;
    }
}

void goo() {
    cout << ((&uThisCoroutine()) == (&uThisTask())) << endl;
    cout << "coroutine:" << typeid(uThisCoroutine()).name() << " " << &uThisTask() << endl;
}

_Coroutine X {
    void main() {
        goo();
    }
public:
    void operator()() {
        resume();
    }
};

_Actor Y {
    Allocation receive(Message& msg) {
        Case(StopMsg, msg) {
            goo();
            uThisTask().yield();
            return Delete;
        }
        return Nodelete;
    }
};

int main() {
    uProcessor p[4];
    b b1;
    try {
        foo(b1);
    } _CatchResume (b& b1) {
        cout << "b1" << endl;
        _Throw b1;
    } catch (b&) {
        cout << "b2" << endl;
    } catch (a&) {
        cout << "a" << endl;
    }
    COBEGIN
        BEGIN goo(); END
        BEGIN goo(); END
    COEND
    auto tp = START( goo );
    auto tf = START( goo );
    WAIT(tp);
    WAIT(tf);
    goo();
    X x;
    x();
    uActor::start();
    *new Y() | uActor::stopMsg;
    *new Y() | uActor::stopMsg;
    uActor::stop();
    COFOR(i, 0, 3,
        goo();
    );
    cout << "end" << endl;
    return 0;
}
