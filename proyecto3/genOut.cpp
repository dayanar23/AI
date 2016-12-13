#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char **argv) {

    ifstream in; 
    ofstream out; 
    in.open(argv[1]);
    out.open("output.txt", std::ios::app);
    string sol; 
    in >> sol;
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int segments;

    if(sol == "UNSAT"){
    	out << "UNSAT\n";
    }

    string h[N + 1], haux;
    for (int i = 0; i < N + 1; i++) {
        haux = "";
        for (int j = 0; j < M; j++) {
            in >> segments;
            if (segments < 0) {
                haux += "0";
            }
            else {
                haux += "1";
            }
        }
        h[i] = haux;
    }

    string v[M], vaux;
    for (int i = 0; i < M; i++) {
        vaux = "";
        for (int j = 0; j < M + 1; j++) {
            in >> segments;
            if (segments < 0) {
                vaux += "0";
            }
            else {
                vaux += "1";
            }
        }
        v[i] = vaux;
    }

    out << N << " " << M << " ";
    for (int i = 0; i < N; i++) {

        out << h[i] << " ";
        out << v[i] << " ";
    }
    out << h[N] << " ";
    in.close();
    out.close();
    return 0;
}