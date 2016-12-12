#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
N: rows
M: columns
S: # segments 
D: dimention (N*M)
*/
int N, M, S, D;

int q(int i, int j, char dir) {
    switch (dir) {
        case 'n': return (j + (i-1) * M);
        case 's': return (j + i * M);
        case 'e': return (j + (i-1) * (N+1) + M * (N+1) + 1);
        case 'w': return (j + (i-1) * (N+1) + M * (N+1));
    }
    return 0;
}

int z(int i, int j) {
    return (j + (i-1) * M + S);
}

int r(int i, int j, int i1, int j1) {
    int auxi = (i-1)* M + j;
    int auxj = (i1-1)* M + j1;
    return ((auxi -1) * D + auxj + D + S);
}

int main(int argc, const char *argv[]) {

    ifstream in; 
    ofstream out; 
    in.open(argv[1]);
    out.open("aux.txt");
    in >> N;
    in >> M;
    D = N*M;
    S =  (N +1) * M + (M +1) * N;
    int numV = S + D;
    int numC = 0;
    char cell;
    char n = 'n';
    char e = 'e';
    char s = 's';
    char w = 'w';

    /*  
            Tipo 1, 2 y 3
    */

    for (int i = 1; i <= N; i++) {       
        for (int j = 1; j <= M; j++) {
            in >> cell;

            /*
                Tipo 0
            */

            if(i<N){
                out << "c Tipo 0: \n";
                out << q(i,j,s) << " " << -q(i+1, j,n) << " 0\n";
                out << -q(i,j,s) << " " << q(i+1, j, n) << " 0\n";
                numC = numC + 2;
            }

            if(j<M){
                out << "c Tipo 0: \n";
                out << q(i,j,e) << " " << -q(i,j+1,w) << " 0\n";
                out << -q(i,j,e) << " " << q(i, j+1, w) << " 0\n";
                numC = numC + 2;
            } 

            /*
                Tipo1
            */
    
            out << "c Tipo 1: \n";//

            switch (cell) {
                case '0':
                        out << -q(i,j,n) << " 0\n";
                        out << -q(i,j,e) << " 0\n";
                        out << -q(i,j,s) << " 0\n";
                        out << -q(i,j,w) << " 0\n";
                        numC = numC + 4;
                    break;//

                case '1':
                out << " caso 1: ";
                        out <<  q(i,j,n) << " " << q(i,j,s) << " " << q(i,j,e) << " " << q(i,j,w) << " 0\n";
                        out << -q(i,j,n) << " " << -q(i,j,s) << " 0\n";
                        out << -q(i,j,n) << " " << -q(i,j,e) << " 0\n";
                        out << -q(i,j,n) << " " << -q(i,j,w) << " 0\n";
                        out << -q(i,j,e) << " " << -q(i,j,s) << " 0\n";
                        out << -q(i,j,e) << " " << -q(i,j,w) << " 0\n";
                        out << -q(i,j,s) << " " << -q(i,j,w) << " 0\n";
                        numC = numC + 7;
                    break;
                case '2': 
                out << " caso 2: ";
                        out << q(i,j,n) << " " << q(i,j,e) << " " << q(i,j,s) << " 0\n";
                        out << q(i,j,n) << " " << q(i,j,e) << " " << q(i,j,w) << " 0\n";
                        out << q(i,j,n) << " " << q(i,j,s) << " " << q(i,j,e) << " 0\n";
                        out << q(i,j,n) << " " << q(i,j,s) << " " << q(i,j,w) << " 0\n";
                        out << q(i,j,n) << " " << q(i,j,w) << " " << q(i,j,s) << " 0\n";
                        out << q(i,j,n) << " " << q(i,j,w) << " " << q(i,j,e) << " 0\n";
                        out << q(i,j,e) << " " << q(i,j,s) << " " << q(i,j,n) << " 0\n";
                        out << q(i,j,e) << " " << q(i,j,s) << " " << q(i,j,w) << " 0\n";
                        out << q(i,j,e) << " " << q(i,j,w) << " " << q(i,j,n) << " 0\n";
                        out << q(i,j,e) << " " << q(i,j,w) << " " << q(i,j,s) << " 0\n";
                        out << q(i,j,s) << " " << q(i,j,w) << " " << q(i,j,n) << " 0\n";
                        out << q(i,j,s) << " " << q(i,j,w) << " " << q(i,j,e) << " 0\n";
                        numC = numC + 12;
                    break;
                case '3':
                out << " caso 3: ";
                        out << -q(i,j,n) << " " << -q(i,j,s) << " " << -q(i,j,e) << " " << -q(i,j,w) << " 0\n";
                        numC = numC + 1;
                    break;
                case '4':
                out << " caso 4: ";
                        out <<  q(i,j,n) << " 0\n";
                        out <<  q(i,j,s) << " 0\n";
                        out <<  q(i,j,e) << " 0\n";
                        out <<  q(i,j,w) << " 0\n";
                        numC = numC + 4;
                    break;
            }
            /*
                Tipo 3
            
            */
            out << "c Tipo 3: \n";
            out << r(i,j,i,j) << " 0\n";
            for (int i1 = 1; i1 <= N; i1++) {
                for (int j1 = 1; j1 <= M; j1++) {
                    if (i1 - 1 > 1) {
                        out << -r(i,j,i1,j1) << " " << q(i1,j1,n) << " " << r(i,j,i1-1,j1) << " 0\n";
                        numC = numC + 1;
                    }
                    if (j1 + 1 < M) {
                        out << -r(i,j,i1,j1) << " " << q(i1,j1,e) << " " << r(i,j,i1,j1+1) << " 0\n";
                        numC = numC + 1;
                    }
                    if (i1 + 1 < N ) {
                        out << -r(i,j,i1,j1) << " " << q(i1,j1,s) << " " << r(i,j,i1+1,j1) << " 0\n";
                        numC = numC + 1;
                    }
                    if (j1 - 1 > 1) {
                        out << -r(i,j,i1,j1) << " " << q(i1,j1,w) << " " << r(i,j,i1,j1-1) << " 0\n";
                        numC = numC + 1;
                    }
                    /*
                        Tipo 4 
                    
                    */
                    out << "c Tipo 4: \n";
                
                    out << -z(i,j) << " " << -z(i1,j1) << " " << r(i,j,i1,j1) << " 0\n";
                    numC = numC + 1;
                }
            }
        }  
    }
    
    /*  
            Tipo 2
    */

    /*  
            Horizontales
    */

    for(int j=1; j<= M; j++){
        out << "c Tipo 2: \n";
        out <<  q(1,j,n) << " " << z(1,j) << " 0\n";
        out << -q(1,j,n) << " " <<  -z(1,j) << " 0\n";
        numC = numC + 2;
        out <<  q(N,j,s) << " " <<  z(N,j) << " 0\n";
        out << -q(N,j,s) << " " << -z(N,j) << " 0\n";
        numC = numC + 2;
    }

    /*  
            Verticales
    */

    for(int i=1; i<= N; i++){
        out << "c Tipo 2: \n";
        out <<  q(i,1,w) << " " << z(i,1) << " 0\n";
        out << -q(i,1,w) << " " <<  -z(i,1) << " 0\n";
        numC = numC + 2;
        out <<  q(i,M,e) << " " <<  z(i,M) << " 0\n";
        out << -q(i,M,e) << " " << -z(i,M) << " 0\n";
        numC = numC + 2;
    }

    /*  
            Centrales
    */

    for(int i=2; i< N; i++) {
        for(int j=2; j< M; j++){
            out << "c Tipo 2: \n";
            out << -q(i,j,e) << " " << -q(i,j,w) << " 0\n";
            out << z(i,j+1) << " " << -q(i,j,w) << " 0\n";
            out << -q(i,j,e) << " " << z(i,j-1) << " 0\n";
            out << z(i,j+1) << " " << z(i,j-1) << " 0\n";
            out << -z(i,j) << " " << -q(i,j,n) << " " << -q(i,j,s) << " 0\n";
            out << -z(i,j) << " " << z(i-1,j) << " " << -q(i,j,s) << " 0\n";
            out << -z(i,j) << " " << -q(i,j,n) << " " << z(i+1,j) << " 0\n";
            out << -z(i,j) << " " << z(i-1,j) << " " << z(i+1,j) << " 0\n";
            out << z(i,j) << " " << q(i,j,n) << " " << -z(i-1,j) << " 0\n";
            out << z(i,j) << " " << q(i,j,s) << " " << -z(i+1,j) << " 0\n";
            out << z(i,j) << " " << q(i,j,e) << " " << -z(i,j+1) << " 0\n";
            out << z(i,j) << " " << q(i,j,w) << " " << -z(i,j-1) << " 0\n";

            numC = numC + 12;
        } 
    }   

    /*
        Tipo 5 
   */ 

    /*  
            Esquinas
    */

    out << "c Tipo 5: \n";
    out << -q(1,1,n) << " " << q(1,1,w) << " 0\n";
    out << -q(1,1,w) << " " << q(1,1,n) << " 0\n";
    out << -q(1,M,n) << " " << q(1,M,e) << " 0\n";
    out << -q(1,M,e) << " " << q(1,M,n) << " 0\n";
    out << -q(N,1,s) << " " << q(N,1,w) << " 0\n";
    out << -q(N,1,w) << " " << q(N,1,s) << " 0\n";
    out << -q(N,M,s) << " " << q(N,M,e) << " 0\n";
    out << -q(N,M,e) << " " << q(N,M,s) << " 0\n";
    
    numC = numC + 8; 
    
    /*  
            Horizontales
    */

    for (int j = 2; j < M; j++ ) {
        out << -q(1,j,n) << " " <<  q(1,j,e) << " " <<  q(1,j+1,n) << " 0\n";  
        out <<  q(1,j,n) << " " << -q(1,j,e) << " " <<  q(1,j+1,n) << " 0\n"; 
        out <<  q(1,j,n) << " " <<  q(1,j,e) << " " << -q(1,j+1,n) << " 0\n";
        out << -q(N,j,s) << " " <<  q(N,j,e) << " " <<  q(N,j+1,s) << " 0\n";
        out <<  q(N,j,s) << " " << -q(N,j,e) << " " <<  q(N,j+1,s) << " 0\n";
        out <<  q(N,j,s) << " " <<  q(N,j,e) << " " << -q(N,j+1,s) << " 0\n";
        numC = numC + 6;
    }


    /*  
            Verticales
    */

    for (int i = 2; i < N; i++ ){
        out << -q(i,M,e) << " " <<  q(i,M,s) << " " <<  q(i+1,M,e) << " 0\n";
        out <<  q(i,M,e) << " " << -q(i,M,s) << " " <<  q(i+1,M,e) << " 0\n";
        out <<  q(i,M,e) << " " <<  q(i,M,s) << " " << -q(i+1,M,e) << " 0\n";
        out << -q(i,1,w) << " " <<  q(i,1,s) << " " <<  q(i+1,1,w) << " 0\n";
        out <<  q(i,1,w) << " " << -q(i,1,s) << " " <<  q(i+1,1,w) << " 0\n";
        out <<  q(i,1,w) << " " <<  q(i,1,s) << " " << -q(i+1,1,w) << " 0\n";
        numC = numC + 6;
    }

    /*  
            Centrales
    */

    for (int i = 2; i < N; i++ ){
        for (int j = 2; j < M; j++ ) {
            out << -q(i,j,s)     << " " << q(i,j,s) << " " << q(i,j,e) << " " << q(i+1,j+1,n) << " " << q(i+1,j+1,w) << " 0\n";
            out << -q(i,j,e)     << " " << q(i,j,s) << " " << q(i,j,e) << " " << q(i+1,j+1,n) << " " << q(i+1,j+1,w) << " 0\n";
            out << -q(i+1,j+1,n) << " " << q(i,j,s) << " " << q(i,j,e) << " " << q(i+1,j+1,n) << " " << q(i+1,j+1,w) << " 0\n";
            out << -q(i+1,j+1,w) << " " << q(i,j,s) << " " << q(i,j,e) << " " << q(i+1,j+1,n) << " " << q(i+1,j+1,w) << " 0\n";
            numC = numC + 4;
        }
    } 

    in.close();
    out.close();
    ifstream aux; 
    ofstream clau;
    aux.open("aux.txt");
    clau.open("out.cnf");
    string line;
    clau << "p cnf " << numV << " " << numC << "\n";
    if (aux.is_open()) {
        while (!aux.eof() ) {
            getline (aux,line);
            clau << line << endl;
        }
        aux.close();
    }
    clau.close();
    return 0;
}