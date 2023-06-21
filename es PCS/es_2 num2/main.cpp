#include <iostream>
#include <fstream>
#include <sstream>
#include<cstdio>
using namespace std;

/// \brief ImportText import the text for encryption
/// \param inputFilePath: the input file path
/// \param text: the resulting text
/// \return the result of the operation, true is success, false is error
bool ImportText(const string& inputFilePath,
                string& text);

/// \brief Encrypt encrypt the text
/// \param text: the text to encrypt
/// \param password: the password for encryption
/// \param encryptedText: the resulting encrypted text
/// \return the result of the operation, true is success, false is error
bool Encrypt(const string& text,
             const string& password,
             string& encryptedText);

/// \brief Decrypt decrypt the text
/// \param text: the text to decrypt
/// \param password: the password for decryption
/// \param decryptedText: the resulting decrypted text
/// \return the result of the operation, true is success, false is error
bool Decrypt(const string& text,
             const string& password,
             string& decryptedText);

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cerr<< "Password shall passed to the program"<< endl;
    return -1;
  }
  string password = argv[1];

  string inputFileName = "./text.txt", text;
  if (!ImportText(inputFileName, text))
  {
    cerr<< "Something goes wrong with import"<< endl;
    return -1;
  }
  else
    cout<< "Import successful: result= "<< text<< endl;

  string encryptedText = text;
  if (!Encrypt(text, password, encryptedText))
  {
    cerr<< "Something goes wrong with encryption"<< endl;
    return -1;
  }
  else
    cout<< "Encryption successful: result= "<< encryptedText<< endl;

  string decryptedText = text;
  if (!Decrypt(encryptedText, password, decryptedText) || text != decryptedText)
  {
    cerr<< "Something goes wrong with decryption"<< endl;
    return -1;
  }
  else
    cout<< "Decryption successful: result= "<< decryptedText<< endl;

  return 0;
}

bool ImportText(const string& inputFilePath,
                string& text)
{
    ifstream file;
    file.open(inputFilePath);

    getline(file,text);
    file.close();
  return true;
}

bool Encrypt(const string& text,
             const string& password,
             string& encryptedText)
{
    unsigned long i=0;
    unsigned long j=0;
    //string line="THIS IS A GENERIC TEXT TO BE ENCRYPTED";

    int* ascii_encryp=new int[text.length()];
    for(i=0;i<text.length();i++){
        ascii_encryp[i]=int(text[i]);

    }
    i=0;
    while(i<text.length() && j<=password.length()){
        if(text[i]== ' '){
            encryptedText[i]=' ';
            i++;
        }
        else{
            if(j==password.length()){
                j=0;
                ascii_encryp[i]=int(text[i])+int(password[j])-65;
                j++;
            }
            else{
                ascii_encryp[i]=int(text[i])+int(password[j])-65;
                j++;
            }
            if(ascii_encryp[i]>90){
                ascii_encryp[i]=ascii_encryp[i]-26;
            }
            if(ascii_encryp[i]<64){
                ascii_encryp[i]=ascii_encryp[i]+26;
            }
            encryptedText[i]=char(ascii_encryp[i]);
            i++;
        }
    }

  delete[] ascii_encryp;
  return true;
}

bool Decrypt(const string& text,
             const string& password,
             string& decryptedText)
{
    unsigned long i=0;
    unsigned long j=0;
    //string line="ZHBL WY A ZXBKRBV HKXM MC HE XGQXYMSJ";

    int* ascii_decryp=new int[text.length()];
    while(i<text.length() && j<=password.length()){
        if(text[i]== ' '){
            decryptedText[i]= ' ';
            i++;
        }
        else{
            if(j==password.length()){
                j=0;
                ascii_decryp[i]=int(text[i])-int(password[j])+65;
                j++;
            }
            else{
                ascii_decryp[i]=int(text[i])-int(password[j])+65;
                j++;
            }
            if(ascii_decryp[i]>90){
                ascii_decryp[i]=ascii_decryp[i]-26;
            }
            if(ascii_decryp[i]<64){
                ascii_decryp[i]=ascii_decryp[i]+26;
            }
            decryptedText[i]=char(ascii_decryp[i]);
            i++;
        }
    }
  delete[] ascii_decryp;
  return true;
}
