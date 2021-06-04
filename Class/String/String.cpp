#pragma warning(disable: 4996)

#include "String.h"
#include <iostream>
#include <cassert>


// ************************** CONSTRUCTOR *****************************
String::String()
{
    mLen = 1;
    allocSpace();
}
String::String(const int& sz)
{
    mLen = sz == 0 ? 1 : sz + 1;
    allocSpace();
    memset(mStr, '\0', mLen);
}
String::String(const char* arr)
{
    mLen = strlen(arr) + 1;
    allocSpace();
    for (int idex = 0; idex < mLen; idex++)
    {
        mStr[idex] = arr[idex];
    }
}
String::String(const String& str)
{
    mLen = strlen(str.mStr) + 1;
    allocSpace();
    for (int idex = 0; idex < mLen; idex++)
    {
        mStr[idex] = str.mStr[idex];
    }
}
String::~String()
{
    mLen = 0;
    delete[] mStr;
}


// ************************** GET - SET *******************************
char* String::getMStr() const
{
    return mStr;
}
void String::setMStr(char* mStr)
{
    this->mStr = mStr;
}
int String::getMlength() const
{
    return mLen;
}
void String::setMlength(int mlength)
{
    mLen = mlength;
}
// **************************** METHOD ********************************
int String::length()
{
    return mLen;
}

// ********************* Operator Overloading **************************
String& String::operator=(const String& s)
{
    // TODO: insert return statement here
    // if same String -> return itself
    if (this == &s)
        return *this;
    // delete old
    delete[] mStr;

    // allocate new one;
    mLen = s.mLen;
    allocSpace();
    // deep copy
    for (int iRun = 0; iRun < mLen; iRun++)
    {
        mStr[iRun] = s.mStr[iRun];
    }
    
    return *this;
}
String& String::operator+=(const String& s)
{
    // TODO: insert return statement here
    String temp{ *this };
    
    // plus one for space between 2 strings
    mLen = strlen(temp.mStr) + s.mLen + 1;
    allocSpace();

    strcpy(mStr, temp.mStr);
    strcat(mStr, " ");
    strcat(mStr, s.mStr);
    return *this;
}
bool String::operator==(const String& s)
{
    if (strcmp(mStr, s.mStr) == 0)
        return true;
    return false;
    // TODO: insert return statement here
}
bool String::operator>(const String& s)
{
    if (strcmp(mStr, s.mStr) > 0)
        return true;
    return false;
    // TODO: insert return statement here
}
bool String::operator<(const String& s)
{
    if (strcmp(mStr, s.mStr) < 0)
        return true;
    return false;
    // TODO: insert return statement here
}
String operator+(const String& s1, const String& s2)
{
    int len = s1.getMlength() + s2.getMlength();
    String temp{ len + 2 };
    strcpy(temp.getMStr(), s1.getMStr());
    strcat(temp.getMStr(), " ");
    strcat(temp.getMStr(), s2.getMStr());

    return String(temp);
}
String operator+(const String& s, const char*& arr)
{
    int len = s.getMlength() + strlen(arr);
    String temp{ len + 2 };
    strcpy(temp.getMStr(), s.getMStr());
    strcat(temp.getMStr(), " ");
    strcat(temp.getMStr(), arr);

    return String(temp);
}


