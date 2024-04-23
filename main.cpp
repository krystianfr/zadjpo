#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>


class Graph
{
protected:
	int n;
public:
	virtual int GenN() = 0;
	virtual int GenE() = 0;
	virtual bool isEdge(short a, short b) = 0;
	Graph(int n_) : n(n_) {}
};

class SquareMatrix
{
protected:
	std::vector<std::vector<bool>> matrix;
public:
	SquareMatrix(int n);
	SquareMatrix() = default;
};

class SimpleGraph : public Graph, public SquareMatrix
{
public:
    int GenE()
    {
        return 1;
    }
    int GenN()
    {
        return Graph::n;
    }
    bool isEdge(short a, short b)
    {
        return SquareMatrix::matrix[a][b];
    }
	SimpleGraph(int n) :Graph(n) {
        std::vector<bool> set;
        for(int i = 0 ; i < n ;i ++)
        {
            set.push_back(false);
        }
        for(int i = 0 ;i < n; i++)
        {
            SquareMatrix::matrix.push_back(set);
        }

    }
	~SimpleGraph() {};

    SimpleGraph& operator~()
    {
        for(int i = 0 ; i < n;i++)
        {
            std::vector<bool> temp;
            temp = SquareMatrix::matrix[i];
            SquareMatrix::matrix[i].clear();
            for(int j=0;j<n;j++)
            {
                if(j == i)
                {
                    SquareMatrix::matrix[i].push_back(temp[j]);
                    continue;
                }
                SquareMatrix::matrix[i].push_back(!temp[j]);
            }
        }

        return *this;
    }

    SimpleGraph& dodaj_wierzcholek(short a, short b)
    {
        std::vector<bool> temp = SquareMatrix::matrix[a];

        SquareMatrix::matrix[a].clear();
        for(int i =0 ; i < n;i++)
        {
            if(b == i)
            {
                SquareMatrix::matrix[a].push_back(true);
            }else
            SquareMatrix::matrix[a].push_back(temp[i]);
        }
        return *this;
    }

    void wypisz()
    {
        for (int i = 0;i<n;i++)
        {
            for(int j =0;j<n;j++)
            {
                std::cout <<SquareMatrix::matrix[i][j];
            }
            std::cout<<std::endl;
        }
    }

    bool operator<(SimpleGraph oth)
    {
        return (this->Graph::n < oth.Graph::n);
    }

};


void zad1()
{
	SimpleGraph graf(4);
    (~graf).wypisz();
}


// zadanie 2 //

class Pojemnik_Grafow
{
private:

    std::vector<SimpleGraph*> grafy;
public:
    Pojemnik_Grafow() = default;
    ~Pojemnik_Grafow(){
        for(int i = 0 ; i < grafy.size();i++)
        {
            delete grafy[i];
        }
    };

    void wczytaj_plik(const char* plik)
    {
        std::ifstream fs(plik);
        std::string data;

        while(!fs.eof())
        {

        std::getline(fs, data);
        std::stringstream ss(data);
        int n;
        ss >> n;

        if(n <= 0)
        {
            //throw("zly wierzcholek");
            continue;
        }

        grafy.push_back(new SimpleGraph(n));
        
        int b;
        ss >> n;
        ss >> n;
        while(ss >> b)
        {
            grafy.back()->dodaj_wierzcholek(n,b);
            ss >> n;
        }
        }
    }

    std::vector<short> maks_stopien()
    {
        std::vector<short> out;
        for (const auto g : grafy) {
            int maxDeg = 0;

            for (int i = 0; i < g->GenN(); ++i) {
                int degree = 0;

                for (int j = 0; j < g->GenN(); ++j) {
                    if (g->isEdge(i, j)) {
                        degree++;
                    }
                }
                if (degree > maxDeg) {
                    maxDeg = degree;
                }
            }

            out.push_back(maxDeg);
        }
        return out;
    }
    
    void sortuj()
    {
        std::sort(grafy.begin(), grafy.end());
    }

private:
    
};

    
void zad2()
{
    Pojemnik_Grafow poj;
    poj.wczytaj_plik("dane.txt");
    poj.maks_stopien();
}

void zad3()
{
    Pojemnik_Grafow poj;
    poj.wczytaj_plik("dane.txt");
    poj.sortuj();
}

int main() {
    int wybor;
    do {
        std::cout << "Wybierz zadanie (1, 2, 3) lub 0 aby wyjść: ";
        std::cin >> wybor;

        switch (wybor) {
        case 1:
            zad1();
            break;
        case 2:
            zad2();
            break;
        case 3:
            zad3();
            break;
        case 0:
            std::cout << "Wybrano wyjście z programu." << std::endl;
            break;
        default:
            std::cout << "Nieprawidłowy wybór. Spróbuj ponownie." << std::endl;
        }
    } while (wybor != 0);

    return 0;
}
