#ifndef BATTLECAT_CATLIST_HPP
#define BATTLECAT_CATLIST_HPP

#include "UnitCard.hpp"
#include "App.hpp"

class CatList{
private:
    static std::vector<std::shared_ptr<UnitCard>> Init(){
        std::vector<std::shared_ptr<UnitCard>> catlist;
        catlist.reserve(MAXUNITS);
        for(unsigned int i=0;i<MAXUNITS;i++){
            auto &unit = catlist.emplace_back
                         (std::make_shared<UnitCard>(i,1.9f));
            //m_Root. AddChild(unit);
        }
        return catlist;
    }
    static inline std::vector<std::shared_ptr<UnitCard>> m_catlist = Init();
};
#endif // BATTLECAT_CATLIST_HPP
