#include <iostream>
#include <memory> //Used for smart pointers

//auto_ptr deprecated in C++11 and removed in C++17

/* Memory header provides a number of smart/managed pointers:
 * shared_ptr
 * unique_ptr
 * weak_ptr (weak shared ptr)
 */


struct Test{
    Test()
    :pid(++ID){
        std::cout << pid << ". Created" << std::endl;
    }
    ~Test(){
        std::cout << pid << ". Destroyed" << std::endl;
    }

    static int ID;
    const int pid;
};



int Test::ID = 0;
int main() {

    //DONT FORGET: YOU CAN USE SMART POINTERS AS FUNCTION RETURN TYPES TOO!

    ////UNIQUE_PTR////
    std::cout << "\nUnique Pointers:" << std::endl;
    //Extra curly braces to control scope of vars
    {
        //Can only be one unique_ptr (hence the name) pointing at any one resource, as when it goes out of scope, it immediately deletes the object and deallocates the memory
        //Can also use std::make_unique<Class>({Ctor Args})
        std::unique_ptr<Test> pTest1(new Test);

#if 0 //Although a unique_ptr cannot be copied, it can be moved using std::move
        std::unique_ptr<Test> pTest2(new Test);
#endif

        std::unique_ptr<Test> pTest2 = std::move(pTest1); //Now pTest1 is nullptr
    }



    ////SHARED_PTR////
    std::cout << "\nShared Pointers:" << std::endl;
    //In contrast, there can be multiple shared_ptrs pointing at a single resource. When all shared_ptrs pointing at a single resource go out of scope, then the object is deleted and memory is deallocated
    //Be careful not to introduced a reference cycles here (where you point a shared pointer at another, and it points back). This will mess up the reference counting.
    auto pTest3 = std::make_shared<Test>(); //{Ctor Args} list go in ()
    {
        std::shared_ptr<Test> pTest4 = pTest3;
    }

    //NOTE: A weak_ptr is like a shared pointer, but it doesn't increase the reference count. Using a weak_ptr, you can tell whether a shared pointer has deallocated resources or not
    std::weak_ptr<Test> sPtrPtr (pTest3);
    if(sPtrPtr.lock() == nullptr){ //Lock returns nullptr if resources are deallocated. This differs from normal pointer which would point to same place, giving undefined behaviour
        std::cout << "Weak is expired" << std::endl;
    }else{
        std::cout << "Weak not expired... Points to: " << sPtrPtr.lock()->pid << std::endl;
    }


    //Dtor only called when both shared_ptrs go out of scope
    std::cout << "Finished main()" << std::endl;


    ////YOU CAN ALSO USE POINTER CASTS (Only on shared pointers as this would create shared ownership)
    //e.g. std::static_pointer_cast<DerivedClass> x(baseObject) <--- object slicing will occur        Can also use dynamic or const cast

    return 0;
}