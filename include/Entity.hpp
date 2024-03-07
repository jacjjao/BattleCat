#pragma once

#include "Enemy.hpp"
#include "Cat.hpp"
#include "Util/Image.hpp"

template<typename EntityType>
class Entity {
public:
    static Entity<CatType>
    CreateCat(const EntityStats &stats, CatType type, const CatStatsMat &mat,
              int level,
              std::function<void(const Entity<CatType> &)> hit_callback);

    static Entity<EnemyType>
    CreateEnemy(const EntityStats &stats, EnemyType type, float stats_modifier,
                std::function<void(const Entity<EnemyType> &)> hit_callback);

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
