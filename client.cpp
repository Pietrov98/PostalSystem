
#include "client.h"
#include "exceptions.h"
#include <iostream>
#include <string>
#include <vector>

static double max(double matrix2[])
{
    double a = matrix2[0];
    for (int i = 1; i < 4; i++)
    {
        if (matrix2[i] > a)
            a = matrix2[i];
    }
    return a;
}

static double similarity(const int seq1, const int seq2)
{
    double a;
    if (seq1 == seq2)
    {
        a = 3;
    }
    else
        a = -3;
    return a;
}

std::string Client::getIdNumber()
{
    return this->idNumber;
}
std::string Client::getFullName()
{
    return this->fullName;
}
void Client::setFullName(const std::string& fullName)
{
    this->fullName = fullName;
}

int Client::getPriority()
{
    return this->priority;
}
void Client::updatePriority(int priority)
{
    this->priority = priority;
}

const std::string& Client::getBiometricData()
{
    return biometricData;
}
void Client::updateBiometricData(const std::string& biometricData)
{
    int numerator = 0;
    for (int i = 0; i < biometricData.length(); i++)
    {
        if ((biometricData[i] != 'T') && (biometricData[i] != 'G') && (biometricData[i] != 'C') && (biometricData[i] != 'A'))
        {   throw IncorrectBiometricDataException("Illegal BiometricData");
            numerator++;
        }
    }
    if(numerator == 0)
        this->biometricData = biometricData;
}

bool Client::verifyBiometricData(const std::string& biometricData, double threshold)
{
    
    int numerator = 0;
    for (int i = 0; i < biometricData.length(); i++)
    {
        if ((biometricData[i] != 'T') && (biometricData[i] != 'G') && (biometricData[i] != 'C') && (biometricData[i] != 'A'))
        {    throw IncorrectBiometricDataException("Illegal BiometricData");
            numerator++;
        }
    }
    if(numerator == 0)
    {   
        double matrix2[4];
        size_t n = this->biometricData.length(), m = biometricData.length();
        std::vector<std::vector<double>> W_1((n + 1), std::vector<double>(m + 1));

        for (int k = 0; k <= n; k++)
            W_1[k][0] = 0;
        for (int l = 0; l <= m; l++)
            W_1[0][l] = 0;
        int maksimum = W_1[0][0];
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                matrix2[0] = W_1[i - 1][j - 1] + similarity(this->biometricData[i - 1], biometricData[j - 1]);
                matrix2[1] = W_1[i - 1][j] - 2;
                matrix2[2] = W_1[i][j - 1] - 2;
                matrix2[3] = 0;
                W_1[i][j] = max(matrix2);
                if (W_1[i][j] > maksimum)
                    maksimum = W_1[i][j];
             }
        }
        if (m >= n)
        {
            if ((maksimum / n) > threshold)
                return true;
            else
                return false;
        }
        else
        {
            if ((maksimum / m) > threshold)
                return true;
            else
                return false;
        }
    }
}

void Client::newPackage(const std::string& packageId)
{
    int numerator = 0;
    for (int i = 0; i < Packages.size(); i++)
    {
        if (Packages[i] == packageId)
        {   throw PackageExistsException("This package number was used");
            numerator++; 
        }
    }
    if(numerator == 0)
        Packages.push_back(packageId);
}
std::vector<std::string> Client::awaitingPackages()
{
    return Packages;
}
void Client::packagesCollected()
{
    Packages.clear();
}
