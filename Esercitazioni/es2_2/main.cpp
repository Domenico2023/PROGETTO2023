#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool ImportText(const string& inputFilePath, string& text);
bool Encrypt(const string& text, const string& password, string& encryptedText, bool Maius = true);
bool Encrypt2(const string& text, const string& password, string& encryptedText);
bool Decrypt(const string& text, const string& password, string& decryptedText, bool Maius = true);
bool Decrypt2(const string& text, const string& password, string& decryptedText);

int main(int argc, char** argv)
{
  if(argc < 3){cerr<<"Password and file name shall passed to the program"<<endl; return -1;}
  string password = argv[1], filename = argv[2], text, encryptedText, decryptedText;
  if(!ImportText(filename,text)){cerr<<"Something goes wrong with import"<<endl; return -1;}
  cout<<"Text to be encrypted: "<<text<<endl;
  cout<<"Password selected: "<<password<<endl;
  //encrypt
  if(!Encrypt(text,password,encryptedText)){cerr<<"error in encrypting"<<endl; return -1;}
  cout<<"Encrypted text: "<<encryptedText<<endl;
  //decrypt
  if(!Decrypt(encryptedText,password,decryptedText)){cerr<<"error in decrypting"<<endl; return -1;}
  cout<<"Decrypted text: "<<decryptedText<<endl;

  return 0;
}

bool ImportText(const string& inputFilePath, string& text)
{
  ifstream file;
  file.open(inputFilePath); if(file.fail()){cerr<<"file open failed"<<endl; return false;}
  getline(file,text); file.close(); return true;
}
/*     //sufficienti se si vogliono evitare i controlli sul case delle lettere e si assume che siano tutte maiuscole
bool Encrypt(const string& text, const string& password, string& encryptedText)
{
  encryptedText = text;
  int n = password.length();
  if(n == 0){cerr<<"no password"<<endl; return false;}
  int k = 0, A = 'A', len = 26;
  for(int i=0;i<text.length();i++)
  {
    if(isspace(text[i])){encryptedText[i] = text[i];}
    else{encryptedText[i] = (char)(((text[i] + password[k%n])%A)%len+A); k++;}
  }
  return true;
}

bool Decrypt(const string& text, const string& password, string& decryptedText)
{
  decryptedText = text;
  int n = password.length();
  if(n == 0){cerr<<"no password"<<endl; return false;}
  int k = 0, A = 'A', len = 26;
  for(int i=0;i<text.length();i++)
  {
    if(isspace(text[i])){decryptedText[i] = text[i];}
    else{decryptedText[i] = (char)(((len + text[i] - password[k%n])%A)%len+A); k++;}
  }
  return true;
}
*/
//Encrypt implementata con controlli sui case

bool Encrypt(const string& text, const string& password, string& encryptedText, bool Maius)
{
  encryptedText = text;
  int lp = password.length(), lt = text.length();
  if(lp == 0){cerr<<"no password"<<endl; return false;}
  if(Maius==true)
  {
    for(int i=0;i<lt;i++)
    {
      if(isspace(text[i]));
      else if(islower(text[i])){cerr<<"text must be all uppercase"<<endl;return false;}
    }
    for(int i=0;i<lp;i++)
    {
      if(islower(password[i])){cerr<<"password must be all uppercase"<<endl;return false;}
    }
  }
  else
  {
    for(int i=0;i<lt;i++)
    {
      if(isspace(text[i]));
      else if(isupper(text[i])){cerr<<"text must be all lowercase"<<endl;return false;}
    }
    for(int i=0;i<lp;i++)
    {
      if(isupper(password[i])){cerr<<"password must be all lowercase"<<endl;return false;}
    }
  }
  int k = 0, A, len = 26;
  if(Maius == true) A = 'A'; else A = 'a';
  for(int i=0;i<lt;i++)
  {
    if(isspace(text[i])){encryptedText[i] = text[i];}
    else{encryptedText[i] = (char)(((text[i] + password[k%lp])%A)%len+A); k++;}
  }
  return true;
}

bool Decrypt(const string& text, const string& password, string& decryptedText, bool Maius)
{
  decryptedText = text;
  int lp = password.length(), lt = text.length();
  if(lp == 0){cerr<<"no password"<<endl; return false;}
/*  if(Maius==true)     //controlli non necessari se prima viene utilizzata la Encrypt
  {
    for(int i=0;i<lt;i++)
    {
      if(isspace(text[i]));
      else if(islower(text[i])){cerr<<"text must be all uppercase"<<endl;return false;}
    }
    for(int i=0;i<lp;i++)
    {
      if(islower(password[i])){cerr<<"password must be all uppercase"<<endl;return false;}
    }
  }
  else
  {
    for(int i=0;i<lt;i++)
    {
      if(isspace(text[i]));
      else if(isupper(text[i])){cerr<<"text must be all lowercase"<<endl;return false;}
    }
    for(int i=0;i<lp;i++)
    {
      if(isupper(password[i])){cerr<<"password must be all lowercase"<<endl;return false;}
    }
  }
*/
  int k = 0, A, len = 26;
  if(Maius == true) A = 'A'; else A = 'a';
  for(int i=0;i<lt;i++)
  {
    if(isspace(text[i])){decryptedText[i] = text[i];}
    else{decryptedText[i] = (char)(((len + text[i] - password[k%lp])%A)%len+A); k++;}
  }
  return true;
}

//si può generalizzare utilizzando la lettera 'a' con il case giusto per ogni lettera del messaggio, e con una password di caratteri generici
bool Encrypt2(const string& text, const string& password, string& encryptedText)
{
  encryptedText = text;
  int lp = password.length(), lt = text.length();
  if(lp == 0){cerr<<"no password"<<endl; return false;}
  int k = 0, A = 'A', a = 'a', len = 26, first;
  for(int i=0;i<lt;i++)
  {
    if(isspace(text[i])){encryptedText[i] = text[i];}
    else{first = isupper(text[i])? A : a; encryptedText[i] = (char)((text[i] - first + password[k%lp]) % len + first); k++;}
  }
  return true;
}

bool Decrypt2(const string& text, const string& password, string& decryptedText)
{
  decryptedText = text;
  int lp = password.length(), lt = text.length();
  if(lp == 0){cerr<<"no password"<<endl; return false;}
  int k = 0, A = 'A', a = 'a', len = 26, first;
  for(int i=0;i<lt;i++)
  {
    if(isspace(text[i])){decryptedText[i] = text[i];}
    else{first = isupper(text[i])? A : a; decryptedText[i] = (char)((((text[i] - first - password[k%lp]) % len) + (((text[i] - first - password[k%lp])%len < 0)? len : 0)) + first); k++;}  //sommo len solo se la quantità è negativa
  }
  return true;
}

