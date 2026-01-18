#include "organ_transplant.h"
#include <algorithm>
#include <expected>

namespace ass1 {

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

WaitingPatient::WaitingPatient(std::string name) : fullName(std::move(name)) {}

TreatedPatient::TreatedPatient(const std::string& name, const Date& date) 
    : fullName(name), treatmentDate(date) {}

void OrganTransplantWaitingList::addPatient(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);
    waitingList.emplace_back(name);
}

void OrganTransplantWaitingList::treatPatient(const std::string& name, const Date& treatmentDate) {
    std::lock_guard<std::mutex> lock(mtx);
    
    auto it = std::find_if(waitingList.begin(), waitingList.end(),
        [&name](const WaitingPatient& p) { return p.fullName == name; });
    
    if (it != waitingList.end()) {
        treatedList.emplace_back(name, treatmentDate);
        waitingList.erase(it);
    }
}
//note to self, since c++ 20 we can use std::erase_if
void OrganTransplantWaitingList::deleteOldRecords(const Date& beforeDate) {
    std::lock_guard<std::mutex> lock(mtx);
    
    treatedList.erase(
        std::remove_if(treatedList.begin(), treatedList.end(),
            [&beforeDate](const TreatedPatient& tp) {
                return tp.treatmentDate < beforeDate;
            }),
        treatedList.end()
    );
}

std::vector<std::string> OrganTransplantWaitingList::getWaitingPatients() const {
    std::lock_guard<std::mutex> lock(mtx);
    
    std::vector<std::string> names;
    for (const auto& patient : waitingList) {
        names.emplace_back(patient.fullName);
    }
    return names;
}

std::vector<std::string> OrganTransplantWaitingList::getTreatedPatients() const {
    std::lock_guard<std::mutex> lock(mtx);
    
    std::vector<std::string> names;
    for (const auto& patient : treatedList) {
        names.emplace_back(patient.fullName);
    }
    return names;
}

std::expected<PatientStatus, PatientError>OrganTransplantWaitingList::getPatientStatus(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mtx);

    auto waitIt = std::find_if(
        waitingList.begin(), waitingList.end(),
        [&name](const WaitingPatient& p) { return p.fullName == name; }
    );
    if (waitIt != waitingList.end()) {
        return PatientStatus::Waiting;
    }

    auto treatIt = std::find_if(
        treatedList.begin(), treatedList.end(),
        [&name](const TreatedPatient& tp) { return tp.fullName == name; }
    );
    if (treatIt != treatedList.end()) {
        return PatientStatus::Treated;
    }

    return std::unexpected(PatientError::NotFound);
}


}