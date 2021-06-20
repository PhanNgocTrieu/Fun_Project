#include <iostream>

using namespace std;

class Student
{
    int _mID;
    std::string _mName;

public:
    Student() : _mName{""} {}
    Student(const int &_ID, const string &name) : _mID{_ID}, _mName{name} {}
    Student(const Student &sv) : _mID{sv._mID}, _mName{sv._mName} {}
    ~Student()
    {
    }
    int getID() const
    {
        return _mID;
    }
    void setID(const int &id)
    {
        _mID = id;
    }
    std::string getName() const
    {
        return _mName;
    }
    void setName(const string &name)
    {
        _mName = name;
    }

    friend ostream &operator<<(ostream &os, const Student &sv)
    {
        os << "ID: " << sv._mID << "\tStudent: " << sv._mName << endl;
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

class dLinkedList
{
protected:
    struct nodeBlock
    {
        Student mStudent;
        nodeBlock *next;
        nodeBlock *prev;
        nodeBlock() : next{nullptr}, prev{nullptr} {}
        nodeBlock(const Student &st) : mStudent{st}, next{nullptr}, prev{nullptr} {}
        ~nodeBlock()
        {
            printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
            if (next)
            {
                next = nullptr;
            }
            if (prev)
            {
                prev = nullptr;
            }
        }
    };
    nodeBlock *mSvList;
    nodeBlock *mMiddle;
    nodeBlock *mTail;
    int _mSize;

public:
    dLinkedList() : mSvList{nullptr}, mMiddle{nullptr}, mTail{nullptr}, _mSize{0} {}
    dLinkedList(const dLinkedList &svList) : mSvList{svList.mSvList}, mMiddle{svList.mMiddle}, mTail{svList.mTail}, _mSize{svList._mSize} {}
    ~dLinkedList()
    {
        printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
        if (mSvList)
        {
            while (mTail != nullptr && mTail->prev != nullptr)
            {
                nodeBlock *save = mTail;
                mTail = mTail->prev;
                delete save;
                save = nullptr;
            }
        }
        mSvList = nullptr;
        mMiddle = nullptr;
        printf("Line: %d - Function: %s( delete mSvList - DONE )\n", __LINE__, __FUNCTION__);
    }

    nodeBlock &getSvList() const
    {
        return *mSvList;
    }
    nodeBlock &getMiddle() const
    {
        return *mMiddle;
    }
    nodeBlock &getTail() const
    {
        return *mTail;
    }
    int getSize() const
    {
        return _mSize;
    }

    void setMiddle()
    {
        printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
        if (mSvList == nullptr)
        {
            mMiddle = nullptr;
        }
        nodeBlock *List = mSvList;
        int iDex = 1;
        int _limit = _mSize / 2;
        while (iDex < _limit && List->next != nullptr)
        {
            List = List->next;
            iDex++;
        }
        mMiddle = List;
        List = nullptr;
    }

    void push_back(const Student &_student)
    {
        printf("Line: %d - Function: %s( const Student &_student )\n", __LINE__, __FUNCTION__);
        nodeBlock *newNode = new nodeBlock();
        newNode->mStudent = _student;
        newNode->next = nullptr;

        if (mSvList == nullptr)
        {
            newNode->prev = nullptr;
            this->mSvList = newNode;
            mTail = mSvList;
            mMiddle = mSvList;
            _mSize++;
            return;
        }
        newNode->prev = mTail;
        mTail->next = newNode;
        mTail = newNode;
        setMiddle();
        _mSize++;
    }

    void push_front(const Student &_student)
    {
        printf("Line: %d - Function: %s( const Student& _student )\n", __LINE__, __FUNCTION__);
        nodeBlock *newNode = new nodeBlock();
        newNode->mStudent = _student;
        newNode->prev = nullptr; // push front -> this one always be nullptr
        if (mSvList == nullptr)
        {
            newNode->next = nullptr;
            mSvList = newNode;
            mTail = mSvList;
            mMiddle = mSvList;
            _mSize++;
        }
        else
        {
            newNode->next = mSvList;
            mSvList->prev = newNode;
            mSvList = newNode;
            _mSize++;
        }
        setMiddle();
    }

    void searchID(const int &ID_find)
    {
        if (mSvList == nullptr)
        {
            cout << "Not Found! List is Empty!" << std::endl;
            return;
        }
        nodeBlock *listSearching = mSvList;
        while (listSearching != nullptr)
        {
            if (listSearching->mStudent.getID() == ID_find)
            {
                cout << "Found! This is Information: " << std::endl;
                cout << listSearching->mStudent;
                return;
            }
            listSearching = listSearching->next;
        }
        cout << "Not Found!" << std::endl;
    }

    void binarySearchID(const int &ID_find)
    {
        if (mSvList == nullptr)
        {
            cout << "Not Found! List is Empty!" << std::endl;
            return;
        }
        nodeBlock *listSearching = mSvList;

        while (listSearching != nullptr)
        {
            if (listSearching->mStudent.getID() == ID_find)
            {
                cout << "Found! This is Information: " << std::endl;
                cout << listSearching->mStudent;
                return;
            }

            if (listSearching->mStudent.getID() < ID_find)
            {
                listSearching = listSearching->next;
            }
            if (listSearching->mStudent.getID() > ID_find)
            {
                listSearching = listSearching->prev;
            }
        }
        cout << "Not Found!: " << std::endl;
    }

    void insert(const Student &_student, const int &_pos)
    {
        if (_pos < 0 || _pos > _mSize)
        {
            throw std::runtime_error("Invalid!");
        }
        if (_pos == _mSize)
        {
            push_back(_student);
            return;
        }
        if (_pos == 0)
        {
            push_front(_student);
        }
        nodeBlock *newNode = new nodeBlock(_student);
        nodeBlock *prev = nullptr;
        nodeBlock *listMove = mSvList;
        int i = 0;
        while (i < _pos)
        {
            i++;
            prev = listMove;
            listMove = listMove->next;
        }
        newNode->next = listMove;
        newNode->prev = listMove->prev;
        listMove->prev->next = newNode;
        listMove->prev = newNode;
    }

    void pop_front()
    {
        if (mSvList == nullptr)
        {
            printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
            throw std::runtime_error("Invalid!");
        }
        else
        {
            printf("Line: %d - Function: %s( svList->next != nullptr )\n", __LINE__, __FUNCTION__);
            nodeBlock *saveNode = this->mSvList;
            mSvList = mSvList->next;
            saveNode->next = nullptr;
            saveNode->prev = nullptr;
            delete saveNode;
            saveNode = nullptr;
            printf("Line: %d - Function: %s( svList->next != nullptr )\n", __LINE__, __FUNCTION__);
            _mSize--;
        }
        setMiddle();
        printf("Line: %d - Function: %s( Out of Pop_front() )\n", __LINE__, __FUNCTION__);
    }

    void bubbleSortID()
    {
        if (mSvList == nullptr)
        {
            std::cout << "Empty list!" << std::endl;
            return;
        }

        nodeBlock *List; // point to head of list
        bool swap = true;
        while (swap)
        {
            List = mSvList;
            swap = false;
            while (List->next != nullptr)
            {
                if (List->mStudent.getID() > List->next->mStudent.getID())
                {
                    int _idValue = List->mStudent.getID();
                    List->mStudent.setID(List->next->mStudent.getID());
                    List->next->mStudent.setID(_idValue);
                    swap = true;
                }
                List = List->next;
            }
        }
    }

    void print()
    {
        if (mSvList == nullptr)
            return;
        nodeBlock *List = mSvList;
        cout << "Print List: " << endl;
        cout << "KeyID\t Student!" << endl;
        while (List != nullptr)
        {
            std::cout << List->mStudent << std::endl;
            List = List->next;
        }
        std::cout << endl;
    }
};

int main()
{
    //*********** INTITIALIZING ****************
    dLinkedList *svList = new dLinkedList;
    dLinkedList *svList2 = new dLinkedList;
    Student sv1(23, "Trieu John 1");
    Student sv2(234, "Trieu John 2");
    Student sv3(235, "Trieu John 3");
    Student sv4(66, "Trieu John 4");
    Student sv5(13231, "Trieu John 5");
    Student sv6(324, "Trieu John 6");
    Student sv7(43, "Trieu John 7");
    Student sv8(33532, "Trieu John 8");
    Student sv9(3551, "Trieu John 9");
    Student sv10(23432135, "Trieu John 10");
    Student sv11(232123135, "Trieu John 11");

    //************* PUSHING DATA **************
    cout << "***************************** List 1! **************************" << endl;
    printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
    svList->push_back(sv1);
    svList->push_back(sv2);
    svList->push_back(sv3);
    svList->push_back(sv4);
    printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
    svList->push_front(sv5);
    svList->push_front(sv6);
    svList->push_front(sv7);
    svList->print();
    svList->pop_front();
    printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
    svList->print();
    svList->insert(sv8, 3);

    cout << "***************************** List 2! **************************" << endl;
    svList2->push_front(sv2);
    svList2->pop_front();

    cout << "***************************** AFTER SORTING ID BY BUBBLE SORT! **************************" << endl;
    //svList->bubbleSortID();
    //svList->print();

    cout << "******************************** Searching in Normal Way! *******************************" << endl;
    //svList->searchID(324);

    cout << "****************** Searching in Bianry Seaching Way of List in ordering! ****************" << endl;
    //svList->binarySearchID(324);

    std::cout << std::endl;
    // //std::cout << "Enter sv : " << std::endl;
    // //std::cin >> sv1;
    // printf("Line: %d - Function: %s\n", __LINE__,__FUNCTION__);
    // studentList->push_back(sv1);
    // studentList->push_back(sv2);
    // studentList->push_back(sv3);
    // studentList->push_back(sv4);
    // studentList->push_front(sv11);
    // studentList->push_back(sv5);
    // // studentList->push_back(sv6);
    // // studentList->push_front(sv7);
    // // studentList->push_back(sv8);
    // // studentList->push_back(sv9);
    // // studentList->push_front(sv10);
    // // std::cout << "Full: " << std::endl;
    // // studentList->print();
    // // studentList->pop_back();
    // // std::cout << "pop 1: " << std::endl;
    // // studentList->print();
    // // studentList->pop_back();
    // // std::cout << "pop 2: " << std::endl;
    // // studentList->print();
    // // studentList->pop_back();
    // // std::cout << "pop 3: " << std::endl;
    // // studentList->print();=-
    // // studentList->pop_back();
    // // std::cout << "pop 4: " << std::endl;
    // std::cout << "******************************Print List!***************************" << std::endl;
    // studentList->print();

    // studentList->bubbleSortID();
    // std::cout << "After sorting by ID!" << std::endl;
    // studentList->print();

    svList->searchID(13231);
    std::cout << std::endl;

    svList->binarySearchID(13231);
    std::cout << std::endl;

    if (svList)
    {
        delete svList;
        svList = nullptr;
    }
    if (svList2)
    {
        delete svList2;
        svList2 = nullptr;
    }
    printf("Line: %d - Function: %s( End of Main! )\n", __LINE__, __FUNCTION__);
    return 0;
}