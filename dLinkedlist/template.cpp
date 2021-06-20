#include <iostream>

using namespace std;

class Student
{
    int _mID;
    std::string _mName;
    std::string _Class;

public:
    Student() : _mName{""} {} 
    Student(const int& _ID, const string& name) : _mID{_ID}, _mName{name} {}
    Student(const Student &sv) : _mID{sv._mID}, _mName{sv._mName} {}
    
    int getID() const
    {
        return _mID;
    }

    std::string getName() const
    {
        return _mName;
    }

    bool operator>(const Student& sv)
    {
        return _mID > sv._mID;
    }
    bool operator<(const Student& sv)
    {
        return _mID < sv._mID;
    }
    bool operator==(const Student& sv)
    {
        return _mID == sv._mID;
    }

    bool operator>(const int& _ID)
    {
        return _mID > _ID;
    }
    bool operator<(const int& _ID)
    {
        return _mID < _ID;
    }

    bool operator==(const int& _ID)
    {
        return _mID == _ID;
    }

    bool operator>(const string& name)
    {
        return _mName > name;
    }
    bool operator<(const string& name)
    {
        return _mName < name;
    }

    bool operator==(const string& name)
    {
        return _mName == name;
    }

    bool operator==(const string& _class)
    {
        return _Class == _class;
    }

    friend ostream &operator<<(ostream &os, const Student &sv)
    {
        os << "ID: " << sv._mID << endl;
        os << "Student: " << sv._mName << endl;
        return os;
    }
    friend istream &operator>>(istream &is, Student &sv)
    {
        std::cout << "Enter SV: " << endl;
        cout << "ID: ", is >> sv._mID;
        cout << "Student: ", is >> sv._mName;
        return is;
    }


};



template <class _Targ>
class dLinkedList
{
protected:
    struct nodeManaged
    {
        _Targ _value;
        nodeManaged *next;
        nodeManaged *prev;
        nodeManaged() = default;
    };

private:
    nodeManaged *_mManaged;
    nodeManaged *_mTail;
    nodeManaged *_mMiddle;
    int _mSize;
public:
    dLinkedList() : _mManaged{nullptr}
    {
        _mSize = 0;
    }
    dLinkedList(const _Targ &value)
    {
        _mManaged = new nodeManaged();
        _mManaged->_value = value;
        _mManaged->next = nullptr;
        _mManaged->prev = nullptr;
        _mTail = _mManaged;
        _mMiddle = _mManaged;
        _mSize++;
    }
    ~dLinkedList()
    {
        if (_mManaged == nullptr)
        {
            printf("Line: %d - Function(_mManaged == nullptr): %s\n", __LINE__,__FUNCTION__);
            return;
        }
        if(_mManaged)
        {
            printf("Line: %d - Function: %s(_mManaged->_mSize == 0 && _mManaged == nullptr)\n", __LINE__,__FUNCTION__);
            delete _mManaged;
            _mManaged = nullptr;
        }
        
    }

    void setMiddle()
    {
        if (_mManaged == nullptr)
        {
            _mMiddle = nullptr;
        }
        nodeManaged* managedRun = _mManaged;
        int iDex = 1;
        int _limit = _mSize / 2;
        while (iDex < _limit)
        {
            managedRun = managedRun->next;
            iDex++;
        }
        _mMiddle = managedRun;
    }

    nodeManaged* getManagedBlock()
    {
        return _mManaged;
    }

    void push_back(const _Targ &_value)
    {
        nodeManaged *newNode = new nodeManaged();
        newNode->_value = _value;
        newNode->next = nullptr;
        if (_mManaged == nullptr)
        {
            //std::cout << "come here!" << std::endl;
            newNode->prev = nullptr;
            _mManaged = newNode;
            _mTail = _mManaged;
            _mMiddle = _mManaged;
            _mSize++;
            setMiddle();
            return;
        }
        else
        {
            _mTail->next = newNode;
            newNode->prev = _mTail;
            _mTail = newNode;
            _mSize++;
            setMiddle();
        }
    }

    void pop_back()
    {
        if (_mManaged == nullptr)
        {
            throw std::runtime_error("Invalid!");
        }
        nodeManaged* saveNode = _mTail;
        _mTail = _mTail->prev;
        _mTail->next = nullptr;
        delete saveNode;
        saveNode = nullptr;
        _mSize--;
        if (_mSize == 0)
        {
            _mManaged = nullptr;
        }
        setMiddle();
    }


    void push_front(const _Targ& _value)
    {
        nodeManaged *newNode = new nodeManaged();
        newNode->_value = _value;
        newNode->prev = nullptr; // push front -> this one always be nullptr
        if (_mManaged == nullptr)
        {
            newNode->next = nullptr;
            _mManaged = newNode;
            _mTail = _mManaged;
            _mMiddle = _mManaged;
            _mSize++;
            return;
        }
        else
        {
            newNode->next = _mManaged;
            _mManaged->prev = newNode;
            _mManaged = newNode;
            _mSize++;
        }
        setMiddle();
    }


