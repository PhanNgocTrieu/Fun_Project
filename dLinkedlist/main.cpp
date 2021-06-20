#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <limits>
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

    bool checkName() const
    {
        for (char c : _mName)
        {
            if (c == ' ')
                continue;
            else
            {
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
                {
                    return false;
                }
            }
            
        }
        return true;
    }

    friend ostream &operator<<(ostream &os, const Student &sv)
    {
        os << "\tID : " << sv._mID << " ------- Name: " << sv._mName;
        return os;
    }
    friend istream &operator>>(istream &is, Student &sv)
    {
        printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
        std::cout << "Enter SV: " << endl;
        cout << "ID_enter: ", is >> sv._mID;
        while (is.fail())
        {
            if (is.fail())
            {
                is.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<< "You have entered wrong input" <<endl;
                cout << "ID_enter: ", is >> sv._mID;
            }
        }
        // clear buffer
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
        cout << "Name_enter: ", getline(is,sv._mName);
        while (!sv.checkName())
        {
            cout << "You enter wrong input - Not include special characters!" << endl;
            cout << "Name_enter: ", getline(is,sv._mName,'\n');
        }
        is.sync();
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
        printf("Line: %d - Function: %s( const Student& _student )\n", __LINE__, __FUNCTION__);
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
                cout << "Found ID: " << ID_find << " -- This is Information: " << std::endl;
                cout << listSearching->mStudent;
                return;
            }
            listSearching = listSearching->next;
        }
        cout << "Not Found!" << std::endl;
    }

    void searchName(const string &nameFind)
    {
        printf("Line: %d - Function: %s( const string& nameFind )\n", __LINE__, __FUNCTION__);
        if (mSvList == nullptr)
        {
            cout << "Not Found! List is Empty!" << std::endl;
            return;
        }
        nodeBlock *listSearching = mSvList;
        while (listSearching != nullptr)
        {
            if (listSearching->mStudent.getName() == nameFind)
            {
                cout << "Found name: " << nameFind << " -- This is Information: " << std::endl;
                cout << listSearching->mStudent;
                return;
            }
            listSearching = listSearching->next;
        }
        cout << "Not Found!" << std::endl;
    }

    void binarySearchID(const int &ID_find)
    {
        printf("Line: %d - Function: %s(const int& ID_find)\n", __LINE__, __FUNCTION__);
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
        printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
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
        int i = 1;
        while (i < _pos)
        {
            i++;
            prev = listMove;
            listMove = listMove->next;
        }
        prev->next = newNode;
        newNode->prev = prev;
        newNode->next = listMove;
        listMove->prev = newNode;

        prev = nullptr;
        listMove = nullptr;
        _mSize++;
        setMiddle();
    }

    void remove(const int &ID_find)
    {
        printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
        if (mSvList == nullptr)
        {
            cout << "Not Found! Empty List!" << endl;
            return;
        }
        nodeBlock *prevNode = nullptr;
        nodeBlock *saveNode = mSvList;
        while (saveNode != nullptr)
        {
            if (saveNode->mStudent.getID() == ID_find)
            {
                // this for last node
                if (saveNode->next == nullptr)
                {
                    prevNode->next = nullptr;
                    saveNode->prev = nullptr;
                    mTail = prevNode;
                }
                else
                {
                    // node is in middle
                    prevNode->next = saveNode->next;
                    saveNode->next->prev = prevNode;
                }
                delete saveNode;
                saveNode = nullptr;
                _mSize--;
                if (_mSize == 0)
                {
                    mSvList = nullptr;
                }
                setMiddle();
                return;
            }
            prevNode = saveNode;
            saveNode = saveNode->next;
        }
        cout << "Not Found!" << endl;
    }

    void pop_back()
    {
        if (mSvList == nullptr)
        {
            printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
            throw std::runtime_error("Invalid!");
        }
        else
        {
            printf("Line: %d - Function: %s( svList->next != nullptr )\n", __LINE__, __FUNCTION__);
            nodeBlock *saveNode = this->mTail;
            mTail = mTail->prev;
            saveNode->next = nullptr;
            saveNode->prev = nullptr;
            delete saveNode;
            saveNode = nullptr;
            _mSize--;
            if (_mSize == 0)
            {
                mSvList = nullptr;
            }
        }
        setMiddle();
        printf("Line: %d - Function: %s( Out of Pop_back() )\n", __LINE__, __FUNCTION__);
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
            _mSize--;
            if (_mSize == 0)
            {
                mSvList = nullptr;
            }
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
        cout << "\nPrint List: " << endl;
        if (mSvList == nullptr)
            return;
        nodeBlock *List = mSvList;
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
    Student sv12;
    cin >> sv12;
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
    printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
    svList->insert(sv8, 3);
    printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);

    // cout << "***************************** List 2! **************************" << endl;
    // svList2->push_front(sv2);
    // svList2->pop_back();
    // svList2->print();
    // cout << "***************************** AFTER SORTING ID BY BUBBLE SORT! **************************" << endl;
    // svList->bubbleSortID();
    // svList->print();

    // cout << "****************** Searching in Bianry Seaching Way of List in ordering! ****************" << endl;
    //  printf("Line: %d - Function: %s( binary searching )\n", __LINE__, __FUNCTION__);
    // svList->binarySearchID(324);
    // std::cout << std::endl;
    // printf("Line: %d - Function: %s( binary Searching )\n", __LINE__, __FUNCTION__);
    // svList->binarySearchID(13231);
    // std::cout << std::endl;
    // cout << "****************** Searching in Normal Way of List in ordering! ****************" << endl;
    // printf("Line: %d - Function: %s( Normal Searching )\n", __LINE__, __FUNCTION__);
    // svList->searchID(13231);
    // std::cout << std::endl;
    // printf("Line: %d - Function: %s( Normal Searching )\n", __LINE__, __FUNCTION__);
    // svList->searchName("John 3");
    // std::cout << std::endl;
    // svList->searchName("Trieu John 8");
    // std::cout << std::endl;
    svList->insert(sv12, 5);
    svList->print();

    // cout << "****************** Removing by ID ****************" << endl;
    // svList->remove(66);
    // svList->print();

    cout << "****************** Deallocated All Memories Have Been Allocated ************" << endl;
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