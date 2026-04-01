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


TEST(OrganTransplantTest, ThreadSafety_EverythingElse) {
    OrganTransplantWaitingList list;
    
    list.addPatient("Initial1");
    list.addPatient("Initial2");
    list.treatPatient("Initial1", Date(2025, 1, 15));

    auto threadFunc = [&list]() {
        for (int i = 0; i < 500; i++) {
            std::string name = "Patient_" + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + "_" + std::to_string(i);
            
            list.addPatient(name);
            list.getWaitingPatients();
            list.getPatientStatus(name);
            
            list.treatPatient(name, Date(2026, 6, 1));
            
            list.getTreatedPatients();
            list.deleteOldRecords(Date(2026, 1, 1));
        }
    };
    
    std::thread t1(threadFunc);
    std::thread t2(threadFunc);
    
    t1.join();
    t2.join();
    
    SUCCEED();
}

TEST(OrganTransplantTest, ThreadSafety_MoveAndCopy) {
    static_assert(!std::is_copy_constructible_v<OrganTransplantWaitingList>, "Copy constructor must be deleted");
    static_assert(!std::is_copy_assignable_v<OrganTransplantWaitingList>, "Copy assignment must be deleted");

    OrganTransplantWaitingList shared_list;
    shared_list.addPatient("PatientBase");

    auto threadFunc = [&shared_list]() {
        for (int i = 0; i < 500; i++) {
            OrganTransplantWaitingList local_list = std::move(shared_list);
            
            std::string name = "Patient_" + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + "_" + std::to_string(i);
            local_list.addPatient(name);
            
            shared_list = std::move(local_list);
        }
    };

    std::thread t1(threadFunc);
    std::thread t2(threadFunc);

    t1.join();
    t2.join();

    SUCCEED();
}