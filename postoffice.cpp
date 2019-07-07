#include "postoffice.h"
#include "ipostoffice.h"
#include "client.h"
#include "iclient.h"
#include "exceptions.h"
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <fstream>

PostOffice::PostOffice(unsigned gate_count) : gate_count(gate_count)
{
    for (int i = 0; i < gate_count; i++)
        status.push_back("");
    for (int i = 0; i < gate_count; i++)
        Clients_at_gates.push_back(nullptr);
	std::ifstream file("clients.txt");
	std::string name, bio, id, package;
	std::vector<std::string> packages;
	unsigned p, i = 0, vec_size = 0, package_size = 0;
	char c;
	file >> vec_size;
	while (i < vec_size)
	{
		file >> package_size;
		for (int i = 0; i < package_size; i++)
		{
			file >> package;
			packages.push_back(package);
		}
		file >> id >> bio >> p;
		getline(file, name);
		name.erase(0, 1);
		i++;
		if (Clients.begin() == Clients.end())
		{
			auto tmp_client = std::make_shared<Client>(id);
			tmp_client->setFullName(name);
			tmp_client->updateBiometricData(bio);
			tmp_client->updatePriority(p);
			for(int i = 0; i < package_size; i++)
				tmp_client->newPackage(packages[i]);
			Clients.push_back(tmp_client);
		}
		else
		{
			unsigned k = 0, l = 0;
			auto it = Clients_queue.begin();
			for (auto it = Clients.begin(); it != Clients.end(); it++)
			{
				if (Clients[k]->getIdNumber() == id)
					l++;
				k++;
			}
			if (l == 0)
			{
				auto tmp_client = std::make_shared<Client>(id);
				tmp_client->setFullName(name);
				tmp_client->updateBiometricData(bio);
				tmp_client->updatePriority(p);
				Clients.push_back(tmp_client);
			}
		}
	}
	file.close();
}

PostOffice::~PostOffice()
{
    unsigned i = 0;
	std::ofstream file("clients.txt");
	file << Clients.size() << std::endl;
	for (auto it = Clients.begin(); it != Clients.end(); it++)
	{
		auto package = Clients[i]->awaitingPackages();
		file << package.size() << " ";
		for (int j = 0; j < package.size(); j++)
			file << package[j] << " ";
		file << Clients[i]->getIdNumber() << " " << Clients[i]->getBiometricData() << " " << Clients[i]->getPriority() << " " << Clients[i]->getFullName() << std::endl;
		i++;
	}
	file.close();
}

std::shared_ptr<IClient> PostOffice::getClient(const std::string & identificationNumber)
{

    if (Clients.begin() == Clients.end())
	{
		auto tmp_client = std::make_shared<Client>(identificationNumber);
		Clients.push_back(tmp_client);
		return tmp_client;
	}
	else
	{
		unsigned i = 0;
		for(auto it = Clients.begin(); it != Clients.end(); it++)
		{
			if (Clients[i]->getIdNumber() == identificationNumber)
				return Clients[i];
			i++;
		}
		auto tmp_client = std::make_shared<Client>(identificationNumber);
		Clients.push_back(tmp_client);
		return tmp_client;
	}

}

void PostOffice::enqueueClient(const std::shared_ptr<IClient>& client)
{

    int i = 0, j = 0;
    for(auto it = Clients_queue.begin(); it != Clients_queue.end(); it++)
    {
        if (it->second->getIdNumber() != client->getIdNumber())
            j++;
        i++;
    }
    if(i == j)
        Clients_queue.insert(std::make_pair(client->getPriority(), client));

}

std::vector<std::string> PostOffice::getStatus()
{
    return status;
}

void PostOffice::gateReady(unsigned gateIndex)
{
    if ((gateIndex > (gate_count - 1)) || (gateIndex < 0))
        throw IncorrectGateException("Incorrect Gate Exceptions");
    else
    {
        auto it = Clients_queue.begin();
		if(it != Clients_queue.end())
     	{	status[gateIndex] = it->second->getFullName();
        	Clients_at_gates[gateIndex] = it->second;
        	Clients_queue.erase(it);
		}
		else
		{
			status[gateIndex] = "";
			Clients_at_gates[gateIndex] = nullptr;
		}
    }
}

void PostOffice::collectPackages(unsigned gateIndex)
{
    if ((gateIndex > (gate_count - 1)) || (gateIndex < 0))
        throw IncorrectGateException("Wrong gateIndex");
	else if(Clients_at_gates[gateIndex] == nullptr)
	    throw IncorrectGateException("Client is no here");
    else
        Clients_at_gates[gateIndex]->packagesCollected();

}



