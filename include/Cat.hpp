#ifndef CAT_HPP
#define CAT_HPP

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include <array>

enum class CatType : size_t {
    CAT_TOWER = 0,
    CAT,
    TANK_CAT,
    AXE_CAT,
    CRAZED_GROSS_CAT,
    CAT_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<CatType>, size_t>);

class Cat : public Entity {
public:
    friend class CatFactory;

    Cat(CatType type, int level);

    void StartAttack();

    void Draw(Util::Transform trans, Util::Image &image) const;

    void UpdateTimer(double dt);

    void Walk(double dt);

    void DealDamage(Entity &e) override;

    CatType GetCatType() const;

    bool OnAttack();

    void ResetState();

    void Move(float dx);

private:
    void Attack();
    void CoolDownComplete();

    HitBox ToWorldSpace(HitBox hitbox) const override;

    CatType m_Type;
    bool m_OnAttack = false;
};

namespace BaseCatStats {

    inline const EntityStats CatTower = [] {
        EntityStats stats;
        stats.health = 250;
        stats.kb = 1;
        stats.single_target = true;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "CatTower";
#endif
        return stats;
    }();

    inline const EntityStats Cat = [] {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 30;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 100;
        stats.det_box = {0, 1};
        stats.hit_box = {0, 1};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Cat";
#endif
        return stats;
    }();

    inline const EntityStats TankCat = [] {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 30;
        stats.single_target = false;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 200;
        stats.det_box = {0, 10};
        stats.hit_box = {0, 10};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "TankCat";
#endif
        return stats;
    }();

    inline const EntityStats AxeCat = [] {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 100;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 30;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 300;
        stats.det_box = {0, 140};
        stats.hit_box = {0, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
        stats.strong = {EnemyAttr::RED};
#ifdef ENABLE_BATTLE_LOG
        stats.name = "AxeCat";
#endif
        return stats;
    }();

    inline const EntityStats CrazedGrossCat = [] {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 30;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 600;
        stats.det_box = {0, 140};
        stats.hit_box = {0, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "CrazedGrossCat";
#endif
        return stats;
    }();

    inline const std::array<EntityStats,
                            static_cast<size_t>(CatType::CAT_TYPE_COUNT)>
        Stats = {BaseCatStats::CatTower, BaseCatStats::Cat,
                 BaseCatStats::TankCat, BaseCatStats::AxeCat,
                 BaseCatStats::CrazedGrossCat};

} // BaseCatStats

#endif // CAT_HPP