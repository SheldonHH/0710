#include <iostream>
#include <string>

class Employee {
public:
    virtual ~Employee() = default;
    virtual std::string getRole() const {
        return "Employee";
    }
    virtual void displayInfo() const {
        std::cout << "Role: " << getRole() << std::endl;
    }
};

class Manager : public Employee {
public:
    std::string getRole() const override {
        return "Manager";
    }
    void displayInfo() const override {
        std::cout << "Role: " << getRole() << " (Manager specific info)" << std::endl;
    }
};

class Developer : public Employee {
public:
    std::string getRole() const override {
        return "Developer";
    }
    void displayInfo() const override {
        std::cout << "Role: " << getRole() << " (Developer specific info)" << std::endl;
    }
};

int main() {
    Employee e;
    Manager m;
    Developer d;

    e.displayInfo();
    m.displayInfo();
    d.displayInfo();

    return 0;
}
