#pragma once

#include "EntityStats.hpp"
#include "Util/Image.hpp"
#include <cassert>

class Entity {
public:
    virtual ~Entity() = default;

    void StartAttack();
    
    void SetStats(const EntityStats& stats);

    virtual void GetHit(int damage, std::optional<EnemyAttr> attr);

    virtual int GetDamage(int damage, std::optional<EnemyAttr> attr) const;

    std::optional<EnemyAttr> GetEnemyAttr() const;

    HitBox GetHitBox() const; // when attacking

    HitBox GetHurtBox() const; // when being attacked

    virtual void Draw(Util::Image &image) const = 0;

    EntityState GetState() const;

    bool CanAttack(const Entity &e) const;

    float GetPosX() const;

    float SetPosX(float x);

protected:
    virtual HitBox ToWorldSpace(HitBox hitbox) const = 0;

    EntityState m_State = EntityState::WALK;
    EntityStats m_Stats;
    float m_PosX = 0;
    bool m_IsEnemy = false;
};
