#ifndef POSTOFFICE_H
#define POSTOFFICE_H


#include "ipostoffice.h"
#include "client.h"
#include "iclient.h"
#include <queue>
#include <iostream>
#include <map>
#include <memory>

class PostOffice: public IPostOffice
{
public:
    PostOffice(unsigned gate_count);
    ~PostOffice();
    virtual std::shared_ptr<IClient> getClient(const std::string& identificationNumber);
    virtual void enqueueClient(const std::shared_ptr<IClient>& client);
    virtual std::vector<std::string> getStatus();
    virtual void gateReady(unsigned gateIndex);
    virtual void collectPackages(unsigned gateIndex);
private:
    unsigned gate_count;
    unsigned gateIndex;
    std::multimap<int, std::shared_ptr<IClient>, std::greater<int>> Clients_queue;
    std::vector<std::shared_ptr<IClient>> Clients_at_gates;
    std::vector<std::string> status;
    std::vector<std::shared_ptr<IClient>> Clients;
};

#endif // POSTOFFICE_H
