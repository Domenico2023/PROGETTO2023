#include "web.hpp"

namespace WebLibrary {

  Server::Server(vector<string> urls)
  {
    this->pages.reserve(urls.size());
    for(auto it = urls.begin(); it != urls.end(); it++)
    {
      Page page(*it);
      this->pages.push_back(page);
    }
  }


  void Server::CreateHtmlContent(const string& url,
                         const string& content)
  {
    Page page(url);
    auto it = find(pages.begin(), pages.end(), page);
    if( it != pages.end() )
      it -> htmlContent = content;
  }

  Surfer::Surfer(const Server& server):
    server(server)
  {
  }

  MatrixXd Surfer::computeAdjacencyMatrix()
  {
      unsigned int numPages = server.pages.size();
      MatrixXd adjacencyMatrix = MatrixXd::Zero(numPages, numPages);

      for(unsigned int i = 0; i < numPages; i++)
      {
        istringstream split(server.pages[i].htmlContent);
        string neighbor;
        while(split >> neighbor)
        {
          Page page(neighbor);
          auto it = find(server.pages.begin(), server.pages.end(), page);
          if (it ==  server.pages.end())
            throw runtime_error("Not a valid page");

          unsigned int index = it - server.pages.begin();
          adjacencyMatrix(i, index) = 1;
        }
      }

      return adjacencyMatrix;
  }


}