// ***************************** Methods *******************************
void String::allocSpace()
{
    mStr = new char[mLen];
    memset(mStr,0, mLen);
}
void String::erase(const int& _beg, const int& _count)
{
    String temp = *this;
    /*std::cout << "len temp: " << temp.mLen << std::endl;*/
    int iRun = 0;
    int i_max = _beg + _count;
    mLen = temp.mLen - _count;
    /*std::cout << "len : " << mLen << std::endl;*/
    allocSpace();

    /*std::cout << "_beg: " << _beg << std::endl;
    std::cout << "_count: " << _count << std::endl;*/
    for (int idex = 0; idex < temp.mLen; idex++)
    {
        /*std::cout << "call idex: " << idex << std::endl;*/
        if (temp.mStr[idex] == '\0')
        {
            /*std::cout << "call '\0': " << idex << std::endl;*/
            mStr[iRun] = '\0';
            break;
        }
        else
        {
            if (idex < _beg || idex >= i_max)
            {
                //std::cout << "call iRun: " << iRun << std::endl;
                //std::cout << "call insert: " << idex << std::endl;
                mStr[iRun] = temp.mStr[idex];
                iRun++;
            }
        }
    }
}
void String::insert(const int& _pos, const String& s)
{
    assert(_pos >= 0 && _pos < mLen);
    if (_pos == mLen - 1)
    {
        std::cout << "Insert function: _pos == mLen - 1! " << std::endl;
        String save = *this;
        mLen = strlen(mStr) + strlen(s.mStr) + 1;
        allocSpace();
        strcpy(mStr, save.mStr);
        strcat(mStr, s.mStr);
        mStr[mLen - 1] = '\0';
        return;
    }
    else
    {
        std::cout << "Insert function: _pos != mLen - 1! " << std::endl;
        int iRun = 0;
        String save = *this;
        mLen = strlen(mStr) + strlen(s.mStr);
        allocSpace();
        for (int idex = 0; idex < mLen - strlen(s.mStr); idex++)
        {
            if (idex == _pos)
            {
                int iPos = 0;
                while (iPos < strlen(s.mStr))
                {
                    mStr[iRun] = s.mStr[iPos];
                    //std::cout << "mStr[" << iPos << "] = " << mStr[iRun]  << std::endl;
                    iRun++;
                    iPos++;
                }
                mStr[iRun] = save.mStr[idex];
            }
            else
            {
                mStr[iRun] = save.mStr[idex];
                //std::cout << "mStr[" << iRun << "] = " << mStr[iRun] << std::endl;
                iRun++; 
            }
        }
        mStr[mLen - 1] = '\0';
    }    
}
void String::push_back(const char& c)
{
    String temp = *this;
    mLen = temp.mLen + 1;
    allocSpace();
    for (int idex = 0; idex < temp.mLen; idex++)
    {
        if (idex == mLen - 2)
        {
            mStr[idex] = c;
            break;
        }
        else
        {
            mStr[idex] = temp.mStr[idex];
        }
    }
    mStr[mLen - 1] = '\0';
}
void String::pop_back()
{
    String temp = *this;
    mLen = temp.mLen - 1;
    allocSpace();
    for (int idex = 0; idex < mLen - 1; idex++)
    {
        mStr[idex] = temp.mStr[idex];
    }
    mStr[mLen - 1] = '\0';
}
String String::substr(const int& _pos, const size_t& _count)
{
    if (_pos < 0 && _pos >= mLen)
        std::cerr << "Error! Out of Index!" << std::endl;
    
    int len = _count + 1;
    String Temp{ len };
    int iPus = 0;
    int iAss = _pos;
    while (iAss < len && mStr[iAss] != '\0')
    {
        Temp.mStr[iPus] = mStr[iAss];
        iPus++;
        iAss++;
    }
    Temp.mStr[len - 1] = '\0';
    return String(Temp);
}
const char& String::at(const int& _pos)
{
    // TODO: insert return statement here
    if (_pos < 0 && _pos > mLen)
        std::cerr << "Error! Out of index!" << std::endl;
    return mStr[_pos];
}
int strCompare(const String& s1, const String& s2)
{
    if (s1.getMStr() < s2.getMStr())
        return -1;
    else
    {
        if (s1.getMStr() > s2.getMStr())
            return 1;
        else
            return 0;
    }
}

// ********************** In-Out Overloading **************************
std::ostream& operator<<(std::ostream& out, const String& s)
{
    // TODO: insert return statement here
    out << s.mStr << std::endl;
    return out;
}
std::istream& operator>>(std::istream& in, String& s)
{
    // TODO: insert return statement here:
    std::cout << std::endl;
    std::cout << "Enter string:" << std::endl;
    in.getline(s.mStr, 1024);
    String temp{ s };
    s.mLen = strlen(temp.mStr) + 1;
    s.allocSpace();
    for (int idex = 0; idex < s.mLen; idex++)
    {
        s.mStr[idex] = temp.mStr[idex];
    }
    return in;
}
