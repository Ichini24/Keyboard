#ifndef KEYAUTH_H
#define KEYAUTH_H

#include <vector>
#include <string>
#include <iostream>

struct Odtime
{
    std::vector<double> value;
    std::vector<bool> valid;
};

struct TestingStr
{
    std::vector<Odtime> dtimes;
    std::vector<bool> valid;
};

class KeyAuth
{
public:
    KeyAuth(int level);
    ~KeyAuth();

    bool CheckVocPass(std::string pass);
    int CheckPrimaryPass(std::string pass);
    void calculateLimits();

    double coefLevel;

    std::string MainPass;
    int length;
    std::vector<double> dtimes;
    std::string workingPass;
    std::vector<std::vector<double>> authtimes;
    std::vector<double> lowLim;
    std::vector<double> highLim;
    std::vector<double> ref;
    TestingStr userTests;
    TestingStr intruderTests;

private:
    bool CheckSpecial(char c);

    bool firstPass;
};

#endif // KEYAUTH_H
