// Copyright 2021 Kuklin Andrey
#include <gtest/gtest.h>
#include "./linear_topology.h"
#include <gtest-mpi-listener.hpp>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listener =
      ::testing::UnitTest::GetInstance()->listeners();

  listener.Release(listener.default_result_printer());
  listener.Release(listener.default_xml_generator());

  listener.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}