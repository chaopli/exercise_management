#include <string>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
using namespace std;
enum class Gender { female, male };

class User
{
private:
    string id_;
    string alias_;
    Gender gender_;
    uint32_t age_;
    vector<uint32_t> steps_;
    uint32_t continuous_days_;
    static uint32_t day_;
public:
    string output() const
    {
        ostringstream oss;
        oss << id_ << ',' << alias_ << ','
            << ((gender_ == Gender::female ? "女" : "男")) << ','
            << age_ << ',';
        for (auto step : steps_) 
        {
            oss << step << ',';
        }
        oss << continuous_days_;
        return oss.str();
    }
    User() :
        steps_(vector<uint32_t>(7)),
        continuous_days_(0)
    {}
    User(string id, string alias, Gender gender, uint32_t age) :
        id_(id),
        alias_(alias),
        gender_(gender),
        age_(age),
        steps_(vector<uint32_t>(7)),
        continuous_days_(0)
    {
    }
    User(string id, string alias, Gender gender, uint32_t age, vector<uint32_t> steps, int continuous_days) :
        id_(id),
        alias_(alias),
        gender_(gender),
        age_(age),
        steps_(steps),
        continuous_days_(continuous_days)
    {
    }
    User(const User& other) :
        id_(other.id_),
        alias_(other.alias_),
        gender_(other.gender_),
        age_(other.age_),
        steps_(other.steps_),
        continuous_days_(other.continuous_days_)
    {
    }
    User(User&& other) :
        id_(other.id_),
        alias_(other.alias_),
        gender_(other.gender_),
        age_(other.age_),
        steps_(other.steps_),
        continuous_days_(other.continuous_days_)
    {}
    string id() { return id_; }
    string alias() { return alias_; }
    uint32_t day() { return day_; }
    bool operator==(const User &other)
    {
        return id_ == other.id_;
    }
    User& operator= ( User&& other) {
        id_ = other.id_;
        alias_ = other.alias_;
        gender_ = other.gender_;
        age_ = other.age_;
        steps_ = other.steps_;
        continuous_days_ = other.continuous_days_;
        return *this;
    }
    uint32_t weekly_steps() { return accumulate(steps_.begin(), steps_.end(), 0);}
    uint32_t continuous_days() { return continuous_days_; }
    uint32_t today_step() { return steps_[day_ % 7];}
    friend ostream& operator<< (ostream& os, const User& user);
};

ostream& operator<<(ostream& os, const User& user)
{
    return os << user.output();
}

class UserHash
{
public:
    size_t operator() (User user)
    {
        return stoi(user.id());
    }
};

class UserManager
{
private:
    vector<User> users_;
public:
    void add_user(User user) {
        for (auto it = users_.begin(); it != users_.end(); it++)
        {
            if (it->id() == user.id()) {
                cout << "用户已存在" << endl;
                return;
            }
        }
        users_.push_back(user);
    }
    void delete_user_by_id(string id) {
        for (auto it = users_.begin(); it != users_.end();)
        {
            if (it->id().compare(id) == 0) {
                users_.erase(it);
            } else {
                it++;
            }
        }
    }
    void delete_user_by_name(string name) {
        for (auto it = users_.begin(); it != users_.end();)
        {
            if (it->alias().compare(name) == 0) {
                users_.erase(it);
            } else {
                it++;
            }
        }
    }
    User& get_user_by_id(string id) {
        for (auto it = users_.begin(); it != users_.end(); it++)
        {
            if (it->id() == id) {
                return *it;
            }
        }
        cout << "用户不存在" << endl;
        return *users_.begin();
    }
    User& get_user_by_name(string name) {
        for (auto it = users_.begin(); it != users_.end(); it++)
        {
            if (it->alias() == name) {
                return *it;
            }
        }
        cout << "用户不存在" << endl;
        return *users_.begin();

    }
    void update_user_by_id(string id) {
        User& user = get_user_by_id(id);
    }
    void update_user_by_name(string name) {
        User& user = get_user_by_name(name);
    }
    vector<User> users() { 
        return users_;
    }
};