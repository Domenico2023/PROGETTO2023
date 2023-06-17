#ifndef __WEB_H
#define __WEB_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"

using namespace std;
using namespace Eigen;

namespace WebLibrary {

  struct Page
  {
      string url;
      string htmlContent;
      double rank;

      Page(const string& url):
        url(url){};
  };

  inline bool operator==(const Page &page1, const Page &page2)
  {
    return page1.url == page2.url;
  }

  inline bool operator!=(const Page &page1, const Page &page2)
  {
    return !(page1 == page2);
  }

  inline bool operator<(const Page &page1, const Page &page2)
  {
    return page1.rank < page2.rank;
  }

  inline bool operator>(const Page &page1, const Page &page2)
  {
    return page1.rank > page2.rank;
  }

  struct Server
  {
      vector<Page> pages;
      Server(vector<string> urls);

      void CreateHtmlContent(const string& url,
                             const string& content);
  };

  struct Surfer
  {
      Server server;

      Surfer(const Server& server);

      MatrixXd computeAdjacencyMatrix();

  };

}

#endif // __WEB_H
