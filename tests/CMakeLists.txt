add_executable(test_score_dice test_score_dice.cpp ../src/ScoreDice.cpp)
target_include_directories(test_score_dice PRIVATE ../src)
add_test(NAME ScoreDice COMMAND test_score_dice)
