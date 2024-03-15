#pragma once

#include "EntityStats.hpp"
#include "Util/Image.hpp"
#include "Timer.hpp"
#include <cassert>

class Entity {
public:
    virtual ~Entity() = default;
    
    void SetStats(const EntityStats& stats);

    void GetHit(int damage, std::optional<EnemyAttr> attr);

    HitBox GetHitBox() const; // when attacking

    virtual void Draw(Util::Image &image) const = 0;

    EntityState GetState() const;

    bool CanAttack(const Entity &e) const;

    float GetPosX() const;

    float SetPosX(float x);

    bool IsSingleTarget() const;

    virtual void DealDamage(Entity &e) = 0;

    bool IsDead() const;

#ifdef ENABLE_BATTLE_LOG
    const std::string& GetName() const {
        return m_Stats.name;
    }
#endif // ENABLE_BATTLE_LOG

    static constexpr float s_KnockbackSpeed = 50.0f;
    static constexpr double s_KnockbackDuration = 1.0;

protected:
    void SetState(EntityState state);
    void OnUpdate();
    virtual HitBox ToWorldSpace(HitBox hitbox) const = 0;

    EntityStats m_Stats;
    float m_PosX = 0;
    bool m_IsEnemy = false;
    Timer m_AtkPrepTimer;
    Timer m_AtkCoolDownTimer;
    Timer m_KnockbackTimer;
    int m_KnockBackHealth = 0;
    int m_TotalDamage = 0;

private:
    void ExitKnockbackState();

    EntityState m_State = EntityState::WALK;
};
