#ifndef BATTLECAT_EQUIPLIST_HPP
#define BATTLECAT_EQUIPLIST_HPP

#include "UnitCard.hpp"
#define MAXEQUIP 10

class EquipList{
friend class EquipScene;
public:
    EquipList() = delete;

    static std::vector<std::shared_ptr<UnitCard>> GetEquipList(){
        return m_equiplist;
    };

private:
    static inline std::vector<std::shared_ptr<UnitCard>> m_equiplist = []()->std::vector<std::shared_ptr<UnitCard>>{
        std::vector<std::shared_ptr<UnitCard>> eq_list;
        eq_list.reserve(MAXEQUIP);
        return eq_list;
    }();
};



#endif // BATTLECAT_EQUIPLIST_HPP
