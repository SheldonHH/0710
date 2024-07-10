import java.util.ArrayList;
import java.util.List;

class Student {
    private int id;
    private String name;

    public Student(int id, String name) {
        this.id = id;
        this.name = name;
        System.out.println("Student(ID: " + id + ", Name: " + name + ") constructed");
    }

    @Override
    protected void finalize() throws Throwable {
        System.out.println("Student(ID: " + id + ", Name: " + name + ") destroyed");
        super.finalize();
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }
}

public class Main {
    public static void main(String[] args) {
        List<Student> students = new ArrayList<>();

        // 向列表中添加学生对象
        students.add(new Student(101, "Alice"));
        students.add(new Student(102, "Bob"));
        students.add(new Student(103, "Charlie"));

        // 输出所有学生的ID和姓名
        System.out.println("学生列表:");
        for (Student student : students) {
            System.out.println("ID: " + student.getId() + ", Name: " + student.getName());
        }
    }
}
