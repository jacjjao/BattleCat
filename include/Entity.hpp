#pragma once

#include "Enemy.hpp"
#include "Cat.hpp"
#include "Util/Image.hpp"
#include <cassert>

template<typename EntityType>
class Entity {
public:
    static Entity<CatType>
        CreateCat(const EntityStats& stats, CatType type, const CatStatsMat& mat,
            int level,
            std::function<void(const Entity<CatType>&)> hit_callback) {
        assert(1 <= level && level < 10);
        Entity<CatType> cat;
        cat.m_Type = type;
        cat.m_IsEnemy = false;
        cat.m_Stats = stats;
        cat.m_Stats.damage = mat.damage[level];
        cat.m_Stats.health = mat.health[level];
        cat.m_DamageCallback = hit_callback;
        return cat;
    }

    static Entity<EnemyType>
        CreateEnemy(const EntityStats& stats, EnemyType type, float stats_modifier,
            std::function<void(const Entity<EnemyType>&)> hit_callback) {
        Entity<CatType> enemy;
        enemy.m_Type = type;
        enemy.m_IsEnemy = false;
        enemy.m_Stats = stats;
        enemy.m_Stats.damage *= stats_modifier;
        enemy.m_Stats.health *= stats_modifier;
        enemy.m_DamageCallback = hit_callback;
        return enemy;
    }

    void StartAttack() { 
        m_State = EntityState::ON_ATTACK;
    }

    void GetHit(int damage, std::optional<EnemyAttr> attr) {
        m_Stats.health -= GetDamage();
    }

    int GetDamage(int damage, std::optional<EnemyAttr> attr) const { 
        if constexpr (std::is_same_v<EntityType, CatType>) {
            return GetDamageCatImpl(damage)
        } else {
            return damage;
        }
    }

    std::optional<EnemyAttr> GetEnemyAttr() const {
        return m_Stats.attr;
    }

    EntityType GetEntityType() const {
        return m_Type;
    }

    HitBox GetHitBox() const {
        return {};
    }

    void Draw(Util::Image& image) const {

    }

private:
    void GetDamageCatImpl(int damage, EnemyAttr attr) {
        assert(attr != EnemyAttr::NIL);
    }

    EntityState m_State = EntityState::WALK;
    EntityStats m_Stats;
    EntityType m_Type;
    bool m_IsEnemy = false;
    std::function<void(const Entity<EntityType> &)> m_DamageCallback;
};
