#include "exercise_manager.h"

uint32_t User::day_ = 0;


int main()
{
    ExerciseManager exercise("路径.txt");
    exercise.print_exercise_path("万向城");
    exercise.load_users("人物.txt");
    exercise.save_all_user("人物1.txt");
    exercise.save_exercise_path("路径1.txt", "万向城");
    return 0;
}