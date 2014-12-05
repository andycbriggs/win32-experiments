#include <iostream>
#include <thread>
#include <memory>
#include <concrt.h>

using namespace std;

class Response
{
  concurrency::event ev;
public:
  int data;
  void set(int d)a
  {
    data = d;
    ev.set();
  }
  int get()
  {
    ev.wait();
    return data;
  }
};

class Request
{
  Response m_response;
public:
  int data;
  void setResponse(int d)
  {
    m_response.set(d);
  }
  int getResponse()
  {
    return m_response.get();
  }
};

class Handler
{
public:
  void handle(shared_ptr<Request> request)
  {
    // handler reads the request data
    int temp = request->data;
    // handler does some work
    this_thread::sleep_for(chrono::seconds(5));
    int res = temp * 2;
    // handler sets response
    request->setResponse(res);
  }
};

void mediator(shared_ptr<Request> request) {
  // mediator gets a handler for the message
  shared_ptr<Handler> handler = shared_ptr<Handler>(new Handler());
  // mediator tells handler to handle the request
  handler->handle(request);
}

int main() 
{

  // create the request
  shared_ptr<Request> request = shared_ptr<Request>(new Request());

  // set request data
  request->data = 10;

  // post the request to the mediator thread
  thread t(mediator, request);

  // get data from the request as if we were all on one thread
  cout << "data: " << request->getResponse() << endl;

  // give us some time to read the output
  this_thread::sleep_for(chrono::seconds(5));

  // close cleanly
  t.join();

}
