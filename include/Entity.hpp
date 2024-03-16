#pragma once

#include "EntityStats.hpp"
#include "Util/Image.hpp"
#include "AnimatedGameObject.hpp"
#include "Timer.hpp"
#include <cassert>

class Entity {
public:
    void GetHit(int damage, std::optional<EnemyAttr> attr);

    HitBox GetHitBox() const; // when attacking

    virtual void Draw(Util::Image &image) const = 0;

    EntityState GetState() const;

    bool IsInRange(const Entity &e) const;

    float GetPosX() const;

    bool IsSingleTarget() const;

    virtual void DealDamage(Entity &e) = 0;

    bool IsDead() const;

#ifdef ENABLE_BATTLE_LOG
    const std::string_view GetName() const {
        return m_Stats.name;
    }
#endif // ENABLE_BATTLE_LOG

    static constexpr float s_KnockbackSpeed = 50.0f;
    static constexpr double s_KnockbackDuration = 1.0;

protected:
    void SetStats(const EntityStats &stats);
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

    std::shared_ptr<AnimatedGameObject> m_attack;
    std::shared_ptr<AnimatedGameObject> m_walk;
    std::shared_ptr<AnimatedGameObject> m_idle;
    std::shared_ptr<AnimatedGameObject> m_hitback;

private:
    void ExitKnockbackState();

    EntityState m_State = EntityState::WALK;
};
