// Copyright 2021 Stoicheva Darya
#include <gtest/gtest.h>
#include <tbb/tick_count.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/stoicheva_d_dijkstra_tbb/dijkstra_tbb.h"


std::vector<int> create_random_graph(const size_t points,
                                     const size_t max_distance) {
    size_t size = points * points;
    std::mt19937 gen(uint32_t(time(0)));
    std::uniform_int_distribution<int> uid(-static_cast<int>(max_distance*10),
                                            static_cast<int>(max_distance));
    std::vector<int> matrix(size);

    for (size_t i = 0; i < points; i++) {
        for (size_t j = 0; j < points; j++) {
            if (i == j) {
                matrix[i * points + j] = 0;
            } else {
                int distance = uid(gen);
                if (distance <= 0) {
                    distance = -1;
                }
                matrix[i * points + j] = distance;
                matrix[i + j * points] = distance;
            }
        }
    }
    return matrix;
}

TEST(Tbb, Test_Time) {
    const size_t root = 1;
    const size_t pointsCount = 5000;
    tbb::tick_count time_start, time_end;

    std::vector<int> graph = create_random_graph(pointsCount, 100);
    // print_graph(g raph, pointsCount);

    time_start = tbb::tick_count::now();
    auto distances_seq = dijkstra(graph, root, 4800);
    time_end = tbb::tick_count::now();
    std::cout << "Sequential Time: " << (time_end - time_start).seconds() << std::endl;

    time_start = tbb::tick_count::now();
    auto distances_parallel = dijkstra_tbb(graph, root, 4800);
    time_end = tbb::tick_count::now();
    std::cout << "Parallel Time: " << (time_end - time_start).seconds() << std::endl;
    ASSERT_EQ(distances_seq, distances_parallel);
}

TEST(Tbb, Test_EmptyGraph) {
    std::vector<int> graph = {};

    ASSERT_ANY_THROW(dijkstra_tbb(graph, 0, 0));
}

TEST(Tbb, Test_Graph1) {
    std::vector<int> graph = { 0 };
    std::vector<int> result = { 1 };

    ASSERT_EQ(result, dijkstra_tbb(graph, 1, 1));
}

TEST(Tbb, Test_Graph2) {
    std::vector<int> graph = { 0, 7,
                               7, 0 };
    std::vector<int> result = { 1, 2 };

    ASSERT_EQ(result, dijkstra_tbb(graph, 1, 2));
}

TEST(Tbb, Test_Graph6) {
    std::vector<int> graph = { 0, 7, 9, -1, -1, 14,
                              7, 0, 10, 15, -1, -1,
                              9, 10, 0, 11, -1, 2,
                              -1, 15, 11, 0, 6, -1,
                              -1, -1, -1, 6, 0, 9,
                               14, -1, 2, -1, 9, 0 };
    std::vector<int> result = { 1, 3, 6 };

    print_graph(graph, 6);

    ASSERT_EQ(result, dijkstra_tbb(graph, 1, 6));
}

TEST(Tbb, Test_Graph6a) {
    std::vector<int> graph = { 0, 1, 4, -1, 2, -1,
                              1, 0, -1, 9, -1, -1,
                              4, -1, 0, 7, -1, -1,
                              -1, 9, 7, 0, -1, 2,
                              2, -1, -1, -1, 0, 8,
                              -1, -1, -1, 2, 8, 0 };
    std::vector<int> result = { 1, 5, 6 };

    ASSERT_EQ(result, dijkstra_tbb(graph, 1, 6));
}

TEST(Sequential, Test_EmptyGraph) {
    std::vector<int> graph = {};

    ASSERT_ANY_THROW(dijkstra(graph, 0, 0));
}

TEST(Sequential, Test_Graph1) {
    std::vector<int> graph = {0};
    std::vector<int> result = {1};

    ASSERT_EQ(result, dijkstra(graph, 1, 1));
}

TEST(Sequential, Test_Graph2) {
    std::vector<int> graph = { 0, 7,
                               7, 0 };
    std::vector<int> result = {1, 2};

    ASSERT_EQ(result, dijkstra(graph, 1, 2));
}

TEST(Sequential, Test_Graph6) {
    std::vector<int> graph = { 0, 7, 9, -1, -1, 14,
                              7, 0, 10, 15, -1, -1,
                              9, 10, 0, 11, -1, 2,
                              -1, 15, 11, 0, 6, -1,
                              -1, -1, -1, 6, 0, 9,
                               14, -1, 2, -1, 9, 0 };
    std::vector<int> result = { 1, 3, 6 };

    print_graph(graph, 6);

    ASSERT_EQ(result, dijkstra(graph, 1, 6));
}

TEST(Sequential, Test_Graph6a) {
    std::vector<int> graph = {
                               0,  1,  4, -1,  2, -1,
                               1,  0, -1,  9, -1, -1,
                               4, -1,  0,  7, -1, -1,
                              -1,  9,  7,  0, -1,  2,
                               2, -1, -1, -1,  0,  8,
                              -1, -1, -1,  2,  8,  0 };
    std::vector<int> result = { 1, 5, 6 };

    print_graph(graph, 6);

    ASSERT_EQ(result, dijkstra(graph, 1, 6));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}