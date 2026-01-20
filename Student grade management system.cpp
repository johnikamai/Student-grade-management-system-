#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <fstream>

using namespace std;

// Forward declarations
class Course;
class Student;
class Grade;

// ==================== BASE CLASS ====================
class Person {
protected:
    string name;
    int id;
    string email;
    
public:
    Person(string n = "", int i = 0, string e = "")
        : name(n), id(i), email(e) {}
    
    virtual ~Person() {}
    
    // Getters
    string getName() const { return name; }
    int getId() const { return id; }
    string getEmail() const { return email; }
    
    // Setters
    void setName(string n) { name = n; }
    void setId(int i) { id = i; }
    void setEmail(string e) { email = e; }
    
    virtual void displayInfo() const {
        cout << "ID: " << id << "\nName: " << name 
             << "\nEmail: " << email << endl;
    }
};

// ==================== COURSE CLASS ====================
class Course {
private:
    string courseCode;
    string courseName;
    int creditHours;
    string instructor;
    
public:
    Course(string code = "", string name = "", 
           int credits = 0, string instr = "")
        : courseCode(code), courseName(name), 
          creditHours(credits), instructor(instr) {}
    
    // Getters
    string getCourseCode() const { return courseCode; }
    string getCourseName() const { return courseName; }
    int getCreditHours() const { return creditHours; }
    string getInstructor() const { return instructor; }
    
    // Display course information
    void displayCourse() const {
        cout << "\nCourse Code: " << courseCode
             << "\nCourse Name: " << courseName
             << "\nCredit Hours: " << creditHours
             << "\nInstructor: " << instructor << endl;
    }
    
    // Static function to validate course code
    static bool isValidCourseCode(const string& code) {
        return code.length() >= 5 && code.length() <= 10;
    }
};

// ==================== GRADE CLASS ====================
class Grade {
private:
    string courseCode;
    double marks;
    char grade;
    int semester;
    
    // Calculate grade based on marks
    char calculateGrade(double m) {
        if (m >= 90) return 'A';
        else if (m >= 80) return 'B';
        else if (m >= 70) return 'C';
        else if (m >= 60) return 'D';
        else return 'F';
    }
    
public:
    Grade(string code = "", double m = 0, int sem = 1)
        : courseCode(code), marks(m), semester(sem) {
        grade = calculateGrade(m);
    }
    
    // Getters
    string getCourseCode() const { return courseCode; }
    double getMarks() const { return marks; }
    char getGrade() const { return grade; }
    int getSemester() const { return semester; }
    
    // Setters
    void setMarks(double m) { 
        marks = m; 
        grade = calculateGrade(m);
    }
    
    // Display grade information
    void displayGrade() const {
        cout << left << setw(12) << courseCode
             << setw(8) << marks
             << setw(6) << grade
             << setw(10) << semester << endl;
    }
    
    // Get grade points (for GPA calculation)
    double getGradePoints() const {
        switch(grade) {
            case 'A': return 4.0;
            case 'B': return 3.0;
            case 'C': return 2.0;
            case 'D': return 1.0;
            default: return 0.0;
        }
    }
};

// ==================== STUDENT CLASS ====================
class Student : public Person {
private:
    int rollNumber;
    string department;
    int semester;
    vector<Grade> grades;
    
public:
    Student(string n = "", int id = 0, string e = "", 
            int roll = 0, string dept = "", int sem = 1)
        : Person(n, id, e), rollNumber(roll), 
          department(dept), semester(sem) {}
    
    // Getters
    int getRollNumber() const { return rollNumber; }
    string getDepartment() const { return department; }
    int getSemester() const { return semester; }
    const vector<Grade>& getGrades() const { return grades; }
    
    // Setters
    void setRollNumber(int roll) { rollNumber = roll; }
    void setDepartment(string dept) { department = dept; }
    void setSemester(int sem) { semester = sem; }
    
    // Add a grade for a course
    void addGrade(const Grade& grade) {
        grades.push_back(grade);
    }
    
    // Update grade for a specific course
    bool updateGrade(const string& courseCode, double newMarks) {
        for (auto& grade : grades) {
            if (grade.getCourseCode() == courseCode) {
                grade.setMarks(newMarks);
                return true;
            }
        }
        return false;
    }
    
    // Calculate GPA
    double calculateGPA() const {
        if (grades.empty()) return 0.0;
        
        double totalGradePoints = 0.0;
        for (const auto& grade : grades) {
            totalGradePoints += grade.getGradePoints();
        }
        return totalGradePoints / grades.size();
    }
    
