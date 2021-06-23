#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
class Student
{
    string mName;
    string mDate;
    string mClass;

public:
    Student() : mName{""}, mDate{""}, mClass{""} {}
    Student(const string &_name, const string &_date, const string &_class)
        : mName{_name}, mDate{_date}, mClass{_class} {}
    Student(const Student &st)
    {
        mName = st.mName;
        mDate = st.mDate;
        mClass = st.mClass;
    }
    ~Student() {}

    void setName(const string &_name)
    {
        mName = _name;
    }
    void setDate(const string &_date)
    {
        mDate = _date;
    }
    void setClass(const string &_class)
    {
        mClass = _class;
    }
    string getName() const
    {
        return mName;
    }
    string getDate() const
    {
        return mDate;
    }
    string getClass() const
    {
        return mClass;
    }

    friend istream &operator>>(istream &is, Student &obj)
    {
        cout << endl;
        cout << "Enter infor for Student!" << endl;
        cout << "Name: ", is >> obj.mName;
        cout << "Date: ", is >> obj.mDate;
        cout << "Class: ", is >> obj.mClass;

        return is;
    }

    friend ostream &operator<<(ostream &os, const Student &obj)
    {
        os << "Name: " << obj.mName << " - Date: " << obj.mDate << " - Class: " << obj.mClass << endl;
        return os;
    }
};

class StudentManage
{
protected:
    struct StudentNode
    {
        Student _student;
        StudentNode *next;
        StudentNode() : next{nullptr} {}
        StudentNode(const Student &student)
        {
            _student = student;
            next = nullptr;
        }
    };

public:
    StudentManage() : mStudentList{nullptr}, mTail{nullptr}, _mSize{0} {}
    StudentManage(const StudentManage &_stuListMange)
    {
        mStudentList = _stuListMange.mStudentList;
        mTail = _stuListMange.mTail;
        _mSize = _stuListMange._mSize;
    }
    StudentManage(const Student &_student)
    {
        mStudentList->_student = _student;
        mStudentList->next = nullptr;
        mTail = mStudentList;
        _mSize++;
    }
    ~StudentManage()
    {
        if (mStudentList)
        {
            delete mStudentList;
            mStudentList = nullptr;
        }
    }

    /**
     * @brief   push_back - insert elem at the back of list
     * @algorithm: 
     *          tail->next = newNode
     *          tail = newNode
     * @result  A list was added a node at back
    */
    void push_back(const Student &_student)
    {
        printf("Line: %d - Function: %s( const Student &_student )\n", __LINE__, __FUNCTION__);
        StudentNode *newNode = new StudentNode();
        newNode->_student = _student;
        newNode->next = nullptr;
        if (mStudentList == nullptr)
        {
            printf("Line: %d - Function: %s( List == null )\n", __LINE__, __FUNCTION__);
            mStudentList = newNode;
            mTail = mStudentList;
        }
        else
        {
            mTail->next = newNode;
            mTail = newNode;
        }
        // newNode points to null
        _mSize++;
        printf("Line: %d - Function: %s( End )\n", __LINE__, __FUNCTION__);
    }

    void pop_back()
    {
        if (mStudentList == nullptr)
        {
            cout << "Empty list!" << endl;
            return;
        }
        StudentNode *prev = nullptr;
        StudentNode *curr = mStudentList;
        while (curr != nullptr && curr->next != nullptr)
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = nullptr;
        mTail = prev;
        delete curr;
        curr = nullptr;
        prev = nullptr;
        _mSize--;
        if (_mSize == 0)
        {
            mStudentList = nullptr;
        }
        handleFile();
    }   

    /**
     * @brief push_back - insert elem at the back of list
     * @algorithm: 
     *          newNode->next = nullptr
     *          tail->next = newNode
     *          head = newNode (make head point to the first elem of list)
     * @result Print infors if true - otherwise "Not Found"
    */
    void push_front(const Student &_student)
    {
        printf("Line: %d - Function: %s( const Student &_student )\n", __LINE__, __FUNCTION__);
        StudentNode *newNode = new StudentNode();
        newNode->_student = _student;
        if (mStudentList == nullptr)
        {
            newNode->next = nullptr;
            mStudentList = newNode;
            mTail = mStudentList;
        }
        else
        {
            newNode->next = mStudentList;
            mStudentList = newNode;
        }
        // newNode points to null
        newNode = nullptr;
        _mSize++;
    }

    void pop_front()
    {
        if (mStudentList == nullptr)
        {
            cout << "Empty list!" << endl;
            return;
        }
        StudentNode *saveNode = mStudentList;
        mStudentList = mStudentList->next;
        saveNode->next = nullptr;
        delete saveNode;
        saveNode = nullptr;
        _mSize--;
        if (_mSize == 0)
        {
            mStudentList = nullptr;
        }
        handleFile();
    }

