#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){
    if(argc!=2)
    {
        cout << "usage splflix input_file" << endl;
        return 0;
    }
    Session* s = new Session(argv[1]);
//    Session* s = new Session("/users/studs/bsc/2020/igale/CLionProjects/Assignment1_0/config2.json");
    s->start();
    Session* s2 = new Session(argv[1]);
//    Session s2 = std::move(*s);
//    s2.start();
    s2->start();
    *s2 = std::move(*s);
    delete(s);
    s2->start();
    delete(s2);
//    Session s3 = *s;
//    Session s2 = *s;
//    Session s4 = std::move(s->test());
//    delete (s);
//    s2.start();
//    s3=s2;
//    s3.start();
//    s2.start();
    return 0;
}