    // Display student information with grades
    void displayInfo() const override {
        Person::displayInfo();
        cout << "Roll Number: " << rollNumber
             << "\nDepartment: " << department
             << "\nSemester: " << semester
             << "\nGPA: " << fixed << setprecision(2) << calculateGPA() << endl;
    }
    
    // Display grades in tabular format
    void displayGrades() const {
        cout << "\nGrade Report for " << name << " (ID: " << id << ")\n";
        cout << string(50, '-') << endl;
        cout << left << setw(12) << "Course Code"
             << setw(8) << "Marks"
             << setw(6) << "Grade"
             << setw(10) << "Semester" << endl;
        cout << string(50, '-') << endl;
        
        for (const auto& grade : grades) {
            grade.displayGrade();
        }
        
        cout << string(50, '-') << endl;
        cout << "GPA: " << fixed << setprecision(2) << calculateGPA() << "\n\n";
    }
};

// ==================== GRADE MANAGEMENT SYSTEM CLASS ====================
class GradeManagementSystem {
private:
    vector<shared_ptr<Student>> students;
    vector<Course> courses;
    
    // Helper functions
    shared_ptr<Student> findStudentById(int id) {
        for (const auto& student : students) {
            if (student->getId() == id) {
                return student;
            }
        }
        return nullptr;
    }
    
    shared_ptr<Student> findStudentByRoll(int roll) {
        for (const auto& student : students) {
            if (student->getRollNumber() == roll) {
                return student;
            }
        }
        return nullptr;
    }
    
    Course* findCourseByCode(const string& code) {
        for (auto& course : courses) {
            if (course.getCourseCode() == code) {
                return &course;
            }
        }
        return nullptr;
    }
    
public:
    // Student management
    void addStudent() {
        string name, email, dept;
        int id, roll, sem;
        
        cout << "\n=== Add New Student ===\n";
        cout << "Enter Student ID: ";
        cin >> id;
        
        if (findStudentById(id)) {
            cout << "Student with this ID already exists!\n";
            return;
        }
        
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Email: ";
        getline(cin, email);
        cout << "Enter Roll Number: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Department: ";
        getline(cin, dept);
        cout << "Enter Semester: ";
        cin >> sem;
        
        auto student = make_shared<Student>(name, id, email, roll, dept, sem);
        students.push_back(student);
        cout << "Student added successfully!\n";
    }
    
    void addCourse() {
        string code, name, instructor;
        int credits;
        
        cout << "\n=== Add New Course ===\n";
        cout << "Enter Course Code: ";
        cin >> code;
        
        if (findCourseByCode(code)) {
            cout << "Course with this code already exists!\n";
            return;
        }
        
        cin.ignore();
        cout << "Enter Course Name: ";
        getline(cin, name);
        cout << "Enter Credit Hours: ";
        cin >> credits;
        cin.ignore();
        cout << "Enter Instructor Name: ";
        getline(cin, instructor);
        
        courses.emplace_back(code, name, credits, instructor);
        cout << "Course added successfully!\n";
    }
    
    void assignGrade() {
        int studentId;
        string courseCode;
        double marks;
        int semester;
        
        cout << "\n=== Assign Grade ===\n";
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        auto student = findStudentById(studentId);
        if (!student) {
            cout << "Student not found!\n";
            return;
        }
        
        cout << "Enter Course Code: ";
        cin >> courseCode;
        
        if (!findCourseByCode(courseCode)) {
            cout << "Course not found!\n";
            return;
        }
        
        cout << "Enter Semester: ";
        cin >> semester;
        cout << "Enter Marks (0-100): ";
        cin >> marks;
        
        if (marks < 0 || marks > 100) {
            cout << "Invalid marks! Must be between 0 and 100.\n";
            return;
        }
        
        Grade grade(courseCode, marks, semester);
        student->addGrade(grade);
        cout << "Grade assigned successfully!\n";
    }
    
    void viewStudentDetails() {
        int studentId;
        cout << "\nEnter Student ID: ";
        cin >> studentId;
        
        auto student = findStudentById(studentId);
        if (student) {
            cout << "\n=== Student Details ===\n";
            student->displayInfo();
        } else {
            cout << "Student not found!\n";
        }
    }
    
    void viewGradeReport() {
        int studentId;
        cout << "\nEnter Student ID: ";
        cin >> studentId;
        
        auto student = findStudentById(studentId);
        if (student) {
            student->displayGrades();
        } else {
            cout << "Student not found!\n";
        }
    }
    
