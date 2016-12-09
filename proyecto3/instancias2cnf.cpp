#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
 
int main()
{
    using namespace std;
 
    // ifstream is used for reading files
    // We'll read from a file called Sample.dat
    ifstream inf("example_input2.txt");
    ofstream outf("salida.cnf");
 
    // If we couldn't open the input file stream for reading
    if (!inf)
    {
        // Print an error and exit
        cerr << "Uh oh, example_input.txt could not be opened for reading!" << endl;
        exit(1);
    }

     if (!outf)
    {
        // Print an error and exit
        cerr << "Uh oh, Sample.dat could not be opened for writing!" << endl;
        exit(1);
    }
    // Formato traducción
    outf << "c Descripción de las variables\nc Para instancia, existen N*M*4 variables";
    outf << "c Cada variable representa q(i,j,d), es decir, un segmento\n";
    outf << "c Correspondencia: 1 => q(i,j,N), 2 => q(i,j,S), 3 => q(i,j,W), 4 => q(i,j,E)\n";

    string strInput;
    //getline(inf, strInput);
    vector<string> x = split(strInput, ' ');


    unsigned int campo = 1, N = 0, M = 0;
    int nroGrupo = 0;
    for (vector<string>::iterator i = x.begin(); i != x.end(); ++i, campo += 1)
    {
        
        if (campo == 1) {
            if ( ! (istringstream(*i) >> N ) ) N  = 0;
            N = 2;
        }
        else if (campo == 2) 
        {
            if ( ! (istringstream(*i) >> M ) ) M  = 0;
            M = 3;
            // Imprimir cabecera
            outf << "p cnf " << N*M << " " << 10 << endl; 

            // Clausulas de tipo 0
            outf << "c CLAUSULAS DE TIPO 0 -> EW\n";
            for (int j = 4; j <= M*N*4; j += 4)
            {   
                if ( (j % M) != 0) 
                {
                    outf << -j << " " << j+3 << " 0 " 
                        << j << " " << -(j+3) << " 0\n";
                }
            }
            outf << endl << 
                "c CLAUSULAS DE TIPO 0 -> NS. Desde el principio\n";
            nroGrupo = (M*N) *2;
            for (int j = 1; j <= nroGrupo; j += 4)
            {   
                //cout << -(j + nroGrupo + 1) << endl;
                cout << -j << " " << j + nroGrupo + 1 << " 0 " << j << " " << -(j + nroGrupo + 1) << " 0 \n";
                outf << -j << " " << j + nroGrupo + 1 << " 0 " 
                    << j << " " << -(j + nroGrupo + 1) << " 0 \n";
            }
            outf << endl;
        } 
        if (campo > 2){
            outf << "c CLAUSULAS TIPO 1\n";
            int base = (campo - 3) * nroGrupo, despl = 0;
            for (int j = 0; j < M; ++j)
            {   
                despl = j*4 + base;
                switch ((*i)[j]) 
                {
                    case '0':
                        // "2 3 0.2 .04"
                        //(-q(i,j,n) && -q(i,j,s) && -q(i,j,e) && -q(i,j,w)))
                        outf << "c CASO N = 0: \n";
                        outf << -(despl + 1) << " 0 " << -(despl + 2) << " 0 "
                            << -(despl + 3) << " 0 " << -(despl + 4) << " 0\n";
                        break;  
                    case '1':
                        outf << "c CASO N = 1: \n";
                        // Clausula: q(i,j,n) v q(i,j,e) v q(i,j,s) v q(i,j,w)
                        outf << despl + 1 << " " << despl + 2 << " "
                            << despl + 3 << " " << despl + 4 << " 0\n";
                        // Clausula: -q(i,j,n) v -q(i,j,e)
                        outf << -(despl + 1) << -(despl + 4) << " 0\n";
                        // Clausula: -q(i,j,n) v -q(i,j,s)
                        outf << -(despl + 1) << -(despl + 2) << " 0\n";
                        // Clausula: -q(i,j,n) v -q(i,j,w)
                        outf << -(despl + 1) << -(despl + 3) << " 0\n";
                        // Clausula: -q(i,j,e) v -q(i,j,s)
                        outf << -(despl + 4) << -(despl + 2) << " 0\n";
                        // Clausula: -q(i,j,e) v -q(i,j,w)
                        outf << -(despl + 4) << -(despl + 3) << " 0\n";
                        // Clausula: -q(i,j,s) v -q(i,j,w)
                        outf << -(despl + 2) << -(despl + 3) << " 0\n";
                        break;
                    case '2':
                        break;
                    case '3':
                        break;
                    case '4':
                        outf << "c CASO N = 4: \n";
                            outf << despl + 1 << " 0 " << despl + 2 << " 0 "
                                << despl + 3 << " 0 " << despl + 4 << " 0\n";
                        break;
                    case '.':
                        break;
                }
            }
        }

    }    
    return 0;  
    // When inf goes out of scope, the ifstream
    // destructor will close the file
}
