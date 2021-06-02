#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
class String
{
    private:
        char *mStr;
        int m_length;
    public:
        String() 
        {
            m_length = 0;
            mStr = new char[m_length + 1];
            mStr[0] = '\0';
        }
        String(const String& s)
        {
            mStr = new char[strlen(s.mStr) + 1];
            strcpy(mStr, s.mStr);
            m_length = strlen(s.mStr) + 1;
        }
        ~String() 
        {
            if (mStr != NULL)
            {
                delete[] mStr;
                mStr = NULL;
            }
        }
        String(char* s)
        {
            if (s == nullptr)
            {
                mStr = new char[1];
                mStr[0] = '\0';
            }
            else
            {
                mStr = new char[strlen(s) + 1];
                strcpy(mStr,s);
            }
        }
        void init();
        void remove(const String& s,const int& pos);
        void reveser();
        int getLength();
        int calLength();
        bool compare(const String& _s);
        void print();
        friend int countString(const String&, const String&);
        String& operator=(const String& s);
        friend ostream& operator << (ostream& out, const String& s);
        friend istream& operator >> (istream& in, String& s);
        String& operator-(const String& s);
        String& operator+(const String& s);
};



istream & operator >> (istream& in, String& s)
{
    char *str = new char[100];
    in.getline(str,100);
    in.sync();
    s.m_length = strlen(str);
    s.mStr = new char[s.m_length+1];
    strcpy(s.mStr,str);
    delete[] str;
    str = NULL;
    return in;
}
ostream & operator << (ostream &out, const String &s)
{
    out << s.mStr;
    return out;
}
String& String::operator=(const String& s)
{
    if (this == &s)
        return *this;
    delete[] mStr;
    mStr = NULL;
    mStr = new char[strlen(s.mStr) + 1];
    m_length = strlen(s.mStr) + 1;
    strcpy(mStr, s.mStr);
    return *this;
}

    reString& String::operator-(const String& s)
{
    cout << "Go here!" << endl;
    if (this->compare(s) == true)
    {
        cout << "Go here 1!" << endl;
        char* preStr = mStr;
        m_length = 0;
        mStr = new char[m_length + 1];
        mStr[0] = '\0';
        delete[] preStr;
        preStr = NULL;
        return *this;
    }
    else
    {
        cout << "Go here 2!" << endl;
        // save old String
        // count how many word s in mStr
        int count_s = countString(*this, s);
        cout << "Count =  " << count_s << endl;
        if (count_s == 0)
        {
            return *this;
        }
        else
        {
            char* save = mStr;
            int max_save = strlen(save);
            int max_s = strlen(s.mStr);
            m_length = max_save - count_s * max_s + 1;
            cout << "len: " << m_length << endl;
            char* mStr = new char[m_length];
            memset(mStr, 0, m_length);
            int max = max_save - max_s;
            int idex = 0;
            int idex_insert = 0;
            int flag = 0;
            while (idex < max_save)
            {
                // "Trieu Nhan" - 10
                // "Nhan" - 4
                // 7
                //    idex = 6
                if (save[idex] != s.mStr[0])
                {
                    *(mStr + idex_insert) = *(save + idex);
                    //mStr[idex_insert] = save[idex];
                    idex++;
                    idex_insert++;
                }
                else
                {
                    // idex = 6;
                    int t_idex = idex;
                    int t_idex_s2 = 0;
                    cout << s.mStr << "- max: " << max_s << endl;
                    while (t_idex_s2 < max_s)
                    {
                        // 'ABCDCEF' = 6
                        // 'DE' = 
                        cout << "save = " << *(save + t_idex) << "; s = " << *(s.mStr + t_idex_s2) << endl;
                        if (*(save+t_idex) != *(s.mStr + t_idex_s2))
                        {
                            flag++;
                            break;
                        }
                        else
                        {
                            cout << "call!" << endl;
                            t_idex++;
                            t_idex_s2++;
                        }
                    }
                    if (flag == 0)
                    {
                        idex = idex + max_s;
                    }
                    else
                    {
                        *(mStr + idex_insert) = *(save + idex);
                        //mStr[idex] = save[idex];
                        idex++;
                        idex_insert++;
                        flag = 0;
                    }
                }
            }
            mStr[m_length - 1] = '\0';
            delete[] save;
            save = NULL;
        }
    }
    return *this;
}
String& String::operator+(const String& s)
{
    char *save = mStr;
    m_length = m_length + strlen(s.mStr);
    mStr = new char[m_length + 2];
    strcpy(mStr,save);
    strcat(mStr," ");
    strcat(mStr,s.mStr);

    cout << save[0] << endl;
    cout << mStr[0] << endl;
    delete[] save;
    save = NULL;
    return *this;
}
int String::calLength()
{
    int count = 0;
    for (int idex = 0; idex < m_length; idex++)
    {
        count++;
    }
    return count;
}
bool String::compare(const String& _s)
{
    if (strcmp(mStr,_s.mStr) == 0 && m_length == _s.m_length)
        return true;
    return false;
}
void String::remove(const String& s, const int& pos)
{
    
}
int countString(const String& _longS, const String& _shortS)
{
    if (strlen(_longS.mStr) < strlen(_shortS.mStr))
    {
        cout << "_longS compare _shortS = 0" << endl; 
        return 0;
    }
    int count = 0;
    int max_loS = strlen(_longS.mStr); // "ABCD" => 4
    int max_shS = strlen(_shortS.mStr); // "BC"  => 2
    int idex_loS = 0;
    int idex_shS = 0;
    int flag = 0;
    while (idex_loS < max_loS)
    {
        cout << "Check ides_loS" << idex_loS << endl;
        cout << "_longs.mStr[idex_loS] = " << _longS.mStr[idex_loS] << endl;
        if (idex_loS > max_shS + 1)
        {
            return count;
        }
        if (_longS.mStr[idex_loS] == _shortS.mStr[idex_shS])
        {
            cout << "Going to check similar string!" << endl;
            int temp_idexloS = idex_loS + 1;
            idex_shS++;
            while (idex_shS < max_shS)
            {
                if (_longS.mStr[temp_idexloS] != _shortS.mStr[idex_shS])
                {
                    flag++;
                    break;
                }
                else
                {
                    idex_loS++;
                    temp_idexloS++;
                }
            }
            if (flag == 0)
            {
                count++;
            }
        }
        else
        {
            idex_loS++;
        }
    }

    return count;
}





int main()
{
    String s;
    char *name = new char[6];
    String s1{(char*)"Trieu"};
    String s2{(char*)"Nhan"};
    String s3{(char*)"Trieu"};
    // cout << "Enter s: ", cin >> s;
    // cout << s << endl;
    if (s1.compare(s3) == true)
    {
        cout << "True!" << endl;   
    }
    else
    {
        cout << "False!" << endl;
    }

    String s6 = s1 + s2;
    cout << s6 << endl;

    // String s7 = s6 - s2;
    // cout << s7 << endl;
    return 0;
}
