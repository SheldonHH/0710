#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

class Student {
public:
    Student(int id, const string& name) : id(id), name(name) {
        cout << "Student(ID: " << id << ", Name: " << name << ") constructed\n";
    }
    ~Student() {
        cout << "Student(ID: " << id << ", Name: " << name << ") destroyed\n";
    }
    int getId() const { return id; }
    string getName() const { return name; }
private:
    int id;
    string name;
};

int main() {
    vector<shared_ptr<Student>> students;
    
    // 向容器中添加学生对象
    students.push_back(make_shared<Student>(101, "Alice"));
    students.push_back(make_shared<Student>(102, "Bob"));
    students.push_back(make_shared<Student>(103, "Charlie"));

    // 输出所有学生的ID和姓名
    cout << "学生列表:\n";
    for (const auto& student : students) {
        cout << "ID: " << student->getId() << ", Name: " << student->getName() << "\n";
    }

    return 0;
}