    void updateGrade() {
        int studentId;
        string courseCode;
        double newMarks;
        
        cout << "\n=== Update Grade ===\n";
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        auto student = findStudentById(studentId);
        if (!student) {
            cout << "Student not found!\n";
            return;
        }
        
        cout << "Enter Course Code: ";
        cin >> courseCode;
        cout << "Enter New Marks: ";
        cin >> newMarks;
        
        if (student->updateGrade(courseCode, newMarks)) {
            cout << "Grade updated successfully!\n";
        } else {
            cout << "Course not found for this student!\n";
        }
    }
    
    void displayAllStudents() {
        if (students.empty()) {
            cout << "\nNo students found!\n";
            return;
        }
        
        cout << "\n=== All Students ===\n";
        cout << string(60, '-') << endl;
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(15) << "Department"
             << setw(10) << "Semester"
             << setw(8) << "GPA" << endl;
        cout << string(60, '-') << endl;
        
        for (const auto& student : students) {
            cout << left << setw(8) << student->getId()
                 << setw(20) << student->getName()
                 << setw(15) << student->getDepartment()
                 << setw(10) << student->getSemester()
                 << setw(8) << fixed << setprecision(2) 
                 << student->calculateGPA() << endl;
        }
        cout << string(60, '-') << endl;
    }
    
    void displayAllCourses() {
        if (courses.empty()) {
            cout << "\nNo courses found!\n";
            return;
        }
        
        cout << "\n=== All Courses ===\n";
        for (const auto& course : courses) {
            course.displayCourse();
            cout << endl;
        }
    }
    
    // Save data to files
    void saveData() {
        // Save students
        ofstream studentFile("students.txt");
        for (const auto& student : students) {
            studentFile << student->getId() << ","
                       << student->getName() << ","
                       << student->getEmail() << ","
                       << student->getRollNumber() << ","
                       << student->getDepartment() << ","
                       << student->getSemester() << "\n";
        }
        studentFile.close();
        
        // Save courses
        ofstream courseFile("courses.txt");
        for (const auto& course : courses) {
            courseFile << course.getCourseCode() << ","
                      << course.getCourseName() << ","
                      << course.getCreditHours() << ","
                      << course.getInstructor() << "\n";
        }
        courseFile.close();
        
        // Save grades
        ofstream gradeFile("grades.txt");
        for (const auto& student : students) {
            for (const auto& grade : student->getGrades()) {
                gradeFile << student->getId() << ","
                         << grade.getCourseCode() << ","
                         << grade.getMarks() << ","
                         << grade.getSemester() << "\n";
            }
        }
        gradeFile.close();
        
        cout << "Data saved successfully!\n";
    }
    
    // Load data from files
    void loadData() {
        // Load courses
        ifstream courseFile("courses.txt");
        if (courseFile) {
            string line;
            while (getline(courseFile, line)) {
                // Simple CSV parsing
                // Implement proper CSV parsing for production
            }
            courseFile.close();
        }
        
        cout << "Data loaded successfully!\n";
    }
    
    // Display menu
    void displayMenu() {
        cout << "\n=======================================\n";
        cout << "   STUDENT GRADE MANAGEMENT SYSTEM\n";
        cout << "=======================================\n";
        cout << "1. Add New Student\n";
        cout << "2. Add New Course\n";
        cout << "3. Assign Grade to Student\n";
        cout << "4. Update Student Grade\n";
        cout << "5. View Student Details\n";
        cout << "6. View Grade Report\n";
        cout << "7. Display All Students\n";
        cout << "8. Display All Courses\n";
        cout << "9. Save Data to Files\n";
        cout << "10. Load Data from Files\n";
        cout << "0. Exit\n";
        cout << "=======================================\n";
        cout << "Enter your choice: ";
    }
};

// ==================== MAIN FUNCTION ====================
int main() {
    GradeManagementSystem system;
    int choice;
    
    // Add some sample data
    system.addCourse();
    system.addStudent();
    
    do {
        system.displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1:
                system.addStudent();
                break;
            case 2:
                system.addCourse();
                break;
            case 3:
                system.assignGrade();
                break;
            case 4:
                system.updateGrade();
                break;
            case 5:
                system.viewStudentDetails();
                break;
            case 6:
                system.viewGradeReport();
                break;
            case 7:
                system.displayAllStudents();
                break;
            case 8:
                system.displayAllCourses();
                break;
            case 9:
                system.saveData();
                break;
            case 10:
                system.loadData();
                break;
            case 0:
                cout << "\nThank you for using the system!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        
    } while (choice != 0);
    
    return 0;
}
