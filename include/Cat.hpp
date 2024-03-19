#ifndef CAT_HPP
#define CAT_HPP

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include <array>

enum class CatType : size_t {
    CAT = 0,
    CAT_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<CatType>, size_t>);

class Cat : public Entity {
public:
    Cat(CatType type, float pos, std::function<void(Cat&)> atk_callback);

    void StartAttack();

    void Draw(Util::Image &image) const override;

    void Update();

    void Walk(float dt);

    void DealDamage(Entity &e) override;

    CatType GetCatType() const;

    Cat(Cat &&other) noexcept;
    Cat &operator=(Cat &&other) noexcept;

private:
    void SetCallbacks();
    void Attack();
    void CoolDownComplete();

    HitBox ToWorldSpace(HitBox hitbox) const override;

    const std::function<void(Cat &)> m_AtkCallback;
    CatType m_Type;
};

namespace CatStats {

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
        stats.cost = 75;
        stats.det_box = {0, 10};
        stats.hit_box = {0, 10};
        stats.attr = std::nullopt;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Cat";
#endif
        return stats;
    }();

    inline const std::array<EntityStats,
                            static_cast<size_t>(CatType::CAT_TYPE_COUNT)>
        Stats = {CatStats::Cat};

} // CatStats
#endif //CAT_HPP
