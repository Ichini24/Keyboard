#include "keyauth.h"


KeyAuth::KeyAuth(int level)
{
    firstPass = true;
    workingPass = "";
    MainPass = "";

    if(level == 0)
    {
        coefLevel = 0.3;
    }
    else if(level == 1)
    {
        coefLevel = 0.2;
    }
    else if(level == 2)
    {
        coefLevel = 0.1;
    }
}

KeyAuth::~KeyAuth()
{

}

bool KeyAuth::CheckVocPass(std::string pass)
{
    bool upper = false;
    bool lower = false;
    bool special = false;
    bool num = false;

    for (auto c : pass)
    {
        if (isupper(c))
        {
            upper = true;
        }
        else if (islower(c))
        {
            lower = true;
        }
        else if (isdigit(c))
        {
            num = true;
        }
        else if (CheckSpecial(c))
        {
            special = true;
        }
    }
    if (upper && lower && num && special)
    {
        return true;
    }
    return false;
}

int KeyAuth::CheckPrimaryPass(std::string pass)
{
    if (length != pass.length())
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            if (pass[i] != MainPass[i])
            {
                return -2;
            }
        }
    }
    return 1;
}

bool KeyAuth::CheckSpecial(char c)
{
    int d = (int)c;
    if ((d > 32 && d < 48) || (d > 57 && d < 65) || (d > 90 && d < 97) || (d > 122 && d < 127))
    {
        return true;
    }
    return false;
}

void KeyAuth::calculateLimits()
{
    for(int i = 0; i < authtimes[0].size(); i++)
    {
        double low = 999;
        double high = 0;
        double average = 0;
        for(int j = 0; j < authtimes.size(); j++)
        {
            double value = authtimes[j][i];
            average += value;
            if(value < low)
            {
                low = value;
            }

            if(value > high)
            {
                high = value;
            }
        }
        ref.push_back(average/3);
        lowLim.push_back(low * (1 - coefLevel));
        highLim.push_back(high * (1 + coefLevel));
    }
}
