// Copyright 2021 Kuklin Andrey
#include <gtest/gtest.h>
#include "./linear_topology.h"
#include <gtest-mpi-listener.hpp>

TEST(Parrallel_Operations_MPI, can_create_linear_topology) {
  ASSERT_NO_THROW(GetlinearTopologyComm(MPI_COMM_WORLD));
}

TEST(Parrallel_Operations_MPI, can_send_data_in_linear_comm) {
  MPI_Comm lincom = GetlinearTopologyComm(MPI_COMM_WORLD);
  int procSize, procRank;
  MPI_Comm_size(lincom, &procSize);
  MPI_Comm_rank(lincom, &procRank);

  if (procSize == 1) {
    ASSERT_TRUE(true);
  } else {
    if (procRank == 0) {
      int dataMessage = 10;
      MPI_Send(&dataMessage, 1, MPI_INT, procSize - 1, 0, lincom);
    }
    if (procRank == procSize - 1) {
      int dataBuff;
      MPI_Status stat;
      MPI_Recv(&dataBuff, 1, MPI_INT, 0, 0, lincom, &stat);

      ASSERT_EQ(dataBuff, 10);
    }
  }
}

TEST(Parrallel_Operations_MPI,
     count_neighbors_of_first_proc_in_linear_comm_is_correct) {
  MPI_Comm lincom = GetlinearTopologyComm(MPI_COMM_WORLD);
  int procSize;
  MPI_Comm_size(lincom, &procSize);

  if (procSize == 1) {
    ASSERT_TRUE(true);
  } else {
    int neigh_count;
    MPI_Graph_neighbors_count(lincom, 0, &neigh_count);

    ASSERT_EQ(neigh_count, 1);
  }
}

TEST(Parrallel_Operations_MPI,
     count_neighbors_of_last_proc_in_linear_comm_is_correct) {
  MPI_Comm lincom = GetlinearTopologyComm(MPI_COMM_WORLD);
  int procSize;
  MPI_Comm_size(lincom, &procSize);

  if (procSize == 1) {
    ASSERT_EQ(1, 1);
  } else {
    int neigh_count;
    MPI_Graph_neighbors_count(lincom, procSize - 1, &neigh_count);

    ASSERT_EQ(neigh_count, 1);
  }
}

TEST(Parrallel_Operations_MPI,
     count_neighbors_of_middle_proc_in_linear_comm_is_correct) {
  MPI_Comm lincom = GetlinearTopologyComm(MPI_COMM_WORLD);
  int procSize;
  MPI_Comm_size(lincom, &procSize);

  if (procSize > 2) {
    int neigh_count;
    MPI_Graph_neighbors_count(lincom, 2, &neigh_count);

    ASSERT_EQ(neigh_count, 2);
  } else ASSERT_EQ(1, 1);
}

TEST(Parrallel_Operations_MPI,
     test) {
    ASSERT_TRUE(true);
}

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
