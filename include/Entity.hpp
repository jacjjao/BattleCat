#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "EntityStats.hpp"
#include "Util/Image.hpp"
#include "AnimatedGameObject.hpp"
#include "Timer.hpp"
#include <cassert>

class Entity {
public:
    virtual ~Entity() = default;

    void GetHit(int damage, const Entity &attacker);

    HitBox GetHitBox() const; // when attacking

    virtual void Draw(Util::Image &image) const = 0;

    void SetState(EntityState state);
    EntityState GetState() const;

    bool IsInRange(const Entity &e) const;

    float GetPosX() const;

    void SetPosX(float pos);

    bool IsSingleTarget() const;

    virtual void DealDamage(Entity &e) = 0;

    bool IsDead() const;

    std::optional<EnemyAttr> GetAttr() const;

    double GetHealthPercent() const;

#ifdef ENABLE_BATTLE_LOG
    const std::string_view GetName() const {
        return m_Stats.name;
    }
#endif // ENABLE_BATTLE_LOG

    static constexpr float s_KnockbackSpeed = 50.0f;
    static constexpr double s_KnockbackDuration = 1.0;

protected:
    void SetStats(const EntityStats &stats);
    virtual HitBox ToWorldSpace(HitBox hitbox) const = 0;

    EntityStats m_Stats;
    float m_PosX = 0;
    bool m_IsEnemy = false;
    Timer m_AtkPrepTimer;
    Timer m_AtkCoolDownTimer;
    Timer m_KnockbackTimer;
    int m_KnockBackHealth = 0;
    int m_TotalDamage = 0;
    double m_Health = 0;

    std::shared_ptr<AnimatedGameObject> m_attack;
    std::shared_ptr<AnimatedGameObject> m_walk;
    std::shared_ptr<AnimatedGameObject> m_idle;
    std::shared_ptr<AnimatedGameObject> m_hitback;

private:
    EntityState m_State = EntityState::WALK;
};
#endif // ENTITY_HPP
