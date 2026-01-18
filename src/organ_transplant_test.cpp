#include "gtest/gtest.h"
#include "organ_transplant.h"
#include <thread>
#include <vector>

using namespace ass1;

// Note to reviewer: These tests were generated with the help of a LLM.

TEST(OrganTransplantTest, AddPatient_SinglePatient) {
    OrganTransplantWaitingList list;
    list.addPatient("John Smith");
    
    auto waiting = list.getWaitingPatients();
    EXPECT_EQ(waiting.size(), 1);
    EXPECT_EQ(waiting[0], "John Smith");
}

TEST(OrganTransplantTest, TreatPatient_MovesFromWaitingToTreated) {
    OrganTransplantWaitingList list;
    list.addPatient("Jane Doe");
    
    list.treatPatient("Jane Doe", Date(2025, 1, 15));
    auto treated = list.getTreatedPatients();
    EXPECT_EQ(treated.size(), 1);
    EXPECT_EQ(treated[0], "Jane Doe");
    EXPECT_EQ(list.getWaitingPatients().size(), 0);
    EXPECT_EQ(list.getTreatedPatients().size(), 1);
}

TEST(OrganTransplantTest, DeleteOldRecords_RemovesOldPatients) {
    OrganTransplantWaitingList list;
    
    list.addPatient("Old Patient");
    list.treatPatient("Old Patient", Date(2020, 6, 10));
    
    list.addPatient("New Patient");
    list.treatPatient("New Patient", Date(2025, 1, 15));
    
    EXPECT_EQ(list.getTreatedPatients().size(), 2);
    
    list.deleteOldRecords(Date(2021, 1, 1));
    
    EXPECT_EQ(list.getTreatedPatients().size(), 1);
}

TEST(OrganTransplantTest, GetPatientStatus_ReturnsCorrectStatus) {
    OrganTransplantWaitingList list;
    
    list.addPatient("Waiting Patient");
    list.addPatient("Will Be Treated");
    list.treatPatient("Will Be Treated", Date(2025, 1, 15));

    auto waitingStatus = list.getPatientStatus("Waiting Patient");
    ASSERT_TRUE(waitingStatus.has_value());
    EXPECT_EQ(*waitingStatus, PatientStatus::Waiting);

    auto treatedStatus = list.getPatientStatus("Will Be Treated");
    ASSERT_TRUE(treatedStatus.has_value());
    EXPECT_EQ(*treatedStatus, PatientStatus::Treated);

    auto missingStatus = list.getPatientStatus("Non Existent");
    ASSERT_FALSE(missingStatus.has_value());
    EXPECT_EQ(missingStatus.error(), PatientError::NotFound);
}


// Thread safety tests
TEST(OrganTransplantTest, ConcurrentAdd_MultipleThreads) {
    OrganTransplantWaitingList list;
    const int NUM_THREADS = 10;
    const int ADDS_PER_THREAD = 100;
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([&list, i]() {
            for (int j = 0; j < ADDS_PER_THREAD; j++) {
                list.addPatient("Thread" + std::to_string(i) + 
                              "-Patient" + std::to_string(j));
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(list.getWaitingPatients().size(), NUM_THREADS * ADDS_PER_THREAD);
}

TEST(OrganTransplantTest, ConcurrentAddAndDelete_NoDataRace) {
    OrganTransplantWaitingList list;
    
    std::thread addThread([&list]() {
        for (int i = 0; i < 100; i++) {
            list.addPatient("Patient-" + std::to_string(i));
        }
    });
    
    std::thread deleteThread([&list]() {
        for (int i = 0; i < 50; i++) {
            list.deleteOldRecords(Date(2020, 1, 1));
        }
    });
    
    addThread.join();
    deleteThread.join();
    
    // If we reach here without crash, thread safety works
    EXPECT_EQ(list.getWaitingPatients().size(), 100);
}

TEST(OrganTransplantTest, ConcurrentReadWrite_NoDataRace) {
    OrganTransplantWaitingList list;
    
    // Pre-populate
    for (int i = 0; i < 50; i++) {
        list.addPatient("Patient-" + std::to_string(i));
    }
    
    std::thread writeThread([&list]() {
        for (int i = 50; i < 100; i++) {
            list.addPatient("Patient-" + std::to_string(i));
        }
    });
    
    std::thread readThread([&list]() {
        for (int i = 0; i < 100; i++) {
            auto patients = list.getWaitingPatients();
            // Just reading, checking we don't crash
        }
    });
    
    writeThread.join();
    readThread.join();
    
    EXPECT_EQ(list.getWaitingPatients().size(), 100);
}