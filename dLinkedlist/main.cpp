#include <iostream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <string.h>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

bool checkName(const string &nameCheck);
class Student
{
    int _mID;
    string _mName;
    int _mAge;

public:
    Student() : _mName{""} {}
    Student(const int &_ID, const string &name, const int &age) : _mID{_ID}, _mName{name}, _mAge{age} {}
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

    void setAge(const int &age)
    {
        _mAge = age;
    }

    int getAge() const
    {
        return _mAge;
    }

    friend ostream &operator<<(ostream &os, const Student &sv)
    {
        os << "\tID : " << sv._mID << " ------- Name: " << sv._mName << " -------- Age: " << sv._mAge;
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "You have entered wrong input" << endl;
                cout << "ID_enter: ", is >> sv._mID;
            }
        }
        // clear buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);
        cout << "Name_enter: ", getline(is, sv._mName);
        while (!checkName(sv._mName))
        {
            cout << "You enter wrong input - Not include special characters!" << endl;
            cout << "Name_enter: ", getline(is, sv._mName, '\n');
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
        printf("Line: %d - Function: %s( Start destructor function)\n", __LINE__, __FUNCTION__);
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

    bool isIDExist(const int &_idFind) const
    {
        if (mSvList == nullptr)
        {
            return false;
        }
        nodeBlock *runList = mSvList;
        while (runList != nullptr)
        {
            if (runList->mStudent.getID() == _idFind)
            {
                runList = nullptr;
                return true;
            }
            runList = runList->next;
        }
        runList = nullptr;
        return false;
    }

    /**
     * @brief setMiddle - middle pointer point to the middle elem of list
     * @algorithm: 
     *          using 2 step:
     *          when (fast != nullptr)
     *              slow : goes 1 step
     *              fast : goes 2 steps
     *     
     *          
     * @result set Middle Pointer to the middle node of list
    */
    void setMiddle()
    {
        printf("Line: %d - Function: %s()\n", __LINE__, __FUNCTION__);

        if (mSvList == nullptr)
        {
            mMiddle = nullptr;
        }
        nodeBlock *slow = mSvList;
        nodeBlock *fast = mSvList->next;

        // handling finding middle node
        do
        {
            while (fast != mTail)
            {
                fast = fast->next;
                if (fast != mTail)
                {
                    slow = slow->next;
                    fast = fast->next;
                }
            }
        } while (0);

        mMiddle = slow;
    }

    /**
     * @brief push_back - insert elem at the back of list
     * @algorithm: 
     *          newNode->prev = tail
     *          tail->next = newNode
     *          tail = newNode
     * @result A list was added a node at back
    */
    void push_back(const Student &_student)
    {
        printf("Line: %d - Function: %s( const Student &_student )\n", __LINE__, __FUNCTION__);

        /**
         *  Handling with pushing an elem at back of list
         *     
         *      elem1 -> elem2 > elem3 -> elem4 -> nullptr
         *      ^                            ^
         *      |                            |
         *     head                        mTail 
         * 
         *  newElem = elem5 : elem5 -> next = nullptr
         *  
         *   mTail -> next = newElem
        */
        do
        {
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
        } while (0);
    }

    /**
     * @brief push_front - insert elem at the front of list
     * @algorithm: 
     *          newNode->prev = nullptr
     *          newNode->next = head
     *          head->prev = newNode
     *          head = newNode (make head point to the first elem of list)
     * @result A list was added a node at front
    */
    void push_front(const Student &_student)
    {
        printf("Line: %d - Function: %s( const Student& _student )\n", __LINE__, __FUNCTION__);

        // Handling with a front elem to push in
        do
        {
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

        } while (0);

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
                listSearching = nullptr;
                return;
            }
            listSearching = listSearching->next;
        }
        cout << "Not Found!" << endl;
        listSearching = nullptr;
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

        // Searching name
        do
        {
            nodeBlock *listSearching = mSvList;
            while (listSearching != nullptr)
            {
                if (listSearching->mStudent.getName() == nameFind)
                {
                    cout << "Found name: " << nameFind << " -- This below is Information: " << endl;
                    cout << listSearching->mStudent;
                    listSearching = nullptr;
                    return;
                }
                listSearching = listSearching->next;
            }
            cout << "Not Found!" << endl;
            listSearching = nullptr;
        } while (0);
    }

    /**
     * @brief binarySearchID - Using binary searching algorithm to search elem has ID value
     * @algorithm: 
     *          
     * @result Print infors if true - otherwise "Not Found"
    */
    void binarySearchID(const int &ID_find)
    {
        // List is Empty
        if (mSvList == nullptr)
        {
            return;
        }
        dLinkedList *runList = this;
        while (runList->mSvList != runList->mTail)
        {
            runList->setMiddle();
            if (runList->mMiddle->mStudent.getID() == ID_find)
            {
                cout << "Found! This below informations!" << endl;
                cout << runList->mMiddle->mStudent;
                return;
            }

            if (runList->mMiddle->mStudent.getID() < ID_find)
            {
                runList->mSvList = runList->mMiddle->next;
            }

            if (runList->mMiddle->mStudent.getID() > ID_find)
            {
                runList->mTail = runList->mMiddle;
            }
        }
        if (runList->mSvList == runList->mTail)
        {
            cout << "Found! This below informations!" << endl;
            cout << runList->mMiddle->mStudent;
            return;
        }
        else
        {
            cout << "Not Found!" << endl;
        }
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
        printf("Line: %d - Function: %s( Start Function )\n", __LINE__, __FUNCTION__);

        // Checking conditions
        do
        {
            if (_pos < 0 || _pos > _mSize)
            {
                throw runtime_error("Invalid!");
            }
            if (isIDExist(_student.getID()))
            {
                cout << "Already has this ID!" << endl
                     << endl;
                return;
            }
            if (_pos == _mSize)
            {
                push_back(_student);
                return;
            }
            if (_pos == 0)
            {
                push_front(_student);
                return;
            }
        } while (0);

        // Handling Inserting An Elem
        do
        {
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
        } while (0);

        printf("Line: %d - Function: %s( End of Function )\n", __LINE__, __FUNCTION__);
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
    void removeByID(const int &ID_find)
    {
        printf("Line: %d - Function: %s( Start Function )\n", __LINE__, __FUNCTION__);

        // checking for null
        if (mSvList == nullptr)
        {
            cout << "Not Found! Empty List!" << endl;
            return;
        }

        // Handling remove an elem in position of _pos
        do
        {
            nodeBlock *prevNode = nullptr;
            nodeBlock *curr = mSvList;
            while (curr != nullptr)
            {
                if (curr->mStudent.getID() == ID_find)
                {
                    // this for last node
                    if (curr->next == nullptr)
                    {
                        pop_back();
                        prevNode->next = nullptr;
                        curr = nullptr;
                        prevNode = nullptr;
                        return;
                    }
                    else
                    {
                        // for the first node
                        if (curr->prev == nullptr)
                        {
                            pop_front();
                            prevNode = nullptr;
                            curr = nullptr;
                            return;
                        }
                        else
                        {
                            // node is in middle
                            prevNode->next = curr->next;
                            curr->next->prev = prevNode;
                            delete curr;
                            curr = nullptr;
                            prevNode = nullptr;
                            _mSize--;
                            if (_mSize == 0)
                            {
                                mSvList = nullptr;
                            }
                            setMiddle();
                            printf("Line: %d - Function: %s( Out of Function )\n", __LINE__, __FUNCTION__);
                            return;
                        }
                    }
                }
                prevNode = curr;
                curr = curr->next;
            }
            cout << "Not Found!" << endl;
        } while (0);

        printf("Line: %d - Function: %s( End of Function )\n", __LINE__, __FUNCTION__);
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

            // Handling a last elem
            do
            {
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
            } while (0);
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

            // Handling with a first elem
            do
            {
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
            } while (0);
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
        List = nullptr;
        setMiddle();
    }

    /** 
     * @brief This function do print all elems in list
     * @param void return
     * @result list of elems
    **/
    void print()
    {
        cout << "Print List: " << endl;
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

/** 
 * @brief This function do checking special character in input Name
 * @param nameCheck input string_Name for checking
 * @result true if no special character - otherwise false
**/
bool checkName(const string &nameCheck)
{
    for (char c : nameCheck)
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

void exerciseFrom1To3()
{
    printf("\n\nLine: %d - Function: %s( ...............  )\n\n", __LINE__, __FUNCTION__);
    int _idFind;
    int _idRemove;
    string _nameFind;
    dLinkedList *svList = new dLinkedList;
    // make list of student
    Student sv1(23, "Trieu John 1", 23);
    Student sv2(234, "Trieu John 2", 18);
    Student sv3(235, "Trieu John 3", 19);
    Student sv4(66, "Trieu John 4", 20);
    Student sv5(13231, "Trieu John 5", 21);
    Student sv6(324, "Trieu John 6", 22);
    Student sv7(43, "Trieu John 7", 17);
    Student sv8(33532, "Trieu John 8", 24);
    Student sv9(3551, "Trieu John 9", 25);
    Student sv10(23432135, "Trieu John 10", 26);
    Student sv11(4132, "Trieu John 11", 23);

    /* Insert Data */
    do
    {
        svList->push_back(sv1);
        svList->push_back(sv2);
        svList->push_back(sv3);
        svList->push_back(sv4);
        svList->push_back(sv5);
        svList->push_back(sv6);
        svList->push_front(sv7);
        svList->push_back(sv8);
        svList->push_front(sv9);
        svList->push_back(sv10);
    } while (0);

    // Test case for sorting list with buble sort.
    do
    {
        cout << endl;
        cout << "*****************List after insert 10 datas*************" << endl;
        svList->print();
        cout << endl;
        cout << "*****************Using bubble-sort to sort elems by ID - increasingly*************" << endl;
        svList->bubbleSortID();
        svList->print();
        cout << endl;
    } while (0);

    // test cast for searching ID - in normal way and binary way
    do
    {
        cout << " ****************Search Student by ID***********" << endl;
        cout << "_idFind -- enter: ", cin >> _idFind;
        while (cin.fail())
        {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "You have entered wrong input" << endl;
                cout << "ID_enter: ", cin >> _idFind;
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;
        cout << " ****************Search Student by ID - in normal way***********" << endl;
        svList->searchID(_idFind);
        cout << endl;
        cout << "Binary Searching Way" << endl;
        svList->binarySearchID(_idFind);
        cout << endl;

    } while (0);

    //Test cases for searching name function
    do
    {
        cout << " ****************Search Student by ID - in normal way***********" << endl;
        cout << "_idName -- enter: ", getline(cin, _nameFind);
        while (!checkName(_nameFind))
        {
            cout << "You enter wrong input - Not include special characters!" << endl;
            cout << "Name_enter: ", getline(cin, _nameFind);
        }

        cout << "name for searching: " << _nameFind << endl;
        svList->searchName(_nameFind);
        cout << endl;
    } while (0);

    // Test cases for remove function
    do
    {
        cout << " ****************Remove Student by ID***********" << endl;
        cout << "_idRemove -- enter: ", cin >> _idRemove;
        while (cin.fail())
        {
            if (cin.fail())
            {
                // clear buffer
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "You have entered wrong input" << endl;
                cout << "ID_enter: ", cin >> _idRemove;
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        svList->removeByID(_idRemove);
        cout << endl;
        cout << "*****************After removing******************" << endl;
        svList->print();
    } while (0);

    // Test cases for insert function
    do
    {
        svList->insert(sv6, 2); // Already Exist
        svList->insert(sv11,5); // Work Fine!
        //svList->insert(sv8, 15); // runtime error -- turn off comment for checking
        cout << "*****************After Inserting******************" << endl;
        svList->print();
    } while (0);

    // Deallocated svList
    do
    {

        if (svList)
        {
            delete svList;
            svList = nullptr;
        }
    } while (0);
}

void Console()
{
    cout << "**********************CONSOLE MENU***********************" << endl;
    cout << "*               1. Checking Exercise 1 to 3             *" << endl;
    cout << "*               2. Checking Exercise 4                  *" << endl;
    cout << "*               0. Exist                                *" << endl;
    cout << "*********************************************************" << endl;
}

/** 
 * @brief       This function do searching first non negative number which is not in array
 * @algorithm:
 *              We recorgnize that the first non negative number which is missing never larger than size of array
 *              If worst case -> that woul be array.size() + 1
 *              So if arr[i] < 0 || arr[i] > arr.size() => set equal 1
 *              Using hasing sum to make boundary for this array
 *                  increase arr[(arr[i]- 1)% n] += n    | n is arr.size()
 *              Passing whole array - if first elem is <= arr.size() -> return index;
 *              otherwise, return arr.size() + 1;
 * @param arr   Vector input
 * @result      a smallest non-negative number
**/
int SearchMinNonNeg(std::vector<int> arr)
{
    int _size = arr.size();

    cout << "Input arr: ";
    for (int idex = 0; idex < _size; idex++)
    {
        cout << arr[idex] << " ";
    }
    cout << endl;

    bool miss_one = true;
    // checking 1! - because this is a smallest non negative number
    for (int idex = 0; idex < _size; idex++)
    {
        if (arr[idex] == 1)
        {
            miss_one = false;
            break;
        }
    }
    // if we have no number 1 in array -> return it
    if (miss_one)
        return 1;

    for (int idex = 0; idex < _size; idex++)
    {
        if (arr[idex] <= 0 || arr[idex] > _size)
        {
            arr[idex] = 1;
        }
    }

    for (int idex = 0; idex < _size; idex++)
    {
        arr[(arr[idex] - 1) % _size] += _size;
    }

    // checking for the first elem < size -> if so return index + 1
    for (int idex = 0; idex < _size; idex++)
    {
        if (arr[idex] <= _size)
        {
            return idex + 1;
        }
    }

    // if this array is sorting array and no missing
    // return size + 1
    return _size + 1;
}

void exercise4()
{
    // test cases:
    vector<int> arr{1, 2, 3, 4, 5, 8, 9, 0};
    vector<int> arr2{2, 3, 1, -2, -3, -1, -4};
    vector<int> arr3{1, 1000};

    // Fining non-negative number which is not in arrays
    do
    {
        cout << "Output res: " << SearchMinNonNeg(arr) << endl
             << endl;
        cout << "Output res: " << SearchMinNonNeg(arr2) << endl
             << endl;
        cout << "Output res: " << SearchMinNonNeg(arr3) << endl
             << endl;
    } while (0);
}

int main()
{
    int _choice{};

    // Console Program Running
    while (1)
    {
        system("cls");
        Console();

        // Enter choice
        do
        {
            cout << "Enter choice: ", cin >> _choice;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error type of in put! just number!" << endl;
                cout << "Enter choice again: ", cin >> _choice;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (0);

        // Handle choices
        do
        {
            switch (_choice)
            {
            case 1:
                system("cls");
                 exerciseFrom1To3();
                cout << endl
                    << "Take time for looking your output!" << endl;
                cout << "If you want to return to consol, Type anything to comeback console!" << endl;
                _getch();
                break;
            case 2:
                system("cls");
                exercise4();
                cout << endl
                    << "Take time for looking your output!" << endl;
                cout << "If you want to return to consol, Type anything to comeback console!" << endl;
                _getch();
                break;
            case 0:
                system("cls");
                cout << "Bye bye!!!!!!!";
                return 0;

            default:
                system("cls");
                cout << "What do you mean? We have no option for that one!" << endl;
                cout << "Type anything to comeback console!" << endl;
                _getch();
                break;
            }
        } while (0);

    }

    printf("Line: %d - Function: %s( End of Main! )\n", __LINE__, __FUNCTION__);
    return 0;
}