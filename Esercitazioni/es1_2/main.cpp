#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <iomanip>

using namespace std;

bool ImportData(const string& inputFilePath, double& S, size_t& n, double*& w, double*& r);

void ComputeRateOfReturn(const double& S, const size_t& n, const double* const& w, const double* const& r, double& rateOfReturn, double& V);

void ExportData(ostream& out, const double& S, const size_t& n, const double* const& w, const double* const& r, const double& rateOfReturn, const double& V);

string ArrayToString(const size_t& n, const double* const& v)
{
  ostringstream toString;
  toString << "[ ";
  for (unsigned int i = 0; i < n; i++){toString<< v[i]<< " ";}
  toString << "]";
  return toString.str();
}

int main()
{
  string inputFileName = "./data.txt";
  double S = 0.0;
  size_t n = 0;
  double* w = nullptr; double* r = nullptr;

  if(!ImportData(inputFileName, S, n, w, r)){cerr<< "Qualcosa e' andato storto"<< endl; return -1;}

  double rateOfReturn = 0.0; double V = 0.0;
  ComputeRateOfReturn(S, n, w, r, rateOfReturn, V);

  // Export data on the standard output
  ExportData(cout, S, n, w, r, rateOfReturn, V);

  // Write data to a file
  string outputFileName = "./result.txt";
  ofstream file;
  file.open(outputFileName);
  if(!file.fail()){ExportData(file, S, n, w, r, rateOfReturn, V);}
  file.close();

  delete[] w;
  delete[] r;
  return 0;
}

bool ImportData(const string& inputFilePath, double& S, size_t& n, double*& w, double*& r)
{
    ifstream file;
    file.open(inputFilePath);
    if(file.fail()){cerr << "Errore di apertura file"<< endl; return false;}
    string word;
    int n_row = 0;

    getline(file,word,';');
    getline(file,word); S = stod(word);
    getline(file,word,';');
    getline(file,word); n = stoi(word);
    w = new double[n];
    r = new double[n];
    getline(file,word,';'); getline(file,word);
    getline(file,word,';');
    while(!file.eof())
    {
        w[n_row] = stod(word);
        getline(file,word); r[n_row] = stod(word);
        n_row++;
        getline(file,word,';');
    }
    return true;
}


void ComputeRateOfReturn(const double& S, const size_t& n, const double* const& w, const double* const& r, double& rateOfReturn, double& V)
{
    for (unsigned int i = 0; i < n; i++)
      rateOfReturn += w[i] * r[i];
    V = S * (1 + rateOfReturn);
}


void ExportData(ostream& out, const double& S, const size_t& n, const double* const& w, const double* const& r, const double& rateOfReturn, const double& V)
{
    out<<setprecision(2)<<fixed<<"S = "<<S<<", n = "<<n<<endl;
    out<<"w = [ ";
    for(int i=0;i<n;i++){out<<w[i]<<" ";}
    out<<"]";
    out<<endl<<"r = [ ";
    for(int i=0;i<n;i++){out<<r[i]<<" ";}
    out<<"]";
    out<<endl<<"Tasso di crescita : "<<rateOfReturn<<endl;
    out<<setprecision(2)<<fixed<<"V = "<<V<<endl;
}
