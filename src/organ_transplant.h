#pragma once

#include <string>
#include <vector>
#include <mutex>

namespace ass1 {

struct Date {
    int year;
    int month;
    int day;
    
    Date(int y, int m, int d);
    bool operator<(const Date& other) const;
};

struct WaitingPatient {
    std::string fullName;
    WaitingPatient(const std::string& name);
};

struct TreatedPatient {
    std::string fullName;
    Date treatmentDate;
    TreatedPatient(const std::string& name, const Date& date);
};

class OrganTransplantWaitingList {
private:
    std::vector<WaitingPatient> waitingList;
    std::vector<TreatedPatient> treatedList;
    mutable std::mutex mtx; // not sure if mutable is needed here

public:
    OrganTransplantWaitingList() = default;
    
    // Delete copy operations
    OrganTransplantWaitingList(const OrganTransplantWaitingList&) = delete;
    OrganTransplantWaitingList& operator=(const OrganTransplantWaitingList&) = delete;
    
    void addPatient(const std::string& name);
    void treatPatient(const std::string& name, const Date& treatmentDate);
    void deleteOldRecords(const Date& beforeDate);
    std::vector<std::string> getWaitingPatients() const;
    std::vector<std::string> getTreatedPatients() const;
    int getPatientStatus(const std::string& name) const;
};

}