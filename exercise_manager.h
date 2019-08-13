#include "user.h"
#include "graph.h"
#include <algorithm>

class ExerciseManager
{
public:
    // 创建 Exercise Manager的时候需要初始化地图文件路径
    ExerciseManager(string filename) :
        graph_(filename)
    {
    }
    void add_users_step();
    void update_user_step();
    void save_all_user(string filename)
    {
        ofstream fout(filename);
        for (auto user : user_mgr_.users())
        {
            fout << user << endl;
        }
    }
    void load_users(string filename)
    {
        ifstream fin(filename);
        while (fin.good())
        {
            string id;
            getline(fin, id, ',');
            string alias;
            getline(fin, alias, ',');
            string gender_str;
            getline(fin, gender_str, ',');
            Gender gender = gender_str.compare("女") == 0 ? Gender::female : Gender::male;
            int age;
            char ch;
            fin >> age >> ch;
            vector<uint32_t> steps(7);
            for (int i = 0; i < 7; i++) 
            {
                fin >> steps[i] >> ch;
            }
            int continuous_days;
            fin >> continuous_days;
            User user(id, alias, gender, age, steps, continuous_days);
            user_mgr_.add_user(user);
        }
    }
    vector<User> today_rank() {
        vector<User> ret(user_mgr_.users());
        sort(ret.begin(), ret.end(),
             [](User u1, User u2) {
                 return u1.today_step() > u2.today_step();
             });
        return ret;
    }
    vector<User> continuous_day_rank() {
        vector<User> ret(user_mgr_.users());
        sort(ret.begin(), ret.end(),
             [](User u1, User u2) {
                 return u1.continuous_days() > u2.continuous_days();
             });
        return ret;
    }
    vector<User> week_rank() { 
        vector<User> ret(user_mgr_.users());
        sort(ret.begin(), ret.end(),
             [](User u1, User u2) {
                 return u1.weekly_steps()/7 > u2.weekly_steps()/7;
             });
        return ret;
    }
    void print_exercise_path(string start) 
    {
        vector<string> paths = graph_.shortest_paths(start);
        for (auto path : paths)
        {
            cout << path << endl;
        }
    }
    void save_exercise_path(string filename, string start) 
    {
        ofstream fout(filename);
        vector<string> paths = graph_.shortest_paths(start);
        for (auto path : paths)
        {
            fout << path << endl;
        }
    }
private:
    UserManager user_mgr_;
    EdgeWeightedGraph<float> graph_;
};