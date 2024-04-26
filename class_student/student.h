#ifndef TELECOM_FINAL_CLASS_STUDENT_STUDENT_H_
#define TELECOM_FINAL_CLASS_STUDENT_STUDENT_H_
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

struct Student {
  int64_t id;
  std::string name;
  int64_t age;
};

class DataBaseStudent {
 private:
  std::unordered_map<int64_t, std::shared_ptr<Student>> all_students_;
 public:
  DataBaseStudent() = default;
  void AddStudent(const Student& student) {
    std::shared_ptr<Student> newStudent = std::make_shared<Student>(student);
    all_students_[newStudent->id] = newStudent;
  }

  void RemoveStudent(int64_t id) {
    all_students_.erase(id);
  }

  std::shared_ptr<Student> GetStudent(int64_t id) {
    if (all_students_.find(id) != all_students_.end()) {
      return all_students_[id];
    }
    return nullptr;
  }
};

#endif
