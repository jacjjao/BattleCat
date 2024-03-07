#pragma once

#include "Enemy.hpp"
#include "Cat.hpp"
#include "Util/Image.hpp"

enum class EntityType : size_t {
    CAT = 0,
    DOGE,
    TYPE_COUNT
};

class Entity {
public:
    static Entity CreateCat(const EntityStats &stats, EntityType type,
                            const CatStatsMat &mat, int level,
                            std::function<void(const Cat &)> hit_callback);

    static Entity CreateEnemy(const EntityStats &stats, EntityType type,
                              float stats_modifier,
                              std::function<void(const Enemy &)> hit_callback);

    void StartAttack();

    void GetHit(int damage, std::optional<EnemyAttr> attr);

    int GetDamage(std::optional<EnemyAttr> attr) const;

    std::optional<EnemyAttr> GetEnemyAttr() const;

    EntityType GetEntityType() const;

    HitBox GetHitBox() const;

    void Draw(Util::Image &image) const;

private:
    EntityState m_State = EntityState::WALK;
    EntityStats m_Stats;
    bool is_enemy = false;
};
