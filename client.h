#ifndef CLIENT_H
#define CLIENT_H


#include "iclient.h"
#include <string>
#include <vector>

class Client: public IClient
{
public:
    Client(std::string idNumber) : idNumber(idNumber){}
    ~Client() {}
    virtual std::string getIdNumber();
    virtual std::string getFullName();
    virtual void setFullName(const std::string& fullName);
    virtual int getPriority();
    virtual void updatePriority(int priority);
    virtual const std::string& getBiometricData();
    virtual void updateBiometricData(const std::string& biometricData);
    virtual bool verifyBiometricData(const std::string& biometricData, double threshold);
    virtual void newPackage(const std::string& packageId);
    virtual std::vector<std::string> awaitingPackages();
    virtual void packagesCollected();

private:
    std::string idNumber;
    std::string fullName;
    std::string biometricData;
    int priority;
    std::string packageId;
    std::vector<std::string> Packages;
};





#endif // CLIENT_H