    void pop_front()
    {
        if (_mManaged == nullptr)
        {
            throw std::runtime_error("Invalid!");
        }
        nodeManaged * savedNode = _mManaged;
        _mManaged = _mManaged->next;
        _mManaged->prev = nullptr;
        delete savedNode;
        savedNode = nullptr;
        _mSize--;
        if (_mSize == 0)
        {
            _mManaged = nullptr;
        }
        setMiddle();
    }

    void searchID(const int& ID_find)
    {  
        if (_mManaged == nullptr)
        {
            cout << "Not Found! List is Empty!" << std::endl;
            return;
        }
        nodeManaged* nodeSearching = _mManaged;
        while(nodeSearching != nullptr)
        {
            if (nodeSearching->_value == ID_find)
            {
                cout << "Found! This is Information: " << std::endl;
                cout << nodeSearching->_value;
                return;
            }
            nodeSearching = nodeSearching->next;
        }
        cout << "Not Found!" << std::endl;
    }

    void binarySearchID(const int& ID_find)
    {  
        if (_mManaged == nullptr)
        {
            cout << "Not Found! List is Empty!" << std::endl;
            return;
        }
        nodeManaged* nodeSearching = _mMiddle;

        while (nodeSearching != nullptr)
        {
            if (nodeSearching->_value == ID_find)
            {
                cout << "Found! This is Information: " << std::endl;
                cout << nodeSearching->_value;
                return;
            }

            if (nodeSearching->_value < ID_find)
            {
                nodeSearching = nodeSearching->next;
            }
            if (nodeSearching->_value > ID_find)
            {
                nodeSearching = nodeSearching->prev;
            }
        }    
        cout << "Not Found!: " << std::endl;
    }

    void insert(const _Targ& _value, const int& _nth)
    {
                
    }

    void bubbleSortID()
    {
        if (_mManaged == nullptr)
        {
            std::cout << "Empty list!" << std::endl;
            return;
        }
        
        nodeManaged* _mManagedRun; // point to head of list 
        bool swap = true;
        while (swap)
        {
            _mManagedRun = _mManaged;
            swap = false;
            while (_mManagedRun->next!=nullptr)
            {
                if (_mManagedRun->_value > _mManagedRun->next->_value)
                {
                    _Targ temp = _mManagedRun->_value;
                    _mManagedRun->_value = _mManagedRun->next->_value;
                    _mManagedRun->next->_value = temp;
                    swap = true;
                }
                _mManagedRun = _mManagedRun->next;
            }
        }
    }

    void print()
    {
        if (_mManaged == nullptr)
            return;
        nodeManaged* _managedRun = _mManaged;
        while (_managedRun != nullptr)
        {
            std::cout << _managedRun->_value << std::endl;
            _managedRun = _managedRun->next;
        }
        std::cout << endl;
    }

    dLinkedList& operator->()
    {
        return *_mManaged;
    }

    dLinkedList& operator*()
    {
        return *_mManaged;
    }

};


int main()
{
    dLinkedList<Student>* studentList = new dLinkedList<Student>();
    Student sv1(23,"Trieu John 1");
    Student sv2(234,"Trieu John 2");
    Student sv3(235,"Trieu John 3");
    Student sv4(66,"Trieu John 4");
    Student sv5(13231,"Trieu John 5");
    Student sv6(324,"Trieu John 6");
    Student sv7(43,"Trieu John 7");
    Student sv8(33532,"Trieu John 8");
    Student sv9(3551,"Trieu John 9");
    Student sv10(23432135,"Trieu John 10");
    Student sv11(232123135,"Trieu John 11");

    // Student sv2;
    // Student sv3;
    //std::cout << "Enter sv : " << std::endl;
    //std::cin >> sv1;
    printf("Line: %d - Function: %s\n", __LINE__,__FUNCTION__);
    studentList->push_back(sv1);
    studentList->push_back(sv2);
    studentList->push_back(sv3);
    studentList->push_back(sv4);
    studentList->push_front(sv11);
    studentList->push_back(sv5);
    // studentList->push_back(sv6);
    // studentList->push_front(sv7);
    // studentList->push_back(sv8);
    // studentList->push_back(sv9);
    // studentList->push_front(sv10);
    // std::cout << "Full: " << std::endl;
    // studentList->print();
    // studentList->pop_back();
    // std::cout << "pop 1: " << std::endl;
    // studentList->print();
    // studentList->pop_back();
    // std::cout << "pop 2: " << std::endl;
    // studentList->print();
    // studentList->pop_back();
    // std::cout << "pop 3: " << std::endl;
    // studentList->print();=-
    // studentList->pop_back();
    // std::cout << "pop 4: " << std::endl;
    std::cout << "******************************Print List!***************************" << std::endl;
    studentList->print();

    studentList->bubbleSortID();
    std::cout << "After sorting by ID!" << std::endl;
    studentList->print();


    studentList->searchID(13231);
    std::cout << std::endl;

    studentList->binarySearchID(13231);
    std::cout << std::endl;


    if (studentList)
    {
        delete studentList;
        studentList = nullptr;
    }
    return 0;
}