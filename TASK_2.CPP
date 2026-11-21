#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
const int NUM_SUBJECTS = 5;
const int MAX_MARKS = 100;
const int PASS_MARKS = 40;
const std::string FILE_NAME = "students.txt";
const std::string SUBJECTS[NUM_SUBJECTS] = {"Maths", "Physics", "Electronics", "OOP", "Signals"};
// ---------------------------------------------------------------------------
// Custom exception for invalid marks
// ---------------------------------------------------------------------------
class InvalidMarksException : public std::exception {
 std::string message;
public:
 InvalidMarksException(const std::string &subject, int marks) {
 message = "Invalid marks " + std::to_string(marks) + " in " + subject +
 " (must be between 0 and " + std::to_string(MAX_MARKS) + ")";
 }
 const char *what() const noexcept override { return message.c_str(); }
};
// ---------------------------------------------------------------------------
// Base class: Student
// ---------------------------------------------------------------------------
class Student {
protected:
 std::string usn;
 std::string name;
public:
 Student(const std::string &usn, const std::string &name) : usn(usn), name(name) {}
 virtual ~Student() = default;
 const std::string &getUsn() const { return usn; }
 const std::string &getName() const { return name; }
 // Virtual functions - overridden in Result
 virtual void displayDetails() const {
 std::cout << "USN : " << usn << "\nName: " << name << "\n";
 }
 virtual void displayMarks() const { std::cout << "No marks available.\n"; }
 virtual int getTotal() const { return 0; }
 virtual std::string toRecord() const { return usn + "|" + name; }
};
// ---------------------------------------------------------------------------
// Derived class: Result
// ---------------------------------------------------------------------------
class Result : public Student {
 std::vector<int> marks;
 int total;
 static void validate(const std::vector<int> &m) {
 for (size_t i = 0; i < m.size(); ++i) {
 if (m[i] < 0 || m[i] > MAX_MARKS) {
 throw InvalidMarksException(SUBJECTS[i], m[i]);
 }
 }
 }
public:
 Result(const std::string &usn, const std::string &name, const std::vector<int> &m)
 : Student(usn, name), marks(m), total(0) {
 validate(marks); // throws InvalidMarksException on bad input
 for (int x : marks) total += x;
 }
 double getPercentage() const { return total * 100.0 / (NUM_SUBJECTS * MAX_MARKS); }
 bool hasPassed() const {
 return std::all_of(marks.begin(), marks.end(), [](int m) { return m >= PASS_MARKS; });
 }
 std::string getGrade() const {
 if (!hasPassed()) return "F";
 double p = getPercentage();
 if (p >= 90) return "S";
 if (p >= 80) return "A";
 if (p >= 70) return "B";
 if (p >= 60) return "C";
 if (p >= 50) return "D";
 return "E";
 }
 int getTotal() const override { return total; }
 void displayDetails() const override {
 Student::displayDetails();
 std::cout << "Total Marks: " << total << " / " << NUM_SUBJECTS * MAX_MARKS
 << " Percentage: " << std::fixed << std::setprecision(2) << getPercentage()
 << "%\n";
 }
 void displayMarks() const override {
 for (int i = 0; i < NUM_SUBJECTS; ++i) {
 std::cout << " " << std::left << std::setw(12) << SUBJECTS[i] << ": " << marks[i]
 << "\n";
 }
 std::cout << "Result: " << (hasPassed() ? "PASS" : "FAIL") << " Grade: " << getGrade()
 << "\n";
 }
 std::string toRecord() const override {
 std::string rec = Student::toRecord();
 for (int m : marks) rec += "|" + std::to_string(m);
 return rec;
 }
};
