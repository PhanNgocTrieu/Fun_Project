#pragma once
#ifndef __STRING_H__
#define __STRING_H__
#include <iostream>

class String
{
private:
    char* mStr;
    int mLen;
public:
    String();
    String(const int&);
    String(const char*);
    String(const String&);
    ~String();
 
    friend std::ostream& operator << (std::ostream& out, const String& s);
    friend std::istream& operator >> (std::istream& in, String& s);
    String& operator=(const String&);
    String& operator+=(const String&);
    bool operator==(const String&);
    bool operator>(const String&);
    bool operator<(const String&);

    // Method:
    void erase(const int& _beg, const int& _count);
    void insert(const int& _pos, const String& _s);
    void push_back(const char&);
    void pop_back();
    String substr(const int& _pos, const size_t& _count);
    const char& at(const int& _pos);



    void allocSpace();
public:
    char* getMStr() const;
    void setMStr(char* mStr);

    int getMlength() const;
    void setMlength(int mlength);
    int length();
};


//String operator-(const String&, const String&);

int strCompare(const String&, const String&);
String operator+(const String&, const String&);
String operator+(const String&, const char*&);
#endif // !__STRING_H__
