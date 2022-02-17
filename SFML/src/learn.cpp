#include <iostream>
using namespace std;
class Book{
     public:
          string name;
          Book(){}
};
class Employee{
     public:
          int Age;
          Book book;
          string Name ;
          Employee(int age , string name,Book somebook){
               Age = age;
               Name = name;
               book = somebook;
          }
          void introduce (){
               cout <<"name- "<< Name << endl;
               cout <<"age-" << Age << endl;
          }
};