    /**
     * @brief   Insert - inserting elem which is @a _pos position
     * 
     * @algorithm: 
     *              if (_pos < 0 && _pos > _mSize)
     *                  return runtime::error("Invalid!");
     *              else
     *                  searching _pos
     *                  insert elem in _pos
     *              delete tempory pointers
     * @result  A list was insert an elem in @a _pos position
    */
    void insert(const Student &_student, const int &_pos)
    {
        printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
        if (_pos < 0 || _pos > _mSize)
        {
            throw runtime_error("Invalid!");
        }
        if (_pos == 0)
        {
            push_front(_student);
            return;
        }
        if (_pos == _mSize)
        {
            push_back(_student);
            return;
        }
        StudentNode *prev = nullptr;
        StudentNode *curr = mStudentList;
        StudentNode *newNode = new StudentNode(_student);
        int idex = 0;
        while (idex < _pos)
        {
            prev = curr;
            curr = curr->next;
            idex++;
        }
        prev->next = newNode;
        newNode->next = curr;
        prev = nullptr;
        curr = nullptr;
        newNode = nullptr;
        _mSize++;
        handleFile();
    }

    void Remove(const int &_pos)
    {
        printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
        if (_pos < 0 || _pos > _mSize)
        {
            throw runtime_error("Invalid!");
        }
        if (_pos == 0)
        {
            pop_front();
            return;
        }
        if (_pos == _mSize)
        {
            pop_back();
            return;
        }
        StudentNode *prev = nullptr;
        StudentNode *curr = mStudentList;
        int idex = 0;
        while (idex < _pos)
        {
            prev = curr;
            curr = curr->next;
            idex++;
        }
        prev->next = curr->next;
        curr->next = nullptr;
        delete curr;
        curr = nullptr;

        handleFile();
    }


    void sortingByName()
    {
        if (mStudentList == nullptr)
        {
            cout << "Empty list!" << endl;
            return;
        }

        StudentNode *List; // point to head of list
        bool swap = true;
        while (swap)
        {
            List = mStudentList;
            swap = false;
            while (List->next != nullptr)
            {
                if (List->_student.getName() > List->next->_student.getName())
                {
                    string _nameVal = List->next->_student.getName();
                    List->_student.setName(List->next->_student.getName());
                    List->next->_student.setName(_nameVal);
                    swap = true;
                }
                List = List->next;
            }
        }
        List = nullptr;
    }



    void handleFile()
    {
        StudentNode *runList = mStudentList;
        ofstream handleFile;
        handleFile.open("handle.txt", ios::app | ios::out);
        while (runList != nullptr)
        {
            handleFile << runList->_student.getName() << ", " << runList->_student.getDate() << ", " << runList->_student.getClass() << endl;
            runList = runList->next;
        }
        runList = nullptr;
        handleFile.close();
        remove("input.txt");
        rename("handle.txt", "input.txt");
    }

    void handleStringLine(const string &stringLine)
    {
        if (stringLine == "")
        {
            return;
        }
        Student createStudent;
        stringstream ss(stringLine);
        string word;
        int indexPushing = 0;

        while (getline(ss, word, ','))
        {
            string subStr;
            indexPushing++;
            if (word[0] == ' ')
            {
                subStr = word.substr(1, word.length() - 1);
            }
            else
            {
                subStr = word;
            }
            if (indexPushing == 1)
            {
                createStudent.setName(subStr);
            }
            if (indexPushing == 2)
            {
                createStudent.setDate(subStr);
            }
            if (indexPushing == 3)
            {
                createStudent.setClass(subStr);
            }
        }
        this->push_back(createStudent);
    }

    void readingFromFile(const string &filename)
    {
        ifstream fileRead(filename);
        if (fileRead.is_open())
        {
            string line;
            while (getline(fileRead, line))
            {
                cout << "line infor: " << line << endl;
                handleStringLine(line);
            }
            fileRead.close();
        }
        else
        {
            cout << "cout not read file!" << endl;
        }
    }

    friend ostream &operator<<(ostream &os, const StudentManage &stuMange)
    {
        os << "Student infor: " << endl;
        StudentManage::StudentNode *runList = stuMange.mStudentList;
        while (runList != nullptr)
        {
            os << runList->_student;
            runList = runList->next;
        }
        return os;
    }

    void print()
    {
        StudentNode *runList = mStudentList;
        while (runList != nullptr)
        {
            cout << runList->_student;
            runList = runList->next;
        }
        cout << endl;
    }

private:
    StudentNode *mStudentList;
    StudentNode *mTail;
    int _mSize;
};

int main()
{
    StudentManage *svList = new StudentManage();
    string myFile = "input.txt";
    Student sv1{"Phan Ngoc Trieu 6", "20/07", "Fresher_C/C++"};
    Student sv2{"Nguyen Nhat Anh","23/3","Life"};
    Student sv3{"Nguyen Minh Vuong","23/7","Soccer"};
    svList->readingFromFile(myFile);
    cout << endl;
    svList->print();
    svList->insert(sv1,7);
    svList->insert(sv2,3);
    svList->insert(sv3,1);
    svList->print();
    svList->sortingByName();
    svList->Remove(4);
    svList->print();


    if (svList)
    {
        delete svList;
        svList = nullptr;
    }

    printf("Line: %d - Function: %s( End of Main )\n", __LINE__, __FUNCTION__);
    return 0;
}