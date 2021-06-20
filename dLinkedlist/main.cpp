#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <limits>
using namespace std;

class Student
{
    int _mID;
    string _mName;

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
    string getName() const
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
        cout << "Enter SV: " << endl;
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


    /**
     * @brief setMiddle - middle pointer point to the middle elem of list
     * @algorithm: 
     *          passing to middle of list
     *          middle = curr (middle of list)
     * @result Print infors if true - otherwise "Not Found"
    */
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

    /**
     * @brief push_back - insert elem at the back of list
     * @algorithm: 
     *          newNode->prev = tail
     *          tail->next = newNode
     *          tail = newNode
     *          head = newNode (make head point to the first elem of list)
     * @result Print infors if true - otherwise "Not Found"
    */
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
            newNode = nullptr;
            return;
        }
        newNode->prev = mTail;
        mTail->next = newNode;
        mTail = newNode;
        setMiddle();
        _mSize++;

        // newNode points to null
        newNode = nullptr;
    }

    /**
     * @brief push_front - insert elem at the front of list
     * @algorithm: 
     *          newNode->prev = nullptr
     *          newNode->next = head
     *          head->prev = newNode
     *          head = newNode (make head point to the first elem of list)
     * @result Print infors if true - otherwise "Not Found"
    */
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

    /**
     * @brief searchingID - Searching elem has 
     * @param ID_find ID for searching
     * @algorithm: 
     *          passing all elems
     *          print infors if true - otherwise "Not Found"
     * @result Print infors if true - otherwise "Not Found"
    */
    void searchID(const int &ID_find)
    {
        if (mSvList == nullptr)
        {
            cout << "Not Found! List is Empty!" << endl;
            return;
        }
        nodeBlock *listSearching = mSvList;
        while (listSearching != nullptr)
        {
            if (listSearching->mStudent.getID() == ID_find)
            {
                cout << "Found ID: " << ID_find << " -- This is Information: " << endl;
                cout << listSearching->mStudent;
                return;
            }
            listSearching = listSearching->next;
        }
        cout << "Not Found!" << endl;
    }

    /**
     * @brief SearchingName - Searching elem has @a _nameFind value
     * @param nameFind name for searching
     * @algorithm: 
     *          Passing over all
     *          If true -> print
     *          otherwise -> "Not Found!"
     * @delete: temporary pointers
     * @result Print infors if true - otherwise "Not Found"
    */
    void searchName(const string &nameFind)
    {
        printf("Line: %d - Function: %s( const string& nameFind )\n", __LINE__, __FUNCTION__);
        if (mSvList == nullptr)
        {
            cout << "Not Found! List is Empty!" << endl;
            return;
        }
        nodeBlock *listSearching = mSvList;
        while (listSearching != nullptr)
        {
            if (listSearching->mStudent.getName() == nameFind)
            {
                cout << "Found name: " << nameFind << " -- This is Information: " << endl;
                cout << listSearching->mStudent;
                return;
            }
            listSearching = listSearching->next;
        }
        cout << "Not Found!" << endl;
    }

    /**
     * @brief binarySearchID - Using binary searching algorithm to search elem has ID value
     * @algorithm: 
     *          
     * @result Print infors if true - otherwise "Not Found"
    */
    void binarySearchID(const int &ID_find)
    {
        printf("Line: %d - Function: %s(const int& ID_find)\n", __LINE__, __FUNCTION__);
        if (mSvList == nullptr)
        {
            cout << "Not Found! List is Empty!" << endl;
            return;
        }
        nodeBlock *listSearching = mSvList;

        while (listSearching != nullptr)
        {
            if (listSearching->mStudent.getID() == ID_find)
            {
                cout << "Found! This is Information: " << endl;
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
        cout << "Not Found!: " << endl;
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

        // clear pointers
        prev = nullptr;
        listMove = nullptr;
        newNode = nullptr;

        _mSize++;
        setMiddle();
    }

    /**
     * @brief   remove - remove elem which has the same ID we input
     * @param ID_find   input ID for searching
     * @algorithm:  return if list is nullptr
     *              else
     *                  passing while list to find elem has the same ID
     *                  if (found)
     *                      // This is last node
     *                      if (curr->next != nullptr)
     *                      *prev->next = nullptr (make last pointer equal nullptr)
     *                      *curr->prev = nullptr (make prev pointer equal nullptr)
     *                  else
     *                      prevNode->next = curr->next;
     *                      curr->next->prev = prevNode;
     *              deleve curr;
     *              prev = nullptr;
     * @result A list was removed elem has the same input ID
    */
    void remove(const int &ID_find)
    {
        printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
        if (mSvList == nullptr)
        {
            cout << "Not Found! Empty List!" << endl;
            return;
        }
        nodeBlock *prevNode = nullptr;
        nodeBlock *curr = mSvList;
        while (curr != nullptr)
        {
            if (curr->mStudent.getID() == ID_find)
            {
                // this for last node
                if (curr->next == nullptr)
                {
                    prevNode->next = nullptr;
                    curr->prev = nullptr;
                    mTail = prevNode;
                }
                else
                {
                    // node is in middle
                    prevNode->next = curr->next;
                    curr->next->prev = prevNode;
                }
                delete curr;
                curr = nullptr;
                prevNode = nullptr;
                _mSize--;
                if (_mSize == 0)
                {
                    mSvList = nullptr;
                }
                setMiddle();
                return;
            }
            prevNode = curr;
            curr = curr->next;
        }
        cout << "Not Found!" << endl;
        prevNode = nullptr;
        curr = nullptr;
    }

    /**
     * @brief pop_front - popping elem which is in front of list
     * 
     * @algorithm: return if list is nullptr
     *             else
     *              *saveNode = *tail
     *              *tail->prev->next = nullptr (move to one elem)
     *              *saveNode->prev = nullptr (make prev pointer equal nullptr)
     *              deleve saveNode
     * @result A list was removed first elem
    */
    void pop_back()
    {
        if (mSvList == nullptr)
        {
            printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
            throw runtime_error("Invalid!");
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

    /**
     * @brief pop_front - popping elem which is in front of list
     * 
     * @algorithm: return if list is nullptr
     *             else
     *              *saveNode = *head
     *              *head = head->next (move to one elem)
     *              *heaed->prev = nullptr (make prev pointer equal nullptr)
     *              deleve saveNode
     * @result A list was removed first elem
    */
    void pop_front()
    {
        if (mSvList == nullptr)
        {
            printf("Line: %d - Function: %s( svList == nullptr )\n", __LINE__, __FUNCTION__);
            throw runtime_error("Invalid!");
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

    /**
     * @brief BubbleSortID - do sorting elems in list by value of ID - increasingly
     * @param void return
     * @result void - *this - A List was ordered by ID
     * 
    **/
    void bubbleSortID()
    {
        if (mSvList == nullptr)
        {
            cout << "Empty list!" << endl;
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

    /** 
     * @brief This function do print all elems in list
     * @param void return
     * @result list of elems
    **/
    void print()
    {
        cout << "\nPrint List: " << endl;
        if (mSvList == nullptr)
            return;
        nodeBlock *List = mSvList;
        while (List != nullptr)
        {
            cout << List->mStudent << endl;
            List = List->next;
        }
        cout << endl;
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
    // cout << "Test case for pop_back() with list has 1 elem " << endl;
    // svList2->push_front(sv2);
    // svList2->pop_back();
    // svList2->print();

    // cout << "***************************** AFTER SORTING ID BY BUBBLE SORT! **************************" << endl;
    // svList->bubbleSortID();
    // svList->print();


    // cout << "****************** Searching in Bianry Seaching Way of List in ordering! ****************" << endl;
    //  printf("Line: %d - Function: %s( binary searching )\n", __LINE__, __FUNCTION__);
    // svList->binarySearchID(324);
    // cout << endl;
    // printf("Line: %d - Function: %s( binary Searching )\n", __LINE__, __FUNCTION__);
    // svList->binarySearchID(13231);
    // cout << endl;


    // cout << "****************** Searching in Normal Way of List in ordering! ****************" << endl;
    // printf("Line: %d - Function: %s( Normal Searching )\n", __LINE__, __FUNCTION__);
    // svList->searchID(13231);
    // cout << endl;

    // cout << "name searching case: 1 found and 1 not found! " << endl;
    // printf("Line: %d - Function: %s( Normal Searching )\n", __LINE__, __FUNCTION__);
    // svList->searchName("John 3");
    // cout << endl;
    // svList->searchName("Trieu John 8");
    // cout << endl;


    cout << "****************** Inserting by elem which was entered ****************" << endl;
    svList->insert(sv12, 5);
    svList->print();


    cout << "****************** Removing by ID ****************" << endl;
    svList->remove(66);
    svList->print();


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